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
#include "n3k_mgmt_flow_tbl.h"
#include "n3k_mgmt_flow_tbl_handles.h"
#include "n3k_mgmt_flow_tracker.h"
#include "n3k_mgmt_handle_mapping.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_lock.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_ports.h"
#include "n3k_mgmt_regs_ctrl.h"
#include "n3k_mgmt_flow_id_allocator.h"
#include "n3k_mgmt_recirc_id_allocator.h"
#include "regs/n3k_mgmt_flow_tbl_regs_api.h"
#include "regs/n3k_mgmt_regs.h"

#include <rte_pmd_n3k_malloc.h>
#include <rte_pmd_n3k_flow_tbl_mgmt.h>

#include <errno.h>

enum n3k_mgmt_pars_tunnel_type {
	N3K_MGMT_PARS_TUNNEL_TYPE_NONE = 0,
	N3K_MGMT_PARS_TUNNEL_TYPE_VXLAN = 1,
	N3K_MGMT_PARS_TUNNEL_TYPE_MPLSOUDP = 6,
	N3K_MGMT_PARS_TUNNEL_TYPE_MPLSOGRE = 7,
};

static void fill_decap_key_regs(struct lookup_emlc_key_regs *key_regs,
	const struct n3k_mgmt_flow_tbl_key_tunnel *tunnel)
{
	enum n3k_mgmt_pars_tunnel_type parse_tunnel_type = N3K_MGMT_PARS_TUNNEL_TYPE_NONE;
	uint32_t vxlnid_teid_key = 0;

	// Outer L3 key regs
	// TODO(n3k): add ipv6
	key_regs->key_0.field.tunnel_dst_ip = tunnel->outer_l3.ipv4.dst_ip;
	key_regs->key_1.field.tunnel_src_ip = tunnel->outer_l3.ipv4.src_ip;
	key_regs->key_2.field.tunnel_ip_protocol = tunnel->outer_l3.ip_prot;

	// MPLS/VXLAN key regs
	switch (tunnel->type) {
	case N3K_MGMT_TUNNEL_TYPE_MPLSOUDP:
		parse_tunnel_type = N3K_MGMT_PARS_TUNNEL_TYPE_MPLSOUDP;
		vxlnid_teid_key = BITS_RANGE32(tunnel->mpls_udp.label, 0, 20) |
				(BITS_RANGE32(tunnel->mpls_udp.tos, 0, 3) << 20) |
				(1 << 23) | // set S to 1
				(0xff << 24);
		break;
	case N3K_MGMT_TUNNEL_TYPE_MPLSOGRE:
		parse_tunnel_type = N3K_MGMT_PARS_TUNNEL_TYPE_MPLSOGRE;
		break;
	case N3K_MGMT_TUNNEL_TYPE_VXLAN:
		parse_tunnel_type = N3K_MGMT_PARS_TUNNEL_TYPE_VXLAN;
		vxlnid_teid_key = BITS_RANGE32(tunnel->vxlan.vni, 0, 24);
		break;
	}
	key_regs->key_13.field.parse_flag |= BITS_RANGE32(parse_tunnel_type, 0, 4) << 12;
	key_regs->key_3.field.vxlnid_teid_key_0 = BITS_RANGE32(vxlnid_teid_key, 0, 24);
	key_regs->key_4.field.vxlnid_teid_key_1 = BITS_RANGE32(vxlnid_teid_key, 24, 8);
}

int n3k_mgmt_flow_tbl_fill_key_regs(struct n3k_mgmt_hw *hw,
	struct lookup_emlc_key_regs *key_regs, const struct n3k_mgmt_flow_tbl_key *key)
{
	memset(key_regs, 0, sizeof(*key_regs));

	uint8_t vid;
	int ret = n3k_mgmt_dpdk_port_to_hw_vid(hw, key->port.port_id, &vid);
	if (ret != 0)
		return ret;

	key_regs->key_17.field.vid = vid;

	// L2 key regs
	if (key->use_l2_key) {
		uint64_t dst_mac = n3k_mgmt_convert_rte_ether_addr_to_uint(key->l2.dst_mac);
		uint64_t src_mac = n3k_mgmt_convert_rte_ether_addr_to_uint(key->l2.src_mac);
		key_regs->key_14.field.dst_mac_0 = BITS_RANGE64(dst_mac, 0, 32);
		key_regs->key_15.field.dst_mac_1 = BITS_RANGE64(dst_mac, 32, 16);
		key_regs->key_15.field.src_mac_0 = BITS_RANGE64(src_mac, 0, 16);
		key_regs->key_16.field.src_mac_1 = BITS_RANGE64(src_mac, 16, 32);

		key_regs->key_2.field.tci_vlan_a0 =
			BITS_RANGE64(key->l2.vlan_tci, 0, 8);
		key_regs->key_3.field.tci_vlan_a1 =
			BITS_RANGE64(key->l2.vlan_tci, 8, 8);
	}

	// L3 key regs
	key_regs->key_13.field.parse_flag = BITS_RANGE32(key->l3.type, 0, 4) << 6;

	switch (key->l3.type) {
		case N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4:
			key_regs->key_4.field.dst_ip_0 = BITS_RANGE32(key->l3.ipv4.dst_ip, 0, 24);
			key_regs->key_5.field.dst_ip_1 = BITS_RANGE32(key->l3.ipv4.dst_ip, 24, 8);
			key_regs->key_8.field.src_ip_0 = BITS_RANGE32(key->l3.ipv4.src_ip, 0, 24);
			key_regs->key_9.field.src_ip_1 = BITS_RANGE32(key->l3.ipv4.src_ip, 24, 8);
			break;

		case N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6:
			key_regs->key_8.field.dst_ip_4 = key->l3.ipv6.dst_ip[0];
			memcpy_bswap32(&key_regs->key_7.val, (const uint32_t *)&key->l3.ipv6.dst_ip[1], 4);
			memcpy_bswap32(&key_regs->key_6.val, (const uint32_t *)&key->l3.ipv6.dst_ip[5], 4);
			memcpy_bswap32(&key_regs->key_5.val, (const uint32_t *)&key->l3.ipv6.dst_ip[9], 4);
			key_regs->key_4.field.dst_ip_0 = (key->l3.ipv6.dst_ip[13] << 16) |
				(key->l3.ipv6.dst_ip[14] << 8) | key->l3.ipv6.dst_ip[15];

			key_regs->key_12.field.src_ip_4 = key->l3.ipv6.src_ip[0];
			memcpy_bswap32(&key_regs->key_11.val, (const uint32_t *)&key->l3.ipv6.src_ip[1], 4);
			memcpy_bswap32(&key_regs->key_10.val, (const uint32_t *)&key->l3.ipv6.src_ip[5], 4);
			memcpy_bswap32(&key_regs->key_9.val, (const uint32_t *)&key->l3.ipv6.src_ip[9], 4);
			key_regs->key_8.field.src_ip_0 = (key->l3.ipv6.src_ip[13] << 16) |
				(key->l3.ipv6.src_ip[14] << 8) | key->l3.ipv6.src_ip[15];
			break;

		default:
			N3K_MGMT_LOG(FLOW, ERR,
				"Can't recognize %d as a l3 ethertype",
				key->l3.type);
			break;
	}

