/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_dump.h"
#include "n3k_mgmt_flow_tbl.h"
#include "n3k_mgmt_flow_tracker.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_smac_tbl.h"
#include "n3k_mgmt_smac_tbl_cache.h"
#include "n3k_mgmt_tbl.h"
#include "n3k_mgmt_tunnel_tbl.h"
#include "n3k_mgmt_tunnel_tbl_cache.h"
#include "n3k_mgmt_vplkp_tbl.h"
#include "n3k_mgmt_vplkp_tbl_cache.h"

int n3k_mgmt_get_key_action_tbl_entries(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_entry* flow_tbl_entry,
	struct n3k_mgmt_flow_entry *flow)
{
	int ret;
	// Get key related table entries
	if (flow_tbl_entry->key.use_tunnel_key) {
		switch (flow_tbl_entry->key.tunnel.type) {
		case N3K_MGMT_TUNNEL_TYPE_MPLSOUDP:
			ret = n3k_mgmt_vplkp_tbl_cache_get_by_label(hw,
				flow_tbl_entry->key.tunnel.mpls_udp.label,
				&flow->key.vplkp_raw);
			if (ret < 0)
				return ret;
			break;
		case N3K_MGMT_TUNNEL_TYPE_MPLSOGRE:
			// TODO(n3k): implement
			return -ENOTSUP;
		case N3K_MGMT_TUNNEL_TYPE_VXLAN:
			break;
		}
	}

	// Get action related table entries
	switch (flow_tbl_entry->action.type) {
	case N3K_MGMT_FLOW_TBL_ACTION_ENCAP:
		ret = n3k_mgmt_tunnel_get(hw,
			flow_tbl_entry->action.encap_action.tunnel_idx,
			&flow->action.tunnel);
		if (ret < 0)
			return ret;
	case N3K_MGMT_FLOW_TBL_ACTION_DECAP:
	case N3K_MGMT_FLOW_TBL_ACTION_LOCAL:
	case N3K_MGMT_FLOW_TBL_ACTION_DROP:
		break;
	}

	return 0;
}

int n3k_mgmt_flow_get(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle,
	struct n3k_mgmt_flow_entry *flow)
{
	int ret;
	struct n3k_mgmt_flow_tbl_entry flow_tbl_entry;

	ret = n3k_mgmt_flow_tbl_get(hw, handle, &flow_tbl_entry);
	if (ret < 0)
		return ret;

	ret = n3k_mgmt_get_key_action_tbl_entries(hw, &flow_tbl_entry, flow);
	if (ret < 0)
		return ret;

	memcpy(&flow->key.key_raw, &flow_tbl_entry.key, sizeof(flow_tbl_entry.key));
	memcpy(&flow->action.action_raw, &flow_tbl_entry.action, sizeof(flow_tbl_entry.action));

	return 0;
}

int n3k_mgmt_tunnel_get(struct n3k_mgmt_hw *hw,
	uint16_t tunnel_id,
	struct n3k_mgmt_tunnel_entry *tunnel)
{
	int ret;
	ret = n3k_mgmt_tunnel_tbl_cache_get_by_id(hw, tunnel_id, &tunnel->tunnel_raw);
	if (ret < 0)
		return ret;
	ret = n3k_mgmt_smac_tbl_cache_get_by_id(hw, tunnel->tunnel_raw.l2.src_phy_port_id,
		&tunnel->smac_raw);
	if (ret < 0)
		return ret;
	return 0;
}

int n3k_mgmt_key_entries_remove(struct n3k_mgmt_hw *hw, const struct n3k_mgmt_flow_key *key)
{
	int ret;
	if (key->key_raw.use_tunnel_key) {
		switch (key->key_raw.tunnel.type) {
		case N3K_MGMT_TUNNEL_TYPE_MPLSOUDP:
			ret = n3k_mgmt_vplkp_tbl_cache_yield(hw,
				key->key_raw.tunnel.mpls_udp.label);
			if (ret < 0) {
				N3K_MGMT_LOG(ERR,
					"Failed to remove vplkp tbl entry: ret=%d",
					ret);
				return ret;
			}
			break;
		case N3K_MGMT_TUNNEL_TYPE_MPLSOGRE:
			// TODO(n3k): implement
		case N3K_MGMT_TUNNEL_TYPE_VXLAN:;
			// TODO(n3k): implement
			return -ENOTSUP;
		}
	}
	return 0;
}

int n3k_mgmt_action_entries_remove(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flow_action *action)
{
	int ret;
	switch (action->action_raw.type) {
	case N3K_MGMT_FLOW_TBL_ACTION_ENCAP:
		ret = n3k_mgmt_tunnel_remove(hw,
			action->action_raw.encap_action.tunnel_idx,
			&action->tunnel);
		if (ret < 0) {
			N3K_MGMT_LOG(ERR,
				"Failed to remove tunnel entry: ret=%d",
				ret);
			return ret;
		}
		break;
	case N3K_MGMT_FLOW_TBL_ACTION_DECAP:
	case N3K_MGMT_FLOW_TBL_ACTION_LOCAL:
	case N3K_MGMT_FLOW_TBL_ACTION_DROP:
		break;
	}
	return 0;
}

