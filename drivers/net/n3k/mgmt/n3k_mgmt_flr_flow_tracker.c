/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_bitwise.h"
#include "n3k_mgmt_flow_tbl_handles.h"
#include "n3k_mgmt_flow_tracker.h"
#include "n3k_mgmt_flr_cmd_queue.h"
#include "n3k_mgmt_flr_command.h"
#include "n3k_mgmt_flr_flow_tracker.h"
#include "n3k_mgmt_flr_pkts.h"
#include "n3k_mgmt_flr_pkts_blocks.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_log.h"

#include <rte_atomic.h>
#include <rte_pmd_n3k_flow_tbl_mgmt.h>

#include <stdint.h>

int
n3k_mgmt_flr_flow_tracker_schedule_get(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_query *flow_query_cmd)
{
	struct n3k_mgmt_flow_tbl_handle *handle = flow_query_cmd->handle;
	int ret;

	N3K_MGMT_LOG(FLOW, DEBUG,
		"Scheduling flow query command for flow_id=%" PRIu32 ", stats_id=%" PRIu32,
		handle->flow_id,
		n3k_mgmt_flow_id_to_stats_id(hw, handle->flow_id));

	enum n3k_mgmt_flow_tbl_flow_status status;
	while (true) {
		rte_rwlock_read_lock(&handle->lock);
		if (handle->status != N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDING) {
			status = handle->status;
			rte_rwlock_read_unlock(&handle->lock);
			break;
		}
		rte_rwlock_read_unlock(&handle->lock);
		rte_delay_ms(1);
	}
	if (status != N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDED)
		return -ENOENT;

	ret = n3k_mgmt_flr_cmd_queue_push_command(hw, container_of(flow_query_cmd,
		struct n3k_mgmt_flr_command, flow_query));
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR, "Failed to push flow query command to cmd queue");
		return ret;
	}

	return 0;
}

static int
flr_flow_tracker_finish_get_success(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_query *flow_query_cmd,
	const struct n3k_mgmt_flr_parsed_block_range *parsed_cmd)
{
	struct flow_tracker_data_regs ft_regs;
	N3K_MGMT_LOG(FLOW, DEBUG,
		"Successfully completed scheduled flow query command: flow_id=%" PRIu32,
		flow_query_cmd->handle->flow_id);
	flow_query_cmd->error_code = 0;
	memcpy_bswap32(&ft_regs, parsed_cmd->blocks[N3K_MGMT_FLR_FLOW_QUERY_READ_FT_DATA_BLOCK].data, sizeof(ft_regs));
	n3k_mgmt_flow_tracker_fill_stats(hw, &flow_query_cmd->stats, &ft_regs);
	n3k_mgmt_flow_tbl_set_cached_stats(flow_query_cmd->handle, &flow_query_cmd->stats);
	rte_mb();
	rte_atomic16_set(&flow_query_cmd->done, 1);
	return 0;
}

static int
flr_flow_tracker_finish_get_failure(
	struct n3k_mgmt_flr_command_flow_query *flow_query_cmd)
{
	flow_query_cmd->error_code = -EIO;
	N3K_MGMT_LOG(FLOW, DEBUG,
		"Failed to complete scheduled flow query command: flow_id=%" PRIu32,
		flow_query_cmd->handle->flow_id);
	rte_mb();
	rte_atomic16_set(&flow_query_cmd->done, 1);
	return 0;
}

int
n3k_mgmt_flr_flow_tracker_get_handle_response(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_query *flow_query_cmd,
	const struct n3k_mgmt_flr_parsed_block_range *parsed_cmd,
	bool retry_limit_reached)
{
	if (parsed_cmd != NULL && !n3k_mgmt_flr_contains_error_block(
			parsed_cmd->blocks, parsed_cmd->blocks_num))
		return flr_flow_tracker_finish_get_success(hw,
			flow_query_cmd, parsed_cmd);
	if (retry_limit_reached)
		return flr_flow_tracker_finish_get_failure(flow_query_cmd);
	return -EAGAIN;
}