	key_regs->key_13.field.ip_protocol = key->l3.ip_prot;

	// L4 key regs
	key_regs->key_12.field.dst_port = key->l4.dst_port;
	key_regs->key_12.field.src_port_0 = BITS_RANGE32(key->l4.src_port, 0, 8);
	key_regs->key_13.field.src_port_1 = BITS_RANGE32(key->l4.src_port, 8, 8);

	// Tunnel/Non-tunnel specific key regs
	if (key->use_tunnel_key)
		fill_decap_key_regs(key_regs, &key->tunnel);
	else
		key_regs->key_2.field.tunnel_ip_protocol = 0xff;

	return 0;
}

static void fill_decap_res_regs(struct lookup_emlc_res_regs *res_regs,
	const struct n3k_mgmt_flow_tbl_action_decap *decap_action)
{
	if (decap_action->insert_l2) {
		uint64_t dst_mac = n3k_mgmt_convert_rte_ether_addr_to_uint(
			decap_action->inserted_l2.dst_mac);
		uint64_t src_mac = n3k_mgmt_convert_rte_ether_addr_to_uint(
			decap_action->inserted_l2.src_mac);

		res_regs->res_19.field.insert_inner_l2 = 1;

		res_regs->res_0.field.mac_da_0 = BITS_RANGE64(dst_mac, 0, 32);
		res_regs->res_1.field.mac_da_1 = BITS_RANGE64(dst_mac, 32, 16);
		res_regs->res_1.field.mac_sa_0 = BITS_RANGE64(src_mac, 0, 16);
		res_regs->res_2.field.mac_sa_1 = BITS_RANGE64(src_mac, 16, 32);
	}
	switch (decap_action->type) {
	case N3K_MGMT_TUNNEL_TYPE_MPLSOUDP:
		res_regs->res_18.field.mpls_oudpde_eth_hdr = 1;
		res_regs->res_18.field.mpls_oudpde_ip_hdr = 1;
		break;
	case N3K_MGMT_TUNNEL_TYPE_MPLSOGRE:
		res_regs->res_18.field.mpls_ogre_de_eth_hdr = 1;
		res_regs->res_18.field.mpls_ogre_de_ip_hdr = 1;
		break;
	case N3K_MGMT_TUNNEL_TYPE_VXLAN:
		res_regs->res_18.field.decap_vxlan_en = 1;
		break;
	}
}

static void fill_encap_res_regs(struct lookup_emlc_res_regs *res_regs,
	const struct n3k_mgmt_flow_tbl_action_encap *encap_action)
{
	res_regs->res_17.field.dec_ttl_en = encap_action->decr_ttl;
	res_regs->res_19.field.strip_inner_l2 = encap_action->strip_l2;
	switch (encap_action->type) {
	case N3K_MGMT_TUNNEL_TYPE_MPLSOUDP:
		res_regs->res_11.field.emlc_res_11 =
			1 | // use tunnel table
			(BITS_RANGE32(encap_action->tunnel_idx, 0, 12) << 1) |
			(BITS_RANGE32(encap_action->mpls_udp.label, 0, 16) << 16);
		res_regs->res_12.field.emlc_res_12 =
			BITS_RANGE32(encap_action->mpls_udp.label, 16, 4) |
			(BITS_RANGE32(encap_action->mpls_udp.tos, 0, 3) << 4);
		res_regs->res_18.field.encap_l4_src_port_0 =
			BITS_RANGE32(encap_action->mpls_udp.src_port, 0, 9);
		res_regs->res_19.field.encap_l4_src_port_1 =
			BITS_RANGE32(encap_action->mpls_udp.src_port, 9, 7);
		break;
	case N3K_MGMT_TUNNEL_TYPE_MPLSOGRE:
		// TODO(n3k): implement along with the rest of
		// MPLSoGRE functionality
		break;
	case N3K_MGMT_TUNNEL_TYPE_VXLAN:
		res_regs->res_11.field.emlc_res_11 =
			1 | // use tunnel table
			(BITS_RANGE32(encap_action->tunnel_idx, 0, 12) << 1);

		res_regs->res_18.field.encap_l4_src_port_0 =
			BITS_RANGE32(encap_action->vxlan.src_port, 0, 9);
		res_regs->res_19.field.encap_l4_src_port_1 =
			BITS_RANGE32(encap_action->vxlan.src_port, 9, 7);
		break;
	}
}

static void fill_local_res_regs(struct lookup_emlc_res_regs *res_regs,
	const struct n3k_mgmt_flow_tbl_action_local *local_action)
{
	res_regs->res_17.field.dec_ttl_en = local_action->decr_ttl;
	if (local_action->modify_l2) {
		uint64_t src_mac, dst_mac;
		dst_mac = n3k_mgmt_convert_rte_ether_addr_to_uint(local_action->modified_l2.dst_mac);
		src_mac = n3k_mgmt_convert_rte_ether_addr_to_uint(local_action->modified_l2.src_mac);
		res_regs->res_0.field.mac_da_0 = BITS_RANGE64(dst_mac, 0, 32);
		res_regs->res_1.field.mac_da_1 = BITS_RANGE64(dst_mac, 32, 16);
		res_regs->res_1.field.mac_sa_0 = BITS_RANGE64(src_mac, 0, 16);
		res_regs->res_2.field.mac_sa_1 = BITS_RANGE64(src_mac, 16, 32);
		res_regs->res_17.field.mac_nat_da_en = 1;
		res_regs->res_17.field.mac_nat_sa_en = 1;
	}
}

static void fill_res_regs_mirror(struct lookup_emlc_res_regs *res_regs,
	const struct n3k_mgmt_flow_tbl_action *action,
	uint32_t recirc_id)
{
	res_regs->res_18.field.recirc_en = 1;
	res_regs->res_15.field.recirc_id_0 = BITS_RANGE32(recirc_id, 0, 16);
	res_regs->res_16.field.recirc_id_1 = BITS_RANGE32(recirc_id, 16, 8);
	res_regs->res_18.field.recirc_mode = (uint32_t)action->mirror.mode;
}

