/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_BASIC_FLOW_STATS_H_
#define _PMD_NET_N3K_MGMT_BASIC_FLOW_STATS_H_

#include "n3k_mgmt_hw.h"

#include <rte_pmd_n3k_flow_tbl_mgmt.h>
#include <stdint.h>

struct n3k_mgmt_general_flow_stats n3k_mgmt_general_flow_stats_get(struct n3k_mgmt_hw *hw);
void n3k_mgmt_general_flow_stats_dump(struct n3k_mgmt_hw *hw);

#endif /* _PMD_NET_N3K_MGMT_BASIC_FLOW_STATS_H_ */
