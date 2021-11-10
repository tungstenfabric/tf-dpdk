/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_LOG_H_
#define _PMD_NET_N3K_LOG_H_

#include <rte_log.h>

#include <inttypes.h>

const char *n3k_loglevel_to_str(uint32_t loglevel);

#define N3K_LOG_IMPL(type_str, subtype_log_val, subtype_log_str, lvl, msg, ...) \
	rte_log(RTE_LOG_ ## lvl, subtype_log_val,                               \
		"%s(%s): %s: %s():%d: " msg "\n",                               \
		type_str, subtype_log_str,                                      \
		n3k_loglevel_to_str(RTE_LOG_ ## lvl),                           \
		__func__, __LINE__, ##__VA_ARGS__)

#define N3K_SHOULD_LOG(subtype_log_val, lvl)                                   \
	({ RTE_LOG_ ## lvl <= rte_logs.level &&                                \
	(uint32_t)subtype_log_val < rte_logs.dynamic_types_len &&              \
	(int)RTE_LOG_ ## lvl <= rte_log_get_level(subtype_log_val); })

#endif /* _PMD_NET_N3K_LOG_H_ */
