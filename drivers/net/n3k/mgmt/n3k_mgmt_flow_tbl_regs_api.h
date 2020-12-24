/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_FLOW_TBL_REGS_API_H_
#define _PMD_NET_N3K_MGMT_FLOW_TBL_REGS_API_H_

#include "n3k_mgmt_regs.h"

enum LKUP_EMLC_MGMT_CTRL_REQ_TYPE {
  LKUP_EMLC_MGMT_CTRL_REQ_TYPE_FLUSH = 0,
  LKUP_EMLC_MGMT_CTRL_REQ_TYPE_INSERT = 1,
  LKUP_EMLC_MGMT_CTRL_REQ_TYPE_DELETE = 2,
  LKUP_EMLC_MGMT_CTRL_REQ_TYPE_SEARCH = 3,
  LKUP_EMLC_MGMT_CTRL_REQ_TYPE_MODIFY_KEY = 4,
  LKUP_EMLC_MGMT_CTRL_REQ_TYPE_MODIFY_HANDLE = 5,
  LKUP_EMLC_MGMT_CTRL_REQ_TYPE_DELETE_HANDLE = 6,
  LKUP_EMLC_MGMT_CTRL_REQ_TYPE_SEARCH_HANDLE = 7,
  LKUP_EMLC_MGMT_CTRL_REQ_TYPE_INSERT_MODIFY = 8,
  LKUP_EMLC_MGMT_CTRL_REQ_TYPE_GET_HANDLE = 9,
  LKUP_EMLC_MGMT_CTRL_REQ_TYPE_FAST_INSERT_KEY = 10,
};

enum LKUP_EMLC_MGMT_CTRL_ERR_CODE {
  LKUP_EMLC_MGMT_CTRL_ERR_CODE_NONE = 0,
  LKUP_EMLC_MGMT_CTRL_ERR_CODE_INSERT_ROW_FULL = 1,
  LKUP_EMLC_MGMT_CTRL_ERR_CODE_INSERT_REHASH_FAIL = 2,
  LKUP_EMLC_MGMT_CTRL_ERR_CODE_INSERT_KEY_EXISTS = 3,
  LKUP_EMLC_MGMT_CTRL_ERR_CODE_PTR_MGMT = 4,
  LKUP_EMLC_MGMT_CTRL_ERR_CODE_INSERT_TABLE_FULL = 5,
  LKUP_EMLC_MGMT_CTRL_ERR_CODE_KEY_NOT_EXIST = 6,
  LKUP_EMLC_MGMT_CTRL_ERR_CODE_INVALID_OPCODE = 7,
  LKUP_EMLC_MGMT_CTRL_ERR_CODE_HASH_CHECKSUM = 8,
  LKUP_EMLC_MGMT_CTRL_ERR_CODE_KEY_CHECKSUM = 9,
};

struct __attribute__((packed, aligned(4))) lookup_emlc_key_regs {
  LKUP_EMLC_KEY_0_t key_0;
  LKUP_EMLC_KEY_1_t key_1;
  LKUP_EMLC_KEY_2_t key_2;
  LKUP_EMLC_KEY_3_t key_3;
  LKUP_EMLC_KEY_4_t key_4;
  LKUP_EMLC_KEY_5_t key_5;
  LKUP_EMLC_KEY_6_t key_6;
  LKUP_EMLC_KEY_7_t key_7;
  LKUP_EMLC_KEY_8_t key_8;
  LKUP_EMLC_KEY_9_t key_9;
  LKUP_EMLC_KEY_10_t key_10;
  LKUP_EMLC_KEY_11_t key_11;
  LKUP_EMLC_KEY_12_t key_12;
  LKUP_EMLC_KEY_13_t key_13;
  LKUP_EMLC_KEY_14_t key_14;
  LKUP_EMLC_KEY_15_t key_15;
  LKUP_EMLC_KEY_16_t key_16;
  LKUP_EMLC_KEY_17_t key_17;
};

struct __attribute__((packed, aligned(4))) lookup_emlc_res_regs {
  LKUP_EMLC_RES_0_t res_0;
  LKUP_EMLC_RES_1_t res_1;
  LKUP_EMLC_RES_2_t res_2;
  LKUP_EMLC_RES_3_t res_3;
  LKUP_EMLC_RES_4_t res_4;
  LKUP_EMLC_RES_5_t res_5;
  LKUP_EMLC_RES_6_t res_6;
  LKUP_EMLC_RES_7_t res_7;
  LKUP_EMLC_RES_8_t res_8;
  LKUP_EMLC_RES_9_t res_9;
  LKUP_EMLC_RES_10_t res_10;
  LKUP_EMLC_RES_11_t res_11;
  LKUP_EMLC_RES_12_t res_12;
  LKUP_EMLC_RES_13_t res_13;
  LKUP_EMLC_RES_14_t res_14;
  LKUP_EMLC_RES_15_t res_15;
  LKUP_EMLC_RES_16_t res_16;
  LKUP_EMLC_RES_17_t res_17;
  LKUP_EMLC_RES_18_t res_18;
  LKUP_EMLC_RES_19_t res_19;
  LKUP_EMLC_RES_20_t res_20;
  LKUP_EMLC_RES_21_t res_21;
  LKUP_EMLC_RES_22_t res_22;
};

#endif /* _PMD_NET_N3K_MGMT_FLOW_TBL_REGS_API_H_ */
