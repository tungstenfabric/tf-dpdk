/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_dump.h"
#include "n3k_mgmt_ethdev.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_smac_tbl.h"
#include "n3k_mgmt_smac_tbl_cache.h"

#include "../rte_pmd_n3k_malloc.h"
#include <rte_pmd_n3k_smac_tbl_mgmt.h>

#include <rte_atomic.h>
#include <rte_hash.h>
#include <rte_rwlock.h>

#define N3K_MGMT_SMAC_TBL_MAX_ENTRIES 16

#define N3K_MGMT_SMAC_TBL_CACHE_NAME "n3k_mgmt_smac_cache"

struct _key_with_ref_count {
	struct rte_ether_addr* key;
	rte_atomic32_t ref_count;
};

struct n3k_mgmt_smac_tbl_cache {
	struct _key_with_ref_count id_to_key[N3K_MGMT_SMAC_TBL_MAX_ENTRIES];
	struct rte_hash* key_to_id;
	rte_rwlock_t lock;
	rte_atomic32_t total_entries;
};

int n3k_mgmt_smac_tbl_cache_next_index(struct n3k_mgmt_hw *hw, uint8_t *smac_id)
{
	struct n3k_mgmt_smac_tbl_cache *smac_cache = hw->smac_cache;

	N3K_MGMT_SMAC_LOG(DEBUG, "Started iterating: smac_id=%" PRIu8, (uint8_t)(*smac_id + 1));

	for (*smac_id = *smac_id + 1;
			*smac_id < N3K_MGMT_SMAC_TBL_MAX_ENTRIES;
			++(*smac_id)) {
		struct _key_with_ref_count *cur_id_to_key = &smac_cache->id_to_key[*smac_id];

		if (rte_atomic32_read(&cur_id_to_key->ref_count)) {
			N3K_MGMT_SMAC_LOG(DEBUG, "Found smac entry: smac_id=%" PRIu8,
				*smac_id);
			return 0;
		}
	}

	N3K_MGMT_SMAC_LOG(DEBUG, "Entry not found");

	return -ENOENT;
}

int n3k_mgmt_smac_tbl_cache_first_index(struct n3k_mgmt_hw *hw, uint8_t *smac_id)
{
	*smac_id = -1;
	return n3k_mgmt_smac_tbl_cache_next_index(hw, smac_id);
}

uint32_t n3k_mgmt_smac_tbl_cache_total_entries(struct n3k_mgmt_hw *hw)
{
	return rte_atomic32_read(&hw->smac_cache->total_entries);
}

// TODO(n3k): extract common code from smac tbl cache and tunnel tbl cache
//            to stop duplicating code
int n3k_mgmt_smac_tbl_cache_init(struct n3k_mgmt_hw *hw)
{
	int i;

	N3K_MGMT_SMAC_LOG(INFO, "Initializing smac tbl cache");

	hw->smac_cache = n3k_zmalloc(N3K_MGMT_SMAC_TBL_CACHE_NAME,
		sizeof(*hw->smac_cache), 0);
	if (hw->smac_cache == NULL)
		return -ENOMEM;

	for (i = 0; i < N3K_MGMT_SMAC_TBL_MAX_ENTRIES; i++)
		rte_atomic32_init(&hw->smac_cache->id_to_key[i].ref_count);

	static const struct rte_hash_parameters hash_params = {
		.name = N3K_MGMT_SMAC_TBL_CACHE_NAME,
		.entries = N3K_MGMT_SMAC_TBL_MAX_ENTRIES,
		.key_len = sizeof(struct rte_ether_addr),
		.extra_flag = RTE_HASH_EXTRA_FLAGS_EXT_TABLE |
			RTE_HASH_EXTRA_FLAGS_RW_CONCURRENCY,
	};
	hw->smac_cache->key_to_id = rte_hash_create(&hash_params);
	if (hw->smac_cache->key_to_id == NULL) {
		n3k_free(hw->smac_cache);
		return -rte_errno;
	}

	rte_rwlock_init(&hw->smac_cache->lock);

	rte_atomic32_init(&hw->smac_cache->total_entries);

	return 0;
}

static int find_cache_val_by_key(struct n3k_mgmt_hw *hw,
	const struct rte_ether_addr* key,
	uint8_t *val)
{
	return rte_hash_lookup_data(hw->smac_cache->key_to_id, key, (void**)val);
}

