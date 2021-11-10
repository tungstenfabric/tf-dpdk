/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "mgmt/n3k_mgmt_ethdev.h"
#include "mgmt/n3k_mgmt_general_flow_stats.h"
#include "mgmt/n3k_mgmt_flow_tbl.h"
#include "mgmt/n3k_mgmt_dump.h"
#include "mgmt/n3k_mgmt_flow_tracker.h"
#include "mgmt/n3k_mgmt_hw.h"
#include "mgmt/n3k_mgmt_tbl.h"
#include "rte_pmd_n3k_flow_tbl_mgmt.h"

#include <rte_ethdev.h>
#include <stdint.h>

int rte_pmd_n3k_flow_tbl_add(struct rte_eth_dev *dev,
	const struct n3k_mgmt_flow_tbl_entry *entry,
	struct n3k_mgmt_flow_tbl_handle **handle)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_flow_tbl_add(hw, entry, handle);
}

int rte_pmd_n3k_flow_tbl_get(struct rte_eth_dev *dev,
	struct n3k_mgmt_flow_tbl_handle *handle,
	struct n3k_mgmt_flow_tbl_entry *entry)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_flow_tbl_get(hw, handle, entry);
}

int rte_pmd_n3k_flow_tbl_del(struct rte_eth_dev *dev,
	struct n3k_mgmt_flow_tbl_handle *handle)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_flow_tbl_del(hw, handle);
}

void rte_pmd_n3k_flow_tbl_action_dump(const struct n3k_mgmt_flow_tbl_action *action)
{
	n3k_mgmt_flow_tbl_action_dump(action);
}

void rte_pmd_n3k_flow_tbl_key_dump(const struct n3k_mgmt_flow_tbl_key *key)
{
	n3k_mgmt_flow_tbl_key_dump(key);
}

void rte_pmd_n3k_flow_tbl_entry_dump(const struct n3k_mgmt_flow_tbl_entry *entry)
{
	n3k_mgmt_flow_tbl_entry_dump(entry);
}

int rte_pmd_n3k_flow_tbl_dump(struct rte_eth_dev *dev,
	struct n3k_mgmt_flow_tbl_handle *handle)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_flow_tbl_dump(hw, handle);
}

int rte_pmd_n3k_general_flow_stats_get(struct rte_eth_dev *dev,
	struct n3k_mgmt_general_flow_stats *stats)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	*stats = n3k_mgmt_general_flow_stats_get(hw);
	return 0;
}

int rte_pmd_n3k_general_flow_stats_dump(struct rte_eth_dev *dev)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	n3k_mgmt_general_flow_stats_dump(hw);
	return 0;
}

int rte_pmd_n3k_flow_tracker_get(struct rte_eth_dev *dev,
	struct n3k_mgmt_flow_tbl_handle *handle,
	struct n3k_mgmt_flow_tracker_stats *ft_stats)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_flow_tracker_get(hw, handle, ft_stats);
}

int rte_pmd_n3k_flow_tracker_dump(struct rte_eth_dev *dev,
	struct n3k_mgmt_flow_tbl_handle *handle)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_flow_tracker_dump(hw, handle);
}

void rte_pmd_n3k_flow_tracker_struct_dump(const struct n3k_mgmt_flow_tracker_stats *ft_stats)
{
	n3k_mgmt_flow_tracker_struct_dump(ft_stats);
}

int rte_pmd_n3k_flow_get(struct rte_eth_dev *dev,
	struct n3k_mgmt_flow_tbl_handle *handle,
	struct n3k_mgmt_flow_entry *flow)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_flow_get(hw, handle, flow);
}

void rte_pmd_n3k_flow_action_dump(const struct n3k_mgmt_flow_action *action)
{
	return n3k_mgmt_flow_action_dump(action);
}

void rte_pmd_n3k_flow_key_dump(const struct n3k_mgmt_flow_key *key)
{
	return n3k_mgmt_flow_key_dump(key);
}

void rte_pmd_n3k_flow_entry_dump(const struct n3k_mgmt_flow_entry *entry)
{
	return n3k_mgmt_flow_entry_dump(entry);
}

int rte_pmd_n3k_flow_dump(struct rte_eth_dev *dev, struct n3k_mgmt_flow_tbl_handle *handle)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_flow_dump(hw, handle);
}

int rte_pmd_n3k_flow_add(struct rte_eth_dev *dev, struct n3k_mgmt_flow_entry *flow,
	struct n3k_mgmt_flow_tbl_handle **handle)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_flow_add(hw, flow, handle);
}

int rte_pmd_n3k_flow_remove(struct rte_eth_dev *dev,
	struct n3k_mgmt_flow_tbl_handle *handle,
	const struct n3k_mgmt_flow_entry *flow)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;
	return n3k_mgmt_flow_remove(hw, handle, flow);
}
