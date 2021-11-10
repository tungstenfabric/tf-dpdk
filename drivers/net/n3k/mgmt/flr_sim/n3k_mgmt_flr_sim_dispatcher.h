/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_FLR_SIM_DISPATCHER_H_
#define _PMD_NET_N3K_MGMT_FLR_SIM_DISPATCHER_H_

#include <rte_ethdev.h>

void n3k_mgmt_flr_sim_dispatcher_rx(
	struct rte_eth_dev *mgmt_dev, struct rte_eth_dev *pf_dev);

#endif /* _PMD_NET_N3K_MGMT_FLR_SIM_DISPATCHER_H_ */
