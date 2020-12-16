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

#define N3K_DUMP_LOG_PREFIX "N3KFLOW_DUMP"

#define N3K_DUMP_LOG(lvl, msg, ...)                               \
