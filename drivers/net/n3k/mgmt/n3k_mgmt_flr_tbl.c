/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_bitwise.h"
#include "n3k_mgmt_flr_cmd_queue.h"
#include "n3k_mgmt_flr_command.h"
#include "n3k_mgmt_flr_flow_tbl.h"
#include "n3k_mgmt_flr_pkts.h"
#include "n3k_mgmt_flr_tbl.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_log.h"

#include <rte_pmd_n3k_flow_tbl_mgmt.h>

int n3k_mgmt_flr_flow_schedule_add(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_create *flow_create_cmd,
	struct n3k_mgmt_flow_tbl_handle **handle)
{
	int ret;

	ret = n3k_mgmt_action_entries_add(hw, &flow_create_cmd->flow.action);
	if (ret < 0)
		return ret;
	ret = n3k_mgmt_key_entries_add(hw, &flow_create_cmd->flow.key);
	if (ret < 0)
		goto remove_action;

	ret = n3k_mgmt_flr_flow_tbl_schedule_add(hw, flow_create_cmd, handle);
	if (ret) {
		N3K_MGMT_FLR_LOG(ERR, "Failed to schedule add flow tbl entry: ret=%d", ret);
		goto remove_key;
	}

	return 0;

remove_key:
	{
		int remove_ret = n3k_mgmt_key_entries_remove(hw, &flow_create_cmd->flow.key);
		if (remove_ret < 0) {
			N3K_MGMT_FLR_LOG(ERR,
				"Failed to remove key related table entries: ret=%d",
				remove_ret);
		}
	}
remove_action:
	{
		int remove_ret = n3k_mgmt_action_entries_remove(hw, &flow_create_cmd->flow.action);
		if (remove_ret < 0) {
			N3K_MGMT_FLR_LOG(ERR,
				"Failed to remove action related table entries: ret=%d",
				remove_ret);
		}
	}
	return ret;
}

int
n3k_mgmt_flr_flow_schedule_del(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_destroy *flow_del_cmd)
{
	return n3k_mgmt_flr_flow_tbl_schedule_del(hw, flow_del_cmd);
}
