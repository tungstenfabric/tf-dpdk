/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _N3K_MGMT_LAG_H_
#define _N3K_MGMT_LAG_H_

#include "n3k_mgmt_hw.h"

int n3k_mgmt_lag_init(struct n3k_mgmt_hw *hw);
void n3k_mgmt_lag_enable(struct n3k_mgmt_hw *hw);
void n3k_mgmt_lag_disable(struct n3k_mgmt_hw *hw);

int n3k_mgmt_get_lag_port_id(struct n3k_mgmt_hw *hw);
int n3k_mgmt_is_port_valid_lag_bond(uint16_t port_id);

#endif /* _N3K_MGMT_LAG_H_ */