static int
flow_tracker_append_pkt_blocks(struct n3k_mgmt_hw *hw, uint32_t flow_id,
	struct rte_mbuf *pkt)
{
	int ret;
	uint32_t stats_id = n3k_mgmt_flow_id_to_stats_id(hw, flow_id);

	ret = n3k_mgmt_flr_pkt_add_ft_command_block(pkt,
		stats_id, MKS_FLOW_TRACKER_FT_COMMAND_READ);
	if (ret < 0)
		return ret;

	ret = n3k_mgmt_flr_pkt_add_read_ft_data_block(pkt);
	if (ret < 0)
		goto remove_ft_command_block;

	return 0;

remove_ft_command_block:
	n3k_mgmt_flr_remove_block(pkt, sizeof(MKS_FLOW_TRACKER_FT_COMMAND_t));
	return ret;
}

int
n3k_mgmt_flr_flow_tracker_get_append_pkt_blocks(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flr_command_flow_query *flow_query_cmd,
	struct rte_mbuf *pkt)
{
	return flow_tracker_append_pkt_blocks(hw,
		flow_query_cmd->handle->flow_id, pkt);
}

int
n3k_mgmt_flr_flow_tracker_get_sync(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *flow_handle,
	struct n3k_mgmt_flow_tracker_stats *ft_stats)
{
	int ret;
	struct n3k_mgmt_flr_command *cmd;
	cmd = n3k_mgmt_flr_command_alloc(hw, N3K_MGMT_FLR_COMMAND_FLOW_QUERY);
	if (cmd == NULL) {
		N3K_MGMT_LOG(FLOW, ERR, "Failed to allocate flr cmd");
		return -ENOMEM;
	}
	cmd->flow_query.handle = flow_handle;
	ret = n3k_mgmt_flr_flow_tracker_schedule_get(hw, &cmd->flow_query);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR, "Failed to schedule a flow tracker get command");
		n3k_mgmt_flr_command_free(hw, cmd);
		return ret;
	}

	while (rte_atomic16_read(&cmd->flow_dump.done) == 0)
		rte_delay_ms(1);
	ret = cmd->flow_query.error_code;
	memcpy(ft_stats, &cmd->flow_query.stats, sizeof(*ft_stats));

	n3k_mgmt_flr_command_free(hw, cmd);
	return ret;
}

int
n3k_mgmt_flr_flow_tracker_update_handle_response(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_stats_update *flow_stats_update_cmd,
	const struct n3k_mgmt_flr_parsed_block_range *parsed_cmd,
	__rte_unused bool retry_limit_reached)
{
	struct flow_tracker_data_regs ft_regs;
	int ret = 0;
	if (parsed_cmd == NULL || n3k_mgmt_flr_contains_error_block(
			parsed_cmd->blocks, parsed_cmd->blocks_num)) {
		N3K_MGMT_LOG(FLOW, WARNING,
			"Failed to update flow statistics for flow_id=%d",
			flow_stats_update_cmd->handle->flow_id);
		ret = -EIO;
		goto free_cmd;
	}

	memcpy_bswap32(&ft_regs,
		parsed_cmd->blocks[N3K_MGMT_FLR_FLOW_QUERY_READ_FT_DATA_BLOCK].data,
		sizeof(ft_regs));

	struct n3k_mgmt_flow_tracker_stats stats;
	n3k_mgmt_flow_tracker_fill_stats(hw, &stats, &ft_regs);

	n3k_mgmt_flow_tbl_set_cached_stats(flow_stats_update_cmd->handle, &stats);

free_cmd:
	n3k_mgmt_flr_command_free(hw, container_of(flow_stats_update_cmd,
		struct n3k_mgmt_flr_command, flow_stats_update));
	return ret;
}

int
n3k_mgmt_flr_flow_tracker_update_append_pkt_blocks(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flr_command_flow_stats_update *flow_stats_update_cmd,
	struct rte_mbuf *pkt)
{
	return flow_tracker_append_pkt_blocks(hw,
		flow_stats_update_cmd->handle->flow_id, pkt);
}
