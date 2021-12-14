/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _N3K_MGMT_INFO_H_
#define _N3K_MGMT_INFO_H_

#include <rte_ethdev.h>

int n3k_mgmt_info_fw_version_get(struct rte_eth_dev *dev,
	char *fw_version, size_t fw_size);

#endif // _N3K_MGMT_INFO_H_
