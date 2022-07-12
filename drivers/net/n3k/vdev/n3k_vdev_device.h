/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_VDEV_DEVICE_H_
#define _PMD_NET_N3K_VDEV_DEVICE_H_

#include <stdbool.h>
#include <stdint.h>

#include <rte_bus_vdev.h>
#include <rte_ethdev.h>
#include <rte_pmd_n3k_port.h>

#include "n3k_vdev_dispatcher.h"

extern struct rte_vdev_driver pmd_n3k_vdev_drv;

struct n3k_vdev_dev_shared;

struct n3k_vdev_dev_info {
	struct n3k_vdev_dev_shared *shared;
	struct rte_ether_addr mac;
	uint16_t id;
	enum n3k_vdev_dev_type type;
};

struct n3k_vdev_dev_internal {
	struct n3k_vdev_dev_shared *shared;
	uint16_t id;
	enum n3k_vdev_dev_type type;
	bool configured;
	bool dedicated_queue_enabled; // for LACP
	uint16_t dedicated_queue_id; // for LACP
};

struct n3k_vdev_pf_rx_conf {
	uint16_t nb_desc;
	unsigned int socket_id;
	struct rte_eth_rxconf conf;
};

struct n3k_vdev_pf_tx_conf {
	uint16_t nb_desc;
	unsigned int socket_id;
	struct rte_eth_txconf conf;
};

struct n3k_vdev_dev_shared {
	struct rte_eth_dev *pf_dev;
	struct rte_eth_dev *mgmt_dev;
	struct n3k_vdev_dispatcher *dispatcher;

	struct n3k_vdev_pf_rx_conf pf_rx_conf[RTE_MAX_QUEUES_PER_PORT];
	struct n3k_vdev_pf_tx_conf pf_tx_conf[RTE_MAX_QUEUES_PER_PORT];

	bool pf_configured;
	uint16_t configured_vdevs_count;
	uint16_t started_vdevs_count;

	uint16_t configured_rx_queues_count[RTE_MAX_QUEUES_PER_PORT];
	uint16_t configured_tx_queues_count[RTE_MAX_QUEUES_PER_PORT];

	uint16_t configured_dedicated_rx_queue_count;
	uint16_t configured_dedicated_tx_queue_count;

	struct rte_mempool *pf_rx_mempools[RTE_MAX_QUEUES_PER_PORT];
	struct rte_mempool *pf_tx_mempools[RTE_MAX_QUEUES_PER_PORT];

	struct rte_mempool *pf_dedicated_tx_mempool;

	/* TODO: Add a lock if this is to be modified concurrently */
};

int n3k_vdev_dev_create(
	struct rte_vdev_device *dev,
	struct n3k_vdev_dev_info *dev_info,
	struct rte_eth_dev **vdev);

struct n3k_vdev_dev_shared * n3k_vdev_dev_alloc_shared_data(
	struct rte_eth_dev *pf_dev,
	struct rte_eth_dev *mgmt_dev);

bool n3k_vdev_is_valid_n3k_repr(const struct rte_eth_dev *dev);
bool n3k_vdev_is_valid_n3k_vf_repr(const struct rte_eth_dev *dev);
bool n3k_vdev_is_valid_n3k_phy_repr(const struct rte_eth_dev *dev);

void n3k_vdev_get_representor_info(
	const struct rte_eth_dev *dev,
	enum n3k_vdev_dev_type *out_type,
	uint16_t *out_id);
int n3k_vdev_get_mgmt_dev(const struct rte_eth_dev *dev, struct rte_eth_dev **mgmt);

#endif /* _PMD_NET_N3K_VDEV_DEVICE_H_ */
