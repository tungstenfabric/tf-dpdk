/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_dump_json.h"
#include "n3k_dump_log.h"
#include "n3k_dump_printers.h"

#include <rte_pmd_n3k_mp.h>
#include <rte_pmd_n3k_flow_tbl_mgmt.h>

#include <stdbool.h>

void
n3k_dump_print_table_entry(struct n3k_mgmt_mp_entries_response *response,
	enum n3k_mgmt_mp_request_table table)
{
	switch (table) {
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW:
		return rte_pmd_n3k_mp_flow_dump(&response->flow);
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW_RAW:
		return rte_pmd_n3k_mp_flow_raw_dump(&response->flow_raw);
	case N3K_MGMT_MP_REQUEST_TABLE_TUNNEL:
		return rte_pmd_n3k_mp_tunnel_dump(&response->tunnel);
	case N3K_MGMT_MP_REQUEST_TABLE_TUNNEL_RAW:
		return rte_pmd_n3k_mp_tunnel_raw_dump(&response->tunnel_raw);
	case N3K_MGMT_MP_REQUEST_TABLE_SMAC:
		return rte_pmd_n3k_mp_smac_dump(&response->smac);
	case N3K_MGMT_MP_REQUEST_TABLE_VPLKP:
		return rte_pmd_n3k_mp_vplkp_dump(&response->vplkp);
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW_TRACKER:
		return rte_pmd_n3k_mp_flow_tracker_dump(&response->flow_tracker);
	case N3K_MGMT_MP_REQUEST_TABLE_PORT:
		return rte_pmd_n3k_mp_port_dump(&response->port);
	}
	N3K_DUMP_LOG(ERR, "Invalid table type %d", table);
}

void
n3k_dump_print_table_entry_json(__rte_unused struct n3k_mgmt_mp_entries_response *response,
	enum n3k_mgmt_mp_request_table table)
{
	switch (table) {
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW:
		return n3k_dump_mp_json_flow_dump(&response->flow);
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW_RAW:
		return n3k_dump_mp_json_flow_raw_dump(&response->flow_raw);
	case N3K_MGMT_MP_REQUEST_TABLE_TUNNEL:
		return n3k_dump_mp_json_tunnel_dump(&response->tunnel);
	case N3K_MGMT_MP_REQUEST_TABLE_TUNNEL_RAW:
		return n3k_dump_mp_json_tunnel_raw_dump(&response->tunnel_raw);
	case N3K_MGMT_MP_REQUEST_TABLE_SMAC:
		return n3k_dump_mp_json_smac_dump(&response->smac);
	case N3K_MGMT_MP_REQUEST_TABLE_VPLKP:
		return n3k_dump_mp_json_vplkp_dump(&response->vplkp);
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW_TRACKER:
		return n3k_dump_mp_json_flow_tracker_dump(&response->flow_tracker);
	case N3K_MGMT_MP_REQUEST_TABLE_PORT:
