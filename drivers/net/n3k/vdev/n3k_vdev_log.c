/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_vdev_log.h"

#include <rte_log.h>

int n3k_vdev_logtype[_N3K_VDEV_LOGTYPE_MAX];

const char *
n3k_vdev_logtype_to_str(enum n3k_vdev_logtype type)
{
	switch (type) {
		case N3K_VDEV_LOGTYPE_DEFAULT: return "default";
		case N3K_VDEV_LOGTYPE_DISPATCHER: return "dispatcher";
		case _N3K_VDEV_LOGTYPE_MAX:;
	}
	return "UNKNOWN-LOGTYPE";
}

void
n3k_vdev_log_register(enum n3k_vdev_logtype type, const char *name)
{
	n3k_vdev_logtype[type] = rte_log_register(name);
	if (n3k_vdev_logtype[type] >= 0)
		rte_log_set_level(n3k_vdev_logtype[type], RTE_LOG_NOTICE);
}
