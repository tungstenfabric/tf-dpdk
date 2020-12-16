/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_NIOS_H_
#define _PMD_NET_N3K_MGMT_NIOS_H_

#include <rte_ethdev.h>
#include "n3k_mgmt_hw.h"

int
n3k_mgmt_nios_handler_register_and_run(struct n3k_mgmt_hw *hw);

