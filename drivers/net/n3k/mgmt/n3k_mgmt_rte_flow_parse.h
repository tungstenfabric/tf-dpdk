/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_RTE_FLOW_PARSE_H_
#define _PMD_NET_N3K_MGMT_RTE_FLOW_PARSE_H_

#include <rte_pmd_n3k_flow_tbl_mgmt.h>
#include <rte_pmd_n3k_tunnel_tbl_mgmt.h>
#include <rte_pmd_n3k_vplkp_tbl_mgmt.h>

enum n3k_mgmt_parser_pattern_state {
	PATTERN_STATE_INVALID,
	PATTERN_STATE_START,
	PATTERN_STATE_PORT_ID,

	PATTERN_STATE_OUTER_ETH,
	PATTERN_STATE_OUTER_IPV4,
	PATTERN_STATE_OUTER_UDP,
	PATTERN_STATE_VXLAN,
	PATTERN_STATE_MPLS,

	PATTERN_STATE_INNER_ETH,
	PATTERN_STATE_INNER_IPV4,
	PATTERN_STATE_INNER_IPV6,
	PATTERN_STATE_INNER_UDP,
	PATTERN_STATE_INNER_TCP,

	PATTERN_STATE_END,
	PATTERN_STATE_MAX,
};

enum n3k_mgmt_parser_action_state {
	ACTION_STATE_INVALID,
	ACTION_STATE_START,

	ACTION_STATE_ROUTING_SMAC,
	ACTION_STATE_ROUTING_SMAC_DMAC,
	ACTION_STATE_ROUTING_SMAC_DMAC_TTL,

	ACTION_STATE_SET_IPV4_DST,
	ACTION_STATE_SET_IPV4_SRC,
	ACTION_STATE_ENCAP_VXLAN,
	ACTION_STATE_DECAP_VXLAN,

	ACTION_STATE_RAW_ENCAP,
	ACTION_STATE_RAW_DECAP,
	ACTION_STATE_RAW_DEC_TTL,

	ACTION_STATE_PORT_ID,

	ACTION_STATE_MIRROR,

	ACTION_STATE_DROP,

	ACTION_STATE_END,
	ACTION_STATE_MAX,
};

enum n3k_mgmt_parser_flow_type {
	FLOW_TYPE_UNKNOWN,
	FLOW_TYPE_LOCAL,
	FLOW_TYPE_EGRESS,
	FLOW_TYPE_INGRESS,
};

static inline bool
n3k_mgmt_parsed_flow_does_encap(
	const struct n3k_mgmt_flow_entry *parsed_flow)
{
	if (parsed_flow == NULL)
		return false;

	return parsed_flow->action.action_raw.type == N3K_MGMT_FLOW_TBL_ACTION_ENCAP;
}

static inline bool
n3k_mgmt_parsed_flow_is_smac_set(
	const struct n3k_mgmt_flow_entry *flow)
{
	return n3k_mgmt_parsed_flow_does_encap(flow);
}

static inline bool
n3k_mgmt_parsed_flow_is_tunnel_set(
	const struct n3k_mgmt_flow_entry *flow)
{
	return n3k_mgmt_parsed_flow_does_encap(flow);
}

static inline bool
n3k_mgmt_parsed_flow_is_vport_set(
	const struct n3k_mgmt_flow_entry *parsed_flow)
{
	if (parsed_flow == NULL)
		return false;

	const struct n3k_mgmt_flow_tbl_key *key = &parsed_flow->key.key_raw;

	return key->use_tunnel_key &&
		(key->tunnel.type == N3K_MGMT_TUNNEL_TYPE_MPLSOUDP ||
		 key->tunnel.type == N3K_MGMT_TUNNEL_TYPE_MPLSOGRE);
}

struct rte_flow_item;
struct rte_flow_action;

int n3k_mgmt_rte_flow_to_tables(
	const struct rte_flow_item patterns[],
	const struct rte_flow_action actions[],
	struct n3k_mgmt_flow_entry *flow);

bool n3k_mgmt_rte_flow_is_lacp_flow(
	const struct rte_flow_item patterns[],
	const struct rte_flow_action actions[]);

bool n3k_mgmt_rte_flow_verify(
	const struct rte_flow_item patterns[],
	const struct rte_flow_action actions[]);

#endif // _PMD_NET_N3K_MGMT_RTE_FLOW_PARSE_H_
