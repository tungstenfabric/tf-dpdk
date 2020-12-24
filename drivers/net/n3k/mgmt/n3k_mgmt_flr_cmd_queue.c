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

#include "n3k_mgmt_flr_cmd_queue.h"
#include "n3k_mgmt_flr_command.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_log.h"

#include "../rte_pmd_n3k_malloc.h"

#include <time.h>

enum { WAITING_COMMANDS_MAX_COUNT = 2 * 1024 * 1024 };
enum { COMMAND_TIMEOUT_SEC = 1 };

struct n3k_mgmt_flr_cmd_queue {
	uint16_t packet_id;
	struct rte_ring *waiting_commands;
	struct n3k_mgmt_flr_command *
		processed_commands[N3K_MGMT_FLR_COMMAND_PROCESSED_MAX_COUNT];
	rte_spinlock_t processed_commands_lock;
};

static int
init_waiting_commands_ring(struct n3k_mgmt_flr_cmd_queue *queue)
{
	queue->waiting_commands = rte_ring_create(
		"n3k_flr_ring", WAITING_COMMANDS_MAX_COUNT, 0, RING_F_SC_DEQ);
	if (!queue->waiting_commands) {
		N3K_MGMT_LOG(ERR, "Failed to allocate rte_ring");
		return -rte_errno;
	}

	return 0;
}

int
n3k_mgmt_flr_cmd_queue_init(struct n3k_mgmt_hw *hw)
{
	int ret;
	N3K_MGMT_LOG(INFO, "Initializing FLR command queue");

	if (hw->flr_cmd_queue) {
		N3K_MGMT_LOG(ERR, "FLR command queue already initialized");
		return -EINVAL;
	}

	hw->flr_cmd_queue = n3k_zmalloc(
		"n3k_flr_cmd_queue", sizeof(*hw->flr_cmd_queue), 0);
	if (!hw->flr_cmd_queue) {
		N3K_MGMT_LOG(ERR, "Failed to allocate FLR command queue");
		return -ENOMEM;
	}

	ret = init_waiting_commands_ring(hw->flr_cmd_queue);
	if (ret) {
		n3k_free(hw->flr_cmd_queue);
		hw->flr_cmd_queue = NULL;
		return ret;
	}

	rte_spinlock_init(&hw->flr_cmd_queue->processed_commands_lock);
	hw->flr_cmd_queue->packet_id = 0;

	return 0;
}

int
n3k_mgmt_flr_cmd_queue_push_command(
	struct n3k_mgmt_hw *hw, struct n3k_mgmt_flr_command *cmd)
{
	return rte_ring_enqueue(hw->flr_cmd_queue->waiting_commands, cmd);
}

struct n3k_mgmt_flr_command *
n3k_mgmt_flr_cmd_queue_process_command(struct n3k_mgmt_hw *hw)
{
	uint16_t i;
	struct n3k_mgmt_flr_command *cmd = NULL;
	struct n3k_mgmt_flr_cmd_queue *queue = hw->flr_cmd_queue;

	if (rte_ring_empty(queue->waiting_commands))
		return NULL;

	rte_spinlock_lock(&queue->processed_commands_lock);

	for (i = 0; i < N3K_MGMT_FLR_COMMAND_PROCESSED_MAX_COUNT; ++i) {
		if (queue->processed_commands[i] == NULL)
			break;
	}

	if (i != N3K_MGMT_FLR_COMMAND_PROCESSED_MAX_COUNT) {
		/* If the ring is empty (unlikely), the cmd will be NULL */
		rte_ring_dequeue(queue->waiting_commands, (void **)&cmd);

		if (likely(cmd)) {
			queue->processed_commands[i] = cmd;
			n3k_mgmt_flr_command_mark_processed(cmd,
				hw->flr_cmd_queue->packet_id++);
		}
	}

	rte_spinlock_unlock(&queue->processed_commands_lock);

	return cmd;
}

struct n3k_mgmt_flr_command *
n3k_mgmt_flr_cmd_queue_pop_processed_command(
	struct n3k_mgmt_hw *hw, uint16_t cmd_id)
{
	struct n3k_mgmt_flr_command *cmd;
	struct n3k_mgmt_flr_cmd_queue *queue = hw->flr_cmd_queue;
	uint16_t i;

	rte_spinlock_lock(&queue->processed_commands_lock);

	for (i = 0; i < N3K_MGMT_FLR_COMMAND_PROCESSED_MAX_COUNT; ++i) {
		if (queue->processed_commands[i] &&
			queue->processed_commands[i]->metadata.packet_id == cmd_id)
			break;
	}

	if (i != N3K_MGMT_FLR_COMMAND_PROCESSED_MAX_COUNT) {
		cmd = queue->processed_commands[i];
		queue->processed_commands[i] = NULL;
	} else {
		cmd = NULL;
	}

	rte_spinlock_unlock(&queue->processed_commands_lock);

	return cmd;
}

static inline void
timespec_diff(const struct timespec *a, const struct timespec *b,
	struct timespec *result)
{
	result->tv_sec  = a->tv_sec  - b->tv_sec;
	result->tv_nsec = a->tv_nsec - b->tv_nsec;
	if (result->tv_nsec < 0) {
		--result->tv_sec;
		result->tv_nsec += 1000000000L;
	}
}


struct n3k_mgmt_flr_command *
n3k_mgmt_flr_cmd_queue_pop_timeouted_command(struct n3k_mgmt_hw *hw)
{
	struct n3k_mgmt_flr_command *cmd;
	struct n3k_mgmt_flr_cmd_queue *queue = hw->flr_cmd_queue;
	struct timespec cur_time;
	uint16_t i;

	rte_spinlock_lock(&queue->processed_commands_lock);

	clock_gettime(CLOCK_MONOTONIC_RAW, &cur_time);

	for (i = 0; i < N3K_MGMT_FLR_COMMAND_PROCESSED_MAX_COUNT; ++i) {
		if (queue->processed_commands[i]) {
			struct timespec diff_time;
			timespec_diff(&cur_time,
				&queue->processed_commands[i]->metadata.processed_time,
				&diff_time);
			if (diff_time.tv_sec >= COMMAND_TIMEOUT_SEC)
				break;
		}
	}

	if (i != N3K_MGMT_FLR_COMMAND_PROCESSED_MAX_COUNT) {
		cmd = queue->processed_commands[i];
		queue->processed_commands[i] = NULL;
	} else {
		cmd = NULL;
	}

	rte_spinlock_unlock(&queue->processed_commands_lock);

	return cmd;
}
