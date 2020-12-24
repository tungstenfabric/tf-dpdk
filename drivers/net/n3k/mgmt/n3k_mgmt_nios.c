/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_nios.h"
#include "n3k_mgmt_nios_shm.h"
#include "n3k_mgmt_log.h"
#include "../rte_pmd_n3k_malloc.h"

#include <stdbool.h>
#include <stdint.h>

#include <rte_malloc.h>
#include <rte_service_component.h>

/*
 * Macros to process N3K_MSG_FREE_HNDL message
 */
#define N3K_START_HANDLE_OFFSET	0
#define N3K_DEL_RULES_COUNT_OFFSET	1
#define N3K_DEL_RULES_DATA_HDR_SIZE	2

#define N3K_NIOS_MSG_BUFF_SIZE_MAX	1536

static void
n3k_mgmt_nios_free_handles(uint32_t* data, uint16_t count)
{
	uint16_t wc;
	uint16_t bc;
	uint32_t start_handle;
	uint32_t rcvd_handles;
	uint32_t processed = 0;
	uint32_t handle;

	start_handle = data[N3K_START_HANDLE_OFFSET];
	rcvd_handles = data[N3K_DEL_RULES_COUNT_OFFSET];
	for (wc = N3K_DEL_RULES_DATA_HDR_SIZE;
	     wc < count && rcvd_handles > processed;
	     wc++) {
		if (data[wc] == 0)
			continue;

		for (bc = 0; bc < 32; bc++) {
			if (data[wc] & (1 << bc)) {
				handle = start_handle + (wc - 2) * 32 + bc;
				N3K_MGMT_NIOS_LOG(DEBUG, "freed handle %" PRIu32, handle);
				processed++;
			}
		}
	}
}

static void
n3k_mgmt_nios_free_recirc_ids(uint32_t* recirc_id, uint32_t count)
{
	uint32_t i;

	for (i = 0; i < count; i++) {
		/* not used */
		N3K_MGMT_NIOS_LOG(DEBUG, "Freeing recirc_id:%u\n", recirc_id[i]);
	}
}

static int32_t
n3k_mgmt_nios_handler_run(void* args)
{
	int rc;
	struct n3k_shm_msg* msg;
	char buff_msg[N3K_NIOS_MSG_BUFF_SIZE_MAX];
	struct n3k_mgmt_hw *hw = args;
	uint16_t len, count;

	len = N3K_NIOS_MSG_BUFF_SIZE_MAX;
	memset(buff_msg, 0, N3K_NIOS_MSG_BUFF_SIZE_MAX);
	msg = (struct n3k_shm_msg*)buff_msg;

	rc = n3k_mgmt_nios_shm_msg_recv(hw, (uint32_t*)msg, &len);
	if (rc) {
		if (rc != -ENOENT){
			N3K_MGMT_NIOS_LOG(
			  ERR,
			  "error while receiving nios shm msg; ret = %d",
			  rc);
		}

		return rc;
	}

	len = len - sizeof(struct n3k_shm_msg_hdr);
	N3K_MGMT_NIOS_LOG(DEBUG,
				"magic:%u msg_type:%u len:%u\n",
				msg->hdr.magic,
				msg->hdr.msg_type,
				msg->hdr.len);

	if (len) {
		count = len / sizeof(uint32_t);
		switch (msg->hdr.msg_type) {
		case N3K_MSG_FREE_RECIRC: {
			N3K_MGMT_NIOS_LOG(DEBUG, "N3K_MSG_FREE_RECIRC");
			n3k_mgmt_nios_free_recirc_ids(msg->data, count);
		} break;
		case N3K_MSG_FREE_HNDL: {
			N3K_MGMT_NIOS_LOG(DEBUG, "N3K_MSG_FREE_HNDL");
			n3k_mgmt_nios_free_handles(msg->data, count);
		} break;
		default:
			N3K_MGMT_NIOS_LOG(DEBUG, "Received invalid message");
			break;
		}
	}

	return 0;
}

static int
n3k_mgmt_nios_setup_service(struct n3k_mgmt_hw *hw) {
	struct rte_service_spec spec;
	int ret;

	snprintf(spec.name, sizeof(spec.name), "%s_nios_handler", "net_n3k0");
	spec.callback = n3k_mgmt_nios_handler_run;
	spec.callback_userdata = hw;
	spec.capabilities = 0;
	spec.socket_id = 0; /* TODO: Set properly */

	uint32_t service_id;

	ret = rte_service_component_register(&spec, &service_id);
	if (ret != 0) {
		N3K_MGMT_NIOS_LOG(ERR, "failed to register component");
		return ret;
	}

	rte_service_component_runstate_set(service_id, 1);

	return 0;
}

int
n3k_mgmt_nios_handler_register_and_run(struct n3k_mgmt_hw *hw)
{
	uint32_t ret;

	if (hw->nios_internal != NULL) {
		return -EINVAL;
	}

	hw->nios_internal = n3k_zmalloc("n3k_mgmt_nios_internal", sizeof(*hw->nios_internal), 0);
	if (hw->nios_internal == NULL) {
		N3K_MGMT_NIOS_LOG(ERR, "failed to allocate memory for n3k_mgmt_nios_shm internal data");
		return -ENOMEM;
	}

	ret = n3k_mgmt_nios_shm_msgq_init(hw);
	if (ret != 0)
		goto exit_cleanup_internal_data;

	ret = n3k_mgmt_nios_setup_service(hw);
	if (ret != 0)
		goto exit_cleanup_internal_data;

	return 0;

exit_cleanup_internal_data:
	n3k_free(hw->nios_internal);
	hw->nios_internal = NULL;
	return ret;
}

void
n3k_mgmt_nios_exit(struct n3k_mgmt_hw *hw) {
	if(hw->nios_internal != NULL) {
		n3k_free(hw->nios_internal);
		hw->nios_internal = NULL;
	}
}
