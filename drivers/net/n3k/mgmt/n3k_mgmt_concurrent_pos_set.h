/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_CONCURRENT_POS_SET_H_
#define _PMD_NET_N3K_MGMT_CONCURRENT_POS_SET_H_

#include <rte_atomic.h>
#include <stdlib.h>

struct n3k_mgmt_concurrent_pos_set {
	size_t size;
	rte_atomic32_t *subtree_on_cnt;
};

int n3k_mgmt_conc_set_init(struct n3k_mgmt_concurrent_pos_set *set, size_t size);
void n3k_mgmt_conc_set_deinit(struct n3k_mgmt_concurrent_pos_set *set);

void n3k_mgmt_conc_set_turn_on_pos(struct n3k_mgmt_concurrent_pos_set *set, size_t pos);
void n3k_mgmt_conc_set_turn_off_pos(struct n3k_mgmt_concurrent_pos_set *set, size_t pos);
int n3k_mgmt_conc_set_find_next_turned_on_pos(
	const struct n3k_mgmt_concurrent_pos_set *set, size_t pos, size_t *next_pos);

#endif /* _PMD_NET_N3K_MGMT_CONCURRENT_POS_SET_H_ */
