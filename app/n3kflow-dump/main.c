/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "../../drivers/net/n3k/mgmt/n3k_mgmt_flow_tbl_regs_api.h"
#include "../../drivers/net/n3k/mgmt/n3k_mgmt_tunnel_tbl_regs_api.h"
#include "../../drivers/net/n3k/mgmt/n3k_mgmt_flow_tracker_regs_api.h"

#include "n3k_dump_debug_regs.h"

#include "n3k_dump_cmdline.h"
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

	return 0;
}

static void
prepare_n3k_request(struct rte_mp_msg *msg, const char *req_type, enum n3k_mgmt_mp_request_table table)
{
	struct n3k_mgmt_mp_request n3k_req;

	memset(msg, 0, sizeof(*msg));
	strncpy(msg->name, req_type, RTE_MP_MAX_NAME_LEN - 1);

	n3k_req.table = table;
	memcpy(n3k_req.device_name, n3k_dump_options.mgmt_device_name, N3K_MGMT_MP_DEVICE_MAX_LEN);

	msg->len_param = sizeof(n3k_req);
	memcpy(msg->param, &n3k_req, msg->len_param);
}

static void
prepare_n3k_total_entries_request(struct rte_mp_msg *msg,
	struct rte_mp_reply *response,
	struct timespec *timespec,
	const uint64_t timeout_secs,
	const char *req_type,
	enum n3k_mgmt_mp_request_table table)
{
	prepare_n3k_request(msg, req_type, table);

	memset(response, 0, sizeof(*response));

	memset(timespec, 0, sizeof(*timespec));
	timespec->tv_sec = timeout_secs;
}

static void
prepare_n3k_register_request(struct rte_mp_msg *msg,
	struct rte_mp_reply *response,
	struct timespec *timespec,
	const uint64_t timeout_secs,
	uint64_t base,
	uint64_t offset)
{
	struct n3k_mgmt_mp_register_request n3k_req;

	memset(msg, 0, sizeof(*msg));
	strncpy(msg->name, N3K_MGMT_MP_REQUEST_REGISTER, RTE_MP_MAX_NAME_LEN);

	n3k_req.base = base;
	n3k_req.offset = offset;
	memcpy(n3k_req.device_name, n3k_dump_options.mgmt_device_name, N3K_MGMT_MP_DEVICE_MAX_LEN);

	msg->len_param = sizeof(n3k_req);
	memcpy(msg->param, &n3k_req, msg->len_param);

	memset(response, 0, sizeof(*response));

	memset(timespec, 0, sizeof(*timespec));
	timespec->tv_sec = timeout_secs;
}

static int
n3k_dump_get_register_value(size_t base_module, size_t offset, uint32_t *reg_value)
{
	struct rte_mp_msg msg;
	struct rte_mp_reply response;
	struct timespec timespec;

	struct rte_mp_msg *rte_response;
	struct n3k_mgmt_mp_dump_register_response *n3k_response;

	int ret;

	prepare_n3k_register_request(&msg, &response, &timespec,
		N3K_DUMP_DEFAULT_TIMEOUT, base_module, offset);

	ret = rte_mp_request_sync(&msg, &response, &timespec);
	if (ret < 0) {
		N3K_DUMP_LOG(ERR,
			"sending synchronous multi-process msg failed; rte_errno = %d",
			rte_errno);
		return ret;
	}

	if (response.nb_sent != 1 || response.nb_received != 1) {
		N3K_DUMP_LOG(ERR,
			"rte_mp_reply problem; nb_sent = %d, nb_received = %d",
			response.nb_sent, response.nb_received);
		return ret;
	}

	rte_response = &response.msgs[0];
	n3k_response = (struct n3k_mgmt_mp_dump_register_response *)(&rte_response->param[0]);
	*reg_value = n3k_response->reg_value;
	free(response.msgs);
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

	printf(
		"register(0x%04" PRIX64 ".0x%04" PRIX64 ") = %" PRIu32 "\n",
		read_addr >> 16, read_addr & 0xFFFF, reg_value);
	fflush(NULL);
}

static void
n3k_dump_handle_request_debug_registers(void)
{
	struct regs_array_description {
		size_t base;
		size_t beg_offset;
		size_t num;
		const char *base_name;
	};
	static const struct regs_array_description debug_reg_arrays[] = {
		{
			.base = MKS_CHIP_LKUP_BASE,
			.beg_offset = LKUP_EMLC_KEY_0,
			.num = sizeof(struct lookup_emlc_key_regs) / 4,
			.base_name = "EMLC_KEY",
		},
		{
			.base = MKS_CHIP_LKUP_BASE,
			.beg_offset = LKUP_EMLC_RES_0,
			.num = sizeof(struct lookup_emlc_res_regs) / 4,
			.base_name = "EMLC_RES",
		},
		{
			.base = MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE,
			.beg_offset = MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_0,
			.num = sizeof(struct tunnel_tbl_data_regs) / 4,
			.base_name = "TUNNEL_TBL_DATA",
		},
		{
			.base = MKS_CHIP_MKS_FLOW_TRACKER_BASE,
			.beg_offset = MKS_FLOW_TRACKER_FT_DATA_0,
			.num = sizeof(struct flow_tracker_data_regs) / 4,
			.base_name = "FT_DATA",
		},
	};

	uint32_t i;
	int ret;

	for (i = 0; i < debug_regs_size; ++i) {
		uint32_t reg_value;
		ret = n3k_dump_get_register_value(debug_regs[i].base,
			debug_regs[i].offset, &reg_value);
		if (ret < 0) {
			printf("Couldn't get register value for %s\n", debug_regs[i].name);
			return;
		}
		printf("%s\t= 0x%08" PRIx32 " (%" PRIu32 ")\n", debug_regs[i].name,
			reg_value, reg_value);
	}

	printf("\n");

	for (i = 0; i < RTE_DIM(debug_reg_arrays); ++i) {
		uint32_t j;
		for (j = 0; j < debug_reg_arrays[i].num; ++j) {
			uint32_t reg_value;
			ret = n3k_dump_get_register_value(
				debug_reg_arrays[i].base,
				debug_reg_arrays[i].beg_offset + 4 * j,
				&reg_value);
			if (ret < 0) {
				printf("Couldn't get register value for %s_%" PRIu32 "\n",
					debug_reg_arrays[i].base_name, j);
				return;
			}
			printf("%s_%" PRIu32 "\t= 0x%08" PRIx32 " (%" PRIu32 ")\n",
				debug_reg_arrays[i].base_name, j,
				reg_value, reg_value);
		}
		printf("\n");
	}

}

