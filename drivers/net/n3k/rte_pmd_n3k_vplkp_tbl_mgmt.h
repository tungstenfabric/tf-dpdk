/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_VPLKP_TBL_MGMT_H_
#define _PMD_NET_N3K_VPLKP_TBL_MGMT_H_

#include <rte_ethdev.h>
#include <stdint.h>

enum n3k_mgmt_vplkp_tbl_vport_type {
	N3K_MGMT_VPLKP_TBL_VPORT_SLOWPATH = 0,
	N3K_MGMT_VPLKP_TBL_VPORT_L2PACKET = 1,
	N3K_MGMT_VPLKP_TBL_VPORT_L3PACKET = 2,
};

struct n3k_mgmt_vplkp_tbl_res {
	uint8_t keymask : 3;
	uint8_t : 5;
	enum n3k_mgmt_vplkp_tbl_vport_type vport;
};


int rte_pmd_n3k_vplkp_tbl_cache_insert(struct rte_eth_dev *dev,
	uint32_t mpls_label, const struct n3k_mgmt_vplkp_tbl_res *res);

int rte_pmd_n3k_vplkp_tbl_cache_get_by_label(struct rte_eth_dev *dev,
	uint32_t mpls_label, struct n3k_mgmt_vplkp_tbl_res *res);

uint32_t rte_pmd_n3k_vplkp_tbl_cache_get_ref_count(struct rte_eth_dev *dev,
	uint32_t mpls_label);

/*
 * Yielding a vplkp entry with ref_count = 0 may lead to undefined behaviour.
 */
int rte_pmd_n3k_vplkp_tbl_cache_yield(struct rte_eth_dev *dev, uint32_t mpls_label);

