/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_VDEV_DISPATCHER_H_
#define _PMD_NET_N3K_VDEV_DISPATCHER_H_

#include <rte_ethdev.h>

struct n3k_vdev_dispatcher;

struct n3k_vdev_dispatcher *
n3k_vdev_dispatcher_register(
	struct rte_eth_dev *mgmt_dev, struct rte_eth_dev *pf_dev,
	size_t vdevs_count, struct rte_eth_dev **vdevs);

void
n3k_vdev_dispatcher_unregister(struct n3k_vdev_dispatcher *dispatcher);

void
n3k_vdev_dispatcher_start(struct n3k_vdev_dispatcher *dispatcher);

void
n3k_vdev_dispatcher_stop(struct n3k_vdev_dispatcher *dispatcher);

#endif /* _PMD_NET_N3K_VDEV_DISPATCHER_H_ */
