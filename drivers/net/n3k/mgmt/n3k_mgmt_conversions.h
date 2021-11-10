/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_CONVERSIONS_H_
#define _PMD_NET_N3K_MGMT_CONVERSIONS_H_

#include <rte_ether.h>
#include <stdint.h>

#include "n3k_mgmt_bitwise.h"

uint64_t n3k_mgmt_convert_rte_ether_addr_to_uint(struct rte_ether_addr addr);
struct rte_ether_addr n3k_mgmt_convert_uint_to_rte_ether_addr(uint64_t addr);

const char* n3k_mgmt_convert_uint_to_bits_str(uint32_t val, uint8_t size);

#define N3K_MGMT_CONVERT_MAC_FORMAT \
	"%02" PRIx8 ":%02" PRIx8 ":%02" PRIx8 ":%02" PRIx8 ":%02" PRIx8 ":%02" PRIx8

#define N3K_MGMT_CONVERT_MAC_VALUE(addr) \
	((addr).addr_bytes)[0], \
	((addr).addr_bytes)[1], \
	((addr).addr_bytes)[2], \
	((addr).addr_bytes)[3], \
	((addr).addr_bytes)[4], \
	((addr).addr_bytes)[5]

#define N3K_MGMT_CONVERT_IPV4_FORMAT \
	"%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8

#define N3K_MGMT_CONVERT_IPV4_VALUE(addr) \
	BITS_RANGE32(addr, 24, 8), \
	BITS_RANGE32(addr, 16, 8), \
	BITS_RANGE32(addr, 8, 8), \
	BITS_RANGE32(addr, 0, 8)

#endif /* _PMD_NET_N3K_MGMT_CONVERSIONS_H_ */
