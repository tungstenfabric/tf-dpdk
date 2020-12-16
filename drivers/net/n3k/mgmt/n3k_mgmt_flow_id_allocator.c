/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_flow_id_allocator.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_flow_tbl.h"

#include "../rte_pmd_n3k_malloc.h"

struct n3k_mgmt_flow_id_allocator_internal {
	struct rte_ring *flow_id_ring;
};

static int
init_flow_id_ring(struct n3k_mgmt_hw *hw)
{
	struct rte_ring **flow_id_ring;
	uint32_t max_handles;
	uint32_t min_handle;
	uint32_t i;
	int ret;

	max_handles = n3k_mgmt_flow_tbl_get_max_flow_id(hw);
	min_handle = n3k_mgmt_flow_tbl_get_min_flow_id(hw);

	flow_id_ring = &hw->flow_id_alloc_internal->flow_id_ring;
	*flow_id_ring = rte_ring_create(
		"n3k_flow_id_allocator", max_handles - min_handle, 0, RING_F_EXACT_SZ);
	if (*flow_id_ring == NULL) {
		N3K_MGMT_LOG(ERR, "Failed to allocate rte_ring");
		return -rte_errno;
	}

	for (i = min_handle; i < max_handles; ++i) {
		uintptr_t val_as_ptr;
		// TODO(n3k): uncomment when added support for adding flows
		//            using flow id
		// if (hw->insert_type == N3K_MGMT_HW_INSERT_TYPE_FLR) {
		// 	uint32_t flow_id;
		// 	ret = n3k_mgmt_flow_tbl_alloc_next_flow_id_nolock(hw, &flow_id);
		// 	if (ret)
		// 		goto free_ring;
		// 	val_as_ptr = flow_id;
		// } else {
			val_as_ptr = i;
		// }
		ret = rte_ring_enqueue(*flow_id_ring, (void *)val_as_ptr);
		if (ret < 0) {
			N3K_MGMT_LOG(ERR, "Failed to enqueue flow_id to ring");
			goto free_ring;
		}
	}

	return 0;

free_ring:
	// TODO(n3k): free ids to hw
	rte_ring_free(*flow_id_ring);
	return ret;
}

int
n3k_mgmt_flow_id_allocator_init(struct n3k_mgmt_hw *hw)
{
	int ret;
	N3K_MGMT_LOG(INFO, "Initializing flow_id allocator");

	if (hw->flow_id_alloc_internal) {
		N3K_MGMT_LOG(ERR, "flow_id allocator already initialized");
		return -EINVAL;
	}

	hw->flow_id_alloc_internal = n3k_zmalloc("n3k_flow_id_allocator",
		sizeof(*hw->flow_id_alloc_internal), 0);
	if (!hw->flow_id_alloc_internal) {
		N3K_MGMT_LOG(ERR, "Failed to allocate flow_id_alloc_internal");
		return -ENOMEM;
	}

	ret = init_flow_id_ring(hw);
	if (ret < 0) {
		N3K_MGMT_LOG(ERR, "Failed to init flow_id ring");
		goto free_allocator;
	}

	return 0;

free_allocator:
	n3k_free(hw->flow_id_alloc_internal);
	hw->flow_id_alloc_internal = NULL;
	return ret;
}

int
n3k_mgmt_flow_id_alloc(struct n3k_mgmt_hw *hw, uint32_t *handle)
{
	struct rte_ring *flow_id_ring = hw->flow_id_alloc_internal->flow_id_ring;
	int ret;
	uintptr_t val_as_ptr;

	ret = rte_ring_dequeue(flow_id_ring, (void **)&val_as_ptr);
	if (ret < 0) {
		N3K_MGMT_LOG(ERR, "Failed to dequeue from flow_id ring");
		return ret;
	}
	*handle = val_as_ptr;

	return 0;
}

int
n3k_mgmt_flow_id_free(struct n3k_mgmt_hw *hw, uint32_t handle)
{
	struct rte_ring *flow_id_ring = hw->flow_id_alloc_internal->flow_id_ring;
	int ret;
	uintptr_t val_as_ptr;

	val_as_ptr = handle;
	ret = rte_ring_enqueue(flow_id_ring, (void *)val_as_ptr);
	if (ret < 0) {
		N3K_MGMT_LOG(ERR, "Failed to enqueue flow_id to ring");
		return ret;
