/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "../n3k_mgmt_bitwise.h"
#include "../n3k_mgmt_flow_tbl_regs_api.h"
#include "../n3k_mgmt_flow_tbl.h"
#include "../n3k_mgmt_flow_tracker_regs_api.h"
#include "../n3k_mgmt_flow_tracker.h"
#include "../n3k_mgmt_flr_flow_tbl.h"
#include "../n3k_mgmt_flr_flow_tracker.h"
#include "../n3k_mgmt_flr_pkts_blocks.h"
#include "../n3k_mgmt_flr_pkts.h"
#include "../n3k_mgmt_lock.h"
#include "../n3k_mgmt_log.h"
#include "n3k_mgmt_flr_sim_request_handler.h"

#include "../ifc/ifc_port.h"

#include "../../rte_pmd_n3k_malloc.h"

#include <rte_mbuf.h>
#include <rte_ether.h>

enum n3k_mgmt_flr_sim_request_type {
	N3K_MGMT_FLR_SIM_REQUEST_FLOW_CREATE,
	N3K_MGMT_FLR_SIM_REQUEST_MIRROR_FLOW_CREATE,
	N3K_MGMT_FLR_SIM_REQUEST_FLOW_DESTROY,
	N3K_MGMT_FLR_SIM_REQUEST_MIRROR_FLOW_DESTROY,
	N3K_MGMT_FLR_SIM_REQUEST_FLOW_QUERY,
	N3K_MGMT_FLR_SIM_REQUEST_FLOW_DUMP,
	N3K_MGMT_FLR_SIM_REQUEST_READ_REG_32,
	N3K_MGMT_FLR_SIM_REQUEST_UNKNOWN,
};

inline static const char *
request_type_to_str(enum n3k_mgmt_flr_sim_request_type type)
{
	switch (type) {
	case N3K_MGMT_FLR_SIM_REQUEST_FLOW_CREATE:
		return "FLOW_CREATE";
	case N3K_MGMT_FLR_SIM_REQUEST_MIRROR_FLOW_CREATE:
		return "MIRROR_FLOW_CREATE";
	case N3K_MGMT_FLR_SIM_REQUEST_FLOW_DESTROY:
		return "FLOW_DESTROY";
	case N3K_MGMT_FLR_SIM_REQUEST_MIRROR_FLOW_DESTROY:
		return "MIRROR_FLOW_DESTROY";
	case N3K_MGMT_FLR_SIM_REQUEST_FLOW_QUERY:
		return "FLOW_QUERY";
	case N3K_MGMT_FLR_SIM_REQUEST_FLOW_DUMP:
		return "FLOW_DUMP";
	case N3K_MGMT_FLR_SIM_REQUEST_READ_REG_32:
		return "READ_REG_32";
	case N3K_MGMT_FLR_SIM_REQUEST_UNKNOWN:
		return "UNKNOWN";
	}
	return "INVALID-REQUEST-TYPE";
}

struct n3k_mgmt_flr_sim_request_flow_create {
	struct lookup_emlc_key_regs key;
	struct lookup_emlc_res_regs res;
	struct flow_tracker_data_regs ft;
	uint32_t stats_id;
};

struct n3k_mgmt_flr_sim_request_mirror_flow_create {
	struct lookup_emlc_key_regs key;
	struct lookup_emlc_res_regs res;
};

struct n3k_mgmt_flr_sim_request_flow_destroy {
	struct flow_tracker_data_regs ft;
	uint32_t flow_id;
	uint32_t stats_id;
};

struct n3k_mgmt_flr_sim_request_mirror_flow_destroy {
	uint32_t flow_id;
};

struct n3k_mgmt_flr_sim_request_flow_query {
	uint32_t stats_id;
};

