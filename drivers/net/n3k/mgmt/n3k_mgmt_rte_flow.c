/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_ethdev.h"
#include "n3k_mgmt_flow_tbl.h"
#include "n3k_mgmt_flr_command.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_rte_flow.h"
#include "n3k_mgmt_rte_flow_parse.h"
#include "n3k_mgmt_tbl.h"
#include "n3k_mgmt_flr_tbl.h"
#include "n3k_mgmt_flr_flow_tracker.h"

#include "../rte_pmd_n3k_malloc.h"

#include <rte_common.h>
#include <rte_debug.h>
#include <rte_ethdev.h>
#include <rte_flow_driver.h>
#include <rte_flow.h>

#include <string.h>

struct rte_flow {
	struct n3k_mgmt_flow_entry_description desc;
};

static int
n3k_mgmt_rte_flow_validate(struct rte_eth_dev *dev,
			const struct rte_flow_attr *attr,
			const struct rte_flow_item pattern[],
			const struct rte_flow_action actions[],
			struct rte_flow_error *error)
{
	RTE_SET_USED(dev);
	RTE_SET_USED(attr);
	RTE_SET_USED(pattern);
	RTE_SET_USED(actions);
	RTE_SET_USED(error);

	return rte_flow_error_set(error, ENOSYS,
				  RTE_FLOW_ERROR_TYPE_UNSPECIFIED, NULL,
				  "Not implemented");
}

static struct rte_flow *
n3k_mgmt_rte_flow_create(struct rte_eth_dev *dev,
			const struct rte_flow_attr *attr,
			const struct rte_flow_item pattern[],
			const struct rte_flow_action actions[],
			struct rte_flow_error *error)
{
	struct n3k_mgmt_hw *hw;
	RTE_SET_USED(attr);

	hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw) {
		rte_flow_error_set(error, EINVAL, RTE_FLOW_ERROR_TYPE_UNSPECIFIED,
			NULL, "invalid device");
		return NULL;
	}

	struct n3k_mgmt_flow_entry parsed_flow;
	int ret = n3k_mgmt_rte_flow_to_tables(pattern, actions, &parsed_flow);
	if (ret != 0) {
		rte_flow_error_set(error, -ret, RTE_FLOW_ERROR_TYPE_UNSPECIFIED,
			   NULL, "converting provided pattern and actions failed");
		return NULL;
	}

	struct rte_flow *flow = n3k_malloc("n3k_mgmt_rte_flow", sizeof(*flow), 0);
	if (!flow) {
		rte_flow_error_set(error, ENOMEM, RTE_FLOW_ERROR_TYPE_UNSPECIFIED,
			NULL, "couldn't allocate memory for handle");
		return NULL;
	}

	struct n3k_mgmt_flow_tbl_handle *flow_handle;
	ret = n3k_mgmt_flow_add(hw, &parsed_flow, &flow_handle);
	if (ret) {
		rte_flow_error_set(error, -ret, RTE_FLOW_ERROR_TYPE_UNSPECIFIED,
			   NULL, "inserting flow to hardware failed");
		n3k_free(flow);
		return NULL;
	}
	n3k_mgmt_flow_entry_to_description(flow_handle, &parsed_flow, &flow->desc);

	return flow;
}

static struct rte_flow *
n3k_mgmt_flr_rte_flow_create(struct rte_eth_dev *dev,
			const struct rte_flow_attr *attr,
			const struct rte_flow_item pattern[],
			const struct rte_flow_action actions[],
			struct rte_flow_error *error)
{
	struct n3k_mgmt_hw *hw;
	struct n3k_mgmt_flr_command *cmd;
	RTE_SET_USED(attr);

	hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw) {
		rte_flow_error_set(error, EINVAL, RTE_FLOW_ERROR_TYPE_UNSPECIFIED,
			NULL, "invalid device");
		return NULL;
	}

	cmd = n3k_mgmt_flr_command_alloc(hw, N3K_MGMT_FLR_COMMAND_FLOW_CREATE);
	if (cmd == NULL) {
		rte_flow_error_set(error, ENOMEM, RTE_FLOW_ERROR_TYPE_UNSPECIFIED,
			NULL, "couldn't allocate flr command");
		return NULL;
	}

	int ret = n3k_mgmt_rte_flow_to_tables(pattern, actions, &cmd->flow_create.flow);
	if (ret != 0) {
		rte_flow_error_set(error, -ret, RTE_FLOW_ERROR_TYPE_UNSPECIFIED,
			   NULL, "converting provided pattern and actions failed");
		n3k_mgmt_flr_command_free(hw, cmd);
		return NULL;
	}

	struct rte_flow *flow = n3k_malloc("n3k_mgmt_rte_flow", sizeof(*flow), 0);
	if (flow == NULL) {
		rte_flow_error_set(error, ENOMEM, RTE_FLOW_ERROR_TYPE_UNSPECIFIED,
			NULL, "couldn't allocate memory for handle");
		n3k_mgmt_flr_command_free(hw, cmd);
		return NULL;
	}

	struct n3k_mgmt_flow_tbl_handle *handle;
	n3k_mgmt_flow_entry_to_description(NULL, &cmd->flow_create.flow,
		&flow->desc);
	ret = n3k_mgmt_flr_flow_schedule_add(hw, &cmd->flow_create, &handle);
	if (ret) {
		rte_flow_error_set(error, -ret, RTE_FLOW_ERROR_TYPE_UNSPECIFIED,
			   NULL, "scheduling inserting flow to hardware failed");
		n3k_mgmt_flr_command_free(hw, cmd);
		n3k_free(flow);
		return NULL;
	}
	n3k_mgmt_set_description_handle(handle, &flow->desc);

	return flow;
}

static int
n3k_mgmt_rte_flow_destroy(struct rte_eth_dev *dev,
			struct rte_flow *flow,
			struct rte_flow_error *error)
{
	struct n3k_mgmt_hw *hw;

	hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw) {
		rte_flow_error_set(error, EINVAL, RTE_FLOW_ERROR_TYPE_UNSPECIFIED,
			NULL, "invalid device");
		return -EINVAL;
	}

	int ret = n3k_mgmt_flow_entry_description_remove(hw, &flow->desc);
	if (ret) {
		N3K_MGMT_LOG(ERR, "N3K flow deletion failed");
		rte_flow_error_set(error, -ret, RTE_FLOW_ERROR_TYPE_UNSPECIFIED,
			NULL, "deleting flow from hardware failed");
		return ret;
	}

	n3k_free(flow);

	return 0;
}

static int
n3k_mgmt_flr_rte_flow_destroy(struct rte_eth_dev *dev,
			struct rte_flow *flow,
			struct rte_flow_error *error)
{
	struct n3k_mgmt_flr_command *cmd;
	struct n3k_mgmt_hw *hw;
	int ret;

	hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw) {
		rte_flow_error_set(error, EINVAL, RTE_FLOW_ERROR_TYPE_UNSPECIFIED,
			NULL, "invalid device");
		return -EINVAL;
	}

	cmd = n3k_mgmt_flr_command_alloc(hw, N3K_MGMT_FLR_COMMAND_FLOW_DESTROY);
	if (cmd == NULL) {
		rte_flow_error_set(error, ENOMEM, RTE_FLOW_ERROR_TYPE_UNSPECIFIED,
			NULL, "couldn't allocate flr command");
		return -ENOMEM;
	}
	cmd->flow_destroy.desc = flow->desc;

	ret = n3k_mgmt_flr_flow_schedule_del(hw, &cmd->flow_destroy);
	if (ret < 0) {
		rte_flow_error_set(error, -ret, RTE_FLOW_ERROR_TYPE_UNSPECIFIED,
			   NULL, "scheduling deleting flow from hardware failed");
		n3k_mgmt_flr_command_free(hw, cmd);
		return ret;
	}

	n3k_free(flow);
	return 0;
}

