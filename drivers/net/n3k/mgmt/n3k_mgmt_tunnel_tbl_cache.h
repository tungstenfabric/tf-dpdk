/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_TUNNEL_TBL_CACHE_H_
#define _PMD_NET_N3K_MGMT_TUNNEL_TBL_CACHE_H_

#include <stdint.h>

#include <rte_flow.h>

#include "n3k_mgmt_hw.h"

#include "rte_pmd_n3k_tunnel_tbl_mgmt.h"

int n3k_mgmt_tunnel_tbl_cache_init(struct n3k_mgmt_hw *hw);

int n3k_mgmt_tunnel_tbl_cache_next_index(struct n3k_mgmt_hw *hw, uint16_t *tunnel_id);
int n3k_mgmt_tunnel_tbl_cache_first_index(struct n3k_mgmt_hw *hw, uint16_t *tunnel_id);

uint32_t n3k_mgmt_tunnel_tbl_cache_total_entries(struct n3k_mgmt_hw *hw);

/**
 * Retrieves the tunnel_id for given key and increments the ref_count.
 * If the key is not in the cache, inserts it to the cache and hardware
 * and sets ref_count to 1. Does not assume ownership of the key struct.
 */
int n3k_mgmt_tunnel_tbl_cache_fetch(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_tunnel_tbl_entry* key,
	uint16_t* tunnel_id);

/**
 * Finds the tunnel_id for provided key, fails if not found.
 * Does not change the ref_count.
 */
int n3k_mgmt_tunnel_tbl_cache_find(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_tunnel_tbl_entry* key,
	uint16_t* tunnel_id);

/**
 * Gets the key by tunnel_id. Does not change the ref_count.
 * Copies the key to out_key in memory managed by the client code.
 */
int n3k_mgmt_tunnel_tbl_cache_get_by_id(struct n3k_mgmt_hw *hw,
	uint16_t tunnel_id, struct n3k_mgmt_tunnel_tbl_entry* out_key);

/**
 * Return ref_count for given tunnel_id. Return 0 in case given tunnel doesn't
 * exist.
 */
uint32_t n3k_mgmt_tunnel_tbl_cache_get_ref_count(struct n3k_mgmt_hw *hw,
	uint16_t tunnel_id);

/**
 * Decrements the ref_count of entry with given tunnel_id.
 * If ref_count reaches 0, deletes the entry from cache and hw.
 * Yielding a tunnel with ref_count = 0 may lead to undefined behaviour.
