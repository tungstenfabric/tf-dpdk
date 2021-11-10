/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_vdev_flow.h"
#include "n3k_vdev_device.h"

#include <rte_flow.h>
#include <rte_flow_driver.h>

static int
n3k_vdev_flow_validate(struct rte_eth_dev *dev,
	const struct rte_flow_attr *attr,
	const struct rte_flow_item item[],
	const struct rte_flow_action action[],
	struct rte_flow_error *error)
{
	struct n3k_vdev_dev_internal *dev_internal = dev->data->dev_private;
	struct n3k_vdev_dev_shared *dev_shared = dev_internal->shared;
	struct rte_eth_dev *mgmt_dev = dev_shared->mgmt_dev;
	uint16_t port_id = mgmt_dev->data->port_id;

	return rte_flow_validate(port_id, attr, item, action, error);
}

static struct rte_flow *
n3k_vdev_flow_create(struct rte_eth_dev *dev,
	const struct rte_flow_attr *attr,
	const struct rte_flow_item item[],
	const struct rte_flow_action action[],
	struct rte_flow_error *error)
{
	struct n3k_vdev_dev_internal *dev_internal = dev->data->dev_private;
	struct n3k_vdev_dev_shared *dev_shared = dev_internal->shared;
	struct rte_eth_dev *mgmt_dev = dev_shared->mgmt_dev;
	uint16_t port_id = mgmt_dev->data->port_id;

	return rte_flow_create(port_id, attr, item, action, error);
}

static int
n3k_vdev_flow_destroy(struct rte_eth_dev *dev,
	struct rte_flow *flow,
	struct rte_flow_error *error)
{
	struct n3k_vdev_dev_internal *dev_internal = dev->data->dev_private;
	struct n3k_vdev_dev_shared *dev_shared = dev_internal->shared;
	struct rte_eth_dev *mgmt_dev = dev_shared->mgmt_dev;
	uint16_t port_id = mgmt_dev->data->port_id;

	return rte_flow_destroy(port_id, flow, error);
}

static int
n3k_vdev_flow_flush(struct rte_eth_dev *dev,
	struct rte_flow_error *error)
{
	struct n3k_vdev_dev_internal *dev_internal = dev->data->dev_private;
	struct n3k_vdev_dev_shared *dev_shared = dev_internal->shared;
	struct rte_eth_dev *mgmt_dev = dev_shared->mgmt_dev;
	uint16_t port_id = mgmt_dev->data->port_id;

	return rte_flow_flush(port_id, error);
}

static int
n3k_vdev_flow_query(struct rte_eth_dev *dev,
	struct rte_flow *flow,
	const struct rte_flow_action *action,
	void *data,
	struct rte_flow_error *error)
{
	struct n3k_vdev_dev_internal *dev_internal = dev->data->dev_private;
	struct n3k_vdev_dev_shared *dev_shared = dev_internal->shared;
	struct rte_eth_dev *mgmt_dev = dev_shared->mgmt_dev;
	uint16_t port_id = mgmt_dev->data->port_id;

	return rte_flow_query(port_id, flow, action, data, error);
}

static int
n3k_vdev_flow_isolate(struct rte_eth_dev *dev,
	int set,
	struct rte_flow_error *error)
{
	struct n3k_vdev_dev_internal *dev_internal = dev->data->dev_private;
	struct n3k_vdev_dev_shared *dev_shared = dev_internal->shared;
	struct rte_eth_dev *mgmt_dev = dev_shared->mgmt_dev;
	uint16_t port_id = mgmt_dev->data->port_id;

	return rte_flow_isolate(port_id, set, error);
}

const struct rte_flow_ops n3k_vdev_flow_ops = {
	.validate = n3k_vdev_flow_validate,
	.create   = n3k_vdev_flow_create,
	.destroy  = n3k_vdev_flow_destroy,
	.flush    = n3k_vdev_flow_flush,
	.query    = n3k_vdev_flow_query,
	.isolate  = n3k_vdev_flow_isolate,
};
