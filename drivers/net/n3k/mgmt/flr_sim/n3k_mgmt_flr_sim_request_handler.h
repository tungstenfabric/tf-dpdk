/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_FLR_SIM_REQUEST_HANDLER_H_
#define _PMD_NET_N3K_MGMT_FLR_SIM_REQUEST_HANDLER_H_

#include <stdint.h>

struct n3k_mgmt_hw;

int n3k_mgmt_flr_sim_request_handler_init(struct n3k_mgmt_hw *hw);

/* Parses pkt_data, executes request and modifies pkt_data so it stores the
 * response to the request.
 */
int n3k_mgmt_flr_sim_execute_request(struct n3k_mgmt_hw *hw, void *pkt_data, uint16_t pkt_size);

#endif /* _PMD_NET_N3K_MGMT_FLR_SIM_REQUEST_HANDLER_H_ */