inline static enum n3k_mgmt_flr_sim_request_type
guess_request_type(struct n3k_mgmt_flr_parsed_pkt *parsed_pkt)
{
	// TODO(n3k): guess request type by register addresses and other metadata
	if (parsed_pkt->blocks_num == 1)
		return N3K_MGMT_FLR_SIM_REQUEST_READ_REG_32;
	if (parsed_pkt->blocks_num == 2) {
		if (parsed_pkt->blocks[1].cmd_type == N3K_MGMT_FLR_BLOCK_HEADER_CMD_TYPE_READ)
			return N3K_MGMT_FLR_SIM_REQUEST_FLOW_QUERY;
		return N3K_MGMT_FLR_SIM_REQUEST_MIRROR_FLOW_DESTROY;
	}
	if (parsed_pkt->blocks_num == 4) {
		if (parsed_pkt->blocks[2].cmd_type == N3K_MGMT_FLR_BLOCK_HEADER_CMD_TYPE_READ)
			return N3K_MGMT_FLR_SIM_REQUEST_FLOW_DUMP;
		return N3K_MGMT_FLR_SIM_REQUEST_FLOW_DESTROY;
	}
	if (parsed_pkt->blocks_num == 5)
		return N3K_MGMT_FLR_SIM_REQUEST_MIRROR_FLOW_CREATE;
	if (parsed_pkt->blocks_num == 7)
		return N3K_MGMT_FLR_SIM_REQUEST_FLOW_CREATE;
	return N3K_MGMT_FLR_SIM_REQUEST_UNKNOWN;
}

inline static int
parse_flow_create_request(struct n3k_mgmt_flr_parsed_pkt *parsed_pkt,
	struct n3k_mgmt_flr_sim_request_flow_create *flow_create)
{
	LKUP_EMLC_MGMT_CTRL_t emlc_mgmt_ctrl;
	MKS_FLOW_TRACKER_FT_COMMAND_t ft_comm;
	memcpy_bswap32(&flow_create->ft,
		parsed_pkt->blocks[N3K_MGMT_FLR_FLOW_CREATE_FLOW_TRACKER_DATA_BLOCK].data,
		sizeof(flow_create->ft));
	memcpy_bswap32(&ft_comm,
		parsed_pkt->blocks[N3K_MGMT_FLR_FLOW_CREATE_FLOW_TRACKER_COMMAND_BLOCK].data,
		sizeof(ft_comm));
	memcpy_bswap32(&flow_create->key,
		parsed_pkt->blocks[N3K_MGMT_FLR_FLOW_CREATE_KEY_BLOCK].data,
		sizeof(flow_create->key));
	memcpy_bswap32(&flow_create->res,
		parsed_pkt->blocks[N3K_MGMT_FLR_FLOW_CREATE_ACTION_BLOCK].data,
		sizeof(flow_create->res));
	memcpy_bswap32(&emlc_mgmt_ctrl,
		parsed_pkt->blocks[N3K_MGMT_FLR_FLOW_CREATE_EMLC_MGMT_CTRL_BLOCK].data,
		sizeof(emlc_mgmt_ctrl));

	if (ft_comm.field.operation != MKS_FLOW_TRACKER_FT_COMMAND_WRITE) {
		N3K_MGMT_FLR_SIM_LOG(ERR, "Invalid ft_command operation type: %" PRIu32,
			ft_comm.field.operation);
		return -EINVAL;
	}

	if (emlc_mgmt_ctrl.field.req_type != LKUP_EMLC_MGMT_CTRL_REQ_TYPE_INSERT) {
		N3K_MGMT_FLR_SIM_LOG(ERR, "Invalid emlc_mgmt_ctrl req type: %" PRIu32,
			emlc_mgmt_ctrl.field.req_type);
		return -EINVAL;
	}

	flow_create->stats_id = ft_comm.field.index;

	N3K_MGMT_FLR_SIM_LOG(DEBUG, "Parsed flow create request for stats_id=%" PRIu32,
		flow_create->stats_id);

	return 0;
}

