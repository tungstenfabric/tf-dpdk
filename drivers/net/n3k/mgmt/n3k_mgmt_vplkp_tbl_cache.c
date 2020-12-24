/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_log.h"
#include "n3k_mgmt_dump.h"
#include "n3k_mgmt_vplkp_tbl.h"
#include "n3k_mgmt_vplkp_tbl_cache.h"

#include "../rte_pmd_n3k_malloc.h"
#include <rte_pmd_n3k_vplkp_tbl_mgmt.h>

#include <rte_atomic.h>
#include <rte_hash.h>
#include <rte_rwlock.h>

#define N3K_MGMT_VPLKP_TBL_ENTRIES (16 * 1000)
#define N3K_MGMT_VPLKP_TBL_CACHE_NAME "n3k_mgmt_vpklp_cache"

struct _res_with_ref_count {
	struct n3k_mgmt_vplkp_tbl_res res;
	rte_atomic32_t ref_count;
};

struct n3k_mgmt_vplkp_tbl_cache {
	struct rte_hash* label_to_data;
	rte_rwlock_t lock;
	rte_atomic32_t total_entries;
};


int n3k_mgmt_vplkp_tbl_cache_init(struct n3k_mgmt_hw *hw)
{
	N3K_MGMT_VPLKP_LOG(INFO, "Initializing vplkp tbl cache");

	hw->vplkp_cache = n3k_zmalloc(N3K_MGMT_VPLKP_TBL_CACHE_NAME,
		sizeof(*hw->vplkp_cache), 0);
	if (hw->vplkp_cache == NULL) {
		N3K_MGMT_VPLKP_LOG(INFO, "Not enough memory to init");
		return -ENOMEM;
	}

	static const struct rte_hash_parameters hash_params = {
		.name = N3K_MGMT_VPLKP_TBL_CACHE_NAME,
		.entries = N3K_MGMT_VPLKP_TBL_ENTRIES,
		.key_len = sizeof(uint32_t),
		.extra_flag = RTE_HASH_EXTRA_FLAGS_EXT_TABLE |
			RTE_HASH_EXTRA_FLAGS_RW_CONCURRENCY,
	};
	hw->vplkp_cache->label_to_data = rte_hash_create(&hash_params);
	if (hw->vplkp_cache->label_to_data == NULL) {
		n3k_free(hw->vplkp_cache);
		N3K_MGMT_VPLKP_LOG(INFO, "Failed to init hash table");
		return -ENOMEM;
	}

	rte_rwlock_init(&hw->vplkp_cache->lock);

	rte_atomic32_init(&hw->vplkp_cache->total_entries);

	return 0;
}

int n3k_mgmt_vplkp_tbl_cache_incr_iter(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_vplkp_tbl_cache_iterator *iterator)
{
	struct _res_with_ref_count *val;
	const uint32_t *mpls_label;
	int rc;

	rc = rte_hash_iterate(hw->vplkp_cache->label_to_data,
		(const void**)&mpls_label,
		(void**)&val,
		&iterator->hash_handle);
	if (rc < 0) {
		N3K_MGMT_VPLKP_LOG(DEBUG, "Entry not found");
		return rc;
	}

	N3K_MGMT_VPLKP_LOG(DEBUG, "Found vplkp entry: label=%" PRIu32, *mpls_label);

	iterator->mpls_label = *mpls_label;

	return 0;
}

int n3k_mgmt_vplkp_tbl_cache_init_iter(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_vplkp_tbl_cache_iterator *iterator)
{
	iterator->hash_handle = 0;
	return n3k_mgmt_vplkp_tbl_cache_incr_iter(hw, iterator);
}

uint32_t n3k_mgmt_vplkp_tbl_cache_total_entries(struct n3k_mgmt_hw *hw)
{
	return rte_atomic32_read(&hw->vplkp_cache->total_entries);
}

static int find_cache_val_by_label(struct n3k_mgmt_hw *hw, uint32_t mpls_label,
	struct _res_with_ref_count **val)
{
	return rte_hash_lookup_data(hw->vplkp_cache->label_to_data, &mpls_label,
		(void**)val);
}

