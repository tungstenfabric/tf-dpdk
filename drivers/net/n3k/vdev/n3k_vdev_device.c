/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_vdev_log.h"
#include "n3k_vdev_dispatcher.h"
#include "n3k_vdev_flow.h"
#include "n3k_vdev_device.h"
#include "n3k_vdev_rxtx.h"

#include "mgmt/n3k_mgmt_ethdev.h"

#include <rte_pmd_n3k_port.h>
#include <rte_pmd_n3k_malloc.h>
#include <rte_bus_vdev.h>
#include <rte_ethdev_driver.h>

static inline void
n3k_vdev_init_internal_from_info(struct n3k_vdev_dev_internal *internal,
	struct n3k_vdev_dev_info *info)
{
	internal->shared = info->shared;
	internal->id = info->id;
	internal->type = info->type;
}

static int
n3k_vdev_pf_dev_configure(struct rte_eth_dev *pf_dev,
	uint16_t nb_rx_queues, uint16_t nb_tx_queues,
	const struct rte_eth_conf *conf)
{
	int ret;
	bool was_started = false;

	if (pf_dev->data->dev_started) {
		was_started = true;
		rte_eth_dev_stop(pf_dev->data->port_id);
	}

	ret = rte_eth_dev_configure(pf_dev->data->port_id,
		nb_rx_queues, nb_tx_queues, conf);
	if (ret)
		return ret;

	if (was_started) {
		ret = rte_eth_dev_start(pf_dev->data->port_id);
		if (ret)
			return ret;
	}

	return 0;
}

static bool
n3k_vdev_check_if_hardware_config_matches(
	const struct rte_eth_dev *pf_dev, const struct rte_eth_dev *vf_dev)
{
	const struct rte_eth_conf *pf_conf = &pf_dev->data->dev_conf;
	const struct rte_eth_conf *vf_conf = &vf_dev->data->dev_conf;
	const struct n3k_vdev_dev_internal *dev_internal = vf_dev->data->dev_private;
	const bool dedic_q = dev_internal->dedicated_queue_enabled;
	const uint16_t vf_nb_rx_queues = vf_dev->data->nb_rx_queues - dedic_q;
	const uint16_t vf_nb_tx_queues = vf_dev->data->nb_rx_queues - dedic_q;

	if (pf_dev->data->nb_rx_queues != vf_nb_rx_queues)
		return false;

	if (pf_dev->data->nb_tx_queues != vf_nb_tx_queues)
		return false;

	if (memcmp(pf_conf, vf_conf, sizeof(struct rte_eth_conf)))
		return false;

	return true;
}

static int
n3k_vdev_dev_configure(struct rte_eth_dev *dev)
{
	int ret;

	struct n3k_vdev_dev_internal *dev_internal = dev->data->dev_private;
	struct n3k_vdev_dev_shared *dev_shared = dev_internal->shared;
	struct rte_eth_dev *pf_dev = dev_shared->pf_dev;

	N3K_VDEV_LOG(DEFAULT, DEBUG, "Configuring %s", dev->data->name);

	if (!dev_shared->pf_configured || (!pf_dev->data->dev_started &&
			!n3k_vdev_check_if_hardware_config_matches(pf_dev, dev))) {
		ret = n3k_vdev_pf_dev_configure(pf_dev,
			dev->data->nb_rx_queues - dev_internal->dedicated_queue_enabled,
			dev->data->nb_tx_queues - dev_internal->dedicated_queue_enabled,
			&dev->data->dev_conf);
		if (ret) {
			N3K_VDEV_LOG(DEFAULT, ERR,
				 "Configuration of PF dev while configuring %s failed",
				 dev->data->name);
			return ret;
		}
		dev_shared->pf_configured = true;
	} else if (!n3k_vdev_check_if_hardware_config_matches(pf_dev, dev)) {
		N3K_VDEV_LOG(DEFAULT, ERR,
			 "Configuration for %s does not match PF configuration",
			 dev->data->name);
		return -EINVAL;
	}

	/* TODO: Add configuration of VF here if needed. */

	if (!dev_internal->configured) {
		dev_internal->configured = true;
		dev_shared->configured_vdevs_count++;
	}

	return 0;
}

