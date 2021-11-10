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

	N3K_MGMT_LOG(DEFAULT, INFO, "Initializing FLR commands");

	if (hw->flr_commands_internal) {
		N3K_MGMT_LOG(DEFAULT, ERR, "FLR commands already initialized");
		return -EINVAL;
	}

	struct rte_mempool **mempool =
		(struct rte_mempool **)&hw->flr_commands_internal;
	*mempool = rte_mempool_create(
		"n3k_flr_commands", COMMANDS_POOL_SIZE, COMMAND_SIZE, CACHE_SIZE,
		0, NULL, NULL, NULL, NULL, 0, 0);
	if (!*mempool) {
		N3K_MGMT_LOG(DEFAULT, ERR, "Failed to allocate rte_mempool");
		return -rte_errno;
	}

	return 0;
}

size_t
n3k_mgmt_flr_command_blocks_num(struct n3k_mgmt_flr_command *cmd)
{
	switch (cmd->metadata.type) {
	case N3K_MGMT_FLR_COMMAND_FLOW_QUERY:
		return _N3K_MGMT_FLR_FLOW_QUERY_MAX;
	case N3K_MGMT_FLR_COMMAND_FLOW_STATS_UPDATE:
		return _N3K_MGMT_FLR_FLOW_QUERY_MAX;
	case N3K_MGMT_FLR_COMMAND_FLOW_CREATE:
		return cmd->flow_create.insert_mirror_flow ?
			_N3K_MGMT_FLR_MIRROR_FLOW_CREATE_MAX :
			_N3K_MGMT_FLR_FLOW_CREATE_MAX;
	case N3K_MGMT_FLR_COMMAND_FLOW_DESTROY:
		return cmd->flow_destroy.destroy_mirror_flow ?
			_N3K_MGMT_FLR_MIRROR_FLOW_DESTROY_MAX :
			_N3K_MGMT_FLR_FLOW_DESTROY_MAX;
	case N3K_MGMT_FLR_COMMAND_READ_REG_32:
		return _N3K_MGMT_FLR_READ_REG_32_MAX;
	case N3K_MGMT_FLR_COMMAND_FLOW_DUMP:
		return _N3K_MGMT_FLR_FLOW_DUMP_MAX;
	case N3K_MGMT_FLR_COMMAND_FLOW_HANDLE:
		 // TODO: implement along with the rest of software id management
		 // when it is implemented in the hardware
		RTE_ASSERT(false);
		return 0;
	}
	RTE_ASSERT(false);
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
	case N3K_MGMT_FLR_COMMAND_FLOW_STATS_UPDATE:
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
		// TODO: implement along with the rest of software id management
		 // when it is implemented in the hardware
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

static int
append_command_blocks(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flr_command *cmd, struct rte_mbuf *pkt)
{
	int ret = -EINVAL;
	switch (cmd->metadata.type) {
	case N3K_MGMT_FLR_COMMAND_FLOW_CREATE:
		ret = n3k_mgmt_flr_flow_tbl_add_append_pkt_blocks(hw,
			&cmd->flow_create, pkt);
		if (ret < 0) {
			N3K_MGMT_LOG(FLR_DEFAULT, ERR, "Failed to append flow create blocks");
			return ret;
		}
		break;
	case N3K_MGMT_FLR_COMMAND_FLOW_DESTROY:
		ret = n3k_mgmt_flr_flow_tbl_del_append_pkt_blocks(hw,
			&cmd->flow_destroy, pkt);
		if (ret < 0) {
			N3K_MGMT_LOG(FLR_DEFAULT, ERR, "Failed to append flow destroy blocks");
			return ret;
		}
		break;
	case N3K_MGMT_FLR_COMMAND_FLOW_STATS_UPDATE:
		ret = n3k_mgmt_flr_flow_tracker_update_append_pkt_blocks(hw,
			&cmd->flow_stats_update, pkt);
		if (ret < 0) {
			N3K_MGMT_LOG(FLR_DEFAULT, ERR, "Failed to append flow stats update blocks");
			return ret;
		}
		break;
	case N3K_MGMT_FLR_COMMAND_FLOW_QUERY:
		ret = n3k_mgmt_flr_flow_tracker_get_append_pkt_blocks(hw,
			&cmd->flow_query, pkt);
		if (ret < 0) {
			N3K_MGMT_LOG(FLR_DEFAULT, ERR, "Failed to append flow query blocks");
			return ret;
		}
		break;
	case N3K_MGMT_FLR_COMMAND_READ_REG_32:
		ret = n3k_mgmt_flr_read_reg_32_append_pkt_blocks(hw,
			&cmd->reg_32_read, pkt);
		if (ret < 0) {
			N3K_MGMT_LOG(FLR_DEFAULT, ERR, "Failed to append read reg 32 blocks");
			return ret;
		}
		break;
	case N3K_MGMT_FLR_COMMAND_FLOW_HANDLE:
		return -ENOTSUP;
	case N3K_MGMT_FLR_COMMAND_FLOW_DUMP:
		ret = n3k_mgmt_flr_flow_tbl_dump_append_pkt_blocks(hw,
			&cmd->flow_dump, pkt);
		if (ret < 0) {
			N3K_MGMT_LOG(FLR_DEFAULT, ERR, "Failed to append flow dump blocks");
			return ret;
		}
		break;
	}
	return ret;
}

int
n3k_mgmt_flr_command_to_pkt(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flr_command *cmd, uint16_t packet_id,
	struct rte_mbuf *pkt)
{
	int ret;
	ret = n3k_mgmt_flr_init_pkt(pkt, packet_id);
	if (ret < 0) {
		N3K_MGMT_LOG(FLR_DEFAULT, ERR, "Failed to init packet");
		return ret;
	}
	ret = append_command_blocks(hw, cmd, pkt);
	if (ret < 0)
		return ret;
	ret = n3k_mgmt_flr_add_icv(pkt);
	if (ret < 0) {
		N3K_MGMT_LOG(FLR_DEFAULT, ERR, "Failed to append ICV");
		return ret;
	}
	return 0;
}

int
n3k_mgmt_flr_command_pack_to_pkt(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flr_command_pack *pack, struct rte_mbuf *pkt)
{
	int ret;
	ret = n3k_mgmt_flr_init_pkt(pkt, pack->metadata.packet_id);
	if (ret < 0) {
		N3K_MGMT_LOG(FLR_DEFAULT, ERR, "Failed to init packet");
		return ret;
	}
	size_t i;
	for (i = 0; i < pack->metadata.cmd_cnt; ++i) {
		ret = append_command_blocks(hw, pack->cmds[i], pkt);
		if (ret < 0)
			return ret;
	}
	ret = n3k_mgmt_flr_add_icv(pkt);
	if (ret < 0) {
		N3K_MGMT_LOG(FLR_DEFAULT, ERR, "Failed to append ICV");
		return ret;
	}
	return 0;
}

static const char *
n3k_mgmt_flr_command_type_str(enum n3k_mgmt_flr_command_type type)
{
	switch (type) {
	case N3K_MGMT_FLR_COMMAND_FLOW_CREATE: return "FCREATE";
	case N3K_MGMT_FLR_COMMAND_FLOW_DESTROY: return "FDESTROY";
	case N3K_MGMT_FLR_COMMAND_FLOW_QUERY: return "FQUERY";
	case N3K_MGMT_FLR_COMMAND_FLOW_STATS_UPDATE: return "FSTATS";
	case N3K_MGMT_FLR_COMMAND_FLOW_HANDLE: return "FHANDLE";
	case N3K_MGMT_FLR_COMMAND_FLOW_DUMP: return "FDUMP";
	case N3K_MGMT_FLR_COMMAND_READ_REG_32: return "REG32";
	}
	return "INVALID_COMMAND_TYPE";
}

int
n3k_mgmt_flr_command_handle_response(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command *cmd,
	const struct n3k_mgmt_flr_parsed_block_range *parsed_cmd)
{
	bool retry_limit_reached = cmd->metadata.processed_count == N3K_MGMT_FLR_COMMAND_MAX_FAILURES;
	int ret = -EINVAL;

	// Response handlers should return -EAGAIN when command should be
	// retried. Passed parsed_cmd may be NULL which means that command
	// was timeouted.
	switch (cmd->metadata.type) {
	case N3K_MGMT_FLR_COMMAND_FLOW_CREATE:
		ret = n3k_mgmt_flr_flow_tbl_add_handle_response(hw,
			&cmd->flow_create, parsed_cmd, retry_limit_reached);
		break;
	case N3K_MGMT_FLR_COMMAND_FLOW_DESTROY:
		ret = n3k_mgmt_flr_flow_tbl_del_handle_response(hw,
			&cmd->flow_destroy, parsed_cmd, retry_limit_reached);
		break;
	case N3K_MGMT_FLR_COMMAND_FLOW_QUERY:
		ret = n3k_mgmt_flr_flow_tracker_get_handle_response(hw,
			&cmd->flow_query, parsed_cmd, retry_limit_reached);
		break;
	case N3K_MGMT_FLR_COMMAND_FLOW_STATS_UPDATE:
		ret = n3k_mgmt_flr_flow_tracker_update_handle_response(hw,
			&cmd->flow_stats_update, parsed_cmd, retry_limit_reached);
		break;
	case N3K_MGMT_FLR_COMMAND_READ_REG_32:
		ret = n3k_mgmt_flr_read_reg_32_handle_response(hw,
			&cmd->reg_32_read, parsed_cmd, retry_limit_reached);
		break;
	case N3K_MGMT_FLR_COMMAND_FLOW_HANDLE:
		ret = -ENOTSUP;
		break;
	case N3K_MGMT_FLR_COMMAND_FLOW_DUMP:
		ret = n3k_mgmt_flr_flow_tbl_dump_handle_response(hw,
			&cmd->flow_dump, parsed_cmd, retry_limit_reached);
		break;
	}

	if (!retry_limit_reached && ret == -EAGAIN) {
		N3K_MGMT_LOG(FLR_DEFAULT, WARNING,
			"Retrying FLR command: %s(pc=%" PRIu16 ")",
			n3k_mgmt_flr_command_type_str(cmd->metadata.type),
			cmd->metadata.processed_count);
		n3k_mgmt_flr_cmd_queue_push_command(hw, cmd);
		return 0;
	}

	return ret;
}

int
n3k_mgmt_flr_command_handle_response_cmd_pack(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_pack *pack,
	const struct n3k_mgmt_flr_parsed_pkt *parsed_pkt)
{
	size_t i;
	int part_ret, ret = 0;
	if (parsed_pkt == NULL) {
		for (i = 0; i < pack->metadata.cmd_cnt; ++i) {
			part_ret = n3k_mgmt_flr_command_handle_response(hw, pack->cmds[i], NULL);
			if (part_ret < 0) {
				N3K_MGMT_LOG(FLR_DEFAULT, ERR,
					"Failed to handle timeout for command %zu of cmd pack: packet_id=%" PRIu16 ", ret=%d",
					i, pack->metadata.packet_id, part_ret);
				ret = part_ret;
			}
		}
		return ret;
	}
	size_t cur_block = 0;
	for (i = 0; i < pack->metadata.cmd_cnt; ++i) {
		struct n3k_mgmt_flr_parsed_block_range parsed_cmd;
		struct n3k_mgmt_flr_command *cmd = pack->cmds[i];

		size_t j;
		parsed_cmd.blocks_num = n3k_mgmt_flr_command_blocks_num(cmd);
		for (j = 0; j < parsed_cmd.blocks_num; ++j)
			parsed_cmd.blocks[j] = parsed_pkt->blocks[j + cur_block];

		part_ret = n3k_mgmt_flr_command_handle_response(hw, cmd, &parsed_cmd);
		if (part_ret < 0) {
			N3K_MGMT_LOG(FLR_DEFAULT, ERR,
				"Failed to handle response for command %zu of cmd pack: packet_id=%" PRIu16 ", ret=%d",
				i, pack->metadata.packet_id, part_ret);
			ret = part_ret;
		}

		cur_block += parsed_cmd.blocks_num;
	}
	return ret;
}

bool
n3k_mgmt_flr_command_try_add_cmd_to_pack(struct n3k_mgmt_flr_command_pack *pack,
	struct n3k_mgmt_flr_command *cmd)
{
	size_t cmd_blocks_cnt = n3k_mgmt_flr_command_blocks_num(cmd);
	if (pack->metadata.blocks_cnt + cmd_blocks_cnt <=
			N3K_MGMT_FLR_MAX_BLOCKS_NUM) {
		pack->cmds[pack->metadata.cmd_cnt] = cmd;
		pack->metadata.cmd_cnt++;
		pack->metadata.blocks_cnt += cmd_blocks_cnt;
		return true;
	}
	return false;
}

void
n3k_mgmt_flr_command_metadata_dump(struct n3k_mgmt_flr_command *cmd)
{
	printf("%s(pc=%" PRIu16 ")",
		n3k_mgmt_flr_command_type_str(cmd->metadata.type),
		cmd->metadata.processed_count);
}

void
n3k_mgmt_flr_command_pack_dump(struct n3k_mgmt_flr_command_pack *pack)
{
	size_t i;
	printf("blocks_cnt=%zu, cmds=", pack->metadata.blocks_cnt);
	for (i = 0; i < pack->metadata.cmd_cnt; ++i) {
		n3k_mgmt_flr_command_metadata_dump(pack->cmds[i]);
		if (i + 1 < pack->metadata.cmd_cnt)
			printf(",");
	}
	printf("\n");
}
