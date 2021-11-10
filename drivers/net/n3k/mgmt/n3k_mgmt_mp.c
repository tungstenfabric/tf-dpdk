/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_dump.h"
#include "n3k_mgmt_ethdev.h"
#include "n3k_mgmt_flow_tbl.h"
#include "n3k_mgmt_flow_tracker.h"
#include "n3k_mgmt_general_flow_stats.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_mp_callbacks.h"
#include "n3k_mgmt_mp.h"
#include "n3k_mgmt_smac_tbl.h"
#include "n3k_mgmt_tbl.h"
#include "n3k_mgmt_tunnel_tbl.h"
#include "n3k_mgmt_vplkp_tbl.h"

#include <rte_atomic.h>
#include <rte_debug.h>
#include <rte_eal.h>
#include <rte_errno.h>
#include <rte_ethdev_driver.h>
#include <rte_ether.h>
#include <rte_service_component.h>
#include <rte_pmd_n3k_malloc.h>
#include <rte_pmd_n3k_mp.h>

#include <stdint.h>
#include <string.h>

struct n3k_mgmt_mp_internal {
	struct rte_ring *entries_ring;
	struct rte_mempool *entries_pool;

	uint32_t service_id;

	enum n3k_mgmt_mp_request_table request_table;
	rte_atomic16_t busy;
};

static int
_nonblocking_rte_mempool_get(struct rte_mempool *pool, void **obj)
{
	enum {
		MAX_ATTEMPTS = 10,
		DELAY = 10
	};

	int i;
	int ret;

	for (i = 0; i < MAX_ATTEMPTS; ++i) {
		ret = rte_mempool_get(pool, obj);
		if (ret == 0) {
			return 0;
		}
		if (i < MAX_ATTEMPTS - 1) {
			rte_delay_ms(DELAY);
		}
	}

	return ret;
}

static int
_nonblocking_rte_ring_enqueue(struct rte_ring *ring, void *obj)
{
	enum {
		MAX_ATTEMPTS = 10,
		DELAY = 10
	};

	int i;
	int ret;

	for (i = 0; i < MAX_ATTEMPTS; ++i) {
		ret = rte_ring_enqueue(ring, obj);
		if (ret == 0) {
			return 0;
		}
		if (i < MAX_ATTEMPTS - 1) {
			rte_delay_ms(DELAY);
		}
	}

	return ret;
}

static const char *
n3k_mgmt_mp_request_type_to_str(enum n3k_mgmt_mp_request_table type)
{
	switch (type) {
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW:
		return "FLOW";
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW_RAW:
		return "FLOW_RAW";
	case N3K_MGMT_MP_REQUEST_TABLE_TUNNEL:
		return "TUNNEL";
	case N3K_MGMT_MP_REQUEST_TABLE_TUNNEL_RAW:
		return "TUNNEL_RAW";
	case N3K_MGMT_MP_REQUEST_TABLE_SMAC:
		return "SMAC";
	case N3K_MGMT_MP_REQUEST_TABLE_VPLKP:
		return "VPLKP";
	case N3K_MGMT_MP_REQUEST_TABLE_FLOW_TRACKER:
		return "FLOW_TRACKER";
	case N3K_MGMT_MP_REQUEST_TABLE_PORT:
		return "PORT";
	}
	return "UNKNOWN";
}

static int
n3k_mgmt_mp_parse_request(const struct rte_mp_msg *msg,
	struct n3k_mgmt_mp_request *req, struct n3k_mgmt_hw **hw)
{
	struct rte_eth_dev *eth_dev;
	memcpy(req, msg->param, sizeof(*req));
	req->device_name[sizeof(req->device_name) - 1] = 0;
	eth_dev = rte_eth_dev_allocated(req->device_name);
	*hw = n3k_mgmt_get_hw_from_dev_private(eth_dev);
	if (*hw == NULL)
		return -EINVAL;
	return 0;
}

static struct n3k_mgmt_mp_response *
n3k_mgmt_mp_setup_response(struct rte_mp_msg *response, const char *req_name)
{
	memset(response, 0, sizeof(*response));
	strncpy(response->name, req_name, RTE_MP_MAX_NAME_LEN - 1);
	RTE_BUILD_BUG_ON(sizeof(struct n3k_mgmt_mp_response) > sizeof(response->param));
	response->len_param = sizeof(struct n3k_mgmt_mp_response);
	return (struct n3k_mgmt_mp_response *)response->param;
}

