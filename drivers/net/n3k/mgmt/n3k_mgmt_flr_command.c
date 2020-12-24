/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include <rte_mbuf.h>
#include <rte_mempool.h>

#include "n3k_mgmt_flow_tbl_handles.h"
#include "n3k_mgmt_flr_cmd_queue.h"
#include "n3k_mgmt_flr_command.h"
#include "n3k_mgmt_flr_flow_tbl.h"
#include "n3k_mgmt_flr_flow_tracker.h"
#include "n3k_mgmt_flr_pkts_blocks.h"
#include "n3k_mgmt_flr_pkts.h"
#include "n3k_mgmt_flr_read_reg_32.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_log.h"

enum { N3K_MGMT_FLR_COMMAND_MAX_FAILURES = 5 };

/* This is just a rte_mempool ptr to avoid slow double ptr dereference */
struct n3k_mgmt_flr_commands_internal;

int
n3k_mgmt_flr_commands_init(struct n3k_mgmt_hw *hw)
{
	const size_t COMMANDS_POOL_SIZE = 2 * 1024 * 1024 - 1;
	const size_t COMMAND_SIZE = sizeof(struct n3k_mgmt_flr_command);
	const size_t CACHE_SIZE =
		RTE_MIN(RTE_CACHE_LINE_SIZE, RTE_MEMPOOL_CACHE_MAX_SIZE);

	N3K_MGMT_LOG(INFO, "Initializing FLR commands");

	if (hw->flr_commands_internal) {
		N3K_MGMT_LOG(ERR, "FLR commands already initialized");
		return -EINVAL;
	}

	struct rte_mempool **mempool =
		(struct rte_mempool **)&hw->flr_commands_internal;
	*mempool = rte_mempool_create(
		"n3k_flr_commands", COMMANDS_POOL_SIZE, COMMAND_SIZE, CACHE_SIZE,
		0, NULL, NULL, NULL, NULL, 0, 0);
	if (!*mempool) {
		N3K_MGMT_LOG(ERR, "Failed to allocate rte_mempool");
		return -rte_errno;
	}

	return 0;
}

static inline void
n3k_mgmt_flr_command_reset(
	struct n3k_mgmt_flr_command *cmd, enum n3k_mgmt_flr_command_type type)
{
	cmd->metadata.type = type;
	cmd->metadata.processed_count = 0;

	switch (type) {
	case N3K_MGMT_FLR_COMMAND_FLOW_QUERY:
		rte_atomic16_init(&cmd->flow_query.done);
		break;
	case N3K_MGMT_FLR_COMMAND_FLOW_CREATE:
		cmd->flow_create.insert_mirror_flow = false;
		cmd->flow_create.timeouted = false;
		cmd->flow_create.flow_created = FLOW_CREATE_CMD_FLOW_NOT_ADDED;
		break;
	case N3K_MGMT_FLR_COMMAND_FLOW_DESTROY:
		cmd->flow_destroy.destroy_mirror_flow = false;
		cmd->flow_destroy.timeouted = false;
		cmd->flow_destroy.flow_destroyed = FLOW_DESTROY_CMD_FLOW_NOT_DESTROYED;
		break;
	case N3K_MGMT_FLR_COMMAND_READ_REG_32:
		rte_atomic16_init(&cmd->reg_32_read.done);
		break;
	case N3K_MGMT_FLR_COMMAND_FLOW_DUMP:
		rte_atomic16_init(&cmd->flow_dump.done);
		break;
	case N3K_MGMT_FLR_COMMAND_FLOW_HANDLE:
		// TODO(n3k): implement
		break;
	}
}

struct n3k_mgmt_flr_command *
n3k_mgmt_flr_command_alloc(
	struct n3k_mgmt_hw *hw, enum n3k_mgmt_flr_command_type type)
{
	struct n3k_mgmt_flr_command *cmd = NULL;
	struct rte_mempool *mp = (struct rte_mempool *)hw->flr_commands_internal;
	rte_mempool_get(mp, (void **)&cmd);

	if (unlikely(cmd == NULL))
		return NULL;

	n3k_mgmt_flr_command_reset(cmd, type);

	return cmd;
}

void
n3k_mgmt_flr_command_free(
	struct n3k_mgmt_hw *hw, struct n3k_mgmt_flr_command *cmd)
{
	struct rte_mempool *mp = (struct rte_mempool *)hw->flr_commands_internal;
	rte_mempool_put(mp, cmd);
}