inline static int
parse_mirror_flow_create_request(struct n3k_mgmt_flr_parsed_pkt *parsed_pkt,
	struct n3k_mgmt_flr_sim_request_mirror_flow_create *flow_create)
{
	LKUP_EMLC_MGMT_CTRL_t emlc_mgmt_ctrl;
	memcpy_bswap32(&flow_create->key,
		parsed_pkt->blocks[N3K_MGMT_FLR_MIRROR_FLOW_CREATE_KEY_BLOCK].data,
		sizeof(flow_create->key));
	memcpy_bswap32(&flow_create->res,
		parsed_pkt->blocks[N3K_MGMT_FLR_MIRROR_FLOW_CREATE_ACTION_BLOCK].data,
		sizeof(flow_create->res));
	memcpy_bswap32(&emlc_mgmt_ctrl,
		parsed_pkt->blocks[N3K_MGMT_FLR_MIRROR_FLOW_CREATE_EMLC_MGMT_CTRL_BLOCK].data,
		sizeof(emlc_mgmt_ctrl));

	if (emlc_mgmt_ctrl.field.req_type != LKUP_EMLC_MGMT_CTRL_REQ_TYPE_INSERT) {
		N3K_MGMT_FLR_SIM_LOG(ERR, "Invalid emlc_mgmt_ctrl req type: %" PRIu32,
			emlc_mgmt_ctrl.field.req_type);
		return -EINVAL;
	}

	N3K_MGMT_FLR_SIM_LOG(DEBUG, "Parsed mirror flow create request");

	return 0;
}

inline static int
parse_flow_destroy_request(struct n3k_mgmt_flr_parsed_pkt *parsed_pkt,
	struct n3k_mgmt_flr_sim_request_flow_destroy *flow_destroy)
{
	LKUP_EMLC_MGMT_CTRL_t emlc_mgmt_ctrl;
	MKS_FLOW_TRACKER_FT_COMMAND_t ft_comm;
	memcpy_bswap32(&flow_destroy->flow_id,
		parsed_pkt->blocks[N3K_MGMT_FLR_FLOW_DESTROY_HANDLE_BLOCK].data,
		sizeof(flow_destroy->flow_id));
	memcpy_bswap32(&emlc_mgmt_ctrl,
		parsed_pkt->blocks[N3K_MGMT_FLR_FLOW_DESTROY_EMLC_MGMT_CTRL_BLOCK].data,
		sizeof(emlc_mgmt_ctrl));
	memcpy_bswap32(&flow_destroy->ft,
		parsed_pkt->blocks[N3K_MGMT_FLR_FLOW_DESTROY_FLOW_TRACKER_DATA_BLOCK].data,
		sizeof(flow_destroy->ft));
	memcpy_bswap32(&ft_comm,
		parsed_pkt->blocks[N3K_MGMT_FLR_FLOW_DESTROY_FLOW_TRACKER_COMMAND_BLOCK].data,
		sizeof(ft_comm));

	if (ft_comm.field.operation != MKS_FLOW_TRACKER_FT_COMMAND_WRITE) {
		N3K_MGMT_FLR_SIM_LOG(ERR, "Invalid ft_command operation type: %" PRIu32,
			ft_comm.field.operation);
		return -EINVAL;
	}

	flow_destroy->stats_id = ft_comm.field.index;

	if (emlc_mgmt_ctrl.field.req_type != LKUP_EMLC_MGMT_CTRL_REQ_TYPE_DELETE_HANDLE) {
		N3K_MGMT_FLR_SIM_LOG(ERR, "Invalid emlc_mgmt_ctrl req type: %" PRIu32,
			emlc_mgmt_ctrl.field.req_type);
		return -EINVAL;
	}

	N3K_MGMT_FLR_SIM_LOG(DEBUG, "Parsed flow destroy request for flow_id=%" PRIu32,
		flow_destroy->flow_id);

	return 0;
}

