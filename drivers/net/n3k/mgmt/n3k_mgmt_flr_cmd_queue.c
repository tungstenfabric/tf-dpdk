/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include <rte_common.h>
#include <rte_ring.h>
#include <rte_spinlock.h>
#include <rte_pmd_n3k_malloc.h>
#include <time.h>

#include "n3k_mgmt_flr_cmd_queue.h"
#include "n3k_mgmt_flr_command.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_log.h"

enum { WAITING_COMMANDS_MAX_COUNT = 2 * 1024 * 1024 };

/* This is just a rte_ring ptr to avoid slow double ptr dereference */
struct n3k_mgmt_flr_cmd_queue;

static int
init_waiting_commands_ring(struct rte_ring **queue)
{
	*queue = rte_ring_create(
		"n3k_flr_ring", WAITING_COMMANDS_MAX_COUNT, 0, RING_F_SC_DEQ);
	if (!*queue) {
		N3K_MGMT_LOG(DEFAULT, ERR, "Failed to allocate rte_ring");
		return -rte_errno;
	}

	return 0;
}

int
n3k_mgmt_flr_cmd_queue_init(struct n3k_mgmt_hw *hw)
{
	struct rte_ring **queue;
	int ret;
	N3K_MGMT_LOG(DEFAULT, INFO, "Initializing FLR command queue");

	if (hw->flr_cmd_queue) {
		N3K_MGMT_LOG(DEFAULT, ERR, "FLR command queue already initialized");
		return -EINVAL;
	}

	queue = (struct rte_ring **)&hw->flr_cmd_queue;
	*queue = n3k_zmalloc("n3k_flr_cmd_queue", sizeof(**queue), 0);
	if (!*queue) {
		N3K_MGMT_LOG(DEFAULT, ERR, "Failed to allocate FLR command queue");
		return -ENOMEM;
	}

	ret = init_waiting_commands_ring(queue);
	if (ret) {
		n3k_free(hw->flr_cmd_queue);
		hw->flr_cmd_queue = NULL;
		return ret;
	}

	return 0;
}

int
n3k_mgmt_flr_cmd_queue_push_command(
	struct n3k_mgmt_hw *hw, struct n3k_mgmt_flr_command *cmd)
{
	return rte_ring_enqueue((struct rte_ring *)hw->flr_cmd_queue, cmd);
}

struct n3k_mgmt_flr_command *
n3k_mgmt_flr_cmd_queue_pop(struct n3k_mgmt_hw *hw)
{
	struct n3k_mgmt_flr_command *cmd = NULL;
	struct n3k_mgmt_flr_cmd_queue *queue = hw->flr_cmd_queue;
	int ret;
	ret = rte_ring_dequeue((struct rte_ring *)queue, (void **)&cmd);
	if (ret < 0)
		return NULL;
	return cmd;
}