int n3k_mgmt_flow_tbl_fill_res_regs(
	struct n3k_mgmt_hw *hw, struct lookup_emlc_res_regs *res_regs,
	const struct n3k_mgmt_flow_tbl_action *action, uint32_t stats_idx,
	uint32_t recirc_id)
{
	memset(res_regs, 0, sizeof(*res_regs));
	uint8_t port, subport;
	int ret;

	// Metadata action regs
	res_regs->res_14.field.stats_idx_0 = BITS_RANGE32(stats_idx, 0, 16);
	res_regs->res_15.field.stats_idx_1 = BITS_RANGE32(stats_idx, 16, 5);

	res_regs->res_16.field.qid = action->queue;

	if (action->type != N3K_MGMT_FLOW_TBL_ACTION_DROP) {
		ret = n3k_mgmt_dpdk_port_to_hw_port_subport(
			hw, action->port.port_id, &port, &subport);
		if (ret)
			return ret;

		res_regs->res_17.field.port = port;
		res_regs->res_17.field.subport = subport;
	}

	// NAT action
	if (action->nat_type == N3K_MGMT_FLOW_TBL_NAT_TYPE_DNAT) {
		res_regs->res_17.field.ip_nat_da_en = 1;
		res_regs->res_3.field.ip_da_0 = action->nat.modified_ip;
	}
	else if (action->nat_type == N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT) {
		res_regs->res_17.field.ip_nat_sa_en = 1;
		res_regs->res_7.field.ip_sa_0 = action->nat.modified_ip;
	}

	// Vlan action
	switch(action->vlan.type) {
	case N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_INSERT:
		res_regs->res_17.field.insert_vlan = 1;
		res_regs->res_14.field.mod_vid_vlan = action->vlan.tci & 0xFFF;
		res_regs->res_14.field.mod_cfi_cos_vlan = BITS_RANGE32(action->vlan.tci, 12, 4);
		break;
	case N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_MOD:
		res_regs->res_17.field.mod_vlan = 1;
		res_regs->res_14.field.mod_vid_vlan = action->vlan.tci & 0xFFF;
		res_regs->res_14.field.mod_cfi_cos_vlan = BITS_RANGE32(action->vlan.tci, 12, 4);
		break;
	case N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_STRIP:
		res_regs->res_17.field.strip_vlan = 1;
		break;
	case N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_NONE:
		break;
	}

	// Encap/decap/local specific action regs
	switch (action->type) {
	case N3K_MGMT_FLOW_TBL_ACTION_DECAP:
		fill_decap_res_regs(res_regs, &action->decap_action);
		break;
	case N3K_MGMT_FLOW_TBL_ACTION_ENCAP:
		fill_encap_res_regs(res_regs, &action->encap_action);
		break;
	case N3K_MGMT_FLOW_TBL_ACTION_LOCAL:
		fill_local_res_regs(res_regs, &action->local_action);
		break;
	case N3K_MGMT_FLOW_TBL_ACTION_DROP:
		res_regs->res_18.field.drop = 1;
		break;
	}

	if (action->use_mirroring)
		fill_res_regs_mirror(res_regs, action, recirc_id);

	return 0;
}

static int get_next_hw_flow_id(struct n3k_mgmt_hw *hw, uint32_t *hw_flow_id)
{
	int timeout = 100;
	LKUP_EMLC_NXT_HANDLE_t next_handle;

	while (timeout > 0) {
		next_handle.val = csr_read(hw, MKS_CHIP_LKUP_BASE, LKUP_EMLC_NXT_HANDLE);
		if (next_handle.field.valid) {
			*hw_flow_id = next_handle.field.handle;
			RTE_ASSERT(*hw_flow_id < n3k_mgmt_flow_tbl_get_max_flow_id(hw));
			return 0;
		}
		timeout--;
	}

	return -ETIMEDOUT;
}

static int exec_flow_tbl_cmd(struct n3k_mgmt_hw *hw,
	enum LKUP_EMLC_MGMT_CTRL_REQ_TYPE type,
	LKUP_EMLC_MGMT_CTRL_t *emlc_mgmt)
{
	memset(emlc_mgmt, 0, sizeof(*emlc_mgmt));
	emlc_mgmt->field.req_type = type;
	csr_write(hw, MKS_CHIP_LKUP_BASE, LKUP_EMLC_MGMT_CTRL, emlc_mgmt->val);
	return n3k_mgmt_poll_ctrl_reg_with_result(hw,
		N3K_MGMT_CTRL_REG_LKUP_EMLC_MGMT_CTRL, &emlc_mgmt->val);
}

static int program_flow_tbl_entry(struct n3k_mgmt_hw *hw,
	const struct lookup_emlc_key_regs *key_regs,
	const struct lookup_emlc_res_regs *res_regs,
	LKUP_EMLC_MGMT_CTRL_t *emlc_mgmt)
{
	int ret;
	memset(emlc_mgmt, 0, sizeof(*emlc_mgmt));
	n3k_mgmt_program_reg_tbl(hw, N3K_MGMT_REG_TBL_FLOW_TBL_KEY, key_regs);
	n3k_mgmt_program_reg_tbl(hw, N3K_MGMT_REG_TBL_FLOW_TBL_RES, res_regs);

	ret = exec_flow_tbl_cmd(hw, LKUP_EMLC_MGMT_CTRL_REQ_TYPE_INSERT, emlc_mgmt);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR,
			"Error occurred while waiting to complete flow entry add operation: %d",
			ret);
		return ret;
	}
	if (!emlc_mgmt->field.success) {
		N3K_MGMT_LOG(FLOW, ERR,
			"Couldn't insert flow entry. Info code: %u, Error code: %u",
			emlc_mgmt->field.info_code, emlc_mgmt->field.err_code);
		return -EINVAL;
	}

	return 0;
}

int n3k_mgmt_flow_tbl_add_by_regs(struct n3k_mgmt_hw *hw,
	const struct lookup_emlc_key_regs *key_regs,
	const struct lookup_emlc_res_regs *res_regs,
	LKUP_EMLC_MGMT_CTRL_t *emlc_mgmt,
	uint32_t *hw_flow_id)
{
	int ret;

	n3k_mgmt_lock(hw);

	RTE_ASSERT(n3k_mgmt_ctrl_reg_is_busy(hw, N3K_MGMT_CTRL_REG_LKUP_EMLC_MGMT_CTRL) == 0 &&
		"Another read/write operation on flow table is in progress.");

	ret = get_next_hw_flow_id(hw, hw_flow_id);
	if (ret < 0) {
		n3k_mgmt_unlock(hw);
		N3K_MGMT_LOG(FLOW, WARNING, "Failed to get free flow handle");
		return ret;
	}


	if (N3K_MGMT_SHOULD_LOG(REGS, DEBUG)) {
		N3K_MGMT_LOG(REGS, DEBUG, "Adding flow(regs)");
		n3k_mgmt_dump_reg_tbl(N3K_MGMT_REG_TBL_FLOW_TBL_KEY, key_regs);
		n3k_mgmt_dump_reg_tbl(N3K_MGMT_REG_TBL_FLOW_TBL_RES, res_regs);
	}

	ret = program_flow_tbl_entry(hw, key_regs, res_regs, emlc_mgmt);
	if (ret < 0) {
		n3k_mgmt_unlock(hw);
		return ret;
	}

	n3k_mgmt_unlock(hw);
	return 0;
}

