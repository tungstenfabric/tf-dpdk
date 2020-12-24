/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_FLOW_TBL_H_
#define _PMD_NET_N3K_MGMT_FLOW_TBL_H_

#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_flow_tbl_regs_api.h"
#include "n3k_mgmt_flow_tbl_handles.h"

#include <rte_pmd_n3k_flow_tbl_mgmt.h>
#include <stdint.h>

int n3k_mgmt_flow_tbl_fill_res_regs(
    struct n3k_mgmt_hw *hw, struct lookup_emlc_res_regs *res_regs,
    const struct n3k_mgmt_flow_tbl_action *action, uint32_t stats_idx,
    uint32_t recirc_id);
int n3k_mgmt_flow_tbl_fill_key_regs(struct n3k_mgmt_hw *hw,
	struct lookup_emlc_key_regs *key_regs,
	const struct n3k_mgmt_flow_tbl_key *key);

int n3k_mgmt_flow_tbl_fill_regs_mirror(
	struct n3k_mgmt_hw *hw,
	struct lookup_emlc_key_regs *key_regs,
	struct lookup_emlc_res_regs *res_regs,
	const struct n3k_mgmt_flow_tbl_action *action,
	uint32_t recirc_id);

int n3k_mgmt_flow_tbl_add(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flow_tbl_entry *entry,
	struct n3k_mgmt_flow_tbl_handle **handle);
int n3k_mgmt_flow_tbl_get(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle,
	struct n3k_mgmt_flow_tbl_entry *entry);
int n3k_mgmt_flow_tbl_del(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle);
int n3k_mgmt_flow_tbl_dump(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle);

int n3k_mgmt_flow_tbl_add_by_regs(struct n3k_mgmt_hw *hw,
	const struct lookup_emlc_key_regs *key_regs,
	const struct lookup_emlc_res_regs *res_regs,
	LKUP_EMLC_MGMT_CTRL_t *emlc_mgmt,
	uint32_t *hw_flow_id);
int n3k_mgmt_flow_tbl_del_by_regs(struct n3k_mgmt_hw *hw, uint32_t hw_flow_id);

int n3k_mgmt_flow_tbl_alloc_next_flow_id_nolock(struct n3k_mgmt_hw *hw, uint32_t *flow_id);

int n3k_mgmt_flow_tbl_get_key_res_by_regs(
	struct n3k_mgmt_hw *hw, uint32_t hw_flow_id,
	struct lookup_emlc_key_regs *key_regs,
	struct lookup_emlc_res_regs *res_regs);

int n3k_mgmt_flow_tbl_fill_key_info(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_key *key,
	const struct lookup_emlc_key_regs *key_regs);
int n3k_mgmt_flow_tbl_fill_res_info(struct n3k_mgmt_hw *hw,
	enum n3k_mgmt_tunnel_type encap_type,
	struct n3k_mgmt_flow_tbl_action *action,
	const struct lookup_emlc_res_regs *res_regs);
int n3k_mgmt_flow_tbl_fill_res_mirror_info(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_action *action,
	const struct lookup_emlc_res_regs *mirror_res_regs);

#endif /* _PMD_NET_N3K_MGMT_FLOW_TBL_H_ */
