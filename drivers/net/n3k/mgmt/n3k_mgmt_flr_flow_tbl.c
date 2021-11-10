/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_bitwise.h"
#include "n3k_mgmt_dump.h"
#include "n3k_mgmt_flow_id_allocator.h"
#include "n3k_mgmt_flow_tbl.h"
#include "n3k_mgmt_flow_tbl_handles.h"
#include "n3k_mgmt_flow_tracker.h"
#include "n3k_mgmt_flr_cmd_queue.h"
#include "n3k_mgmt_flr_flow_tbl.h"
#include "n3k_mgmt_flr_pkts.h"
#include "n3k_mgmt_flr_pkts.h"
#include "n3k_mgmt_flr_pkts_blocks.h"
#include "n3k_mgmt_handle_mapping.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_recirc_id_allocator.h"
#include "n3k_mgmt_tbl.h"
#include "n3k_mgmt_regs_ctrl.h"
#include "regs/n3k_mgmt_flow_tbl_regs_api.h"
#include "regs/n3k_mgmt_regs.h"

#include <rte_pmd_n3k_flow_tbl_mgmt.h>
#include <stdint.h>

////////// FLOW TBL ADD

static int
flr_flow_tbl_init_mirror_flow(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_create *flow_create_cmd)
{
	int ret;
	uint32_t mirror_flow_id;
	uint32_t recirc_id;

	ret = n3k_mgmt_flow_id_alloc(hw, &mirror_flow_id);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR, "n3k_mgmt_flr_flow_get_flow_id failed: ret=%d", ret);
		return ret;
	}

	ret = n3k_mgmt_recirc_id_alloc(hw, &recirc_id);

	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR, "Failed to allocate recirc id");
		goto free_flow_id;
	}

	ret = n3k_mgmt_flow_tbl_init_mirror_flow_handle(hw, &flow_create_cmd->mirror_handle,
		recirc_id, mirror_flow_id);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR, "Failed to create flow handle");
		goto free_recirc_id;
	}

	ret = n3k_mgmt_flow_tbl_change_flow_status(hw, flow_create_cmd->mirror_handle,
		N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETED,
		N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDING);
	RTE_ASSERT(ret == 0);

	flow_create_cmd->insert_mirror_flow = true;
	flow_create_cmd->handle->recirc_id = flow_create_cmd->mirror_handle->recirc_id;
	flow_create_cmd->handle->mirror_flow = flow_create_cmd->mirror_handle;

	return 0;

free_recirc_id:
	n3k_mgmt_recirc_id_free(hw, recirc_id);

free_flow_id:
	{
		__rte_unused int ret_free = n3k_mgmt_flow_id_free(hw, mirror_flow_id);
		RTE_ASSERT(ret_free == 0);
	}
	return ret;
}

static void
free_mirror_data(struct n3k_mgmt_hw *hw, uint32_t flow_id, uint32_t recirc_id)
{
	__rte_unused int ret;
	ret = n3k_mgmt_flow_id_free(hw, flow_id);
	RTE_ASSERT(ret == 0);
	ret = n3k_mgmt_recirc_id_free(hw, recirc_id);
	RTE_ASSERT(ret == 0);
}


int
n3k_mgmt_flr_flow_tbl_schedule_add(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_create *flow_create_cmd,
	struct n3k_mgmt_flow_tbl_handle **handle)
{
	uint32_t flow_id;
	bool mirror = flow_create_cmd->flow.action.action_raw.use_mirroring;
	int ret = 0;

	ret = n3k_mgmt_flow_id_alloc(hw, &flow_id);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR, "n3k_mgmt_flr_flow_get_flow_id failed: ret=%d", ret);
		return ret;
	}

	ret = n3k_mgmt_flow_tbl_init_flow_handle(hw, &flow_create_cmd->handle,
		&flow_create_cmd->flow.action.action_raw, flow_id);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR, "Failed to init flow handle");
		goto free_flow_id;
	}
	*handle = flow_create_cmd->handle;

	if (mirror) {
		ret = flr_flow_tbl_init_mirror_flow(hw, flow_create_cmd);
		if (ret < 0) {
			N3K_MGMT_LOG(FLOW, DEBUG, "n3k_mgmt_flr_flow_tbl_init_mirror_flow failed ret=%d", ret);
			goto free_flow_id;
		}
	}

	ret = n3k_mgmt_flow_tbl_change_flow_status(hw, flow_create_cmd->handle,
		N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETED,
		N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDING);
	RTE_ASSERT(ret == 0);

	if (N3K_MGMT_SHOULD_LOG(FLOW, DEBUG)) {
		N3K_MGMT_LOG(FLOW, DEBUG,
			"Scheduling flow add command: flow_id=%" PRIu32,
			flow_create_cmd->handle->flow_id);
		n3k_mgmt_flow_entry_dump(&flow_create_cmd->flow);
	}

	ret = n3k_mgmt_flr_cmd_queue_push_command(hw, container_of(flow_create_cmd,
		struct n3k_mgmt_flr_command, flow_create));
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR, "Failed to push flow create command to cmd queue");
		goto free_mirror;
	}

	return 0;

