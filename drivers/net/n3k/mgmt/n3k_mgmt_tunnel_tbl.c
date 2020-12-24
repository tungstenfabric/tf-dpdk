/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_init/n3k_mgmt_init_port.h"

#include "n3k_mgmt_bitwise.h"
#include "n3k_mgmt_conversions.h"
#include "n3k_mgmt_dump.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_lock.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_regs_ctrl.h"
#include "n3k_mgmt_regs.h"
#include "n3k_mgmt_tunnel_tbl_regs_api.h"
#include "n3k_mgmt_tunnel_tbl.h"

#include <errno.h>
#include <rte_byteorder.h>
#include <rte_debug.h>
#include <stdint.h>

static void fill_tunnel_regs(struct tunnel_tbl_data_regs *regs,
	const struct n3k_mgmt_tunnel_tbl_entry *entry)
{
	uint64_t dst_mac = 0;

	memset(regs, 0, sizeof(*regs));

	// L2 tunnel regs
	dst_mac = n3k_mgmt_convert_rte_ether_addr_to_uint(entry->l2.dst_mac);
	regs->data_8.field.dest_mac_0 = BITS_RANGE64(dst_mac, 0, 31);
	regs->data_9.field.dest_mac_1 = BITS_RANGE64(dst_mac, 31, 17);
	regs->data_9.field.physical_port_id = entry->l2.src_phy_port_id;
	regs->data_12.field.encap_eth_type = entry->l2.eth_type;
	// TODO(n3k): fill if vlan tags needed
	regs->data_11.field.encap_tci_0 = 0;
	regs->data_11.field.encap_tci_1 = 0;
	regs->data_12.field.encap_tci_1 = 0;

	// L3 tunnel regs
	regs->data_0.field.ip_type = 0; // IPv4
	regs->data_10.field.encap_ttl = entry->l3.ttl;
	regs->data_0.field.encap_ip_sa_0 = BITS_RANGE32(entry->l3.src_ip, 0, 31);
	regs->data_1.field.encap_ip_sa_1 = BITS_RANGE32(entry->l3.src_ip, 31, 1);
	regs->data_4.field.encap_ip_da_0 = BITS_RANGE32(entry->l3.dst_ip, 0, 31);
	regs->data_5.field.encap_ip_da_1 = BITS_RANGE32(entry->l3.dst_ip, 31, 1);
	regs->data_10.field.tos_0 = BITS_RANGE32(entry->l3.tos, 0, 3);
	regs->data_11.field.tos_1 = BITS_RANGE32(entry->l3.tos, 3, 5);
	regs->data_11.field.encap_ip_prot = entry->l3.ip_prot;

	// L4 tunnel regs
	regs->data_13.field.encap_l4_dst_port = entry->l4.dst_port;

	// Tunnel specific regs
	switch (entry->type) {
	case N3K_MGMT_TUNNEL_TYPE_MPLSOUDP:
		regs->data_14.field.mpls_oudpen_eth_hdr = 1;
		regs->data_14.field.mpls_oudpen_ip_hdr = 1;
		break;
	case N3K_MGMT_TUNNEL_TYPE_MPLSOGRE:
		regs->data_14.field.mpls_ogre_en_eth_hdr = 1;
		regs->data_14.field.mpls_ogre_en_ip_hdr = 1;
		// TODO(n3k): implement
		break;
	case N3K_MGMT_TUNNEL_TYPE_VXLAN:
		regs->data_14.field.encap_vxlan_en = 1;
		regs->data_9.field.encap_vxlan_vni_0 = BITS_RANGE32(entry->vxlan.vni, 0, 11);
		regs->data_10.field.encap_vxlan_vni_1 = BITS_RANGE32(entry->vxlan.vni, 11, 13);
		regs->data_12.field.encap_vxlan_tci_0_en = 0;
		regs->data_12.field.encap_vxlan_tci_1_en = 0;
		const uint32_t vxlan_flags = rte_cpu_to_be_16(0x0800);
		regs->data_13.field.encap_vxlan_flags_0 = BITS_RANGE32(vxlan_flags, 0, 1);
		regs->data_14.field.encap_vxlan_flags_1 = BITS_RANGE32(vxlan_flags, 1, 7);
		break;
	}
}

