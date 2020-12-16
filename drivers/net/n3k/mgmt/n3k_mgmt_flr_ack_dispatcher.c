/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_ethdev.h"
#include "n3k_mgmt_flr_ack_dispatcher.h"
#include "n3k_mgmt_flr_cmd_queue.h"
#include "n3k_mgmt_flr_command.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_flr_pkts.h"

#include "../rte_pmd_n3k_malloc.h"

#include <rte_hexdump.h>
#include <rte_mbuf.h>
#include <rte_ring.h>
#include <stdint.h>

struct n3k_mgmt_flr_ack_dispatcher_internal {
	struct rte_ring *response_pkts;
};

static int
init_response_pkts_ring(struct rte_ring **queue)
{
	*queue = rte_ring_create(
		"n3k_flr_ack_dispatcher", N3K_MGMT_FLR_COMMAND_PROCESSED_MAX_COUNT, 0,
		RING_F_SC_DEQ | RING_F_SP_ENQ | RING_F_EXACT_SZ);
	if (*queue == NULL) {
		N3K_MGMT_FLR_ACK_DISPATCHER_LOG(ERR, "Failed to allocate rte_ring");
		return -rte_errno;
	}
	return 0;
}

int
n3k_mgmt_flr_ack_dispatcher_init(struct n3k_mgmt_hw *hw)
{
	int ret;
	N3K_MGMT_FLR_ACK_DISPATCHER_LOG(INFO, "Initializing FLR command queue");

	if (hw->flr_ack_dispatcher_internal) {
		N3K_MGMT_FLR_ACK_DISPATCHER_LOG(ERR, "FLR command queue already initialized");
		return -EINVAL;
	}

	hw->flr_ack_dispatcher_internal = n3k_zmalloc(
		"n3k_flr_cmd_queue", sizeof(*hw->flr_ack_dispatcher_internal), 0);
	if (!hw->flr_ack_dispatcher_internal) {
		N3K_MGMT_FLR_ACK_DISPATCHER_LOG(ERR, "Failed to allocate FLR command queue");
		return -ENOMEM;
	}

	ret = init_response_pkts_ring(&hw->flr_ack_dispatcher_internal->response_pkts);
	if (ret < 0) {
		N3K_MGMT_FLR_ACK_DISPATCHER_LOG(ERR, "Failed to init response pkt ring");
		n3k_free(hw->flr_ack_dispatcher_internal);
		hw->flr_ack_dispatcher_internal = NULL;
		return ret;
	}

	return 0;

}

int
n3k_mgmt_flr_ack_dispatcher_push_response(struct n3k_mgmt_hw *hw, struct rte_mbuf *response)
{
	return rte_ring_enqueue(hw->flr_ack_dispatcher_internal->response_pkts, response);
}

inline static void
dump_packet(const char *title, struct rte_mbuf *pkt)
{
	rte_hexdump(rte_log_get_stream(), title,
		(uint8_t*)pkt->buf_addr + pkt->data_off,
		pkt->data_len);
}

static void
handle_response_packet(struct n3k_mgmt_hw *hw, struct rte_mbuf *pkt)
{
	struct n3k_mgmt_flr_parsed_pkt parsed_pkt;
	int ret;
	struct n3k_mgmt_flr_command *cmd;

	if (N3K_MGMT_FLR_ACK_DISPATCHER_SHOULD_LOG(DEBUG)) {
		N3K_MGMT_FLR_ACK_DISPATCHER_LOG(DEBUG, "Received response packet");
		dump_packet("Response packet", pkt);
	}

	ret = n3k_mgmt_flr_parse_config_pkt(pkt, &parsed_pkt);
	if (unlikely(ret < 0)) {
		N3K_MGMT_FLR_ACK_DISPATCHER_LOG(ERR, "Failed to parse response pkt");
		return;
	}

	cmd = n3k_mgmt_flr_cmd_queue_pop_processed_command(hw, parsed_pkt.id);
	if (!cmd) {
		N3K_MGMT_FLR_ACK_DISPATCHER_LOG(WARNING,
			"Received FLR packet but it doesn't refer to any existing command.");
		return;
	}

	ret = n3k_mgmt_flr_command_handle_response(hw, cmd, &parsed_pkt);
	if (ret < 0) {
		N3K_MGMT_FLR_ACK_DISPATCHER_LOG(ERR,
			"Failed to handle command response. ret=%d", ret);
		return;
	}
}

void
n3k_mgmt_flr_ack_dispatcher_process_responses(struct rte_eth_dev *dev)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (unlikely(!hw))
		return;
	struct rte_ring *resp_queue = hw->flr_ack_dispatcher_internal->response_pkts;
	struct rte_mbuf *resp[N3K_MGMT_FLR_COMMAND_PROCESSED_MAX_COUNT];
	unsigned int resp_cnt;
	unsigned int i;

	resp_cnt = rte_ring_dequeue_burst(resp_queue, (void **)resp,
		N3K_MGMT_FLR_COMMAND_PROCESSED_MAX_COUNT, NULL);
	if (resp_cnt == 0)
		return;

	for (i = 0; i < resp_cnt; ++i)
		handle_response_packet(hw, resp[i]);

	rte_pktmbuf_free_bulk(resp, resp_cnt);
}

void
n3k_mgmt_flr_ack_dispatcher_process_command_timeouts(struct rte_eth_dev *dev)
{
	struct n3k_mgmt_flr_command *cmd;
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (unlikely(!hw))
		return;

	while ((cmd = n3k_mgmt_flr_cmd_queue_pop_timeouted_command(hw)) != NULL) {
		int ret;
		N3K_MGMT_FLR_ACK_DISPATCHER_LOG(WARNING,
			"Command timeouted: packet_id=%" PRIu16,
			cmd->metadata.packet_id);
		ret = n3k_mgmt_flr_command_handle_response(hw, cmd, NULL);
		if (ret < 0) {
			N3K_MGMT_FLR_ACK_DISPATCHER_LOG(ERR,
				"Failed to handle command timeout. ret=%d", ret);