int
n3k_mgmt_flow_tbl_fill_regs_mirror(
	struct n3k_mgmt_hw *hw,
	struct lookup_emlc_key_regs *key_regs,
	struct lookup_emlc_res_regs *res_regs,
	const struct n3k_mgmt_flow_tbl_action *action,
	uint32_t recirc_id)
{
	uint8_t port, subport;
	int ret;

	memset(key_regs, 0, sizeof(*key_regs));
	memset(res_regs, 0, sizeof(*res_regs));

	/* fill mirror key */
	key_regs->key_13.field.parse_flag = (1<<1);
	key_regs->key_15.field.src_mac_0 = BITS_RANGE32(recirc_id, 0, 16);
	key_regs->key_16.field.src_mac_1 = BITS_RANGE32(recirc_id, 16, 8);

	/* fill mirror res*/
	ret = n3k_mgmt_dpdk_port_to_hw_port_subport(
		hw, action->mirror.port.port_id, &port, &subport);
	if (ret)
		return ret;

	res_regs->res_17.field.port = port;
	res_regs->res_17.field.subport = subport;

	return 0;
}

static int flow_tbl_add_mirror_by_id(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flow_tbl_action *action,
	uint32_t local_flow_id,
	struct n3k_mgmt_flow_tbl_handle **handle)
{
	struct lookup_emlc_key_regs key_regs;
	struct lookup_emlc_res_regs res_regs;
	LKUP_EMLC_MGMT_CTRL_t emlc_mgmt;
	uint32_t recirc_id;
	uint32_t hw_flow_id;
	int ret;

	N3K_MGMT_LOG(FLOW, DEBUG, "Adding Mirror flow");

	ret = n3k_mgmt_recirc_id_alloc(hw, &recirc_id);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR, "Failed to allocate recirc id");
		return ret;
	}

	ret = n3k_mgmt_flow_tbl_init_mirror_flow_handle(hw, handle, recirc_id, local_flow_id);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR, "Failed to create flow handle");
		goto free_recirc_id;
	}

	ret = n3k_mgmt_flow_tbl_fill_regs_mirror(hw, &key_regs, &res_regs, action, recirc_id);
	if (ret < 0)
		goto free_recirc_id;

	ret = n3k_mgmt_flow_tbl_change_flow_status(hw, *handle,
		N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETED,
		N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDING);
	RTE_ASSERT(ret == 0);

	ret = n3k_mgmt_flow_tbl_add_by_regs(hw, &key_regs, &res_regs, &emlc_mgmt, &hw_flow_id);
	if (ret < 0)
		goto set_status_to_deleted;

	ret = n3k_mgmt_map_local_to_hw_handle(hw, local_flow_id, hw_flow_id);
	RTE_ASSERT(ret == 0);

	ret = n3k_mgmt_flow_tbl_change_flow_status(hw, *handle,
		N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDING,
		N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDED);
	RTE_ASSERT(ret == 0);

	N3K_MGMT_LOG(FLOW, INFO, "New mirror flow added successfully: flow_id=%" PRIu32,
		local_flow_id);

	return 0;

set_status_to_deleted:
	{
		__rte_unused int change_ret = n3k_mgmt_flow_tbl_change_flow_status(
			hw, *handle,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDING,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETED);
		RTE_ASSERT(change_ret == 0);
	}

free_recirc_id:
	n3k_mgmt_recirc_id_free(hw, recirc_id);

	return ret;
}

static int flow_tbl_add_by_id(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flow_tbl_entry *entry,
	uint32_t local_flow_id,
	struct n3k_mgmt_flow_tbl_handle **handle,
	struct n3k_mgmt_flow_tbl_handle *mirror_handle)
{
	struct lookup_emlc_key_regs key_regs;
	struct lookup_emlc_res_regs res_regs;
	LKUP_EMLC_MGMT_CTRL_t emlc_mgmt;
	uint32_t recirc_id = 0;
	uint32_t hw_flow_id;
	uint32_t stats_id = n3k_mgmt_flow_id_to_stats_id(hw, local_flow_id);
	int ret;

	if (N3K_MGMT_SHOULD_LOG(FLOW, DEBUG)) {
		N3K_MGMT_LOG(FLOW, DEBUG, "Adding flow");
		n3k_mgmt_flow_tbl_entry_dump(entry);
	}

	if (!entry->key.use_l2_key && !entry->key.use_tunnel_key) {
		N3K_MGMT_LOG(FLOW, ERR, "L3 key is supported only with tunnel keys.");
		return -EINVAL;
	}

	if (mirror_handle) {
		recirc_id = mirror_handle->recirc_id;
	}

	ret = n3k_mgmt_flow_tbl_fill_key_regs(hw, &key_regs, &entry->key);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR,
			"Failed to translate flow table key to regs: %d", ret);
		return ret;
	}

	ret = n3k_mgmt_flow_tbl_fill_res_regs(
		hw, &res_regs, &entry->action, stats_id, recirc_id);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR,
			"Failed to translate flow table action to regs: %d", ret);
		return ret;
	}

	ret = n3k_mgmt_flow_tbl_init_flow_handle(hw, handle, &entry->action, local_flow_id);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR, "Failed to create flow handle");
		return ret;
	}

	ret = n3k_mgmt_flow_tbl_change_flow_status(hw, *handle,
		N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETED,
		N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDING);
	RTE_ASSERT(ret == 0);

	ret = n3k_mgmt_flow_tracker_enable(hw, stats_id, entry->key.l3.ip_prot);
	if (ret < 0)
		goto set_status_to_deleted;

	ret = n3k_mgmt_flow_tbl_add_by_regs(hw, &key_regs, &res_regs,
		&emlc_mgmt, &hw_flow_id);
	if (ret < 0)
		goto disable_flow_tracker;

	(*handle)->mirror_flow = mirror_handle;

	ret = n3k_mgmt_map_local_to_hw_handle(hw, local_flow_id, hw_flow_id);
	RTE_ASSERT(ret == 0);

	ret = n3k_mgmt_flow_tbl_change_flow_status(hw, *handle,
		N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDING,
		N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDED);
	RTE_ASSERT(ret == 0);

	N3K_MGMT_LOG(FLOW, INFO, "New flow added successfully: flow_id=%" PRIu32,
		local_flow_id);

	return 0;

disable_flow_tracker:
	{
		int ret_ft = n3k_mgmt_flow_tracker_disable(hw, stats_id);
		if (ret_ft < 0) {
			N3K_MGMT_LOG(FLOW, ERR, "Couldn't disable flow tracker. ret = %d",
				ret_ft);
		}
	}
set_status_to_deleted:
	{
		__rte_unused int change_ret = n3k_mgmt_flow_tbl_change_flow_status(
			hw, *handle,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDING,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETED);
		RTE_ASSERT(change_ret == 0);
	}
	return ret;
}

int n3k_mgmt_flow_tbl_add(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flow_tbl_entry *entry,
	struct n3k_mgmt_flow_tbl_handle **handle)
{
	struct n3k_mgmt_flow_tbl_handle *mirror_handle = NULL;
	__rte_unused int free_ret;
	uint32_t mirror_flow_id;
	uint32_t flow_id;
	int ret;

