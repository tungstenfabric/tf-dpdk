/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "mgmt/n3k_mgmt_bitwise.h"
#include "mgmt/n3k_mgmt_flr_pkts.h"
#include "mgmt/n3k_mgmt_init/n3k_mgmt_init_port.h"
#include "mgmt/n3k_mgmt_lock.h"
#include "mgmt/n3k_mgmt_log.h"
#include "mgmt/regs/n3k_mgmt_regs.h"
#include "n3k_mgmt_flr_sim_request_handler.h"

#include <rte_pmd_n3k_malloc.h>
#include <rte_hash.h>

struct waiting_register_write_handler_data {
	uint32_t poll_reg_addr;
	uint8_t bit_offset;
};

union write_reg_handler_data {
	struct waiting_register_write_handler_data wait;
};

typedef int (*write_reg_handler_func)(struct n3k_mgmt_hw *hw, uint32_t reg_addr,
	uint32_t val, const union write_reg_handler_data *data);

struct write_reg_handler {
	write_reg_handler_func func;
	union write_reg_handler_data data;
};

struct write_reg_handler_key_val {
	uint32_t key;
	struct write_reg_handler val;
};

static int
waiting_register_write_handler(struct n3k_mgmt_hw *hw, uint32_t reg_addr,
	uint32_t val, const union write_reg_handler_data *data);

struct write_reg_handler_key_val write_regs_desc[] = {
	{
		.key = MKS_CHIP_LKUP_BASE + LKUP_EMLC_MGMT_CTRL,
		.val = {
			.func = waiting_register_write_handler,
			.data = {
				.wait = {
					.poll_reg_addr = MKS_CHIP_LKUP_BASE +
						LKUP_EMLC_MGMT_CTRL,
					.bit_offset =
						LKUP_EMLC_MGMT_CTRL_BUSY_SHIFT
				}
			}
		}
	},
	{
		.key = MKS_CHIP_MKS_FLOW_TRACKER_BASE +
			MKS_FLOW_TRACKER_FT_COMMAND,
		.val = {
			.func = waiting_register_write_handler,
			.data = {
				.wait = {
					.poll_reg_addr =
						MKS_CHIP_MKS_FLOW_TRACKER_BASE +
						MKS_FLOW_TRACKER_FT_COMMAND,
					.bit_offset =
						MKS_FLOW_TRACKER_FT_COMMAND_BUSY_SHIFT
				}
			}
		}
	},
};

struct n3k_mgmt_flr_sim_request_handler_internal {
	struct rte_hash* reg_to_write_hdl_data;
};

int
n3k_mgmt_flr_sim_request_handler_init(struct n3k_mgmt_hw *hw)
{
	N3K_MGMT_LOG(FLR_SIM, INFO, "Initializing flr sim request handler");

	hw->flr_sim_req_hdl_internal = n3k_zmalloc("n3k_flr_sim_req_hdl",
		sizeof(*hw->flr_sim_req_hdl_internal), 0);
	if (hw->flr_sim_req_hdl_internal == NULL)
		return -ENOMEM;

	static const struct rte_hash_parameters hash_params = {
		.name = "n3k_flr_sim_req_hdl_write_map",
		.entries = sizeof(write_regs_desc),
		.key_len = sizeof(uint32_t),
		.extra_flag = RTE_HASH_EXTRA_FLAGS_EXT_TABLE,
	};
	hw->flr_sim_req_hdl_internal->reg_to_write_hdl_data = rte_hash_create(&hash_params);
	if (hw->flr_sim_req_hdl_internal->reg_to_write_hdl_data == NULL) {
		n3k_free(hw->flr_sim_req_hdl_internal);
		return -rte_errno;
	}

	size_t i;
	for (i = 0; i < RTE_DIM(write_regs_desc); ++i) {
		int ret = rte_hash_add_key_data(
			hw->flr_sim_req_hdl_internal->reg_to_write_hdl_data,
			&write_regs_desc[i].key, &write_regs_desc[i].val);
		if (ret < 0) {
			N3K_MGMT_LOG(FLR_SIM, ERR,
				"Error while adding entry to hash table");
			n3k_free(hw->flr_sim_req_hdl_internal);
			return ret;
		}
	}

	return 0;
}

