/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _N3K_MGMT_FLR_READY_CMD_HANDLER_
#define _N3K_MGMT_FLR_READY_CMD_HANDLER_

#include "n3k_mgmt_flr_pkts_blocks.h"

typedef struct n3k_mgmt_flr_command *(*n3k_mgmt_flr_rdy_cmd_hdlr_cmd_src_t)(
	struct n3k_mgmt_hw *hw);

int n3k_mgmt_flr_rdy_cmd_hdlr_init(struct n3k_mgmt_hw *hw);

int n3k_mgmt_flr_rdy_cmd_hdlr_register_src(struct n3k_mgmt_hw *hw,
	n3k_mgmt_flr_rdy_cmd_hdlr_cmd_src_t cmd_src);

struct n3k_mgmt_flr_command_pack n3k_mgmt_flr_rdy_cmd_hdlr_process_cmd_pack(
	struct n3k_mgmt_hw *hw);
struct n3k_mgmt_flr_command_pack n3k_mgmt_flr_rdy_cmd_hdlr_pop_processed_cmd_pack(
	struct n3k_mgmt_hw *hw, uint16_t cmd_id);
struct n3k_mgmt_flr_command_pack n3k_mgmt_flr_rdy_cmd_hdlr_pop_timeouted_cmd_pack(
	struct n3k_mgmt_hw *hw);

#endif /* _N3K_MGMT_FLR_READY_CMD_HANDLER_ */
