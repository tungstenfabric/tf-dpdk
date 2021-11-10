/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_RTE_FLOW_H_
#define _PMD_NET_N3K_MGMT_RTE_FLOW_H_

struct rte_flow_ops;

const struct rte_flow_ops *n3k_mgmt_rte_flow_get_ops(void);
const struct rte_flow_ops *n3k_mgmt_flr_rte_flow_get_ops(void);

#endif // _PMD_NET_N3K_MGMT_RTE_FLOW_H_
