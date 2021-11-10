/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_dump.h"
#include "n3k_mgmt_general_flow_stats.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_init/n3k_mgmt_init_port.h"
#include "regs/n3k_mgmt_regs.h"

struct n3k_mgmt_general_flow_stats n3k_mgmt_general_flow_stats_get(struct n3k_mgmt_hw *hw)
{
	struct n3k_mgmt_general_flow_stats stats;

	stats.def_hit_cnt = csr_read(hw, MKS_CHIP_MKS_LKUP_BASE, MKS_LKUP_LKUP_DEF_HIT_CNT);
	stats.def_miss_cnt = csr_read(hw, MKS_CHIP_MKS_LKUP_BASE, MKS_LKUP_LKUP_DEF_MISS_CNT);
	stats.psg_hit_cnt = csr_read(hw, MKS_CHIP_MKS_LKUP_BASE, MKS_LKUP_LKUP_PSG_HIT_CNT);
	stats.psg_miss_cnt = csr_read(hw, MKS_CHIP_MKS_LKUP_BASE, MKS_LKUP_LKUP_PSG_MISS_CNT);
	stats.emlc_total_entries = csr_read(hw, MKS_CHIP_LKUP_BASE, LKUP_EMLC_TOTAL_ENTRIES);

	return stats;
}

void n3k_mgmt_general_flow_stats_dump(struct n3k_mgmt_hw *hw)
{
	struct n3k_mgmt_general_flow_stats stats = n3k_mgmt_general_flow_stats_get(hw);
	n3k_mgmt_general_flow_stats_struct_dump(&stats);
}
