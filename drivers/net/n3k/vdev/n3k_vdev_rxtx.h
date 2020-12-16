/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_VDEV_RXTX_H_
#define _PMD_NET_N3K_VDEV_RXTX_H_

#include <rte_ethdev.h>

struct n3k_vdev_queue {
	uint16_t queue_id;
	struct rte_ring *ring_used;
	struct rte_ring *ring_free;
	struct rte_eth_dev *dev;
	struct rte_mempool *mempool;
};

int
n3k_vdev_rx_queue_setup(struct rte_eth_dev *dev, uint16_t queue_id,
	uint16_t nb_desc, unsigned int socket_id,
	const struct rte_eth_rxconf *conf, struct rte_mempool *mb_pool);

void
n3k_vdev_rx_queue_release(void *rx_queue);

int
n3k_vdev_tx_queue_setup(struct rte_eth_dev *dev, uint16_t queue_id,
	uint16_t nb_desc, unsigned int socket_id,
	const struct rte_eth_txconf *conf);

void
n3k_vdev_tx_queue_release(void *tx_queue);

uint16_t
n3k_vdev_rx_burst(void *rx_queue, struct rte_mbuf **rx_pkts, uint16_t nb_pkts);