	ret = n3k_mgmt_flow_id_alloc(hw, &flow_id);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, DEBUG, "Failed to allocate handle");
		return ret;
	}

	if (entry->action.use_mirroring) {
		ret = n3k_mgmt_flow_id_alloc(hw, &mirror_flow_id);
		if (ret < 0) {
			N3K_MGMT_LOG(FLOW, DEBUG, "Failed to allocate handle");
			goto free_flow_id;
		}

		ret = flow_tbl_add_mirror_by_id(hw, &entry->action,
						mirror_flow_id, &mirror_handle);
		if (ret < 0) {
			N3K_MGMT_LOG(FLOW, DEBUG, "Failed offload mirror flow");
			goto free_mirror_flow_id;
		}
	}

	ret = flow_tbl_add_by_id(hw, entry, flow_id, handle, mirror_handle);
	if (ret < 0) {
		if (entry->action.use_mirroring)
			goto free_mirror_flow;
		else
			goto free_flow_id;
	}

	return 0;

free_mirror_flow:
	free_ret = n3k_mgmt_flow_tbl_del(hw, mirror_handle);
	if (free_ret < 0) {
		N3K_MGMT_LOG(FLOW, DEBUG, "Failed to free mirror flow");
		goto free_flow_id;
	}

free_mirror_flow_id:
	free_ret = n3k_mgmt_flow_id_free(hw, mirror_flow_id);
	RTE_ASSERT(free_ret == 0);

free_flow_id:
	free_ret = n3k_mgmt_flow_id_free(hw, flow_id);
	RTE_ASSERT(free_ret == 0);
	return ret;
}

