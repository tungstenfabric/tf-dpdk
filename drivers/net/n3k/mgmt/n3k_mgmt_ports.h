/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_PORTS_H_
#define _PMD_NET_N3K_MGMT_PORTS_H_

#include "n3k_mgmt_hw.h"

#include <rte_pmd_n3k_flow_tbl_mgmt.h>
#include <stdint.h>

#define N3K_MOCKABLE __attribute__((weak))

int n3k_mgmt_dpdk_port_to_hw_vid(
	struct n3k_mgmt_hw *hw, uint16_t dpdk_port, uint8_t *vid);
int n3k_mgmt_dpdk_port_to_hw_port_subport(
	struct n3k_mgmt_hw *hw, uint16_t dpdk_port,
	uint8_t *port, uint8_t *subport);
int n3k_mgmt_dpdk_port_to_hw_modifier_port(
	struct n3k_mgmt_hw *hw, uint16_t dpdk_port, uint8_t *mod_port);

int n3k_mgmt_hw_vid_to_dpdk_port(struct n3k_mgmt_hw *hw, uint8_t vid,
	uint16_t *dpdk_port);
int n3k_mgmt_hw_port_subport_to_dpdk_port(struct n3k_mgmt_hw *hw,
	uint8_t port, uint8_t subport, uint16_t *dpdk_port);
