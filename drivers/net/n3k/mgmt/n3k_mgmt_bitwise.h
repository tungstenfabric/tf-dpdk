/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_BITWISE_H_
#define _PMD_NET_N3K_MGMT_BITWISE_H_

#include <stdint.h>
#include <stdlib.h>
#include <rte_byteorder.h>

#define BITS_RANGE32(val, start, nb) \
	(((val) >> (start)) & ((~(uint32_t)0) >> (32 - nb)))
#define BITS_RANGE64(val, start, nb) \
	(((val) >> (start)) & ((~(uint64_t)0) >> (64 - nb)))

inline static void
memcpy_bswap32(void *dst, const uint32_t *src, size_t regs_size)
{
	uint32_t *dst_regs = dst;
	size_t i;
	for (i = 0; i < regs_size / 4; ++i)