free_mirror:
	if (mirror) {
		free_mirror_data(hw, flow_create_cmd->mirror_handle->flow_id,
			flow_create_cmd->mirror_handle->recirc_id);
	}
	// change status back
	{
		__rte_unused int change_ret = n3k_mgmt_flow_tbl_change_flow_status(
			hw, flow_create_cmd->handle,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDING,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETED);
		RTE_ASSERT(change_ret == 0);
	}
free_flow_id:
	{
		__rte_unused int ret_free = n3k_mgmt_flow_id_free(hw, flow_id);
		RTE_ASSERT(ret_free == 0);
	}
	return ret;
}

inline static void
flr_flow_tbl_finish_add_failure(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_create *flow_create_cmd)
{
	__rte_unused int ret;
	struct n3k_mgmt_flow_tbl_handle *cur_handle =
		flow_create_cmd->insert_mirror_flow ?
		flow_create_cmd->mirror_handle :
		flow_create_cmd->handle;

	if (flow_create_cmd->insert_mirror_flow) {
		N3K_MGMT_LOG(FLOW, WARNING,
			"Failed to add scheduled mirror flow: flow_id=%" PRIu32,
			cur_handle->flow_id);
		ret = n3k_mgmt_flow_tbl_change_flow_status(hw, flow_create_cmd->handle,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDING,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_FAILED_ABSENT);
		RTE_ASSERT(ret == 0);
	} else {
		N3K_MGMT_LOG(FLOW, WARNING,
			"Failed to add scheduled flow: flow_id=%" PRIu32,
			cur_handle->flow_id);
	}

	switch (flow_create_cmd->flow_created) {
	case FLOW_CREATE_CMD_FLOW_NOT_ADDED:
		ret = n3k_mgmt_flow_tbl_change_flow_status(hw, cur_handle,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDING,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_FAILED_ABSENT);
		RTE_ASSERT(ret == 0);
		break;
	case FLOW_CREATE_CMD_FLOW_ADDED:
		ret = n3k_mgmt_map_local_to_hw_handle(hw,
			cur_handle->flow_id,
			flow_create_cmd->hw_flow_id);
		RTE_ASSERT(ret == 0);
		ret = n3k_mgmt_flow_tbl_change_flow_status(hw, cur_handle,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDING,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_FAILED_PRESENT);
		RTE_ASSERT(ret == 0);
		break;
	case FLOW_CREATE_CMD_FLOW_MAY_BE_ADDED:
		ret = n3k_mgmt_flow_tbl_change_flow_status(hw, cur_handle,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDING,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_FAILED_MAY_BE_PRESENT);
		RTE_ASSERT(ret == 0);
		break;
	default:
		RTE_ASSERT(false && "invalid flow create cmd result type");
	}

	n3k_mgmt_flr_command_free(hw, container_of(flow_create_cmd,
		struct n3k_mgmt_flr_command, flow_create));
}

inline static void
flr_flow_tbl_finish_add_success(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_create *flow_create_cmd)
{
	int ret;
	struct n3k_mgmt_flow_tbl_handle *cur_handle =
		flow_create_cmd->insert_mirror_flow ?
		flow_create_cmd->mirror_handle :
		flow_create_cmd->handle;
	ret = n3k_mgmt_map_local_to_hw_handle(hw,
		cur_handle->flow_id,
		flow_create_cmd->hw_flow_id);
	RTE_ASSERT(ret == 0);
	ret = n3k_mgmt_flow_tbl_change_flow_status(hw, cur_handle,
		N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDING,
		N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDED);
	RTE_ASSERT(ret == 0);

	if (flow_create_cmd->insert_mirror_flow) {
		N3K_MGMT_LOG(FLOW, INFO,
			"Successfully added scheduled mirror flow: flow_id=%" PRIu32,
			cur_handle->flow_id);

		// now insert base flow
		flow_create_cmd->insert_mirror_flow = false;
		flow_create_cmd->timeouted = false;
		flow_create_cmd->flow_created = FLOW_CREATE_CMD_FLOW_NOT_ADDED;
		ret = n3k_mgmt_flr_cmd_queue_push_command(hw, container_of(flow_create_cmd,
			struct n3k_mgmt_flr_command, flow_create));
		if (ret < 0) {
			N3K_MGMT_LOG(FLOW, ERR, "Failed to push flow create command to cmd queue");
			flr_flow_tbl_finish_add_failure(hw, flow_create_cmd);
		}
	} else {
		N3K_MGMT_LOG(FLOW, INFO,
			"Successfully added scheduled flow: flow_id=%" PRIu32,
			cur_handle->flow_id);

		n3k_mgmt_flr_command_free(hw, container_of(flow_create_cmd,
			struct n3k_mgmt_flr_command, flow_create));
	}
}

