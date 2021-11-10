/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "vdev/n3k_vdev_ifc.h"
#include "mgmt/n3k_mgmt_hw.h"
#include "mgmt/n3k_mgmt_log.h"
#include "n3k_mgmt_flr_sim_queues_mgmt.h"
#include "n3k_mgmt_flr_sim_request_handler.h"

#include <rte_pmd_n3k_malloc.h>
#include <rte_service_component.h>
#include <rte_random.h>

#define N3K_MGMT_FLR_SERVICE_NAME "n3k_mgmt_flr_service"

#define N3K_MGMT_FLR_REQ_RING_NAME "n3k_mgmt_flr_req_ring"
#define N3K_MGMT_FLR_RESP_RING_NAME "n3k_mgmt_flr_resp_ring"
#define N3K_MGMT_FLR_MEMPOOL_NAME "n3k_mgmt_flr_entries_pool"
#define N3K_MGMT_FLR_MEMPOOL_SIZE 1024 - 1
#define N3K_FLR_QUEUE_SIZE 8

struct n3k_mgmt_flr_sim_queues_mgmt_internal {
	struct rte_ring *req_queue;
	struct rte_ring *resp_queue;
	struct rte_mempool *pkt_mp;
	uint32_t service_id;
	struct {
		uint64_t rx_drop;
		uint64_t sleep;
		uint64_t tx_drop;
	} action_prob_shares;
};

static int32_t n3k_mgmt_flr_handle_requests_service_callback(void *args);

static void
n3k_mgmt_flr_sim_cleanup_service(struct n3k_mgmt_hw *hw)
{
	struct n3k_mgmt_flr_sim_queues_mgmt_internal* flr_sim_queues_mgmt_internal =
	  hw->flr_sim_queues_mgmt_internal;
	int ret;

	ret = rte_service_component_unregister(flr_sim_queues_mgmt_internal->service_id);
	if (ret < 0) {
		N3K_MGMT_LOG(FLR_SIM, ERR,
			     "failed to unregister service for n3k_mgmt_flr "
			     "module; ret = %d",
			     ret);
		return;
	}
	N3K_MGMT_LOG(FLR_SIM, DEBUG,
		     "unregistered service callback for n3k_mgmt_flr module");
}

static void
n3k_mgmt_flr_sim_cleanup_queues(struct n3k_mgmt_hw *hw)
{
	struct n3k_mgmt_flr_sim_queues_mgmt_internal* flr_sim_queues_mgmt_internal =
	  hw->flr_sim_queues_mgmt_internal;

	if (flr_sim_queues_mgmt_internal->req_queue) {
		rte_ring_free(flr_sim_queues_mgmt_internal->req_queue);
		flr_sim_queues_mgmt_internal->req_queue = NULL;
	}

	if (flr_sim_queues_mgmt_internal->resp_queue) {
		rte_ring_free(flr_sim_queues_mgmt_internal->resp_queue);
		flr_sim_queues_mgmt_internal->resp_queue = NULL;
	}
}

static void
n3k_mgmt_flr_sim_cleanup_mempool(struct n3k_mgmt_hw *hw)
{
	struct n3k_mgmt_flr_sim_queues_mgmt_internal* flr_sim_queues_mgmt_internal =
	  hw->flr_sim_queues_mgmt_internal;

	if (flr_sim_queues_mgmt_internal->pkt_mp) {
		rte_mempool_free(flr_sim_queues_mgmt_internal->pkt_mp);
		flr_sim_queues_mgmt_internal->pkt_mp = NULL;
	}
}

static int
n3k_mgmt_flr_sim_setup_service(struct n3k_mgmt_hw *hw)
{
	struct n3k_mgmt_flr_sim_queues_mgmt_internal* flr_sim_queues_mgmt_internal =
	  hw->flr_sim_queues_mgmt_internal;
	struct rte_service_spec spec;
	int ret;

	memset(&spec, 0, sizeof(spec));
	strncpy(spec.name, N3K_MGMT_FLR_SERVICE_NAME, RTE_SERVICE_NAME_MAX);
	spec.callback = n3k_mgmt_flr_handle_requests_service_callback;
	spec.callback_userdata = hw;
	spec.capabilities = 0;
	spec.socket_id = 0; /* TODO(n3k): Set properly when the NUMA nodes
		functionality will be implemented */

	ret = rte_service_component_register(&spec, &flr_sim_queues_mgmt_internal->service_id);
	if (ret < 0) {
		N3K_MGMT_LOG(FLR_SIM, ERR,
			     "failed to register service for n3k_mgmt_flr "
			     "module; ret = %d",
			     ret);
		return ret;
	}
	N3K_MGMT_LOG(FLR_SIM, DEBUG,
		     "registered service callback for n3k_mgmt_flr module; "
		     "service_id = %d;",
		     flr_sim_queues_mgmt_internal->service_id);

	rte_service_component_runstate_set(flr_sim_queues_mgmt_internal->service_id, 1);
	return 0;
}

