/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <getopt.h>

#include <rte_eal.h>
#include <rte_ethdev.h>

#define SHOW_FIRMWARE_VERSION_STR "show-firmware-version"
#define DEVICE_NAME_STR "device-name"
#define RTE_LOGTYPE_N3KINFO RTE_LOGTYPE_USER1

static struct option n3k_info_reader_long_options[] = {
	{
		.name = DEVICE_NAME_STR,
		.has_arg = required_argument,
		.flag = NULL,
		.val = 0,
	},
	{
		.name = SHOW_FIRMWARE_VERSION_STR,
		.has_arg = no_argument,
		.flag = NULL,
		.val = 0,
	},
	{ 0 },
};

static const char *n3k_info_reader_short_options = "";

struct n3k_info_reader_opts {
	bool show_version;
	char mgmt_device_name[RTE_ETH_NAME_MAX_LEN];
};

static const char *n3k_info_reader_usage_msg[] = {
	"Usage: n3k-info <EAL parameters> -- <--" DEVICE_NAME_STR " name> "
		"[--" SHOW_FIRMWARE_VERSION_STR "]",
	"\tOptions:",
	"\t--" DEVICE_NAME_STR "\tdump info for given device",
	"\t--" SHOW_FIRMWARE_VERSION_STR "\tshows firmware version",
	NULL,
};

static void
print_usage(void)
{
	const char **p;
	const char *msg;
	for (p = n3k_info_reader_usage_msg, msg = *p; msg != NULL; ++p, msg = *p) {
		printf("%s\n", msg);
		fflush(NULL);
	}
}

static void
parse_cmdline_options(
	int argc,
	char **argv,
	struct n3k_info_reader_opts *opts)
{
	int opt_idx;
	int ret = -1;

	memset(opts, 0, sizeof(*opts));

	while ((ret = getopt_long(argc, argv, n3k_info_reader_short_options,
		&n3k_info_reader_long_options[0], &opt_idx)) != -1) {
		switch (ret) {
		case 0:
			if (!strcmp(n3k_info_reader_long_options[opt_idx].name, DEVICE_NAME_STR)) {
				size_t dev_name_len = strlen(optarg);
				if (dev_name_len > sizeof(opts->mgmt_device_name)) {
					rte_exit(EXIT_FAILURE, "invalid device name length %zu",
						dev_name_len);
				}
				strncpy(opts->mgmt_device_name, optarg, sizeof(opts->mgmt_device_name) - 1);
			}
			if (!strcmp(n3k_info_reader_long_options[opt_idx].name, SHOW_FIRMWARE_VERSION_STR)) {
				opts->show_version = true;
			}
			break;
		case '?':
			print_usage();
			rte_exit(EXIT_SUCCESS, "unknown options");
			break;
		default:
			break;
		}
	}
}

int
main(int argc, char **argv)
{
	struct n3k_info_reader_opts opts;
	uint16_t port_id = (uint16_t)-1;
	char firmware_version[RTE_ETH_NAME_MAX_LEN];
	int ret = rte_eal_init(argc, argv);
	if (ret < 0) {
		rte_exit(EXIT_FAILURE, "app initialization failed");
	}

	argc -= ret;
	argv += ret;

	parse_cmdline_options(argc, argv, &opts);

	ret = rte_eth_dev_get_port_by_name(opts.mgmt_device_name, &port_id);
	if (ret < 0) {
		rte_exit(EXIT_FAILURE, "device with name: %s not found", opts.mgmt_device_name);
	}

	if (opts.show_version) {
		rte_eth_dev_fw_version_get(port_id, firmware_version, sizeof(firmware_version));
	}

	rte_eal_cleanup();

	return 0;
}
