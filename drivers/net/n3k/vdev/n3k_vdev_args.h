/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_VDEV_ARGS_H_
#define _PMD_NET_N3K_VDEV_ARGS_H_

#include "../n3k.h"

#include <stdbool.h>
#include <stdint.h>
#include <rte_bus_vdev.h>
#include <rte_ethdev.h>

/* TODO: Comment all functions/structs */

struct n3k_vdev_args {
	struct rte_eth_dev *pf_dev;
	struct rte_eth_dev *mgmt_dev;
	uint16_t vfs_count;
	uint16_t vfs[N3K_VF_PORTS_MAX_COUNT];
	bool is_lag_enabled;
};

