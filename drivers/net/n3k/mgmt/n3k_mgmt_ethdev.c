/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include <stdbool.h>

#include <rte_eth_bond.h>
#include <rte_ethdev_pci.h>
#include <rte_flow.h>

#include "n3k_mgmt_ethdev.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_rte_flow.h"

#include "n3k_mgmt_info.h"
#include "n3k_mgmt_lag.h"
#include "n3k_mgmt_init/n3k_mgmt_init_mq.h"
#include "vdev/n3k_vdev_device.h"

extern struct rte_pci_driver rte_n3k_mgmt;

static int
n3k_mgmt_dev_configure(struct rte_eth_dev *dev __rte_unused)
{
	return 0;
}

static int
n3k_mgmt_dev_start(struct rte_eth_dev *dev __rte_unused)
{
	return 0;
}

static void
n3k_mgmt_dev_stop(struct rte_eth_dev *dev __rte_unused)
{

}

static int
n3k_mgmt_link_update(
	struct rte_eth_dev *dev __rte_unused,
	int wait_to_complete __rte_unused)
{
	return 0;
}

static int
n3k_mgmt_dev_infos_get(struct rte_eth_dev *dev __rte_unused,
	struct rte_eth_dev_info *dev_info)
{
	dev_info->max_rx_queues = 1;
	dev_info->max_tx_queues = 1;

	return 0;
}

static int
n3k_mgmt_rx_queue_setup(
	struct rte_eth_dev *dev __rte_unused,
	uint16_t rx_queue_id __rte_unused,
	uint16_t nb_rx_desc __rte_unused,
	unsigned int socket_id __rte_unused,
	const struct rte_eth_rxconf *rx_conf __rte_unused,
	struct rte_mempool *mb_pool __rte_unused)
{
	return 0;
}

static int
n3k_mgmt_tx_queue_setup(
	struct rte_eth_dev *dev __rte_unused,
	uint16_t tx_queue_id __rte_unused,
	uint16_t nb_tx_desc __rte_unused,
	unsigned int socket_id __rte_unused,
	const struct rte_eth_txconf *tx_conf __rte_unused)
{
	return 0;
}

static int
n3k_mgmt_filter_ctrl(struct rte_eth_dev *dev,
	enum rte_filter_type filter_type, enum rte_filter_op filter_op,
	void *arg)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (!hw)
		return -EINVAL;

	if (filter_type == RTE_ETH_FILTER_GENERIC && filter_op == RTE_ETH_FILTER_GET) {
		if (hw->insert_type == N3K_MGMT_HW_INSERT_TYPE_FLR ||
			hw->insert_type == N3K_MGMT_HW_INSERT_TYPE_FLR_SIM)
		{
			N3K_MGMT_LOG(DEFAULT, DEBUG, "Using FLR rte flow methods");
			*(const void **)arg = n3k_mgmt_flr_rte_flow_get_ops();
			return 0;
		} else if (hw->insert_type == N3K_MGMT_HW_INSERT_TYPE_CSR) {
			N3K_MGMT_LOG(DEFAULT, DEBUG, "Using default rte flow methods");
			*(const void **)arg = n3k_mgmt_rte_flow_get_ops();
			return 0;
		}
		return -ENOTSUP;
	}

	return -ENOTSUP;
}

static const struct eth_dev_ops n3kmgmt_dev_ops = {
	.dev_configure  = n3k_mgmt_dev_configure,
	.dev_start      = n3k_mgmt_dev_start,
	.dev_stop       = n3k_mgmt_dev_stop,

	.link_update    = n3k_mgmt_link_update,
	.dev_infos_get  = n3k_mgmt_dev_infos_get,

	.rx_queue_setup = n3k_mgmt_rx_queue_setup,
	.tx_queue_setup = n3k_mgmt_tx_queue_setup,

	.filter_ctrl    = n3k_mgmt_filter_ctrl,
	.fw_version_get = n3k_mgmt_info_fw_version_get,
};

static uint16_t
n3k_ethdev_rx_burst(
	void *rx_queue __rte_unused,
	struct rte_mbuf **rx_pkts __rte_unused,
	uint16_t nb_pkts __rte_unused)
{
	static bool this_function_has_not_been_invoked_yet = true;
	if (this_function_has_not_been_invoked_yet) {
		N3K_MGMT_LOG(DEFAULT, ERR, "This operation is not supported. "
			"This message is not going to be printed for subsequent calls.");
		this_function_has_not_been_invoked_yet = false;
	}
	return 0;
}

static uint16_t
n3k_ethdev_tx_burst(
	void *tx_queue __rte_unused,
	struct rte_mbuf **tx_pkts __rte_unused,
	uint16_t nb_pkts __rte_unused)
{
	static bool this_function_has_not_been_invoked_yet = true;
	if (this_function_has_not_been_invoked_yet) {
		N3K_MGMT_LOG(DEFAULT, ERR, "This operation is not supported. "
			"This message is not going to be printed for subsequent calls.");
		this_function_has_not_been_invoked_yet = false;
	}
	return 0;
}

static int
n3k_mgmt_eth_dev_init(struct rte_eth_dev *eth_dev, void *init_params)
{
	struct n3k_mgmt_eth_dev_internal *eth_dev_private = eth_dev->data->dev_private;
	struct n3k_mgmt_eth_dev_init_info *eth_dev_init_params = init_params;

	eth_dev_private->hw = eth_dev_init_params->hw;

	eth_dev->dev_ops = &n3kmgmt_dev_ops;

	eth_dev->rx_pkt_burst = n3k_ethdev_rx_burst;
	eth_dev->tx_pkt_burst = n3k_ethdev_tx_burst;

	eth_dev->data->mac_addrs = rte_zmalloc(eth_dev->data->name,
		RTE_ETHER_ADDR_LEN, 0);

	if (eth_dev->data->mac_addrs == NULL) {
		N3K_MGMT_LOG(DEFAULT, ERR, "Failed to allocate MAC address");
		return -ENOMEM;
	}

	return 0;
}

