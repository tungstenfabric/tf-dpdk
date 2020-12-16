/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_mp_callbacks.h"

#include "n3k_mgmt_ethdev.h"
#include "n3k_mgmt_flow_tbl_handles.h"
#include "n3k_mgmt_flow_tbl.h"
#include "n3k_mgmt_flow_tracker.h"
#include "n3k_mgmt_flr_flow_tbl.h"
#include "n3k_mgmt_flr_flow_tracker.h"
#include "n3k_mgmt_flr_pkts_blocks.h"
#include "n3k_mgmt_flr_read_reg_32.h"
#include "n3k_mgmt_flr_tbl.h"
#include "n3k_mgmt_general_flow_stats.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_port_stats.h"
#include "n3k_mgmt_smac_tbl_cache.h"
#include "n3k_mgmt_tbl.h"
#include "n3k_mgmt_tunnel_tbl_cache.h"
#include "n3k_mgmt_vplkp_tbl_cache.h"

#include "ifc/ifc_port.h"

#include <rte_pmd_n3k_mp.h>

#include <stdint.h>

static int
n3k_mgmt_mp_increment_port_iterator(struct n3k_mgmt_hw *hw,
	struct n3k_vdev_dev_id *iterator)
{
	while (iterator->type != N3K_VDEV_DEV_TYPE_VF ||
		iterator->id + 1u < RTE_DIM(hw->vf_representors))
	{
		struct rte_eth_dev *dev;
		__rte_unused int ret;
		if (iterator->type == N3K_VDEV_DEV_TYPE_PHY &&
				iterator->id + 1u >= RTE_DIM(hw->phy_representors)) {
			iterator->type = N3K_VDEV_DEV_TYPE_VF;
			iterator->id = 0;
		} else {
			++iterator->id;
		}

		ret = n3k_mgmt_get_device_by_id(hw, iterator, &dev);
		RTE_ASSERT(ret == 0);
		if (dev)
			return 0;
	}
	return -ENOENT;
}

static int
n3k_mgmt_mp_port_init_iterator(struct n3k_mgmt_hw *hw, struct n3k_vdev_dev_id *iterator)
{
	struct rte_eth_dev *dev;
	iterator->id = 0;
	iterator->type = N3K_VDEV_DEV_TYPE_PHY;
	if (n3k_mgmt_get_device_by_id(hw, iterator, &dev) || !dev)
		return n3k_mgmt_mp_increment_port_iterator(hw, iterator);
	return 0;
}

static int
n3k_mgmt_mp_increment_flow_tbl_iterator(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle **flow_tbl_handle)
{
	int ret;

	do {
		ret = n3k_mgmt_flow_tbl_next_handle(hw, flow_tbl_handle);
		if(ret < 0)
			return ret;
	} while ((*flow_tbl_handle)->is_mirror_flow);

	return ret;
}

static int
n3k_mgmt_mp_init_flow_tbl_iterator(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle **flow_tbl_handle)
{
	int ret = n3k_mgmt_flow_tbl_first_handle(hw, flow_tbl_handle);
	if (ret < 0)
		return ret;

	if ((*flow_tbl_handle)->is_mirror_flow)
		return n3k_mgmt_mp_increment_flow_tbl_iterator(hw, flow_tbl_handle);

	return ret;
}

int
n3k_mgmt_mp_init_iterator(struct n3k_mgmt_hw *hw,
	enum n3k_mgmt_mp_request_table type,
	struct n3k_mgmt_mp_table_iterator *iterator)
{
	iterator->type = type;
	switch (type) {
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW:
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW_RAW:
		return n3k_mgmt_mp_init_flow_tbl_iterator(hw, &iterator->flow_tbl_iterator);
	case N3K_MGMT_MP_REQUEST_TABLE_TUNNEL:
	case N3K_MGMT_MP_REQUEST_TABLE_TUNNEL_RAW:
		return n3k_mgmt_tunnel_tbl_cache_first_index(hw, &iterator->tunnel_tbl_iterator);
	case N3K_MGMT_MP_REQUEST_TABLE_SMAC:
		return n3k_mgmt_smac_tbl_cache_first_index(hw, &iterator->smac_tbl_iterator);
	case N3K_MGMT_MP_REQUEST_TABLE_VPLKP:
		return n3k_mgmt_vplkp_tbl_cache_init_iter(hw, &iterator->vplkp_tbl_iterator);
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW_TRACKER:
		return n3k_mgmt_mp_init_flow_tbl_iterator(hw, &iterator->flow_tbl_iterator);
	case N3K_MGMT_MP_REQUEST_TABLE_PORT:
		return n3k_mgmt_mp_port_init_iterator(hw, &iterator->port_iterator);
	}
	N3K_MGMT_MP_LOG(ERR, "Invalid request type %d", type);
	return -EINVAL;
}