static int check_label_in_cache(struct n3k_mgmt_hw *hw,
	uint32_t mpls_label, const struct n3k_mgmt_vplkp_tbl_res *res)
{
	struct _res_with_ref_count *val;
	int rc;

	rc = find_cache_val_by_label(hw, mpls_label, &val);
	if (rc >= 0) {
		if (val->res.vport == res->vport) {
			// found entry, increment ref count
			rte_atomic32_inc(&val->ref_count);
			N3K_MGMT_VPLKP_LOG(INFO,
				"Key found in cache for label=%" PRIu32 ", ref_count=%" PRIu32,
				mpls_label, rte_atomic32_read(&val->ref_count));
			return 0;
		} else {
			N3K_MGMT_VPLKP_LOG(ERR,
				"Key found in cache for label=%" PRIu32
				" but has different vport type", mpls_label);
			return -EEXIST;
		}
	}
	return rc;
}

int n3k_mgmt_vplkp_tbl_cache_insert(struct n3k_mgmt_hw *hw,
	uint32_t mpls_label, const struct n3k_mgmt_vplkp_tbl_res *res)
{
	struct _res_with_ref_count *val;
	int rc;

	if (N3K_MGMT_VPLKP_SHOULD_LOG(DEBUG)) {
		N3K_MGMT_VPLKP_LOG(DEBUG,
			"Inserting vplkp tbl cache entry: label=%" PRIu32,
			mpls_label);
		n3k_mgmt_vplkp_tbl_res_dump(res);
	}

	rte_rwlock_read_lock(&hw->vplkp_cache->lock);
	rc = check_label_in_cache(hw, mpls_label, res);
	if (rc <= 0 && rc != -ENOENT) {
		rte_rwlock_read_unlock(&hw->vplkp_cache->lock);
		return rc;
	}
	rte_rwlock_read_unlock(&hw->vplkp_cache->lock);

	rte_rwlock_write_lock(&hw->vplkp_cache->lock);
	rc = check_label_in_cache(hw, mpls_label, res);
	if (rc <= 0 && rc != -ENOENT)
		goto release_lock;

	N3K_MGMT_VPLKP_LOG(DEBUG, "Entry not found in cache. Adding new vplkp entry.");

	if (rte_atomic32_read(&hw->vplkp_cache->total_entries) == N3K_MGMT_VPLKP_TBL_ENTRIES) {
		rc = -ENOSPC;
		N3K_MGMT_VPLKP_LOG(WARNING, "No free entry in cache");
		goto release_lock;
	}

	val = n3k_malloc(N3K_MGMT_VPLKP_TBL_CACHE_NAME, sizeof(*val), 0);
	if (val == NULL) {
		rc = -ENOMEM;
		goto release_lock;
	}
	rte_atomic32_set(&val->ref_count, 1);
	val->res = *res;

	rc = rte_hash_add_key_data(hw->vplkp_cache->label_to_data, &mpls_label, val);
	if (rc < 0) {
		N3K_MGMT_VPLKP_LOG(ERR, "Error while adding entry to cache");
		goto free_val;
	}

	rc = n3k_mgmt_vplkp_tbl_insert(hw, mpls_label, res);
	if (rc < 0) {
		N3K_MGMT_VPLKP_LOG(ERR, "Error while adding vplkp entry");
		goto delete_hash_entry;
	}

	rte_atomic32_inc(&hw->vplkp_cache->total_entries);

	N3K_MGMT_VPLKP_LOG(INFO,
		"New vplkp entry added successfully to cache: label=%" PRIu32,
		mpls_label);

	goto release_lock;

delete_hash_entry:
	{
		__rte_unused int hash_rc = rte_hash_del_key(hw->vplkp_cache->label_to_data,
			&mpls_label);
		RTE_ASSERT(hash_rc > 0);
	}
free_val:
	n3k_free(val);
release_lock:
	rte_rwlock_write_unlock(&hw->vplkp_cache->lock);
	return rc;
}

