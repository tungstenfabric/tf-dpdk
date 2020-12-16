/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "../../rte_pmd_n3k_malloc.h"
#include "../n3k_mgmt_args.h"
#include "../n3k_mgmt_hw.h"
#include "../n3k_mgmt_log.h"
#include "n3k_mgmt_flr_sim_init.h"
#include "n3k_mgmt_flr_sim_queues_mgmt.h"


int
n3k_mgmt_flr_sim_init(struct n3k_mgmt_hw *hw, const struct n3k_mgmt_args *args)
{
	int ret;

	ret = n3k_mgmt_flr_sim_queues_mgmt_init(hw, &args->flr_sim_queues_params);
	if (ret) {
		N3K_MGMT_FLR_SIM_LOG(ERR,
			"n3k_mgmt_flr_sim_queues_mgmt_init failed. ret = %d",
			ret);
		return ret;
	}
