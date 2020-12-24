/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include <rte_memcpy.h>
#include <rte_ring.h>

#include "../rte_pmd_n3k_malloc.h"

#include "n3k_vdev_log.h"
#include "n3k_vdev_ifc.h"
#include "n3k_vdev_device.h"
#include "n3k_vdev_rxtx.h"

static inline int
n3k_vdev_fill_ring_with_free_mbufs(
	struct rte_ring *ring, struct rte_mempool *mempool)
{
	uint32_t i;
	struct rte_mbuf *pkt;
	uint32_t free_count = rte_ring_free_count(ring);

	/* TODO: Try using rte_pktmbuf_alloc_bulk */

	for (i = 0; i < free_count; ++i) {
		pkt = rte_pktmbuf_alloc(mempool);
		if (!pkt)
			return -ENOMEM;

		if (rte_ring_enqueue(ring, pkt) != 0) {
			rte_pktmbuf_free(pkt);
			return -ENOBUFS;
		}
	}

	return 0;
}

static struct rte_ring *
n3k_vdev_create_ring(const char *dev_name,
	const char *queue_type_str, const char *ring_type_str,
	uint16_t queue_id, uint16_t count, int socket_id)
{
	char ring_name[RTE_RING_NAMESIZE];

	snprintf(ring_name, sizeof(ring_name), "%s_%s_%s_%d",
		dev_name, queue_type_str, ring_type_str, queue_id);

	N3K_VDEV_LOG(DEBUG, "New ring created: %s", ring_name);

	return rte_ring_create(
		ring_name, rte_align32pow2(count), socket_id, 0);
}

static int
n3k_vdev_create_rings(struct n3k_vdev_queue *queue, const char *dev_name,
	const char *queue_type_str,
	uint16_t queue_id, uint16_t nb_desc, int socket_id)
{
	queue->ring_used = n3k_vdev_create_ring(
		dev_name, queue_type_str, "used", queue_id, nb_desc, socket_id);

	if (!queue->ring_used)
		return -rte_errno;

	queue->ring_free = n3k_vdev_create_ring(
		dev_name, queue_type_str, "free", queue_id, nb_desc, socket_id);

	if (!queue->ring_free) {
		rte_ring_free(queue->ring_used);
		queue->ring_used = NULL;
		return -rte_errno;
	}

	return 0;
}

static struct rte_mempool *
n3k_vdev_pf_mempool_alloc(const char *dev_name, const char *queue_type_str,
	uint16_t queue_id, uint16_t nb_desc, int socket_id)
{
	enum { MEMPOOL_ELEMENTS_MULTIPLIER = 2 };

	char mempool_name[RTE_MEMPOOL_NAMESIZE];

	/* The optimum size for a mempool is when
	 * nb_elements is a power of two minus one. */
	const uint32_t nb_elements =
		rte_align32pow2(MEMPOOL_ELEMENTS_MULTIPLIER * nb_desc) - 1;

	const uint32_t cache_size =
		RTE_MIN(RTE_CACHE_LINE_SIZE, RTE_MEMPOOL_CACHE_MAX_SIZE);

	const uint16_t data_size =
		RTE_PKTMBUF_HEADROOM +
		N3K_VDEV_IFC_HEADER_SIZE +
		N3K_VDEV_MBUF_DEFAULT_DATAROOM;

	snprintf(mempool_name, sizeof(mempool_name),
		"%s_%s%d", dev_name, queue_type_str, queue_id);

	return rte_pktmbuf_pool_create(
		mempool_name, nb_elements, cache_size, 0, data_size, socket_id);
}

static void
n3k_vdev_pf_mempool_free(struct rte_mempool **pool)
{
	rte_mempool_free(*pool);
	*pool = NULL;
}

static int
n3k_vdev_pf_rx_queue_setup(struct rte_eth_dev *pf_dev, uint16_t queue_id,
	uint16_t nb_desc, unsigned int socket_id,
	const struct rte_eth_rxconf *conf, struct rte_mempool *mb_pool)
{
	int ret = 0;
	bool was_started = false;

	if (pf_dev->data->dev_started) {
		was_started = true;
		rte_eth_dev_stop(pf_dev->data->port_id);
	}

	ret = rte_eth_rx_queue_setup(
		pf_dev->data->port_id, queue_id, nb_desc, socket_id, conf, mb_pool);
	if (ret)
		return ret;

	if (was_started) {
		ret = rte_eth_dev_start(pf_dev->data->port_id);
		if (ret)
			return ret;
	}

	return ret;
}