static void fill_tunnel_entry(struct n3k_mgmt_tunnel_tbl_entry *entry,
	const struct tunnel_tbl_data_regs *regs)
{
	uint64_t dst_mac = 0;

	memset(entry, 0, sizeof(*entry));

	// L2 tunnel info
	dst_mac = regs->data_8.field.dest_mac_0 |
		  ((uint64_t)regs->data_9.field.dest_mac_1 << 31);
	entry->l2.dst_mac = n3k_mgmt_convert_uint_to_rte_ether_addr(dst_mac);
	entry->l2.src_phy_port_id = regs->data_9.field.physical_port_id;
	entry->l2.eth_type = regs->data_12.field.encap_eth_type;

	// L3 tunnel info
	entry->l3.src_ip = regs->data_0.field.encap_ip_sa_0 |
			   (regs->data_1.field.encap_ip_sa_1 << 31);
	entry->l3.dst_ip = regs->data_4.field.encap_ip_da_0 |
			   (regs->data_5.field.encap_ip_da_1 << 31);
	entry->l3.ttl = regs->data_10.field.encap_ttl;
	entry->l3.tos = regs->data_10.field.tos_0 |
			(regs->data_11.field.tos_1 << 3);
	entry->l3.ip_prot = regs->data_11.field.encap_ip_prot;

	// L4 tunnel info
	entry->l4.dst_port = regs->data_13.field.encap_l4_dst_port;

	// Tunnel specific info
	if (regs->data_14.field.encap_vxlan_en) {
		entry->type = N3K_MGMT_TUNNEL_TYPE_VXLAN;
		entry->vxlan.vni =
			regs->data_9.field.encap_vxlan_vni_0 |
			(regs->data_10.field.encap_vxlan_vni_1 << 11);
	} else if (regs->data_14.field.mpls_oudpen_eth_hdr) {
		entry->type = N3K_MGMT_TUNNEL_TYPE_MPLSOUDP;
	} else if (regs->data_14.field.ip_ogre_en_ip_gre_hdr) {
		// TODO(n3k): implement
	} else if (regs->data_14.field.mpls_ogre_en_eth_hdr) {
		entry->type = N3K_MGMT_TUNNEL_TYPE_MPLSOGRE;
		// TODO(n3k): implement
	}
}

static int exec_tunnel_tbl_cmd(struct n3k_mgmt_hw *hw, uint16_t tunnel_idx,
	enum MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_CMD_OP_TYPE type)
{
	MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_CMD_ADDR_t tunnel_tbl_cmd = {{0}};
	tunnel_tbl_cmd.field.tunnel_tbl_addr = tunnel_idx;
	tunnel_tbl_cmd.field.op_type = type;
	csr_write(hw, MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE,
		MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_CMD_ADDR,
		tunnel_tbl_cmd.val);
	return n3k_mgmt_poll_ctrl_reg(hw, N3K_MGMT_CTRL_REG_TUNNEL_TBL_STS);
}

