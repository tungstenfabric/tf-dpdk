/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_FLR_COMMAND_H_
#define _PMD_NET_N3K_MGMT_FLR_COMMAND_H_

#include "n3k_mgmt_tbl.h"

#include <rte_pmd_n3k_flow_tbl_mgmt.h>

#include <rte_atomic.h>
#include <stdint.h>
#include <time.h>

struct n3k_mgmt_hw;
struct rte_mbuf;
struct n3k_mgmt_flr_parsed_pkt;

enum { N3K_MGMT_FLR_COMMAND_PROCESSED_MAX_COUNT = 8 };

enum n3k_mgmt_flr_command_type {
	N3K_MGMT_FLR_COMMAND_FLOW_CREATE,
	N3K_MGMT_FLR_COMMAND_FLOW_DESTROY,
	N3K_MGMT_FLR_COMMAND_FLOW_QUERY,
	N3K_MGMT_FLR_COMMAND_FLOW_HANDLE,
	N3K_MGMT_FLR_COMMAND_FLOW_DUMP,
	N3K_MGMT_FLR_COMMAND_READ_REG_32,
};

struct n3k_mgmt_flr_command_metadata {
	enum n3k_mgmt_flr_command_type type;
	uint16_t packet_id;
	uint16_t processed_count;
	struct timespec processed_time;
};

struct n3k_mgmt_flr_command_flow_create {
	struct n3k_mgmt_flow_tbl_handle *handle;
	struct n3k_mgmt_flow_entry flow;
	uint32_t hw_flow_id;
	enum {
		FLOW_CREATE_CMD_FLOW_ADDED,
		FLOW_CREATE_CMD_FLOW_NOT_ADDED,
		FLOW_CREATE_CMD_FLOW_MAY_BE_ADDED,
	} flow_created; // refers only to emlc table, not to flow tracker
	bool timeouted;
	bool insert_mirror_flow; // insert mirror flow instead of base flow in case of mirroring
	struct n3k_mgmt_flow_tbl_handle *mirror_handle;
};

struct n3k_mgmt_flr_command_flow_destroy {
	struct n3k_mgmt_flow_entry_description desc;
	enum {
		FLOW_DESTROY_CMD_FLOW_DESTROYED,
		FLOW_DESTROY_CMD_FLOW_NOT_DESTROYED,
		FLOW_DESTROY_CMD_FLOW_MAY_BE_DESTROYED,
	} flow_destroyed; // refers only to emlc table, not to flow tracker
	bool destroy_mirror_flow; // destroy mirror flow instead of base flow in case of mirroring
	bool timeouted;
};

struct n3k_mgmt_flr_command_flow_query {
	struct n3k_mgmt_flow_tbl_handle *handle;
	rte_atomic16_t done;
	int error_code;
	struct n3k_mgmt_flow_tracker_stats stats;
};

struct n3k_mgmt_flr_command_read_reg_32 {
	uint32_t base;
	uint32_t offset;
	rte_atomic16_t done;
	int error_code;
	uint32_t out_result;
};

struct n3k_mgmt_flr_command_flow_dump {
	struct n3k_mgmt_flow_tbl_handle *handle;
	rte_atomic16_t done;
	int error_code;
	bool get_mirror_flow; // get mirror flow instead of base flow in case of mirroring
	struct n3k_mgmt_flow_tbl_entry *out_entry;
};

struct n3k_mgmt_flr_command_flow_handle {
	/* TODO(n3k): Add required fields */
};

struct n3k_mgmt_flr_command {
	struct n3k_mgmt_flr_command_metadata metadata;

	union {
		struct n3k_mgmt_flr_command_flow_create flow_create;
		struct n3k_mgmt_flr_command_flow_destroy flow_destroy;
		struct n3k_mgmt_flr_command_flow_query flow_query;
		struct n3k_mgmt_flr_command_flow_handle flow_handle;
		struct n3k_mgmt_flr_command_flow_dump flow_dump;
		struct n3k_mgmt_flr_command_read_reg_32 reg_32_read;
	};
};

int n3k_mgmt_flr_commands_init(struct n3k_mgmt_hw *hw);

struct n3k_mgmt_flr_command *n3k_mgmt_flr_command_alloc(
	struct n3k_mgmt_hw *hw, enum n3k_mgmt_flr_command_type type);
void n3k_mgmt_flr_command_free(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command *cmd);


int n3k_mgmt_flr_command_to_pkt(struct n3k_mgmt_hw *hw,
	const struct n3k_mgmt_flr_command *cmd, struct rte_mbuf *pkt);

int n3k_mgmt_flr_command_handle_response(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flr_command *cmd,
	const struct n3k_mgmt_flr_parsed_pkt *parsed_pkt);

static inline void
n3k_mgmt_flr_command_mark_processed(
	struct n3k_mgmt_flr_command *cmd, uint16_t packet_id)
{
	cmd->metadata.packet_id = packet_id;
	cmd->metadata.processed_count++;
