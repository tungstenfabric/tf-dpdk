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
#include <rte_pmd_n3k_malloc.h>

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

int
n3k_mgmt_init_hw(struct n3k_mgmt_hw *hw, struct rte_pci_device *dev __rte_unused,
	const struct n3k_mgmt_args *args)
{
	int ret;
	hw->insert_type = args->insert_type;
	hw->pci_resource_start = hw->mem_resource[4].addr;

	ret = n3k_mgmt_init_init(hw, args);
	if (ret) {
		N3K_MGMT_LOG(DEFAULT, ERR, "n3k_mgmt_init_init() failed %d", ret);
		return ret;
	}

	return 0;
}