int
n3k_mgmt_mp_increment_iterator(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_mp_table_iterator *iterator)
{
	switch (iterator->type) {
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW:
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW_RAW:
		return n3k_mgmt_mp_increment_flow_tbl_iterator(hw, &iterator->flow_tbl_iterator);
	case N3K_MGMT_MP_REQUEST_TABLE_TUNNEL:
	case N3K_MGMT_MP_REQUEST_TABLE_TUNNEL_RAW:
		return n3k_mgmt_tunnel_tbl_cache_next_index(hw, &iterator->tunnel_tbl_iterator);
	case N3K_MGMT_MP_REQUEST_TABLE_SMAC:
		return n3k_mgmt_smac_tbl_cache_next_index(hw, &iterator->smac_tbl_iterator);
	case N3K_MGMT_MP_REQUEST_TABLE_VPLKP:
		return n3k_mgmt_vplkp_tbl_cache_incr_iter(hw, &iterator->vplkp_tbl_iterator);
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW_TRACKER:
		return n3k_mgmt_mp_increment_flow_tbl_iterator(hw, &iterator->flow_tbl_iterator);
	case N3K_MGMT_MP_REQUEST_TABLE_PORT:
		return n3k_mgmt_mp_increment_port_iterator(hw, &iterator->port_iterator);
	}
	N3K_MGMT_MP_LOG(ERR, "Invalid request type %d", iterator->type);
	return -EINVAL;
}

static int
n3k_mgmt_mp_flow_entries_response_filler(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *iterator,
	struct n3k_mgmt_mp_flow_response *response)
{
	int ret;
	response->flow_id = iterator->flow_id;
	ret = n3k_mgmt_flow_get(hw, iterator, &response->flow);
	if (ret < 0)
		return ret;
	return n3k_mgmt_flow_tracker_get(hw, iterator, &response->ft);
}

static int
n3k_mgmt_mp_port_response_filler(struct n3k_mgmt_hw *hw,
	struct n3k_vdev_dev_id *iterator,
	struct n3k_mgmt_mp_port_response *response)
{
	struct rte_eth_dev *dev;
	int ret;

	ret = n3k_mgmt_get_device_by_id(hw, iterator, &dev);
	RTE_ASSERT(ret == 0);

	ret = n3k_mgmt_get_vdev_stats(dev, &response->stats);
	if (ret < 0)
		return ret;

	RTE_BUILD_BUG_ON(sizeof(dev->data->name) != sizeof(response->port_name));
	memcpy(response->port_name, dev->data->name, sizeof(dev->data->name));

	return ret;
}

static int
full_flow_dump(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *flow_handle,
	struct n3k_mgmt_mp_flow_response *flow)
{
	int ret;
	flow->flow_id = flow_handle->flow_id;
	ret = n3k_mgmt_flr_flow_tracker_get_sync(hw, flow_handle,
		&flow->ft);
	if (ret < 0) {
		N3K_MGMT_MP_LOG(ERR, "error while getting ft stats %d", ret);
		return ret;
	}
	struct n3k_mgmt_flow_tbl_entry tbl_entry;
	ret = n3k_mgmt_flr_flow_sync_dump(hw,
		flow_handle, &tbl_entry);
	if (ret < 0) {
		N3K_MGMT_MP_LOG(ERR, "error while getting flow tbl entry %d", ret);
		return ret;
	}

	ret = n3k_mgmt_get_key_action_tbl_entries(hw, &tbl_entry, &flow->flow);
	if (ret < 0) {
		N3K_MGMT_MP_LOG(ERR, "error while getting key and action tbl entries %d", ret);
		return ret;
	}

