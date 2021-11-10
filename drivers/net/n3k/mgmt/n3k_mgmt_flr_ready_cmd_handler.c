/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_flr_ready_cmd_handler.h"
#include "n3k_mgmt_log.h"

#include "common/n3k_time_utils.h"

#include <rte_pmd_n3k_malloc.h>

enum { MAX_CMD_SRC = 10 };
enum { COMMAND_TIMEOUT_SEC = 1 };

struct n3k_mgmt_flr_rdy_cmd_hdlr {
	uint16_t packet_id;
	int cmd_src_cnt;
	int current_cmd_src;
	n3k_mgmt_flr_rdy_cmd_hdlr_cmd_src_t cmd_srcs[MAX_CMD_SRC];
	struct n3k_mgmt_flr_command_pack
		processed_cmd_packs[N3K_MGMT_FLR_COMMAND_PROCESSED_MAX_COUNT];
	struct n3k_mgmt_flr_command *saved_cmd; // the last command that didn't fit into the previous pack
	rte_spinlock_t lock;
};

int
n3k_mgmt_flr_rdy_cmd_hdlr_init(struct n3k_mgmt_hw *hw)
{
	N3K_MGMT_LOG(DEFAULT, INFO, "Initializing FLR ready command handler");

	if (hw->flr_rdy_cmd_hdlr) {
		N3K_MGMT_LOG(DEFAULT, ERR, "FLR ready command handler already initialized");
		return -EINVAL;
	}

	hw->flr_rdy_cmd_hdlr = n3k_zmalloc(
		"n3k_flr_rdy_cmd_hdlr", sizeof(*hw->flr_rdy_cmd_hdlr), 0);
	if (!hw->flr_rdy_cmd_hdlr) {
		N3K_MGMT_LOG(DEFAULT, ERR, "Failed to allocate FLR ready command handler");
		return -ENOMEM;
	}

	rte_spinlock_init(&hw->flr_rdy_cmd_hdlr->lock);

	return 0;
}

int
n3k_mgmt_flr_rdy_cmd_hdlr_register_src(struct n3k_mgmt_hw *hw,
	n3k_mgmt_flr_rdy_cmd_hdlr_cmd_src_t cmd_src)
{
	struct n3k_mgmt_flr_rdy_cmd_hdlr *cmd_hdlr = hw->flr_rdy_cmd_hdlr;
	rte_spinlock_lock(&cmd_hdlr->lock);
	if (cmd_hdlr->cmd_src_cnt == MAX_CMD_SRC) {
		rte_spinlock_unlock(&cmd_hdlr->lock);
		return -ENOSPC;
	}
	cmd_hdlr->cmd_srcs[cmd_hdlr->cmd_src_cnt++] = cmd_src;
	rte_spinlock_unlock(&cmd_hdlr->lock);
	return 0;
}

static struct n3k_mgmt_flr_command *
get_cmd_from_cmd_srcs(struct n3k_mgmt_hw *hw)
{
	struct n3k_mgmt_flr_rdy_cmd_hdlr *cmd_hdlr = hw->flr_rdy_cmd_hdlr;
	int i;
	for (i = 0; i < cmd_hdlr->cmd_src_cnt; ++i) {
		struct n3k_mgmt_flr_command *cmd =
			cmd_hdlr->cmd_srcs[cmd_hdlr->current_cmd_src](hw);

		if (cmd_hdlr->current_cmd_src == cmd_hdlr->cmd_src_cnt - 1)
			cmd_hdlr->current_cmd_src = 0;
		else
			++cmd_hdlr->current_cmd_src;

		if (cmd)
			return cmd;
	}
	return NULL;
}

