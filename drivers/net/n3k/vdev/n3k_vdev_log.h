/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_VDEV_LOG_H_
#define _PMD_NET_N3K_VDEV_LOG_H_

#include <rte_log.h>

enum n3k_vdev_logtype {
	N3K_VDEV_LOGTYPE_DEFAULT,
	N3K_VDEV_LOGTYPE_DISPATCHER,
	_N3K_VDEV_LOGTYPE_MAX,
};
extern int n3k_vdev_logtype[_N3K_VDEV_LOGTYPE_MAX];

void n3k_vdev_log_register(enum n3k_vdev_logtype type, const char *name);

const char *n3k_vdev_logtype_to_str(enum n3k_vdev_logtype type);
const char *n3k_vdev_loglevel_to_str(uint32_t loglevel);

#define N3K_VDEV_LOG_TYPE(type, lvl, msg, ...)                                 \
	rte_log(RTE_LOG_ ## lvl, n3k_vdev_logtype[type],                       \
		"N3KVDEV(%s): %s: %s():%d: " msg "\n",                         \
		n3k_vdev_logtype_to_str(type),                                 \
		n3k_vdev_loglevel_to_str(RTE_LOG_ ## lvl),                     \
		__func__, __LINE__,                                            \
		##__VA_ARGS__)

#define N3K_VDEV_SHOULD_LOG_TYPE(type, lvl)                                    \
	({ RTE_LOG_ ## lvl <= rte_logs.level &&                                \
	(uint32_t)n3k_vdev_logtype[type] < rte_logs.dynamic_types_len &&       \
	(int)RTE_LOG_ ## lvl <= rte_log_get_level(n3k_vdev_logtype[type]); })

#define N3K_VDEV_LOG(lvl, msg, ...) \
	N3K_VDEV_LOG_TYPE(N3K_VDEV_LOGTYPE_DEFAULT, lvl, msg, ##__VA_ARGS__)

#define N3K_VDEV_SHOULD_LOG(lvl) \
	N3K_VDEV_SHOULD_LOG_TYPE(N3K_VDEV_LOGTYPE_DEFAULT, lvl)

#define N3K_VDEV_DISPATCHER_LOG(lvl, msg, ...) \
	N3K_VDEV_LOG_TYPE(N3K_VDEV_LOGTYPE_DISPATCHER, lvl, msg, ##__VA_ARGS__)

