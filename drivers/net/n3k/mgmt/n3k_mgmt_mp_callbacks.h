/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_MP_CALLBACKS_H_
#define _PMD_NET_N3K_MGMT_MP_CALLBACKS_H_

#include "../vdev/n3k_vdev_device.h"
#include "n3k_mgmt_flow_tbl_handles.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_vplkp_tbl_cache.h"

#include <rte_pmd_n3k_mp.h>

#include <stdint.h>

struct n3k_mgmt_mp_table_iterator {
	enum n3k_mgmt_mp_request_table type;
	union {
		struct n3k_mgmt_flow_tbl_handle *flow_tbl_iterator;
		struct n3k_vdev_dev_id port_iterator;
		uint16_t tunnel_tbl_iterator;
		uint8_t smac_tbl_iterator;
		struct n3k_mgmt_vplkp_tbl_cache_iterator vplkp_tbl_iterator;
	};
};

int
n3k_mgmt_mp_init_iterator(struct n3k_mgmt_hw *hw,
	enum n3k_mgmt_mp_request_table type,
	struct n3k_mgmt_mp_table_iterator *iterator);

int
n3k_mgmt_mp_increment_iterator(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_mp_table_iterator *iterator);

int
n3k_mgmt_mp_fill_entries_response_csr(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_mp_table_iterator *iterator,
	struct n3k_mgmt_mp_entries_response *response);

int
n3k_mgmt_mp_fill_response(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_mp_table_iterator *iterator,
	struct n3k_mgmt_mp_entries_response *response);

int
n3k_mgmt_mp_fill_total_entries_response(struct n3k_mgmt_hw *hw,
	enum n3k_mgmt_mp_request_table type,
	struct n3k_mgmt_mp_total_entries_response *response);

int
n3k_mgmt_mp_fill_register_response(
	struct n3k_mgmt_hw *hw,
