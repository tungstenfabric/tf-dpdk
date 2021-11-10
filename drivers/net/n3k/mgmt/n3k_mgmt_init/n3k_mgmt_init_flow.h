/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _N3K_MGMT_INIT_FLOW_H_
#define _N3K_MGMT_INIT_FLOW_H_

#include <stdbool.h>
#include "n3k_mgmt_init_osdep.h"

#include "n3k_mgmt_init_port.h"

#include "mgmt/n3k_mgmt_hw.h"
#include "mgmt/regs/n3k_mgmt_regs.h"

int n3k_mgmt_init_ft_init(struct n3k_mgmt_hw* hw);
void n3k_mgmt_init_notify_init_begin(struct n3k_mgmt_hw* hw);
void n3k_mgmt_init_notify_init_done(struct n3k_mgmt_hw* hw);

void n3k_mgmt_init_msk_l4_src0(struct n3k_mgmt_hw* hw);

#endif //#ifndef _N3K_MGMT_INIT_FLOW_H_
