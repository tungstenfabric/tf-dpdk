/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_VDEV_LOG_H_
#define _PMD_NET_N3K_VDEV_LOG_H_

#include "common/n3k_log.h"

#include <rte_log.h>

enum n3k_vdev_logtype {
	N3K_VDEV_LOGTYPE_DEFAULT,
	N3K_VDEV_LOGTYPE_DISPATCHER,
	_N3K_VDEV_LOGTYPE_MAX,
};
extern int n3k_vdev_logtype[_N3K_VDEV_LOGTYPE_MAX];

void n3k_vdev_log_register(enum n3k_vdev_logtype type, const char *name);

const char *n3k_vdev_logtype_to_str(enum n3k_vdev_logtype type);

#define N3K_VDEV_LOG(subtype_val, lvl, msg, ...)                          \
	N3K_LOG_IMPL("N3KVDEV",                                                \
		n3k_vdev_logtype[N3K_VDEV_LOGTYPE_ ## subtype_val],            \
		n3k_vdev_logtype_to_str(N3K_VDEV_LOGTYPE_ ## subtype_val),     \
		lvl, msg, ##__VA_ARGS__)

#define N3K_VDEV_SHOULD_LOG(subtype_val, lvl) \
	N3K_SHOULD_LOG(n3k_vdev_logtype[N3K_VDEV_LOGTYPE_ ## subtype_val], lvl)

#endif /* _PMD_NET_N3K_VDEV_LOG_H_ */
