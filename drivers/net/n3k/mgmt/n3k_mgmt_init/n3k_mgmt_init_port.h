/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _N3K_MGMT_INIT_PORT_
#define _N3K_MGMT_INIT_PORT_

#include "../n3k_mgmt_osdep.h"
#include "../n3k_mgmt_hw.h"

uint32_t csr_read(struct n3k_mgmt_hw *hw, size_t base_module, size_t offset);
void csr_write(struct n3k_mgmt_hw *hw, size_t base_module, size_t offset, uint32_t val);

#endif /* _N3K_MGMT_INIT_PORT_ */
