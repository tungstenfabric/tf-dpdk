/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_flr_read_reg_32.h"

#include "n3k_mgmt_bitwise.h"
#include "n3k_mgmt_flr_cmd_queue.h"
#include "n3k_mgmt_flr_pkts_blocks.h"
#include "n3k_mgmt_flr_pkts.h"
#include "n3k_mgmt_log.h"

int n3k_mgmt_flr_read_reg_32_append_pkt_blocks(__rte_unused struct n3k_mgmt_hw * hw,
	const struct n3k_mgmt_flr_command_read_reg_32 *reg_32_read,
	struct rte_mbuf *pkt)
{
	uint32_t base = reg_32_read->base;
	uint32_t offset = reg_32_read->offset;
	return n3k_mgmt_flr_add_read_block(pkt, base + offset, sizeof(uint32_t));
}

int
n3k_mgmt_flr_schedule_read_reg_32(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_read_reg_32 *reg_32_read)
{
	int ret;
	ret = n3k_mgmt_flr_cmd_queue_push_command(hw, container_of(reg_32_read,
		struct n3k_mgmt_flr_command, reg_32_read));
	if (ret < 0) {
		N3K_MGMT_LOG(FLR_DEFAULT, ERR, "Failed to push reg 32 read command to cmd queue");
		return ret;
	}

	return 0;
}

int
n3k_mgmt_flr_sync_read_reg_32(struct n3k_mgmt_hw *hw, uint32_t base,
	uint32_t reg, uint32_t *val)
{
	int ret = 0;
	struct n3k_mgmt_flr_command* reg_read_cmd =
		n3k_mgmt_flr_command_alloc(hw, N3K_MGMT_FLR_COMMAND_READ_REG_32);
	if (reg_read_cmd == NULL) {
		N3K_MGMT_LOG(FLR_DEFAULT, ERR,
			"couldn't allocate flr command for read 32 reg");
		return -ENOENT;
	}
	reg_read_cmd->reg_32_read.base = base;
	reg_read_cmd->reg_32_read.offset = reg;
	ret = n3k_mgmt_flr_schedule_read_reg_32(hw, &reg_read_cmd->reg_32_read);
	if (ret) {
		N3K_MGMT_LOG(FLR_DEFAULT, ERR, "flow read 32 reg scheduling failed %d", ret);
		n3k_mgmt_flr_command_free(hw, reg_read_cmd);
		return ret;
	}
	// busy wait
	while (rte_atomic16_read(&reg_read_cmd->reg_32_read.done) == 0)
		rte_delay_ms(1);
	if (reg_read_cmd->reg_32_read.error_code) {
		N3K_MGMT_LOG(FLR_DEFAULT, ERR, "flow read 32 reg failed %d",
			reg_read_cmd->reg_32_read.error_code);
		ret = reg_read_cmd->reg_32_read.error_code;
	}
	*val = reg_read_cmd->reg_32_read.out_result;
	n3k_mgmt_flr_command_free(hw, reg_read_cmd);
	return ret;
}

int
n3k_mgmt_flr_read_reg_32_handle_response(__rte_unused struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_read_reg_32 *reg_32_read,
	const struct n3k_mgmt_flr_parsed_block_range *parsed_cmd,
	bool retry_limit_reached)
{
	if (parsed_cmd == NULL) {
		if (retry_limit_reached)
			goto error_unlock_sync;
		return -EAGAIN;
	}

	const struct n3k_mgmt_flr_parsed_block *read_reg_block =
		&parsed_cmd->blocks[N3K_MGMT_FLR_READ_REG_32];

	if (read_reg_block->error_code) {
		if (retry_limit_reached)
			goto error_unlock_sync;
		return -EAGAIN;
	}

	uint32_t out_result;
	memcpy_bswap32(&out_result, read_reg_block->data,
		sizeof(uint32_t));
	N3K_MGMT_LOG(REGS, DEBUG, "read_reg_32 succeeded with result: %" PRIu32,
		out_result);
	reg_32_read->error_code = 0;
	reg_32_read->out_result = out_result;
	rte_atomic16_inc(&reg_32_read->done);
	return 0;

error_unlock_sync:
	reg_32_read->error_code = -EIO;
	rte_atomic16_inc(&reg_32_read->done);
	return -EIO;
}