static int
n3k_mgmt_mp_do_register(const struct rte_mp_msg *msg, const void *peer)
{
	struct rte_mp_msg response;
	struct n3k_mgmt_mp_register_response *reg_resp;
	struct n3k_mgmt_mp_request req;
	struct n3k_mgmt_mp_register_request *reg_req;
	struct n3k_mgmt_hw *hw;
	int ret;

	N3K_MGMT_LOG(MP, INFO, "received '%s' from secondary process",
		N3K_MGMT_MP_REQUEST_REGISTER);

	ret = n3k_mgmt_mp_parse_request(msg, &req, &hw);
	if (ret < 0) {
		N3K_MGMT_LOG(MP, ERR, "specified device name is invalid");
		return ret;
	}
	reg_req = &req.register_req;

	N3K_MGMT_LOG(MP,
		INFO,
		"requested register at base: 0x%" PRIx64 ", offset: 0x%" PRIx64,
		reg_req->base, reg_req->offset);

	reg_resp = &n3k_mgmt_mp_setup_response(&response,
		N3K_MGMT_MP_REQUEST_REGISTER)->register_resp;

	ret = n3k_mgmt_mp_fill_register_response(hw, reg_req,
		&reg_resp->reg_value);
	if (ret) {
		N3K_MGMT_LOG(MP, ERR,
			"error on filling a register response; ret = %d",
			ret);
		return ret;
	}

	ret = rte_mp_reply(&response, peer);
	if (ret < 0) {
		N3K_MGMT_LOG(MP, ERR,
			"error on sending on a response to secondary process; ret = %d",
			ret);
		return ret;
	} else {
		N3K_MGMT_LOG(MP, INFO, "response sent to secondary process");
	}

	return 0;
}

static int
n3k_mgmt_mp_do_vf0_stats(const struct rte_mp_msg *msg, const void *peer)
{
	struct rte_mp_msg response;
	struct n3k_mgmt_mp_vf0_stats_response *vf0_stats_resp;
	struct n3k_mgmt_mp_request req;
	struct n3k_mgmt_hw *hw;
	int ret;

	N3K_MGMT_LOG(MP, INFO, "received '%s' from secondary process",
		N3K_MGMT_MP_REQUEST_VF0_STATS);

	ret = n3k_mgmt_mp_parse_request(msg, &req, &hw);
	if (ret < 0) {
		N3K_MGMT_LOG(MP, ERR, "specified device name is invalid");
		return ret;
	}

	vf0_stats_resp = &n3k_mgmt_mp_setup_response(&response,
		N3K_MGMT_MP_REQUEST_VF0_STATS)->vf0_stats_resp;

	n3k_mgmt_mp_fill_vf0_stats_response(hw, vf0_stats_resp);

	ret = rte_mp_reply(&response, peer);
	if (ret < 0) {
		N3K_MGMT_LOG(MP, ERR,
			"error on sending on a response to secondary process; ret = %d",
			ret);
		return ret;
	} else {
		N3K_MGMT_LOG(MP, INFO, "response sent to secondary process");
	}

	return 0;
}

