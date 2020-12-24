/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_FLR_CMD_QUEUE_H_
#define _PMD_NET_N3K_MGMT_FLR_CMD_QUEUE_H_

#include <stdint.h>

struct n3k_mgmt_hw;
struct n3k_mgmt_flr_command;

int n3k_mgmt_flr_cmd_queue_init(struct n3k_mgmt_hw *hw);

int n3k_mgmt_flr_cmd_queue_push_command(
	struct n3k_mgmt_hw *hw, struct n3k_mgmt_flr_command *cmd);
struct n3k_mgmt_flr_command *n3k_mgmt_flr_cmd_queue_process_command(
	struct n3k_mgmt_hw *hw);
struct n3k_mgmt_flr_command *n3k_mgmt_flr_cmd_queue_pop_processed_command(
	struct n3k_mgmt_hw *hw, uint16_t cmd_id);
struct n3k_mgmt_flr_command *n3k_mgmt_flr_cmd_queue_pop_timeouted_command(
	struct n3k_mgmt_hw *hw);

#endif /* _PMD_NET_N3K_MGMT_FLR_CMD_QUEUE_H_ */
