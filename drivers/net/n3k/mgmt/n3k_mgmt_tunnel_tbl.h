/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_TUNNEL_TBL_H_
#define _PMD_NET_N3K_MGMT_TUNNEL_TBL_H_

#include "n3k_mgmt_hw.h"

#include <rte_pmd_n3k_tunnel_tbl_mgmt.h>
#include <stdint.h>

int n3k_mgmt_tunnel_tbl_set(struct n3k_mgmt_hw *hw, uint16_t tunnel_idx,
	const struct n3k_mgmt_tunnel_tbl_entry *entry);
int n3k_mgmt_tunnel_tbl_get(struct n3k_mgmt_hw *hw, uint16_t tunnel_idx,