static int
n3k_mgmt_flr_sim_init_queues(struct n3k_mgmt_hw *hw)
{
	struct n3k_mgmt_flr_sim_queues_mgmt_internal* flr_sim_queues_mgmt_internal =
	  hw->flr_sim_queues_mgmt_internal;

	flr_sim_queues_mgmt_internal->req_queue = rte_ring_create(
	  N3K_MGMT_FLR_REQ_RING_NAME, N3K_FLR_QUEUE_SIZE, 0, RING_F_EXACT_SZ | RING_F_SC_DEQ);
	if (flr_sim_queues_mgmt_internal->req_queue == NULL) {
		N3K_MGMT_LOG(FLR_SIM, ERR, "failed to allocate rte_ring(%s)", N3K_MGMT_FLR_REQ_RING_NAME);
		return -rte_errno;
	}

	flr_sim_queues_mgmt_internal->resp_queue = rte_ring_create(
	  N3K_MGMT_FLR_RESP_RING_NAME, N3K_FLR_QUEUE_SIZE, 0, RING_F_EXACT_SZ | RING_F_SP_ENQ);
	if (flr_sim_queues_mgmt_internal->resp_queue == NULL) {
		N3K_MGMT_LOG(FLR_SIM, ERR, "failed to allocate rte_ring(%s)", N3K_MGMT_FLR_RESP_RING_NAME);
		n3k_mgmt_flr_sim_cleanup_queues(hw);
		return -rte_errno;
	}

	return 0;
}

static int
n3k_mgmt_flr_sim_init_mempool(struct n3k_mgmt_hw *hw)
{
	struct n3k_mgmt_flr_sim_queues_mgmt_internal* flr_sim_queues_mgmt_internal =
	  hw->flr_sim_queues_mgmt_internal;

	const uint32_t nb_elements = N3K_MGMT_FLR_MEMPOOL_SIZE;

	const uint32_t cache_size =
	  RTE_MIN(RTE_CACHE_LINE_SIZE, RTE_MEMPOOL_CACHE_MAX_SIZE);

	const uint16_t data_size = RTE_PKTMBUF_HEADROOM +
				   N3K_VDEV_IFC_HEADER_SIZE +
				   N3K_VDEV_MBUF_DEFAULT_DATAROOM;

	const int socket_id = 0;

	flr_sim_queues_mgmt_internal->pkt_mp =
	  rte_pktmbuf_pool_create(N3K_MGMT_FLR_MEMPOOL_NAME,
				  nb_elements,
				  cache_size,
				  0,
				  data_size,
				  socket_id);

	if (!flr_sim_queues_mgmt_internal->pkt_mp)
		return -rte_errno;

	return 0;
}

static inline int
init_action_probs(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flr_sim_queues_sim_params *params)
{
	const uint64_t rand_max = UINT64_MAX / 2;
	if (params->rx_drop_prob + params->sleep_prob + params->tx_drop_prob > 1)
		return -EINVAL;
	hw->flr_sim_queues_mgmt_internal->action_prob_shares.rx_drop = params->rx_drop_prob * rand_max;
	hw->flr_sim_queues_mgmt_internal->action_prob_shares.sleep = params->sleep_prob * rand_max;
	hw->flr_sim_queues_mgmt_internal->action_prob_shares.tx_drop = params->tx_drop_prob * rand_max;
	return 0;
}