	memcpy(&flow->flow.key.key_raw, &tbl_entry.key, sizeof(tbl_entry.key));
	memcpy(&flow->flow.action.action_raw, &tbl_entry.action, sizeof(tbl_entry.action));
	return 0;
}

int
n3k_mgmt_mp_fill_response(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_mp_table_iterator *iterator,
	struct n3k_mgmt_mp_entries_response *response)
{
	if (hw->insert_type == N3K_MGMT_HW_INSERT_TYPE_CSR)
		return n3k_mgmt_mp_fill_entries_response_csr(hw, iterator, response);
	switch (iterator->type) {
		case N3K_MGMT_MP_REQUEST_TABLE_FLOW_TRACKER:
			response->flow_tracker.flow_id = iterator->flow_tbl_iterator->flow_id;
			return n3k_mgmt_flr_flow_tracker_get_sync(hw,
				iterator->flow_tbl_iterator,
				&response->flow_tracker.ft);
		case N3K_MGMT_MP_REQUEST_TABLE_FLOW:
			return full_flow_dump(hw, iterator->flow_tbl_iterator, &response->flow);
		case N3K_MGMT_MP_REQUEST_TABLE_FLOW_RAW:
			response->flow_raw.flow_id = iterator->flow_tbl_iterator->flow_id;
			return n3k_mgmt_flr_flow_sync_dump(hw,
				iterator->flow_tbl_iterator, &response->flow_raw.flow);
		default:
			return n3k_mgmt_mp_fill_entries_response_csr(hw, iterator, response);
	}
}

int
n3k_mgmt_mp_fill_entries_response_csr(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_mp_table_iterator *iterator,
	struct n3k_mgmt_mp_entries_response *response)
{
	switch (iterator->type) {
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW:
		return n3k_mgmt_mp_flow_entries_response_filler(hw,
			iterator->flow_tbl_iterator, &response->flow);
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW_RAW:
		response->flow_raw.flow_id = iterator->flow_tbl_iterator->flow_id;
		return n3k_mgmt_flow_tbl_get(hw, iterator->flow_tbl_iterator,
			&response->flow_raw.flow);
	case N3K_MGMT_MP_REQUEST_TABLE_TUNNEL:
		response->tunnel.tunnel_id = iterator->tunnel_tbl_iterator;
		response->tunnel.ref_count =
			n3k_mgmt_tunnel_tbl_cache_get_ref_count(hw, response->tunnel.tunnel_id);
		return n3k_mgmt_tunnel_get(hw, iterator->tunnel_tbl_iterator,
			&response->tunnel.tunnel);
	case N3K_MGMT_MP_REQUEST_TABLE_TUNNEL_RAW:
		response->tunnel_raw.tunnel_id = iterator->tunnel_tbl_iterator;
		response->tunnel_raw.ref_count =
			n3k_mgmt_tunnel_tbl_cache_get_ref_count(hw, response->tunnel_raw.tunnel_id);
		return n3k_mgmt_tunnel_tbl_cache_get_by_id(hw,
			iterator->tunnel_tbl_iterator,
			&response->tunnel_raw.tunnel);
	case N3K_MGMT_MP_REQUEST_TABLE_SMAC:
		response->smac.port_id = iterator->smac_tbl_iterator;
		response->smac.ref_count =
			n3k_mgmt_smac_tbl_cache_get_ref_count(hw, response->smac.port_id);
		return n3k_mgmt_smac_tbl_cache_get_by_id(hw,
			iterator->smac_tbl_iterator,
			&response->smac.smac);
	case N3K_MGMT_MP_REQUEST_TABLE_VPLKP:
		response->vplkp.label = iterator->vplkp_tbl_iterator.mpls_label;
		response->vplkp.ref_count =
			n3k_mgmt_vplkp_tbl_cache_get_ref_count(hw, response->vplkp.label);
		return n3k_mgmt_vplkp_tbl_cache_get_by_label(hw,
			response->vplkp.label, &response->vplkp.res);
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW_TRACKER:
		response->flow_tracker.flow_id = iterator->flow_tbl_iterator->flow_id;
		return n3k_mgmt_flow_tracker_get(hw,
			iterator->flow_tbl_iterator,
			&response->flow_tracker.ft);
	case N3K_MGMT_MP_REQUEST_TABLE_PORT:
		return n3k_mgmt_mp_port_response_filler(hw,
			&iterator->port_iterator, &response->port);
	}
	N3K_MGMT_MP_LOG(ERR, "Invalid request type %d", iterator->type);
	return -EINVAL;
}

