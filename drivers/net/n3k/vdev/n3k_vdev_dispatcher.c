/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include <rte_ethdev.h>
#include <rte_hexdump.h>
#include <rte_service_component.h>
#include <rte_pmd_n3k_malloc.h>

#include "common/n3k_time_utils.h"

#include "n3k_vdev_log.h"
#include "n3k_vdev_dispatcher.h"
#include "n3k_vdev_ifc.h"
#include "n3k_vdev_device.h"
#include "n3k_vdev_rxtx.h"

#include "mgmt/flr_sim/n3k_mgmt_flr_sim_dispatcher.h"
#include "mgmt/n3k_mgmt_ethdev.h"
#include "mgmt/n3k_mgmt_flr_ack_dispatcher.h"
#include "mgmt/n3k_mgmt_flr_dispatcher.h"
#include "mgmt/n3k_mgmt_hw.h"

enum { N3K_BURST_PKT_COUNT = 32 };
enum { N3K_QUEUE_FULL_MESSAGE_INTERVAL = 5 };

struct n3k_vdev_dispatcher_context {
	struct rte_eth_dev *mgmt;
	struct rte_eth_dev *pf;

	size_t vdevs_count;
	size_t next_tx_vdev_idx[RTE_MAX_QUEUES_PER_PORT];
	size_t next_dedicated_tx_vdev_idx;
	struct rte_eth_dev *vdevs[RTE_MAX_ETHPORTS];

	struct rte_eth_dev *phy_devices_map[N3K_PHY_PORTS_COUNT];
	struct rte_eth_dev *vf_devices_map[
		N3K_VF_PORTS_MAX_COUNT + N3K_VF_PORT_MIN_ID];

	struct timespec last_queue_full_check_time;
};

struct n3k_vdev_dispatcher {
	uint32_t service_id;
	struct n3k_vdev_dispatcher_context ctx;
};

static inline void
n3k_vdev_copy_inbound_mbuf(
	struct rte_mbuf *dst_pkt, const struct rte_mbuf *src_pkt)
{
	const char *src_data;
	char *dst_data;
	size_t l3_offset;

	dst_data = rte_pktmbuf_append(dst_pkt,
		src_pkt->pkt_len - N3K_VDEV_IFC_HEADER_SIZE);
	if (!dst_data) {
		N3K_VDEV_LOG(DISPATCHER, WARNING,
			"Mbuf copy failed: multiple segments or buffer size mismatch.");
		return;
	}

	src_data = rte_pktmbuf_mtod(src_pkt, const char *);
	rte_memcpy(dst_data, src_data, N3K_VDEV_IFC_HEADER_OFFSET);
	dst_data += N3K_VDEV_IFC_HEADER_OFFSET;

	l3_offset = N3K_VDEV_IFC_HEADER_OFFSET + N3K_VDEV_IFC_HEADER_SIZE;
	src_data = rte_pktmbuf_mtod_offset(src_pkt, const char *, l3_offset);
	rte_memcpy(dst_data, src_data, src_pkt->pkt_len - l3_offset);
}

static void
dump_packet(const char *title, struct rte_mbuf *pkt)
{
	rte_hexdump(rte_log_get_stream(), title,
		(uint8_t*)pkt->buf_addr + pkt->data_off,
		pkt->data_len);
}

static bool
is_ifc_lacp_pkt(const struct rte_mbuf *pkt)
{
	if (pkt->data_len < N3K_VDEV_IFC_HEADER_SIZE + sizeof(struct rte_ether_hdr))
		return false;

	const struct rte_ether_hdr *hdr =
		rte_pktmbuf_mtod_offset(pkt, const struct rte_ether_hdr *,
		N3K_VDEV_IFC_HEADER_SIZE);

	return hdr->ether_type == rte_cpu_to_be_16(RTE_ETHER_TYPE_SLOW);
}

static inline void
n3k_vdev_dispatcher_rx_parsed_pkt(
	struct rte_eth_dev *vdev, uint16_t queue_id, struct rte_mbuf *src_pkt)
{
	struct rte_mbuf *dst_pkt;
	struct n3k_vdev_queue *queue;
	struct n3k_vdev_dev_internal *dev_internal = vdev->data->dev_private;

	if (!vdev->data->dev_started)
		return;

	if (dev_internal->dedicated_queue_enabled && is_ifc_lacp_pkt(src_pkt))
		queue_id = dev_internal->dedicated_queue_id;

	queue = vdev->data->rx_queues[queue_id];

	if (rte_ring_full(queue->ring_used)) {
		queue->pkts_dropped_since_last_queue_full_msg++;
		return;
	}