inline static int
flr_flow_tbl_add_parse_error_type(
	struct n3k_mgmt_flr_command_flow_create *flow_create_cmd,
	const struct n3k_mgmt_flr_parsed_block_range *parsed_cmd)
{
	bool mirror = flow_create_cmd->insert_mirror_flow;
	if (parsed_cmd == NULL) {
		flow_create_cmd->timeouted = true;
		if (flow_create_cmd->flow_created == FLOW_CREATE_CMD_FLOW_NOT_ADDED)
			flow_create_cmd->flow_created = FLOW_CREATE_CMD_FLOW_MAY_BE_ADDED;
		return -EAGAIN;
	}

	LKUP_EMLC_MGMT_CTRL_t emlc_mgmt;
	const struct n3k_mgmt_flr_parsed_block *emlc_key_handle_block =
		&parsed_cmd->blocks[mirror ?
			N3K_MGMT_FLR_MIRROR_FLOW_CREATE_READ_EMLC_KEY_HANDLE_BLOCK :
			N3K_MGMT_FLR_FLOW_CREATE_READ_EMLC_KEY_HANDLE_BLOCK];
	const struct n3k_mgmt_flr_parsed_block *emlc_mgmt_block =
		&parsed_cmd->blocks[mirror ?
			N3K_MGMT_FLR_MIRROR_FLOW_CREATE_EMLC_MGMT_CTRL_BLOCK :
			N3K_MGMT_FLR_FLOW_CREATE_EMLC_MGMT_CTRL_BLOCK];
	const struct n3k_mgmt_flr_parsed_block *read_emlc_mgmt_block =
		&parsed_cmd->blocks[mirror ?
			N3K_MGMT_FLR_MIRROR_FLOW_CREATE_READ_EMLC_MGMT_CTRL_BLOCK :
			N3K_MGMT_FLR_FLOW_CREATE_READ_EMLC_MGMT_CTRL_BLOCK];

	if (flow_create_cmd->flow_created != FLOW_CREATE_CMD_FLOW_ADDED) {
		memcpy_bswap32(&emlc_mgmt, read_emlc_mgmt_block->data, sizeof(emlc_mgmt));
		if (emlc_mgmt_block->error_code || emlc_mgmt.field.err_code) {
			if (emlc_mgmt.field.err_code == LKUP_EMLC_MGMT_CTRL_ERR_CODE_INSERT_KEY_EXISTS) {
				// in case of timeout it could be inserted earlier
				if (flow_create_cmd->timeouted)
					flow_create_cmd->flow_created = FLOW_CREATE_CMD_FLOW_MAY_BE_ADDED;
				return -EEXIST;
			}
			// verify if err_code != key exists then in this case flow is indeed not added
			flow_create_cmd->flow_created = FLOW_CREATE_CMD_FLOW_NOT_ADDED;
			return -EAGAIN;
		} else {
			LKUP_EMLC_NXT_HANDLE_t next_handle;
			flow_create_cmd->flow_created = FLOW_CREATE_CMD_FLOW_ADDED;
			memcpy_bswap32(&next_handle, emlc_key_handle_block->data, sizeof(next_handle));
			flow_create_cmd->hw_flow_id = next_handle.field.handle;
		}
	}
	if (!mirror) {
		const struct n3k_mgmt_flr_parsed_block *ft_cmd_block =
			&parsed_cmd->blocks[N3K_MGMT_FLR_FLOW_CREATE_FLOW_TRACKER_COMMAND_BLOCK];
		if (ft_cmd_block->error_code) {
			// TODO(n3k): if emlc_mgmt succeeds resend only flow tracker enable
			return -EAGAIN;
		}
	}

	return 0;
}

int
n3k_mgmt_flr_flow_tbl_add_handle_response(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_create *flow_create_cmd,
	const struct n3k_mgmt_flr_parsed_block_range *parsed_cmd,
	bool retry_limit_reached)
{
	int ret = flr_flow_tbl_add_parse_error_type(flow_create_cmd, parsed_cmd);
	if (ret == 0) {
		flr_flow_tbl_finish_add_success(hw, flow_create_cmd);
		return 0;
	}
	if (ret != -EAGAIN || retry_limit_reached) {
		flr_flow_tbl_finish_add_failure(hw, flow_create_cmd);
		return 0;
	}
	return -EAGAIN;
}