static int
read_total_entries_flr(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_mp_total_entries_response *response)
{
	uint32_t val;
	int ret;
	ret = n3k_mgmt_flr_sync_read_reg_32(hw, MKS_CHIP_LKUP_BASE,
		LKUP_EMLC_TOTAL_ENTRIES, &val);
	if (ret) {
		N3K_MGMT_MP_LOG(ERR, "sync read reg failed %d", ret);
		return ret;
	}
	response->total_entries = val;
	return 0;
}

static int
n3k_mgmt_mp_flow_total_entries(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_mp_total_entries_response *response)
{
	switch (hw->insert_type) {
		case N3K_MGMT_HW_INSERT_TYPE_FLR_SIM:
		case N3K_MGMT_HW_INSERT_TYPE_FLR:
			return read_total_entries_flr(hw, response);
		case N3K_MGMT_HW_INSERT_TYPE_CSR:
		{
			struct n3k_mgmt_general_flow_stats stats;
			stats = n3k_mgmt_general_flow_stats_get(hw);
			response->total_entries = stats.emlc_total_entries;
			break;
		}
	}
	return 0;
}

static int
n3k_mgmt_mp_port_total_entries(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_mp_total_entries_response *response)
{
	uint32_t i;
	response->total_entries = 0;
	for (i = 0; i < RTE_DIM(hw->vf_representors); ++i)
		if (hw->vf_representors[i])
			++response->total_entries;
	for (i = 0; i < RTE_DIM(hw->phy_representors); ++i)
		if (hw->phy_representors[i])
			++response->total_entries;
	return 0;
}

int
n3k_mgmt_mp_fill_total_entries_response(struct n3k_mgmt_hw *hw,
	enum n3k_mgmt_mp_request_table type,
	struct n3k_mgmt_mp_total_entries_response *response)
{
	switch (type) {
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW_TRACKER:
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW:
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW_RAW:
		// TODO(n3k): Should we extract that info from registers or from cache?
		//            Card can remove flows by itself so those values may differ.
		return n3k_mgmt_mp_flow_total_entries(hw, response);
	case N3K_MGMT_MP_REQUEST_TABLE_TUNNEL:
	case N3K_MGMT_MP_REQUEST_TABLE_TUNNEL_RAW:
		response->total_entries = n3k_mgmt_tunnel_tbl_cache_total_entries(hw);
		return 0;
	case N3K_MGMT_MP_REQUEST_TABLE_SMAC:
		response->total_entries = n3k_mgmt_smac_tbl_cache_total_entries(hw);
		return 0;
	case N3K_MGMT_MP_REQUEST_TABLE_VPLKP:
		response->total_entries = n3k_mgmt_vplkp_tbl_cache_total_entries(hw);
		return 0;
	case N3K_MGMT_MP_REQUEST_TABLE_PORT:
		return n3k_mgmt_mp_port_total_entries(hw, response);
	}
	N3K_MGMT_MP_LOG(ERR, "Invalid request type %d", type);
	return -EINVAL;
}

int
n3k_mgmt_mp_fill_register_response(
	struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_mp_register_request *request,
	uint32_t *value)
{
	int ret;
	switch (hw->insert_type) {
		case N3K_MGMT_HW_INSERT_TYPE_FLR:
		case N3K_MGMT_HW_INSERT_TYPE_FLR_SIM:
			if (request->base != MKS_CHIP_LKUP_BASE &&
				request->base != MKS_CHIP_MKS_FLOW_TRACKER_BASE)
				return *value = csr_read(hw, request->base, request->offset), 0;
			ret = n3k_mgmt_flr_sync_read_reg_32(hw, request->base,
				request->offset, value);
			if (ret) {
				N3K_MGMT_MP_LOG(ERR, "sync read reg failed %d",
					ret);
			}
			return ret;
		case N3K_MGMT_HW_INSERT_TYPE_CSR:
			*value = csr_read(hw, request->base, request->offset);
