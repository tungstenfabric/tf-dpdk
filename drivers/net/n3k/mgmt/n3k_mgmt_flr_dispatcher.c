/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "flr_sim/n3k_mgmt_flr_sim_queues_mgmt.h"
#include "n3k_mgmt_ethdev.h"
#include "n3k_mgmt_flr_cmd_queue.h"
#include "n3k_mgmt_flr_command.h"
#include "n3k_mgmt_flr_dispatcher.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_log.h"

#include <rte_ethdev.h>
#include <rte_hexdump.h>


/* This is just a rte_mempool ptr to avoid slow double ptr dereference */
struct n3k_mgmt_flr_dispatcher_internal;

int
n3k_mgmt_flr_dispatcher_init(struct n3k_mgmt_hw *hw)
{
	// TODO(n3k): PKTS_POOL_SIZE should be set to something greater than
	//            number of tx descriptors + max number of sent packets at
	//            once
	const size_t PKTS_POOL_SIZE = 64 * 1024 - 1;
	const size_t PKT_SIZE = RTE_PKTMBUF_HEADROOM + 2048;
	const size_t CACHE_SIZE =
		RTE_MIN(RTE_CACHE_LINE_SIZE, RTE_MEMPOOL_CACHE_MAX_SIZE);

	/* TODO(n3k): Set properly */
	const int SOCKET_ID = 0;

	N3K_MGMT_FLR_DISPATCHER_LOG(INFO, "Initializing FLR pkt pool");

	if (hw->flr_dispatcher_internal) {
		N3K_MGMT_FLR_DISPATCHER_LOG(ERR, "FLR pkt pool already initialized");
		return -EINVAL;
	}

	struct rte_mempool **mempool =
		(struct rte_mempool **)&hw->flr_dispatcher_internal;
	*mempool = rte_pktmbuf_pool_create(
		"n3k_flr_dispatcher_pkts",
		PKTS_POOL_SIZE, CACHE_SIZE, 0, PKT_SIZE, SOCKET_ID);
	if (!*mempool) {
		N3K_MGMT_FLR_DISPATCHER_LOG(ERR, "Failed to allocate rte_mempool");
		return -rte_errno;
	}

	return 0;
}

inline static void
dump_packet(const char *title, struct rte_mbuf *pkt)
{
	rte_hexdump(rte_log_get_stream(), title,
		(uint8_t*)pkt->buf_addr + pkt->data_off,
		pkt->data_len);
}

void
n3k_mgmt_flr_dispatcher_tx(
	struct rte_eth_dev *mgmt_dev, struct rte_eth_dev *pf_dev)
{
	/* TODO: Handle multiple queues */
	const uint16_t queue_id = 0;
	uint16_t idx;
	uint16_t processed_pkts = 0;
	struct rte_mbuf *pkts[N3K_MGMT_FLR_COMMAND_PROCESSED_MAX_COUNT];

	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(mgmt_dev);
	if (unlikely(!hw))
		return;

	struct rte_mempool *mempool =
		(struct rte_mempool *)hw->flr_dispatcher_internal;

	for (idx = 0; idx < N3K_MGMT_FLR_COMMAND_PROCESSED_MAX_COUNT; ++idx) {
		/* TODO: Optimize: get multiple packets maybe? */
		struct n3k_mgmt_flr_command *cmd =
			n3k_mgmt_flr_cmd_queue_process_command(hw);
		if (!cmd)
			break;

		struct rte_mbuf *pkt = rte_pktmbuf_alloc(mempool);
		if (!pkt) {
			/* The command is not freed here. Let it be handled by timeout. */
			N3K_MGMT_FLR_DISPATCHER_LOG(ERR, "Failed to allocate mbuf");
			break;
		}

		if (unlikely(n3k_mgmt_flr_command_to_pkt(hw, cmd, pkt))) {
			/* The command is not freed here. Let it be handled by timeout. */
			N3K_MGMT_FLR_DISPATCHER_LOG(ERR, "Failed to convert command to packet");
			rte_pktmbuf_free(pkt);
			continue;
		}

		if (N3K_MGMT_FLR_DISPATCHER_SHOULD_LOG(DEBUG)) {
			N3K_MGMT_FLR_DISPATCHER_LOG(DEBUG, "Prepared packet to send");
			dump_packet("Config packet", pkt);
		}

		pkts[processed_pkts++] = pkt;
	}

	uint16_t nb_sent = 0;
	if (hw->insert_type == N3K_MGMT_HW_INSERT_TYPE_FLR_SIM)
		nb_sent = n3k_mgmt_flr_sim_tx(hw, pkts, processed_pkts);
	else if (hw->insert_type == N3K_MGMT_HW_INSERT_TYPE_FLR)
		nb_sent = rte_eth_tx_burst(pf_dev->data->port_id, queue_id, pkts, processed_pkts);

	if (nb_sent) {
		N3K_MGMT_FLR_DISPATCHER_LOG(INFO, "Sent %" PRIu16 " FLR config packets", nb_sent);
		rte_atomic64_add(&hw->vf0_stats.flr.tx_pkts, nb_sent);
	}

	if (unlikely(nb_sent != processed_pkts)) {
		/* The commands are not freed here. Let them be handled by timeout. */
		N3K_MGMT_FLR_DISPATCHER_LOG(
			ERR, "Failed to send %d packets", processed_pkts - nb_sent);
		rte_pktmbuf_free_bulk(pkts + nb_sent, processed_pkts - nb_sent);
	}
}