static int
n3k_vdev_dev_start(struct rte_eth_dev *dev)
{
	int ret = 0;

	struct n3k_vdev_dev_internal *dev_internal = dev->data->dev_private;
	struct n3k_vdev_dev_shared *dev_shared = dev_internal->shared;
	struct rte_eth_dev *pf_dev = dev_shared->pf_dev;

	N3K_VDEV_LOG(DEFAULT, DEBUG, "Starting %s", dev->data->name);

	if (!dev_internal->configured) {
		N3K_VDEV_LOG(DEFAULT, ERR, "Device %s cannot be started before it is configured",
			dev->data->name);
		return -ENOTSUP;
	} else if (!n3k_vdev_check_if_hardware_config_matches(pf_dev, dev)) {
		N3K_VDEV_LOG(DEFAULT, ERR,
			 "Configuration for %s does not match PF configuration",
			 dev->data->name);
		return -EINVAL;
	}

	if (!pf_dev->data->dev_started) {
		N3K_VDEV_LOG(DEFAULT, DEBUG, "PF was stopped. Starting...");

		ret = rte_eth_dev_start(pf_dev->data->port_id);
		if (ret) {
			N3K_VDEV_LOG(DEFAULT, ERR, "Failed to start PF %s for VDEV %s",
				pf_dev->data->name, dev->data->name);
				return ret;
		}
	}

	n3k_vdev_dispatcher_start(dev_shared->dispatcher);
	dev_shared->started_vdevs_count++;

	return 0;
}

static void
n3k_vdev_dev_stop(struct rte_eth_dev *dev)
{
	struct n3k_vdev_dev_internal *dev_internal = dev->data->dev_private;
	struct n3k_vdev_dev_shared *dev_shared = dev_internal->shared;
	struct rte_eth_dev *pf_dev = dev_shared->pf_dev;

	N3K_VDEV_LOG(DEFAULT, DEBUG, "Stopping %s", dev->data->name);

	dev_shared->started_vdevs_count--;

	if (dev_shared->started_vdevs_count == 0) {
		N3K_VDEV_LOG(DEFAULT, DEBUG, "All VDEVs stopped. Stopping PF and dispatcher.");
		n3k_vdev_dispatcher_stop(dev_shared->dispatcher);
		rte_eth_dev_stop(pf_dev->data->port_id);
	}
}

static int
n3k_vdev_promisc_enable(__rte_unused struct rte_eth_dev *dev)
{
	// Enabled by default
	return 0;
}

static int
n3k_vdev_allmulti_enable(__rte_unused struct rte_eth_dev *dev)
{
	// Enabled by default
	return 0;
}

static int
n3k_vdev_link_update(struct rte_eth_dev *dev, int wait_to_complete)
{
	struct rte_eth_link pf_link;

	struct n3k_vdev_dev_internal *dev_internal = dev->data->dev_private;
	struct n3k_vdev_dev_shared *dev_shared = dev_internal->shared;
	struct rte_eth_dev *pf_dev = dev_shared->pf_dev;

	if (wait_to_complete)
		rte_eth_link_get(pf_dev->data->port_id, &pf_link);
	else
		rte_eth_link_get_nowait(pf_dev->data->port_id, &pf_link);

	return rte_eth_linkstatus_set(dev, &pf_link);
}

static int
n3k_vdev_mac_addr_set(struct rte_eth_dev *dev, struct rte_ether_addr *mac_addr)
{
	if (!rte_is_valid_assigned_ether_addr(mac_addr)) {
		N3K_VDEV_LOG(DEFAULT, ERR, "Tried to set invalid MAC address");
		return -EINVAL;
	}

	rte_ether_addr_copy(mac_addr, dev->data->mac_addrs);

	return 0;
}

