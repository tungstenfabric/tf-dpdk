/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "mgmt/n3k_mgmt_dump.h"
#include "rte_pmd_n3k_mp.h"

_Static_assert(sizeof(struct n3k_mgmt_mp_request) <= RTE_MP_MAX_PARAM_LEN,
	"invalid n3k_mgmt_mp_entry size");

void rte_pmd_n3k_mp_flow_dump(const struct n3k_mgmt_mp_flow_response *flow)
{
	n3k_mgmt_mp_flow_dump(flow);
}

void rte_pmd_n3k_mp_flow_raw_dump(const struct n3k_mgmt_mp_flow_raw_response *flow_raw)
{
	n3k_mgmt_mp_flow_raw_dump(flow_raw);
}

void rte_pmd_n3k_mp_tunnel_dump(const struct n3k_mgmt_mp_tunnel_response *tunnel)
{
	n3k_mgmt_mp_tunnel_dump(tunnel);
}

void rte_pmd_n3k_mp_tunnel_raw_dump(const struct n3k_mgmt_mp_tunnel_raw_response *tunnel_raw)
{
	n3k_mgmt_mp_tunnel_raw_dump(tunnel_raw);
}

void rte_pmd_n3k_mp_smac_dump(const struct n3k_mgmt_mp_smac_response *smac)
{
	n3k_mgmt_mp_smac_dump(smac);
}

void rte_pmd_n3k_mp_vplkp_dump(const struct n3k_mgmt_mp_vplkp_response *vplkp)
{
	n3k_mgmt_mp_vplkp_dump(vplkp);
}

void rte_pmd_n3k_mp_flow_tracker_dump(const struct n3k_mgmt_mp_flow_tracker_response *flow_tracker)
{
	n3k_mgmt_mp_flow_tracker_dump(flow_tracker);
}