int n3k_mgmt_key_entries_add(struct n3k_mgmt_hw *hw, struct n3k_mgmt_flow_key *key)
{
	int ret;
	if (key->key_raw.use_tunnel_key) {
		switch (key->key_raw.tunnel.type) {
		case N3K_MGMT_TUNNEL_TYPE_MPLSOUDP:
			ret = n3k_mgmt_vplkp_tbl_cache_insert(hw,
				key->key_raw.tunnel.mpls_udp.label,
				&key->vplkp_raw);
			if (ret < 0) {
				N3K_MGMT_LOG(ERR,
					"Failed to add vplkp tbl entry: ret=%d",
					ret);
				return ret;
			}
			break;
		case N3K_MGMT_TUNNEL_TYPE_MPLSOGRE:
			// TODO(n3k): implement
		case N3K_MGMT_TUNNEL_TYPE_VXLAN:
			break;
		}
	}
	return 0;
}

int n3k_mgmt_action_entries_add(struct n3k_mgmt_hw *hw, struct n3k_mgmt_flow_action *action)
{
	int ret;
	switch (action->action_raw.type) {
	case N3K_MGMT_FLOW_TBL_ACTION_ENCAP: {
		uint16_t tunnel_id;
		ret = n3k_mgmt_tunnel_add(hw, &action->tunnel, &tunnel_id);
		if (ret < 0) {
			N3K_MGMT_LOG(ERR, "Failed to add tunnel entry: ret=%d", ret);
			return ret;
		}
		action->action_raw.encap_action.tunnel_idx = tunnel_id;
		break;
	}
	case N3K_MGMT_FLOW_TBL_ACTION_DECAP:
	case N3K_MGMT_FLOW_TBL_ACTION_LOCAL:
	case N3K_MGMT_FLOW_TBL_ACTION_DROP:
		break;
	}
	return 0;
}

int n3k_mgmt_flow_add(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_entry *flow,
	struct n3k_mgmt_flow_tbl_handle **handle)
{
	int ret;

	ret = n3k_mgmt_action_entries_add(hw, &flow->action);
	if (ret < 0)
		return ret;
	ret = n3k_mgmt_key_entries_add(hw, &flow->key);
	if (ret < 0)
		goto remove_action;

	const struct n3k_mgmt_flow_tbl_entry flow_tbl_entry = {
		.key = flow->key.key_raw,
		.action = flow->action.action_raw,
	};
	ret = n3k_mgmt_flow_tbl_add(hw, &flow_tbl_entry, handle);
	if (ret) {
		N3K_MGMT_LOG(ERR, "Failed to add flow tbl entry: ret=%d", ret);
		goto remove_key;
	}

	return 0;

remove_key:
	{
		int remove_ret = n3k_mgmt_key_entries_remove(hw, &flow->key);
		if (remove_ret < 0) {
			N3K_MGMT_LOG(ERR,
				"Failed to remove key related table entries: ret=%d",
				remove_ret);
		}
	}
remove_action:
	{
		int remove_ret = n3k_mgmt_action_entries_remove(hw, &flow->action);
		if (remove_ret < 0) {
			N3K_MGMT_LOG(ERR,
				"Failed to remove action related table entries: ret=%d",
				remove_ret);
		}
	}
	return ret;
}

int n3k_mgmt_tunnel_add(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_tunnel_entry *tunnel,
	uint16_t *tunnel_id)
{
	uint8_t smac_id;
	int ret;

	ret = n3k_mgmt_smac_tbl_cache_fetch(hw, &tunnel->smac_raw, &smac_id);
	if (ret < 0) {
		N3K_MGMT_LOG(ERR,
			"Failed to add smac tbl entry: ret=%d",
			ret);
		return ret;
	}
	tunnel->tunnel_raw.l2.src_phy_port_id = smac_id;

	ret = n3k_mgmt_tunnel_tbl_cache_fetch(hw, &tunnel->tunnel_raw, tunnel_id);
	if (ret < 0) {
		N3K_MGMT_LOG(ERR,
			"Failed to add tunnel tbl entry: ret=%d",
			ret);
		goto remove_smac;
	}

	return 0;

remove_smac:
	{
		int remove_ret = n3k_mgmt_smac_tbl_cache_yield(hw, smac_id);
		if (remove_ret < 0) {
			N3K_MGMT_LOG(ERR,
				"Failed to remove smac entry: ret=%d",
				remove_ret);
		}
	}
	return ret;
}

