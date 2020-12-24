/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _N3K_MGMT_INIT_MQ_
#define _N3K_MGMT_INIT_MQ_

#include "../n3k_mgmt_hw.h"

struct n3k_mgmt_init_vq {
	u16 id;
	u16 next_queue;
	u8 pf;
	u8 vf;
	u8 size;
	u8 is_available : 1;
	u8 is_pf : 1;
	u8 is_ctrl : 1;
};

void
n3k_mgmt_init_vq_pool_init(void);

int
n3k_mgmt_init_set_vf_mq(struct n3k_mgmt_hw *hw, u8 pf, u8 vf, u8 queues);

int
n3k_mgmt_init_set_pf_mq(struct n3k_mgmt_hw *hw, u8 pf, u8 queues);

void
n3k_mgmt_init_get_pf_mq(u8 pf, u8 *queues);

void
n3k_mgmt_init_get_vf_mq(u8 pf, u8 vf, u8 *queues);

#endif /*_N3K_MGMT_INIT_MQ_*/
