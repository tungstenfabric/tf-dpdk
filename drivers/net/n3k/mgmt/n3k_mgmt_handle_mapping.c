/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_flow_tbl.h"
#include "../rte_pmd_n3k_malloc.h"
#include "n3k_mgmt_handle_mapping.h"

#include <rte_spinlock.h>

#include <stdint.h>

struct handle_mapping {
	uint32_t mapped_handle;
	bool present;
};

struct n3k_mgmt_handle_mapping_internal {
	uint32_t max_handles;
	struct handle_mapping *local_to_hw;
	rte_spinlock_t lock;
};

int
n3k_mgmt_handle_mapping_init(struct n3k_mgmt_hw *hw)
{
	uint32_t max_handles;
	int ret;

	N3K_MGMT_FLOW_LOG(INFO, "Initializing handle mapping");

	if (hw->handle_map_internal) {
		N3K_MGMT_FLOW_LOG(ERR, "Handle mapping already initialized");
		return -EINVAL;
	}

	hw->handle_map_internal = n3k_zmalloc("handle_mapping",
		sizeof(*hw->handle_map_internal), 0);
	if (!hw->handle_map_internal) {
		N3K_MGMT_FLOW_LOG(ERR, "Failed to allocate handle_map_internal");
		return -ENOMEM;
	}

	max_handles = n3k_mgmt_flow_tbl_get_max_flow_id(hw);
	hw->handle_map_internal->local_to_hw = n3k_zmalloc("handle_mapping",
		max_handles * sizeof(*hw->handle_map_internal->local_to_hw), 0);
	if (!hw->handle_map_internal->local_to_hw) {
		N3K_MGMT_FLOW_LOG(ERR, "Failed to allocate local_to_hw array");
		ret = -ENOMEM;
		goto free_handles;
	}
	hw->handle_map_internal->max_handles = max_handles;

	rte_spinlock_init(&hw->handle_map_internal->lock);

	return 0;

free_handles:
	n3k_free(hw->handle_map_internal);
	hw->handle_map_internal = NULL;
	return ret;
}

int
n3k_mgmt_map_local_to_hw_handle(struct n3k_mgmt_hw *hw,
	uint32_t local_handle, uint32_t hw_handle)
{
	struct n3k_mgmt_handle_mapping_internal *handle_map_internal =
		hw->handle_map_internal;
	if (local_handle >= handle_map_internal->max_handles)
		return -EINVAL;
	rte_spinlock_lock(&handle_map_internal->lock);
	if (handle_map_internal->local_to_hw[local_handle].present) {
		rte_spinlock_unlock(&handle_map_internal->lock);
		return -EEXIST;
	}
	N3K_MGMT_FLOW_LOG(DEBUG, "Mapping local:%" PRIu32 "->hw:%" PRIu32,
		local_handle, hw_handle);
	handle_map_internal->local_to_hw[local_handle].mapped_handle = hw_handle;
	handle_map_internal->local_to_hw[local_handle].present = true;
	rte_spinlock_unlock(&handle_map_internal->lock);
	return 0;
}

int
n3k_mgmt_unmap_local_to_hw_handle(struct n3k_mgmt_hw *hw,
	uint32_t local_handle)
{
	struct n3k_mgmt_handle_mapping_internal *handle_map_internal =
		hw->handle_map_internal;
	if (local_handle >= handle_map_internal->max_handles)
		return -EINVAL;
	rte_spinlock_lock(&handle_map_internal->lock);
	if (!handle_map_internal->local_to_hw[local_handle].present) {
		rte_spinlock_unlock(&handle_map_internal->lock);
		return -ENOENT;
	}
	N3K_MGMT_FLOW_LOG(DEBUG, "Unmapping local:%" PRIu32 "->hw:%" PRIu32,
		local_handle, handle_map_internal->local_to_hw[local_handle].mapped_handle);
	handle_map_internal->local_to_hw[local_handle].present = false;
	rte_spinlock_unlock(&handle_map_internal->lock);
	return 0;
}

int
n3k_mgmt_local_to_hw_handle(struct n3k_mgmt_hw *hw,
	uint32_t local_handle, uint32_t *hw_handle)
{
	struct n3k_mgmt_handle_mapping_internal *handle_map_internal =
		hw->handle_map_internal;
	if (local_handle >= handle_map_internal->max_handles)
		return -EINVAL;
	rte_spinlock_lock(&handle_map_internal->lock);
	if (!handle_map_internal->local_to_hw[local_handle].present) {
		rte_spinlock_unlock(&handle_map_internal->lock);
		return -ENOENT;
	}
	*hw_handle = handle_map_internal->local_to_hw[local_handle].mapped_handle;
	rte_spinlock_unlock(&handle_map_internal->lock);
	return 0;
}