int n3k_mgmt_tunnel_tbl_set(struct n3k_mgmt_hw *hw, uint16_t tunnel_idx,
	const struct n3k_mgmt_tunnel_tbl_entry *entry)
{
	struct tunnel_tbl_data_regs tunnel_regs;
	int ret;

	if (tunnel_idx >= (1 << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_CMD_ADDR_TUNNEL_TBL_ADDR_WIDTH)) {
		N3K_MGMT_TUNNEL_LOG(ERR, "Invalid tunnel_id=%" PRIu32, tunnel_idx);
		return -EINVAL;
	}

	fill_tunnel_regs(&tunnel_regs, entry);

	if (N3K_MGMT_REGS_SHOULD_LOG(DEBUG)) {
		N3K_MGMT_REGS_LOG(DEBUG, "Adding tunnel: tunnel_id=%" PRIu16 " regs", tunnel_idx);
		n3k_mgmt_dump_reg_tbl(N3K_MGMT_REG_TBL_TUNNEL_TBL_DATA, &tunnel_regs);
	}

	if (N3K_MGMT_TUNNEL_SHOULD_LOG(DEBUG)) {
		N3K_MGMT_TUNNEL_LOG(DEBUG, "Adding tunnel: tunnel_id=%" PRIu16, tunnel_idx);
		n3k_mgmt_tunnel_tbl_entry_dump(entry);
	}

	n3k_mgmt_lock(hw);

	RTE_ASSERT(n3k_mgmt_ctrl_reg_is_busy(hw, N3K_MGMT_CTRL_REG_TUNNEL_TBL_STS) == 0 &&
		"Another read/write operation on tunnel table is in progress.");

	n3k_mgmt_program_reg_tbl(hw, N3K_MGMT_REG_TBL_TUNNEL_TBL_DATA, &tunnel_regs);

	ret = exec_tunnel_tbl_cmd(hw, tunnel_idx,
		MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_CMD_WRITE);
	if (ret < 0) {
		N3K_MGMT_TUNNEL_LOG(ERR,
			"Error occurred while waiting to complete tunnel entry add operation: "
			"tunnel_id=%" PRIu16 ", ret=%d",
			tunnel_idx, ret);
		goto release_lock;
	}

	N3K_MGMT_TUNNEL_LOG(INFO,
		"New tunnel added successfully: tunnel_id=%" PRIu16, tunnel_idx);

release_lock:
	n3k_mgmt_unlock(hw);
	return ret;
}

int n3k_mgmt_tunnel_tbl_get(struct n3k_mgmt_hw *hw, uint16_t tunnel_idx,
	struct n3k_mgmt_tunnel_tbl_entry *entry)
{
	struct tunnel_tbl_data_regs tunnel_regs;
	int ret;

	if (tunnel_idx >= (1 << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_CMD_ADDR_TUNNEL_TBL_ADDR_WIDTH)) {
		N3K_MGMT_TUNNEL_LOG(ERR, "Invalid tunnel_id=%" PRIu32, tunnel_idx);
		return -EINVAL;
	}

	n3k_mgmt_lock(hw);

	RTE_ASSERT(n3k_mgmt_ctrl_reg_is_busy(hw, N3K_MGMT_CTRL_REG_TUNNEL_TBL_STS) == 0 &&
		"Another read/write operation on tunnel table is in progress.");

	ret = exec_tunnel_tbl_cmd(hw, tunnel_idx,
		MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_CMD_READ);
	if (ret) {
		N3K_MGMT_TUNNEL_LOG(ERR,
			"Error occurred while waiting to complete tunnel entry read operation: "
			"tunnel_id=%" PRIu16 ", ret=%d", tunnel_idx, ret);
		goto release_lock;
	}

	n3k_mgmt_read_reg_tbl(hw, N3K_MGMT_REG_TBL_TUNNEL_TBL_DATA, &tunnel_regs);
	fill_tunnel_entry(entry, &tunnel_regs);

	if (N3K_MGMT_TUNNEL_SHOULD_LOG(DEBUG)) {
		N3K_MGMT_TUNNEL_LOG(DEBUG, "Read tunnel: tunnel_id=%" PRIu32, tunnel_idx);
		n3k_mgmt_tunnel_tbl_entry_dump(entry);
	}

release_lock:
	n3k_mgmt_unlock(hw);
	return ret;
}

int n3k_mgmt_tunnel_tbl_dump(struct n3k_mgmt_hw *hw, uint16_t tunnel_idx)
{
	struct n3k_mgmt_tunnel_tbl_entry entry;
	int ret = n3k_mgmt_tunnel_tbl_get(hw, tunnel_idx, &entry);
	if (ret < 0)
		return ret;
	n3k_mgmt_tunnel_tbl_entry_dump(&entry);
	return 0;
}
