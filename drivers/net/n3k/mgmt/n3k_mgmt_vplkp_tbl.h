/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_VPLKP_TBL_H_
#define _PMD_NET_N3K_MGMT_VPLKP_TBL_H_

#include "n3k_mgmt_hw.h"

#include <rte_pmd_n3k_vplkp_tbl_mgmt.h>
#include <stdint.h>

int n3k_mgmt_vplkp_tbl_init(struct n3k_mgmt_hw *hw);

int n3k_mgmt_vplkp_tbl_flush(struct n3k_mgmt_hw *hw);
int n3k_mgmt_vplkp_tbl_insert(struct n3k_mgmt_hw *hw, uint32_t key,
	const struct n3k_mgmt_vplkp_tbl_res *res);
int n3k_mgmt_vplkp_tbl_delete(struct n3k_mgmt_hw *hw, uint32_t key);

/**
 * Search virtual port lookup table for entry with given key and fill res struct
 * with corresponding result.
 *
 * @return
 *   0 when no match found, 1 when match found, negative when error occurred
 */
int n3k_mgmt_vplkp_tbl_search(struct n3k_mgmt_hw *hw, uint32_t key,
	struct n3k_mgmt_vplkp_tbl_res *res);