inline static int
parse_mirror_flow_destroy_request(struct n3k_mgmt_flr_parsed_pkt *parsed_pkt,
	struct n3k_mgmt_flr_sim_request_mirror_flow_destroy *flow_destroy)
{
	LKUP_EMLC_MGMT_CTRL_t emlc_mgmt_ctrl;
	memcpy_bswap32(&flow_destroy->flow_id,
		parsed_pkt->blocks[N3K_MGMT_FLR_MIRROR_FLOW_DESTROY_HANDLE_BLOCK].data,
		sizeof(flow_destroy->flow_id));
	memcpy_bswap32(&emlc_mgmt_ctrl,
		parsed_pkt->blocks[N3K_MGMT_FLR_MIRROR_FLOW_DESTROY_EMLC_MGMT_CTRL_BLOCK].data,
		sizeof(emlc_mgmt_ctrl));

	if (emlc_mgmt_ctrl.field.req_type != LKUP_EMLC_MGMT_CTRL_REQ_TYPE_DELETE_HANDLE) {
		N3K_MGMT_FLR_SIM_LOG(ERR, "Invalid emlc_mgmt_ctrl req type: %" PRIu32,
			emlc_mgmt_ctrl.field.req_type);
		return -EINVAL;
	}

	N3K_MGMT_FLR_SIM_LOG(DEBUG, "Parsed mirror flow destroy request for flow_id=%" PRIu32,
		flow_destroy->flow_id);

	return 0;
}

inline static int
parse_flow_query_request(struct n3k_mgmt_flr_parsed_pkt *parsed_pkt,
	struct n3k_mgmt_flr_sim_request_flow_query *flow_query)
{
	MKS_FLOW_TRACKER_FT_COMMAND_t ft_comm;
	memcpy_bswap32(&ft_comm,
		parsed_pkt->blocks[N3K_MGMT_FLR_FLOW_QUERY_FT_COMMAND_BLOCK].data,
		sizeof(ft_comm));

	if (ft_comm.field.operation != MKS_FLOW_TRACKER_FT_COMMAND_READ) {
		N3K_MGMT_FLR_SIM_LOG(ERR, "Invalid ft_command operation type: %" PRIu32,
			ft_comm.field.operation);
		return -EINVAL;
	}

	flow_query->stats_id = ft_comm.field.index;

	N3K_MGMT_FLR_SIM_LOG(DEBUG, "Parsed flow query request for stats_id=%" PRIu32,
		flow_query->stats_id);

	return 0;
}

inline static struct n3k_mgmt_flr_block_header *
get_block_header(struct n3k_mgmt_flr_parsed_block *parsed_block)
{
	return (struct n3k_mgmt_flr_block_header *)
		((uint8_t *)parsed_block->data - sizeof(struct n3k_mgmt_flr_block_header));
}

inline static void
set_block_error_code(struct n3k_mgmt_flr_parsed_pkt *parsed_pkt,
	uint8_t block_id, uint8_t error_code)
{
	struct n3k_mgmt_flr_parsed_block *parsed_block = &parsed_pkt->blocks[block_id];
	struct n3k_mgmt_flr_block_header *block_hdr = get_block_header(parsed_block);
	block_hdr->error_code = parsed_block->error_code = error_code;
}

inline static void
clear_error_codes(struct n3k_mgmt_flr_parsed_pkt *parsed_pkt)
{
	int block_id;
	for (block_id = 0; block_id < parsed_pkt->blocks_num; ++block_id)
		set_block_error_code(parsed_pkt, block_id, 0);
}