int n3k_mgmt_vplkp_tbl_cache_get_by_label(struct n3k_mgmt_hw *hw,
	uint32_t mpls_label, struct n3k_mgmt_vplkp_tbl_res *res)
{
	struct _res_with_ref_count *val;
	int rc;

	rte_rwlock_read_lock(&hw->vplkp_cache->lock);

	rc = find_cache_val_by_label(hw, mpls_label, &val);
	if (rc < 0) {
		rte_rwlock_read_unlock(&hw->vplkp_cache->lock);
		N3K_MGMT_VPLKP_LOG(DEBUG,
			"Couldn't get entry by label: label=%" PRIu32, mpls_label);
		return rc;
	}
	*res = val->res;

	if (N3K_MGMT_VPLKP_SHOULD_LOG(DEBUG)) {
		N3K_MGMT_VPLKP_LOG(DEBUG,
			"Found entry by label: label=%" PRIu32, mpls_label);
		n3k_mgmt_vplkp_tbl_res_dump(res);
	}

	rte_rwlock_read_unlock(&hw->vplkp_cache->lock);
	return 0;
}

uint32_t n3k_mgmt_vplkp_tbl_cache_get_ref_count(struct n3k_mgmt_hw *hw,
	uint32_t mpls_label)
{
	struct _res_with_ref_count *val;
	uint32_t ref_count;

	rte_rwlock_read_lock(&hw->vplkp_cache->lock);
	if (find_cache_val_by_label(hw, mpls_label, &val) < 0) {
		rte_rwlock_read_unlock(&hw->vplkp_cache->lock);
		return 0;
	}

	ref_count = rte_atomic32_read(&val->ref_count);

	rte_rwlock_read_unlock(&hw->vplkp_cache->lock);
	return ref_count;
}

int n3k_mgmt_vplkp_tbl_cache_yield(struct n3k_mgmt_hw *hw,
	uint32_t mpls_label)
{
	int rc;
	struct _res_with_ref_count *val;

	rte_rwlock_read_lock(&hw->vplkp_cache->lock);
	rc = find_cache_val_by_label(hw, mpls_label, &val);
	if (rc < 0) {
		rte_rwlock_read_unlock(&hw->vplkp_cache->lock);
		N3K_MGMT_VPLKP_LOG(ERR,
			"Vplkp tbl cache entry not found: label=%" PRIu32,
			mpls_label);
		return rc;
	}
	if (!rte_atomic32_dec_and_test(&val->ref_count)) {
		rte_rwlock_read_unlock(&hw->vplkp_cache->lock);
		N3K_MGMT_VPLKP_LOG(INFO,
			"Vplkp tbl cache entry still in use. Not removing: "
			"label=%" PRIu32 ", ref_count=%" PRIu32,
			mpls_label, rte_atomic32_read(&val->ref_count));
		return 0;
	}
	rte_rwlock_read_unlock(&hw->vplkp_cache->lock);

	rte_rwlock_write_lock(&hw->vplkp_cache->lock);
	rc = find_cache_val_by_label(hw, mpls_label, &val);
	if (rc < 0) {
		N3K_MGMT_VPLKP_LOG(ERR,
			"Vplkp tbl cache entry not found: label=%" PRIu32,
			mpls_label);
		goto release_lock;
	}
	if (rte_atomic32_read(&val->ref_count) != 0) {
		rc = 0;
		N3K_MGMT_VPLKP_LOG(INFO,
			"Vplkp tbl cache entry still in use. Not removing: "
			"label=%" PRIu32 ", ref_count=%" PRIu32,
			mpls_label, rte_atomic32_read(&val->ref_count));
		goto release_lock;
	}

	rc = n3k_mgmt_vplkp_tbl_delete(hw, mpls_label);
	if (rc < 0) {
		rte_atomic32_inc(&val->ref_count);
		N3K_MGMT_VPLKP_LOG(ERR,
			"Failed to delete vplkp table entry from hardware. Not removing from cache: "
			"label=%" PRIu32, mpls_label);
		goto release_lock;
	}

	rc = rte_hash_del_key(hw->vplkp_cache->label_to_data, &mpls_label);
	RTE_ASSERT(rc > 0);

	rte_atomic32_dec(&hw->vplkp_cache->total_entries);

	n3k_free(val);

	N3K_MGMT_VPLKP_LOG(INFO,
		"Successfully removed vplkp tbl cache entry: label=%" PRIu32,
		mpls_label);

	rc = 0;

release_lock:
	rte_rwlock_write_unlock(&hw->vplkp_cache->lock);
	return rc;
}
