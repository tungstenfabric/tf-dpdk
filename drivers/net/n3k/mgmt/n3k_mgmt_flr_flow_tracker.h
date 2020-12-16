/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_FLR_FLOW_TRACKER_H_
#define _PMD_NET_N3K_MGMT_FLR_FLOW_TRACKER_H_

#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_flr_command.h"
#include "n3k_mgmt_flr_pkts.h"

#include <rte_pmd_n3k_flow_tbl_mgmt.h>
#include <stdint.h>

enum n3k_mgmt_flr_flow_query_blocks_order {
	N3K_MGMT_FLR_FLOW_QUERY_FT_COMMAND_BLOCK,
	N3K_MGMT_FLR_FLOW_QUERY_READ_FT_DATA_BLOCK,
};

// After successful call, flow_query_cmd can be used. The command should be
// freed manually after flow_query_cmd.done set to 1.
int n3k_mgmt_flr_flow_tracker_schedule_get(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_query *flow_query_cmd);

int n3k_mgmt_flr_flow_tracker_get_handle_response(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_query *flow_query_cmd,
	const struct n3k_mgmt_flr_parsed_pkt *parsed_pkt,
	bool retry_limit_reached);

int n3k_mgmt_flr_flow_tracker_get_append_pkt_blocks(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flr_command_flow_query *flow_query_cmd,
	struct rte_mbuf *pkt);

int n3k_mgmt_flr_flow_tracker_get_sync(struct n3k_mgmt_hw *hw,