static int
n3k_vdev_dev_infos_get(
	struct rte_eth_dev *dev, struct rte_eth_dev_info *dev_info)
{
	struct n3k_vdev_dev_internal *dev_internal = dev->data->dev_private;
	struct n3k_vdev_dev_shared *dev_shared = dev_internal->shared;
	struct rte_eth_dev *pf_dev = dev_shared->pf_dev;

	int ret = rte_eth_dev_info_get(pf_dev->data->port_id, dev_info);
	if (ret)
		return ret;

	dev_info->device = dev->device;
	dev_info->max_mac_addrs = 1;

	if (dev_internal->dedicated_queue_enabled) {
		++dev_info->max_rx_queues;
		++dev_info->max_tx_queues;
	}

	return 0;
}

static int
n3k_vdev_stats_get(struct rte_eth_dev *dev, struct rte_eth_stats *stats)
{
	struct n3k_mgmt_port_stats n3k_stats;
	int ret;

	ret = rte_pmd_n3k_get_vdev_stats(dev, &n3k_stats);
	if (ret < 0)
		return ret;

	stats->ibytes = stats->q_ibytes[0] = n3k_stats.ibytes;
	stats->ipackets = stats->q_ipackets[0] = n3k_stats.ipackets;
	stats->obytes = stats->q_obytes[0] = n3k_stats.obytes;
	stats->opackets = stats->q_opackets[0] = n3k_stats.opackets;

	return 0;
}

static int
n3k_vdev_filter_ctrl(struct rte_eth_dev *dev __rte_unused,
	enum rte_filter_type filter_type, enum rte_filter_op filter_op,
	void *arg)
{
	if (filter_type == RTE_ETH_FILTER_GENERIC) {
		if (filter_op != RTE_ETH_FILTER_GET)
			return -EINVAL;
		*(const void **)arg = &n3k_vdev_flow_ops;
		return 0;
	}
	else {
		N3K_VDEV_LOG(DEFAULT, WARNING, "Filter type (%d) not supported",
			filter_type);
		return -ENOTSUP;
	}
}

static const struct eth_dev_ops n3k_vdev_dev_ops = {
	.dev_configure    = n3k_vdev_dev_configure,

	.dev_start        = n3k_vdev_dev_start,
	.dev_stop         = n3k_vdev_dev_stop,

	.link_update      = n3k_vdev_link_update,

	.promiscuous_enable  = n3k_vdev_promisc_enable,
	.allmulticast_enable = n3k_vdev_allmulti_enable,

	.mac_addr_set     = n3k_vdev_mac_addr_set,

	.stats_get        = n3k_vdev_stats_get,

	.dev_infos_get    = n3k_vdev_dev_infos_get,

	.rx_queue_setup   = n3k_vdev_rx_queue_setup,
	.rx_queue_release = n3k_vdev_rx_queue_release,

	.tx_queue_setup   = n3k_vdev_tx_queue_setup,
	.tx_queue_release = n3k_vdev_tx_queue_release,

	.filter_ctrl      = n3k_vdev_filter_ctrl,
};

static int
n3k_vdev_dev_init(struct rte_eth_dev *eth_dev, void *init_params)
{
	struct n3k_vdev_dev_internal *dev_internal = eth_dev->data->dev_private;
	struct n3k_vdev_dev_info *params = init_params;

	n3k_vdev_init_internal_from_info(dev_internal, params);
	dev_internal->configured = false;
	dev_internal->dedicated_queue_enabled = false;

	eth_dev->data->dev_flags |= RTE_ETH_DEV_REPRESENTOR;
	eth_dev->data->representor_id = params->id | (params->type << 15);

	eth_dev->dev_ops = &n3k_vdev_dev_ops;

	eth_dev->rx_pkt_burst = n3k_vdev_rx_burst;
	eth_dev->tx_pkt_burst = n3k_vdev_tx_burst;

	eth_dev->data->mac_addrs = n3k_zmalloc(eth_dev->data->name,
		RTE_ETHER_ADDR_LEN, 0);

	if (eth_dev->data->mac_addrs == NULL) {
		N3K_VDEV_LOG(DEFAULT, ERR, "Failed to allocate MAC address");
		return -ENOMEM;
	}

	*(eth_dev->data->mac_addrs) = params->mac;

	return 0;
}

