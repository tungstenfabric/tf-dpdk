/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_FLOW_TBL_HANDLES_H_
#define _PMD_NET_N3K_MGMT_FLOW_TBL_HANDLES_H_

#include "n3k_mgmt_hw.h"

#include <rte_pmd_n3k_flow_tbl_mgmt.h>
#include <stdint.h>
#include <rte_rwlock.h>

enum n3k_mgmt_flow_tbl_flow_status {
	N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETED,
	N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDED,
	N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETING,
	N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDING,
	N3K_MGMT_FLOW_TBL_FLOW_STATUS_FAILED_PRESENT,
	N3K_MGMT_FLOW_TBL_FLOW_STATUS_FAILED_ABSENT,
	N3K_MGMT_FLOW_TBL_FLOW_STATUS_FAILED_MAY_BE_PRESENT,
};

struct n3k_mgmt_flow_tbl_handle {
	uint32_t flow_id;
	enum n3k_mgmt_tunnel_type encap_type; // in case of encap flow

	// take read lock when needed a guarantee that the flow's fields won't change
	// take write lock when needed to change the flow's fields
	rte_rwlock_t lock;

	bool is_mirror_flow;
	struct n3k_mgmt_flow_tbl_handle *mirror_flow;
	uint32_t recirc_id;

	enum n3k_mgmt_flow_tbl_flow_status status;
	struct n3k_mgmt_flow_tracker_stats cached_stats;
};

int n3k_mgmt_flow_tbl_change_flow_status(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle,
	enum n3k_mgmt_flow_tbl_flow_status from,
	enum n3k_mgmt_flow_tbl_flow_status to);
int n3k_mgmt_flow_tbl_change_flow_status_nolock(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle,
	enum n3k_mgmt_flow_tbl_flow_status from,
	enum n3k_mgmt_flow_tbl_flow_status to);

uint32_t n3k_mgmt_flow_tbl_get_max_flow_id(struct n3k_mgmt_hw *hw);
uint32_t n3k_mgmt_flow_tbl_get_min_flow_id(struct n3k_mgmt_hw *hw);

int n3k_mgmt_flow_tbl_init_flow_handle(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle **handle,
	const struct n3k_mgmt_flow_tbl_action *action,
	uint32_t flow_id);
int n3k_mgmt_flow_tbl_init_mirror_flow_handle(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle **handle,
	uint32_t recirc_id,
	uint32_t flow_id);
int n3k_mgmt_flow_tbl_get_status_by_flow_id(struct n3k_mgmt_hw *hw,
	uint32_t flow_id,
	enum n3k_mgmt_flow_tbl_flow_status *status);
void n3k_mgmt_flow_tbl_get_cached_stats(struct n3k_mgmt_flow_tbl_handle *handle,
	struct n3k_mgmt_flow_tracker_stats *stats);
void n3k_mgmt_flow_tbl_set_cached_stats(struct n3k_mgmt_flow_tbl_handle *handle,
	const struct n3k_mgmt_flow_tracker_stats *stats);

int n3k_mgmt_flow_tbl_next_handle(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle **handle);
int n3k_mgmt_flow_tbl_next_no_mirror_handle(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle **handle);
int n3k_mgmt_flow_tbl_first_handle(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle **handle);
int n3k_mgmt_flow_tbl_first_no_mirror_handle(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle **handle);

int n3k_mgmt_flow_tbl_handles_init(struct n3k_mgmt_hw *hw);

#endif /* _PMD_NET_N3K_MGMT_FLOW_TBL_HANDLES_H_ */
