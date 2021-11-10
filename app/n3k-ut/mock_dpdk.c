/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>
#include "n3k_rte_flow_helper.h"

// declare the same functions without the weak attribute
void *n3k_malloc(const char *type, size_t size, unsigned align);
void *n3k_calloc(const char *type, size_t num, size_t size, unsigned align);
void *n3k_zmalloc(const char *type, size_t size, unsigned align);
void *n3k_zmalloc_socket(const char *type, size_t size, unsigned align,
	int socket);
void n3k_free(void *mem);
int n3k_vdev_dpdk_port_to_vdev_id(uint16_t dpdk_port,
	struct n3k_vdev_dev_id  *device_id);

#define UNUSED __attribute__((unused))

static void* zmalloc(size_t size)
{
	void* ret = test_malloc(size);
	if (ret == NULL)
		return NULL;
	memset(ret, 0, size);
	return ret;
}

void*
n3k_malloc(const char* type UNUSED, size_t size, unsigned align UNUSED)
{
	return test_malloc(size);
}

void*
n3k_calloc(const char* type UNUSED, size_t num, size_t size,
	unsigned align UNUSED)
{
	return test_calloc(num, size);
}

void*
n3k_zmalloc(const char* type UNUSED, size_t size, unsigned align UNUSED)
{
	return zmalloc(size);
}

void*
n3k_zmalloc_socket(const char* type UNUSED, size_t size, unsigned align UNUSED,
	int socket UNUSED)
{
	return zmalloc(size);
}

void n3k_free(void* mem)
{
	test_free(mem);
}

int
n3k_vdev_dpdk_port_to_vdev_id(uint16_t dpdk_port,
	struct n3k_vdev_dev_id  *device_id)
{
	*device_id = get_device_id(dpdk_port);
	return 0;
}

int rte_log(uint32_t level __attribute__((unused)),
        uint32_t logtype __attribute__((unused)), const char *format __attribute__((unused)), ...)
{
    return 0;
}