int
n3k_mgmt_flr_flow_tbl_add_append_pkt_blocks(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flr_command_flow_create *flow_create_cmd,
	struct rte_mbuf *pkt)
{
	int ret;
	uint32_t stats_id = n3k_mgmt_flow_id_to_stats_id(hw, flow_create_cmd->handle->flow_id);
	uint32_t recirc_id = flow_create_cmd->handle->recirc_id;
	const struct n3k_mgmt_flow_tbl_action *action = &flow_create_cmd->flow.action.action_raw;
	const struct n3k_mgmt_flow_tbl_key *key = &flow_create_cmd->flow.key.key_raw;

	if (!flow_create_cmd->insert_mirror_flow) {
		ret = n3k_mgmt_flr_pkt_add_ft_enable_data_block(hw, pkt, key->l3.ip_prot);
		if (ret < 0)
			return ret;

		ret = n3k_mgmt_flr_pkt_add_ft_command_block(pkt, stats_id, MKS_FLOW_TRACKER_FT_COMMAND_WRITE);
		if (ret < 0)
			goto remove_ft_data_block;

		ret = n3k_mgmt_flr_pkt_add_key_action_block(hw, pkt, key, action, stats_id, recirc_id);
		if (ret < 0)
			goto remove_ft_command_block;
	} else {
		ret = n3k_mgmt_flr_pkt_add_mirror_key_action_block(hw, pkt, action, recirc_id);
		if (ret < 0)
			return ret;
	}

	ret = n3k_mgmt_flr_pkt_add_emlc_mgmt_ctrl_block(pkt, LKUP_EMLC_MGMT_CTRL_REQ_TYPE_INSERT);
	if (ret < 0)
		goto remove_key_action_block;

	ret = n3k_mgmt_flr_pkt_add_read_emlc_key_handle_block(pkt);
	if (ret < 0)
		goto remove_emlc_mgmt_ctrl_block;

	ret = n3k_mgmt_flr_pkt_add_read_emlc_mgmt_ctrl_block(pkt);
	if (ret < 0)
		goto remove_read_emlc_key_handle_block;

	return 0;

remove_read_emlc_key_handle_block:
	n3k_mgmt_flr_remove_block(pkt, sizeof(LKUP_EMLC_NXT_HANDLE_t));
remove_emlc_mgmt_ctrl_block:
	n3k_mgmt_flr_remove_block(pkt, sizeof(LKUP_EMLC_MGMT_CTRL_t));
remove_key_action_block:
	n3k_mgmt_flr_remove_block(pkt, sizeof(*action));
	n3k_mgmt_flr_remove_block(pkt, sizeof(*key));
remove_ft_command_block:
	if (!flow_create_cmd->insert_mirror_flow) {
		n3k_mgmt_flr_remove_block(pkt, sizeof(MKS_FLOW_TRACKER_FT_COMMAND_t));
remove_ft_data_block:
		n3k_mgmt_flr_remove_block(pkt, sizeof(struct flow_tracker_data_regs));
	}
	return ret;
}

////////// FLOW TBL DEL

inline static void
flr_flow_tbl_finish_del_success(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_destroy *flow_destroy_cmd,
	bool unmap_handle)
{
	struct n3k_mgmt_flow_entry_description *desc = &flow_destroy_cmd->desc;
	struct n3k_mgmt_flow_tbl_handle *handle =
		flow_destroy_cmd->destroy_mirror_flow ?
		desc->flow_handle->mirror_flow :
		desc->flow_handle;
	bool mirror = flow_destroy_cmd->destroy_mirror_flow;
	bool schedule_mirror_del = !mirror &&
		flow_destroy_cmd->desc.flow_handle->mirror_flow;
	bool remove_description = !schedule_mirror_del;
	int action_ret = 0, key_ret = 0;
	__rte_unused int ret;

	if (flow_destroy_cmd->destroy_mirror_flow) {
		N3K_MGMT_LOG(FLOW, INFO,
			"Successfully deleted scheduled mirror flow: flow_id=%" PRIu32,
			handle->flow_id);
	} else {
		N3K_MGMT_LOG(FLOW, INFO,
			"Successfully deleted scheduled flow: flow_id=%" PRIu32,
			handle->flow_id);
	}


	if (unmap_handle) {
		ret = n3k_mgmt_unmap_local_to_hw_handle(hw, handle->flow_id);
		RTE_ASSERT(ret == 0);
	}

	if (remove_description) {
		if (desc->use_action_desc) {
			action_ret = n3k_mgmt_flow_action_description_remove(hw, &desc->action);
			if (action_ret < 0) {
				N3K_MGMT_LOG(FLOW, ERR,
					"Failed to remove action related table entries: ret=%d",
					action_ret);
			}
		}

		if (desc->use_key_desc) {
			key_ret = n3k_mgmt_flow_key_description_remove(hw, &desc->key);
			if (key_ret < 0) {
				N3K_MGMT_LOG(FLOW, ERR,
					"Failed to remove key related table entries: ret=%d",
					key_ret);
			}
		}

		if (mirror) {
			ret = n3k_mgmt_flow_tbl_change_flow_status(hw,
				flow_destroy_cmd->desc.flow_handle->mirror_flow,
				N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETING,
				N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETED);
			RTE_ASSERT(ret == 0);
			free_mirror_data(hw, desc->flow_handle->mirror_flow->flow_id,
				desc->flow_handle->mirror_flow->recirc_id);
		}
		ret = n3k_mgmt_flow_tbl_change_flow_status(
			hw, flow_destroy_cmd->desc.flow_handle,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETING,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETED);
		RTE_ASSERT(ret == 0);
		ret = n3k_mgmt_flow_id_free(hw, desc->flow_handle->flow_id);
		RTE_ASSERT(ret == 0);

		n3k_mgmt_flr_command_free(hw, container_of(flow_destroy_cmd,
			struct n3k_mgmt_flr_command, flow_destroy));
	} else if (schedule_mirror_del) {
		flow_destroy_cmd->destroy_mirror_flow = true;
		flow_destroy_cmd->timeouted = false;
		flow_destroy_cmd->flow_destroyed = FLOW_DESTROY_CMD_FLOW_NOT_DESTROYED;
		ret = n3k_mgmt_flr_flow_tbl_schedule_del(hw, flow_destroy_cmd);
		RTE_ASSERT(ret == 0);
	}
}