int
n3k_mgmt_flr_command_to_pkt(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flr_command *cmd,
	struct rte_mbuf *pkt)
{
	int ret;
	ret = n3k_mgmt_flr_init_pkt(pkt, cmd->metadata.packet_id);
	if (ret < 0) {
		N3K_MGMT_FLR_LOG(ERR, "Failed to init packet");
		return ret;
	}
	switch (cmd->metadata.type) {
	case N3K_MGMT_FLR_COMMAND_FLOW_CREATE:
		ret = n3k_mgmt_flr_flow_tbl_add_append_pkt_blocks(hw,
			&cmd->flow_create, pkt);
		if (ret < 0) {
			N3K_MGMT_FLR_LOG(ERR, "Failed to append flow create blocks");
			return ret;
		}
		break;
	case N3K_MGMT_FLR_COMMAND_FLOW_DESTROY:
		ret = n3k_mgmt_flr_flow_tbl_del_append_pkt_blocks(hw,
			&cmd->flow_destroy, pkt);
		if (ret < 0) {
			N3K_MGMT_FLR_LOG(ERR, "Failed to append flow destroy blocks");
			return ret;
		}
		break;
	case N3K_MGMT_FLR_COMMAND_FLOW_QUERY:
		ret = n3k_mgmt_flr_flow_tracker_get_append_pkt_blocks(hw,
			&cmd->flow_query, pkt);
		if (ret < 0) {
			N3K_MGMT_FLR_LOG(ERR, "Failed to append flow query blocks");
			return ret;
		}
		break;
	case N3K_MGMT_FLR_COMMAND_READ_REG_32:
		ret = n3k_mgmt_flr_read_reg_32_append_pkt_blocks(hw,
			&cmd->reg_32_read, pkt);
		if (ret < 0) {
			N3K_MGMT_FLR_LOG(ERR, "Failed to append read reg 32 blocks");
			return ret;
		}
		break;
	case N3K_MGMT_FLR_COMMAND_FLOW_HANDLE:
		return -ENOTSUP;
	case N3K_MGMT_FLR_COMMAND_FLOW_DUMP:
		ret = n3k_mgmt_flr_flow_tbl_dump_append_pkt_blocks(hw,
			&cmd->flow_dump, pkt);
		if (ret < 0) {
			N3K_MGMT_FLR_LOG(ERR, "Failed to append flow dump blocks");
			return ret;
		}
		break;
	}
	ret = n3k_mgmt_flr_add_icv(pkt);
	if (ret < 0) {
		N3K_MGMT_FLR_LOG(ERR, "Failed to append ICV");
		return ret;
	}
	return 0;
}

int
n3k_mgmt_flr_command_handle_response(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command *cmd,
	const struct n3k_mgmt_flr_parsed_pkt *parsed_pkt)
{
	bool retry_limit_reached = cmd->metadata.processed_count == N3K_MGMT_FLR_COMMAND_MAX_FAILURES;
	int ret = -EINVAL;

	// Response handlers should return -EAGAIN when command should be
	// retried. Passed parsed_pkt may be NULL which means that command
	// was timeouted.
	switch (cmd->metadata.type) {
	case N3K_MGMT_FLR_COMMAND_FLOW_CREATE:
		ret = n3k_mgmt_flr_flow_tbl_add_handle_response(hw,
			&cmd->flow_create, parsed_pkt, retry_limit_reached);
		break;
	case N3K_MGMT_FLR_COMMAND_FLOW_DESTROY:
		ret = n3k_mgmt_flr_flow_tbl_del_handle_response(hw,
			&cmd->flow_destroy, parsed_pkt, retry_limit_reached);
		break;
	case N3K_MGMT_FLR_COMMAND_FLOW_QUERY:
		ret = n3k_mgmt_flr_flow_tracker_get_handle_response(hw,
			&cmd->flow_query, parsed_pkt, retry_limit_reached);
		break;
	case N3K_MGMT_FLR_COMMAND_READ_REG_32:
		ret = n3k_mgmt_flr_read_reg_32_handle_response(hw,
			&cmd->reg_32_read, parsed_pkt, retry_limit_reached);
		break;
	case N3K_MGMT_FLR_COMMAND_FLOW_HANDLE:
		ret = -ENOTSUP;
		break;
	case N3K_MGMT_FLR_COMMAND_FLOW_DUMP:
		ret = n3k_mgmt_flr_flow_tbl_dump_handle_response(hw,
			&cmd->flow_dump, parsed_pkt, retry_limit_reached);
		break;
	}

	if (!retry_limit_reached && ret == -EAGAIN) {
		n3k_mgmt_flr_cmd_queue_push_command(hw, cmd);
		return 0;
	}

	return ret;
}