inline static int
execute_flow_create_request(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_parsed_pkt *parsed_pkt)
{
	int ret = 0, tmp_ret;
	struct n3k_mgmt_flr_sim_request_flow_create flow_create;
	uint32_t hw_flow_id;
	LKUP_EMLC_MGMT_CTRL_t emlc_mgmt = {{0}};

	clear_error_codes(parsed_pkt);

	ret = parse_flow_create_request(parsed_pkt, &flow_create);
	if (ret < 0) {
		N3K_MGMT_FLR_SIM_LOG(ERR,
			"Invalid flow create request: ret=%d, pkt_id=%" PRIu16,
			ret, parsed_pkt->id);
		set_block_error_code(parsed_pkt, 0, -ret);
		return ret;
	}

	tmp_ret = n3k_mgmt_flow_tracker_write_regs(hw, flow_create.stats_id,
		&flow_create.ft);
	if (tmp_ret < 0) {
		N3K_MGMT_FLR_SIM_LOG(ERR,
			"Failed to execute flow tracker enable request: ret=%d, pkt_id=%" PRIu16,
			tmp_ret, parsed_pkt->id);
		set_block_error_code(parsed_pkt,
			N3K_MGMT_FLR_FLOW_CREATE_FLOW_TRACKER_COMMAND_BLOCK, -tmp_ret);
		ret = tmp_ret;
	}

	tmp_ret = n3k_mgmt_flow_tbl_add_by_regs(hw, &flow_create.key, &flow_create.res, &emlc_mgmt, &hw_flow_id);
	if (tmp_ret < 0) {
		N3K_MGMT_FLR_SIM_LOG(ERR,
			"Failed to execute flow create request: ret=%d, pkt_id=%" PRIu16,
			tmp_ret, parsed_pkt->id);
		set_block_error_code(parsed_pkt,
			N3K_MGMT_FLR_FLOW_CREATE_EMLC_MGMT_CTRL_BLOCK, -tmp_ret);
		ret = tmp_ret;
	}

	if (ret == 0)
		N3K_MGMT_FLR_SIM_LOG(DEBUG, "Successfully created flow: pkt_id=%" PRIu16,
			parsed_pkt->id);

	memcpy_bswap32(parsed_pkt->blocks[N3K_MGMT_FLR_FLOW_CREATE_READ_EMLC_MGMT_CTRL_BLOCK].data,
		&emlc_mgmt.val, sizeof(emlc_mgmt));
	memcpy_bswap32(parsed_pkt->blocks[N3K_MGMT_FLR_FLOW_CREATE_READ_EMLC_KEY_HANDLE_BLOCK].data,
		&hw_flow_id, sizeof(hw_flow_id));
	return ret;
}

inline static int
execute_mirror_flow_create_request(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_parsed_pkt *parsed_pkt)
{
	int ret = 0, tmp_ret;
	struct n3k_mgmt_flr_sim_request_mirror_flow_create flow_create;
	uint32_t hw_flow_id;
	LKUP_EMLC_MGMT_CTRL_t emlc_mgmt = {{0}};

	clear_error_codes(parsed_pkt);

	ret = parse_mirror_flow_create_request(parsed_pkt, &flow_create);
	if (ret < 0) {
		N3K_MGMT_FLR_SIM_LOG(ERR,
			"Invalid mirror flow create request: ret=%d, pkt_id=%" PRIu16,
			ret, parsed_pkt->id);
		set_block_error_code(parsed_pkt, 0, -ret);
		return ret;
	}

	tmp_ret = n3k_mgmt_flow_tbl_add_by_regs(hw, &flow_create.key,
		&flow_create.res, &emlc_mgmt, &hw_flow_id);
	if (tmp_ret < 0) {
		N3K_MGMT_FLR_SIM_LOG(ERR,
			"Failed to execute mirror flow create request: ret=%d, pkt_id=%" PRIu16,
			tmp_ret, parsed_pkt->id);
		set_block_error_code(parsed_pkt,
			N3K_MGMT_FLR_MIRROR_FLOW_CREATE_EMLC_MGMT_CTRL_BLOCK, -tmp_ret);
		ret = tmp_ret;
	}

	if (ret == 0)
		N3K_MGMT_FLR_SIM_LOG(DEBUG, "Successfully created mirror flow: pkt_id=%" PRIu16,
			parsed_pkt->id);

	memcpy_bswap32(parsed_pkt->blocks[N3K_MGMT_FLR_MIRROR_FLOW_CREATE_READ_EMLC_MGMT_CTRL_BLOCK].data,
		&emlc_mgmt.val, sizeof(emlc_mgmt));
	memcpy_bswap32(parsed_pkt->blocks[N3K_MGMT_FLR_MIRROR_FLOW_CREATE_READ_EMLC_KEY_HANDLE_BLOCK].data,
		&hw_flow_id, sizeof(hw_flow_id));
	return ret;
}

