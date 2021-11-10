/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_TUNNEL_TBL_MGMT_H_
#define _PMD_NET_N3K_TUNNEL_TBL_MGMT_H_

#include <rte_ethdev.h>
#include <rte_ether.h>
#include <stdint.h>

enum n3k_mgmt_tunnel_type {
	N3K_MGMT_TUNNEL_TYPE_MPLSOUDP,
	N3K_MGMT_TUNNEL_TYPE_MPLSOGRE,
	N3K_MGMT_TUNNEL_TYPE_VXLAN,
};

struct n3k_mgmt_tunnel_tbl_entry_outer_l2 {
	uint8_t src_phy_port_id : 4;              /* An index to SMAC table. */
	uint8_t : 4;
	struct rte_ether_addr dst_mac;
	uint16_t eth_type;
};

struct n3k_mgmt_tunnel_tbl_entry_outer_l3 {
	uint32_t src_ip;
	uint32_t dst_ip;
	uint8_t tos;
	uint8_t ip_prot;
	uint8_t ttl;
};

struct n3k_mgmt_tunnel_tbl_entry_outer_l4 {
	uint16_t dst_port;
};

struct n3k_mgmt_tunnel_tbl_entry_vxlan {
	uint32_t vni : 24;
};

struct n3k_mgmt_tunnel_tbl_entry {
	enum n3k_mgmt_tunnel_type type;
	struct n3k_mgmt_tunnel_tbl_entry_outer_l2 l2;
	struct n3k_mgmt_tunnel_tbl_entry_outer_l3 l3;
	struct n3k_mgmt_tunnel_tbl_entry_outer_l4 l4;
	struct n3k_mgmt_tunnel_tbl_entry_vxlan    vxlan;
};

/**
 * Retrieves the tunnel_id for given key and increments the ref_count.
 * If the key is not in the cache, inserts it to the cache and hardware
 * and sets ref_count to 1. Does not assume ownership of the key struct.
 */
int rte_pmd_n3k_tunnel_tbl_cache_fetch(struct rte_eth_dev *dev,
	const struct n3k_mgmt_tunnel_tbl_entry* key,
	uint16_t* tunnel_id);

/**
 * Finds the tunnel_id for provided key, fails if not found.
 * Does not change the ref_count.
 */
int rte_pmd_n3k_tunnel_tbl_cache_find(struct rte_eth_dev *dev,
	const struct n3k_mgmt_tunnel_tbl_entry* key,
	uint16_t* tunnel_id);

/**
 * Gets the key by tunnel_id. Does not change the ref_count.
 * Copies the key to out_key in memory managed by the client code.
 */
int rte_pmd_n3k_tunnel_tbl_cache_get_by_id(struct rte_eth_dev *dev,
	uint16_t tunnel_id, struct n3k_mgmt_tunnel_tbl_entry* out_key);

/**
 * Decrements the ref_count of entry with given tunnel_id.
 * If ref_count reaches 0, deletes the entry from cache and hw.
 * Yielding a tunnel with ref_count = 0 may lead to undefined behaviour.
 */
int rte_pmd_n3k_tunnel_tbl_cache_yield(struct rte_eth_dev *dev, uint16_t tunnel_id);

int rte_pmd_n3k_tunnel_tbl_dump(struct rte_eth_dev *dev, uint16_t tunnel_idx);
void rte_pmd_n3k_tunnel_tbl_entry_dump(const struct n3k_mgmt_tunnel_tbl_entry *entry);

/* Full tunnel info */

struct n3k_mgmt_tunnel_entry {
	struct n3k_mgmt_tunnel_tbl_entry tunnel_raw;
	struct rte_ether_addr smac_raw;
};

int rte_pmd_n3k_tunnel_get(struct rte_eth_dev *dev,
	uint16_t tunnel_idx, struct n3k_mgmt_tunnel_entry *tunnel);

void rte_pmd_n3k_tunnel_entry_dump(const struct n3k_mgmt_tunnel_entry *tunnel);
int rte_pmd_n3k_tunnel_dump(struct rte_eth_dev *dev, uint16_t tunnel_idx);

/*
 * Adds given tunnel into tunnel and smac table.
 * Updates tunnel.tunnel_raw.l2.src_phy_port_id to id used to store given smac.
 */
int rte_pmd_n3k_tunnel_add(struct rte_eth_dev *dev,
	struct n3k_mgmt_tunnel_entry *tunnel,
	uint16_t *tunnel_idx);

int rte_pmd_n3k_tunnel_remove(struct rte_eth_dev *dev, uint16_t tunnel_idx,
	const struct n3k_mgmt_tunnel_entry *tunnel);

#endif /* _PMD_NET_N3K_TUNNEL_TBL_MGMT_H_ */
