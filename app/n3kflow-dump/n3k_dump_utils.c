/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_dump_utils.h"
#include <rte_ethdev.h>
#include <inttypes.h>
#include <stdbool.h>

#define BITS_RANGE32(val, start, nb) \
	(((val) >> (start)) & ((~(uint32_t)0) >> (32 - nb)))

const char *
n3k_dump_convert_mac_to_str(char *buff, struct rte_ether_addr addr)
{
	sprintf(buff, "%02" PRIx8 ":%02" PRIx8 ":%02" PRIx8
		":%02" PRIx8 ":%02" PRIx8 ":%02" PRIx8,
		addr.addr_bytes[0], addr.addr_bytes[1],
		addr.addr_bytes[2], addr.addr_bytes[3],
		addr.addr_bytes[4], addr.addr_bytes[5]);
	return buff;
}

const char *
n3k_dump_convert_ip_to_str(char *buff, uint32_t addr)
{
	sprintf(buff, "%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8,
