/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_dump_cmdline.h"
#include "n3k_dump_log.h"
#include <rte_pmd_n3k_mp.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define DEVICE_NAME_STR "device-name"
#define TABLE_TYPE_STR "table-type"
#define DUMP_REGISTER_STR  "dump-register"
#define BASE_STR  "base"
#define OFFSET_STR "offset"
#define DEBUG_REGS_STR "debug-regs"
#define SHOW_FIRMWARE_VERSION_STR "show-firmware-version"
#define VF0_STATS_STR "vf0-stats"

static struct option n3k_dump_long_options[] = {
	{
		.name = DEVICE_NAME_STR,
		.has_arg = required_argument,
		.flag = NULL,
		.val = 0,
	},
	{
		.name = TABLE_TYPE_STR,
		.has_arg = required_argument,
		.flag = NULL,
		.val = 0,
	},
	{
		.name = DUMP_REGISTER_STR,
		.has_arg = no_argument,
		.flag = NULL,
		.val = 0,
	},
	{
		.name = BASE_STR,
		.has_arg = required_argument,
		.flag = NULL,
		.val = 0,
	},
	{
		.name = OFFSET_STR,
		.has_arg = required_argument,
		.flag = NULL,
		.val = 0,
	},
	{
		.name = DEBUG_REGS_STR,
		.has_arg = no_argument,
		.flag = NULL,
		.val = 0,
	},
	{
		.name = SHOW_FIRMWARE_VERSION_STR,
		.has_arg = no_argument,
		.flag = NULL,
		.val = 0,
	},
	{
		.name = VF0_STATS_STR,
		.has_arg = no_argument,
		.flag = NULL,
		.val = 0,
	},
	{ 0 },
};

static const char *n3k_dump_short_options = "trj";

static const char *n3k_dump_usage_msg[] = {
	"Usage: n3k-dump <EAL parameters> -- <--" DEVICE_NAME_STR " name> [-j] "
		"[--" SHOW_FIRMWARE_VERSION_STR "]",
	"<--" TABLE_TYPE_STR " type> [-t] [-r]",
	"or",
	"<--" DUMP_REGISTER_STR "> <--" BASE_STR
	" base_addr> <--" OFFSET_STR " offset_addr>",
	"or",
	"<--" DEBUG_REGS_STR ">",
	"or",
	"<--" VF0_STATS_STR ">",
	"",
	"\tRequest register mode:",
	"\t--" DUMP_REGISTER_STR "\tselect dump register mode",
	"\t--" BASE_STR "\tdump register at given base address(in hex)",
	"\t--" OFFSET_STR "\tdump register at given offset(in hex)",
	"",
	"\tRequest debug registers mode:",
	"\t--" DEBUG_REGS_STR "\tdump debug registers(in hex)",
	"",
	"\tRequest table mode:",
	"\t--" TABLE_TYPE_STR "\tdump entries for given table (flow, "
	"tunnel, smac, vplkp, flow-tracker, port)",
	"\t-t\tdump only number of entries for given table",
	"\t-r\tdump raw table without referring to other tables",
	"",
	"\tRequest VF0 stats mode:",
	"\t--" VF0_STATS_STR "\tdump vf0 stats (slow path, flr)",
	"",
	"\tOther options:",
	"\t--" DEVICE_NAME_STR "\tdump info for given device",
	"\t--" SHOW_FIRMWARE_VERSION_STR "\tshows firmware version",
#ifndef N3K_DUMP_NO_JSON
	"\t-j\tdump in json format",
#endif /* N3K_DUMP_NO_JSON */
	NULL,
};

static void
n3k_dump_print_usage(void)
{
	const char **p;
	const char *msg;
	for (p = n3k_dump_usage_msg, msg = *p; msg != NULL; ++p, msg = *p) {
		printf("%s\n", msg);
		fflush(NULL);
	}
}

struct table_name_to_table_type {
	const char *name;
	enum n3k_mgmt_mp_request_table table_type;
};

static struct table_name_to_table_type table_name_to_table_type[] = {
	{
		.name = "flow",
		.table_type = N3K_MGMT_MP_REQUEST_TABLE_FLOW,
	},
	{
		.name = "tunnel",
		.table_type = N3K_MGMT_MP_REQUEST_TABLE_TUNNEL,
	},
	{
		.name = "smac",
		.table_type = N3K_MGMT_MP_REQUEST_TABLE_SMAC,
	},
	{
		.name = "vplkp",
		.table_type = N3K_MGMT_MP_REQUEST_TABLE_VPLKP,
	},
	{
		.name = "flow-tracker",
		.table_type = N3K_MGMT_MP_REQUEST_TABLE_FLOW_TRACKER,
	},
	{
		.name = "port",
		.table_type = N3K_MGMT_MP_REQUEST_TABLE_PORT,
	},
};