int
n3k_vdev_dev_create(struct rte_vdev_device *dev,
	struct n3k_vdev_dev_info *dev_info, struct rte_eth_dev **vdev)
{
	int ret;
	const char *dev_name;
	char name[RTE_ETH_NAME_MAX_LEN];

	dev_name = rte_vdev_device_name(dev);

	if (dev_name == NULL)
		return -ENODEV;

	snprintf(name, sizeof(name), "%s_%s%d", dev_name,
		dev_info->type == N3K_VDEV_DEV_TYPE_PHY ? "phy" : "vf", dev_info->id);

	ret = rte_eth_dev_create(&dev->device, name,
		sizeof(struct n3k_vdev_dev_internal), NULL, NULL,
		n3k_vdev_dev_init, dev_info);
	if (ret)
		return ret;

	*vdev = rte_eth_dev_allocated(name);
	(*vdev)->data->promiscuous = 1;
	(*vdev)->data->all_multicast = 1;

	return ret;
}

struct n3k_vdev_dev_shared *
n3k_vdev_dev_alloc_shared_data(struct rte_eth_dev *pf_dev,
	struct rte_eth_dev *mgmt_dev)
{
	struct n3k_vdev_dev_shared *shared = n3k_zmalloc("n3k_shared",
		sizeof(*shared), 0);

	if (!shared)
		return NULL;

	shared->pf_dev = pf_dev;
	shared->mgmt_dev = mgmt_dev;
	shared->configured_vdevs_count = 0;
	shared->started_vdevs_count = 0;

	return shared;
}

bool
n3k_vdev_is_valid_n3k_repr(const struct rte_eth_dev *dev)
{
	return dev
		&& dev->data
		&& dev->data->dev_private
		&& (dev->data->dev_flags & RTE_ETH_DEV_REPRESENTOR)
		&& dev->device
		&& dev->device->driver
		&& (dev->device->driver->name == pmd_n3k_vdev_drv.driver.name);
}

bool
n3k_vdev_is_valid_n3k_vf_repr(const struct rte_eth_dev *dev)
{
	return n3k_vdev_is_valid_n3k_repr(dev) &&
		((struct n3k_vdev_dev_internal *)dev->data->dev_private)->type == N3K_VDEV_DEV_TYPE_VF;
}

bool
n3k_vdev_is_valid_n3k_phy_repr(const struct rte_eth_dev *dev)
{
	return n3k_vdev_is_valid_n3k_repr(dev) &&
		((struct n3k_vdev_dev_internal *)dev->data->dev_private)->type == N3K_VDEV_DEV_TYPE_PHY;
}


void
n3k_vdev_get_representor_info(
	const struct rte_eth_dev *dev,
	enum n3k_vdev_dev_type *out_type,
	uint16_t *out_id)
{
	struct n3k_vdev_dev_internal *dev_internal = dev->data->dev_private;

	*out_type = dev_internal->type;
	*out_id = dev_internal->id;
}

int
n3k_vdev_get_mgmt_dev(
	const struct rte_eth_dev *dev,
	struct rte_eth_dev **mgmt)
{
	struct n3k_vdev_dev_internal *dev_internal;
	struct n3k_vdev_dev_shared *dev_shared;
	if (!n3k_vdev_is_valid_n3k_repr(dev))
		return -EINVAL;
	dev_internal = dev->data->dev_private;
	dev_shared = dev_internal->shared;
	*mgmt = dev_shared->mgmt_dev;
	return 0;
}
