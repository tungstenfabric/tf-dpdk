/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_REGS_CTRL_H_
#define _PMD_NET_N3K_MGMT_REGS_CTRL_H_

#include "n3k_mgmt_hw.h"
#include "regs/n3k_mgmt_flow_tbl_regs_api.h"
#include "regs/n3k_mgmt_flow_tracker_regs_api.h"
#include "regs/n3k_mgmt_regs.h"
#include "regs/n3k_mgmt_tunnel_tbl_regs_api.h"

enum n3k_mgmt_ctrl_reg_type {
	N3K_MGMT_CTRL_REG_FT_COMMAND,
	N3K_MGMT_CTRL_REG_FT_STAT,
	N3K_MGMT_CTRL_REG_LKUP_EMLC_MGMT_CTRL,
	N3K_MGMT_CTRL_REG_TUNNEL_TBL_STS,
	N3K_MGMT_CTRL_REG_VPLKP_EMLC_GEN_CTRL,
	N3K_MGMT_CTRL_REG_VPLKP_EMLC_MGMT_CTRL,
	N3K_MGMT_CTRL_REG_PARS_STATS_CFG,
	N3K_MGMT_CTRL_REG_MOD_STATS_CFG,
};

int
n3k_mgmt_ctrl_reg_is_busy(struct n3k_mgmt_hw *hw, enum n3k_mgmt_ctrl_reg_type type);

int
n3k_mgmt_poll_ctrl_reg_with_result(struct n3k_mgmt_hw *hw,
	enum n3k_mgmt_ctrl_reg_type type, uint32_t *reg);
int
n3k_mgmt_poll_ctrl_reg(struct n3k_mgmt_hw *hw, enum n3k_mgmt_ctrl_reg_type type);


enum n3k_mgmt_reg_tbl_type {
	N3K_MGMT_REG_TBL_FLOW_TBL_RES,
	N3K_MGMT_REG_TBL_FLOW_TBL_KEY,
	N3K_MGMT_REG_TBL_FLOW_TRACKER_DATA,
	N3K_MGMT_REG_TBL_TUNNEL_TBL_DATA,
};

void
n3k_mgmt_program_reg_tbl(struct n3k_mgmt_hw *hw, enum n3k_mgmt_reg_tbl_type type,
	const void* regs_table);
void
n3k_mgmt_read_reg_tbl(struct n3k_mgmt_hw *hw, enum n3k_mgmt_reg_tbl_type type,
	void* regs_table);
void
n3k_mgmt_dump_reg_tbl(enum n3k_mgmt_reg_tbl_type type, const void* regs_table);

#endif /* _PMD_NET_N3K_MGMT_REGS_CTRL_H_ */
