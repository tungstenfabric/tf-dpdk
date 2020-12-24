/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_ct.h"

#include <stdint.h>

#include "n3k_mgmt_init/n3k_mgmt_init_port.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_regs.h"

/**
 * Bits in FT_SCRATCH for shared communication between NIOS and FT.
 *
 * Bit values are taken from ifc/ifc_pm source code, regarding enabling
 * conntrack mode.
 */
enum N3K_MGMT_CT_BITS {
	N3K_MGMT_CT_TRACKING_MODE_BIT = 0,
	N3K_MGMT_CT_INIT_STATUS_BIT = 1,
};

enum N3K_MGMT_CT_MASKS {
	N3K_MGMT_CT_TRACKING_MODE_MASK = (1 << N3K_MGMT_CT_TRACKING_MODE_BIT),
	N3K_MGMT_CT_INIT_STATUS_MASK = (1 << N3K_MGMT_CT_INIT_STATUS_BIT),
};

static uint32_t
read_ft_scratch(struct n3k_mgmt_hw *hw)
{
	return csr_read(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE, MKS_FLOW_TRACKER_FT_SCRATCH);
}

static void
write_ft_scratch(struct n3k_mgmt_hw *hw, uint32_t val)
{
	csr_write(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE, MKS_FLOW_TRACKER_FT_SCRATCH, val);
}

void
n3k_mgmt_ct_enable_hw_tracking(struct n3k_mgmt_hw *hw)
{
	uint32_t val;

	val = read_ft_scratch(hw);
	if (!(val & N3K_MGMT_CT_TRACKING_MODE_MASK)) {
		N3K_MGMT_LOG(INFO, "FT/NIOS: HW connection tracking was on");
		return;
	}
	val = val & ~N3K_MGMT_CT_TRACKING_MODE_MASK;
	write_ft_scratch(hw, val);
	N3K_MGMT_LOG(INFO, "FT/NIOS: switched to HW connection tracking");
}

void
n3k_mgmt_ct_enable_sw_tracking(struct n3k_mgmt_hw *hw)
{
	uint32_t val;

	val = read_ft_scratch(hw);
	if (val & N3K_MGMT_CT_TRACKING_MODE_MASK) {
		N3K_MGMT_LOG(INFO, "FT/NIOS: SW connection tracking was on");
		return;
	}
	val = val | N3K_MGMT_CT_TRACKING_MODE_MASK;
	write_ft_scratch(hw, val);
	N3K_MGMT_LOG(INFO, "FT/NIOS: switched to SW connection tracking");
}