inline static int
execute_flow_destroy_request(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_parsed_pkt *parsed_pkt)
{
	int ret = 0, tmp_ret;
	struct n3k_mgmt_flr_sim_request_flow_destroy flow_destroy;

	clear_error_codes(parsed_pkt);

	ret = parse_flow_destroy_request(parsed_pkt, &flow_destroy);
	if (ret < 0) {
		N3K_MGMT_FLR_SIM_LOG(ERR,
			"Invalid flow create request: ret=%d, pkt_id=%" PRIu16,
			ret, parsed_pkt->id);
		set_block_error_code(parsed_pkt, 0, -ret);
		return ret;
	}

	tmp_ret = n3k_mgmt_flow_tbl_del_by_regs(hw, flow_destroy.flow_id);
	if (tmp_ret < 0) {
		N3K_MGMT_FLR_SIM_LOG(ERR,
			"Failed to execute flow destroy request: ret=%d, pkt_id=%" PRIu16,
			tmp_ret, parsed_pkt->id);
		set_block_error_code(parsed_pkt,
			N3K_MGMT_FLR_FLOW_DESTROY_EMLC_MGMT_CTRL_BLOCK, -tmp_ret);
		ret = tmp_ret;
	}

	tmp_ret = n3k_mgmt_flow_tracker_write_regs(hw, flow_destroy.stats_id,
		&flow_destroy.ft);
	if (tmp_ret < 0) {
		N3K_MGMT_FLR_SIM_LOG(ERR,
			"Failed to execute flow tracker disable request: ret=%d, pkt_id=%" PRIu16,
			tmp_ret, parsed_pkt->id);
		set_block_error_code(parsed_pkt,
			N3K_MGMT_FLR_FLOW_DESTROY_FLOW_TRACKER_COMMAND_BLOCK, -tmp_ret);
		ret = tmp_ret;
	}

	if (ret == 0)
		N3K_MGMT_FLR_SIM_LOG(DEBUG, "Successfully destroyed flow: pkt_id=%" PRIu16,
			parsed_pkt->id);

	return ret;
}

inline static int
execute_mirror_flow_destroy_request(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_parsed_pkt *parsed_pkt)
{
	int ret = 0;
	struct n3k_mgmt_flr_sim_request_mirror_flow_destroy flow_destroy;

	clear_error_codes(parsed_pkt);

	ret = parse_mirror_flow_destroy_request(parsed_pkt, &flow_destroy);
	if (ret < 0) {
		N3K_MGMT_FLR_SIM_LOG(ERR,
			"Invalid flow create request: ret=%d, pkt_id=%" PRIu16,
			ret, parsed_pkt->id);
		set_block_error_code(parsed_pkt, 0, -ret);
		return ret;
	}

	ret = n3k_mgmt_flow_tbl_del_by_regs(hw, flow_destroy.flow_id);
	if (ret < 0) {
		N3K_MGMT_FLR_SIM_LOG(ERR,
			"Failed to execute mirror flow destroy request: ret=%d, pkt_id=%" PRIu16,
			ret, parsed_pkt->id);
		set_block_error_code(parsed_pkt,
			N3K_MGMT_FLR_MIRROR_FLOW_DESTROY_EMLC_MGMT_CTRL_BLOCK, -ret);
	}

	if (ret == 0)
		N3K_MGMT_FLR_SIM_LOG(DEBUG,
			"Successfully destroyed mirror flow: pkt_id=%" PRIu16,
			parsed_pkt->id);

	return ret;
}

