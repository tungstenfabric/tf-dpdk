/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_FLR_FLOW_TBL_H_
#define _PMD_NET_N3K_MGMT_FLR_FLOW_TBL_H_

#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_flr_command.h"

#include <rte_pmd_n3k_flow_tbl_mgmt.h>
#include <rte_pmd_n3k_mp.h>
#include <stdint.h>

enum n3k_mgmt_flr_flow_create_blocks_order {
	N3K_MGMT_FLR_FLOW_CREATE_FLOW_TRACKER_DATA_BLOCK,
	N3K_MGMT_FLR_FLOW_CREATE_FLOW_TRACKER_COMMAND_BLOCK,
	N3K_MGMT_FLR_FLOW_CREATE_KEY_BLOCK,
	N3K_MGMT_FLR_FLOW_CREATE_ACTION_BLOCK,
	N3K_MGMT_FLR_FLOW_CREATE_EMLC_MGMT_CTRL_BLOCK,
	N3K_MGMT_FLR_FLOW_CREATE_READ_EMLC_KEY_HANDLE_BLOCK,
	N3K_MGMT_FLR_FLOW_CREATE_READ_EMLC_MGMT_CTRL_BLOCK,
	_N3K_MGMT_FLR_FLOW_CREATE_MAX,
};

enum n3k_mgmt_flr_mirror_flow_create_blocks_order {
	N3K_MGMT_FLR_MIRROR_FLOW_CREATE_KEY_BLOCK,
	N3K_MGMT_FLR_MIRROR_FLOW_CREATE_ACTION_BLOCK,
	N3K_MGMT_FLR_MIRROR_FLOW_CREATE_EMLC_MGMT_CTRL_BLOCK,
	N3K_MGMT_FLR_MIRROR_FLOW_CREATE_READ_EMLC_KEY_HANDLE_BLOCK,
	N3K_MGMT_FLR_MIRROR_FLOW_CREATE_READ_EMLC_MGMT_CTRL_BLOCK,
	_N3K_MGMT_FLR_MIRROR_FLOW_CREATE_MAX,
};

enum n3k_mgmt_flr_flow_destroy_blocks_order {
	N3K_MGMT_FLR_FLOW_DESTROY_HANDLE_BLOCK,
	N3K_MGMT_FLR_FLOW_DESTROY_EMLC_MGMT_CTRL_BLOCK,
	N3K_MGMT_FLR_FLOW_DESTROY_FLOW_TRACKER_DATA_BLOCK,
	N3K_MGMT_FLR_FLOW_DESTROY_FLOW_TRACKER_COMMAND_BLOCK,
	_N3K_MGMT_FLR_FLOW_DESTROY_MAX,
};

enum n3k_mgmt_flr_mirror_flow_destroy_blocks_order {
	N3K_MGMT_FLR_MIRROR_FLOW_DESTROY_HANDLE_BLOCK,
	N3K_MGMT_FLR_MIRROR_FLOW_DESTROY_EMLC_MGMT_CTRL_BLOCK,
	_N3K_MGMT_FLR_MIRROR_FLOW_DESTROY_MAX,
};

enum n3k_mgmt_flr_flow_dump_blocks_order {
	N3K_MGMT_FLR_FLOW_DUMP_HANDLE_BLOCK,
	N3K_MGMT_FLR_FLOW_DUMP_EMLC_MGMT_CTRL_BLOCK,
	N3K_MGMT_FLR_FLOW_DUMP_FLOW_READ_KEY_BLOCK,
	N3K_MGMT_FLR_FLOW_DUMP_FLOW_READ_RES_BLOCK,
	_N3K_MGMT_FLR_FLOW_DUMP_MAX,
};

/* After successful call, flow_create_cmd shouldn't be used. The command will
 * be freed automatically in success or failure handler. Using the command after
 * successful call may cause races.
 */
int n3k_mgmt_flr_flow_tbl_schedule_add(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_create *flow_create_cmd,
	struct n3k_mgmt_flow_tbl_handle **handle);

int n3k_mgmt_flr_flow_tbl_add_handle_response(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_create *flow_create_cmd,
	const struct n3k_mgmt_flr_parsed_block_range *parsed_cmd,
	bool retry_limit_reached);

int n3k_mgmt_flr_flow_tbl_add_append_pkt_blocks(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flr_command_flow_create *flow_create_cmd,
	struct rte_mbuf *pkt);

/* After successful call, flow_destroy_cmd shouldn't be used. The command will
 * be freed automatically in success or failure handler. Using the command after
 * successful call may cause races.
 */
int n3k_mgmt_flr_flow_tbl_schedule_del(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_destroy *flow_destroy_cmd);

int n3k_mgmt_flr_flow_tbl_del_handle_response(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_destroy *flow_destroy_cmd,
	const struct n3k_mgmt_flr_parsed_block_range *parsed_cmd,
	bool retry_limit_reached);

int n3k_mgmt_flr_flow_tbl_del_append_pkt_blocks(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flr_command_flow_destroy *flow_destroy_cmd,
	struct rte_mbuf *pkt);

/* Schedule flow read and wait for the result. Due to synchronization,
 * this function should be used only for debug purposes or in places not
 * critical to driver performance.
 */
int n3k_mgmt_flr_flow_sync_dump(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *flow_handle,
	struct n3k_mgmt_flow_tbl_entry *tbl_entry);

int n3k_mgmt_flr_flow_tbl_dump_handle_response(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_dump *flow_dump_cmd,
	const struct n3k_mgmt_flr_parsed_block_range *parsed_cmd,
	bool retry_limit_reached);

int n3k_mgmt_flr_flow_tbl_dump_append_pkt_blocks(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flr_command_flow_dump *flow_dump_cmd,
	struct rte_mbuf *pkt);

#endif /* _PMD_NET_N3K_MGMT_FLR_FLOW_TBL_H_ */
