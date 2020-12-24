/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_VPLKP_TBL_REGS_API_H_
#define _PMD_NET_N3K_MGMT_VPLKP_TBL_REGS_API_H_

#include "n3k_mgmt_regs.h"

enum VPLKP_EMLC_MGMT_CTRL_CMDS {
  VPLKP_EMLC_MGMT_CTRL_CMD_FLUSH = 0,
  VPLKP_EMLC_MGMT_CTRL_CMD_INSERT = 1,
  VPLKP_EMLC_MGMT_CTRL_CMD_DELETE = 2,
  VPLKP_EMLC_MGMT_CTRL_CMD_SEARCH = 3,
};

#endif /* _PMD_NET_N3K_MGMT_VPLKP_TBL_REGS_API_H_ */
