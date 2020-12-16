/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (c) 2019, Intel Corporation. */
#ifndef _IFC_PORT_
#define _IFC_PORT_

#include "../n3k_mgmt_osdep.h"
#include "../n3k_mgmt_hw.h"

uint32_t csr_read(struct n3k_mgmt_hw *hw, size_t base_module, size_t offset);
void csr_write(struct n3k_mgmt_hw *hw, size_t base_module, size_t offset, uint32_t val);

#endif /* _IFC_PORT_ */