static void
n3k_vdev_save_pf_rx_config(struct n3k_vdev_dev_shared *dev_shared,
	uint16_t queue_id, uint16_t nb_desc, unsigned int socket_id,
	const struct rte_eth_rxconf *conf)
{
	struct n3k_vdev_pf_rx_conf *queue_conf = &dev_shared->pf_rx_conf[queue_id];
	queue_conf->nb_desc = nb_desc;
	queue_conf->socket_id = socket_id;
	rte_memcpy(&queue_conf->conf, conf, sizeof(struct rte_eth_rxconf));
}

static int
n3k_vdev_check_pf_rx_config(struct n3k_vdev_dev_shared *dev_shared,
	uint16_t queue_id, uint16_t nb_desc, unsigned int socket_id __rte_unused,
	const struct rte_eth_rxconf *conf)
{
	struct n3k_vdev_pf_rx_conf lcfg = dev_shared->pf_rx_conf[queue_id];
	struct rte_eth_rxconf rte_lcfg = lcfg.conf;
	struct rte_eth_thresh th_lcfg = rte_lcfg.rx_thresh;

	/* TODO: Uncomment after implementing proper handling of NUMA nodes */
	if (lcfg.nb_desc != nb_desc /*|| lcfg.socket_id != socket_id*/)
		return -EINVAL;

	if (rte_lcfg.rx_free_thresh != conf->rx_free_thresh ||
			rte_lcfg.rx_drop_en != conf->rx_drop_en ||
			rte_lcfg.rx_deferred_start != conf->rx_deferred_start ||
			rte_lcfg.offloads != conf->offloads)
		return -EINVAL;

	if (th_lcfg.pthresh != conf->rx_thresh.pthresh ||
			th_lcfg.hthresh != conf->rx_thresh.hthresh ||
			th_lcfg.wthresh != conf->rx_thresh.wthresh)
		return -EINVAL;

	return 0;
}

int
n3k_vdev_rx_queue_setup(struct rte_eth_dev *dev, uint16_t queue_id,
	uint16_t nb_desc, unsigned int socket_id,
	const struct rte_eth_rxconf *conf, struct rte_mempool *mb_pool)
{
	int ret = 0;
	struct rte_mempool **pf_mempool = NULL;

	struct n3k_vdev_dev_internal *dev_internal = dev->data->dev_private;
	struct n3k_vdev_dev_shared *dev_shared = dev_internal->shared;
	struct rte_eth_dev *pf_dev = dev_shared->pf_dev;

	N3K_VDEV_LOG(DEBUG, "Setting up RX queue #%d for %s [Desc: %d; Socket: %d]",
		queue_id, dev->data->name, nb_desc, socket_id);

	if (dev_shared->configured_rx_queues_count[queue_id] == 0) {
		pf_mempool = &dev_shared->pf_rx_mempools[queue_id];

		/* TODO: Change after fixing virtio queues logic */
		if (!*pf_mempool)
			*pf_mempool = n3k_vdev_pf_mempool_alloc(
				pf_dev->device->name, "rx", queue_id, nb_desc, socket_id);

		if (!*pf_mempool) {
			N3K_VDEV_LOG(ERR,
				"Failed to allocate mempool for RX queue #%d for PF %s of %s",
				queue_id, pf_dev->data->name, dev->data->name);
			return -ENOMEM;
		}

		ret = n3k_vdev_pf_rx_queue_setup(
			pf_dev, queue_id, nb_desc, socket_id, conf, *pf_mempool);
		if (ret) {
			N3K_VDEV_LOG(ERR, "Failed to set up RX queue #%d for PF %s of %s",
				queue_id, pf_dev->data->name, dev->data->name);
			n3k_vdev_pf_mempool_free(pf_mempool);
			return ret;
		}
		n3k_vdev_save_pf_rx_config(
			dev_shared, queue_id, nb_desc, socket_id, conf);

	} else {
		ret = n3k_vdev_check_pf_rx_config(
			dev_shared, queue_id, nb_desc, socket_id, conf);
		if (ret) {
			N3K_VDEV_LOG(ERR, "Configuration of RX queue #%d for %s does not "
							 "match PF's queue configuration", queue_id,
							 dev->data->name);
			return ret;
		}
	}

	struct n3k_vdev_queue *queue = n3k_zmalloc_socket("n3k_rx_queue",
		sizeof(struct n3k_vdev_queue), 0, socket_id);

	if (!queue) {
		ret = -ENOMEM;
		goto error;
	}

	queue->queue_id = queue_id;
	queue->dev = dev;
	queue->mempool = mb_pool;

	ret = n3k_vdev_create_rings(
		queue, dev->data->name, "rx", queue_id, nb_desc, socket_id);
	if (ret)
		goto error;

	ret = n3k_vdev_fill_ring_with_free_mbufs(queue->ring_free, queue->mempool);
	if (ret)
		goto error;

	dev->data->rx_queues[queue_id] = queue;
	dev_shared->configured_rx_queues_count[queue_id]++;

	return 0;

error:
	/* TODO: Uncomment after fixing virtio queues logic */
	// if (pf_mempool)
	// 	n3k_vdev_pf_mempool_free(pf_mempool);

	if (queue) {
		rte_ring_free(queue->ring_free);
		rte_ring_free(queue->ring_used);
		n3k_free(queue);
	}

	return ret;
}