int
n3k_mgmt_flr_sim_queues_mgmt_init(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flr_sim_queues_sim_params *params)
{
	int ret;

	if (hw->flr_sim_queues_mgmt_internal) {
		N3K_MGMT_LOG(FLR_SIM, ERR, "Flr simulator queues already initialized");
		return -EINVAL;
	}

	hw->flr_sim_queues_mgmt_internal = n3k_zmalloc("flr_sim_queues_mgmt_internal",
		sizeof(*hw->flr_sim_queues_mgmt_internal), 0);
	if (!hw->flr_sim_queues_mgmt_internal) {
		N3K_MGMT_LOG(FLR_SIM, ERR, "Failed to allocate flr_sim_queues_mgmt_internal");
		return -ENOMEM;
	}

	ret = init_action_probs(hw, params);
	if (ret < 0) {
		N3K_MGMT_LOG(FLR_SIM, ERR, "Failed to init flr sim probabilities");
		goto free_internal;
	}

	ret = n3k_mgmt_flr_sim_init_queues(hw);
	if (ret) {
		N3K_MGMT_LOG(FLR_SIM, ERR, "Failed to setup flr sim queues");
		goto free_internal;
	}

	ret = n3k_mgmt_flr_sim_setup_service(hw);
	if (ret) {
		N3K_MGMT_LOG(FLR_SIM, ERR, "Failed to setup flr sim queues mgmt service");
		goto free_queues;
	}

	ret = n3k_mgmt_flr_sim_init_mempool(hw);
	if (ret) {
		N3K_MGMT_LOG(FLR_SIM, ERR, "Failed to setup flr sim mbuf mempool");
		goto unregister_service;
	}

	return 0;

unregister_service:
	n3k_mgmt_flr_sim_cleanup_service(hw);
free_queues:
	n3k_mgmt_flr_sim_cleanup_queues(hw);
free_internal:
	n3k_free(hw->flr_sim_queues_mgmt_internal);
	hw->flr_sim_queues_mgmt_internal = NULL;
	return ret;
}

int
n3k_mgmt_flr_sim_queues_mgmt_deinit(struct n3k_mgmt_hw *hw)
{
	n3k_mgmt_flr_sim_cleanup_service(hw);
	n3k_mgmt_flr_sim_cleanup_queues(hw);
	n3k_mgmt_flr_sim_cleanup_mempool(hw);

	n3k_free(hw->flr_sim_queues_mgmt_internal);
	hw->flr_sim_queues_mgmt_internal = NULL;

	return 0;
}

struct packet_data {
	void *data;
	uint16_t size;
	enum {
		ACTION_EXECUTE,
		ACTION_DROP_RX,
		ACTION_DROP_TX,
		ACTION_SLEEP,
	} action;
};

static inline
void set_packet_action(struct n3k_mgmt_hw *hw, struct packet_data *packet)
{
	uint64_t rd = rte_rand() / 2;
	uint64_t rx_drop_share = hw->flr_sim_queues_mgmt_internal->action_prob_shares.rx_drop;
	uint64_t sleep_share = hw->flr_sim_queues_mgmt_internal->action_prob_shares.sleep;
	uint64_t tx_drop_share = hw->flr_sim_queues_mgmt_internal->action_prob_shares.tx_drop;
	if (rd <= rx_drop_share)
		packet->action = ACTION_DROP_RX;
	else if (rd - rx_drop_share <= sleep_share)
		packet->action = ACTION_SLEEP;
	else if (rd - rx_drop_share - sleep_share <= tx_drop_share)
		packet->action = ACTION_DROP_TX;
	else
		packet->action = ACTION_EXECUTE;
}

static void
free_packet_data(struct packet_data *pkt_data)
{
	n3k_free(pkt_data->data);
	n3k_free(pkt_data);
}

static int
copy_mbuf_to_packet_data(const struct rte_mbuf *pkt, struct packet_data **pkt_data)
{
	// TODO(n3k): maybe use mempool for packet_data struct?
	*pkt_data = n3k_malloc("flr_sim_packet_data", sizeof(**pkt_data), 0);
	if (*pkt_data == NULL) {
		N3K_MGMT_LOG(FLR_SIM, ERR, "failed to allocate memory for packet data");
		return -EINVAL;
	}
	(*pkt_data)->data = n3k_malloc("flr_sim_packet_data", pkt->data_len, 0);
	if ((*pkt_data)->data == NULL) {
		n3k_free(*pkt_data);
		N3K_MGMT_LOG(FLR_SIM, ERR, "failed to allocate memory for packet data");
		return -EINVAL;
	}

	(*pkt_data)->size = pkt->data_len;

	memcpy((*pkt_data)->data, ((uint8_t *)pkt->buf_addr + pkt->data_off), pkt->data_len);

	return 0;
}

static int
copy_packet_data_to_mbuf(const struct packet_data *pkt_data, struct rte_mbuf *pkt)
{
	void *added_data_ptr = rte_pktmbuf_append(pkt, pkt_data->size);
	if (added_data_ptr == NULL) {
		N3K_MGMT_LOG(FLR_SIM, ERR, "Error in resizing packet");
		return -ENOMEM;
	}

	memcpy(added_data_ptr, pkt_data->data, pkt_data->size);

	return 0;
}

