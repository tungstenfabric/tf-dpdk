/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_ARGS_H_
#define _PMD_NET_N3K_MGMT_ARGS_H_

#include "../n3k.h"
#include "flr_sim/n3k_mgmt_flr_sim_queues_mgmt.h"
#include "n3k_mgmt_hw.h"

#include <stdbool.h>
#include <stdint.h>

#include <rte_bus_pci.h>

struct n3k_mgmt_args {
	uint16_t pf_mqs;
	uint16_t mqs_param_count;
	uint16_t vf_mqs[N3K_VF_PORTS_MAX_COUNT];
	enum n3k_mgmt_hw_insert_type insert_type;
	struct n3k_mgmt_flr_sim_queues_sim_params flr_sim_queues_params;
};

int
n3k_mgmt_args_parse(struct rte_pci_device *dev, struct n3k_mgmt_args *args);

#endif /* _PMD_NET_N3K_MGMT_ARGS_H_ */