static int
n3k_mgmt_mp_do_total_entries(const struct rte_mp_msg *msg, const void *peer)
{
	struct rte_mp_msg response;
	struct n3k_mgmt_mp_total_entries_response *tot_entries_resp;
	struct n3k_mgmt_mp_request req;
	struct n3k_mgmt_mp_entries_request *entries_req;
	struct n3k_mgmt_hw *hw;
	int ret;

	N3K_MGMT_LOG(MP, INFO, "received '%s' from secondary process",
		N3K_MGMT_MP_REQUEST_TOTAL_ENTRIES);

	ret = n3k_mgmt_mp_parse_request(msg, &req, &hw);
	if (ret < 0) {
		N3K_MGMT_LOG(MP, ERR, "specified device name is invalid");
		return ret;
	}
	entries_req = &req.entries_req;

	N3K_MGMT_LOG(MP, INFO, "requested table '%s'",
		n3k_mgmt_mp_request_type_to_str(entries_req->table));

	tot_entries_resp = &n3k_mgmt_mp_setup_response(&response,
		N3K_MGMT_MP_REQUEST_TOTAL_ENTRIES)->total_entries_resp;

	ret = n3k_mgmt_mp_fill_total_entries_response(hw, entries_req->table,
		tot_entries_resp);
	if (ret < 0) {
		N3K_MGMT_LOG(MP, ERR, "error occurred while extracting number of entries for table '%s'",
			n3k_mgmt_mp_request_type_to_str(entries_req->table));
		return ret;
	}

	ret = rte_mp_reply(&response, peer);
	if (ret < 0) {
		N3K_MGMT_LOG(MP, ERR,
			"error on sending on a response to secondary process; ret = %d",
			ret);
		return ret;
	} else {
		N3K_MGMT_LOG(MP, INFO, "response sent to secondary process");
	}

	return 0;
}

static int
n3k_mgmt_push_response(struct rte_mempool *entries_pool,
	struct rte_ring *entries_ring,
	struct n3k_mgmt_mp_entries_response *response)
{
	void *obj = NULL;
	int ret;

	ret = _nonblocking_rte_mempool_get(entries_pool, &obj);
	if (ret < 0) {
		N3K_MGMT_LOG(MP, ERR,
			"no memory available to dump entries; it is possible that secondary process did not free up the objects");
		return -EINVAL;
	}

	memcpy(obj, response, sizeof(*response));

	ret = _nonblocking_rte_ring_enqueue(entries_ring, obj);
	if (ret < 0) {
		N3K_MGMT_LOG(MP, ERR,
			"no queue space available to dump entries; it is possible that secondary process stopped during dumping entries");
		ret = -EINVAL;
		goto exit_put_entry;
	}

	return 0;

exit_put_entry:
	rte_mempool_put(entries_pool, obj);
	return ret;
}

static int
n3k_mgmt_push_end_response(struct rte_mempool *entries_pool,
	struct rte_ring *entries_ring)
{
	struct n3k_mgmt_mp_entries_response response;
	memset(&response, 0, sizeof(response));
	response.end = true;
	return n3k_mgmt_push_response(entries_pool, entries_ring, &response);
}

static void
n3k_mgmt_mp_dump_table_to_shared_ring(struct n3k_mgmt_hw *hw,
	enum n3k_mgmt_mp_request_table type)
{
	struct rte_mempool *entries_pool = hw->mp_internal->entries_pool;
	struct rte_ring *entries_ring = hw->mp_internal->entries_ring;
	struct n3k_mgmt_mp_table_iterator iterator;
	int ret;

	ret = n3k_mgmt_mp_init_iterator(hw, type, &iterator);

	while (ret != -ENOENT) {
		struct n3k_mgmt_mp_entries_response response;
		ret = n3k_mgmt_mp_fill_response(hw, &iterator, &response);
		if (ret < 0) {
			N3K_MGMT_LOG(MP, ERR, "error while filling response entry; ret = %d", ret);
			ret = n3k_mgmt_mp_increment_iterator(hw, &iterator);
			continue;
		}
		response.end = false;

		ret = n3k_mgmt_push_response(entries_pool, entries_ring, &response);
		if (ret < 0) {
			N3K_MGMT_LOG(MP, WARNING, "error while pushing response; ret = %d", ret);
			break;
		}

		ret = n3k_mgmt_mp_increment_iterator(hw, &iterator);
	}

	ret = n3k_mgmt_push_end_response(entries_pool, entries_ring);
	if (ret < 0)
		N3K_MGMT_LOG(MP, WARNING, "error while pushing end response; ret = %d", ret);
}

static int32_t
n3k_mgmt_mp_send_entries_service_callback(void *args)
{
	struct n3k_mgmt_hw *hw = args;

	N3K_MGMT_LOG(MP, DEBUG, "service callback started");

	n3k_mgmt_mp_dump_table_to_shared_ring(hw, hw->mp_internal->request_table);

	rte_service_component_runstate_set(hw->mp_internal->service_id, 0);

	N3K_MGMT_LOG(MP, DEBUG, "runstate set to 0 for service component; service_id = %d",
		hw->mp_internal->service_id);

	rte_atomic16_set(&hw->mp_internal->busy, 0);

	return 0;
}