inline static void
flr_flow_tbl_finish_del_failure(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_destroy *flow_destroy_cmd)
{
	struct n3k_mgmt_flow_tbl_handle *handle =
		flow_destroy_cmd->destroy_mirror_flow ?
		flow_destroy_cmd->desc.flow_handle->mirror_flow :
		flow_destroy_cmd->desc.flow_handle;
	__rte_unused int ret;

	if (flow_destroy_cmd->destroy_mirror_flow) {
		N3K_MGMT_LOG(FLOW, INFO,
			"Failed to delete scheduled mirror flow: flow_id=%" PRIu32,
			handle->flow_id);
		ret = n3k_mgmt_flow_tbl_change_flow_status(
			hw, flow_destroy_cmd->desc.flow_handle,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETING,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_FAILED_ABSENT);
		RTE_ASSERT(ret == 0);
	} else {
		N3K_MGMT_LOG(FLOW, INFO,
			"Failed to delete scheduled flow: flow_id=%" PRIu32,
			handle->flow_id);
	}

	switch (flow_destroy_cmd->flow_destroyed) {
	case FLOW_DESTROY_CMD_FLOW_DESTROYED:
		RTE_ASSERT(false && "Should be success in current implementation");
		break;
	case FLOW_DESTROY_CMD_FLOW_NOT_DESTROYED:
		ret = n3k_mgmt_flow_tbl_change_flow_status(hw, handle,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETING,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_FAILED_PRESENT);
		RTE_ASSERT(ret == 0);
		break;
	case FLOW_DESTROY_CMD_FLOW_MAY_BE_DESTROYED:
		ret = n3k_mgmt_flow_tbl_change_flow_status(hw, handle,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETING,
			N3K_MGMT_FLOW_TBL_FLOW_STATUS_FAILED_MAY_BE_PRESENT);
		RTE_ASSERT(ret == 0);
		break;
	}
	n3k_mgmt_flr_command_free(hw, container_of(flow_destroy_cmd,
		struct n3k_mgmt_flr_command, flow_destroy));
}

