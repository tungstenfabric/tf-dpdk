/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_FLR_SIM_QUEUES_MGMT_H_
#define _PMD_NET_N3K_MGMT_FLR_SIM_QUEUES_MGMT_H_

#include <rte_mbuf.h>
#include <stdint.h>

struct n3k_mgmt_hw;

struct n3k_mgmt_flr_sim_queues_sim_params {
	// Sum of probabilities shouldn't exceed 1
	double rx_drop_prob;
	double sleep_prob;
	double tx_drop_prob;
};

int n3k_mgmt_flr_sim_queues_mgmt_init(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flr_sim_queues_sim_params *params);
int n3k_mgmt_flr_sim_queues_mgmt_deinit(struct n3k_mgmt_hw *hw);

