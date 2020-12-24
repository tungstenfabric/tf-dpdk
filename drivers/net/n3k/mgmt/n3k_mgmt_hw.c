/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include <stdbool.h>
#include <string.h>

#include <rte_cycles.h>

#include "../rte_pmd_n3k_malloc.h"

#include "n3k_mgmt_ethdev.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_args.h"
#include "n3k_mgmt_init.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_osdep.h"

#include "n3k_mgmt_init/n3k_mgmt_init_flow.h"
#include "n3k_mgmt_init/n3k_mgmt_init_port.h"
#include "n3k_mgmt_init/n3k_mgmt_init_flow.h"
#include "n3k_mgmt_init/n3k_mgmt_init_mq.h"

static void *
get_cap_addr(struct n3k_mgmt_hw *hw, struct n3k_mgmt_pci_cap *cap)
{
	u8 bar = cap->bar;
	u32 length = cap->length;
	u32 offset = cap->offset;

	if (bar > N3K_MGMT_PCI_MAX_RESOURCE - 1) {
		N3K_MGMT_LOG(ERR, "invalid bar: %u\n", bar);
		return NULL;
	}

	if (offset + length < offset) {
		N3K_MGMT_LOG(ERR, "offset(%u) + length(%u) overflows\n",
			offset, length);
		return NULL;
	}

	if (offset + length > hw->mem_resource[cap->bar].len) {
		N3K_MGMT_LOG(ERR, "offset(%u) + length(%u) overflows bar length(%u)",
			offset, length, (u32)hw->mem_resource[cap->bar].len);
		return NULL;
	}

	return hw->mem_resource[bar].addr + offset;
}

static int
configure_pci_capabilities(struct n3k_mgmt_hw *hw, struct rte_pci_device *dev)
{
	struct n3k_mgmt_pci_cap cap;
	int ret;
	u8 pos;

	// configure pci capabilities
	ret = PCI_READ_CONFIG_BYTE(dev, &pos, PCI_CAPABILITY_LIST);
	if (ret < 0) {
		N3K_MGMT_LOG(ERR, "failed to read pci capability list");
		return ret;
	}

	N3K_MGMT_LOG(DEBUG, "reading PCI capabilities");
	for (; pos; pos = cap.cap_next) {
		ret = PCI_READ_CONFIG_RANGE(dev, (u32 *)&cap,
				sizeof(cap), pos);
		if (ret < 0) {
			N3K_MGMT_LOG(ERR, "failed to read cap at pos: %x", pos);
			break;
		}

		if (cap.cap_vndr != PCI_CAP_ID_VNDR) {
			continue;
		}

		N3K_MGMT_LOG(DEBUG, "read vendor-specific PCI capability");
		N3K_MGMT_LOG(DEBUG, "cap: cfg type=%u", cap.cfg_type);
		N3K_MGMT_LOG(DEBUG, "cap: bar     =%u", cap.bar);
		N3K_MGMT_LOG(DEBUG, "cap: offset  =%u", cap.offset);
		N3K_MGMT_LOG(DEBUG, "cap: length  =%u", cap.length);

		switch (cap.cfg_type) {
		case N3K_MGMT_PCI_CAP_COMMON_CFG:
			hw->common_cfg = get_cap_addr(hw, &cap);
			break;
		case N3K_MGMT_PCI_CAP_NOTIFY_CFG:
			PCI_READ_CONFIG_DWORD(dev, &hw->notify_off_multiplier,
					pos + sizeof(cap));
			hw->notify_base = get_cap_addr(hw, &cap);
			hw->notify_region = cap.bar;
			break;
		case N3K_MGMT_PCI_CAP_ISR_CFG:
			hw->isr = get_cap_addr(hw, &cap);
			break;
		case N3K_MGMT_PCI_CAP_DEVICE_CFG:
			hw->dev_cfg = get_cap_addr(hw, &cap);
			break;
		}
	}

	hw->pci_resource_start = hw->mem_resource[4].addr;

	if (hw->common_cfg == NULL || hw->notify_base == NULL ||
			hw->isr == NULL || hw->dev_cfg == NULL) {
		N3K_MGMT_LOG(ERR, "capability incomplete");
		return -EINVAL;
	}

	// TODO(smartnic): How should PCI capabilities be tackled?
	// These seem wrong according to lspci,
	// but there is no need to fix that to make flows work.
	N3K_MGMT_LOG(DEBUG, "cap mapping: common cfg =%p", hw->common_cfg);
	N3K_MGMT_LOG(DEBUG, "cap mapping: notify base=%p", hw->notify_base);
	N3K_MGMT_LOG(DEBUG, "cap mapping: isr cfg    =%p", hw->isr);
	N3K_MGMT_LOG(DEBUG, "cap mapping: device cfg =%p", hw->dev_cfg);
	N3K_MGMT_LOG(DEBUG, "cap mapping: multiplier =%u", hw->notify_off_multiplier);

	return 0;
}

int
n3k_mgmt_init_hw(struct n3k_mgmt_hw *hw, struct rte_pci_device *dev,
	const struct n3k_mgmt_args *args)
{
	int ret;
	hw->insert_type = args->insert_type;

	ret = configure_pci_capabilities(hw, dev);
	if (ret) {
		N3K_MGMT_LOG(ERR, "configure_pci_capabilities() failed %d", ret);
		return ret;
	}

	ret = n3k_mgmt_init_init(hw, args);
	if (ret) {
		N3K_MGMT_LOG(ERR, "n3k_mgmt_init_init() failed %d", ret);
		return ret;
	}

	return 0;
}
