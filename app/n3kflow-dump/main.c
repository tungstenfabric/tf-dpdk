/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "../../drivers/net/n3k/mgmt/regs/n3k_mgmt_regs.h"

#include "n3k_dump_debug_regs.h"

#include "n3k_dump_cmdline.h"
#include "n3k_dump_json.h"
#include "n3k_dump_log.h"
#include "n3k_dump_printers.h"
#include "n3k_dump_utils.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <getopt.h>

#include <rte_common.h>
#include <rte_debug.h>
#include <rte_eal.h>
#include <rte_errno.h>
#include <rte_malloc.h>
#include <rte_mempool.h>
#include <rte_ring.h>
#include <rte_log.h>

#include <rte_pmd_n3k_flow_tbl_mgmt.h>
#include <rte_pmd_n3k_mp.h>
#include <rte_pmd_n3k_tunnel_tbl_mgmt.h>

#define N3K_DUMP_DEFAULT_TIMEOUT 60
#define N3K_DUMP_DEVICE_NAME_MAX_LEN N3K_MGMT_MP_DEVICE_MAX_LEN

static struct n3k_dump_options n3k_dump_options;

static struct rte_mempool *n3k_dump_entries_pool = NULL;
static struct rte_ring *n3k_dump_entries_ring = NULL;

/* Function used only with JSON output, if there's no json library
 * it will be unused, that's why declaration and attribute is used */
static const char*
n3k_dump_table_type_to_str(enum n3k_mgmt_mp_request_table table)
	__attribute__((unused));

static void
dequeue_old_shared_ring_entries(void)
{
	int ret;
	while (true) {
		void *obj;
		int tries = 100;
		while (tries--) {
			ret = rte_ring_dequeue(n3k_dump_entries_ring, &obj);
			if (ret == 0)
				break;
			rte_delay_ms(1);
		}
		if (ret != 0)
			break;
		else
			rte_mempool_put(n3k_dump_entries_pool, obj);
	}
}

static int
n3k_dump_init(int argc, char **argv)
{
	int ret;

	ret = rte_eal_init(argc, argv);
	if (ret < 0) {
		rte_panic("Cannot init EAL");
	}

	argc -= ret;
	argv += ret;

	n3k_dump_logtype = rte_log_register("n3k_dump");

	ret = n3k_dump_parse_cmdline_options(argc, argv, &n3k_dump_options);
	if (ret < 0)
		return ret;

	if (rte_eal_process_type() != RTE_PROC_SECONDARY) {
		N3K_DUMP_LOG(ERR,
			"not running as secondary DPDK process; please use '--proc-type secondary' in command line arguments");
		rte_panic("Not running as secondary DPDK process.");
	}

	n3k_dump_entries_pool = rte_mempool_lookup(N3K_MGMT_MP_MEMPOOL_NAME);
	if (n3k_dump_entries_pool == NULL) {
		N3K_DUMP_LOG(ERR,
			"could not lookup n3k_mp mempool; is primary process with n3k driver running?");
		return -1;
	}

	n3k_dump_entries_ring = rte_ring_lookup(N3K_MGMT_MP_RING_NAME);
	if (n3k_dump_entries_ring == NULL) {
		N3K_DUMP_LOG(ERR,
			"could not lookup n3k_mp ring; is primary process with n3k driver running?");
		return -1;
	}

	dequeue_old_shared_ring_entries();

	return 0;
}

static int
n3k_default_request_sync(struct rte_mp_msg *req, struct rte_mp_msg *resp)
{
	struct rte_mp_reply reply;
	struct timespec timespec;
	int ret;

	memset(&timespec, 0, sizeof(timespec));
	timespec.tv_sec = N3K_DUMP_DEFAULT_TIMEOUT;
	memset(&reply, 0, sizeof(reply));
	ret = rte_mp_request_sync(req, &reply, &timespec);
	if (ret < 0) {
		N3K_DUMP_LOG(ERR,
			"sending synchronous multi-process msg failed; rte_errno = %d",
			rte_errno);
		return ret;
	}

	if (reply.nb_sent != 1 || reply.nb_received != 1) {
		N3K_DUMP_LOG(ERR,
			"rte_mp_reply problem; nb_sent = %d, nb_received = %d",
			reply.nb_sent, reply.nb_received);
		return ret;
	}

	memcpy(resp, &reply.msgs[0], sizeof(*resp));
	free(reply.msgs);

	return 0;
}

static void
init_n3k_mp_request(struct n3k_mgmt_mp_request *n3k_req)
{
	snprintf(n3k_req->device_name, RTE_ETH_NAME_MAX_LEN, "%s",
		n3k_dump_options.mgmt_device_name);
}

