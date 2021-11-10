/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_FLR_TBL_H_
#define _PMD_NET_N3K_MGMT_FLR_TBL_H_

#include "n3k_mgmt_flr_command.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_tbl.h"

#include <rte_pmd_n3k_flow_tbl_mgmt.h>
#include <rte_pmd_n3k_tunnel_tbl_mgmt.h>

/*
 * Schedules adding given flow into flow table and adds some of smac, tunnel
 * and vplkp table entries depending on the flow type.
 * Updates flow_create_cmd.flow.action.action_raw.encap_action.tunnel_idx to
 * id used to store given tunnel and
 * flow_create_cmd.flow.action.tunnel.tunnel_raw.l2.src_phy_port_id to id used
 * to store given smac in case of ENCAP flows.
 * flow_create_cmd shouldn't be used after successful call. Because of that
 * one of the parameters is pointer to flow description (desc). It will be filled
 * with all the necessary information about the flow and can be used after
 * this call.
 */
int n3k_mgmt_flr_flow_schedule_add(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_create *flow_create_cmd,
	struct n3k_mgmt_flow_entry_description *desc);

int n3k_mgmt_flr_flow_schedule_del(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_destroy *flow_del_cmd);

#endif /* _PMD_NET_N3K_MGMT_FLR_TBL_H_ */
