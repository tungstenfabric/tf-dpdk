/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "mgmt/n3k_mgmt_dump.h"
#include "mgmt/n3k_mgmt_ethdev.h"
#include "mgmt/n3k_mgmt_hw.h"
#include "mgmt/n3k_mgmt_port_stats.h"
#include "rte_pmd_n3k_port.h"

#include <rte_ethdev.h>

#include <stdint.h>

int
rte_pmd_n3k_get_vdev_stats(struct rte_eth_dev *dev, struct n3k_mgmt_port_stats *stats)
{
	return n3k_mgmt_get_vdev_stats(dev, stats);
}

int
rte_pmd_n3k_port_stats_dump(struct rte_eth_dev *dev)
{
	return n3k_mgmt_port_stats_dump(dev);
}

void
rte_pmd_n3k_port_stats_struct_dump(struct n3k_mgmt_port_stats *stats)
{
	n3k_mgmt_port_stats_struct_dump(stats);
}

static const char*
rte_pmd_n3k_vdev_type_to_str(enum n3k_vdev_dev_type type)
{
	switch (type) {
	case N3K_VDEV_DEV_TYPE_VF:
		return "VF";
	case N3K_VDEV_DEV_TYPE_PHY:
		return "PHY";
	case N3K_VDEV_DEV_TYPE_LAG:
		return "LAG";
	default:
		return "UNKNOWN";
	}
}

const char *
rte_pmd_n3k_vdev_id_to_str(char *buff,
	struct n3k_vdev_dev_id device_id)
{
	sprintf(buff, "%s%" PRIu16,
