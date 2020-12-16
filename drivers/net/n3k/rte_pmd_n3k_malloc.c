/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "rte_pmd_n3k_malloc.h"

#include <rte_malloc.h>

void*
n3k_malloc(const char *type, size_t size, unsigned align)
{
	return rte_malloc(type, size, align);
}

void*
n3k_calloc(const char * type, size_t num, size_t size, unsigned align)
{
	return rte_calloc(type, num, size, align);
}

void*
n3k_zmalloc(const char * type, size_t size, unsigned align)
{
	return rte_zmalloc(type, size, align);
}

void*
n3k_zmalloc_socket(const char* type, size_t size, unsigned align, int socket)
{
	return rte_zmalloc_socket(type, size, align, socket);
}