static int
n3k_mgmt_mp_do_entries(const struct rte_mp_msg *msg, __rte_unused const void *peer)
{
	struct n3k_mgmt_mp_request req;
	struct n3k_mgmt_mp_entries_request *entries_req;
	struct n3k_mgmt_hw *hw;
	int ret;

	N3K_MGMT_LOG(MP, INFO, "received '%s' from secondary process", N3K_MGMT_MP_REQUEST_ENTRIES);

	ret = n3k_mgmt_mp_parse_request(msg, &req, &hw);
	if (ret < 0) {
		N3K_MGMT_LOG(MP, ERR, "invalid request");
		return ret;
	}
	entries_req = &req.entries_req;

	N3K_MGMT_LOG(MP, INFO, "requested table '%s'",
		n3k_mgmt_mp_request_type_to_str(entries_req->table));

	// TODO(n3k): Consider that dumping flows might block n3k slow-path. It might be solved by
	//                requiring applications to have at least 2 services cores and one of them
	//                should be dedicated to 'request flows' service callback. Needs more thought.
	//
	// It is possible that 'request flows' message was sent before service core has finished
	// dumping flows to the ring. In that case n3k_mgmt has to stop the service core and wait
	// for its completion.

	while (!rte_atomic16_test_and_set(&hw->mp_internal->busy))
		rte_delay_ms(1);

	hw->mp_internal->request_table = entries_req->table;

	rte_service_component_runstate_set(hw->mp_internal->service_id, 1);
	N3K_MGMT_LOG(MP, DEBUG, "runstate set to 1 for service component; service_id = %d",
		hw->mp_internal->service_id);

	return 0;
}

/* Setup */

static int
n3k_mgmt_mp_setup_service(struct n3k_mgmt_hw *hw)
{
	struct rte_service_spec spec;
	int ret;

	memset(&spec, 0, sizeof(spec));
	strncpy(spec.name, "n3k_mgmt_mp_service", RTE_SERVICE_NAME_MAX);
	spec.callback = n3k_mgmt_mp_send_entries_service_callback;
	spec.callback_userdata = hw;
	spec.capabilities = 0;
	spec.socket_id = 0; /* TODO(n3k): Set properly when the NUMA nodes
		functionality is implemented */

	ret = rte_service_component_register(&spec, &hw->mp_internal->service_id);
	if (ret < 0) {
		N3K_MGMT_LOG(MP, ERR, "failed to register service for n3k_mgmt_mp module; ret = %d", ret);
		return ret;
	}
	N3K_MGMT_LOG(MP, DEBUG,
		"registered service callback for n3k_mgmt_mp module; service_id = %d;",
		hw->mp_internal->service_id);

	return 0;
}

static void
n3k_mgmt_mp_cleanup_service(struct n3k_mgmt_hw *hw)
{
	int ret;

	ret = rte_service_component_unregister(hw->mp_internal->service_id);
	if (ret < 0) {
		N3K_MGMT_LOG(MP, ERR,
			"failed to unregister service for n3k_mgmt_mp module; ret = %d",
			ret);
		return;
	}
	N3K_MGMT_LOG(MP, DEBUG, "unregistered service callback for n3k_mgmt_mp module");
}

static int
n3k_mgmt_mp_setup_rings(struct n3k_mgmt_hw *hw)
{
	struct rte_ring *entries_ring = NULL;

	RTE_ASSERT(hw->mp_internal != NULL);
	RTE_ASSERT(hw->mp_internal->entries_ring == NULL);

	RTE_BUILD_BUG_ON(sizeof(N3K_MGMT_MP_RING_NAME) >= RTE_RING_NAMESIZE);

	entries_ring = rte_ring_create(N3K_MGMT_MP_RING_NAME, 512, 0, RING_F_SP_ENQ);
	if (entries_ring == NULL) {
		N3K_MGMT_LOG(MP, ERR, "failed to allocate rte_ring");
		return -rte_errno;
	}

	hw->mp_internal->entries_ring = entries_ring;

	return 0;
}

