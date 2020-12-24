/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_recirc_id_allocator.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_log.h"

#include "../rte_pmd_n3k_malloc.h"

#define MAX_RECIRC_IDX (1048576-1) /* 2^20 - 1, it will give exactly
                                      1048576 (2^20 slots in the ring) */

struct n3k_mgmt_recirc_id_allocator_internal {
	struct rte_ring *recirc_id_ring;
};

static int
init_recirc_id_ring(struct n3k_mgmt_hw *hw)
{
	struct rte_ring **recirc_id_ring;
	uint32_t i;
	int ret;

	recirc_id_ring = &hw->recirc_id_alloc_internal->recirc_id_ring;
	*recirc_id_ring = rte_ring_create(
		"n3k_recirc_id_allocator", MAX_RECIRC_IDX, 0, RING_F_EXACT_SZ);
	if (*recirc_id_ring == NULL) {
		N3K_MGMT_FLOW_LOG(ERR, "Failed to allocate rte_ring");
		return -rte_errno;
	}

	for (i = 0; i < MAX_RECIRC_IDX; ++i) {
		uintptr_t val_as_ptr;

		val_as_ptr = i;
		ret = rte_ring_enqueue(*recirc_id_ring, (void *)val_as_ptr);
		if (ret < 0) {
			N3K_MGMT_FLOW_LOG(ERR, "Failed to enqueue recirc_id to ring");
			goto free_ring;
		}
	}

	return 0;

free_ring:
	rte_ring_free(*recirc_id_ring);
	return ret;
}

int
n3k_mgmt_recirc_id_allocator_init(struct n3k_mgmt_hw *hw)
{
	int ret;
	N3K_MGMT_FLOW_LOG(INFO, "Initializing recirc_id allocator");

	if (hw->recirc_id_alloc_internal) {
		N3K_MGMT_FLOW_LOG(ERR, "recirc_id allocator already initialized");
		return -EINVAL;
	}

	hw->recirc_id_alloc_internal = n3k_zmalloc("n3k_recirc_id_allocator",
		sizeof(*hw->recirc_id_alloc_internal), 0);
	if (!hw->recirc_id_alloc_internal) {
		N3K_MGMT_FLOW_LOG(ERR, "Failed to allocate recirc_id_alloc_internal");
		return -ENOMEM;
	}

	ret = init_recirc_id_ring(hw);
	if (ret < 0) {
		N3K_MGMT_FLOW_LOG(ERR, "Failed to init recirc_id ring");
		goto free_allocator;
	}

	return 0;

free_allocator:
	n3k_free(hw->recirc_id_alloc_internal);
	hw->recirc_id_alloc_internal = NULL;
	return ret;
}

int
n3k_mgmt_recirc_id_alloc(struct n3k_mgmt_hw *hw, uint32_t *recirc_id)
{
	struct rte_ring *recirc_id_ring = hw->recirc_id_alloc_internal->recirc_id_ring;
	int ret;
	uintptr_t val_as_ptr;

	ret = rte_ring_dequeue(recirc_id_ring, (void **)&val_as_ptr);
	if (ret < 0) {
		N3K_MGMT_FLOW_LOG(ERR, "Failed to dequeue from recirc_id ring");
		return ret;
	}
	*recirc_id = val_as_ptr;

	return 0;
}

int
n3k_mgmt_recirc_id_free(struct n3k_mgmt_hw *hw, uint32_t recirc_id)
{
	struct rte_ring *recirc_id_ring = hw->recirc_id_alloc_internal->recirc_id_ring;
	int ret;
	uintptr_t val_as_ptr;

	val_as_ptr = recirc_id;
	ret = rte_ring_enqueue(recirc_id_ring, (void *)val_as_ptr);
	if (ret < 0) {
		N3K_MGMT_FLOW_LOG(ERR, "Failed to enqueue recirc_id to ring");
		return ret;
	}

	return 0;
}
