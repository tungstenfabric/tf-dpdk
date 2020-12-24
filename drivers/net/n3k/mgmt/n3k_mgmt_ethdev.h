/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_ETHDEV_H_
#define _PMD_NET_N3K_MGMT_ETHDEV_H_

#include "../vdev/n3k_vdev_device.h"
#include "n3k_mgmt_hw.h"

#include <stdbool.h>

#include <rte_bus_pci.h>
#include <rte_ethdev_pci.h>
#include <rte_pci.h>

struct n3k_mgmt_eth_dev_internal {
	struct n3k_mgmt_hw* hw;
};

struct n3k_mgmt_eth_dev_init_info {
	struct n3k_mgmt_hw* hw;
};

int n3k_mgmt_eth_dev_create(struct rte_pci_device *pci_dev,
	struct n3k_mgmt_eth_dev_init_info *dev_info, struct rte_eth_dev **eth_dev);

int n3k_mgmt_lag_enable(struct rte_eth_dev *eth_dev, bool enable);

int n3k_mgmt_save_representor_list(struct rte_eth_dev *eth_dev,
	struct rte_eth_dev **reprs, size_t reprs_cnt);

int n3k_mgmt_set_vf_mq(struct n3k_mgmt_hw *hw, uint16_t vf_id,
	uint16_t mq_nb);

int n3k_mgmt_set_pf_mq(struct n3k_mgmt_hw *hw, uint16_t mq_nb);


bool n3k_mgmt_is_valid_n3k_mgmt(const struct rte_eth_dev *dev);

struct n3k_mgmt_hw *n3k_mgmt_get_hw_from_dev_private(const struct rte_eth_dev *dev);

int n3k_mgmt_get_device_by_id(struct n3k_mgmt_hw *hw, const struct n3k_vdev_dev_id *dev_id,
	struct rte_eth_dev **dev);
int n3k_mgmt_is_port_valid_lag_bond(uint16_t port_id);
int n3k_mgmt_get_lag_port_id(struct n3k_mgmt_hw *hw);

#endif /* _PMD_NET_N3K_MGMT_ETHDEV_H_ */