static void
n3k_dump_handle_request_total_entries(void)
{
	struct rte_mp_msg msg;
	struct rte_mp_reply response;
	struct timespec timespec;

	struct rte_mp_msg *rte_response;
	struct n3k_mgmt_mp_total_entries_response *n3k_response;

	int ret;

	prepare_n3k_total_entries_request(&msg, &response, &timespec,
		N3K_DUMP_DEFAULT_TIMEOUT, N3K_MGMT_MP_REQUEST_TOTAL_ENTRIES,
		n3k_dump_options.table_type);

	ret = rte_mp_request_sync(&msg, &response, &timespec);
	if (ret < 0) {
		N3K_DUMP_LOG(ERR,
			"sending synchronous multi-process msg failed; rte_errno = %d",
			rte_errno);
		return;
	}

	RTE_ASSERT(response.nb_sent == 1);
	RTE_ASSERT(response.nb_received == 1);

	rte_response = &response.msgs[0];
	n3k_response = (struct n3k_mgmt_mp_total_entries_response *)(&rte_response->param[0]);
	printf("total_entries = %" PRIu64 "\n", n3k_response->total_entries);
	fflush(NULL);

	free(response.msgs);
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

static void
n3k_dump_handle_request_entries(void)
{
	struct rte_mp_msg msg;
	bool more;
	int ret;

	prepare_n3k_request(&msg, N3K_MGMT_MP_REQUEST_ENTRIES,
		n3k_dump_options.table_type);

	ret = rte_mp_sendmsg(&msg);
	if (ret < 0) {
		N3K_DUMP_LOG(ERR,
			"sending multi-process message failed; rte_errno = %d",
			rte_errno);
		return;
	}

	more = true;

	if (n3k_dump_options.json)
		printf("\"%s\": [", n3k_dump_table_type_to_str(
			n3k_dump_options.table_type));

	bool first = true;
	while (more) {
		struct n3k_mgmt_mp_entries_response response;
		int ret = 0;
		ret = n3k_dump_get_response_from_ring(&response);
		if (ret < 0) {
			N3K_DUMP_LOG(ERR, "getting response failed; ret = %d", ret);
			goto print_json_end;
		}
		if (response.end) {
			more = false;
		} else if (!n3k_dump_options.json) {
			n3k_dump_print_table_entry(&response,
				n3k_dump_options.table_type);
		} else {
			if (!first)
				printf(",");
			printf("\n  ");
			n3k_dump_print_table_entry_json(&response,
				n3k_dump_options.table_type);
		}
		first = false;
	}

print_json_end:
	if (n3k_dump_options.json)
		printf("\n ]\n");
}

static void
n3k_dump_cleanup(void)
{
	rte_eal_cleanup();
}

static void
show_firmware_version(bool output_in_json) {
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

	if(output_in_json) {
		printf("\"FPGA_BUILD_VER\": { \"version\": %x, \"revision\": %x, \"milestone\": %x, \"HCL\": %s, \"OVS\": %x },\n",
				build_version.version, build_version.revision, build_version.milestone,
				build_version.hcl_id == 0x0C ? "true" : "false", build_version.ovs);

		printf("\"FPGA_BUILD_DATE\": { \"day\": %x, \"month\": %x, \"year\": %x },\n",
				build_date.day, build_date.month, build_date.year);
	} else {
		printf("FIRMWARE_VERSION: v%02X.r%02X M%02X\n", build_version.version, build_version.revision, build_version.milestone);
		printf("FIRMWARE_DATE: %02X-%02X-%04X\n", build_date.day, build_date.month, build_date.year);
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

	if (n3k_dump_options.json)
		printf("JSON_DUMP:\n{\n");

	if (n3k_dump_options.show_version) {
		show_firmware_version(n3k_dump_options.json);
	}

	switch (n3k_dump_options.action_type) {
	case N3K_DUMP_REQUEST_DEBUG_REGISTERS:
		n3k_dump_handle_request_debug_registers();
		break;
	case N3K_DUMP_REQUEST_REGISTER:
		n3k_dump_handle_request_register();
		break;
	case N3K_DUMP_REQUEST_TABLE:
		if (!n3k_dump_options.json)
			n3k_dump_handle_request_total_entries();
		if (!n3k_dump_options.only_entries_cnt)
			n3k_dump_handle_request_entries();
		break;
	}

	if (n3k_dump_options.json)
		printf("}\n");

