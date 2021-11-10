/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "mgmt/n3k_mgmt_ethdev.h"
#include "mgmt/n3k_mgmt_flr_ack_dispatcher.h"
#include "mgmt/n3k_mgmt_flr_command.h"
#include "mgmt/n3k_mgmt_log.h"
#include "n3k_mgmt_flr_sim_dispatcher.h"
#include "n3k_mgmt_flr_sim_queues_mgmt.h"

#include <rte_ethdev.h>

void
n3k_mgmt_flr_sim_dispatcher_rx(
	struct rte_eth_dev *mgmt_dev, struct rte_eth_dev *pf_dev)
{
	int ret;
	uint16_t resp_cnt;
	uint16_t i;
	struct rte_mbuf *pkts[N3K_MGMT_FLR_COMMAND_PROCESSED_MAX_COUNT];

	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(mgmt_dev);
	if (unlikely(!hw))
		return;

	resp_cnt = n3k_mgmt_flr_sim_rx(hw, pkts, N3K_MGMT_FLR_COMMAND_PROCESSED_MAX_COUNT);
	RTE_SET_USED(pf_dev);
	rte_atomic64_add(&hw->vf0_stats.flr.rx_pkts, resp_cnt);

	for (i = 0; i < resp_cnt; ++i) {
		ret = n3k_mgmt_flr_ack_dispatcher_push_response(hw, pkts[i]);
		if (unlikely(ret < 0)) {
			N3K_MGMT_LOG(FLR_SIM, ERR, "Failed to push packet to FLR ACK queue");
			break;
		}
	}

	if (unlikely(i != resp_cnt)) {
		N3K_MGMT_LOG(FLR_SIM,
			ERR, "Failed to push %" PRIu16 " packets to FLR ACK queue",
			resp_cnt - i);
		rte_pktmbuf_free_bulk(pkts + i, resp_cnt - i);
	}
}
