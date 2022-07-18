/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>
#include <linux/virtio_net.h>
#include <stdbool.h>

#include <rte_memory.h>
#include <rte_bus_pci.h>
#include <rte_vhost.h>
#include <rte_vfio.h>
#include <rte_spinlock.h>
#include <rte_log.h>
#include <rte_kvargs.h>
#include <rte_devargs.h>
#include <rte_pmd_n3k_malloc.h>

#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_args.h"
#include "n3k_mgmt_ethdev.h"
#include "n3k_mgmt_mp.h"
#include "n3k_mgmt_log.h"

#define N3K_MGMT_PCI_DEV_MAX_DEV 1

struct n3k_mgmt_pci_dev_internal {
	struct rte_pci_device *pci_dev;
	struct rte_eth_dev *eth_dev;
	struct n3k_mgmt_hw hw;
} *n3k_mgmt_pci_dev_internals[N3K_MGMT_PCI_DEV_MAX_DEV];

static pthread_mutex_t internal_list_lock = PTHREAD_MUTEX_INITIALIZER;

static int
alloc_pci_dev_internal(struct n3k_mgmt_pci_dev_internal **internal)
{
	int ret = 0, release_ret;
	*internal = n3k_zmalloc("n3k_mgmt_pci_dev_internal", sizeof(**internal), 0);
	if (*internal == NULL)
		return -ENOMEM;

	release_ret = pthread_mutex_lock(&internal_list_lock);
	if (release_ret < 0) {
		N3K_MGMT_LOG(DEFAULT, CRIT, "mutex lock error: %d", release_ret);
		return release_ret;
	}
	int i;
	for (i = 0; i < N3K_MGMT_PCI_DEV_MAX_DEV; i++)
		if (n3k_mgmt_pci_dev_internals[i] == NULL)
			break;
	if (i == N3K_MGMT_PCI_DEV_MAX_DEV) {
		n3k_free(*internal);
		ret = -ENOSPC;
		goto release_lock;
	}
	n3k_mgmt_pci_dev_internals[i] = *internal;

release_lock:
	release_ret = pthread_mutex_unlock(&internal_list_lock);
	if (release_ret != 0) {
		N3K_MGMT_LOG(DEFAULT, CRIT, "mutex unlock error: %d", release_ret);
		return release_ret;
	}
	return ret;
}

static int
n3k_mgmt_vfio_setup(struct n3k_mgmt_pci_dev_internal *internal)
{
	struct rte_pci_device *dev = internal->pci_dev;
	int ret;
	int i;

	ret = rte_pci_map_device(dev);
	if (ret < 0)
		return ret;

	for (i = 0; i < RTE_MIN(PCI_MAX_RESOURCE, N3K_MGMT_PCI_MAX_RESOURCE); i++) {
		internal->hw.mem_resource[i].addr =
			internal->pci_dev->mem_resource[i].addr;
		internal->hw.mem_resource[i].phys_addr =
			internal->pci_dev->mem_resource[i].phys_addr;
		internal->hw.mem_resource[i].len =
			internal->pci_dev->mem_resource[i].len;
	}

	return 0;
}

static int
n3k_mgmt_pci_probe(struct rte_pci_driver *pci_drv __rte_unused,
		struct rte_pci_device *pci_dev)
{
	int ret;
	struct n3k_mgmt_pci_dev_internal *internal;
	struct n3k_mgmt_eth_dev_init_info eth_dev_init_info;
	struct n3k_mgmt_args args;
	struct rte_eth_dev *eth_dev = NULL;

	if (pci_dev->kdrv == RTE_KDRV_NONE || pci_dev->kdrv == RTE_KDRV_UNKNOWN)
		return -ENOTSUP; // because of a bug in vfio code

	if (rte_eal_process_type() != RTE_PROC_PRIMARY)
		return 0;

	ret = alloc_pci_dev_internal(&internal);
	if (ret < 0) {
		N3K_MGMT_LOG(DEFAULT, ERR, "couldn't allocate pci dev internal, ret=%d", ret);
		return ret;
	}
	internal->pci_dev = pci_dev;

	ret = n3k_mgmt_args_parse(pci_dev, &args);
	if (ret < 0) {
		N3K_MGMT_LOG(DEFAULT, ERR, "failed to parse args, ret=%d", ret);
		goto free_internal;
	}

	ret = n3k_mgmt_vfio_setup(internal);
	if (ret < 0) {
		N3K_MGMT_LOG(DEFAULT, ERR, "failed to setup device %s, ret=%d",
			pci_dev->name, ret);
		goto free_internal;
	}

	ret = n3k_mgmt_init_hw(&internal->hw, internal->pci_dev, &args);
	if (ret < 0) {
		N3K_MGMT_LOG(DEFAULT, ERR, "failed to init device %s, red=%d",
			pci_dev->name, ret);
		goto free_internal;
	}

	ret = n3k_mgmt_mp_init(&internal->hw);
	if (ret < 0) {
		N3K_MGMT_LOG(DEFAULT, ERR,
			"failed to initialize multi-process communication module; ret = %d",
			ret);
		goto free_internal;
	}

	eth_dev_init_info.hw = &internal->hw;
	ret = n3k_mgmt_eth_dev_create(pci_dev, &eth_dev_init_info, &eth_dev);
	if (ret || eth_dev == NULL) {
		N3K_MGMT_LOG(DEFAULT, ERR, "failed to init eth device %s",
			pci_dev->name);
		goto free_internal;
	}

	return 0;

free_internal:
	n3k_free(internal);
	return ret;
}