	if (rte_ring_dequeue(queue->ring_free, (void **)&dst_pkt)) {
		N3K_VDEV_LOG(DISPATCHER, WARNING, "RX Queue #%d for %s has no free entries",
			queue_id, vdev->data->name);
		return;
	}

	n3k_vdev_copy_inbound_mbuf(dst_pkt, src_pkt);

	rte_ring_enqueue(queue->ring_used, dst_pkt);
}

static inline struct rte_eth_dev *
n3k_vdev_map_port_id_to_vdev(
	struct n3k_vdev_dispatcher_context *ctx, uint16_t port_id)
{
	enum n3k_vdev_dev_type vdev_type;
	uint16_t vdev_id;

	n3k_vdev_ifc_port_id_to_dev(port_id, &vdev_type, &vdev_id);

	return vdev_type == N3K_VDEV_DEV_TYPE_PHY ?
		ctx->phy_devices_map[vdev_id] :
		ctx->vf_devices_map[vdev_id];
}

static inline void
n3k_vdev_dispatcher_rx_pkt(
	struct n3k_vdev_dispatcher_context *ctx,
	uint16_t queue_id, struct rte_mbuf *src_pkt)
{
	enum n3k_vdev_ifc_packet_type type;
	uint16_t port_id;
	struct rte_eth_dev *vdev;

	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(ctx->mgmt);
	if (unlikely(!hw))
		goto free_src_pkt;

	type = n3k_vdev_ifc_packet_parse_type(src_pkt);

	if (type == N3K_VDEV_PKT_TYPE_FLR) {
		RTE_ASSERT(hw->insert_type == N3K_MGMT_HW_INSERT_TYPE_FLR);
		int ret;
		rte_atomic64_inc(&hw->vf0_stats.flr.rx_pkts);
		if (N3K_VDEV_SHOULD_LOG(DISPATCHER, DEBUG))
			dump_packet("Received FLR config packet", src_pkt);
		ret = n3k_mgmt_flr_ack_dispatcher_push_response(hw, src_pkt);
		if (unlikely(ret < 0)) {
			N3K_VDEV_LOG(DISPATCHER, ERR,
				"Failed to push packet to FLR ACK queue. ret=%d", ret);
			goto free_src_pkt;
		}
		// src_pkt will be freed by ACK dispatcher
		return;
	}

	if (type != N3K_VDEV_PKT_TYPE_IFC) {
		N3K_VDEV_LOG(DISPATCHER, WARNING,
			"Received invalid packet on VF0 on queue %" PRIu16, queue_id);
		if (N3K_VDEV_SHOULD_LOG(DISPATCHER, WARNING))
			dump_packet("Received packet", src_pkt);
		goto free_src_pkt;
	}

	rte_atomic64_inc(&hw->vf0_stats.slow_path.rx_pkts);

	port_id = n3k_vdev_ifc_packet_valid_parse_port_id(src_pkt);

	vdev = n3k_vdev_map_port_id_to_vdev(ctx, port_id);
	if (!vdev) {
		N3K_VDEV_LOG(DISPATCHER, WARNING,
			"Received packet on VF0 from invalid port: %d", port_id);
		if (N3K_VDEV_SHOULD_LOG(DISPATCHER, WARNING))
			dump_packet("Received packet", src_pkt);
		goto free_src_pkt;
	}

	if (N3K_VDEV_SHOULD_LOG(DISPATCHER, DEBUG)) {
		char buff[128];
		snprintf(buff, sizeof(buff),
			"Received packet on VF0 from port: %s, queue: %" PRIu16,
			vdev->data->name, queue_id);
		dump_packet(buff, src_pkt);
	}

	n3k_vdev_dispatcher_rx_parsed_pkt(vdev, queue_id, src_pkt);

free_src_pkt:
	rte_pktmbuf_free(src_pkt);
}

static inline void
n3k_vdev_dispatcher_rx_on_queue(
	struct n3k_vdev_dispatcher_context *ctx, uint16_t queue_id)
{
	uint16_t pkt_id;
	uint16_t pkt_count;
	struct rte_mbuf *pkts[N3K_BURST_PKT_COUNT];

	pkt_count = rte_eth_rx_burst(
		ctx->pf->data->port_id, queue_id, pkts, N3K_BURST_PKT_COUNT);

	for (pkt_id = 0; pkt_id < pkt_count; ++pkt_id)
		n3k_vdev_dispatcher_rx_pkt(ctx, queue_id, pkts[pkt_id]);
}

