/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _N3K_MGMT_INIT_API_LAG_CALLBACK_H_
#define _N3K_MGMT_INIT_API_LAG_CALLBACK_H_

#include "../n3k_mgmt_hw.h"

void lag_enable(struct n3k_mgmt_hw *hw);
void lag_disable(struct n3k_mgmt_hw *hw);

#endif /* _N3K_MGMT_INIT_API_LAG_CALLBACK_H_ */
