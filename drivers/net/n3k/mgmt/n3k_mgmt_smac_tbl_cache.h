/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_SMAC_TBL_CACHE_H_
#define _PMD_NET_N3K_MGMT_SMAC_TBL_CACHE_H_

#include <rte_ethdev.h>
#include <rte_ether.h>
#include <rte_flow.h>

#include "n3k_mgmt_hw.h"

int n3k_mgmt_smac_tbl_cache_init(struct n3k_mgmt_hw *hw);

int n3k_mgmt_smac_tbl_cache_next_index(struct n3k_mgmt_hw *hw, uint8_t *smac_id);
int n3k_mgmt_smac_tbl_cache_first_index(struct n3k_mgmt_hw *hw, uint8_t *smac_id);

uint32_t n3k_mgmt_smac_tbl_cache_total_entries(struct n3k_mgmt_hw *hw);

// Does not assume ownership of the key struct.
int n3k_mgmt_smac_tbl_cache_fetch(struct n3k_mgmt_hw *hw,
	const struct rte_ether_addr* key,
	uint8_t* smac_id);

int n3k_mgmt_smac_tbl_cache_find(struct n3k_mgmt_hw *hw,
	const struct rte_ether_addr* key,
	uint8_t* smac_id);

int n3k_mgmt_smac_tbl_cache_get_by_id(struct n3k_mgmt_hw *hw,
	uint8_t smac_id, struct rte_ether_addr* out_key);

uint32_t n3k_mgmt_smac_tbl_cache_get_ref_count(struct n3k_mgmt_hw *hw,
	uint8_t smac_id);

/*
 * Yielding a smac with ref_count = 0 may lead to undefined behaviour.