int
n3k_mgmt_eth_dev_create(struct rte_pci_device *pci_dev,
	struct n3k_mgmt_eth_dev_init_info *dev_info, struct rte_eth_dev **eth_dev)
{
	int ret;
	const char *name = pci_dev->device.name;

	ret = rte_eth_dev_create(&pci_dev->device, name,
		sizeof(struct n3k_mgmt_eth_dev_internal), NULL, NULL,
		n3k_mgmt_eth_dev_init, dev_info);
	if (ret) {
		return ret;
	}

	*eth_dev = rte_eth_dev_allocated(name);

	return 0;
}

int
n3k_mgmt_lag_set(struct rte_eth_dev *eth_dev, bool enable)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(eth_dev);

	if (!hw) {
		N3K_MGMT_LOG(DEFAULT, ERR, "Invalid device");
		return -EINVAL;
	}

	if (enable) {
		N3K_MGMT_LOG(DEFAULT, INFO, "Enabling LAG");
		n3k_mgmt_lag_enable(hw);
	} else {
		N3K_MGMT_LOG(DEFAULT, INFO, "Disabling LAG");
		n3k_mgmt_lag_disable(hw);
	}

	return 0;
}

int n3k_mgmt_save_representor_list(struct rte_eth_dev *eth_dev,
	struct rte_eth_dev **reprs, size_t reprs_cnt)
{
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(eth_dev);
	size_t i;

	if (!hw) {
		N3K_MGMT_LOG(DEFAULT, ERR, "Invalid mgmt device");
		return -EINVAL;
	}

	for (i = 0; i < reprs_cnt; ++i) {
		enum n3k_vdev_dev_type type;
		uint16_t id;
		n3k_vdev_get_representor_info(reprs[i], &type, &id);

		switch (type) {
		case N3K_VDEV_DEV_TYPE_PHY:
			RTE_ASSERT(id < N3K_PHY_PORTS_COUNT);
			hw->phy_representors[id] = reprs[i];
			break;

		case N3K_VDEV_DEV_TYPE_VF:
			RTE_ASSERT(id < N3K_VF_PORTS_MAX_COUNT + N3K_VF_PORT_MIN_ID);
			hw->vf_representors[id] = reprs[i];
			break;

		case N3K_VDEV_DEV_TYPE_LAG:
			break;
		}
	}

	return 0;
}

// TODO(n3k): refactor MQ support
int
n3k_mgmt_set_vf_mq(struct n3k_mgmt_hw *hw, uint16_t vf_id,
	uint16_t mq_nb)
{
	if (mq_nb > 0) // +1 for control queue
		++mq_nb;
	N3K_MGMT_LOG(DEFAULT, INFO, "Setting %u MQs for VF%u", mq_nb, vf_id);
	if (n3k_mgmt_init_set_vf_mq(hw, 0, vf_id, mq_nb)) {
		N3K_MGMT_LOG(DEFAULT, ERR, "MQ configuration for vf%u failed", vf_id);
		return -1;
	}
	return 0;
}

int
n3k_mgmt_set_pf_mq(struct n3k_mgmt_hw *hw, uint16_t mq_nb)
{
	if (mq_nb > 0) // +1 for control queue
		++mq_nb;
	N3K_MGMT_LOG(DEFAULT, INFO, "Setting %u MQs for PFs", mq_nb);
	if (n3k_mgmt_init_set_pf_mq(hw, 0, mq_nb)) {
		N3K_MGMT_LOG(DEFAULT, ERR, "MQ configuration for pf 0 failed");
		return -1;
	}
	if (n3k_mgmt_init_set_pf_mq(hw, 1, mq_nb)) {
		N3K_MGMT_LOG(DEFAULT, ERR, "MQ configuration for pf 1 failed");
		return -1;
	}
	return 0;
}

bool n3k_mgmt_is_valid_n3k_mgmt(const struct rte_eth_dev *dev)
{
	return dev
		&& dev->device
		&& dev->device->driver
		&& (dev->device->driver->name == rte_n3k_mgmt.driver.name);
}

struct n3k_mgmt_hw *n3k_mgmt_get_hw_from_dev_private(const struct rte_eth_dev *dev)
{
	struct n3k_mgmt_eth_dev_internal *dev_private;
	if (!n3k_mgmt_is_valid_n3k_mgmt(dev))
		return NULL;
	dev_private = dev->data->dev_private;
	return dev_private->hw;
}

int
n3k_mgmt_get_device_by_id(struct n3k_mgmt_hw *hw, const struct n3k_vdev_dev_id *dev_id,
	struct rte_eth_dev **dev)
{
	switch (dev_id->type) {
	case N3K_VDEV_DEV_TYPE_PHY:
		if (dev_id->id >= RTE_DIM(hw->phy_representors))
			return -EINVAL;
		*dev = hw->phy_representors[dev_id->id];
		return 0;

	case N3K_VDEV_DEV_TYPE_VF:
		if (dev_id->id >= RTE_DIM(hw->vf_representors))
			return -EINVAL;
		*dev = hw->vf_representors[dev_id->id];
		return 0;

	case N3K_VDEV_DEV_TYPE_LAG:
		break;
	}

	return -EINVAL;
}
