/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _N3K_DUMP_JSON_H_
#define _N3K_DUMP_JSON_H_

#include <rte_pmd_n3k_mp.h>
#include <rte_pmd_n3k_flow_tbl_mgmt.h>

void n3k_dump_mp_json_flow_dump(const struct n3k_mgmt_mp_flow_response *rsp);
void n3k_dump_mp_json_flow_raw_dump(const struct n3k_mgmt_mp_flow_raw_response *rsp);

void n3k_dump_mp_json_tunnel_dump(const struct n3k_mgmt_mp_tunnel_response *rsp);
void n3k_dump_mp_json_tunnel_raw_dump(const struct n3k_mgmt_mp_tunnel_raw_response *rsp);

void n3k_dump_mp_json_smac_dump(const struct n3k_mgmt_mp_smac_response *rsp);

void n3k_dump_mp_json_vplkp_dump(const struct n3k_mgmt_mp_vplkp_response *rsp);

void n3k_dump_mp_json_flow_tracker_dump(const struct n3k_mgmt_mp_flow_tracker_response *rsp);

void n3k_dump_mp_json_port_dump(const struct n3k_mgmt_mp_port_response *rsp);

void n3k_dump_mp_json_vf0_stats_dump(const struct n3k_mgmt_mp_vf0_stats_response *rsp);

#endif /* _N3K_DUMP_JSON_H_ */