static int fill_type_key_info(struct n3k_mgmt_flow_tbl_key *key,
	const struct lookup_emlc_key_regs *key_regs)
{
	uint32_t parse_tunnel_type;
	parse_tunnel_type = BITS_RANGE32(key_regs->key_13.field.parse_flag, 12, 4);
	switch (parse_tunnel_type) {
	case N3K_MGMT_PARS_TUNNEL_TYPE_NONE:
		key->use_tunnel_key = false;
		break;
	case N3K_MGMT_PARS_TUNNEL_TYPE_VXLAN:
		key->use_tunnel_key = true;
		key->tunnel.type = N3K_MGMT_TUNNEL_TYPE_VXLAN;
		break;
	case N3K_MGMT_PARS_TUNNEL_TYPE_MPLSOUDP:
		key->use_tunnel_key = true;
		key->tunnel.type = N3K_MGMT_TUNNEL_TYPE_MPLSOUDP;
		break;
	case N3K_MGMT_PARS_TUNNEL_TYPE_MPLSOGRE:
		key->use_tunnel_key = true;
		key->tunnel.type = N3K_MGMT_TUNNEL_TYPE_MPLSOGRE;
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static void fill_tunnel_key_info(struct n3k_mgmt_flow_tbl_key_tunnel *tunnel,
	const struct lookup_emlc_key_regs *key_regs)
{
	// Outer L3 key info
	// TODO(n3k): add ipv6
	tunnel->outer_l3.type = N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4;
	tunnel->outer_l3.ipv4.dst_ip = key_regs->key_0.field.tunnel_dst_ip;
	tunnel->outer_l3.ipv4.src_ip = key_regs->key_1.field.tunnel_src_ip;
	tunnel->outer_l3.ip_prot = key_regs->key_2.field.tunnel_ip_protocol;

	// MPLS/VXLAN key info
	switch (tunnel->type) {
	case N3K_MGMT_TUNNEL_TYPE_MPLSOUDP:
		tunnel->mpls_udp.label = BITS_RANGE32(key_regs->key_3.field.vxlnid_teid_key_0, 0, 20);
		tunnel->mpls_udp.tos = BITS_RANGE32(key_regs->key_3.field.vxlnid_teid_key_0, 20, 3);
		break;
	case N3K_MGMT_TUNNEL_TYPE_MPLSOGRE:
		// TODO(n3k): implement along with the rest of
		// MPLSoGRE functionality
		break;
	case N3K_MGMT_TUNNEL_TYPE_VXLAN:
		tunnel->vxlan.vni = BITS_RANGE32(
			key_regs->key_3.field.vxlnid_teid_key_0, 0, 24);
		break;
	}
}

int n3k_mgmt_flow_tbl_fill_key_info(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_key *key,
	const struct lookup_emlc_key_regs *key_regs)
{
	uint64_t dst_mac = 0, src_mac = 0;
	int ret;

	memset(key, 0, sizeof(*key));

	// First get type of key
	ret = fill_type_key_info(key, key_regs);
	if (ret < 0)
		return ret;

	// L2 key info
	dst_mac = key_regs->key_14.field.dst_mac_0 |
		((uint64_t)key_regs->key_15.field.dst_mac_1 << 32);
	src_mac = key_regs->key_15.field.src_mac_0 |
		((uint64_t)key_regs->key_16.field.src_mac_1 << 16);
	if (dst_mac != 0 || src_mac != 0) {
		key->use_l2_key = true;
		key->l2.dst_mac = n3k_mgmt_convert_uint_to_rte_ether_addr(dst_mac);
		key->l2.src_mac = n3k_mgmt_convert_uint_to_rte_ether_addr(src_mac);

		key->l2.vlan_tci = key_regs->key_2.field.tci_vlan_a0 |
			(key_regs->key_3.field.tci_vlan_a1 << 8);
	}

	// L3 key info
	key->l3.type = BITS_RANGE32(key_regs->key_13.field.parse_flag, 6, 4);

	switch (key->l3.type) {
		case N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4:
			key->l3.ipv4.dst_ip = key_regs->key_4.field.dst_ip_0 |
				(key_regs->key_5.field.dst_ip_1 << 24);
			key->l3.ipv4.src_ip = key_regs->key_8.field.src_ip_0 |
				(key_regs->key_9.field.src_ip_1 << 24);
			break;

		case N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6:
			key->l3.ipv6.dst_ip[0] = key_regs->key_8.field.dst_ip_4;
			memcpy_bswap32(&key->l3.ipv6.dst_ip[1], &(key_regs->key_7.val), 4);
			memcpy_bswap32(&key->l3.ipv6.dst_ip[5], &(key_regs->key_6.val), 4);
			memcpy_bswap32(&key->l3.ipv6.dst_ip[9], &(key_regs->key_5.val), 4);
			key->l3.ipv6.dst_ip[13] = (key_regs->key_4.field.dst_ip_0 >> 16) & 0xFF;
			key->l3.ipv6.dst_ip[14] = (key_regs->key_4.field.dst_ip_0 >> 8) & 0xFF;
			key->l3.ipv6.dst_ip[15] = key_regs->key_4.field.dst_ip_0 & 0xFF;

			key->l3.ipv6.src_ip[0] = key_regs->key_12.field.src_ip_4;
			memcpy_bswap32(&key->l3.ipv6.src_ip[1], &(key_regs->key_11.val), 4);
			memcpy_bswap32(&key->l3.ipv6.src_ip[5], &(key_regs->key_10.val), 4);
			memcpy_bswap32(&key->l3.ipv6.src_ip[9], &(key_regs->key_9.val), 4);
			key->l3.ipv6.src_ip[13] = (key_regs->key_8.field.src_ip_0 >> 16) & 0xFF;
			key->l3.ipv6.src_ip[14] = (key_regs->key_8.field.src_ip_0 >> 8) & 0xFF;
			key->l3.ipv6.src_ip[15] = key_regs->key_8.field.src_ip_0 & 0xFF;
			break;

		default:
			N3K_MGMT_LOG(FLOW, WARNING,
					"Can't recognize %d as a l3 ethertype",
					key->l3.type);
			break;
	}

	key->l3.ip_prot = key_regs->key_13.field.ip_protocol;

	// L4 key info
	key->l4.dst_port = key_regs->key_12.field.dst_port;
	key->l4.src_port = key_regs->key_12.field.src_port_0 |
			(key_regs->key_13.field.src_port_1 << 8);

	ret = n3k_mgmt_hw_vid_to_dpdk_port(hw, key_regs->key_17.field.vid, &key->port.port_id);
	if (ret < 0)
		return ret;

	// Encap/decap/local specific key info
	if (key->use_tunnel_key)
		fill_tunnel_key_info(&key->tunnel, key_regs);

	// Fill port name
	ret = n3k_vdev_dpdk_port_to_vdev_id(key->port.port_id, &key->port.device_id);
	if (ret < 0)
		return ret;

	return 0;
}

static void fill_type_res_info(
	enum n3k_mgmt_tunnel_type encap_type,
	struct n3k_mgmt_flow_tbl_action *action,
	const struct lookup_emlc_res_regs *res_regs)
{
	switch (action->type) { // If it produces warning then add parsing new actions below
	case N3K_MGMT_FLOW_TBL_ACTION_DECAP:
	case N3K_MGMT_FLOW_TBL_ACTION_ENCAP:
	case N3K_MGMT_FLOW_TBL_ACTION_LOCAL:
	case N3K_MGMT_FLOW_TBL_ACTION_DROP:;
	}

	if (res_regs->res_18.field.drop) {
		action->type = N3K_MGMT_FLOW_TBL_ACTION_DROP;
	} else if (res_regs->res_18.field.mpls_oudpde_eth_hdr) {
		action->type = N3K_MGMT_FLOW_TBL_ACTION_DECAP;
		action->decap_action.type = N3K_MGMT_TUNNEL_TYPE_MPLSOUDP;
	} else if (res_regs->res_18.field.mpls_ogre_de_eth_hdr) {
		action->type = N3K_MGMT_FLOW_TBL_ACTION_DECAP;
		action->decap_action.type = N3K_MGMT_TUNNEL_TYPE_MPLSOGRE;
	} else if (res_regs->res_18.field.decap_vxlan_en) {
		action->type = N3K_MGMT_FLOW_TBL_ACTION_DECAP;
		action->decap_action.type = N3K_MGMT_TUNNEL_TYPE_VXLAN;
	} else {
		uint32_t use_tunnel_table;
		use_tunnel_table = BITS_RANGE32(res_regs->res_11.field.emlc_res_11, 0, 1);
		if (use_tunnel_table) {
			action->type = N3K_MGMT_FLOW_TBL_ACTION_ENCAP;
			action->encap_action.type = encap_type;
		} else {
			action->type = N3K_MGMT_FLOW_TBL_ACTION_LOCAL;
		}
	}
}

static void fill_encap_res_info(struct n3k_mgmt_flow_tbl_action_encap *encap_action,
	const struct lookup_emlc_res_regs *res_regs)
{
	encap_action->decr_ttl = res_regs->res_17.field.dec_ttl_en;
	encap_action->strip_l2 = res_regs->res_19.field.strip_inner_l2;
	switch (encap_action->type) {
	case N3K_MGMT_TUNNEL_TYPE_MPLSOUDP:
		encap_action->tunnel_idx = BITS_RANGE32(res_regs->res_11.field.emlc_res_11, 1, 12);
		encap_action->mpls_udp.label =
			BITS_RANGE32(res_regs->res_11.field.emlc_res_11, 16, 16) |
			(BITS_RANGE32(res_regs->res_12.field.emlc_res_12, 0, 4) << 16);
		encap_action->mpls_udp.tos = BITS_RANGE32(res_regs->res_12.field.emlc_res_12, 4, 3);
		encap_action->mpls_udp.src_port = res_regs->res_18.field.encap_l4_src_port_0 |
			(res_regs->res_19.field.encap_l4_src_port_1 << 9);
		break;
	case N3K_MGMT_TUNNEL_TYPE_MPLSOGRE:
		// TODO(n3k): implement along with the rest of
		// MPLSoGRE functionality
		break;
	case N3K_MGMT_TUNNEL_TYPE_VXLAN:
		encap_action->tunnel_idx = BITS_RANGE32(res_regs->res_11.field.emlc_res_11, 1, 12);
		encap_action->vxlan.src_port =
			res_regs->res_18.field.encap_l4_src_port_0 |
			(res_regs->res_19.field.encap_l4_src_port_1 << 9);
		break;
	}
}

static void fill_local_res_info(struct n3k_mgmt_flow_tbl_action_local *local_action,
	const struct lookup_emlc_res_regs *res_regs)
{
	local_action->decr_ttl = res_regs->res_17.field.dec_ttl_en;

	if (res_regs->res_17.field.mac_nat_sa_en || res_regs->res_17.field.mac_nat_da_en) {
		uint64_t dst_mac = 0, src_mac = 0;
		local_action->modify_l2 = 1;
		dst_mac = res_regs->res_0.field.mac_da_0 |
			((uint64_t)res_regs->res_1.field.mac_da_1 << 32);
		src_mac = res_regs->res_1.field.mac_sa_0 |
			((uint64_t)res_regs->res_2.field.mac_sa_1 << 16);
		local_action->modified_l2.dst_mac = n3k_mgmt_convert_uint_to_rte_ether_addr(dst_mac);
		local_action->modified_l2.src_mac = n3k_mgmt_convert_uint_to_rte_ether_addr(src_mac);
	}
}

static void fill_decap_res_info(struct n3k_mgmt_flow_tbl_action_decap *decap_action,
	__rte_unused const struct lookup_emlc_res_regs *res_regs)
{
	if (res_regs->res_19.field.insert_inner_l2) {
		uint64_t dst_mac = 0, src_mac = 0;
		decap_action->insert_l2 = 1;
		dst_mac = res_regs->res_0.field.mac_da_0 |
			((uint64_t)res_regs->res_1.field.mac_da_1 << 32);
		src_mac = res_regs->res_1.field.mac_sa_0 |
			((uint64_t)res_regs->res_2.field.mac_sa_1 << 16);
		decap_action->inserted_l2.dst_mac = n3k_mgmt_convert_uint_to_rte_ether_addr(dst_mac);
		decap_action->inserted_l2.src_mac = n3k_mgmt_convert_uint_to_rte_ether_addr(src_mac);
	}
}

int n3k_mgmt_flow_tbl_fill_res_info(struct n3k_mgmt_hw *hw,
	enum n3k_mgmt_tunnel_type encap_type,
	struct n3k_mgmt_flow_tbl_action *action,
	const struct lookup_emlc_res_regs *res_regs)
{
	int ret;

	memset(action, 0, sizeof(*action));

	// First get type of result
	fill_type_res_info(encap_type, action, res_regs);

	if (action->type != N3K_MGMT_FLOW_TBL_ACTION_DROP) {
		ret = n3k_mgmt_hw_port_subport_to_dpdk_port(hw,
			res_regs->res_17.field.port, res_regs->res_17.field.subport,
			&action->port.port_id);
		if (ret)
			return ret;

		ret = n3k_vdev_dpdk_port_to_vdev_id(
			action->port.port_id, &action->port.device_id);
		if (ret < 0)
			return ret;
	}

	action->queue = res_regs->res_16.field.qid;

	if (res_regs->res_17.field.insert_vlan == 1) {
		action->vlan.type = N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_INSERT;
		action->vlan.tci = res_regs->res_14.field.mod_vid_vlan;
		action->vlan.tci =
			res_regs->res_14.field.mod_vid_vlan |
			(res_regs->res_14.field.mod_cfi_cos_vlan << 12);
	} else if (res_regs->res_17.field.mod_vlan == 1) {
		action->vlan.type = N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_MOD;
		action->vlan.tci = res_regs->res_14.field.mod_vid_vlan;
		action->vlan.tci =
			res_regs->res_14.field.mod_vid_vlan |
			(res_regs->res_14.field.mod_cfi_cos_vlan << 12);
	} else if (res_regs->res_17.field.strip_vlan == 1) {
		action->vlan.type = N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_STRIP;
	}

	if (res_regs->res_17.field.ip_nat_da_en) {
		action->nat_type = N3K_MGMT_FLOW_TBL_NAT_TYPE_DNAT;
		action->nat.modified_ip = res_regs->res_3.field.ip_da_0;
	} else if (res_regs->res_17.field.ip_nat_sa_en) {
		action->nat_type = N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT;
		action->nat.modified_ip = res_regs->res_7.field.ip_sa_0;
	}

	if (res_regs->res_18.field.recirc_en)
		action->mirror.mode = res_regs->res_18.field.recirc_mode;

	switch (action->type) {
	case N3K_MGMT_FLOW_TBL_ACTION_ENCAP:
		fill_encap_res_info(&action->encap_action, res_regs);
		break;
	case N3K_MGMT_FLOW_TBL_ACTION_LOCAL:
		fill_local_res_info(&action->local_action, res_regs);
		break;
	case N3K_MGMT_FLOW_TBL_ACTION_DECAP:
		fill_decap_res_info(&action->decap_action, res_regs);
		break;
	case N3K_MGMT_FLOW_TBL_ACTION_DROP:
		break;
	}

	return 0;
}

int n3k_mgmt_flow_tbl_get_key_res_by_regs(struct n3k_mgmt_hw *hw,
	uint32_t hw_flow_id, struct lookup_emlc_key_regs *key_regs,
	struct lookup_emlc_res_regs *res_regs)
{
	LKUP_EMLC_MGMT_CTRL_t emlc_mgmt = {{0}};
	LKUP_EMLC_KEY_HANDLE_t handle_reg = {{0}};
	int ret;

	n3k_mgmt_lock(hw);

	handle_reg.field.handle = hw_flow_id;
	csr_write(hw, MKS_CHIP_LKUP_BASE, LKUP_EMLC_KEY_HANDLE, handle_reg.val);

	ret = exec_flow_tbl_cmd(hw, LKUP_EMLC_MGMT_CTRL_REQ_TYPE_SEARCH_HANDLE, &emlc_mgmt);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR,
			"Error occurred while waiting to complete flow entry search operation: %d",
			ret);
		goto release_lock;
	}
	if (!emlc_mgmt.field.success) {
		N3K_MGMT_LOG(FLOW, DEBUG,
			"Couldn't find flow: hw_flow_id=%" PRIu32, hw_flow_id);
		ret = -ENOENT;
		goto release_lock;
	}

	n3k_mgmt_read_reg_tbl(hw, N3K_MGMT_REG_TBL_FLOW_TBL_KEY, key_regs);
	n3k_mgmt_read_reg_tbl(hw, N3K_MGMT_REG_TBL_FLOW_TBL_RES, res_regs);

release_lock:
	n3k_mgmt_unlock(hw);
	return ret;
}

