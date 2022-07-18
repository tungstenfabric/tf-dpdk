/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_LOG_H_
#define _PMD_NET_N3K_MGMT_LOG_H_

#include "common/n3k_log.h"

#include <rte_log.h>

enum n3k_mgmt_logtype {
	N3K_MGMT_LOGTYPE_DEFAULT,
	N3K_MGMT_LOGTYPE_FLOW_PARSE,
	N3K_MGMT_LOGTYPE_FLOW,
	N3K_MGMT_LOGTYPE_FLR_ACK_DISPATCHER,
	N3K_MGMT_LOGTYPE_FLR_DEFAULT,
	N3K_MGMT_LOGTYPE_FLR_DISPATCHER,
	N3K_MGMT_LOGTYPE_FLR_PKTS,
	N3K_MGMT_LOGTYPE_FLR_SIM,
	N3K_MGMT_LOGTYPE_MP,
	N3K_MGMT_LOGTYPE_REGS,
	N3K_MGMT_LOGTYPE_SMAC,
	N3K_MGMT_LOGTYPE_TUNNEL,
	N3K_MGMT_LOGTYPE_VPLKP,
	N3K_MGMT_LOGTYPE_LAG,
	_N3K_MGMT_LOGTYPE_MAX,
};
extern int n3k_mgmt_logtype[_N3K_MGMT_LOGTYPE_MAX];

void n3k_mgmt_log_register(enum n3k_mgmt_logtype type, const char *name);

const char *n3k_mgmt_logtype_to_str(enum n3k_mgmt_logtype type);

#define N3K_MGMT_LOG(subtype_val, lvl, msg, ...)                          \
	N3K_LOG_IMPL("N3KMGMT",                                                \
		n3k_mgmt_logtype[N3K_MGMT_LOGTYPE_ ## subtype_val],            \
		n3k_mgmt_logtype_to_str(N3K_MGMT_LOGTYPE_ ## subtype_val),     \
		lvl, msg, ##__VA_ARGS__)

#define N3K_MGMT_SHOULD_LOG(subtype_val, lvl) \
	N3K_SHOULD_LOG(n3k_mgmt_logtype[N3K_MGMT_LOGTYPE_ ## subtype_val], lvl)

#endif /* _PMD_NET_N3K_MGMT_LOG_H_ */
