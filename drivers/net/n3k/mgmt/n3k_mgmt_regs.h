/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_REGS_H_
#define _PMD_NET_N3K_MGMT_REGS_H_

#include <stdint.h>

#undef LKUP_ACCESSTYPE // avoid collisions with ifc_pm macro
#define GENHDR_STRUCT 1 // enable accessing registers as structs

// TODO(smartnic): Add ifdef guards for multiple registers versions.
#include "regs/n3k_mgmt_regs_mks_v01r41.h"

struct n3k_mgmt_hw;

// TODO(smartnic): Move it to separate file maybe? Not sure how
// init/register access will look in the future versions.
uint32_t n3k_mgmt_regs_get_eml_version(struct n3k_mgmt_hw *hw);

enum EM_LKUP_EML_VERSION_MASKS {
	EM_LKUP_EML_VERSION_MODULE_MASK = 0xff,
};

enum EM_LKUP_EML_VERSION_MODULE_INSTANCES {
	EM_LKUP_EML_VERSION_MODULE_MHL = 0x00,