static int32_t
n3k_mgmt_flr_handle_requests_service_callback(void *args)
{
	struct n3k_mgmt_hw *hw = args;
	struct n3k_mgmt_flr_sim_queues_mgmt_internal* flr_sim_queues_mgmt_internal =
	  hw->flr_sim_queues_mgmt_internal;
	struct packet_data *request_response;
	int ret;

	while (!rte_ring_dequeue(flr_sim_queues_mgmt_internal->req_queue, (void**)&request_response)) {
		N3K_MGMT_LOG(FLR_SIM, DEBUG, "Dequeuing request pkt from queue");

		if (request_response->action == ACTION_SLEEP) {
			uint32_t sleep_ms = rte_rand() % 1500;
			N3K_MGMT_LOG(FLR_SIM, DEBUG, "Sleeping before executing request");
			rte_delay_ms(sleep_ms);
		}

		ret = n3k_mgmt_flr_sim_execute_request(hw, request_response->data, request_response->size);
		if (ret < 0) {
			free_packet_data(request_response);
			N3K_MGMT_LOG(FLR_SIM, ERR, "failed to execute request; ret = %d", ret);
			continue;
		}

		N3K_MGMT_LOG(FLR_SIM, DEBUG, "Enqueuing response pkt to queue");

		if (request_response->action != ACTION_DROP_RX) {
			ret = rte_ring_enqueue(flr_sim_queues_mgmt_internal->resp_queue, request_response);
			if (ret < 0) {
				free_packet_data(request_response);
				N3K_MGMT_LOG(FLR_SIM, ERR, "failed to push flr response; ret = %d", ret);
				continue;
			}
		} else {
			N3K_MGMT_LOG(FLR_SIM, DEBUG, "Dropping response packet");
			free_packet_data(request_response);
		}
	}
	return 0;
}

uint16_t
n3k_mgmt_flr_sim_tx(struct n3k_mgmt_hw *hw, struct rte_mbuf **tx_pkts, uint16_t nb_pkts)
{
	struct n3k_mgmt_flr_sim_queues_mgmt_internal* flr_sim_queues_mgmt_internal =
	  hw->flr_sim_queues_mgmt_internal;
	uint16_t req_count = 0;
	int ret;
	int i;

	for (i = 0; i < nb_pkts; ++i) {
		struct packet_data *request;

		N3K_MGMT_LOG(FLR_SIM, DEBUG, "Enqueuing request pkt to queue");

		ret = copy_mbuf_to_packet_data(tx_pkts[i], &request);
		if (ret < 0) {
			N3K_MGMT_LOG(FLR_SIM, ERR, "failed to copy request pkt; ret = %d", ret);
			break;
		}
		set_packet_action(hw, request);

		if (request->action != ACTION_DROP_TX) {
			ret = rte_ring_enqueue(flr_sim_queues_mgmt_internal->req_queue, request);
			if (ret < 0) {
				free_packet_data(request);
				N3K_MGMT_LOG(FLR_SIM, ERR, "failed to push flr request; ret = %d", ret);
				break;
			}
		} else {
			N3K_MGMT_LOG(FLR_SIM, DEBUG, "Dropping request packet");
			free_packet_data(request);
		}

		++req_count;
	}
	rte_pktmbuf_free_bulk(tx_pkts, req_count);

	return req_count;
}

uint16_t
n3k_mgmt_flr_sim_rx(struct n3k_mgmt_hw *hw, struct rte_mbuf **rx_pkts, uint16_t nb_pkts)
{
	struct n3k_mgmt_flr_sim_queues_mgmt_internal* flr_sim_queues_mgmt_internal =
	  hw->flr_sim_queues_mgmt_internal;
	struct packet_data *response;
	uint16_t resp_count = 0;
	uint32_t i;
	int ret;

	for (i = 0; i < nb_pkts; i++) {
		rx_pkts[i] = rte_pktmbuf_alloc(flr_sim_queues_mgmt_internal->pkt_mp);
		if (rx_pkts[i] == NULL) {
			N3K_MGMT_LOG(FLR_SIM, ERR, "failed to allocate mbuf");
			break;
		}

		if (rte_ring_dequeue(flr_sim_queues_mgmt_internal->resp_queue,
				     (void**)&response)) {
			rte_pktmbuf_free(rx_pkts[i]);
			break;
		}

		N3K_MGMT_LOG(FLR_SIM, DEBUG, "Dequeuing response pkt from queue");

		ret = copy_packet_data_to_mbuf(response, rx_pkts[i]);
		if (ret < 0) {
			rte_pktmbuf_free(rx_pkts[i]);
			free_packet_data(response);
			N3K_MGMT_LOG(FLR_SIM, ERR,
				"failed to create pkt from response; ret = %d",
				ret);
			break;

		}
		free_packet_data(response);

		++resp_count;
	}

	return resp_count;
}