static void
n3k_vdev_dispatcher_check_vdev_rx_queues_full(struct rte_eth_dev *vdev)
{
	uint16_t queue_id;
	for (queue_id = 0; queue_id < vdev->data->nb_rx_queues; ++queue_id) {
		struct n3k_vdev_queue *queue = vdev->data->rx_queues[queue_id];
		if (queue && queue->pkts_dropped_since_last_queue_full_msg) {
			N3K_VDEV_LOG(DISPATCHER, WARNING,
				"RX Queue #%d for %s is full. Dropped packets: %" PRIu64,
				queue->queue_id, vdev->data->name,
				queue->pkts_dropped_since_last_queue_full_msg);
			queue->pkts_dropped_since_last_queue_full_msg = 0;
		}
	}

}

static void
n3k_vdev_dispatcher_monitor_vdev_rx_queues(struct n3k_vdev_dispatcher_context *ctx)
{
	struct timespec cur_time;
	struct timespec diff_time;

	clock_gettime(CLOCK_MONOTONIC_RAW, &cur_time);
	n3k_timespec_diff(&cur_time, &ctx->last_queue_full_check_time, &diff_time);

	if (diff_time.tv_sec >= N3K_QUEUE_FULL_MESSAGE_INTERVAL) {
		size_t vdev_id;
		for (vdev_id = 0; vdev_id < ctx->vdevs_count; ++vdev_id)
			n3k_vdev_dispatcher_check_vdev_rx_queues_full(
				ctx->vdevs[vdev_id]);
		ctx->last_queue_full_check_time = cur_time;
	}
}

static inline void
n3k_vdev_dispatcher_rx(struct n3k_vdev_dispatcher_context *ctx)
{
	uint16_t queue_id;
	uint16_t queues_count = ctx->pf->data->nb_rx_queues;

	n3k_vdev_dispatcher_monitor_vdev_rx_queues(ctx);

	for (queue_id = 0; queue_id < queues_count; ++queue_id)
		n3k_vdev_dispatcher_rx_on_queue(ctx, queue_id);
}

static inline void
n3k_vdev_copy_outbound_mbuf(
	struct rte_mbuf *dst_pkt, const struct rte_mbuf *src_pkt,
	const struct rte_eth_dev *src_dev)
{
	const char *src_data;
	char *dst_data;

	struct n3k_vdev_ifc_header ifc_header;
	n3k_vdev_ifc_header_init(&ifc_header, src_dev);

	dst_data = rte_pktmbuf_append(dst_pkt,
		src_pkt->pkt_len + N3K_VDEV_IFC_HEADER_SIZE);
	if (!dst_data) {
		N3K_VDEV_LOG(DISPATCHER, WARNING,
			"Mbuf copy failed: multiple segments or buffer size mismatch.");
		return;
	}

	src_data = rte_pktmbuf_mtod(src_pkt, const char *);
	rte_memcpy(dst_data, src_data, N3K_VDEV_IFC_HEADER_OFFSET);
	dst_data += N3K_VDEV_IFC_HEADER_OFFSET;

	rte_memcpy(dst_data, &ifc_header, N3K_VDEV_IFC_HEADER_SIZE);
	dst_data += N3K_VDEV_IFC_HEADER_SIZE;

	src_data = rte_pktmbuf_mtod_offset(
		src_pkt, const char *, N3K_VDEV_IFC_HEADER_OFFSET);
	rte_memcpy(
		dst_data, src_data, src_pkt->pkt_len - N3K_VDEV_IFC_HEADER_OFFSET);
}

static inline uint16_t
n3k_vdev_dispatcher_tx_pkts(
	struct rte_eth_dev *pf_dev, struct rte_eth_dev *vf_dev,
	struct n3k_vdev_queue *vf_queue, uint16_t pf_queue_id,
	struct rte_mbuf **src_pkts, uint16_t count)
{
	uint16_t pkt_idx;
	uint16_t nb_sent;
	struct rte_mbuf *dst_pkts[N3K_BURST_PKT_COUNT];

	if (rte_pktmbuf_alloc_bulk(vf_queue->mempool, dst_pkts, count)) {
		N3K_VDEV_LOG(DISPATCHER, WARNING, "Mempool %s has not enough mbufs (%d needed)",
			vf_queue->mempool->name, count);
		return 0;
	}

	for (pkt_idx = 0; pkt_idx < count; ++pkt_idx)
		n3k_vdev_copy_outbound_mbuf(
			dst_pkts[pkt_idx], src_pkts[pkt_idx], vf_dev);

	nb_sent = rte_eth_tx_burst(
		pf_dev->data->port_id, pf_queue_id, dst_pkts, count);