static void
n3k_mgmt_mp_cleanup_rings(struct n3k_mgmt_hw *hw)
{
	RTE_ASSERT(hw->mp_internal != NULL);

	if (hw->mp_internal->entries_ring != NULL) {
		rte_ring_free(hw->mp_internal->entries_ring);
		hw->mp_internal->entries_ring = NULL;
	}
}

static int
n3k_mgmt_mp_setup_mempool(struct n3k_mgmt_hw *hw)
{
	enum {
		_ENTRIES_POOL_SIZE = (1 << 10) - 1,
		_ENTRY_SIZE = sizeof(struct n3k_mgmt_mp_entries_response),
	};

	struct rte_mempool *mempool = NULL;

	RTE_ASSERT(hw->mp_internal != NULL);
	RTE_ASSERT(hw->mp_internal->entries_pool == NULL);

	RTE_BUILD_BUG_ON(sizeof(N3K_MGMT_MP_MEMPOOL_NAME) >= RTE_MEMZONE_NAMESIZE);

	mempool = rte_mempool_create(N3K_MGMT_MP_MEMPOOL_NAME, _ENTRIES_POOL_SIZE,
			_ENTRY_SIZE, 0, 0, NULL, NULL, NULL, NULL, 0, 0);
	if (mempool == NULL) {
		N3K_MGMT_LOG(MP, ERR, "failed to allocate rte_mempool; err: %s",
			rte_strerror(rte_errno));
		return -rte_errno;
	}

	hw->mp_internal->entries_pool = mempool;

	return 0;
}

static void
n3k_mgmt_mp_cleanup_mempool(struct n3k_mgmt_hw *hw)
{
	RTE_ASSERT(hw->mp_internal != NULL);

	if (hw->mp_internal->entries_pool != NULL) {
		RTE_ASSERT(rte_mempool_full(hw->mp_internal->entries_pool));

		rte_mempool_free(hw->mp_internal->entries_pool);
		hw->mp_internal->entries_pool = NULL;
	}
}

