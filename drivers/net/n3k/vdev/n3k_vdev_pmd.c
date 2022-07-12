/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include <rte_eal.h>
#include <rte_bus_vdev.h>

#include "n3k_vdev_log.h"
#include "n3k_vdev_args.h"
#include "n3k_vdev_dispatcher.h"
#include "n3k_vdev_device.h"

#include "mgmt/n3k_mgmt_ethdev.h"

const struct rte_ether_addr null_mac = {{0}};

static int
n3k_vdev_probe(struct rte_vdev_device *dev)
{
	const char *name;
	struct n3k_vdev_args args;
	struct n3k_vdev_dev_info dev_info;
	struct n3k_vdev_dev_shared *vdev_shared;
	struct rte_eth_dev *virtual_devices[RTE_MAX_ETHPORTS];
	int ret;
	int i;

	if (!dev)
		return -EINVAL;

	name = rte_vdev_device_name(dev);
	N3K_VDEV_LOG(DEFAULT, INFO, "Initializing pmd_n3k for %s", name);

	if (rte_eal_process_type() == RTE_PROC_SECONDARY)
		return -1;

	ret = n3k_vdev_args_parse(dev, &args);
	if (ret)
		return ret;

	ret = n3k_mgmt_lag_set(args.mgmt_dev, args.is_lag_enabled);
	if (ret) {
		N3K_VDEV_LOG(DEFAULT, ERR, "%s LAG mode for %s failed",
			args.is_lag_enabled ? "Enabling" : "Disabling", name);
		return ret;
	}

	vdev_shared = n3k_vdev_dev_alloc_shared_data(args.pf_dev, args.mgmt_dev);
	if (!vdev_shared)
		return -ENOMEM;

	dev_info.shared = vdev_shared;

	for (i = 0; i < N3K_PHY_REPRESENTORS_COUNT; ++i) {
		dev_info.id = i;
		dev_info.type = N3K_VDEV_DEV_TYPE_PHY;
		dev_info.mac = (i<args.mac_count)?args.macs[i]:null_mac;

		ret = n3k_vdev_dev_create(dev, &dev_info, &virtual_devices[i]);
		if (ret) {
			/* TODO: Free all created */
			N3K_VDEV_LOG(DEFAULT, ERR, "Initializing phy%d for %s failed", i, name);
			return ret;
		}
	}

	for (i = 0; i < args.vfs_count; ++i) {
		dev_info.id = args.vfs[i];
		dev_info.type = N3K_VDEV_DEV_TYPE_VF;
		dev_info.mac = null_mac;

		ret = n3k_vdev_dev_create(dev, &dev_info,
			&virtual_devices[i + N3K_PHY_REPRESENTORS_COUNT]);
		if (ret) {
			/* TODO: Free all created */
			N3K_VDEV_LOG(DEFAULT, ERR, "Initializing vf%d for %s failed",
				args.vfs[i], name);
			return ret;
		}
	}

	vdev_shared->dispatcher = n3k_vdev_dispatcher_register(
		args.mgmt_dev, args.pf_dev,
		args.vfs_count + N3K_PHY_REPRESENTORS_COUNT, virtual_devices);
	if (!vdev_shared->dispatcher) {
		/* TODO: Free all */
		N3K_VDEV_LOG(DEFAULT, ERR, "Initializing dispatcher for %s failed", name);
		return -ENOMEM;
	}

	ret = n3k_mgmt_save_representor_list(args.mgmt_dev, virtual_devices,
		args.vfs_count + N3K_PHY_REPRESENTORS_COUNT);
	RTE_ASSERT(ret == 0);

	return 0;
}

static int
n3k_vdev_remove(struct rte_vdev_device *dev)
{
	if (!dev)
		return -EINVAL;

	/* TODO: Add and implement n3k_vdev_uninit/remove and call it here for all devices */
	/* TODO: Dispatcher unregister */

	return 0;
}

struct rte_vdev_driver pmd_n3k_vdev_drv = {
	.probe = n3k_vdev_probe,
	.remove = n3k_vdev_remove,
};

RTE_PMD_REGISTER_VDEV(net_n3k, pmd_n3k_vdev_drv);

RTE_PMD_REGISTER_PARAM_STRING(net_n3k,
	"pf=<string> "
	"vfs=<string> "
	"mgmt=<string> "
	"lag=<string> "
	"mac=<string> "
);

RTE_INIT(n3k_vdev_init_log)
{
	n3k_vdev_log_register(N3K_VDEV_LOGTYPE_DEFAULT, "pmd.net.n3k.vdev");
	n3k_vdev_log_register(N3K_VDEV_LOGTYPE_DISPATCHER, "pmd.net.n3k.vdev.dispatcher");
}
