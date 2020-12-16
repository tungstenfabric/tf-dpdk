/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _RTE_PMD_N3K_SMAC_TBL_MGMT_H_
#define _RTE_PMD_N3K_SMAC_TBL_MGMT_H_

#include <rte_ethdev.h>
#include <rte_ether.h>
#include <stdint.h>

int rte_pmd_n3k_smac_tbl_cache_fetch(struct rte_eth_dev *dev,
	const struct rte_ether_addr* key,
	uint8_t* smac_id);

int rte_pmd_n3k_smac_tbl_cache_find(struct rte_eth_dev *dev,
	const struct rte_ether_addr* key,
	uint8_t* smac_id);

int rte_pmd_n3k_smac_tbl_cache_get_by_id(struct rte_eth_dev *dev,
	uint8_t smac_id, struct rte_ether_addr* out_key);

/*
 * Yielding a smac with ref_count = 0 may lead to undefined behaviour.
 */
int rte_pmd_n3k_smac_tbl_cache_yield(struct rte_eth_dev *dev, uint8_t smac_id);