struct n3k_mgmt_flr_command_pack
n3k_mgmt_flr_rdy_cmd_hdlr_process_cmd_pack(struct n3k_mgmt_hw *hw)
{
	struct n3k_mgmt_flr_rdy_cmd_hdlr *cmd_hdlr = hw->flr_rdy_cmd_hdlr;
	struct n3k_mgmt_flr_command_pack pack;
	uint16_t i;

	memset(&pack, 0, sizeof(pack));

	rte_spinlock_lock(&cmd_hdlr->lock);

	for (i = 0; i < N3K_MGMT_FLR_COMMAND_PROCESSED_MAX_COUNT; ++i)
		if (cmd_hdlr->processed_cmd_packs[i].metadata.cmd_cnt == 0)
			break;

	if (i != N3K_MGMT_FLR_COMMAND_PROCESSED_MAX_COUNT) {
		while (true) {
			struct n3k_mgmt_flr_command *cmd;
			if (cmd_hdlr->saved_cmd != NULL) {
				cmd = cmd_hdlr->saved_cmd;
				cmd_hdlr->saved_cmd = NULL;
			} else {
				cmd = get_cmd_from_cmd_srcs(hw);
			}
			if (cmd) {
				if (n3k_mgmt_flr_command_try_add_cmd_to_pack(&pack, cmd)) {
					n3k_mgmt_flr_command_mark_processed(cmd);
				} else {
					cmd_hdlr->saved_cmd = cmd;
					break;
				}
			} else {
				break;
			}
		}
		if (pack.metadata.cmd_cnt > 0) {
			n3k_mgmt_flr_command_pack_mark_processed(&pack, cmd_hdlr->packet_id++);
			cmd_hdlr->processed_cmd_packs[i] = pack;
		}
	}

	rte_spinlock_unlock(&cmd_hdlr->lock);

	return pack;
}

struct n3k_mgmt_flr_command_pack
n3k_mgmt_flr_rdy_cmd_hdlr_pop_processed_cmd_pack(struct n3k_mgmt_hw *hw, uint16_t cmd_id)
{
	struct n3k_mgmt_flr_rdy_cmd_hdlr *cmd_hdlr = hw->flr_rdy_cmd_hdlr;
	struct n3k_mgmt_flr_command_pack pack;
	uint16_t i;

	rte_spinlock_lock(&cmd_hdlr->lock);

	for (i = 0; i < N3K_MGMT_FLR_COMMAND_PROCESSED_MAX_COUNT; ++i) {
		if (cmd_hdlr->processed_cmd_packs[i].metadata.cmd_cnt > 0 &&
			cmd_hdlr->processed_cmd_packs[i].metadata.packet_id == cmd_id)
			break;
	}

	if (i != N3K_MGMT_FLR_COMMAND_PROCESSED_MAX_COUNT) {
		pack = cmd_hdlr->processed_cmd_packs[i];
		cmd_hdlr->processed_cmd_packs[i].metadata.cmd_cnt = 0;
	} else {
		memset(&pack, 0, sizeof(pack));
	}

	rte_spinlock_unlock(&cmd_hdlr->lock);

	return pack;
}

struct n3k_mgmt_flr_command_pack
n3k_mgmt_flr_rdy_cmd_hdlr_pop_timeouted_cmd_pack(struct n3k_mgmt_hw *hw)
{
	struct n3k_mgmt_flr_rdy_cmd_hdlr *cmd_hdlr = hw->flr_rdy_cmd_hdlr;
	struct n3k_mgmt_flr_command_pack pack;
	struct timespec cur_time;
	uint16_t i;

	rte_spinlock_lock(&cmd_hdlr->lock);

	clock_gettime(CLOCK_MONOTONIC_RAW, &cur_time);

	for (i = 0; i < N3K_MGMT_FLR_COMMAND_PROCESSED_MAX_COUNT; ++i) {
		if (cmd_hdlr->processed_cmd_packs[i].metadata.cmd_cnt > 0) {
			struct timespec diff_time;
			n3k_timespec_diff(&cur_time,
				&cmd_hdlr->processed_cmd_packs[i].metadata.processed_time,
				&diff_time);
			if (diff_time.tv_sec >= COMMAND_TIMEOUT_SEC)
				break;
		}
	}

	memset(&pack, 0, sizeof(pack));
	if (i != N3K_MGMT_FLR_COMMAND_PROCESSED_MAX_COUNT) {
		pack = cmd_hdlr->processed_cmd_packs[i];
		cmd_hdlr->processed_cmd_packs[i].metadata.cmd_cnt = 0;
	}

	rte_spinlock_unlock(&cmd_hdlr->lock);

	return pack;
}
