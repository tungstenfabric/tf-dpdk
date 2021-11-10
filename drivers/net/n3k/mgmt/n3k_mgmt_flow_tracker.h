/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_FLOW_TRACKER_H_
#define _PMD_NET_N3K_MGMT_FLOW_TRACKER_H_

#include "n3k_mgmt_hw.h"
#include "regs/n3k_mgmt_flow_tracker_regs_api.h"
#include "regs/n3k_mgmt_regs.h"

#include <rte_pmd_n3k_flow_tbl_mgmt.h>

#include <stdint.h>

int n3k_mgmt_flow_tracker_init(struct n3k_mgmt_hw *hw);

uint32_t n3k_mgmt_flow_tracker_get_min_stats_id(struct n3k_mgmt_hw* hw);
uint32_t n3k_mgmt_flow_id_to_stats_id(struct n3k_mgmt_hw* hw, uint32_t flow_id);

void n3k_mgmt_flow_tracker_enable_fill_regs(struct n3k_mgmt_hw* hw,
	struct flow_tracker_data_regs *ft_regs, uint8_t ip_prot);
void n3k_mgmt_flow_tracker_disable_fill_regs(struct flow_tracker_data_regs *ft_regs);

int n3k_mgmt_flow_tracker_enable(struct n3k_mgmt_hw* hw, uint32_t stats_id,
	uint8_t ip_prot);
int n3k_mgmt_flow_tracker_write_regs(struct n3k_mgmt_hw* hw, uint32_t stats_id,
	const struct flow_tracker_data_regs *ft_regs);

int n3k_mgmt_flow_tracker_disable(struct n3k_mgmt_hw* hw, uint32_t stats_id);

int n3k_mgmt_flow_tracker_get(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle,
	struct n3k_mgmt_flow_tracker_stats *ft_stats);
int n3k_mgmt_flow_tracker_get_by_regs(struct n3k_mgmt_hw *hw,
	uint32_t stats_id, struct flow_tracker_data_regs *ft_regs);

void n3k_mgmt_flow_tracker_fill_stats(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tracker_stats *ft_stats,
	struct flow_tracker_data_regs *ft_regs);

int n3k_mgmt_flow_tracker_dump(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle);

#endif /* _PMD_NET_N3K_MGMT_FLOW_TRACKER_H_ */
