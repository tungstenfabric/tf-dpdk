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
#include "mgmt/n3k_mgmt_vplkp_tbl.h"
#include "mgmt/n3k_mgmt_vplkp_tbl_cache.h"
#include "rte_pmd_n3k_vplkp_tbl_mgmt.h"

#include <rte_ethdev.h>
#include <stdint.h>


int rte_pmd_n3k_vplkp_tbl_cache_insert(struct rte_eth_dev *dev,
	uint32_t mpls_label, const struct n3k_mgmt_vplkp_tbl_res *res)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_vplkp_tbl_cache_insert(hw, mpls_label, res);
}

int rte_pmd_n3k_vplkp_tbl_cache_get_by_label(struct rte_eth_dev *dev,
	uint32_t mpls_label, struct n3k_mgmt_vplkp_tbl_res *res)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_vplkp_tbl_cache_get_by_label(hw, mpls_label, res);
}

uint32_t rte_pmd_n3k_vplkp_tbl_cache_get_ref_count(struct rte_eth_dev *dev,
	uint32_t mpls_label)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_vplkp_tbl_cache_get_ref_count(hw, mpls_label);
}

int rte_pmd_n3k_vplkp_tbl_cache_yield(struct rte_eth_dev *dev, uint32_t mpls_label)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_vplkp_tbl_cache_yield(hw, mpls_label);
}

void rte_pmd_n3k_vplkp_tbl_res_dump(const struct n3k_mgmt_vplkp_tbl_res *res)
{
	n3k_mgmt_vplkp_tbl_res_dump(res);
}

int rte_pmd_n3k_vplkp_tbl_dump(struct rte_eth_dev *dev, uint32_t key)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