void
n3k_vdev_rx_queue_release(void *rx_queue)
{
	struct n3k_vdev_dev_internal *dev_internal;
	struct n3k_vdev_dev_shared *dev_shared;

	struct n3k_vdev_queue *queue = rx_queue;
	if (!queue)
		return;

	dev_internal = queue->dev->data->dev_private;
	dev_shared = dev_internal->shared;

	N3K_VDEV_LOG(DEBUG, "Releasing RX queue #%d of %s",
		queue->queue_id, queue->dev->data->name);

	dev_shared->configured_rx_queues_count[queue->queue_id]--;

	/* TODO: Free mbufs from rings */

	/* TODO: Uncomment after fixing virtio queues logic */
	// if (dev_shared->configured_rx_queues_count[queue->queue_id] == 0)
	// 	n3k_vdev_pf_mempool_free(&dev_shared->pf_rx_mempools[queue->queue_id]);

	rte_ring_free(queue->ring_free);
	rte_ring_free(queue->ring_used);
	n3k_free(queue);
}

static int
n3k_vdev_pf_tx_queue_setup(struct rte_eth_dev *pf_dev, uint16_t queue_id,
	uint16_t nb_desc, unsigned int socket_id,
	const struct rte_eth_txconf *conf)
{
	int ret = 0;
	bool was_started = false;

	if (pf_dev->data->dev_started) {
		was_started = true;
		rte_eth_dev_stop(pf_dev->data->port_id);
	}

	ret = rte_eth_tx_queue_setup(
		pf_dev->data->port_id, queue_id, nb_desc, socket_id, conf);
	if (ret)
		return ret;

	if (was_started) {
		ret = rte_eth_dev_start(pf_dev->data->port_id);
		if (ret)
			return ret;
	}

	return ret;
}

static void
n3k_vdev_save_pf_tx_config(struct n3k_vdev_dev_shared *dev_shared,
	uint16_t queue_id, uint16_t nb_desc, unsigned int socket_id,
	const struct rte_eth_txconf *conf)
{
	struct n3k_vdev_pf_tx_conf *queue_conf = &dev_shared->pf_tx_conf[queue_id];
	queue_conf->nb_desc = nb_desc;
	queue_conf->socket_id = socket_id;
	rte_memcpy(&queue_conf->conf, conf, sizeof(struct rte_eth_txconf));
}

static int
n3k_vdev_check_pf_tx_config(struct n3k_vdev_dev_shared *dev_shared,
	uint16_t queue_id, uint16_t nb_desc, unsigned int socket_id __rte_unused,
	const struct rte_eth_txconf *conf)
{
	struct n3k_vdev_pf_tx_conf lcfg = dev_shared->pf_tx_conf[queue_id];
	struct rte_eth_txconf rte_lcfg = lcfg.conf;
	struct rte_eth_thresh th_lcfg = rte_lcfg.tx_thresh;

	/* TODO: Uncomment after implementing proper handling of NUMA nodes */
	if (lcfg.nb_desc != nb_desc /*|| lcfg.socket_id != socket_id*/)
		return -EINVAL;

	if (rte_lcfg.tx_free_thresh != conf->tx_free_thresh ||
			rte_lcfg.tx_rs_thresh != conf->tx_rs_thresh ||
			rte_lcfg.tx_deferred_start != conf->tx_deferred_start ||
			rte_lcfg.offloads != conf->offloads)
		return -EINVAL;

	if (th_lcfg.pthresh != conf->tx_thresh.pthresh ||
			th_lcfg.hthresh != conf->tx_thresh.hthresh ||
			th_lcfg.wthresh != conf->tx_thresh.wthresh)
		return -EINVAL;

	return 0;
}