inline static int
execute_flow_query_request(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_parsed_pkt *parsed_pkt)
{
	struct n3k_mgmt_flr_sim_request_flow_query flow_query;
	struct flow_tracker_data_regs ft_data;
	int ret = 0, tmp_ret;

	clear_error_codes(parsed_pkt);

	ret = parse_flow_query_request(parsed_pkt, &flow_query);
	if (ret < 0) {
		N3K_MGMT_FLR_SIM_LOG(ERR,
			"Invalid flow query request: ret=%d, pkt_id=%" PRIu16,
			ret, parsed_pkt->id);
		set_block_error_code(parsed_pkt, 0, -ret);
		return ret;
	}

	tmp_ret = n3k_mgmt_flow_tracker_get_by_regs(hw, flow_query.stats_id, &ft_data);
	if (tmp_ret < 0) {
		N3K_MGMT_FLR_SIM_LOG(ERR,
			"Failed to get flow tracker data regs: stats_id=%" PRIu32
			", ret=%d, pkt_id=%" PRIu16,
			flow_query.stats_id, tmp_ret, parsed_pkt->id);
		set_block_error_code(parsed_pkt,
			N3K_MGMT_FLR_FLOW_QUERY_FT_COMMAND_BLOCK, -tmp_ret);
		ret = tmp_ret;
	}
	memcpy_bswap32(parsed_pkt->blocks[N3K_MGMT_FLR_FLOW_QUERY_READ_FT_DATA_BLOCK].data,
		(uint32_t *)(&ft_data), sizeof(ft_data));

	if (ret == 0)
		N3K_MGMT_FLR_SIM_LOG(DEBUG, "Query request success: stats_id=%" PRIu32 ", pkt_id=%" PRIu16,
			flow_query.stats_id, parsed_pkt->id);

	return ret;
}

inline static int
execute_read_reg_32_request(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_parsed_pkt *parsed_pkt)
{
	uint32_t reg_addr = parsed_pkt->blocks[0].start_reg_addr;
	uint32_t val = csr_read(hw, reg_addr, 0);
	memcpy_bswap32(parsed_pkt->blocks[0].data, &val, sizeof(uint32_t));
	parsed_pkt->blocks[0].error_code = 0;

	N3K_MGMT_FLR_SIM_LOG(DEBUG, "Read reg 32 request success: val=%" PRIu32,
		val);

	return 0;
}

inline static int
execute_flow_dump_request(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_parsed_pkt *parsed_pkt)
{
	int block_id;
	for (block_id = 0; block_id < parsed_pkt->blocks_num; ++block_id)
		set_block_error_code(parsed_pkt, block_id, 0);

	uint32_t hw_flow_id = rte_bswap32(*parsed_pkt->blocks[N3K_MGMT_FLR_FLOW_DUMP_HANDLE_BLOCK].data);

	struct lookup_emlc_key_regs key_regs;
	struct lookup_emlc_res_regs res_regs;
	int ret;

	ret = n3k_mgmt_flow_tbl_get_key_res_by_regs(hw, hw_flow_id, &key_regs, &res_regs);

	if (ret < 0) {
		N3K_MGMT_FLR_SIM_LOG(DEBUG,
			"Couldn't read flow entry for hw_flow_id=%" PRIu32 ", ret=%d",
			hw_flow_id, ret);
		parsed_pkt->blocks[N3K_MGMT_FLR_FLOW_DUMP_FLOW_READ_KEY_BLOCK].error_code = -EIO;
		parsed_pkt->blocks[N3K_MGMT_FLR_FLOW_DUMP_FLOW_READ_RES_BLOCK].error_code = -EIO;
		parsed_pkt->error = true;
		return ret;
	}

	memcpy_bswap32(parsed_pkt->blocks[N3K_MGMT_FLR_FLOW_DUMP_FLOW_READ_KEY_BLOCK].data,
		(const uint32_t *) &key_regs, sizeof(key_regs));

	memcpy_bswap32(parsed_pkt->blocks[N3K_MGMT_FLR_FLOW_DUMP_FLOW_READ_RES_BLOCK].data,
		(const uint32_t *) &res_regs, sizeof(res_regs));

	N3K_MGMT_FLR_SIM_LOG(DEBUG, "Flow dump request success: hw_flow_id=%" PRIu32,
		hw_flow_id);

	return 0;
}

