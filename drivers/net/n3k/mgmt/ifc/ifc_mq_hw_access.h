/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (c) 2018, Intel Corporation. */

#ifndef _IFC_MQ_HW_ACCESS_H_
#define _IFC_MQ_HW_ACCESS_H_

#include "ifc_mq.h"

#include "../n3k_mgmt_hw.h"

#include <stdbool.h>

struct ifc_vq;

void ifc_hw_mq_enable(struct n3k_mgmt_hw *hw);
void ifc_hw_max_vq_pairs_set(struct n3k_mgmt_hw *hw, u8 idx, u16 vqp);
void ifc_hw_virtio_features_set(struct n3k_mgmt_hw *hw, u8 idx, bool is_mq);
void ifc_hw_vqs_set(struct n3k_mgmt_hw *hw, u8 idx, u16 vqs);
void ifc_hw_vf_map_set(struct n3k_mgmt_hw *hw, u8 idx, u8 is_pf, u16 vqs, u32 offset);
void ifc_hw_queue_map_set(struct n3k_mgmt_hw *hw, struct ifc_vq *vqs);
void ifc_hw_pvf_xlate_set(struct n3k_mgmt_hw *hw, struct ifc_vq *vqs);
void ifc_hw_pvf_xlate_unset(struct n3k_mgmt_hw *hw, struct ifc_vq *vqs);
void ifc_hw_vf_unmap(struct n3k_mgmt_hw *hw, u8 idx);
void ifc_hw_queue_unmap(struct n3k_mgmt_hw *hw, struct ifc_vq *vqs);
#endif //_IFC_MQ_HW_ACCESS_H_
