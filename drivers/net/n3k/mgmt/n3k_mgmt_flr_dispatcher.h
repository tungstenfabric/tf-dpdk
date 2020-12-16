/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_FLR_DISPATCHER_H_
#define _PMD_NET_N3K_MGMT_FLR_DISPATCHER_H_

struct rte_eth_dev;
struct n3k_mgmt_hw;

int n3k_mgmt_flr_dispatcher_init(struct n3k_mgmt_hw *hw);