static int
default_register_write_handler(struct n3k_mgmt_hw *hw, uint32_t reg_addr,
	uint32_t val, __rte_unused const void *data)
{
	csr_write_reg_addr(hw, reg_addr, val);
	return 0;
}

static int
default_register_read_handler(struct n3k_mgmt_hw *hw, uint32_t reg_addr,
	uint32_t *val, __rte_unused const union write_reg_handler_data *data)
{
	*val = csr_read_reg_addr(hw, reg_addr);
	return 0;
}

static bool
get_busy_bit(struct n3k_mgmt_hw *hw,
	const struct waiting_register_write_handler_data *wait_data)
{
	uint32_t busy_bit_val, reg;
	reg = csr_read_reg_addr(hw, wait_data->poll_reg_addr);
	busy_bit_val = (reg & (1 << wait_data->bit_offset)) >> wait_data->bit_offset;
	return busy_bit_val;
}

static int
waiting_register_write_handler(struct n3k_mgmt_hw *hw, uint32_t reg_addr,
	uint32_t val, __rte_unused const union write_reg_handler_data *data)
{
	csr_write_reg_addr(hw, reg_addr, val);
	const struct waiting_register_write_handler_data *wait_data = &data->wait;
	int tries = 5000;
	while (tries--) {
		if (!get_busy_bit(hw, wait_data))
			return 0;
		rte_delay_ms(1);
	}
	return -ETIMEDOUT;
}

static int
read_reg(struct n3k_mgmt_hw *hw, uint32_t reg_addr, uint32_t *val)
{
	return default_register_read_handler(hw, reg_addr, val, NULL);
}

static int
write_reg(struct n3k_mgmt_hw *hw, uint32_t reg_addr, uint32_t val)
{
	struct write_reg_handler *hdl = NULL;
	rte_hash_lookup_data(hw->flr_sim_req_hdl_internal->reg_to_write_hdl_data,
		&reg_addr, (void**)(&hdl));
	if (hdl)
		return hdl->func(hw, reg_addr, val, &hdl->data);
	return default_register_write_handler(hw, reg_addr, val, NULL);
}

static int
execute_block(struct n3k_mgmt_hw *hw, struct n3k_mgmt_flr_parsed_block *block)
{
	uint8_t reg_cnt;
	int ret;
	n3k_mgmt_lock(hw);
	for (reg_cnt = 0; reg_cnt < block->burst_cnt; ++reg_cnt) {
		uint32_t reg_addr = block->start_reg_addr + reg_cnt * 4;
		switch (block->cmd_type) {
		case N3K_MGMT_FLR_BLOCK_HEADER_CMD_TYPE_WRITE:
			ret = write_reg(hw, reg_addr, rte_bswap32(block->data[reg_cnt]));
			if (ret < 0) {
				N3K_MGMT_LOG(FLR_SIM, ERR,
					"Failed to write %u register (%x=%x) from block, ret=%d",
					reg_cnt, reg_addr, block->data[reg_cnt], ret);
				goto release_lock;
			}
			break;
		case N3K_MGMT_FLR_BLOCK_HEADER_CMD_TYPE_READ: {
			uint32_t reg_val;
			ret = read_reg(hw, reg_addr, &reg_val);
			if (ret < 0) {
				N3K_MGMT_LOG(FLR_SIM, ERR,
					"Failed to read %u register (%x) from block, ret=%d",
					reg_cnt, reg_addr, ret);
				goto release_lock;
			}
			block->data[reg_cnt] = rte_bswap32(reg_val);
			break;
		}
		}

	}
release_lock:
	n3k_mgmt_unlock(hw);
	return 0;
}

int
n3k_mgmt_flr_sim_execute_request(struct n3k_mgmt_hw *hw, void *pkt_data, uint16_t pkt_size)
{
	int ret;
	struct n3k_mgmt_flr_parsed_pkt parsed_pkt;
	uint8_t block;

	ret = n3k_mgmt_flr_parse_config_pkt_raw(pkt_data, pkt_size, &parsed_pkt);
	if (ret < 0) {
		N3K_MGMT_LOG(FLR_SIM, ERR, "Couldn't parse packet");
		return -EINVAL;
	}

	for (block = 0; block < parsed_pkt.blocks_num; ++block) {
		ret = execute_block(hw, &parsed_pkt.blocks[block]);
		if (ret < 0)
			return ret;
	}

	return 0;
}
