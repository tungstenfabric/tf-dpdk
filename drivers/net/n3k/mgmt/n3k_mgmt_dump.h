/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_DUMP_H_
#define _PMD_NET_N3K_MGMT_DUMP_H_

#include <rte_pmd_n3k_flow_tbl_mgmt.h>
#include <rte_pmd_n3k_smac_tbl_mgmt.h>
#include <rte_pmd_n3k_tunnel_tbl_mgmt.h>
#include <rte_pmd_n3k_vplkp_tbl_mgmt.h>
#include <rte_pmd_n3k_mp.h>

/* Flows */

void n3k_mgmt_flow_tbl_key_dump(const struct n3k_mgmt_flow_tbl_key *key);
void n3k_mgmt_flow_tbl_action_dump(const struct n3k_mgmt_flow_tbl_action *action);
void n3k_mgmt_flow_tbl_entry_dump(const struct n3k_mgmt_flow_tbl_entry *entry);

void n3k_mgmt_flow_key_dump(const struct n3k_mgmt_flow_key *key);
void n3k_mgmt_flow_action_dump(const struct n3k_mgmt_flow_action *action);
void n3k_mgmt_flow_entry_dump(const struct n3k_mgmt_flow_entry *entry);

void n3k_mgmt_general_flow_stats_struct_dump(const struct n3k_mgmt_general_flow_stats *gf_stats);
void n3k_mgmt_flow_tracker_struct_dump(const struct n3k_mgmt_flow_tracker_stats *ft_stats);

/* Tunnels */

void n3k_mgmt_tunnel_tbl_entry_dump(const struct n3k_mgmt_tunnel_tbl_entry *entry);
void n3k_mgmt_tunnel_entry_dump(const struct n3k_mgmt_tunnel_entry *entry);

/* Vplkps */

void n3k_mgmt_vplkp_tbl_res_dump(const struct n3k_mgmt_vplkp_tbl_res *res);

/* SMACs */

void n3k_mgmt_smac_tbl_addr_dump(const struct rte_ether_addr *addr);

/* Port */

void n3k_mgmt_port_stats_struct_dump(const struct n3k_mgmt_port_stats *port_stats);

/* MP structs */

void n3k_mgmt_mp_flow_dump(const struct n3k_mgmt_mp_flow_response *flow);
void n3k_mgmt_mp_flow_raw_dump(const struct n3k_mgmt_mp_flow_raw_response *flow_raw);
void n3k_mgmt_mp_tunnel_dump(const struct n3k_mgmt_mp_tunnel_response *tunnel);
void n3k_mgmt_mp_tunnel_raw_dump(const struct n3k_mgmt_mp_tunnel_raw_response *tunnel_raw);
void n3k_mgmt_mp_smac_dump(const struct n3k_mgmt_mp_smac_response *smac);
void n3k_mgmt_mp_vplkp_dump(const struct n3k_mgmt_mp_vplkp_response *vplkp);
void n3k_mgmt_mp_flow_tracker_dump(const struct n3k_mgmt_mp_flow_tracker_response *flow_tracker);
void n3k_mgmt_mp_port_dump(const struct n3k_mgmt_mp_port_response *port);
void n3k_mgmt_mp_vf0_stats_dump(const struct n3k_mgmt_mp_vf0_stats_response *stats);

#endif /* _PMD_NET_N3K_MGMT_DUMP_H_ */
