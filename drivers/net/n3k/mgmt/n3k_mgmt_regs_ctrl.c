/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_bitwise.h"
#include "n3k_mgmt_conversions.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_regs_ctrl.h"
#include "n3k_mgmt_init/n3k_mgmt_init_port.h"

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <rte_cycles.h>

/* Checking for busy bit of mgmt registers */

struct wait_reg_info {
	uintptr_t reg_base;
	uintptr_t reg_offset;
	uint8_t busy_bit_shift;
	uint8_t busy_bit_value; // value of the bit when the operation is busy
};

static const struct wait_reg_info wait_regs[] = {
	[N3K_MGMT_CTRL_REG_FT_COMMAND] = {
		.reg_base = MKS_CHIP_MKS_FLOW_TRACKER_BASE,
		.reg_offset = MKS_FLOW_TRACKER_FT_COMMAND,
		.busy_bit_shift = MKS_FLOW_TRACKER_FT_COMMAND_BUSY_SHIFT,
		.busy_bit_value = 1,
	},
	[N3K_MGMT_CTRL_REG_FT_STAT] = {
		.reg_base = MKS_CHIP_MKS_FLOW_TRACKER_BASE,
		.reg_offset = MKS_FLOW_TRACKER_FT_STAT,
		.busy_bit_shift = MKS_FLOW_TRACKER_FT_STAT_INIT_DONE_SHIFT,
		.busy_bit_value = 0,
	},
	[N3K_MGMT_CTRL_REG_LKUP_EMLC_MGMT_CTRL] = {
		.reg_base = MKS_CHIP_LKUP_BASE,
		.reg_offset = LKUP_EMLC_MGMT_CTRL,
		.busy_bit_shift = LKUP_EMLC_MGMT_CTRL_BUSY_SHIFT,
		.busy_bit_value = 1,
	},
	[N3K_MGMT_CTRL_REG_TUNNEL_TBL_STS] = {
		.reg_base = MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE,
		.reg_offset = MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_STS,
		.busy_bit_shift = MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_STS_BUSY_SHIFT,
		.busy_bit_value = 1,
	},
	[N3K_MGMT_CTRL_REG_VPLKP_EMLC_GEN_CTRL] = {
		.reg_base = MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE,
		.reg_offset = MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_GEN_CTRL,
		.busy_bit_shift = MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_GEN_CTRL_INIT_DONE_SHIFT,
		.busy_bit_value = 0,
	},
	[N3K_MGMT_CTRL_REG_VPLKP_EMLC_MGMT_CTRL] = {
		.reg_base = MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE,
		.reg_offset = MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL,
		.busy_bit_shift = MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_BUSY_SHIFT,
		.busy_bit_value = 1,
	},
	[N3K_MGMT_CTRL_REG_PARS_STATS_CFG] = {
		.reg_base = MKS_CHIP_MKS_PARS_BASE,
		.reg_offset = MKS_PARS_PARS_STATS_CFG,
		.busy_bit_shift = MKS_PARS_PARS_STATS_CFG_STATUS_RD_ENABLE_SHIFT,
		.busy_bit_value = 1,
	},
	[N3K_MGMT_CTRL_REG_MOD_STATS_CFG] = {
		.reg_base = MKS_CHIP_MKS_MOD_BASE,
		.reg_offset = MKS_MOD_MOD_STATS_CFG,
		.busy_bit_shift = MKS_MOD_MOD_STATS_CFG_STATUS_RD_ENABLE_SHIFT,
		.busy_bit_value = 1,
	},
};

static const char *
get_wait_reg_str(enum n3k_mgmt_ctrl_reg_type type)
{
	switch (type) {
	case N3K_MGMT_CTRL_REG_FT_COMMAND:
		return "FT_COMMAND";
	case N3K_MGMT_CTRL_REG_FT_STAT:
		return "FT_STAT";
	case N3K_MGMT_CTRL_REG_LKUP_EMLC_MGMT_CTRL:
		return "LKUP_EMLC_MGMT_CTRL";
	case N3K_MGMT_CTRL_REG_TUNNEL_TBL_STS:
		return "TUNNEL_TBL_STS";
	case N3K_MGMT_CTRL_REG_VPLKP_EMLC_GEN_CTRL:
		return "VPLKP_EMLC_GEN_CTRL";
	case N3K_MGMT_CTRL_REG_VPLKP_EMLC_MGMT_CTRL:
		return "VPLKP_EMLC_MGMT_CTRL";
	case N3K_MGMT_CTRL_REG_PARS_STATS_CFG:
		return "PARS_STATS_CFG";
	case N3K_MGMT_CTRL_REG_MOD_STATS_CFG:
		return "MOD_STATS_CFG";
	}
	return "UNKNOWN";
}

static int
get_wait_reg_is_busy(struct n3k_mgmt_hw *hw, enum n3k_mgmt_ctrl_reg_type type, uint32_t *reg)
{
	struct wait_reg_info wait_reg = wait_regs[type];
	uint32_t busy_bit_value;
	*reg = csr_read(hw, wait_reg.reg_base, wait_reg.reg_offset);
	busy_bit_value = ((*reg & (1 << wait_reg.busy_bit_shift)) >> wait_reg.busy_bit_shift);
	return busy_bit_value == wait_reg.busy_bit_value;
}