static int
get_key_res_regs_from_handle(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle,
	struct lookup_emlc_key_regs *key_regs,
	struct lookup_emlc_res_regs *res_regs)
{
	int ret;
	uint32_t hw_flow_id;

	ret = n3k_mgmt_local_to_hw_handle(hw, handle->flow_id, &hw_flow_id);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, DEBUG,
			"Couldn't find flow: flow_id=%" PRIu32, handle->flow_id);
		return ret;
	}

	ret = n3k_mgmt_flow_tbl_get_key_res_by_regs(hw, hw_flow_id, key_regs, res_regs);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, DEBUG,
			"Failed to read flow key and res: flow_id=%" PRIu32, handle->flow_id);
		return ret;
	}

	return 0;
}

int n3k_mgmt_flow_tbl_fill_res_mirror_info(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_action *action,
	const struct lookup_emlc_res_regs *mirror_res_regs)
{
	int ret;

	ret = n3k_mgmt_hw_port_subport_to_dpdk_port(hw,
		mirror_res_regs->res_17.field.port,
		mirror_res_regs->res_17.field.subport,
		&action->mirror.port.port_id);
	if (ret < 0)
		return ret;

	ret = n3k_vdev_dpdk_port_to_vdev_id(action->mirror.port.port_id,
		&action->mirror.port.device_id);
	if (ret < 0)
		return ret;

	action->use_mirroring = true;

	return 0;
}

static int flow_tbl_get_mirror(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle,
	struct n3k_mgmt_flow_tbl_action *action)
{

	struct lookup_emlc_key_regs mirror_key_regs;
	struct lookup_emlc_res_regs mirror_res_regs;
	int ret;

	ret = get_key_res_regs_from_handle(hw,
		handle->mirror_flow, &mirror_key_regs, &mirror_res_regs);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, DEBUG,
			"Couldn't read flow entry for flow_id=%" PRIu32 ", ret=%d",
			handle->flow_id, ret);
		return ret;
	}

	ret = n3k_mgmt_flow_tbl_fill_res_mirror_info(hw, action, &mirror_res_regs);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR,
			"Couldn't find mirror info in mirrored flow: "
			"flow_id=%" PRIu32,
			handle->flow_id);
		return ret;
	}

	return 0;
}

int n3k_mgmt_flow_tbl_get(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle,
	struct n3k_mgmt_flow_tbl_entry *entry)
{
	struct lookup_emlc_key_regs key_regs;
	struct lookup_emlc_res_regs res_regs;
	enum n3k_mgmt_tunnel_type encap_type;
	int ret;

