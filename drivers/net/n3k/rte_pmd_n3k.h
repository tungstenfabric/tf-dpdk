/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _RTE_PMD_N3K_H_
#define _RTE_PMD_N3K_H_

#include <stdint.h>

int rte_pmd_n3k_get_repr_id_by_port_id(uint16_t port_id, uint16_t *repr_id);
int rte_pmd_n3k_get_vdpa_did_by_repr_name(const char *name);

#endif /* _RTE_PMD_N3K_H_ */
