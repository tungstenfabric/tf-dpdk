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
#include "mgmt/n3k_mgmt_tbl.h"
#include "mgmt/n3k_mgmt_tunnel_tbl.h"
#include "mgmt/n3k_mgmt_tunnel_tbl_cache.h"
#include "rte_pmd_n3k_tunnel_tbl_mgmt.h"

#include <rte_ethdev.h>
#include <stdint.h>

int rte_pmd_n3k_tunnel_tbl_cache_fetch(struct rte_eth_dev *dev,
	const struct n3k_mgmt_tunnel_tbl_entry* key,
	uint16_t* tunnel_id)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_tunnel_tbl_cache_fetch(hw, key, tunnel_id);
}

int rte_pmd_n3k_tunnel_tbl_cache_find(struct rte_eth_dev *dev,
	const struct n3k_mgmt_tunnel_tbl_entry* key,
	uint16_t* tunnel_id)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_tunnel_tbl_cache_find(hw, key, tunnel_id);
}

int rte_pmd_n3k_tunnel_tbl_cache_get_by_id(struct rte_eth_dev *dev,
	uint16_t tunnel_id, struct n3k_mgmt_tunnel_tbl_entry* out_key)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_tunnel_tbl_cache_get_by_id(hw, tunnel_id, out_key);
}

int rte_pmd_n3k_tunnel_tbl_cache_yield(struct rte_eth_dev *dev, uint16_t tunnel_id)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_tunnel_tbl_cache_yield(hw, tunnel_id);
}

int rte_pmd_n3k_tunnel_tbl_dump(struct rte_eth_dev *dev, uint16_t tunnel_idx)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_tunnel_tbl_dump(hw, tunnel_idx);
}

void rte_pmd_n3k_tunnel_tbl_entry_dump(const struct n3k_mgmt_tunnel_tbl_entry *entry)
{
	n3k_mgmt_tunnel_tbl_entry_dump(entry);
}

int rte_pmd_n3k_tunnel_get(__rte_unused struct rte_eth_dev *dev,
	__rte_unused uint16_t tunnel_idx, __rte_unused struct n3k_mgmt_tunnel_entry *tunnel)
{
	return 0;
}

void rte_pmd_n3k_tunnel_entry_dump(const struct n3k_mgmt_tunnel_entry *tunnel)
{
	n3k_mgmt_tunnel_entry_dump(tunnel);
}

int rte_pmd_n3k_tunnel_dump(struct rte_eth_dev *dev, uint16_t tunnel_idx)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_tunnel_dump(hw, tunnel_idx);
}

int rte_pmd_n3k_tunnel_add(struct rte_eth_dev *dev,
	struct n3k_mgmt_tunnel_entry *tunnel,
	uint16_t *tunnel_idx)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_tunnel_add(hw, tunnel, tunnel_idx);
}

int rte_pmd_n3k_tunnel_remove(struct rte_eth_dev *dev, uint16_t tunnel_idx,
	const struct n3k_mgmt_tunnel_entry *tunnel)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_tunnel_remove(hw, tunnel_idx, tunnel);
}
