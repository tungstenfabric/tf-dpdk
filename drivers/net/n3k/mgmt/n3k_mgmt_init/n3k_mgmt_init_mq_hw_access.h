/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _N3K_MGMT_INIT_MQ_HW_ACCESS_H_
#define _N3K_MGMT_INIT_MQ_HW_ACCESS_H_

#include "n3k_mgmt_init_mq.h"

#include "mgmt/n3k_mgmt_hw.h"

#include <stdbool.h>

struct n3k_mgmt_init_vq;

void n3k_mgmt_init_hw_mq_enable(struct n3k_mgmt_hw *hw);
void n3k_mgmt_init_hw_max_vq_pairs_set(struct n3k_mgmt_hw *hw, u8 idx, u16 vqp);
void n3k_mgmt_init_hw_virtio_features_set(struct n3k_mgmt_hw *hw, u8 idx, bool is_mq);
void n3k_mgmt_init_hw_vqs_set(struct n3k_mgmt_hw *hw, u8 idx, u16 vqs);
void n3k_mgmt_init_hw_vf_map_set(struct n3k_mgmt_hw *hw, u8 idx, u8 is_pf, u16 vqs, u32 offset);
void n3k_mgmt_init_hw_queue_map_set(struct n3k_mgmt_hw *hw, struct n3k_mgmt_init_vq *vqs);
void n3k_mgmt_init_hw_pvf_xlate_set(struct n3k_mgmt_hw *hw, struct n3k_mgmt_init_vq *vqs);
void n3k_mgmt_init_hw_pvf_xlate_unset(struct n3k_mgmt_hw *hw, struct n3k_mgmt_init_vq *vqs);
void n3k_mgmt_init_hw_vf_unmap(struct n3k_mgmt_hw *hw, u8 idx);
void n3k_mgmt_init_hw_queue_unmap(struct n3k_mgmt_hw *hw, struct n3k_mgmt_init_vq *vqs);
#endif //_N3K_MGMT_INIT_MQ_HW_ACCESS_H_
