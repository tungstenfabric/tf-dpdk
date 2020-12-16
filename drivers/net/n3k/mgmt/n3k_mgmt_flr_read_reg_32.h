/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _N3K_MGMT_FLR_REG_32_READ_
#define _N3K_MGMT_FLR_REG_32_READ_

#include "n3k_mgmt_flr_pkts_blocks.h"

enum n3k_mgmt_flr_read_reg_32_blocks_order {
	N3K_MGMT_FLR_READ_REG_32,
};

int
n3k_mgmt_flr_sync_read_reg_32(struct n3k_mgmt_hw *hw, uint32_t base,
	uint32_t reg, uint32_t *val);

int
n3k_mgmt_flr_read_reg_32_handle_response(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_read_reg_32 *reg_32_read,
	const struct n3k_mgmt_flr_parsed_pkt *parsed_pkt,
	bool retry_limit_reached);

int n3k_mgmt_flr_read_reg_32_append_pkt_blocks(struct n3k_mgmt_hw * hw,
	const struct n3k_mgmt_flr_command_read_reg_32 *reg_32_read,
	struct rte_mbuf *pkt);

int
