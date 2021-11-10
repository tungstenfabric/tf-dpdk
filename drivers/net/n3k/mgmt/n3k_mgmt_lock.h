/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _N3K_MGMT_LOCK_H_
#define _N3K_MGMT_LOCK_H_

#include "n3k_mgmt_hw.h"

void n3k_mgmt_lock(struct n3k_mgmt_hw *hw);
void n3k_mgmt_unlock(struct n3k_mgmt_hw *hw);

#endif // _N3K_MGMT_LOCK_H_
