/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (c) 2018, Intel Corporation. */

#ifndef _IFC_API_LAG_CALLBACK_H_
#define _IFC_API_LAG_CALLBACK_H_

#include "../n3k_mgmt_hw.h"

void lag_enable(struct n3k_mgmt_hw *hw);
void lag_disable(struct n3k_mgmt_hw *hw);

#endif /* _IFC_API_LAG_CALLBACK_H_ */