	for (pkt_idx = nb_sent; pkt_idx < count; ++pkt_idx) {
		N3K_VDEV_LOG(DISPATCHER, WARNING, "TX Queue #%d for %s is full",
			pf_queue_id, pf_dev->data->name);
		rte_pktmbuf_free(dst_pkts[pkt_idx]);
	}

	return nb_sent;
}

static inline uint16_t
n3k_vdev_dispatcher_tx_on_dev(
	struct rte_eth_dev *vdev, struct rte_eth_dev *pf_dev,
	struct rte_eth_dev *mgmt_dev, uint16_t vf_queue_id, uint16_t pf_queue_id,
	uint16_t burst_count)
{
	struct n3k_vdev_queue *vf_queue;
	uint16_t pkt_count, real_pkt_count;
	struct rte_mbuf *pkts[N3K_BURST_PKT_COUNT];
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(mgmt_dev);
	if (!hw)
		return 0;

	if (!vdev->data->dev_started)
		return 0;

	vf_queue = vdev->data->tx_queues[vf_queue_id];

	pkt_count = rte_ring_free_count(vf_queue->ring_free);
	burst_count = RTE_MIN(pkt_count, burst_count);

	if (burst_count == 0)
		return 0;

	pkt_count = rte_ring_dequeue_burst(
		vf_queue->ring_used, (void **)pkts, burst_count, NULL);

	real_pkt_count = n3k_vdev_dispatcher_tx_pkts(
		pf_dev, vdev, vf_queue, pf_queue_id, pkts, pkt_count);
	rte_atomic64_add(&hw->vf0_stats.slow_path.tx_pkts, real_pkt_count);

	rte_ring_enqueue_burst(vf_queue->ring_free, (void **)pkts, pkt_count, NULL);

	return pkt_count;
}

static inline void
n3k_vdev_dispatcher_tx_on_queue(
	struct n3k_vdev_dispatcher_context *ctx, uint16_t queue_id)
{
	struct rte_eth_dev *vdev;
	size_t vdev_n;
	size_t next_vdev_idx = ctx->next_tx_vdev_idx[queue_id];
	uint16_t pkt_count = 0;

	for (vdev_n = 0; vdev_n < ctx->vdevs_count; ++vdev_n) {
		vdev = ctx->vdevs[next_vdev_idx];
		next_vdev_idx = (next_vdev_idx + 1 == ctx->vdevs_count) ?
			0 : (next_vdev_idx + 1);

		pkt_count += n3k_vdev_dispatcher_tx_on_dev(
			vdev, ctx->pf, ctx->mgmt, queue_id, queue_id,
			N3K_BURST_PKT_COUNT - pkt_count);

		if (pkt_count == N3K_BURST_PKT_COUNT)
			break;
	}

	ctx->next_tx_vdev_idx[queue_id] = next_vdev_idx;
}

static inline void
n3k_vdev_dispatcher_tx_on_dedicated_queue(
	struct n3k_vdev_dispatcher_context *ctx)
{
	struct rte_eth_dev *vdev;
	size_t vdev_n;
	size_t next_vdev_idx = ctx->next_dedicated_tx_vdev_idx;
	uint16_t pkt_count = 0;

	for (vdev_n = 0; vdev_n < ctx->vdevs_count; ++vdev_n) {
		vdev = ctx->vdevs[next_vdev_idx];
		next_vdev_idx = (next_vdev_idx + 1 == ctx->vdevs_count) ?
			0 : (next_vdev_idx + 1);

		struct n3k_vdev_dev_internal *dev_internal = vdev->data->dev_private;
		if (!dev_internal->dedicated_queue_enabled)
			continue;

		pkt_count += n3k_vdev_dispatcher_tx_on_dev(
			vdev, ctx->pf, ctx->mgmt, dev_internal->dedicated_queue_id, 0,
			N3K_BURST_PKT_COUNT - pkt_count);

		if (pkt_count == N3K_BURST_PKT_COUNT)
			break;
	}

	ctx->next_dedicated_tx_vdev_idx = next_vdev_idx;
}

static inline void
n3k_vdev_dispatcher_tx(struct n3k_vdev_dispatcher_context *ctx)
{
	uint16_t queue_id;
	uint16_t queues_count = ctx->pf->data->nb_tx_queues;

	n3k_vdev_dispatcher_tx_on_dedicated_queue(ctx);
	for (queue_id = 0; queue_id < queues_count; ++queue_id)
		n3k_vdev_dispatcher_tx_on_queue(ctx, queue_id);
}