void n3k_mgmt_flow_entry_to_description(struct n3k_mgmt_flow_tbl_handle *handle,
	const struct n3k_mgmt_flow_entry *flow,
	struct n3k_mgmt_flow_entry_description *desc)
{
	memset(desc, 0, sizeof(*desc));
	desc->flow_handle = handle;

	if (flow->key.key_raw.use_tunnel_key &&
			flow->key.key_raw.tunnel.type == N3K_MGMT_TUNNEL_TYPE_MPLSOUDP) {
		desc->use_key_desc = true;
		desc->key = (struct n3k_mgmt_flow_key_description) {
			.vplkp_id = flow->key.key_raw.tunnel.mpls_udp.label,
		};
	}

	if (flow->action.action_raw.type == N3K_MGMT_FLOW_TBL_ACTION_ENCAP) {
		desc->use_action_desc = true;
		desc->action = (struct n3k_mgmt_flow_action_description) {
			.tunnel_id = flow->action.action_raw.encap_action.tunnel_idx,
			.smac_id = flow->action.tunnel.tunnel_raw.l2.src_phy_port_id,
		};
	}
}

void n3k_mgmt_set_description_handle(struct n3k_mgmt_flow_tbl_handle *handle,
	struct n3k_mgmt_flow_entry_description *desc)
{
	desc->flow_handle = handle;
}

int n3k_mgmt_flow_remove(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle,
	const struct n3k_mgmt_flow_entry *flow)
{
	struct n3k_mgmt_flow_entry_description desc;
	n3k_mgmt_flow_entry_to_description(handle, flow, &desc);
	return n3k_mgmt_flow_entry_description_remove(hw, &desc);
}

int n3k_mgmt_tunnel_remove(struct n3k_mgmt_hw *hw, uint16_t tunnel_id,
	const struct n3k_mgmt_tunnel_entry *tunnel)
{
	struct n3k_mgmt_flow_action_description desc = {
		.tunnel_id = tunnel_id,
		.smac_id = tunnel->tunnel_raw.l2.src_phy_port_id,
	};
	return n3k_mgmt_flow_action_description_remove(hw, &desc);
}

int n3k_mgmt_flow_entry_description_remove(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_entry_description *desc)
{
	int ret;

	ret = n3k_mgmt_flow_tbl_del(hw, desc->flow_handle);
	if (ret < 0) {
		N3K_MGMT_LOG(ERR, "Failed to remove flow table entry: ret=%d",
			ret);
	}

	if (desc->use_action_desc) {
		ret = n3k_mgmt_flow_action_description_remove(hw, &desc->action);
		if (ret < 0) {
			N3K_MGMT_LOG(ERR,
				"Failed to remove action related table entries: ret=%d",
				ret);
			return ret;
		}
	}

	if (desc->use_key_desc) {
		ret = n3k_mgmt_flow_key_description_remove(hw, &desc->key);
		if (ret < 0) {
			N3K_MGMT_LOG(ERR,
				"Failed to remove key related table entries: ret=%d",
				ret);
			return ret;
		}
	}

	return ret;
}

int n3k_mgmt_flow_key_description_remove(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flow_key_description *desc)
{
	int ret;
	ret = n3k_mgmt_vplkp_tbl_cache_yield(hw, desc->vplkp_id);
	if (ret < 0) {
		N3K_MGMT_LOG(ERR, "Failed to remove vplkp tbl entry: ret=%d", ret);
		return ret;
	}
	return 0;
}

int n3k_mgmt_flow_action_description_remove(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flow_action_description *desc)
{
	int ret;

	ret = n3k_mgmt_tunnel_tbl_cache_yield(hw, desc->tunnel_id);
	if (ret < 0) {
		N3K_MGMT_LOG(ERR, "Failed to remove tunnel entry: ret=%d", ret);
		return ret;
	}

	ret = n3k_mgmt_smac_tbl_cache_yield(hw, desc->smac_id);
	if (ret < 0) {
		N3K_MGMT_LOG(ERR, "Failed to remove smac entry: ret=%d", ret);
		return ret;
	}

	return 0;
}

int n3k_mgmt_flow_dump(struct n3k_mgmt_hw *hw, struct n3k_mgmt_flow_tbl_handle *handle)
{
	struct n3k_mgmt_flow_entry flow;
	int ret;

	ret = n3k_mgmt_flow_get(hw, handle, &flow);
	if (ret < 0)
		return ret;

	n3k_mgmt_flow_entry_dump(&flow);
	return 0;
}

int n3k_mgmt_tunnel_dump(struct n3k_mgmt_hw *hw, uint16_t tunnel_id)
{
	struct n3k_mgmt_tunnel_entry tunnel;
	int ret;

	ret = n3k_mgmt_tunnel_get(hw, tunnel_id, &tunnel);
	if (ret < 0)
		return ret;

	n3k_mgmt_tunnel_entry_dump(&tunnel);
	return 0;
}
