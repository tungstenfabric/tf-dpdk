/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _RTE_PMD_N3K_MP_H_
#define _RTE_PMD_N3K_MP_H_

#include <rte_ether.h>
#include <rte_pmd_n3k_flow_tbl_mgmt.h>
#include <rte_pmd_n3k_port.h>
#include <rte_pmd_n3k_tunnel_tbl_mgmt.h>
#include <rte_pmd_n3k_vplkp_tbl_mgmt.h>
#include <stdint.h>

#define N3K_MGMT_MP_RING_NAME "n3k_mgmt_mp_entries_ring"
#define N3K_MGMT_MP_MEMPOOL_NAME "n3k_mgmt_mp_entries_pool"

#define N3K_MGMT_MP_REQUEST_TOTAL_ENTRIES "n3k_mgmt_mp_request_total_entries"
#define N3K_MGMT_MP_REQUEST_ENTRIES "n3k_mgmt_mp_request_entries"
#define N3K_MGMT_MP_REQUEST_REGISTER "n3k_mgmt_mp_request_register"

#define N3K_MGMT_MP_DEVICE_MAX_LEN RTE_ETH_NAME_MAX_LEN

enum n3k_mgmt_mp_request_table {
	N3K_MGMT_MP_REQUEST_TABLE_FLOW,
	N3K_MGMT_MP_REQUEST_TABLE_FLOW_RAW,
	N3K_MGMT_MP_REQUEST_TABLE_TUNNEL,
	N3K_MGMT_MP_REQUEST_TABLE_TUNNEL_RAW,
	N3K_MGMT_MP_REQUEST_TABLE_SMAC,
	N3K_MGMT_MP_REQUEST_TABLE_VPLKP,
	N3K_MGMT_MP_REQUEST_TABLE_FLOW_TRACKER,
	N3K_MGMT_MP_REQUEST_TABLE_PORT,
};

struct n3k_mgmt_mp_register_request {
	uint64_t base;
	uint64_t offset;
	char device_name[N3K_MGMT_MP_DEVICE_MAX_LEN];
};

struct n3k_mgmt_mp_dump_register_response {
	uint32_t reg_value;
};

struct n3k_mgmt_mp_request {
	enum n3k_mgmt_mp_request_table table;
	char device_name[N3K_MGMT_MP_DEVICE_MAX_LEN];
};

struct n3k_mgmt_mp_total_entries_response {
	uint64_t total_entries;
};

struct n3k_mgmt_mp_flow_response {
	uint32_t flow_id;
	struct n3k_mgmt_flow_entry flow;
	struct n3k_mgmt_flow_tracker_stats ft;
};

struct n3k_mgmt_mp_flow_raw_response {
	uint32_t flow_id;
	struct n3k_mgmt_flow_tbl_entry flow;
};

struct n3k_mgmt_mp_tunnel_response {
	uint16_t tunnel_id;
	uint32_t ref_count;
	struct n3k_mgmt_tunnel_entry tunnel;
};

struct n3k_mgmt_mp_tunnel_raw_response {
	uint16_t tunnel_id;
	uint32_t ref_count;
	struct n3k_mgmt_tunnel_tbl_entry tunnel;
};

struct n3k_mgmt_mp_smac_response {
	uint16_t port_id;
	uint32_t ref_count;
	struct rte_ether_addr smac;
};

struct n3k_mgmt_mp_vplkp_response {
	uint32_t label;
	uint32_t ref_count;
	struct n3k_mgmt_vplkp_tbl_res res;
};

struct n3k_mgmt_mp_flow_tracker_response {
	uint32_t flow_id;
	struct n3k_mgmt_flow_tracker_stats ft;
};

struct n3k_mgmt_mp_port_response {
	char port_name[N3K_MGMT_MP_DEVICE_MAX_LEN];
	struct n3k_mgmt_port_stats stats;
};

struct n3k_mgmt_mp_entries_response {
	bool end;
	union {
		struct n3k_mgmt_mp_flow_response flow;
		struct n3k_mgmt_mp_flow_raw_response flow_raw;
		struct n3k_mgmt_mp_tunnel_response tunnel;
		struct n3k_mgmt_mp_tunnel_raw_response tunnel_raw;
		struct n3k_mgmt_mp_smac_response smac;
		struct n3k_mgmt_mp_vplkp_response vplkp;
		struct n3k_mgmt_mp_flow_tracker_response flow_tracker;
		struct n3k_mgmt_mp_port_response port;
	};
};

void rte_pmd_n3k_mp_flow_dump(const struct n3k_mgmt_mp_flow_response *flow);
void rte_pmd_n3k_mp_flow_raw_dump(const struct n3k_mgmt_mp_flow_raw_response *flow_raw);
void rte_pmd_n3k_mp_tunnel_dump(const struct n3k_mgmt_mp_tunnel_response *tunnel);
void rte_pmd_n3k_mp_tunnel_raw_dump(const struct n3k_mgmt_mp_tunnel_raw_response *tunnel_raw);
void rte_pmd_n3k_mp_smac_dump(const struct n3k_mgmt_mp_smac_response *smac);
void rte_pmd_n3k_mp_vplkp_dump(const struct n3k_mgmt_mp_vplkp_response *vplkp);
