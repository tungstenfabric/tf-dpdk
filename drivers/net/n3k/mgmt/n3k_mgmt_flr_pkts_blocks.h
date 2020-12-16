/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_FLR_PKTS_BLOCKS_H_
#define _PMD_NET_N3K_MGMT_FLR_PKTS_BLOCKS_H_

#include "n3k_mgmt_flow_tbl_regs_api.h"
#include "n3k_mgmt_flow_tracker_regs_api.h"
#include "n3k_mgmt_flr_command.h"
#include "n3k_mgmt_hw.h"

#include <rte_pmd_n3k_flow_tbl_mgmt.h>

int n3k_mgmt_flr_pkt_add_handle_block(struct rte_mbuf *pkt, uint32_t handle);

int n3k_mgmt_flr_pkt_add_emlc_mgmt_ctrl_block(struct rte_mbuf *pkt,
	enum LKUP_EMLC_MGMT_CTRL_REQ_TYPE type);

int n3k_mgmt_flr_pkt_add_ft_enable_data_block(struct n3k_mgmt_hw *hw, struct rte_mbuf *pkt, uint16_t ip_prot);

int n3k_mgmt_flr_pkt_add_ft_command_block(struct rte_mbuf *pkt, uint32_t stats_idx,
	enum MKS_FLOW_TRACKER_FT_COMMAND_OP_TYPE type);

int n3k_mgmt_flr_pkt_add_read_emlc_mgmt_ctrl_block(struct rte_mbuf *pkt);

int n3k_mgmt_flr_pkt_add_read_ft_data_block(struct rte_mbuf *pkt);

int n3k_mgmt_flr_pkt_add_ft_disable_data_block(struct rte_mbuf *pkt);

int n3k_mgmt_flr_pkt_add_key_action_block(struct n3k_mgmt_hw *hw,
	struct rte_mbuf *pkt,
	const struct n3k_mgmt_flow_tbl_key *key,
	const struct n3k_mgmt_flow_tbl_action *action,
	uint32_t stats_id, uint32_t recirc_id);
int n3k_mgmt_flr_pkt_add_mirror_key_action_block(struct n3k_mgmt_hw *hw, struct rte_mbuf *pkt,
	const struct n3k_mgmt_flow_tbl_action *action, uint32_t recirc_id);