	ret = get_key_res_regs_from_handle(hw, handle, &key_regs, &res_regs);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, DEBUG,
			"Couldn't read flow entry for flow_id=%" PRIu32 ", ret=%d",
			handle->flow_id, ret);
		return ret;
	}

	encap_type = handle->encap_type;

	ret = n3k_mgmt_flow_tbl_fill_key_info(hw, &entry->key, &key_regs);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR, "Invalid key: flow_id=%" PRIu32, handle->flow_id);
		return ret;
	}

	ret = n3k_mgmt_flow_tbl_fill_res_info(hw, encap_type, &entry->action, &res_regs);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR, "Invalid action: flow_id=%" PRIu32, handle->flow_id);
		return ret;
	}

	if (handle->mirror_flow) {
		ret = flow_tbl_get_mirror(hw, handle, &entry->action);
		if (ret < 0) {
			N3K_MGMT_LOG(FLOW, ERR, "Failed filling action mirror info: flow_id=%" PRIu32, handle->flow_id);
			return ret;
		}
	}

	if (N3K_MGMT_SHOULD_LOG(FLOW, DEBUG)) {
		N3K_MGMT_LOG(FLOW, DEBUG, "Read flow: flow_id=%" PRIu32, handle->flow_id);
		n3k_mgmt_flow_tbl_entry_dump(entry);
	}

	return 0;
}

int
n3k_mgmt_flow_tbl_del_by_regs(struct n3k_mgmt_hw *hw, uint32_t hw_flow_id)
{
	LKUP_EMLC_MGMT_CTRL_t emlc_mgmt = {{0}};
	LKUP_EMLC_KEY_HANDLE_t handle_reg = {{0}};
	int ret;

	n3k_mgmt_lock(hw);

	handle_reg.field.handle = hw_flow_id;
	csr_write(hw, MKS_CHIP_LKUP_BASE, LKUP_EMLC_KEY_HANDLE, handle_reg.val);

	ret = exec_flow_tbl_cmd(hw, LKUP_EMLC_MGMT_CTRL_REQ_TYPE_DELETE_HANDLE, &emlc_mgmt);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR,
			"Error occurred while waiting to complete flow entry delete operation: "
			"hw_flow_id=%" PRIu32 ", ret=%d",
			hw_flow_id, ret);
		goto release_lock;
	}
	if (!emlc_mgmt.field.success) {
		N3K_MGMT_LOG(FLOW, ERR,
			"Couldn't delete flow entry: "
			"hw_flow_id=%" PRIu32 ", info_code=%u, error_code=%u",
			hw_flow_id,
			emlc_mgmt.field.info_code, emlc_mgmt.field.err_code);
		ret = -EINVAL;
		goto release_lock;
	}

release_lock:
	n3k_mgmt_unlock(hw);
	return ret;
}

static int flow_tbl_del_single_flow(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle)
{
	uint32_t hw_flow_id;
	int ret, ret_ft = 0;

	N3K_MGMT_LOG(FLOW, INFO, "Removing flow: flow_id=%" PRIu32, handle->flow_id);

	if (handle->flow_id >= n3k_mgmt_flow_tbl_get_max_flow_id(hw)) {
		N3K_MGMT_LOG(FLOW, ERR, "Invalid flow_id=%" PRIu32, handle->flow_id);
		return -EINVAL;
	}

	ret = n3k_mgmt_flow_tbl_change_flow_status(hw, handle,
		N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDED,
		N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETING);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR, "Flow has invalid status");
		return ret;
	}

	ret = n3k_mgmt_local_to_hw_handle(hw, handle->flow_id, &hw_flow_id);
	RTE_ASSERT(ret == 0);

	ret = n3k_mgmt_unmap_local_to_hw_handle(hw, handle->flow_id);
	RTE_ASSERT(ret == 0);

	ret = n3k_mgmt_flow_tbl_del_by_regs(hw, hw_flow_id);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR, "Couldn't delete flow from flow table: "
			"flow_id=%" PRIu32 ", ret = %d", handle->flow_id, ret);
	}

	if (!handle->is_mirror_flow) {
		ret_ft = n3k_mgmt_flow_tracker_disable(hw,
			n3k_mgmt_flow_id_to_stats_id(hw, handle->flow_id));
		if (ret_ft < 0) {
			N3K_MGMT_LOG(FLOW, ERR, "Couldn't disable flow tracker: "
				"flow_id=%" PRIu32 ", ret = %d",
				handle->flow_id, ret_ft);
		}
	}

	if (ret == 0 && ret_ft == 0) {
		__rte_unused int free_ret = n3k_mgmt_flow_id_free(hw, handle->flow_id);
		RTE_ASSERT(free_ret == 0);
	} else {
		N3K_MGMT_LOG(FLOW, ERR,
			"Error while deleting flow or disabling flow tracker. Not releasing handle to allocator.");
	}

	{
		__rte_unused int change_ret = n3k_mgmt_flow_tbl_change_flow_status(
			hw, handle,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETING,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETED);
		RTE_ASSERT(change_ret == 0);
	}
	return ret;
}

int n3k_mgmt_flow_tbl_del(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle)
{
	struct n3k_mgmt_flow_tbl_handle *mirror_handle = handle->mirror_flow;
	int ret;

	ret = flow_tbl_del_single_flow(hw, handle);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR, "flow_tbl_del_single_flow failed with ret=%d\n", ret);
	}

	if (mirror_handle) {
		int free_ret = flow_tbl_del_single_flow(hw, mirror_handle);
		if (free_ret < 0) {
			N3K_MGMT_LOG(FLOW, ERR, "Failed to remove mirror flow.");
			return ret;
		}

		free_ret = n3k_mgmt_recirc_id_free(hw, mirror_handle->recirc_id);
		RTE_ASSERT(free_ret == 0);
	}
	return ret;
}

int n3k_mgmt_flow_tbl_dump(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle)
{
	struct n3k_mgmt_flow_tbl_entry entry;
	int ret;

	ret = n3k_mgmt_flow_tbl_get(hw, handle, &entry);
	if (ret < 0)
		return ret;

	n3k_mgmt_flow_tbl_entry_dump(&entry);

	return ret;
}

int
n3k_mgmt_flow_tbl_alloc_next_flow_id_nolock(struct n3k_mgmt_hw *hw, uint32_t *flow_id)
{
	LKUP_EMLC_MGMT_CTRL_t emlc_mgmt = {{0}};
	int ret;

	ret = exec_flow_tbl_cmd(hw, LKUP_EMLC_MGMT_CTRL_REQ_TYPE_GET_HANDLE, &emlc_mgmt);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR,
			"Error occurred while waiting to complete flow entry get handle operation: "
			"ret=%d", ret);
		return ret;
	}
	if (!emlc_mgmt.field.success) {
		N3K_MGMT_LOG(FLOW, ERR,
			"Couldn't get flow handle: info_code=%u, error_code=%u",
			emlc_mgmt.field.info_code, emlc_mgmt.field.err_code);
		return -EINVAL;
	}

	*flow_id = csr_read(hw, MKS_CHIP_LKUP_BASE, LKUP_EMLC_KEY_HANDLE);

	return 0;
}
