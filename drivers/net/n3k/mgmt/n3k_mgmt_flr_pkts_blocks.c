/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_bitwise.h"
#include "n3k_mgmt_flow_tbl_regs_api.h"
#include "n3k_mgmt_flow_tbl.h"
#include "n3k_mgmt_flow_tracker_regs_api.h"
#include "n3k_mgmt_flow_tracker.h"
#include "n3k_mgmt_flr_cmd_queue.h"
#include "n3k_mgmt_flr_pkts_blocks.h"
#include "n3k_mgmt_flr_pkts.h"
#include "n3k_mgmt_log.h"

#include <rte_pmd_n3k_flow_tbl_mgmt.h>

int
n3k_mgmt_flr_pkt_add_handle_block(struct rte_mbuf *pkt, uint32_t handle)
{
	LKUP_EMLC_KEY_HANDLE_t handle_reg = {{0}};
	handle_reg.field.handle = handle;
	return n3k_mgmt_flr_add_write_block(pkt,
		MKS_CHIP_LKUP_BASE + LKUP_EMLC_KEY_HANDLE,
		&handle_reg, sizeof(handle_reg));
}

int
n3k_mgmt_flr_pkt_add_emlc_mgmt_ctrl_block(struct rte_mbuf *pkt,
	enum LKUP_EMLC_MGMT_CTRL_REQ_TYPE type)
{
	LKUP_EMLC_MGMT_CTRL_t emlc_mgmt_ctrl = {{0}};
	emlc_mgmt_ctrl.field.req_type = type;
	return n3k_mgmt_flr_add_write_block(pkt,
		MKS_CHIP_LKUP_BASE + LKUP_EMLC_MGMT_CTRL,
		&emlc_mgmt_ctrl, sizeof(emlc_mgmt_ctrl));
}

int
n3k_mgmt_flr_pkt_add_ft_enable_data_block(struct n3k_mgmt_hw *hw, struct rte_mbuf *pkt, uint16_t ip_prot)
{
	struct flow_tracker_data_regs ft_regs;
	n3k_mgmt_flow_tracker_enable_fill_regs(hw, &ft_regs, ip_prot);
	return n3k_mgmt_flr_add_write_block(pkt,
		MKS_CHIP_MKS_FLOW_TRACKER_BASE + MKS_FLOW_TRACKER_FT_DATA_0,
		&ft_regs, sizeof(ft_regs));
}

int
n3k_mgmt_flr_pkt_add_ft_command_block(struct rte_mbuf *pkt, uint32_t stats_idx,
	enum MKS_FLOW_TRACKER_FT_COMMAND_OP_TYPE type)
{
	MKS_FLOW_TRACKER_FT_COMMAND_t ft_comm = {{0}};
	ft_comm.field.index = stats_idx;
	ft_comm.field.operation = type;
	return n3k_mgmt_flr_add_write_block(pkt,
		MKS_CHIP_MKS_FLOW_TRACKER_BASE + MKS_FLOW_TRACKER_FT_COMMAND,
		&ft_comm, sizeof(ft_comm));
}

int
n3k_mgmt_flr_pkt_add_read_emlc_mgmt_ctrl_block(struct rte_mbuf *pkt)
{
	return n3k_mgmt_flr_add_read_block(pkt,
		MKS_CHIP_LKUP_BASE + LKUP_EMLC_MGMT_CTRL,
		sizeof(LKUP_EMLC_MGMT_CTRL_t));
}

int
n3k_mgmt_flr_pkt_add_read_ft_data_block(struct rte_mbuf *pkt)
{
	return n3k_mgmt_flr_add_read_block(pkt,
		MKS_CHIP_MKS_FLOW_TRACKER_BASE + MKS_FLOW_TRACKER_FT_DATA_0,
		sizeof(struct flow_tracker_data_regs));
}

int
n3k_mgmt_flr_pkt_add_ft_disable_data_block(struct rte_mbuf *pkt)
{
	struct flow_tracker_data_regs ft_regs;
	n3k_mgmt_flow_tracker_disable_fill_regs(&ft_regs);
	return n3k_mgmt_flr_add_write_block(pkt,
		MKS_CHIP_MKS_FLOW_TRACKER_BASE + MKS_FLOW_TRACKER_FT_DATA_0,
		&ft_regs, sizeof(ft_regs));
}

static int
write_key_block(struct rte_mbuf *pkt,
	struct lookup_emlc_key_regs *key_regs)
{
	return n3k_mgmt_flr_add_write_block(pkt,
		MKS_CHIP_LKUP_BASE + LKUP_EMLC_KEY_0,
		key_regs, sizeof(*key_regs));
}

static int
write_action_block(struct rte_mbuf *pkt,
	struct lookup_emlc_res_regs *res_regs)
{
	return n3k_mgmt_flr_add_write_block(pkt,
		MKS_CHIP_LKUP_BASE + LKUP_EMLC_RES_0,
		res_regs, sizeof(*res_regs));
}

int n3k_mgmt_flr_pkt_add_key_action_block(
    struct n3k_mgmt_hw *hw, struct rte_mbuf *pkt,
    const struct n3k_mgmt_flow_tbl_key *key,
    const struct n3k_mgmt_flow_tbl_action *action, uint32_t stats_id,
    uint32_t recirc_id)
{
	struct lookup_emlc_key_regs key_regs;
	struct lookup_emlc_res_regs res_regs;
	int ret;

	ret = n3k_mgmt_flow_tbl_fill_key_regs(hw, &key_regs, key);
	if (ret < 0)
		return ret;
	ret = n3k_mgmt_flow_tbl_fill_res_regs(hw, &res_regs,
		action, stats_id, recirc_id);
	if (ret < 0)
		return ret;

	ret = write_key_block(pkt, &key_regs);
	if (ret < 0)
		return ret;
	ret = write_action_block(pkt, &res_regs);
	if (ret < 0) {
		n3k_mgmt_flr_remove_block(pkt, sizeof(key_regs));
		return ret;
	}
	return 0;
}

int n3k_mgmt_flr_pkt_add_mirror_key_action_block(
    struct n3k_mgmt_hw *hw, struct rte_mbuf *pkt,
    const struct n3k_mgmt_flow_tbl_action *action, uint32_t recirc_id)
{
	struct lookup_emlc_key_regs key_regs;
	struct lookup_emlc_res_regs res_regs;
	int ret;

	ret = n3k_mgmt_flow_tbl_fill_regs_mirror(hw, &key_regs, &res_regs, action, recirc_id);
	if (ret < 0)
		return ret;

	ret = write_key_block(pkt, &key_regs);
	if (ret < 0)
		return ret;
	ret = write_action_block(pkt, &res_regs);
	if (ret < 0) {
		n3k_mgmt_flr_remove_block(pkt, sizeof(key_regs));
		return ret;
	}
	return 0;
}

int
n3k_mgmt_flr_pkt_add_read_emlc_key_handle_block(struct rte_mbuf *pkt)
{
	return n3k_mgmt_flr_add_read_block(pkt,
		MKS_CHIP_LKUP_BASE + LKUP_EMLC_KEY_HANDLE,
		sizeof(LKUP_EMLC_KEY_HANDLE_t));
}
