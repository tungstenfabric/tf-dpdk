/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _N3K_DUMP_UTILS_H_
#define _N3K_DUMP_UTILS_H_

#include <rte_pmd_n3k_flow_tbl_mgmt.h>
#include <rte_ether.h>
#include <stdint.h>

const char *n3k_dump_convert_mac_to_str(char *buff, struct rte_ether_addr addr);
const char *n3k_dump_convert_ip_to_str(char *buff, uint32_t addr);

#endif /* _N3K_DUMP_UTILS_H_ */
