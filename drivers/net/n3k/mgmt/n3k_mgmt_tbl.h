/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_TBL_H_
#define _PMD_NET_N3K_MGMT_TBL_H_

#include "n3k_mgmt_hw.h"

#include <rte_pmd_n3k_flow_tbl_mgmt.h>
#include <rte_pmd_n3k_tunnel_tbl_mgmt.h>

int n3k_mgmt_flow_get(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle,
	struct n3k_mgmt_flow_entry *flow);
int n3k_mgmt_tunnel_get(struct n3k_mgmt_hw *hw, uint16_t tunnel_idx,
	struct n3k_mgmt_tunnel_entry *tunnel);
int n3k_mgmt_get_key_action_tbl_entries(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_entry* flow_tbl_entry,
	struct n3k_mgmt_flow_entry *flow);

/*
 * Adds given flow into flow table and into some of smac, tunnel and vplkp
 * tables depending on flow type.
 * Updates flow.action.action_raw.encap_action.tunnel_idx to id used to store
 * given tunnel and flow.action.tunnel.tunnel_raw.l2.src_phy_port_id to id used
 * to store given smac in case of ENCAP flows.
 */
int n3k_mgmt_flow_add(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_entry *flow,
	struct n3k_mgmt_flow_tbl_handle **handle);
int n3k_mgmt_key_entries_add(struct n3k_mgmt_hw *hw, struct n3k_mgmt_flow_key *key);
int n3k_mgmt_action_entries_add(struct n3k_mgmt_hw *hw, struct n3k_mgmt_flow_action *action);

/*
 * Adds given tunnel into tunnel and smac table.
 * Updates tunnel.tunnel_raw.l2.src_phy_port_id to id used to store given smac.
 */
int n3k_mgmt_tunnel_add(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_tunnel_entry *tunnel,
	uint16_t *tunnel_idx);

int n3k_mgmt_flow_remove(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle,
	const struct n3k_mgmt_flow_entry *flow);
int n3k_mgmt_tunnel_remove(struct n3k_mgmt_hw *hw, uint16_t tunnel_idx,
	const struct n3k_mgmt_tunnel_entry *tunnel);

int n3k_mgmt_key_entries_remove(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flow_key *key);
int n3k_mgmt_action_entries_remove(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flow_action *action);

/*
 * n3k_mgmt_flow_entry_description stores only table ids of the given flow. It's also
 * the minimum information needed to remove the flow.
 */
struct n3k_mgmt_flow_entry_description {
	struct n3k_mgmt_flow_tbl_handle *flow_handle;

	// Key related table ids
	struct n3k_mgmt_flow_key_description {
		uint32_t vplkp_id;
	} key;

	// Action related table ids
	struct n3k_mgmt_flow_action_description {
		uint32_t tunnel_id;
		uint8_t smac_id;
	} action;

	bool use_key_desc;
	bool use_action_desc;
};

/*
 * Fills n3k_mgmt_flow_entry_description based on given flow and handle.
 * Handle can be NULL - it should be later set manually.
 */
void n3k_mgmt_flow_entry_to_description(struct n3k_mgmt_flow_tbl_handle *handle,
	const struct n3k_mgmt_flow_entry *flow,
	struct n3k_mgmt_flow_entry_description *desc);

int n3k_mgmt_flow_entry_description_remove(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_entry_description *desc);
int n3k_mgmt_flow_key_description_remove(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flow_key_description *desc);
int n3k_mgmt_flow_action_description_remove(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flow_action_description *desc);

int n3k_mgmt_flow_dump(struct n3k_mgmt_hw *hw, struct n3k_mgmt_flow_tbl_handle *handle);
int n3k_mgmt_tunnel_dump(struct n3k_mgmt_hw *hw, uint16_t tunnel_idx);

#endif /* _PMD_NET_N3K_MGMT_TBL_H_ */
