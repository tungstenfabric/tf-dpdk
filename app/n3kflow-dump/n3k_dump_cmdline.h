/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _N3K_DUMP_CMDLINE_H_
#define _N3K_DUMP_CMDLINE_H_

#include <rte_pmd_n3k_mp.h>

#include <getopt.h>
#include <stdbool.h>

enum n3k_dump_action_type {
	N3K_DUMP_REQUEST_TABLE,
	N3K_DUMP_REQUEST_REGISTER,
	N3K_DUMP_REQUEST_DEBUG_REGISTERS,
	N3K_DUMP_REQUEST_VF0_STATS,
};

struct n3k_dump_options {
	enum n3k_dump_action_type action_type;
	uint64_t base, offset;
	bool only_entries_cnt;
	bool json;
	enum n3k_mgmt_mp_request_table table_type;
	char mgmt_device_name[N3K_MGMT_MP_DEVICE_MAX_LEN + 1];
	bool show_version;
};

int
n3k_dump_parse_cmdline_options(int argc, char **argv, struct n3k_dump_options *opts);

#endif /* _N3K_DUMP_CMDLINE_H_ */
