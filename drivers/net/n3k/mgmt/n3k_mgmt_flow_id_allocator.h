/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_FLOW_ID_ALLOCATOR_H_
#define _PMD_NET_N3K_MGMT_FLOW_ID_ALLOCATOR_H_

#include "n3k_mgmt_hw.h"

int n3k_mgmt_flow_id_allocator_init(struct n3k_mgmt_hw *hw);

int n3k_mgmt_flow_id_alloc(struct n3k_mgmt_hw *hw, uint32_t *handle);
int n3k_mgmt_flow_id_free(struct n3k_mgmt_hw *hw, uint32_t handle);

#endif /* _PMD_NET_N3K_MGMT_FLOW_ID_ALLOCATOR_H_ */