static struct n3k_mgmt_pci_dev_internal *
pop_pci_dev_internal(struct rte_pci_device *pci_dev)
{
	struct n3k_mgmt_pci_dev_internal *internal;
	int i;
	int ret;
	ret = pthread_mutex_lock(&internal_list_lock);
	if (ret != 0) {
		N3K_MGMT_LOG(DEFAULT, CRIT, "mutex lock error: %d", ret);
		return NULL;
	}
	for (i = 0; i < N3K_MGMT_PCI_DEV_MAX_DEV; i++)
		if (n3k_mgmt_pci_dev_internals[i] &&
			n3k_mgmt_pci_dev_internals[i]->pci_dev == pci_dev)
			break;
	if (i == N3K_MGMT_PCI_DEV_MAX_DEV) {
		internal = NULL;
		goto release_lock;
	}
	internal = n3k_mgmt_pci_dev_internals[i];
	n3k_mgmt_pci_dev_internals[i] = NULL;
release_lock:
	ret = pthread_mutex_unlock(&internal_list_lock);
	if (ret != 0) {
		N3K_MGMT_LOG(DEFAULT, CRIT, "mutex unlock error: %d", ret);
		return NULL;
	}
	return internal;
}

// TODO(n3k): implement and call deinit functions for all modules
static int
n3k_mgmt_pci_remove(struct rte_pci_device *pci_dev)
{
	struct n3k_mgmt_pci_dev_internal *internal;

	if (rte_eal_process_type() != RTE_PROC_PRIMARY)
		return 0;

	internal = pop_pci_dev_internal(pci_dev);
	if (internal == NULL) {
		N3K_MGMT_LOG(DEFAULT, ERR, "couldn't find pci dev internal for %s",
			pci_dev->name);
		return -EINVAL;
	}

	n3k_mgmt_mp_exit(&internal->hw);

	rte_pci_unmap_device(internal->pci_dev);

	n3k_free(internal);

	return 0;
}

static const struct rte_pci_id pci_id_n3k_mgmt_map[] = {
	{
		.class_id = RTE_CLASS_ANY_ID,
		.vendor_id = N3K_MGMT_VENDOR_ID,
		.device_id = N3K_MGMT_DEVICE_ID,
		.subsystem_vendor_id = N3K_MGMT_SUBSYS_VENDOR_ID,
		.subsystem_device_id = N3K_MGMT_SUBSYS_DEVICE_ID,
	},

	{
		.class_id = RTE_CLASS_ANY_ID,
		.vendor_id = N3K_MGMT_SUBSYS_VENDOR_ID,
		.device_id = N3K_MGMT_SUBSYS_DEVICE_ID,
		.subsystem_vendor_id = N3K_MGMT_SUBSYS_VENDOR_ID,
		.subsystem_device_id = N3K_MGMT_SUBSYS_DEVICE_ID,
	},

	{ .vendor_id = 0, /* sentinel */
	},
};

struct rte_pci_driver rte_n3k_mgmt = {
	.id_table = pci_id_n3k_mgmt_map,
	.drv_flags = 0,
	.probe = n3k_mgmt_pci_probe,
	.remove = n3k_mgmt_pci_remove,
};

RTE_PMD_REGISTER_PCI(net_n3k_mgmt, rte_n3k_mgmt);
RTE_PMD_REGISTER_PCI_TABLE(net_n3k_mgmt, pci_id_n3k_mgmt_map);
RTE_PMD_REGISTER_KMOD_DEP(net_n3k_mgmt, "* vfio-pci");

RTE_PMD_REGISTER_PARAM_STRING(net_n3k_mgmt,
	"pf_mqs=<int> "
	"mqs=<int_list> "
);

RTE_INIT(n3k_mgmt_init_log)
{
	n3k_mgmt_log_register(N3K_MGMT_LOGTYPE_DEFAULT, "pmd.net.n3k.mgmt");
	n3k_mgmt_log_register(N3K_MGMT_LOGTYPE_REGS, "pmd.net.n3k.mgmt.regs");
	n3k_mgmt_log_register(N3K_MGMT_LOGTYPE_FLOW, "pmd.net.n3k.mgmt.flow");
	n3k_mgmt_log_register(N3K_MGMT_LOGTYPE_FLOW_PARSE, "pmd.net.n3k.mgmt.flow_parse");
	n3k_mgmt_log_register(N3K_MGMT_LOGTYPE_MP, "pmd.net.n3k.mgmt.mp");
	n3k_mgmt_log_register(N3K_MGMT_LOGTYPE_TUNNEL, "pmd.net.n3k.mgmt.tunnel");
	n3k_mgmt_log_register(N3K_MGMT_LOGTYPE_SMAC, "pmd.net.n3k.mgmt.smac");
	n3k_mgmt_log_register(N3K_MGMT_LOGTYPE_VPLKP, "pmd.net.n3k.mgmt.vplkp");
	n3k_mgmt_log_register(N3K_MGMT_LOGTYPE_FLR_DEFAULT, "pmd.net.n3k.mgmt.flr");
	n3k_mgmt_log_register(N3K_MGMT_LOGTYPE_FLR_SIM, "pmd.net.n3k.mgmt.flr.sim");
	n3k_mgmt_log_register(N3K_MGMT_LOGTYPE_FLR_DISPATCHER, "pmd.net.n3k.mgmt.flr.dispatcher");
	n3k_mgmt_log_register(N3K_MGMT_LOGTYPE_FLR_ACK_DISPATCHER, "pmd.net.n3k.mgmt.flr.ack-dispatcher");
	n3k_mgmt_log_register(N3K_MGMT_LOGTYPE_FLR_PKTS, "pmd.net.n3k.mgmt.flr.pkts");
	n3k_mgmt_log_register(N3K_MGMT_LOGTYPE_LAG, "pmd.net.n3k.mgmt.lag");
}
