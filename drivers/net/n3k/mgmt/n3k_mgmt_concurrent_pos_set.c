/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_concurrent_pos_set.h"

#include <errno.h>
#include <rte_atomic.h>
#include <rte_pmd_n3k_malloc.h>

int
n3k_mgmt_conc_set_init(struct n3k_mgmt_concurrent_pos_set *set, size_t size)
{
	set->size = 1;
	while (set->size < size)
		set->size <<= 1;
	set->subtree_on_cnt = n3k_zmalloc("n3k_conc_set",
		(set->size << 1) * sizeof(*set->subtree_on_cnt), 0);
	if (set->subtree_on_cnt == NULL)
		return -ENOMEM;
	size_t i;
	for (i = 0; i < (set->size << 1); ++i)
		rte_atomic32_init(&set->subtree_on_cnt[i]);
	return 0;
}

void
n3k_mgmt_conc_set_deinit(struct n3k_mgmt_concurrent_pos_set *set)
{
	n3k_free(set->subtree_on_cnt);
}

void
n3k_mgmt_conc_set_turn_on_pos(struct n3k_mgmt_concurrent_pos_set *set, size_t pos)
{
	pos += set->size;
	while (pos) {
		rte_atomic32_inc(&set->subtree_on_cnt[pos]);
		pos >>= 1;
	}
}

void
n3k_mgmt_conc_set_turn_off_pos(struct n3k_mgmt_concurrent_pos_set *set, size_t pos)
{
	pos += set->size;
	while (pos) {
		rte_atomic32_dec(&set->subtree_on_cnt[pos]);
		pos >>= 1;
	}
}

int
n3k_mgmt_conc_set_find_next_turned_on_pos(
	const struct n3k_mgmt_concurrent_pos_set *set, size_t pos, size_t *next_pos)
{
	pos += set->size;

	if (rte_atomic32_read(&set->subtree_on_cnt[pos]))
		goto pos_found;

	while (pos) {
		if ((pos & 1) == 0 && rte_atomic32_read(&set->subtree_on_cnt[pos + 1])) {
			++pos;
			break;
		}
		pos >>= 1;
	}
	if (pos == 0)
		return -ENOENT;

	while (pos < set->size) {
		pos <<= 1;
		if (rte_atomic32_read(&set->subtree_on_cnt[pos]) == 0)
			++pos;
	}

pos_found:
	*next_pos = pos - set->size;
	return 0;
}