int
n3k_mgmt_flr_flow_tbl_schedule_del(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_destroy *flow_destroy_cmd)
{
	struct n3k_mgmt_flow_tbl_handle *handle =
		flow_destroy_cmd->destroy_mirror_flow ?
		flow_destroy_cmd->desc.flow_handle->mirror_flow :
		flow_destroy_cmd->desc.flow_handle;
	enum n3k_mgmt_flow_tbl_flow_status status;
	enum {
		DELETE_ACTION_DEFAULT,
		DELETE_ACTION_NO_PACKET,
		DELETE_ACTION_DO_NOTHING,
	} action = DELETE_ACTION_DO_NOTHING;
	int ret;

	while (true) {
		rte_rwlock_read_lock(&handle->lock);
		if (handle->status != N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDING &&
			handle->status != N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETING)
		{
			rte_rwlock_read_unlock(&handle->lock);
			break;
		}
		rte_rwlock_read_unlock(&handle->lock);
		rte_delay_ms(1);
	}

	rte_rwlock_write_lock(&handle->lock);
	status = handle->status;
	switch (status) {
	case N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDED:
	case N3K_MGMT_FLOW_TBL_FLOW_STATUS_FAILED_PRESENT:
		action = DELETE_ACTION_DEFAULT;
		n3k_mgmt_flow_tbl_change_flow_status_nolock(hw, handle,
			status, N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETING);
		break;
	case N3K_MGMT_FLOW_TBL_FLOW_STATUS_FAILED_ABSENT:
		action = DELETE_ACTION_NO_PACKET;
		n3k_mgmt_flow_tbl_change_flow_status_nolock(hw, handle,
			status, N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETING);
		break;
	case N3K_MGMT_FLOW_TBL_FLOW_STATUS_FAILED_MAY_BE_PRESENT:
		// in case of hw managed flow ids we shouldn't do anything in this case
		action = DELETE_ACTION_DO_NOTHING;
		n3k_mgmt_flow_tbl_change_flow_status_nolock(hw, handle,
			status, N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETING);
		break;

	case N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETED:
	case N3K_MGMT_FLOW_TBL_FLOW_STATUS_DELETING:
	case N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDING:
		rte_rwlock_write_unlock(&handle->lock);
		return -EINVAL;
	}
	rte_rwlock_write_unlock(&handle->lock);

	switch (action) {
	case DELETE_ACTION_DEFAULT:
		N3K_MGMT_LOG(FLOW, DEBUG,
			"Scheduling flow del command: flow_id=%" PRIu32,
			handle->flow_id);
		ret = n3k_mgmt_flr_cmd_queue_push_command(hw, container_of(flow_destroy_cmd,
			struct n3k_mgmt_flr_command, flow_destroy));
		if (ret < 0) {
			N3K_MGMT_LOG(FLOW, ERR, "Failed to push flow destroy command to cmd queue");
			flow_destroy_cmd->flow_destroyed = FLOW_DESTROY_CMD_FLOW_NOT_DESTROYED;
			flr_flow_tbl_finish_del_failure(hw, flow_destroy_cmd);
		}
		return 0;
	case DELETE_ACTION_NO_PACKET:
		N3K_MGMT_LOG(FLOW, DEBUG,
			"Removing incomplete flow: flow_id=%" PRIu32,
			handle->flow_id);
		flr_flow_tbl_finish_del_success(hw, flow_destroy_cmd, false);
		return 0;
	case DELETE_ACTION_DO_NOTHING:
		N3K_MGMT_LOG(FLOW, DEBUG,
			"Removing flow in unknown state - do nothing: flow_id=%" PRIu32,
			handle->flow_id);
		flow_destroy_cmd->flow_destroyed = FLOW_DESTROY_CMD_FLOW_MAY_BE_DESTROYED;
		flr_flow_tbl_finish_del_failure(hw, flow_destroy_cmd);
		return 0;
	}

	return -EINVAL;
}

inline static int
flr_flow_tbl_del_parse_error_type(
	struct n3k_mgmt_flr_command_flow_destroy *flow_destroy_cmd,
	const struct n3k_mgmt_flr_parsed_block_range *parsed_cmd)
{
	bool mirror = flow_destroy_cmd->destroy_mirror_flow;
	if (parsed_cmd == NULL) {
		flow_destroy_cmd->timeouted = true;
		flow_destroy_cmd->flow_destroyed = FLOW_DESTROY_CMD_FLOW_MAY_BE_DESTROYED;
		return -EIO;
	}

	const struct n3k_mgmt_flr_parsed_block *emlc_mgmt_block =
		&parsed_cmd->blocks[mirror ?
			N3K_MGMT_FLR_MIRROR_FLOW_DESTROY_EMLC_MGMT_CTRL_BLOCK :
			N3K_MGMT_FLR_FLOW_DESTROY_EMLC_MGMT_CTRL_BLOCK];

	// TODO(n3k): In sw managed handles, after command timeout and retry
	// we may need to check why retried flow destroy command failed.
	// It may fail because timeouted command destroyed it.

	if (!flow_destroy_cmd->flow_destroyed) {
		if (emlc_mgmt_block->error_code)
			return -EAGAIN;
		flow_destroy_cmd->flow_destroyed = FLOW_DESTROY_CMD_FLOW_DESTROYED;
	}

	// If ft_cmd block fails it shouldn't make any difference

	return 0;
}

int
n3k_mgmt_flr_flow_tbl_del_handle_response(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_destroy *flow_destroy_cmd,
	const struct n3k_mgmt_flr_parsed_block_range *parsed_cmd,
	bool retry_limit_reached)
{
	int ret = flr_flow_tbl_del_parse_error_type(flow_destroy_cmd, parsed_cmd);
	if (ret == 0) {
		flr_flow_tbl_finish_del_success(hw, flow_destroy_cmd, true);
		return 0;
	}
	if (ret != -EAGAIN || retry_limit_reached) {
		flr_flow_tbl_finish_del_failure(hw, flow_destroy_cmd);
		return 0;
	}
	return -EAGAIN;
}