static int
n3k_mgmt_rte_flow_flush(struct rte_eth_dev *dev, struct rte_flow_error *error)
{
	RTE_SET_USED(dev);
	RTE_SET_USED(error);

	return rte_flow_error_set(error, ENOSYS,
				  RTE_FLOW_ERROR_TYPE_UNSPECIFIED, NULL,
				  "Not implemented");
}

static int
n3k_mgmt_rte_flow_query(struct rte_eth_dev *dev,
			struct rte_flow *flow,
			const struct rte_flow_action *action,
			void *data,
			struct rte_flow_error *error)
{
	// TODO(?): Implement timeout reset
	RTE_SET_USED(action);

	if (!data) {
		rte_flow_error_set(error, EINVAL, RTE_FLOW_ERROR_TYPE_UNSPECIFIED,
			   NULL, "pointer to struct containing statistics is invalid");
		return -EINVAL;
	}

	struct rte_flow_query_count *query = data;

	struct n3k_mgmt_flow_tracker_stats stats;
	int ret = rte_pmd_n3k_flow_tracker_get(dev, flow->desc.flow_handle, &stats);
	if (ret) {
		rte_flow_error_set(error, -ret, RTE_FLOW_ERROR_TYPE_UNSPECIFIED,
			   NULL, "querying flow info from hardware failed");
		return ret;
	}

	// TODO: flow dump

	query->bytes = stats.bytes_cnt;
	query->hits = stats.packet_cnt;

	return 0;
}

static int
n3k_mgmt_flr_rte_flow_query(struct rte_eth_dev *dev,
			struct rte_flow *flow,
			const struct rte_flow_action *action,
			void *data,
			struct rte_flow_error *error)
{
	RTE_SET_USED(action);
	struct n3k_mgmt_hw *hw;
	int ret;

	if (!data) {
		rte_flow_error_set(error, EINVAL, RTE_FLOW_ERROR_TYPE_UNSPECIFIED,
			   NULL, "pointer to struct containing statistics is invalid");
		return -EINVAL;
	}
	struct rte_flow_query_count *query = data;

	hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw) {
		rte_flow_error_set(error, EINVAL, RTE_FLOW_ERROR_TYPE_UNSPECIFIED,
			NULL, "invalid device");
		return -EINVAL;
	}

	struct n3k_mgmt_flow_tracker_stats ft_stats;
	ret = n3k_mgmt_flr_flow_tracker_get_sync(hw, flow->desc.flow_handle, &ft_stats);
	if (ret < 0) {
		rte_flow_error_set(error, EIO, RTE_FLOW_ERROR_TYPE_UNSPECIFIED,
			NULL, "couldn't get flow tracker stats");
		return -EIO;
	}

	query->bytes = ft_stats.bytes_cnt;
	query->hits = ft_stats.packet_cnt;
	return 0;
}

static const struct rte_flow_ops n3k_mgmt_rte_flow_ops = {
	.validate = n3k_mgmt_rte_flow_validate,
	.create = n3k_mgmt_rte_flow_create,
	.destroy = n3k_mgmt_rte_flow_destroy,
	.flush = n3k_mgmt_rte_flow_flush,
	.query = n3k_mgmt_rte_flow_query,
	.isolate = NULL,
};

static const struct rte_flow_ops n3k_mgmt_flr_rte_flow_ops = {
	.validate = n3k_mgmt_rte_flow_validate,
	.create = n3k_mgmt_flr_rte_flow_create,
	.destroy = n3k_mgmt_flr_rte_flow_destroy,
	.flush = n3k_mgmt_rte_flow_flush,
	.query = n3k_mgmt_flr_rte_flow_query,
	.isolate = NULL,
};

const struct rte_flow_ops *
n3k_mgmt_rte_flow_get_ops(void)
{
	return &n3k_mgmt_rte_flow_ops;
}

const struct rte_flow_ops *
n3k_mgmt_flr_rte_flow_get_ops(void)
{
	return &n3k_mgmt_flr_rte_flow_ops;
}
