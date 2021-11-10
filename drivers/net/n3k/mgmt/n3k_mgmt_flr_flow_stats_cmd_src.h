/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _N3K_MGMT_FLR_FLOW_STATS_CMD_SRC_H_
#define _N3K_MGMT_FLR_FLOW_STATS_CMD_SRC_H_

struct n3k_mgmt_hw;
struct n3k_mgmt_flr_command;

int n3k_mgmt_flr_flow_stats_cmd_src_init(struct n3k_mgmt_hw *hw);

struct n3k_mgmt_flr_command *n3k_mgmt_flr_flow_stats_cmd_src_pop(
	struct n3k_mgmt_hw *hw);

#endif /* _N3K_MGMT_FLR_FLOW_STATS_CMD_SRC_H_ */