static int
n3k_dump_get_table_type(enum n3k_mgmt_mp_request_table *table_type,
	const char *table_type_str)
{
	uint32_t i;
	bool table_type_set = false;

	for (i = 0; i < RTE_DIM(table_name_to_table_type); ++i) {
		if (strcmp(table_name_to_table_type[i].name, table_type_str) == 0) {
			*table_type = table_name_to_table_type[i].table_type;
			table_type_set = true;
			break;
		}
	}
	if (!table_type_set)
		return -EINVAL;

	return 0;
}

int
n3k_dump_parse_cmdline_options(int argc, char **argv, struct n3k_dump_options *opts)
{
	bool set_device_name = false;
	bool dump_register = false;
	bool debug_registers = false;
	bool dump_vf0_stats = false;
	const char *table_name = NULL;
	const char *base_str = NULL;
	const char *offset_str = NULL;
	bool raw = false;
	int opt_idx;
	int ret;

	memset(opts, 0, sizeof(*opts));

	while ((ret = getopt_long(argc, argv, n3k_dump_short_options,
			&n3k_dump_long_options[0], &opt_idx)) != -1) {
		switch (ret) {
		case 't':
			opts->only_entries_cnt = true;
			break;
		case 'r':
			raw = true;
			break;
		case 'j':
			opts->json = true;
#ifdef N3K_DUMP_NO_JSON
			N3K_DUMP_LOG(WARNING, "there's no compiled in JSON support");
#endif /* N3K_DUMP_NO_JSON */
			break;
		case 0:
			if (!strcmp(n3k_dump_long_options[opt_idx].name, DEVICE_NAME_STR)) {
				size_t dev_name_len = strlen(optarg);
				if (dev_name_len > sizeof(opts->mgmt_device_name)) {
					N3K_DUMP_LOG(ERR, "invalid device name length %zu",
						dev_name_len);
					return -EINVAL;
				}
				strncpy(opts->mgmt_device_name, optarg, sizeof(opts->mgmt_device_name) - 1);
				set_device_name = true;
			} else if (!strcmp(n3k_dump_long_options[opt_idx].name, TABLE_TYPE_STR)) {
				table_name = optarg;
			} else if (!strcmp(n3k_dump_long_options[opt_idx].name, BASE_STR)) {
				base_str = optarg;
			} else if (!strcmp(n3k_dump_long_options[opt_idx].name, OFFSET_STR)) {
				offset_str = optarg;
			} else if (!strcmp(n3k_dump_long_options[opt_idx].name, DUMP_REGISTER_STR)) {
				dump_register = true;
			} else if (!strcmp(n3k_dump_long_options[opt_idx].name, DEBUG_REGS_STR)) {
				debug_registers = true;
			} else if (!strcmp(n3k_dump_long_options[opt_idx].name, SHOW_FIRMWARE_VERSION_STR)) {
				opts->show_version = true;
			} else if (!strcmp(n3k_dump_long_options[opt_idx].name, VF0_STATS_STR)) {
				dump_vf0_stats = true;
			}
			break;
		case '?':
			n3k_dump_print_usage();
			return -EINVAL;
		default:
			break;
		}
	}

	if (!set_device_name) {
		N3K_DUMP_LOG(ERR, DEVICE_NAME_STR " is mandatory");
		return -EINVAL;
	}

	if (debug_registers) {
		opts->action_type = N3K_DUMP_REQUEST_DEBUG_REGISTERS;
		return 0;
	} else if (dump_register) {
		if (!base_str) {
			N3K_DUMP_LOG(ERR, "Error base addr not specified");
			return -EINVAL;
		}

		if (!offset_str) {
			N3K_DUMP_LOG(ERR, "Error offset not specified");
			return -EINVAL;
		}

		opts->action_type = N3K_DUMP_REQUEST_REGISTER;
		opts->base = strtoul(base_str, NULL, 16);
		opts->offset = strtoul(offset_str, NULL, 16);

		return 0;
	} else if (dump_vf0_stats) {
		opts->action_type = N3K_DUMP_REQUEST_VF0_STATS;
		return 0;
	}

	opts->action_type = N3K_DUMP_REQUEST_TABLE;
	if (!table_name) {
		N3K_DUMP_LOG(INFO, "Table name not specified. Dumping flow table.");
		opts->table_type = N3K_MGMT_MP_REQUEST_TABLE_FLOW;
	} else {
		ret = n3k_dump_get_table_type(&opts->table_type, table_name);
		if (ret < 0) {
			N3K_DUMP_LOG(ERR, "Error while parsing request type");
			return ret;
		}
	}

	if (raw) {
		if (opts->table_type == N3K_MGMT_MP_REQUEST_TABLE_FLOW)
			opts->table_type = N3K_MGMT_MP_REQUEST_TABLE_FLOW_RAW;
		if (opts->table_type == N3K_MGMT_MP_REQUEST_TABLE_TUNNEL)
			opts->table_type = N3K_MGMT_MP_REQUEST_TABLE_TUNNEL_RAW;
	}

	return 0;
}