static int check_key_in_cache(struct n3k_mgmt_hw *hw,
	const struct rte_ether_addr* key,
	uint8_t* smac_id)
{
	int rc;

	rc = find_cache_val_by_key(hw, key, smac_id);
	if (rc >= 0) {
		// found the index, increment ref count
		rte_atomic32_inc(
			&hw->smac_cache->id_to_key[*smac_id].ref_count);
		N3K_MGMT_SMAC_LOG(INFO,
			"Key found in cache: smac_id=%" PRIu8 ", ref_count=%" PRIu32,
			*smac_id,
			rte_atomic32_read(&hw->smac_cache->id_to_key[*smac_id].ref_count));
		return 0;
	}
	return rc;
}

int n3k_mgmt_smac_tbl_cache_fetch(struct n3k_mgmt_hw *hw,
	const struct rte_ether_addr* key,
	uint8_t* smac_id)
{
	int rc;
	long i;

	if (N3K_MGMT_SMAC_SHOULD_LOG(DEBUG)) {
		N3K_MGMT_SMAC_LOG(DEBUG, "Fetching smac tbl cache entry:");
		n3k_mgmt_smac_tbl_addr_dump(key);
	}

	rte_rwlock_read_lock(&hw->smac_cache->lock);
	rc = check_key_in_cache(hw, key, smac_id);
	if (rc <= 0 && rc != -ENOENT) {
		rte_rwlock_read_unlock(&hw->smac_cache->lock);
		return rc;
	}
	rte_rwlock_read_unlock(&hw->smac_cache->lock);

	rte_rwlock_write_lock(&hw->smac_cache->lock);

	// check if smac is still not present
	rc = check_key_in_cache(hw, key, smac_id);
	if (rc <= 0 && rc != -ENOENT)
		goto release_lock;

	N3K_MGMT_SMAC_LOG(DEBUG, "Key not found in cache. Adding new smac.");

	// find a free entry
	for (i = 0; i < N3K_MGMT_SMAC_TBL_MAX_ENTRIES; i++)
		if (hw->smac_cache->id_to_key[i].key == NULL)
			break;
	if (i == N3K_MGMT_SMAC_TBL_MAX_ENTRIES) {
		// all entries used
		N3K_MGMT_SMAC_LOG(WARNING, "No free entry in cache");
		rc = -ENOSPC;
		goto release_lock;
	}

	struct _key_with_ref_count* key_rfcnt = &hw->smac_cache->id_to_key[i];

	struct rte_ether_addr* key_copy;
	key_copy = n3k_malloc(N3K_MGMT_SMAC_TBL_CACHE_NAME, sizeof(*key_copy), 0);
	if (key_copy == NULL) {
		rc = -ENOMEM;
		goto release_lock;
	}
	memcpy(key_copy, key, sizeof(*key));

	*smac_id = i;
	rc = n3k_mgmt_smac_tbl_set(hw, *smac_id, key_copy);
	if (rc)
		goto free_key;

	key_rfcnt->key = key_copy;
	rte_atomic32_set(&key_rfcnt->ref_count, 1);

	uintptr_t id_as_ptr = (uintptr_t)*smac_id;
	rc = rte_hash_add_key_data(hw->smac_cache->key_to_id, key_rfcnt->key,
		(void*)id_as_ptr);
	if (rc < 0) {
		N3K_MGMT_SMAC_LOG(ERR, "Error while adding entry to cache");
		goto clear_cache_entry;
	}

	rte_atomic32_inc(&hw->smac_cache->total_entries);

	N3K_MGMT_SMAC_LOG(INFO,
		"New smac added successfully to cache: smac_id=%" PRIu8,
		*smac_id);

	goto release_lock;

clear_cache_entry:
	key_rfcnt->key = NULL;
	rte_atomic32_clear(&key_rfcnt->ref_count);
free_key:
	n3k_free(key_copy);
release_lock:
	rte_rwlock_write_unlock(&hw->smac_cache->lock);
	return rc;
}

int n3k_mgmt_smac_tbl_cache_find(struct n3k_mgmt_hw *hw,
	const struct rte_ether_addr* key,
	uint8_t* smac_id)
{
	int rc;

	rc = find_cache_val_by_key(hw, key, smac_id);
	if (rc < 0) {
		if (N3K_MGMT_SMAC_SHOULD_LOG(DEBUG)) {
			N3K_MGMT_SMAC_LOG(DEBUG, "Couldn't find id for key: ret=%d", rc);
			n3k_mgmt_smac_tbl_addr_dump(key);
		}
		return rc;
	}

	if (N3K_MGMT_SMAC_SHOULD_LOG(DEBUG)) {
		N3K_MGMT_SMAC_LOG(DEBUG,
			"Found id for key: smac_id=%" PRIu8, *smac_id);
		n3k_mgmt_smac_tbl_addr_dump(key);
	}

	return 0;
}