int
n3k_mgmt_ctrl_reg_is_busy(struct n3k_mgmt_hw *hw, enum n3k_mgmt_ctrl_reg_type type)
{
	uint32_t reg;
	return get_wait_reg_is_busy(hw, type, &reg);
}

int
n3k_mgmt_poll_ctrl_reg_with_result(struct n3k_mgmt_hw *hw,
	enum n3k_mgmt_ctrl_reg_type type, uint32_t *reg)
{
	int tries = 5000;

	N3K_MGMT_LOG(REGS, DEBUG, "Polling %s for completion", get_wait_reg_str(type));

	while (tries--) {
		if (!get_wait_reg_is_busy(hw, type, reg))
			return 0;
		rte_delay_ms(1);
	}
	return -ETIMEDOUT;
}

int
n3k_mgmt_poll_ctrl_reg(struct n3k_mgmt_hw *hw, enum n3k_mgmt_ctrl_reg_type type)
{
	uint32_t reg;
	return n3k_mgmt_poll_ctrl_reg_with_result(hw, type, &reg);
}

/* Table registers arrays */

struct reg_tbl_info {
	uintptr_t reg_base;
	uintptr_t reg_offset; // offset to the first register of the table
	uint8_t table_size;
	const char *reg_str;
};

static const struct reg_tbl_info reg_tbls[] = {
	[N3K_MGMT_REG_TBL_FLOW_TBL_RES] = {
		.reg_base = MKS_CHIP_LKUP_BASE,
		.reg_offset = LKUP_EMLC_RES_0,
		.table_size = sizeof(struct lookup_emlc_res_regs) / sizeof(uint32_t),
		.reg_str = "LKUP_EMLC_RES",
	},
	[N3K_MGMT_REG_TBL_FLOW_TBL_KEY] = {
		.reg_base = MKS_CHIP_LKUP_BASE,
		.reg_offset = LKUP_EMLC_KEY_0,
		.table_size = sizeof(struct lookup_emlc_key_regs) / sizeof(uint32_t),
		.reg_str = "LKUP_EMLC_KEY",
	},
	[N3K_MGMT_REG_TBL_FLOW_TRACKER_DATA] = {
		.reg_base = MKS_CHIP_MKS_FLOW_TRACKER_BASE,
		.reg_offset = MKS_FLOW_TRACKER_FT_DATA_0,
		.table_size = sizeof(struct flow_tracker_data_regs) / sizeof(uint32_t),
		.reg_str = "MKS_FLOW_TRACKER_FT_DATA",
	},
	[N3K_MGMT_REG_TBL_TUNNEL_TBL_DATA] = {
		.reg_base = MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE,
		.reg_offset = MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_0,
		.table_size = sizeof(struct tunnel_tbl_data_regs) / sizeof(uint32_t),
		.reg_str = "MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA",
	},
};

static const char *get_reg_tbl_str(enum n3k_mgmt_reg_tbl_type type)
{
	switch (type) {
	case N3K_MGMT_REG_TBL_FLOW_TBL_RES:
		return "Flow Table result";
	case N3K_MGMT_REG_TBL_FLOW_TBL_KEY:
		return "Flow Table key";
	case N3K_MGMT_REG_TBL_FLOW_TRACKER_DATA:
		return "Flow Tracker data";
	case N3K_MGMT_REG_TBL_TUNNEL_TBL_DATA:
		return "Tunnel data";
	}
	return "UNKNOWN";
}

void
n3k_mgmt_program_reg_tbl(struct n3k_mgmt_hw *hw, enum n3k_mgmt_reg_tbl_type type, const void* regs_table)
{
	RTE_ASSERT(regs_table != NULL);

	N3K_MGMT_LOG(REGS, DEBUG, "Programming %s register table", get_reg_tbl_str(type));

	const uint32_t* curr_reg = regs_table;
	uintptr_t curr_reg_offset = reg_tbls[type].reg_offset;
	uint32_t i;

	for (i = 0; i < reg_tbls[type].table_size;
			++curr_reg, curr_reg_offset += sizeof(*curr_reg), ++i)
		csr_write(hw, reg_tbls[type].reg_base, curr_reg_offset, *curr_reg);
}

void
n3k_mgmt_read_reg_tbl(struct n3k_mgmt_hw *hw, enum n3k_mgmt_reg_tbl_type type, void* regs_table)
{
	RTE_ASSERT(regs_table != NULL);

	N3K_MGMT_LOG(REGS, DEBUG, "Reading %s register table", get_reg_tbl_str(type));

	uint32_t* curr_reg = regs_table;
	uintptr_t curr_reg_offset = reg_tbls[type].reg_offset;
	uint32_t i;

	for (i = 0; i < reg_tbls[type].table_size;
			++curr_reg, curr_reg_offset += sizeof(*curr_reg), ++i)
		*curr_reg = csr_read(hw, reg_tbls[type].reg_base, curr_reg_offset);
}

void
n3k_mgmt_dump_reg_tbl(enum n3k_mgmt_reg_tbl_type type, const void* regs_table)
{
	RTE_ASSERT(regs_table != NULL);

	const uint32_t* curr_reg = regs_table;
	uint32_t i;

	for (i = 0; i < reg_tbls[type].table_size; ++curr_reg, ++i) {
		printf("%s_%u_t = 0x%.8" PRIx32 ", 0b%s\n",
			reg_tbls[type].reg_str, i, *curr_reg,
			n3k_mgmt_convert_uint_to_bits_str(*curr_reg, 32));
	}
}
