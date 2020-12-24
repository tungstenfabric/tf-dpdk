/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_init/n3k_mgmt_init_port.h"
#include "n3k_mgmt_flow_tbl_handles.h"
#include "n3k_mgmt_flow_tbl_regs_api.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_log.h"

#include "../rte_pmd_n3k_malloc.h"

#define N3K_MGMT_MIN_HANDLE_ID 16

struct n3k_mgmt_flow_tbl_handles_internal {
	uint32_t max_handles;
	// Can be replaced by BST or concurrent list to reduce memory usage.
	struct n3k_mgmt_flow_tbl_handle *handles;
};

int n3k_mgmt_flow_tbl_change_flow_status(struct n3k_mgmt_flow_tbl_handle *handle,
	enum n3k_mgmt_flow_tbl_flow_status from,
	enum n3k_mgmt_flow_tbl_flow_status to)
{
	rte_rwlock_write_lock(&handle->lock);
	if (handle->status != from) {
		rte_rwlock_write_unlock(&handle->lock);
		return -EINVAL;
	}
	handle->status = to;
	rte_rwlock_write_unlock(&handle->lock);
	return 0;
}

int n3k_mgmt_flow_tbl_init_flow_handle(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle **handle,
	const struct n3k_mgmt_flow_tbl_action *action,
	uint32_t flow_id)
{
	if (flow_id >= n3k_mgmt_flow_tbl_get_max_flow_id(hw))
		return -EINVAL;

	*handle = &hw->flow_tbl_handles_internal->handles[flow_id];
	memset(*handle, 0, sizeof(**handle));

	(*handle)->flow_id = flow_id;

	if (action->type == N3K_MGMT_FLOW_TBL_ACTION_ENCAP)
		(*handle)->encap_type = action->encap_action.type;

	return 0;
}

int n3k_mgmt_flow_tbl_init_mirror_flow_handle(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle **handle,
	uint32_t recirc_id,
	uint32_t flow_id)
{
	if (flow_id >= n3k_mgmt_flow_tbl_get_max_flow_id(hw))
		return -EINVAL;

	*handle = &hw->flow_tbl_handles_internal->handles[flow_id];
	memset(*handle, 0, sizeof(**handle));

	(*handle)->flow_id = flow_id;
	(*handle)->recirc_id = recirc_id;
	(*handle)->is_mirror_flow = true;

	return 0;
}

int n3k_mgmt_flow_tbl_get_status_by_flow_id(struct n3k_mgmt_hw *hw,
	uint32_t flow_id,
	enum n3k_mgmt_flow_tbl_flow_status *status)
{
	struct n3k_mgmt_flow_tbl_handle *handle;
	if (flow_id >= n3k_mgmt_flow_tbl_get_max_flow_id(hw))
		return -EINVAL;

	handle = &hw->flow_tbl_handles_internal->handles[flow_id];

	rte_rwlock_read_lock(&handle->lock);
	*status = handle->status;
	rte_rwlock_read_unlock(&handle->lock);

	return 0;
}

inline uint32_t n3k_mgmt_flow_tbl_get_max_flow_id(struct n3k_mgmt_hw *hw)
{
	return hw->flow_tbl_handles_internal->max_handles;
}

inline uint32_t n3k_mgmt_flow_tbl_get_min_flow_id(
	struct n3k_mgmt_hw *hw __rte_unused)
{
	return N3K_MGMT_MIN_HANDLE_ID;
}

int n3k_mgmt_flow_tbl_handles_init(struct n3k_mgmt_hw *hw)
{
	uint32_t i;
	EM_LKUP_EML_NUM_EM_t num_em;

	N3K_MGMT_FLOW_LOG(INFO, "Initializing flow tbl");

	if (hw->flow_tbl_handles_internal) {
		N3K_MGMT_FLOW_LOG(ERR, "Flow tbl handles already initialized");
		return -EINVAL;
	}

	hw->flow_tbl_handles_internal = n3k_zmalloc("n3k_flow_tbl",
		sizeof(*hw->flow_tbl_handles_internal), 0);
	if (hw->flow_tbl_handles_internal == NULL) {
		N3K_MGMT_FLOW_LOG(ERR,
			"Failed to allocate memory for flow tbl handles data");
		return -ENOMEM;
	}

	num_em.val = csr_read(hw, MKS_CHIP_EM_LKUP_BASE, EM_LKUP_EML_NUM_EM);
	hw->flow_tbl_handles_internal->max_handles = num_em.field.num_em;

	hw->flow_tbl_handles_internal->handles = n3k_zmalloc("n3k_flow_tbl",
		sizeof(*hw->flow_tbl_handles_internal->handles) *
		hw->flow_tbl_handles_internal->max_handles, 0);
	if (hw->flow_tbl_handles_internal->handles == NULL) {
		N3K_MGMT_FLOW_LOG(ERR,
			"Failed to allocate memory for flow tbl handles array");
		n3k_free(hw->flow_tbl_handles_internal);
		hw->flow_tbl_handles_internal = NULL;
		return -ENOMEM;
	}
	for (i = 0; i < hw->flow_tbl_handles_internal->max_handles; ++i) {
		hw->flow_tbl_handles_internal->handles[i].status =
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETED;
		hw->flow_tbl_handles_internal->handles[i].flow_id = i;
		rte_rwlock_init(&hw->flow_tbl_handles_internal->handles[i].lock);
	}

	return 0;
}


static int find_next_added_flow(struct n3k_mgmt_hw *hw,
	uint32_t flow_id,
	struct n3k_mgmt_flow_tbl_handle **handle)
{
	N3K_MGMT_FLOW_LOG(DEBUG, "Started iterating: flow_id=%" PRIu32, flow_id);

	for (; flow_id < hw->flow_tbl_handles_internal->max_handles; ++flow_id) {
		*handle = &hw->flow_tbl_handles_internal->handles[flow_id];

		rte_rwlock_read_lock(&(*handle)->lock);
		if ((*handle)->status != N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDED) {
			rte_rwlock_read_unlock(&(*handle)->lock);
			continue;
		}
		N3K_MGMT_FLOW_LOG(DEBUG, "Found flow entry: flow_id=%" PRIu32,
			(*handle)->flow_id);
		rte_rwlock_read_unlock(&(*handle)->lock);

		return 0;
	}

	N3K_MGMT_FLOW_LOG(DEBUG, "Entry not found");

	return -ENOENT;
}

int n3k_mgmt_flow_tbl_next_handle(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle **handle)
{
	if (!handle || !*handle)
		return -EINVAL;
	return find_next_added_flow(hw, (*handle)->flow_id + 1, handle);
}

int n3k_mgmt_flow_tbl_first_handle(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle **handle)
{
	return find_next_added_flow(hw, 0, handle);
}
