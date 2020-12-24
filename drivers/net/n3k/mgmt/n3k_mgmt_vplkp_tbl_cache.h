/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_VPLKP_TBL_CACHE_H_
#define _PMD_NET_N3K_MGMT_VPLKP_TBL_CACHE_H_

#include "n3k_mgmt_hw.h"

#include <rte_pmd_n3k_vplkp_tbl_mgmt.h>

#include <stdint.h>
#include <rte_flow.h>

int n3k_mgmt_vplkp_tbl_cache_init(struct n3k_mgmt_hw *hw);

struct n3k_mgmt_vplkp_tbl_cache_iterator {
	uint32_t hash_handle;
	uint32_t mpls_label;
};

int n3k_mgmt_vplkp_tbl_cache_incr_iter(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_vplkp_tbl_cache_iterator *iterator);
int n3k_mgmt_vplkp_tbl_cache_init_iter(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_vplkp_tbl_cache_iterator *iterator);

uint32_t n3k_mgmt_vplkp_tbl_cache_total_entries(struct n3k_mgmt_hw *hw);

int n3k_mgmt_vplkp_tbl_cache_insert(struct n3k_mgmt_hw *hw,
	uint32_t mpls_label, const struct n3k_mgmt_vplkp_tbl_res *res);

int n3k_mgmt_vplkp_tbl_cache_get_by_label(struct n3k_mgmt_hw *hw,
	uint32_t mpls_label, struct n3k_mgmt_vplkp_tbl_res *res);

uint32_t n3k_mgmt_vplkp_tbl_cache_get_ref_count(struct n3k_mgmt_hw *hw,
	uint32_t mpls_label);

/*
 * Yielding a vplkp entry with ref_count = 0 may lead to undefined behaviour.
 */
int n3k_mgmt_vplkp_tbl_cache_yield(struct n3k_mgmt_hw *hw, uint32_t mpls_label);

#endif /* _PMD_NET_N3K_MGMT_VPLKP_TBL_CACHE_H_ */