static void
init_mp_msg(struct rte_mp_msg *msg, const char *req_name,
	const struct n3k_mgmt_mp_request *n3k_req)
{
	memset(msg, 0, sizeof(*msg));
	strncpy(msg->name, req_name, RTE_MP_MAX_NAME_LEN - 1);
	msg->len_param = sizeof(*n3k_req);
	memcpy(msg->param, n3k_req, msg->len_param);
}

static void
prepare_n3k_register_request(struct rte_mp_msg *msg, uint64_t base,
	uint64_t offset)
{
	struct n3k_mgmt_mp_request n3k_req;
	init_n3k_mp_request(&n3k_req);
	n3k_req.register_req.base = base;
	n3k_req.register_req.offset = offset;
	init_mp_msg(msg, N3K_MGMT_MP_REQUEST_REGISTER, &n3k_req);
}

static int
n3k_dump_get_register_value(size_t base_module, size_t offset, uint32_t *reg_value)
{
	struct rte_mp_msg req, resp;
	struct n3k_mgmt_mp_register_response reg_resp;

	int ret;

	prepare_n3k_register_request(&req, base_module, offset);

	ret = n3k_default_request_sync(&req, &resp);
	if (ret < 0) {
		N3K_DUMP_LOG(ERR, "failed to execute request; ret=%d", ret);
		return ret;
	}

	memcpy(&reg_resp, resp.param, sizeof(reg_resp));
	*reg_value = reg_resp.reg_value;
	return 0;
}

static void
n3k_dump_handle_request_register(void)
{
	size_t base_module = n3k_dump_options.base;
	size_t offset = n3k_dump_options.offset;
	size_t read_addr = base_module + offset;
	uint32_t reg_value;
	int ret;

	ret = n3k_dump_get_register_value(base_module, offset, &reg_value);
	if (ret < 0)
		return;

	printf("register(0x%04" PRIX64 ".0x%04" PRIX64 ") = %" PRIu32 "\n",
		read_addr >> 16, read_addr & 0xFFFF, reg_value);
	fflush(NULL);
}

static void
n3k_dump_handle_request_debug_registers(void)
{
	const struct regs_array_description *regs_arr;
	const struct regs_description *regs;
	size_t size;
	uint32_t i;
	int ret;

	get_debug_regs_desc(&regs, &size);
	for (i = 0; i < size; ++i) {
		uint32_t reg_value;
		ret = n3k_dump_get_register_value(regs[i].base,
			regs[i].offset, &reg_value);
		if (ret < 0) {
			printf("Couldn't get register value for %s\n", regs[i].name);
			return;
		}
		printf("%s\t= 0x%08" PRIx32 " (%" PRIu32 ")\n", regs[i].name,
			reg_value, reg_value);
	}

	printf("\n");

	get_debug_regs_array_desc(&regs_arr, &size);
	for (i = 0; i < size; ++i) {
		uint32_t j;
		for (j = 0; j < regs_arr[i].num; ++j) {
			uint32_t reg_value;
			ret = n3k_dump_get_register_value(
				regs_arr[i].base,
				regs_arr[i].beg_offset + 4 * j,
				&reg_value);
			if (ret < 0) {
				printf("Couldn't get register value for %s_%" PRIu32 "\n",
					regs_arr[i].base_name, j);
				return;
			}
			printf("%s_%" PRIu32 "\t= 0x%08" PRIx32 " (%" PRIu32 ")\n",
				regs_arr[i].base_name, j,
				reg_value, reg_value);
		}
		printf("\n");
	}
}

static void
prepare_n3k_vf0_stats_request(struct rte_mp_msg *msg)
{
	struct n3k_mgmt_mp_request n3k_req;
	init_n3k_mp_request(&n3k_req);
	init_mp_msg(msg, N3K_MGMT_MP_REQUEST_VF0_STATS, &n3k_req);
}

static void
n3k_dump_handle_request_vf0_stats(void)
{
	struct rte_mp_msg req, resp;
	struct n3k_mgmt_mp_vf0_stats_response stats_resp;
	int ret;

	prepare_n3k_vf0_stats_request(&req);

	ret = n3k_default_request_sync(&req, &resp);
	if (ret < 0) {
		N3K_DUMP_LOG(ERR, "failed to execute request; ret=%d", ret);
		return;
	}

	memcpy(&stats_resp, resp.param, sizeof(stats_resp));

#ifndef N3K_DUMP_NO_JSON
	if (n3k_dump_options.json) {
		printf(" ");
		n3k_dump_mp_json_vf0_stats_dump(&stats_resp);
		printf("\n");
	} else
#endif /* N3K_DUMP_NO_JSON */
		rte_pmd_n3k_mp_vf0_stats_dump(&stats_resp);
}