int
n3k_mgmt_flr_flow_tbl_del_append_pkt_blocks(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flr_command_flow_destroy *flow_destroy_cmd,
	struct rte_mbuf *pkt)
{
	bool mirror = flow_destroy_cmd->destroy_mirror_flow;
	const struct n3k_mgmt_flow_tbl_handle *handle = mirror ?
		flow_destroy_cmd->desc.flow_handle->mirror_flow :
		flow_destroy_cmd->desc.flow_handle;
	uint32_t hw_flow_id;
	int ret;

	ret = n3k_mgmt_local_to_hw_handle(hw, handle->flow_id, &hw_flow_id);
	if (ret < 0)
		return ret;

	ret = n3k_mgmt_flr_pkt_add_handle_block(pkt, hw_flow_id);
	if (ret < 0)
		return ret;

	ret = n3k_mgmt_flr_pkt_add_emlc_mgmt_ctrl_block(pkt,
		LKUP_EMLC_MGMT_CTRL_REQ_TYPE_DELETE_HANDLE);
	if (ret < 0)
		goto remove_handle_block;

	if (!mirror) {
		uint32_t stats_id = n3k_mgmt_flow_id_to_stats_id(hw, handle->flow_id);
		ret = n3k_mgmt_flr_pkt_add_ft_disable_data_block(pkt);
		if (ret < 0)
			goto remove_emlc_mgmt_ctrl_block;

		ret = n3k_mgmt_flr_pkt_add_ft_command_block(pkt, stats_id,
			MKS_FLOW_TRACKER_FT_COMMAND_WRITE);
		if (ret < 0)
			goto remove_ft_data_block;
	}

	return 0;

remove_ft_data_block:
	n3k_mgmt_flr_remove_block(pkt, sizeof(struct flow_tracker_data_regs));
remove_emlc_mgmt_ctrl_block:
	n3k_mgmt_flr_remove_block(pkt, sizeof(LKUP_EMLC_MGMT_CTRL_t));
remove_handle_block:
	n3k_mgmt_flr_remove_block(pkt, sizeof(LKUP_EMLC_KEY_HANDLE_t));
	return ret;
}

////////// FLOW TBL DUMP
int n3k_mgmt_flr_flow_sync_dump(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle,
	struct n3k_mgmt_flow_tbl_entry *tbl_entry)
{
	int ret;
	struct n3k_mgmt_flr_command *cmd;

	enum n3k_mgmt_flow_tbl_flow_status status;
	while (true) {
		rte_rwlock_read_lock(&handle->lock);
		if (handle->status != N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDING) {
			status = handle->status;
			rte_rwlock_read_unlock(&handle->lock);
			break;
		}
		rte_rwlock_read_unlock(&handle->lock);
		rte_delay_ms(1);
	}
	if (status != N3K_MGMT_FLOW_TBL_FLOW_STATUS_ADDED)
		return -ENOENT;

	cmd = n3k_mgmt_flr_command_alloc(hw, N3K_MGMT_FLR_COMMAND_FLOW_DUMP);
	if (cmd == NULL) {
		N3K_MGMT_LOG(FLOW, ERR, "Failed to allocate flr cmd");
		return -ENOMEM;
	}

	cmd->flow_dump.handle = handle;
	cmd->flow_dump.out_entry = tbl_entry;
	cmd->flow_dump.get_mirror_flow = false;
	ret = n3k_mgmt_flr_cmd_queue_push_command(hw, cmd);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW, ERR, "Failed to push flow dump command to cmd queue");
		n3k_mgmt_flr_command_free(hw, cmd);
		return ret;
	}
	while (rte_atomic16_read(&cmd->flow_dump.done) == 0)
		rte_delay_ms(1);
	ret = cmd->flow_dump.error_code;

	n3k_mgmt_flr_command_free(hw, cmd);
	return ret;
}

