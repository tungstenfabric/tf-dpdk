/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_log.h"

#include <rte_log.h>

const char *
n3k_loglevel_to_str(uint32_t loglevel)
{
	switch (loglevel) {
		case RTE_LOG_EMERG: return "EMERG";
		case RTE_LOG_ALERT: return "ALERT";
		case RTE_LOG_CRIT: return "CRIT";
		case RTE_LOG_ERR: return "ERR";
		case RTE_LOG_WARNING: return "WARNING";
		case RTE_LOG_NOTICE: return "INFO";
		case RTE_LOG_INFO: return "INFO";
		case RTE_LOG_DEBUG: return "DEBUG";
		default: return "UNKNOWN";
	}
}