static void
prepare_n3k_entries_request(struct rte_mp_msg *msg,
	enum n3k_mgmt_mp_request_table table, bool total_entries)
{
	struct n3k_mgmt_mp_request n3k_req;
	init_n3k_mp_request(&n3k_req);
	n3k_req.entries_req.table = table;
	init_mp_msg(msg, total_entries ?
		N3K_MGMT_MP_REQUEST_TOTAL_ENTRIES : N3K_MGMT_MP_REQUEST_ENTRIES,
		&n3k_req);
}

static unsigned
n3k_dump_handle_request_total_entries(void)
{
	struct rte_mp_msg req, resp;
	struct n3k_mgmt_mp_total_entries_response total_entries;
	int ret;

	prepare_n3k_entries_request(&req, n3k_dump_options.table_type, true);

	ret = n3k_default_request_sync(&req, &resp);
	if (ret < 0) {
		N3K_DUMP_LOG(ERR, "failed to execute request; ret=%d", ret);
		return 0;
	}

	memcpy(&total_entries, resp.param, sizeof(total_entries));

	if (!n3k_dump_options.json) {
		printf("total_entries = %" PRIu64 "\n", total_entries.total_entries);
		fflush(NULL);
	}

	return (unsigned)total_entries.total_entries;
}

static int
n3k_dump_get_response_from_ring(struct n3k_mgmt_mp_entries_response *response)
{
	int ret;
	void *obj = NULL;

	int tries = 100;
	while (tries--) {
		ret = rte_ring_dequeue(n3k_dump_entries_ring, &obj);
		if (ret == 0) {
			memcpy(response, obj, sizeof(*response));
			rte_mempool_put(n3k_dump_entries_pool, obj);
			return 0;
		}
		rte_delay_ms(10);
	}
	return ret;
}

static const char*
n3k_dump_table_type_to_str(enum n3k_mgmt_mp_request_table table)
{
	switch (table) {
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW:
		return "flows";
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW_RAW:
		return "flows-raw";
	case N3K_MGMT_MP_REQUEST_TABLE_TUNNEL:
		return "tunnels";
	case N3K_MGMT_MP_REQUEST_TABLE_TUNNEL_RAW:
		return "tunnels-raw";
	case N3K_MGMT_MP_REQUEST_TABLE_SMAC:
		return "smacs";
	case N3K_MGMT_MP_REQUEST_TABLE_VPLKP:
		return "vplkp-entries";
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW_TRACKER:
		return "flow-stats";
	case N3K_MGMT_MP_REQUEST_TABLE_PORT:
		return "ports";
	}
	return "UNKNOWN-TABLE-TYPE";
}

static unsigned
n3k_dump_handle_request_entries(void)
{
	struct rte_mp_msg msg;
	bool more;
	int ret;
	unsigned cnt = 0;

	prepare_n3k_entries_request(&msg, n3k_dump_options.table_type, false);

	ret = rte_mp_sendmsg(&msg);
	if (ret < 0) {
		N3K_DUMP_LOG(ERR,
			"sending multi-process message failed; rte_errno = %d",
			rte_errno);
		return 0;
	}

	more = true;

#ifndef N3K_DUMP_NO_JSON
	if (n3k_dump_options.json)
		printf(" \"%s\": [", n3k_dump_table_type_to_str(
			n3k_dump_options.table_type));
#endif /* N3K_DUMP_NO_JSON */

	/* Variable used only with JSON output, if there's no json library
	 * it will be unused */
	bool first __attribute__((unused)) = true;
	while (more) {
		struct n3k_mgmt_mp_entries_response response;
		int ret = 0;
		ret = n3k_dump_get_response_from_ring(&response);
		if (ret < 0) {
			N3K_DUMP_LOG(ERR, "getting response failed; ret = %d", ret);
			break;
		}
		if (response.end) {
			more = false;
#ifndef N3K_DUMP_NO_JSON
		} else if (n3k_dump_options.json) {
			if (!first)
				printf(",");
			printf("\n  ");
			n3k_dump_print_table_entry_json(&response,
				n3k_dump_options.table_type);
			++cnt;
#endif /* N3K_DUMP_NO_JSON */
		} else {
			n3k_dump_print_table_entry(&response,
				n3k_dump_options.table_type);
		}
		first = false;
	}

#ifndef N3K_DUMP_NO_JSON
	if (n3k_dump_options.json)
		printf("\n ]\n");
#endif /* N3K_DUMP_NO_JSON */

	return cnt;
}

