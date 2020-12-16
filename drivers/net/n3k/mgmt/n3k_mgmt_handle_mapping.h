/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_FLR_SIM_HANDLE_MAPPING_H_
#define _PMD_NET_N3K_MGMT_FLR_SIM_HANDLE_MAPPING_H_

#include "n3k_mgmt_hw.h"

#include <stdint.h>

int n3k_mgmt_handle_mapping_init(struct n3k_mgmt_hw *hw);

int n3k_mgmt_unmap_local_to_hw_handle(struct n3k_mgmt_hw *hw,
	uint32_t local_handle);
int n3k_mgmt_map_local_to_hw_handle(struct n3k_mgmt_hw *hw,
	uint32_t local_handle, uint32_t hw_handle);
