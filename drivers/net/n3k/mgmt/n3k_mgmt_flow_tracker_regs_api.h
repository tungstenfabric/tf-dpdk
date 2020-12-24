/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_FLOW_TRACKER_REGS_API_H_
#define _PMD_NET_N3K_MGMT_FLOW_TRACKER_REGS_API_H_

#include "n3k_mgmt_regs.h"

enum MKS_FLOW_TRACKER_FT_COMMAND_OP_TYPE {
  MKS_FLOW_TRACKER_FT_COMMAND_READ = 0,
  MKS_FLOW_TRACKER_FT_COMMAND_READ_CLEAR = 1,
  MKS_FLOW_TRACKER_FT_COMMAND_WRITE = 2,
  MKS_FLOW_TRACKER_FT_COMMAND_PARTIAL_WRITE = 3,
  MKS_FLOW_TRACKER_FT_COMMAND_FLUSH = 4,
};

struct __attribute__((packed, aligned(4))) flow_tracker_data_regs {
  MKS_FLOW_TRACKER_FT_DATA_0_t data_0;
  MKS_FLOW_TRACKER_FT_DATA_1_t data_1;
  MKS_FLOW_TRACKER_FT_DATA_2_t data_2;
  MKS_FLOW_TRACKER_FT_DATA_3_t data_3;
  MKS_FLOW_TRACKER_FT_DATA_4_t data_4;
  MKS_FLOW_TRACKER_FT_DATA_5_t data_5;
  MKS_FLOW_TRACKER_FT_DATA_6_t data_6;
  MKS_FLOW_TRACKER_FT_DATA_7_t data_7;
  MKS_FLOW_TRACKER_FT_DATA_8_t data_8;
  MKS_FLOW_TRACKER_FT_DATA_9_t data_9;
  MKS_FLOW_TRACKER_FT_DATA_10_t data_10;
  MKS_FLOW_TRACKER_FT_DATA_11_t data_11;
  MKS_FLOW_TRACKER_FT_DATA_12_t data_12;
  MKS_FLOW_TRACKER_FT_DATA_13_t data_13;
  MKS_FLOW_TRACKER_FT_DATA_14_t data_14;
  MKS_FLOW_TRACKER_FT_DATA_15_t data_15;
};

#endif /* _PMD_NET_N3K_MGMT_FLOW_TRACKER_REGS_API_H_ */