inline static struct n3k_mgmt_flr_config_header *
get_config_header_ptr_raw(void *pkt_data)
{
	return (struct n3k_mgmt_flr_config_header *)
		((uint8_t *)pkt_data + sizeof(struct rte_ether_hdr));
}

static void
prepare_response_header(void *pkt_data, bool error)
{
	struct rte_ether_addr smac, dmac;
	struct rte_ether_hdr *ether_hdr = pkt_data;
	struct n3k_mgmt_flr_config_header *cfg_hdr = get_config_header_ptr_raw(pkt_data);

	memcpy(smac.addr_bytes, ether_hdr->s_addr.addr_bytes, sizeof(smac.addr_bytes));
	memcpy(dmac.addr_bytes, ether_hdr->d_addr.addr_bytes, sizeof(dmac.addr_bytes));
	memcpy(ether_hdr->d_addr.addr_bytes, smac.addr_bytes, sizeof(ether_hdr->d_addr.addr_bytes));
	memcpy(ether_hdr->s_addr.addr_bytes, dmac.addr_bytes, sizeof(ether_hdr->s_addr.addr_bytes));

	cfg_hdr->pkt_type = N3K_MGMT_FLR_CONFIG_HEADER_PKT_TYPE_RESPONSE;
	cfg_hdr->error = error;
}

int
n3k_mgmt_flr_sim_execute_request(struct n3k_mgmt_hw *hw, void *pkt_data, uint16_t pkt_size)
{
	int ret;
	struct n3k_mgmt_flr_parsed_pkt parsed_pkt;

	ret = n3k_mgmt_flr_parse_config_pkt_raw(pkt_data, pkt_size, &parsed_pkt);
	if (ret < 0) {
		N3K_MGMT_FLR_SIM_LOG(ERR, "Couldn't parse packet");
		return -EINVAL;
	}

	if (parsed_pkt.pkt_type != N3K_MGMT_FLR_CONFIG_HEADER_PKT_TYPE_REQUEST) {
		N3K_MGMT_FLR_SIM_LOG(ERR, "Parsed packet is not a request packet");
		return -EINVAL;
	}

	enum n3k_mgmt_flr_sim_request_type req_type = guess_request_type(&parsed_pkt);

	N3K_MGMT_FLR_SIM_LOG(INFO, "Executing request id=%" PRIu16 ", request_type=%s",
		parsed_pkt.id, request_type_to_str(req_type));

	switch (req_type) {
	case N3K_MGMT_FLR_SIM_REQUEST_FLOW_CREATE:
		ret = execute_flow_create_request(hw, &parsed_pkt);
		break;
	case N3K_MGMT_FLR_SIM_REQUEST_MIRROR_FLOW_CREATE:
		ret = execute_mirror_flow_create_request(hw, &parsed_pkt);
		break;
	case N3K_MGMT_FLR_SIM_REQUEST_FLOW_DESTROY:
		ret = execute_flow_destroy_request(hw, &parsed_pkt);
		break;
	case N3K_MGMT_FLR_SIM_REQUEST_MIRROR_FLOW_DESTROY:
		ret = execute_mirror_flow_destroy_request(hw, &parsed_pkt);
		break;
	case N3K_MGMT_FLR_SIM_REQUEST_FLOW_QUERY:
		ret = execute_flow_query_request(hw, &parsed_pkt);
		break;
	case N3K_MGMT_FLR_SIM_REQUEST_FLOW_DUMP:
		ret = execute_flow_dump_request(hw, &parsed_pkt);
		break;
	case N3K_MGMT_FLR_SIM_REQUEST_READ_REG_32:
		ret = execute_read_reg_32_request(hw, &parsed_pkt);
		break;
	case N3K_MGMT_FLR_SIM_REQUEST_UNKNOWN:
		// TODO(n3k): implement
		return -ENOTSUP;
	}

