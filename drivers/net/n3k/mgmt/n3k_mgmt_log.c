/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_log.h"

#include <rte_log.h>

int n3k_mgmt_logtype[_N3K_MGMT_LOGTYPE_MAX];

const char *
n3k_mgmt_logtype_to_str(enum n3k_mgmt_logtype type)
{
	switch (type) {
		case N3K_MGMT_LOGTYPE_DEFAULT: return "default";
		case N3K_MGMT_LOGTYPE_REGS: return "regs";
		case N3K_MGMT_LOGTYPE_FLOW: return "flow";
		case N3K_MGMT_LOGTYPE_FLOW_PARSE: return "flow_parse";
		case N3K_MGMT_LOGTYPE_MP: return "mp";
		case N3K_MGMT_LOGTYPE_TUNNEL: return "tunnel";
		case N3K_MGMT_LOGTYPE_SMAC: return "smac";
		case N3K_MGMT_LOGTYPE_VPLKP: return "vplkp";
		case N3K_MGMT_LOGTYPE_FLR_DEFAULT: return "flr";
		case N3K_MGMT_LOGTYPE_FLR_SIM: return "flr-sim";
		case N3K_MGMT_LOGTYPE_FLR_DISPATCHER: return "flr-dispatcher";
		case N3K_MGMT_LOGTYPE_FLR_ACK_DISPATCHER: return "flr-ack-dispatcher";
		case N3K_MGMT_LOGTYPE_FLR_PKTS: return "flr-pkts";
		case N3K_MGMT_LOGTYPE_LAG: return "lag";
		case _N3K_MGMT_LOGTYPE_MAX:;
	}
	return "UNKNOWN-LOGTYPE";
}

void
n3k_mgmt_log_register(enum n3k_mgmt_logtype type, const char *name)
{
	n3k_mgmt_logtype[type] = rte_log_register(name);
	if (n3k_mgmt_logtype[type] >= 0)
		rte_log_set_level(n3k_mgmt_logtype[type], RTE_LOG_NOTICE);
}
