/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _N3K_DUMP_LOG_H_
#define _N3K_DUMP_LOG_H_

#include <rte_log.h>

int n3k_dump_logtype;

int n3k_dump_log(uint32_t level, uint32_t logtype, const char *format, ...);

#define N3K_DUMP_LOG_PREFIX "N3KFLOW_DUMP"

#define N3K_DUMP_LOG(lvl, msg, ...)                               \
	n3k_dump_log(RTE_LOG_ ## lvl, n3k_dump_logtype,                    \
		N3K_DUMP_LOG_PREFIX ": %s(%d) - " msg "\n", __func__, __LINE__, ##__VA_ARGS__)

#endif /* _N3K_DUMP_LOG_H_ */
