/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_flow_tbl_handles.h"
#include "n3k_mgmt_flr_command.h"
#include "n3k_mgmt_flr_flow_stats_cmd_src.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_log.h"

#include "common/n3k_time_utils.h"

#include <rte_pmd_n3k_malloc.h>
#include <rte_spinlock.h>

enum { SIZE_OF_PACK = 8 * 7 };
enum { CMD_WAIT_TIME_SEC = (30000 * SIZE_OF_PACK) / 1000000000 };
enum { CMD_WAIT_TIME_NSEC = (30000 * SIZE_OF_PACK) % 1000000000 };

static const struct timespec cmd_wait_time = {
	.tv_sec = CMD_WAIT_TIME_SEC,
	.tv_nsec = CMD_WAIT_TIME_NSEC,
};

struct n3k_mgmt_flr_flow_stats_cmd_src {
	struct n3k_mgmt_flow_tbl_handle *cur_handle;
	struct timespec last_pack_sent;
	size_t rem_cmd_in_pack;
	rte_spinlock_t lock;
};

int
n3k_mgmt_flr_flow_stats_cmd_src_init(struct n3k_mgmt_hw *hw)
{
	N3K_MGMT_LOG(DEFAULT, INFO, "Initializing FLR flow stats cmd source");

	if (hw->flr_flow_stats_cmd_src) {
		N3K_MGMT_LOG(DEFAULT, ERR, "FLR flow stats cmd source already initialized");
		return -EINVAL;
	}

	hw->flr_flow_stats_cmd_src = n3k_zmalloc("n3k_flow_stats_cmd_src",
		sizeof(*hw->flr_flow_stats_cmd_src), 0);
	if (!hw->flr_flow_stats_cmd_src) {
		N3K_MGMT_LOG(DEFAULT, ERR, "Failed to allocate FLR flow stats cmd source");
		return -ENOMEM;
	}

	rte_spinlock_init(&hw->flr_flow_stats_cmd_src->lock);

	clock_gettime(CLOCK_MONOTONIC_RAW, &hw->flr_flow_stats_cmd_src->last_pack_sent);
	hw->flr_flow_stats_cmd_src->rem_cmd_in_pack = SIZE_OF_PACK;

	return 0;
}

static struct n3k_mgmt_flr_command *
alloc_flow_stats_update_cmd(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle)
{
	struct n3k_mgmt_flr_command *cmd;
	cmd = n3k_mgmt_flr_command_alloc(hw, N3K_MGMT_FLR_COMMAND_FLOW_STATS_UPDATE);
	if (cmd != NULL)
		cmd->flow_stats_update.handle = handle;
	return cmd;
}

struct n3k_mgmt_flr_command *
n3k_mgmt_flr_flow_stats_cmd_src_pop(struct n3k_mgmt_hw *hw)
{
	struct n3k_mgmt_flr_flow_stats_cmd_src *cmd_src = hw->flr_flow_stats_cmd_src;
	struct n3k_mgmt_flr_command *cmd = NULL;
	struct timespec diff_time;
	int ret;

	rte_spinlock_lock(&cmd_src->lock);

	if (cmd_src->rem_cmd_in_pack == 0) {
		struct timespec cur_time;
		clock_gettime(CLOCK_MONOTONIC_RAW, &cur_time);
		n3k_timespec_diff(&cur_time, &cmd_src->last_pack_sent, &diff_time);
		if (!n3k_is_timespec_lesser(&cmd_wait_time, &diff_time))
			goto release_lock;
		cmd_src->last_pack_sent = cur_time;
		cmd_src->rem_cmd_in_pack = SIZE_OF_PACK;
	}

	if (cmd_src->cur_handle) {
		ret = n3k_mgmt_flow_tbl_next_no_mirror_handle(hw,
			&cmd_src->cur_handle);
		if (ret < 0)
			cmd_src->cur_handle = NULL;
		else
			goto set_cmd;
	}

	if (cmd_src->cur_handle == NULL) {
		ret = n3k_mgmt_flow_tbl_first_no_mirror_handle(hw,
			&cmd_src->cur_handle);
		if (ret < 0)
			goto release_lock;
		goto set_cmd;
	}

set_cmd:
	cmd = alloc_flow_stats_update_cmd(hw, cmd_src->cur_handle);
	if (cmd != NULL)
		--cmd_src->rem_cmd_in_pack;
release_lock:
	rte_spinlock_unlock(&cmd_src->lock);
	return cmd;
}