int n3k_mgmt_smac_tbl_cache_get_by_id(struct n3k_mgmt_hw *hw,
	uint8_t smac_id, struct rte_ether_addr* out_key)
{
	int rc;

	if (smac_id >= N3K_MGMT_SMAC_TBL_MAX_ENTRIES) {
		N3K_MGMT_SMAC_LOG(DEBUG, "Invalid smac_id=%" PRIu8, smac_id);
		return -ENOENT;
	}

	rte_rwlock_read_lock(&hw->smac_cache->lock);

	if (hw->smac_cache->id_to_key[smac_id].key == NULL) {
		N3K_MGMT_SMAC_LOG(DEBUG,
			"Couldn't get entry by id: smac_id=%" PRIu8, smac_id);
		rc = -ENOENT;
		goto release_lock;
	}
	memcpy(out_key, hw->smac_cache->id_to_key[smac_id].key, sizeof(*out_key));

	if (N3K_MGMT_SMAC_SHOULD_LOG(DEBUG)) {
		N3K_MGMT_SMAC_LOG(DEBUG, "Found entry by id: smac_id=%" PRIu8, smac_id);
		n3k_mgmt_smac_tbl_addr_dump(out_key);
	}

	rc = 0;

release_lock:
	rte_rwlock_read_unlock(&hw->smac_cache->lock);
	return rc;
}

uint32_t n3k_mgmt_smac_tbl_cache_get_ref_count(struct n3k_mgmt_hw *hw,
	uint8_t smac_id)
{
	if (smac_id >= N3K_MGMT_SMAC_TBL_MAX_ENTRIES)
		return 0;
	return rte_atomic32_read(&hw->smac_cache->id_to_key[smac_id].ref_count);
}

int n3k_mgmt_smac_tbl_cache_yield(struct n3k_mgmt_hw *hw, uint8_t smac_id)
{
	int rc;
	struct _key_with_ref_count* entry;

	if (smac_id >= N3K_MGMT_SMAC_TBL_MAX_ENTRIES) {
		N3K_MGMT_SMAC_LOG(ERR, "Invalid smac_id=%" PRIu8, smac_id);
		return -ENOENT;
	}

	entry = &hw->smac_cache->id_to_key[smac_id];

	rte_rwlock_read_lock(&hw->smac_cache->lock);
	if (entry->key == NULL) {
		rte_rwlock_read_unlock(&hw->smac_cache->lock);
		N3K_MGMT_SMAC_LOG(WARNING,
			"Smac tbl cache entry already cleared: smac_id=%" PRIu8,
			smac_id);
		return -ENOENT;
	}
	if (!rte_atomic32_dec_and_test(&entry->ref_count)) {
		rte_rwlock_read_unlock(&hw->smac_cache->lock);
		N3K_MGMT_SMAC_LOG(INFO,
			"Smac tbl cache entry still in use. Not removing: "
			"smac_id=%" PRIu8 ", ref_count=%" PRIu32,
			smac_id, rte_atomic32_read(&entry->ref_count));
		return 0;
	}
	rte_rwlock_read_unlock(&hw->smac_cache->lock);

	rte_rwlock_write_lock(&hw->smac_cache->lock);
	if (entry->key == NULL) {
		rc = -ENOENT;
		N3K_MGMT_SMAC_LOG(WARNING,
			"Smac tbl cache entry already cleared: smac_id=%" PRIu8,
			smac_id);
		goto release_lock;
	}
	if (rte_atomic32_read(&entry->ref_count) != 0) {
		rc = 0;
		N3K_MGMT_SMAC_LOG(INFO,
			"Smac tbl cache entry still in use. Not removing: "
			"smac_id=%" PRIu8 ", ref_count=%" PRIu32,
			smac_id, rte_atomic32_read(&entry->ref_count));
		goto release_lock;
	}

	rte_atomic32_dec(&hw->smac_cache->total_entries);

	rc = rte_hash_del_key(hw->smac_cache->key_to_id, entry->key);
	RTE_ASSERT(rc > 0);
	// ref_count is 0, we leave it initialized for other uses
	n3k_free(entry->key);
	entry->key = NULL;

	N3K_MGMT_SMAC_LOG(INFO,
		"Successfully removed smac tbl cache entry: smac_id=%" PRIu8,
		smac_id);

	// we do not remove from hw because hw does not support such an
	// operation. instead we know which entries can be used from the
	// id_to_key table and we simply overwrite unused entries

	rc = 0;

release_lock:
	rte_rwlock_write_unlock(&hw->smac_cache->lock);
	return rc;
}
