/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_MP_H_
#define _PMD_NET_N3K_MGMT_MP_H_

#include "n3k_mgmt_hw.h"

#include <stdint.h>
#include <rte_pmd_n3k_mp.h>

int
n3k_mgmt_mp_init(struct n3k_mgmt_hw *hw);

void
n3k_mgmt_mp_exit(struct n3k_mgmt_hw *hw);

#endif /* _PMD_NET_N3K_MGMT_MP_H_ */
