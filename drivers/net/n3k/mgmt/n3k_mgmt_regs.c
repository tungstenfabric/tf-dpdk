/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_regs.h"
#include "n3k_mgmt_hw.h"

#include "ifc/ifc_port.h"

uint32_t
n3k_mgmt_regs_get_eml_version(struct n3k_mgmt_hw *hw)
{
	EM_LKUP_EML_VERSION_t version;