int
n3k_mgmt_flr_flow_tbl_dump_handle_response(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command_flow_dump *flow_dump_cmd,
	const struct n3k_mgmt_flr_parsed_block_range *parsed_cmd,
	bool retry_limit_reached)
{
	int ret;

	struct lookup_emlc_key_regs key_regs;
	struct lookup_emlc_res_regs res_regs;

	const struct n3k_mgmt_flr_parsed_block* key_block =
		&parsed_cmd->blocks[N3K_MGMT_FLR_FLOW_DUMP_FLOW_READ_KEY_BLOCK];
	const struct n3k_mgmt_flr_parsed_block* res_block =
		&parsed_cmd->blocks[N3K_MGMT_FLR_FLOW_DUMP_FLOW_READ_RES_BLOCK];

	if (parsed_cmd == NULL) {
		ret = -EIO;
		goto unlock_sync;
	}

	if (n3k_mgmt_flr_contains_error_block(parsed_cmd->blocks, parsed_cmd->blocks_num)) {
		if (retry_limit_reached) {
			ret = -EIO;
			goto unlock_sync;
		}
		return -EAGAIN;
	}

	memcpy_bswap32(&key_regs, key_block->data, sizeof(struct lookup_emlc_key_regs));
	memcpy_bswap32(&res_regs, res_block->data, sizeof(struct lookup_emlc_res_regs));

	if (flow_dump_cmd->get_mirror_flow) {
		ret = n3k_mgmt_flow_tbl_fill_res_mirror_info(hw,
			&flow_dump_cmd->out_entry->action,
			&res_regs);
		if (ret < 0) {
			N3K_MGMT_LOG(FLOW, ERR,
				"Failed to fill mirror flow res info; ret=%d", ret);
			goto unlock_sync;
		}
	} else {
		ret = n3k_mgmt_flow_tbl_fill_key_info(hw, &flow_dump_cmd->out_entry->key,
			&key_regs);
		if (ret < 0) {
			N3K_MGMT_LOG(FLOW, ERR, "Failed to fill flow key info; ret=%d", ret);
			goto unlock_sync;
		}

		ret = n3k_mgmt_flow_tbl_fill_res_info(hw, flow_dump_cmd->handle->encap_type,
			&flow_dump_cmd->out_entry->action, &res_regs);
		if (ret < 0) {
			N3K_MGMT_LOG(FLOW, ERR, "Failed to fill flow res info; ret=%d", ret);
			goto unlock_sync;
		}

		if (flow_dump_cmd->handle->mirror_flow != NULL) {
			flow_dump_cmd->get_mirror_flow = true;
			ret = n3k_mgmt_flr_cmd_queue_push_command(hw, container_of(flow_dump_cmd,
				struct n3k_mgmt_flr_command, flow_dump));
			if (ret < 0) {
				N3K_MGMT_LOG(FLOW, ERR,
					"Failed to push flow dump command to cmd queue; ret=%d", ret);
				goto unlock_sync;
			}
			return 0;
		}
	}

unlock_sync:
	flow_dump_cmd->error_code = ret;
	rte_atomic16_inc(&flow_dump_cmd->done);
	return ret;
}

static int add_block_read_key_regs(struct rte_mbuf *pkt)
{
	return n3k_mgmt_flr_add_read_block(pkt,
		MKS_CHIP_LKUP_BASE + LKUP_EMLC_KEY_0,
		sizeof(struct lookup_emlc_key_regs));
}

static int add_block_read_res_regs(struct rte_mbuf *pkt)
{
	return n3k_mgmt_flr_add_read_block(pkt,
		MKS_CHIP_LKUP_BASE + LKUP_EMLC_RES_0,
		sizeof(struct lookup_emlc_res_regs));
}

int n3k_mgmt_flr_flow_tbl_dump_append_pkt_blocks(__rte_unused struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flr_command_flow_dump *flow_dump_cmd,
	struct rte_mbuf *pkt)
{
	int ret;
	bool mirror = flow_dump_cmd->get_mirror_flow;
	const struct n3k_mgmt_flow_tbl_handle *handle = mirror ?
		flow_dump_cmd->handle->mirror_flow :
		flow_dump_cmd->handle;
	uint32_t hw_flow_id;

	ret = n3k_mgmt_local_to_hw_handle(hw, handle->flow_id, &hw_flow_id);
	if (ret < 0)
		return ret;

	ret = n3k_mgmt_flr_pkt_add_handle_block(pkt, hw_flow_id);
	if (ret < 0)
		return ret;

	ret = n3k_mgmt_flr_pkt_add_emlc_mgmt_ctrl_block(pkt, LKUP_EMLC_MGMT_CTRL_REQ_TYPE_SEARCH_HANDLE);
	if (ret < 0)
		goto remove_handle_block;

	ret = add_block_read_key_regs(pkt);
	if (ret < 0)
		goto remove_ctrl_block;

	ret = add_block_read_res_regs(pkt);
	if (ret < 0)
		goto remove_key_read_block;

	return 0;

remove_key_read_block:
	n3k_mgmt_flr_remove_block(pkt, sizeof(struct lookup_emlc_key_regs));
remove_ctrl_block:
	n3k_mgmt_flr_remove_block(pkt, sizeof(LKUP_EMLC_MGMT_CTRL_t));
remove_handle_block:
	n3k_mgmt_flr_remove_block(pkt, sizeof(LKUP_EMLC_KEY_HANDLE_t));
	return ret;
}
