/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_conversions.h"

#include <rte_byteorder.h>
#include <rte_ether.h>
#include <stdint.h>

uint64_t n3k_mgmt_convert_rte_ether_addr_to_uint(struct rte_ether_addr addr)
{
	uint64_t mac = 0;
	memcpy(&mac, addr.addr_bytes, sizeof(addr.addr_bytes));
	return rte_bswap64(mac) >> 16;
}

struct rte_ether_addr n3k_mgmt_convert_uint_to_rte_ether_addr(uint64_t addr)
{
	struct rte_ether_addr mac;
	addr = rte_bswap64(addr) >> 16;
	memcpy(mac.addr_bytes, &addr, sizeof(mac.addr_bytes));
	return mac;
}

const char* n3k_mgmt_convert_uint_to_bits_str(uint32_t val, uint8_t num)
{
	static __thread char bits[sizeof(val) * 8 + 1];
	RTE_ASSERT(num <= sizeof(val) * 8);
	uint8_t i;
	for (i = 0; i < num; ++i)
