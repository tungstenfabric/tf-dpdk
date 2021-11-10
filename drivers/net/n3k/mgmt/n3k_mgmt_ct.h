/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_CT_H_
#define _PMD_NET_N3K_MGMT_CT_H_

struct n3k_mgmt_hw;

/**
 * Enable HW connection tracking in flow tracker.
 *
 * Few notes:
 *
 * - This function and it's precedessor (n3k_mgmt_init_ct_tracking_mode()) use FT_SCRATCH register
 *   to communicate with NIOS.
 * - In v01r18 chip docs this register is marked as "Test data."
 *
 * @param hw
 *    N3K hardware handle
 */
void n3k_mgmt_ct_enable_hw_tracking(struct n3k_mgmt_hw *hw);

/**
 * Enable SW (or OVS) connection tracking in flow tracker.
 *
 * @see n3k_mgmt_enable_hw_ct_tracking()
 *
 * @param hw
 *    N3K hardware handle
 */
void n3k_mgmt_ct_enable_sw_tracking(struct n3k_mgmt_hw *hw);

#endif // _PMD_NET_N3K_MGMT_CT_H_
