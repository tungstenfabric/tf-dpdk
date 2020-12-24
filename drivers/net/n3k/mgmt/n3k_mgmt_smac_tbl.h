/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_SMAC_TBL_H_
#define _PMD_NET_N3K_MGMT_SMAC_TBL_H_

#include "n3k_mgmt_hw.h"

#include <rte_ether.h>
#include <stdint.h>

int n3k_mgmt_smac_tbl_set(struct n3k_mgmt_hw *hw, uint8_t port_id,
	const struct rte_ether_addr *addr);
int n3k_mgmt_smac_tbl_get(struct n3k_mgmt_hw *hw, uint8_t port_id,
	struct rte_ether_addr *addr);
int n3k_mgmt_smac_tbl_dump(struct n3k_mgmt_hw *hw, uint8_t port_id);

#endif /* _PMD_NET_N3K_MGMT_SMAC_TBL_H_ */
