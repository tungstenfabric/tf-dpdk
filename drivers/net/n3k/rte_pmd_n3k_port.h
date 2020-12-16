/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_PORT_STATS_H_
#define _PMD_NET_N3K_PORT_STATS_H_

#include <rte_ethdev.h>

#include <stdint.h>

struct n3k_mgmt_port_stats {
	uint64_t ipackets;
	uint64_t opackets;
	uint64_t ibytes;
	uint64_t obytes;
};

enum n3k_vdev_dev_type {
	N3K_VDEV_DEV_TYPE_PHY = 0,
	N3K_VDEV_DEV_TYPE_VF = 1,
	N3K_VDEV_DEV_TYPE_LAG = 2,
};

struct n3k_vdev_dev_id {
	enum n3k_vdev_dev_type type;
	uint16_t id;
};

int
rte_pmd_n3k_get_vdev_stats(struct rte_eth_dev *dev, struct n3k_mgmt_port_stats *stats);

int
rte_pmd_n3k_port_stats_dump(struct rte_eth_dev *dev);
void
rte_pmd_n3k_port_stats_struct_dump(struct n3k_mgmt_port_stats *stats);

const char *
rte_pmd_n3k_vdev_id_to_str(