int
n3k_vdev_tx_queue_setup(struct rte_eth_dev *dev, uint16_t queue_id,
	uint16_t nb_desc, unsigned int socket_id,
	const struct rte_eth_txconf *conf)
{
	int ret = 0;
	struct rte_mempool **pf_mempool = NULL;

	struct n3k_vdev_dev_internal *dev_internal = dev->data->dev_private;
	struct n3k_vdev_dev_shared *dev_shared = dev_internal->shared;
	struct rte_eth_dev *pf_dev = dev_shared->pf_dev;

	N3K_VDEV_LOG(DEBUG, "Setting up TX queue #%d for %s [Desc: %d; Socket: %d]",
		queue_id, dev->data->name, nb_desc, socket_id);

	if (dev_shared->configured_tx_queues_count[queue_id] == 0) {
		pf_mempool = &dev_shared->pf_tx_mempools[queue_id];

		/* TODO: Change after fixing virtio queues logic */
		if (!*pf_mempool)
			*pf_mempool = n3k_vdev_pf_mempool_alloc(
				pf_dev->device->name, "tx", queue_id, nb_desc, socket_id);

		if (!*pf_mempool) {
			N3K_VDEV_LOG(ERR,
				"Failed to allocate mempool for TX queue #%d for PF %s of %s",
				queue_id, pf_dev->data->name, dev->data->name);
			return -ENOMEM;
		}

		ret = n3k_vdev_pf_tx_queue_setup(
			pf_dev, queue_id, nb_desc, socket_id, conf);
		if (ret) {
			N3K_VDEV_LOG(ERR, "Failed to set up TX queue #%d for PF %s of %s",
				queue_id, pf_dev->data->name, dev->data->name);
			n3k_vdev_pf_mempool_free(pf_mempool);
			return ret;
		}

		n3k_vdev_save_pf_tx_config(
			dev_shared, queue_id, nb_desc, socket_id, conf);
	} else {
		ret = n3k_vdev_check_pf_tx_config(
			dev_shared, queue_id, nb_desc, socket_id, conf);
		if (ret) {
			N3K_VDEV_LOG(ERR, "Configuration of RX queue #%d for %s does not "
							 "match PF's queue configuration", queue_id,
							 dev->data->name);
			return ret;
		}
	}

	struct n3k_vdev_queue *queue = n3k_zmalloc_socket("n3k_tx_queue",
		sizeof(struct n3k_vdev_queue), 0, socket_id);

	if (!queue) {
		ret = -ENOMEM;
		goto error;
	}

	queue->queue_id = queue_id;
	queue->dev = dev;
	queue->mempool = dev_shared->pf_tx_mempools[queue_id];

	ret = n3k_vdev_create_rings(
		queue, dev->data->name, "tx", queue_id, nb_desc, socket_id);
	if (ret)
		goto error;

	dev->data->tx_queues[queue_id] = queue;
	dev_shared->configured_tx_queues_count[queue_id]++;

	return 0;

error:
	/* TODO: Uncomment after fixing virtio queues logic */
	// if (pf_mempool)
	// 	n3k_vdev_pf_mempool_free(pf_mempool);

	if (queue) {
		rte_ring_free(queue->ring_free);
		rte_ring_free(queue->ring_used);
		n3k_free(queue);
	}

	return ret;
}

void
n3k_vdev_tx_queue_release(void *tx_queue)
{
	struct n3k_vdev_dev_internal *dev_internal;
	struct n3k_vdev_dev_shared *dev_shared;

	struct n3k_vdev_queue *queue = tx_queue;
	if (!queue)
		return;

	dev_internal = queue->dev->data->dev_private;
	dev_shared = dev_internal->shared;

	N3K_VDEV_LOG(DEBUG, "Releasing TX queue #%d of %s",
		queue->queue_id, queue->dev->data->name);

	dev_shared->configured_tx_queues_count[queue->queue_id]--;

	/* TODO: Free mbufs from rings */

	/* TODO: Uncomment after fixing virtio queues logic */
	// if (dev_shared->configured_tx_queues_count[queue->queue_id] == 0)
	// 	n3k_vdev_pf_mempool_free(&dev_shared->pf_tx_mempools[queue->queue_id]);

	rte_ring_free(queue->ring_free);
	rte_ring_free(queue->ring_used);
	n3k_free(queue);
}

uint16_t
n3k_vdev_rx_burst(void *rx_queue, struct rte_mbuf **rx_pkts, uint16_t nb_pkts)
{
	struct n3k_vdev_queue *queue = rx_queue;

	n3k_vdev_fill_ring_with_free_mbufs(queue->ring_free, queue->mempool);

	return rte_ring_dequeue_burst(
		queue->ring_used, (void **)rx_pkts, nb_pkts, NULL);
}

static inline void
n3k_vdev_free_all_mbufs_from_ring(struct rte_ring *ring)
{
	struct rte_mbuf *pkt;
	while (rte_ring_dequeue(ring, (void **)(&pkt)) == 0) {
		rte_pktmbuf_free(pkt);
	}
}

uint16_t
n3k_vdev_tx_burst(void *tx_queue, struct rte_mbuf **tx_pkts, uint16_t nb_pkts)
{
	struct n3k_vdev_queue *queue = tx_queue;

	/* TODO: Think about using tx_free_thresh from config */
	n3k_vdev_free_all_mbufs_from_ring(queue->ring_free);

	return rte_ring_enqueue_burst(
		queue->ring_used, (void **)tx_pkts, nb_pkts, NULL);
}
