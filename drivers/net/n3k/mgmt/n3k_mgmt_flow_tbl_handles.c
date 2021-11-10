/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_flow_tbl_handles.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_concurrent_pos_set.h"
#include "n3k_mgmt_init/n3k_mgmt_init_port.h"
#include "regs/n3k_mgmt_flow_tbl_regs_api.h"

#include <rte_pmd_n3k_malloc.h>

#define N3K_MGMT_MIN_HANDLE_ID 16

struct n3k_mgmt_flow_tbl_handles_internal {
	uint32_t max_handles;
	struct n3k_mgmt_concurrent_pos_set set;
	// Can be replaced by BST or concurrent list to reduce memory usage.
	struct n3k_mgmt_flow_tbl_handle *handles;
};

int n3k_mgmt_flow_tbl_change_flow_status_nolock(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle,
	enum n3k_mgmt_flow_tbl_flow_status from,
	enum n3k_mgmt_flow_tbl_flow_status to)
{
	if (handle->status != from)
		return -EINVAL;
	handle->status = to;
	if (to == N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETED)
		n3k_mgmt_conc_set_turn_off_pos(
			&hw->flow_tbl_handles_internal->set, handle->flow_id);
	else if (to == N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDED)
		n3k_mgmt_conc_set_turn_on_pos(
			&hw->flow_tbl_handles_internal->set, handle->flow_id);
	return 0;
}

int n3k_mgmt_flow_tbl_change_flow_status(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle,
	enum n3k_mgmt_flow_tbl_flow_status from,
	enum n3k_mgmt_flow_tbl_flow_status to)
{
	int ret;
	rte_rwlock_write_lock(&handle->lock);
	ret = n3k_mgmt_flow_tbl_change_flow_status_nolock(hw, handle, from, to);
	rte_rwlock_write_unlock(&handle->lock);
	return ret;
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

	rte_rwlock_init(&(*handle)->lock);
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

	rte_rwlock_init(&(*handle)->lock);
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

void n3k_mgmt_flow_tbl_get_cached_stats(struct n3k_mgmt_flow_tbl_handle *handle,
	struct n3k_mgmt_flow_tracker_stats *stats)
{
	rte_rwlock_read_lock(&handle->lock);
	*stats = handle->cached_stats;
	rte_rwlock_read_unlock(&handle->lock);
}

void n3k_mgmt_flow_tbl_set_cached_stats(struct n3k_mgmt_flow_tbl_handle *handle,
	const struct n3k_mgmt_flow_tracker_stats *stats)
{
	rte_rwlock_write_lock(&handle->lock);
	handle->cached_stats = *stats;
	rte_rwlock_write_unlock(&handle->lock);
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
	EM_LKUP_EML_NUM_EM_t num_em;
	uint32_t i;
	int ret;

	N3K_MGMT_LOG(FLOW, INFO, "Initializing flow tbl");

	if (hw->flow_tbl_handles_internal) {
		N3K_MGMT_LOG(FLOW, ERR, "Flow tbl handles already initialized");
		return -EINVAL;
	}

	hw->flow_tbl_handles_internal = n3k_zmalloc("n3k_flow_tbl",
		sizeof(*hw->flow_tbl_handles_internal), 0);
	if (hw->flow_tbl_handles_internal == NULL) {
		N3K_MGMT_LOG(FLOW, ERR,
			"Failed to allocate memory for flow tbl handles data");
		return -ENOMEM;
	}

	num_em.val = csr_read(hw, MKS_CHIP_EM_LKUP_BASE, EM_LKUP_EML_NUM_EM);
	hw->flow_tbl_handles_internal->max_handles = num_em.field.num_em;

	hw->flow_tbl_handles_internal->handles = n3k_zmalloc("n3k_flow_tbl",
		sizeof(*hw->flow_tbl_handles_internal->handles) *
		hw->flow_tbl_handles_internal->max_handles, 0);
	if (hw->flow_tbl_handles_internal->handles == NULL) {
		N3K_MGMT_LOG(FLOW, ERR,
			"Failed to allocate memory for flow tbl handles array");
		ret = -ENOMEM;
		goto free_internal;
	}
	for (i = 0; i < hw->flow_tbl_handles_internal->max_handles; ++i) {
		hw->flow_tbl_handles_internal->handles[i].status =
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETED;
	}

	ret = n3k_mgmt_conc_set_init(&hw->flow_tbl_handles_internal->set,
		hw->flow_tbl_handles_internal->max_handles);
	if (ret < 0)
		goto free_handles;
	return 0;

free_handles:
	n3k_free(hw->flow_tbl_handles_internal->handles);
free_internal:
	n3k_free(hw->flow_tbl_handles_internal);
	hw->flow_tbl_handles_internal = NULL;
	return ret;
}


static int find_next_added_flow(struct n3k_mgmt_hw *hw,
	uint32_t flow_id,
	struct n3k_mgmt_flow_tbl_handle **handle)
{
	size_t next_flow_id;
	int ret;
	ret = n3k_mgmt_conc_set_find_next_turned_on_pos(
		&hw->flow_tbl_handles_internal->set, flow_id, &next_flow_id);
	if (ret < 0)
		return ret;
	*handle = &hw->flow_tbl_handles_internal->handles[next_flow_id];
	return 0;
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

int n3k_mgmt_flow_tbl_next_no_mirror_handle(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle **handle)
{
	int ret;
	do {
		ret = n3k_mgmt_flow_tbl_next_handle(hw, handle);
		if (ret < 0)
			return ret;
	} while ((*handle)->is_mirror_flow);
	return ret;
}

int n3k_mgmt_flow_tbl_first_no_mirror_handle(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle **handle)
{
	int ret = n3k_mgmt_flow_tbl_first_handle(hw, handle);
	if (ret < 0)
		return ret;
	if ((*handle)->is_mirror_flow)
		return n3k_mgmt_flow_tbl_next_no_mirror_handle(hw, handle);
	return ret;
}
