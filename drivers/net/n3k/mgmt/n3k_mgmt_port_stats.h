/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_PORT_STATS_H_
#define _PMD_NET_N3K_MGMT_PORT_STATS_H_

#include "n3k_mgmt_hw.h"

#include <rte_pmd_n3k_port.h>

#include <rte_ethdev.h>

#include <stdint.h>

int
n3k_mgmt_get_vdev_stats(struct rte_eth_dev *dev, struct n3k_mgmt_port_stats *stats);

