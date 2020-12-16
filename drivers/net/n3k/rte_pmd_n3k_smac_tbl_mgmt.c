/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "mgmt/n3k_mgmt_dump.h"
#include "mgmt/n3k_mgmt_ethdev.h"
#include "mgmt/n3k_mgmt_hw.h"
#include "mgmt/n3k_mgmt_smac_tbl.h"
#include "mgmt/n3k_mgmt_smac_tbl_cache.h"
#include "rte_pmd_n3k_smac_tbl_mgmt.h"

#include <rte_ethdev.h>
#include <rte_ether.h>
#include <stdint.h>

int rte_pmd_n3k_smac_tbl_cache_fetch(struct rte_eth_dev *dev,
	const struct rte_ether_addr* key,
	uint8_t* smac_id)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_smac_tbl_cache_fetch(hw, key, smac_id);
}

int rte_pmd_n3k_smac_tbl_cache_find(struct rte_eth_dev *dev,
	const struct rte_ether_addr* key,
	uint8_t* smac_id)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_smac_tbl_cache_find(hw, key, smac_id);
}

int rte_pmd_n3k_smac_tbl_cache_get_by_id(struct rte_eth_dev *dev,
	uint8_t smac_id, struct rte_ether_addr* out_key)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_smac_tbl_cache_get_by_id(hw, smac_id, out_key);
}

int rte_pmd_n3k_smac_tbl_cache_yield(struct rte_eth_dev *dev, uint8_t smac_id)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_smac_tbl_cache_yield(hw, smac_id);
}

void rte_pmd_n3k_smac_tbl_addr_dump(const struct rte_ether_addr *addr)
{
	n3k_mgmt_smac_tbl_addr_dump(addr);
}

int rte_pmd_n3k_smac_tbl_dump(struct rte_eth_dev *dev, uint8_t port_id)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