static void
n3k_dump_cleanup(void)
{
	rte_eal_cleanup();
}

static void
show_firmware_version(void) {
	union {
		uint32_t raw;
		struct {
			uint8_t revision;
			uint8_t version;
			uint8_t milestone;
			uint8_t ovs:4;
			uint8_t hcl_id:4;
		};
	} build_version;

	union {
		uint32_t raw;
		struct {
			uint8_t day;
			uint8_t month;
			uint16_t year;
		};
	} build_date;

	int ret = n3k_dump_get_register_value(MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE,
		MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_VERSION_REDU, &build_version.raw);
	if (ret != 0) {
		printf("Error reading version register!\n");
		return;
	}

	ret = n3k_dump_get_register_value(MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE,
		MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_CMPL_DATE_REDU, &build_date.raw);
	if (ret != 0) {
		printf("Error reading build date register!\n");
		return;
	}

#ifndef N3K_DUMP_NO_JSON
	if (n3k_dump_options.json) {
		printf(" \"FPGA_BUILD_VER\": { \"version\": \"%x\", \"revision\": \"%x\", \"milestone\": \"%x\", \"HCL\": \"%s\", \"OVS\": \"%x\" },\n",
				build_version.version, build_version.revision, build_version.milestone,
				build_version.hcl_id == 0x0C ? "true" : "false", build_version.ovs);

		printf(" \"FPGA_BUILD_DATE\": { \"day\": %hhu, \"month\": %hhu, \"year\": %hu },\n",
				build_date.day, build_date.month, build_date.year);
	} else {
#endif /* N3K_DUMP_NO_JSON */
		printf("FIRMWARE_VERSION: v%02X.r%02X M%02X\n", build_version.version, build_version.revision, build_version.milestone);
		printf("FIRMWARE_DATE: %02X-%02X-%04X\n", build_date.day, build_date.month, build_date.year);
#ifndef N3K_DUMP_NO_JSON
	}
#endif /* N3K_DUMP_NO_JSON */
}

#ifndef N3K_DUMP_NO_JSON
static void
n3k_dump_handle_request_json(void) {
	printf("JSON_DUMP:\n{\n");

	if (n3k_dump_options.show_version)
		show_firmware_version();

	switch (n3k_dump_options.action_type) {
	case N3K_DUMP_REQUEST_TABLE:
		if (!n3k_dump_options.only_entries_cnt) {
			unsigned total_entries = n3k_dump_handle_request_total_entries();
			unsigned total_flows = n3k_dump_handle_request_entries();
			unsigned total_mirror_entries = total_entries - total_flows;
            printf(", \"entry_count\": { \"total\": %u, \"flows\": %u, \"mirror\": %u}",
                total_entries, total_flows, total_mirror_entries);
        }
		break;
	case N3K_DUMP_REQUEST_VF0_STATS:
		n3k_dump_handle_request_vf0_stats();
		break;
	case N3K_DUMP_REQUEST_DEBUG_REGISTERS:
	case N3K_DUMP_REQUEST_REGISTER:
		N3K_DUMP_LOG(WARNING, "Dumping not supported");
		break;
	}

	printf("}\n");
}
#endif /* N3K_DUMP_NO_JSON */

static void
n3k_dump_handle_request_human_readable(void) {
	if (n3k_dump_options.show_version)
		show_firmware_version();

	switch (n3k_dump_options.action_type) {
	case N3K_DUMP_REQUEST_TABLE:
		n3k_dump_handle_request_total_entries();
		if (!n3k_dump_options.only_entries_cnt)
			n3k_dump_handle_request_entries();
		break;
	case N3K_DUMP_REQUEST_VF0_STATS:
		n3k_dump_handle_request_vf0_stats();
		break;
	case N3K_DUMP_REQUEST_DEBUG_REGISTERS:
		n3k_dump_handle_request_debug_registers();
		break;
	case N3K_DUMP_REQUEST_REGISTER:
		n3k_dump_handle_request_register();
		break;
	}
}

int
main(int argc, char **argv)
{
	int ret = 0;

	ret = n3k_dump_init(argc, argv);
	if (ret < 0) {
		N3K_DUMP_LOG(ERR, "app initialization failed");
		goto cleanup;
	}

#ifndef N3K_DUMP_NO_JSON
	if (n3k_dump_options.json)
		n3k_dump_handle_request_json();
	else
#endif /* N3K_DUMP_NO_JSON */
		n3k_dump_handle_request_human_readable();

cleanup:
	n3k_dump_cleanup();
	return ret;
}
