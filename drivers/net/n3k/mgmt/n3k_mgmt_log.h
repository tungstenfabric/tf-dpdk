/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_LOG_H_
#define _PMD_NET_N3K_MGMT_LOG_H_

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
	N3K_MGMT_LOGTYPE_NIOS,
	N3K_MGMT_LOGTYPE_REGS,
	N3K_MGMT_LOGTYPE_SMAC,
	N3K_MGMT_LOGTYPE_TUNNEL,
	N3K_MGMT_LOGTYPE_VPLKP,
	_N3K_MGMT_LOGTYPE_MAX,
};
extern int n3k_mgmt_logtype[_N3K_MGMT_LOGTYPE_MAX];

void n3k_mgmt_log_register(enum n3k_mgmt_logtype type, const char *name);

const char *n3k_mgmt_logtype_to_str(enum n3k_mgmt_logtype type);
const char *n3k_mgmt_loglevel_to_str(uint32_t loglevel);

#define N3K_MGMT_LOG_TYPE(type, lvl, msg, ...)                                 \
	rte_log(RTE_LOG_ ## lvl, n3k_mgmt_logtype[type],                       \
		"N3KMGMT(%s): %s: %s():%d: " msg "\n",                         \
		n3k_mgmt_logtype_to_str(type),                                 \
		n3k_mgmt_loglevel_to_str(RTE_LOG_ ## lvl),                     \
		__func__, __LINE__,                                            \
		##__VA_ARGS__)

#define N3K_MGMT_SHOULD_LOG_TYPE(type, lvl)                                    \
	({ RTE_LOG_ ## lvl <= rte_logs.level &&                                \
	(uint32_t)n3k_mgmt_logtype[type] < rte_logs.dynamic_types_len &&       \
	(int)RTE_LOG_ ## lvl <= rte_log_get_level(n3k_mgmt_logtype[type]); })

#define N3K_MGMT_LOG(lvl, msg, ...) \
	N3K_MGMT_LOG_TYPE(N3K_MGMT_LOGTYPE_DEFAULT, lvl, msg, ##__VA_ARGS__)

#define N3K_MGMT_SHOULD_LOG(lvl) \
	N3K_MGMT_SHOULD_LOG_TYPE(N3K_MGMT_LOGTYPE_DEFAULT, lvl)

#define N3K_MGMT_REGS_LOG(lvl, msg, ...) \
	N3K_MGMT_LOG_TYPE(N3K_MGMT_LOGTYPE_REGS, lvl, msg, ##__VA_ARGS__)

#define N3K_MGMT_REGS_SHOULD_LOG(lvl) \
	N3K_MGMT_SHOULD_LOG_TYPE(N3K_MGMT_LOGTYPE_REGS, lvl)

#define N3K_MGMT_FLOW_LOG(lvl, msg, ...) \
	N3K_MGMT_LOG_TYPE(N3K_MGMT_LOGTYPE_FLOW, lvl, msg, ##__VA_ARGS__)

#define N3K_MGMT_FLOW_SHOULD_LOG(lvl) \
	N3K_MGMT_SHOULD_LOG_TYPE(N3K_MGMT_LOGTYPE_FLOW, lvl)

#define N3K_MGMT_MP_LOG(lvl, msg, ...) \
	N3K_MGMT_LOG_TYPE(N3K_MGMT_LOGTYPE_MP, lvl, msg, ##__VA_ARGS__)

#define N3K_MGMT_MP_SHOULD_LOG(lvl) \
	N3K_MGMT_SHOULD_LOG_TYPE(N3K_MGMT_LOGTYPE_MP, lvl)

#define N3K_MGMT_NIOS_LOG(lvl, msg, ...) \
	N3K_MGMT_LOG_TYPE(N3K_MGMT_LOGTYPE_NIOS, lvl, msg, ##__VA_ARGS__)

#define N3K_MGMT_NIOS_SHOULD_LOG(lvl) \
	N3K_MGMT_SHOULD_LOG_TYPE(N3K_MGMT_LOGTYPE_NIOS, lvl)

#define N3K_MGMT_FLOW_PARSE_LOG(lvl, msg, ...) \
	N3K_MGMT_LOG_TYPE(N3K_MGMT_LOGTYPE_FLOW_PARSE, lvl, msg, ##__VA_ARGS__)

#define N3K_MGMT_FLOW_PARSE_SHOULD_LOG(lvl) \
	N3K_MGMT_SHOULD_LOG_TYPE(N3K_MGMT_LOGTYPE_FLOW_PARSE, lvl)

#define N3K_MGMT_TUNNEL_LOG(lvl, msg, ...) \
	N3K_MGMT_LOG_TYPE(N3K_MGMT_LOGTYPE_TUNNEL, lvl, msg, ##__VA_ARGS__)

#define N3K_MGMT_TUNNEL_SHOULD_LOG(lvl) \
	N3K_MGMT_SHOULD_LOG_TYPE(N3K_MGMT_LOGTYPE_TUNNEL, lvl)

#define N3K_MGMT_SMAC_LOG(lvl, msg, ...) \
	N3K_MGMT_LOG_TYPE(N3K_MGMT_LOGTYPE_SMAC, lvl, msg, ##__VA_ARGS__)

#define N3K_MGMT_SMAC_SHOULD_LOG(lvl) \
	N3K_MGMT_SHOULD_LOG_TYPE(N3K_MGMT_LOGTYPE_SMAC, lvl)

#define N3K_MGMT_VPLKP_LOG(lvl, msg, ...) \
	N3K_MGMT_LOG_TYPE(N3K_MGMT_LOGTYPE_VPLKP, lvl, msg, ##__VA_ARGS__)

#define N3K_MGMT_VPLKP_SHOULD_LOG(lvl) \
	N3K_MGMT_SHOULD_LOG_TYPE(N3K_MGMT_LOGTYPE_VPLKP, lvl)

#define N3K_MGMT_FLR_LOG(lvl, msg, ...) \
	N3K_MGMT_LOG_TYPE(N3K_MGMT_LOGTYPE_FLR_DEFAULT, lvl, msg, ##__VA_ARGS__)

#define N3K_MGMT_FLR_SHOULD_LOG(lvl) \
	N3K_MGMT_SHOULD_LOG_TYPE(N3K_MGMT_LOGTYPE_FLR_DEFAULT, lvl)

#define N3K_MGMT_FLR_SIM_LOG(lvl, msg, ...) \
	N3K_MGMT_LOG_TYPE(N3K_MGMT_LOGTYPE_FLR_SIM, lvl, msg, ##__VA_ARGS__)

#define N3K_MGMT_FLR_SIM_SHOULD_LOG(lvl) \
	N3K_MGMT_SHOULD_LOG_TYPE(N3K_MGMT_LOGTYPE_FLR_SIM, lvl)

#define N3K_MGMT_FLR_DISPATCHER_LOG(lvl, msg, ...) \
	N3K_MGMT_LOG_TYPE(N3K_MGMT_LOGTYPE_FLR_DISPATCHER, lvl, msg, ##__VA_ARGS__)

#define N3K_MGMT_FLR_DISPATCHER_SHOULD_LOG(lvl) \
	N3K_MGMT_SHOULD_LOG_TYPE(N3K_MGMT_LOGTYPE_FLR_DISPATCHER, lvl)

#define N3K_MGMT_FLR_ACK_DISPATCHER_LOG(lvl, msg, ...) \
	N3K_MGMT_LOG_TYPE(N3K_MGMT_LOGTYPE_FLR_ACK_DISPATCHER, lvl, msg, ##__VA_ARGS__)

#define N3K_MGMT_FLR_ACK_DISPATCHER_SHOULD_LOG(lvl) \
	N3K_MGMT_SHOULD_LOG_TYPE(N3K_MGMT_LOGTYPE_FLR_ACK_DISPATCHER, lvl)

#define N3K_MGMT_FLR_PKTS_LOG(lvl, msg, ...) \
	N3K_MGMT_LOG_TYPE(N3K_MGMT_LOGTYPE_FLR_PKTS, lvl, msg, ##__VA_ARGS__)

#define N3K_MGMT_FLR_PKTS_SHOULD_LOG(lvl) \
	N3K_MGMT_SHOULD_LOG_TYPE(N3K_MGMT_LOGTYPE_FLR_PKTS, lvl)

#endif /* _PMD_NET_N3K_MGMT_LOG_H_ */
