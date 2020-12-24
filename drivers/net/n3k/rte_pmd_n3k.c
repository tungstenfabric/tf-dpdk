/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "rte_pmd_n3k.h"
#include "vdev/n3k_vdev_device.h"
#include "vdev/n3k_vdev_log.h"

#include <string.h>
#include <rte_pci.h>
#include <rte_vdpa.h>
#include <rte_eth_bond.h>
#include <rte_ethdev_driver.h>

#define PCI_FUNC_MASK 0x07
#define PCI_FUNC_LEN 3

static inline bool
rte_pmd_n3k_is_vf_repr(const struct rte_eth_dev *dev)
{
	return dev->data->representor_id & (N3K_VDEV_DEV_TYPE_VF << 15);
}

static inline uint16_t
rte_pmd_n3k_get_vf_pci_offset(uint16_t vf_id, struct rte_pci_addr *pf)
{
	return vf_id + 1 + pf->function;
}

static inline uint8_t
rte_pmd_n3k_get_vf_pci_devid(uint16_t vf_id, struct rte_pci_addr *pf)
{
	uint16_t offset = rte_pmd_n3k_get_vf_pci_offset(vf_id, pf);

	return ((offset & ~PCI_FUNC_MASK) >> PCI_FUNC_LEN) + pf->devid;
}

static inline uint8_t
rte_pmd_n3k_get_vf_pci_function(uint16_t vf_id, struct rte_pci_addr *pf)
{
	uint16_t offset = rte_pmd_n3k_get_vf_pci_offset(vf_id, pf);

	return offset & PCI_FUNC_MASK;
}

static uint16_t
rte_pmd_n3k_get_repr_id(const struct rte_eth_dev *dev)
{
	uint16_t rid = dev->data->representor_id;

	return rte_pmd_n3k_is_vf_repr(dev) ? (rid & ~(N3K_VDEV_DEV_TYPE_VF << 15)) - 1 : rid;
}

static int
rte_pmd_n3k_get_pci_addr(const struct rte_eth_dev *repr_dev, struct rte_pci_addr *repr_addr)
{
	char pf_name[RTE_DEV_NAME_MAX_LEN];
	struct rte_pci_addr pf_addr;
	struct n3k_vdev_dev_internal *dev_internal = repr_dev->data->dev_private;
	struct n3k_vdev_dev_shared *dev_shared = dev_internal->shared;
	struct rte_eth_dev *pf_dev = dev_shared->pf_dev;

	rte_eth_dev_get_name_by_port(pf_dev->data->port_id, pf_name);

	if (rte_pci_addr_parse(pf_name, &pf_addr)) {
		N3K_VDEV_LOG(ERR, "Parsing vf0 VF's PCI address failed: %s", pf_name);
		return -EINVAL;
	}

	uint16_t vf_id = rte_pmd_n3k_get_repr_id(repr_dev);

	repr_addr->domain = pf_addr.domain;
	repr_addr->bus = pf_addr.bus;
	repr_addr->devid = rte_pmd_n3k_get_vf_pci_devid(vf_id, &pf_addr);
	repr_addr->function =  rte_pmd_n3k_get_vf_pci_function(vf_id, &pf_addr);

	return 0;
}

static int
rte_pmd_n3k_get_repr_id_from_bond(uint16_t port_id, uint16_t *repr_id)
{
	uint16_t slaves[RTE_MAX_ETHPORTS];

	int count = rte_eth_bond_active_slaves_get(port_id, slaves, RTE_MAX_ETHPORTS);
	if (count <= 0)
		return -ENODEV;

	return rte_pmd_n3k_get_repr_id_by_port_id(slaves[0], repr_id);
}

int
rte_pmd_n3k_get_repr_id_by_port_id(uint16_t port_id, uint16_t *repr_id)
{
	const struct rte_eth_dev *repr_dev = &rte_eth_devices[port_id];

	if (n3k_vdev_is_valid_n3k_repr(repr_dev)) {
		*repr_id = rte_pmd_n3k_get_repr_id(repr_dev);
		return 0;
	}

	return rte_pmd_n3k_get_repr_id_from_bond(port_id, repr_id);
}

int rte_pmd_n3k_get_vdpa_did_by_repr_name(const char *name)
{
	struct rte_vdpa_dev_addr addr;

	const struct rte_eth_dev *repr_dev = rte_eth_dev_allocated(name);
	if (!n3k_vdev_is_valid_n3k_repr(repr_dev) || !rte_pmd_n3k_is_vf_repr(repr_dev)) {
		N3K_VDEV_LOG(ERR,
			"Could not find valid n3k VF representor with name: %s", name);
		return -ENODEV;
	}

	int ret = rte_pmd_n3k_get_pci_addr(repr_dev, &addr.pci_addr);
	if (ret)
		return ret;

	addr.type = PCI_ADDR;
	return rte_vdpa_find_device_id(&addr);
}
