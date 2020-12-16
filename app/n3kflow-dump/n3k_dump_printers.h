/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _N3K_DUMP_PRINTERS_H_
#define _N3K_DUMP_PRINTERS_H_

#include <rte_pmd_n3k_mp.h>

#include <stdbool.h>

void
n3k_dump_print_table_entry(struct n3k_mgmt_mp_entries_response *response,
	enum n3k_mgmt_mp_request_table table);

void