static int32_t
n3k_vdev_dispatcher_run(void *args)
{
	struct n3k_vdev_dispatcher_context *ctx = args;
	if (!ctx->pf->data->dev_started)
		return -ENOTSUP;

	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(ctx->mgmt);
	if (!hw)
		return -EINVAL;

	n3k_vdev_dispatcher_rx(ctx);
	n3k_vdev_dispatcher_tx(ctx);

	if (hw->insert_type == N3K_MGMT_HW_INSERT_TYPE_FLR_SIM)
		n3k_mgmt_flr_sim_dispatcher_rx(ctx->mgmt, ctx->pf);

	if (hw->insert_type == N3K_MGMT_HW_INSERT_TYPE_FLR_SIM ||
		hw->insert_type == N3K_MGMT_HW_INSERT_TYPE_FLR)
	{
		n3k_mgmt_flr_dispatcher_tx(ctx->mgmt, ctx->pf);
		n3k_mgmt_flr_ack_dispatcher_process_responses(ctx->mgmt);
		n3k_mgmt_flr_ack_dispatcher_process_command_timeouts(ctx->mgmt);
	}

	return 0;
}

static void
n3k_vdev_dispatcher_init_vdev_maps(struct n3k_vdev_dispatcher_context *ctx)
{
	size_t vdev_idx;
	struct rte_eth_dev *dev;
	struct n3k_vdev_dev_internal *dev_internal;

	for (vdev_idx = 0; vdev_idx < ctx->vdevs_count; ++vdev_idx) {
		dev = ctx->vdevs[vdev_idx];
		dev_internal = dev->data->dev_private;

		if (dev_internal->type == N3K_VDEV_DEV_TYPE_PHY) {
			ctx->phy_devices_map[dev_internal->id] = dev;
		} else {
			ctx->vf_devices_map[dev_internal->id] = dev;
		}
	}
}

static struct n3k_vdev_dispatcher *
n3k_vdev_dispatcher_alloc(
	struct rte_eth_dev *mgmt_dev, struct rte_eth_dev *pf_dev,
	size_t vdevs_count, struct rte_eth_dev **vdevs)
{
	struct n3k_vdev_dispatcher_context *ctx;
	struct n3k_vdev_dispatcher *dispatcher = n3k_zmalloc(
		"n3k_vdev_dispatcher", sizeof(*dispatcher), 0);

	if (!dispatcher)
		return NULL;

	ctx = &dispatcher->ctx;

	ctx->mgmt = mgmt_dev;
	ctx->pf = pf_dev;
	ctx->vdevs_count = vdevs_count;
	memcpy(ctx->vdevs, vdevs, sizeof(*vdevs) * vdevs_count);

	n3k_vdev_dispatcher_init_vdev_maps(ctx);

	return dispatcher;
}

struct n3k_vdev_dispatcher *
n3k_vdev_dispatcher_register(
	struct rte_eth_dev *mgmt_dev, struct rte_eth_dev *pf_dev,
	size_t vdevs_count, struct rte_eth_dev **vdevs)
{
	struct rte_service_spec spec;

	struct n3k_vdev_dispatcher *dispatcher =
		n3k_vdev_dispatcher_alloc(mgmt_dev, pf_dev, vdevs_count, vdevs);
	if (!dispatcher)
		return NULL;

	snprintf(spec.name, sizeof(spec.name), "%s_dispatcher", "net_n3k0");
	spec.callback = n3k_vdev_dispatcher_run;
	spec.callback_userdata = &dispatcher->ctx;
	spec.capabilities = 0;
	spec.socket_id = 0; /* TODO: Set properly when NUMA nodes are implemented */

	if (rte_service_component_register(&spec, &dispatcher->service_id)) {
		n3k_free(dispatcher);
		return NULL;
	}

	return dispatcher;
}

void
n3k_vdev_dispatcher_unregister(struct n3k_vdev_dispatcher *dispatcher)
{
	n3k_vdev_dispatcher_stop(dispatcher);
	rte_service_component_unregister(dispatcher->service_id);
}

void
n3k_vdev_dispatcher_start(struct n3k_vdev_dispatcher *dispatcher)
{
	rte_service_component_runstate_set(dispatcher->service_id, 1);
}

void
n3k_vdev_dispatcher_stop(struct n3k_vdev_dispatcher *dispatcher)
{
	rte_service_component_runstate_set(dispatcher->service_id, 0);

	rte_mb();

	while (rte_service_may_be_active(dispatcher->service_id))
		continue;
}

/* TODO: Stop/start dispatcher on dev configure and queues configure */
