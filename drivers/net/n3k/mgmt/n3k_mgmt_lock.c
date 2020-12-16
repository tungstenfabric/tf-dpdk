/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_lock.h"
#include "n3k_mgmt_regs.h"
#include "n3k_mgmt_log.h"
#include "ifc/ifc_port.h"

#include <stdint.h>

void
n3k_mgmt_lock(struct n3k_mgmt_hw *hw)
{
	const uint32_t sleep_time = 2;
	uint32_t tries = 0;
	volatile uint32_t lock;

	while (1) {
		lock = csr_read(hw, MKS_CHIP_MKS_LKUP_BASE,
			MKS_LKUP_SEMAPHORE_LOCK0);
		if (!(lock & MKS_LKUP_SEMAPHORE_LOCK0_LOCK_MASK))
			return;

		/* Since masks is same for all lock using 0th mask */
		rte_delay_us_block(sleep_time);
		tries++;
		if (tries % (1000000 / sleep_time) == 0) {
			N3K_MGMT_LOG(WARNING, "Waiting for lock for %uus",
				(tries * sleep_time));
		}
	}
}

void