int
n3k_mgmt_mp_init(struct n3k_mgmt_hw *hw)
{
	int ret = 0;

	N3K_MGMT_LOG(MP, INFO, "Initializing mp");

	if (rte_eal_process_type() != RTE_PROC_PRIMARY) {
		N3K_MGMT_LOG(MP, ERR,
			"to initialize n3k_mgmt_mp, process should be running as primary DPDK process");
		return -EINVAL;
	}

	RTE_ASSERT(hw->mp_internal == NULL);

	hw->mp_internal = n3k_zmalloc("n3k_mgmt_mp_internal", sizeof(*hw->mp_internal), 0);
	if (hw->mp_internal == NULL) {
		N3K_MGMT_LOG(MP, ERR, "failed to allocate memory for n3k_mgmt_mp internal data");
		return -ENOMEM;
	}

	rte_atomic16_init(&hw->mp_internal->busy);

	ret = n3k_mgmt_mp_setup_rings(hw);
	if (ret < 0) {
		N3K_MGMT_LOG(MP, ERR, "failed to setup rings in n3k_mgmt_mp module");
		goto exit_cleanup_internal_data;
	}

	ret = n3k_mgmt_mp_setup_mempool(hw);
	if (ret < 0) {
		N3K_MGMT_LOG(MP, ERR, "failed to setup mempools in n3k_mgmt_mp module");
		goto exit_cleanup_rings;
	}

	ret = n3k_mgmt_mp_setup_service(hw);
	if (ret < 0) {
		N3K_MGMT_LOG(MP, ERR, "failed to setup service core for n3k_mgmt_mp module");
		goto exit_cleanup_mempool;
	}

	// TODO(n3k): Check for ENOTSUP
	ret = rte_mp_action_register(N3K_MGMT_MP_REQUEST_VF0_STATS,
		n3k_mgmt_mp_do_vf0_stats);
	if (ret < 0 && rte_errno != EEXIST) {
		ret = -rte_errno;
		N3K_MGMT_LOG(MP, ERR,
			"failed to register a multi-process callback for action '%s'; ret = %d",
			N3K_MGMT_MP_REQUEST_VF0_STATS, ret);
		goto exit_cleanup_service;
	}
	N3K_MGMT_LOG(MP, DEBUG, "registered '%s' MP action", N3K_MGMT_MP_REQUEST_REGISTER);

	// TODO(n3k): Check for ENOTSUP
	ret = rte_mp_action_register(N3K_MGMT_MP_REQUEST_REGISTER,
		n3k_mgmt_mp_do_register);
	if (ret < 0 && rte_errno != EEXIST) {
		ret = -rte_errno;
		N3K_MGMT_LOG(MP, ERR,
			"failed to register a multi-process callback for action '%s'; ret = %d",
			N3K_MGMT_MP_REQUEST_REGISTER, ret);
		goto exit_cleanup_dump_vf0_stats_action;
	}
	N3K_MGMT_LOG(MP, DEBUG, "registered '%s' MP action", N3K_MGMT_MP_REQUEST_REGISTER);

	// TODO(n3k): Check for ENOTSUP
	ret = rte_mp_action_register(N3K_MGMT_MP_REQUEST_TOTAL_ENTRIES,
		n3k_mgmt_mp_do_total_entries);
	if (ret < 0 && rte_errno != EEXIST) {
		ret = -rte_errno;
		N3K_MGMT_LOG(MP, ERR,
			"failed to register a multi-process callback for action '%s'; ret = %d",
			N3K_MGMT_MP_REQUEST_TOTAL_ENTRIES, ret);
		goto exit_cleanup_dump_register_action;
	}
	N3K_MGMT_LOG(MP, DEBUG, "registered '%s' MP action", N3K_MGMT_MP_REQUEST_TOTAL_ENTRIES);

	// TODO(n3k): Check for ENOTSUP
	ret = rte_mp_action_register(N3K_MGMT_MP_REQUEST_ENTRIES, n3k_mgmt_mp_do_entries);
	if (ret < 0 && rte_errno != EEXIST) {
		ret = -rte_errno;
		N3K_MGMT_LOG(MP, ERR,
			"failed to register a multi-process callback for action '%s'; ret = %d",
			N3K_MGMT_MP_REQUEST_ENTRIES, ret);
		goto exit_cleanup_total_entries_action;
	}
	N3K_MGMT_LOG(MP, DEBUG, "registered '%s' MP action", N3K_MGMT_MP_REQUEST_ENTRIES);

	return 0;

exit_cleanup_total_entries_action:
	rte_mp_action_unregister(N3K_MGMT_MP_REQUEST_TOTAL_ENTRIES);

exit_cleanup_dump_register_action:
	rte_mp_action_unregister(N3K_MGMT_MP_REQUEST_REGISTER);

exit_cleanup_dump_vf0_stats_action:
	rte_mp_action_unregister(N3K_MGMT_MP_REQUEST_VF0_STATS);

exit_cleanup_service:
	n3k_mgmt_mp_cleanup_service(hw);

exit_cleanup_mempool:
	n3k_mgmt_mp_cleanup_mempool(hw);

exit_cleanup_rings:
	n3k_mgmt_mp_cleanup_rings(hw);

exit_cleanup_internal_data:
	n3k_free(hw->mp_internal);
	hw->mp_internal = NULL;

	return ret;
}

void
n3k_mgmt_mp_exit(struct n3k_mgmt_hw *hw)
{
	N3K_MGMT_LOG(MP, INFO, "Deinitializing mp");

	if (rte_eal_process_type() != RTE_PROC_PRIMARY) {
		N3K_MGMT_LOG(MP, ERR,
			"to uninitialize n3k_mgmt_mp, process should be running as primary DPDK process");
		return;
	}

	rte_mp_action_unregister(N3K_MGMT_MP_REQUEST_ENTRIES);
	rte_mp_action_unregister(N3K_MGMT_MP_REQUEST_TOTAL_ENTRIES);
	rte_mp_action_unregister(N3K_MGMT_MP_REQUEST_REGISTER);
	rte_mp_action_unregister(N3K_MGMT_MP_REQUEST_VF0_STATS);

	n3k_mgmt_mp_cleanup_service(hw);
	n3k_mgmt_mp_cleanup_mempool(hw);
	n3k_mgmt_mp_cleanup_rings(hw);

	n3k_free(hw->mp_internal);
	hw->mp_internal = NULL;
}
