/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MALLOC_H_
#define _PMD_NET_N3K_MALLOC_H_

#include <stddef.h>

#define N3K_MOCKABLE __attribute__((weak))

void*
n3k_malloc(const char*type, size_t size, unsigned align) N3K_MOCKABLE;

void*
n3k_calloc(const char* type, size_t num, size_t size, unsigned align)
	N3K_MOCKABLE;

void*
n3k_zmalloc(const char* type, size_t size, unsigned align) N3K_MOCKABLE;

void*
n3k_zmalloc_socket(const char* type, size_t size, unsigned align, int socket)
	N3K_MOCKABLE;
