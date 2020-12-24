/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_FLOW_TBL_MGMT_H_
#define _PMD_NET_N3K_FLOW_TBL_MGMT_H_

#include <rte_pmd_n3k_tunnel_tbl_mgmt.h>
#include <rte_pmd_n3k_vplkp_tbl_mgmt.h>
#include <rte_pmd_n3k_port.h>

#include <rte_ether.h>
#include <stdbool.h>
#include <stdint.h>

/* Layers info */

enum n3k_mgmt_flow_tbl_l3_type {
	N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4 = 1,
	// TODO(n3k): add other types
	// N3K_MGMT_FLOW_TBL_L3_TYPE_ARP = 2,
	// N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6 = 3,
};

struct n3k_mgmt_flow_tbl_l2 {
	struct rte_ether_addr src_mac;
	struct rte_ether_addr dst_mac;
};

struct n3k_mgmt_flow_tbl_l3 {
	enum n3k_mgmt_flow_tbl_l3_type type;
	union {
		struct {
			uint32_t src_ip;
			uint32_t dst_ip;
		} ipv4;
		// TODO(n3k): add ipv6
	};
	uint8_t ip_prot;
};

struct n3k_mgmt_flow_tbl_l4 {
	uint16_t src_port;
	uint16_t dst_port;
};

/* Key info */

struct n3k_mgmt_flow_tbl_key_tunnel_mpls_udp {
	uint32_t label : 20;
	uint32_t tos : 3;
	uint32_t : 9;
};

struct n3k_mgmt_flow_tbl_key_tunnel_vxlan {
	uint32_t vni;
};

struct n3k_mgmt_flow_tbl_key_tunnel {
	enum n3k_mgmt_tunnel_type type;
	struct n3k_mgmt_flow_tbl_l3 outer_l3;
	union {
		struct n3k_mgmt_flow_tbl_key_tunnel_mpls_udp mpls_udp;
		struct n3k_mgmt_flow_tbl_key_tunnel_vxlan vxlan;
		// TODO(n3k): add GRE?
	};
};

struct n3k_mgmt_flow_tbl_port {
	uint16_t port_id;
	struct n3k_vdev_dev_id device_id;
};

struct n3k_mgmt_flow_tbl_key {
	bool use_tunnel_key;
	bool use_l2_key; // setting to false works with use_tunnel_key = true only
	struct n3k_mgmt_flow_tbl_port port;
	struct n3k_mgmt_flow_tbl_l2 l2; // not used with l3 flows
	struct n3k_mgmt_flow_tbl_l3 l3;
	struct n3k_mgmt_flow_tbl_l4 l4;
	struct n3k_mgmt_flow_tbl_key_tunnel tunnel; // not used with local and encap flows
};

/* Action info */

enum n3k_mgmt_flow_tbl_mirror_mode {
	N3K_MGMT_FLOW_TBL_MIRROR_MODE_ORIGINAL,
	N3K_MGMT_FLOW_TBL_MIRROR_MODE_MODIFIED,
};

struct n3k_mgmt_flow_tbl_action_mirror {
	enum n3k_mgmt_flow_tbl_mirror_mode mode;
	struct n3k_mgmt_flow_tbl_port port;
};

struct n3k_mgmt_flow_tbl_action_decap {
	enum n3k_mgmt_tunnel_type type;
	bool insert_l2;
	struct n3k_mgmt_flow_tbl_l2 inserted_l2;
};

struct n3k_mgmt_flow_tbl_action_encap_mpls_udp {
	uint16_t src_port;
	uint32_t label : 20;
	uint32_t tos : 3;
	uint32_t : 9;
};

struct n3k_mgmt_flow_tbl_action_tunnel_vxlan {
	uint16_t src_port;
};

struct n3k_mgmt_flow_tbl_action_encap {
	enum n3k_mgmt_tunnel_type type;
	bool decr_ttl;
	bool strip_l2;
	uint16_t tunnel_idx : 12;
	uint16_t : 4;
	union {
		struct n3k_mgmt_flow_tbl_action_encap_mpls_udp mpls_udp;
		struct n3k_mgmt_flow_tbl_action_tunnel_vxlan vxlan;
		// TODO(n3k): add GRE?
	};
};

struct n3k_mgmt_flow_tbl_action_local {
	bool decr_ttl;
	bool modify_l2;
	struct n3k_mgmt_flow_tbl_l2 modified_l2;
};

enum n3k_mgmt_flow_tbl_action_type {
	N3K_MGMT_FLOW_TBL_ACTION_DECAP,
	N3K_MGMT_FLOW_TBL_ACTION_ENCAP,
	N3K_MGMT_FLOW_TBL_ACTION_LOCAL,
	N3K_MGMT_FLOW_TBL_ACTION_DROP,
};

enum n3k_mgmt_flow_tbl_nat_type {
	N3K_MGMT_FLOW_TBL_NAT_TYPE_NONE,
	N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT,
	N3K_MGMT_FLOW_TBL_NAT_TYPE_DNAT,
};

struct n3k_flow_tbl_action_nat {
	uint32_t modified_ip;
};

struct n3k_mgmt_flow_tbl_action {
	enum n3k_mgmt_flow_tbl_action_type type;
	enum n3k_mgmt_flow_tbl_nat_type nat_type;
	struct n3k_flow_tbl_action_nat nat;
	bool use_mirroring;
	struct n3k_mgmt_flow_tbl_action_mirror mirror;
	struct n3k_mgmt_flow_tbl_port port;
	uint16_t queue;
	union {
		struct n3k_mgmt_flow_tbl_action_decap decap_action;
		struct n3k_mgmt_flow_tbl_action_encap encap_action;
		struct n3k_mgmt_flow_tbl_action_local local_action;
	};
};

struct n3k_mgmt_flow_tbl_entry {
	struct n3k_mgmt_flow_tbl_key key;
	struct n3k_mgmt_flow_tbl_action action;
};

struct n3k_mgmt_flow_tbl_handle;

int rte_pmd_n3k_flow_tbl_add(
	struct rte_eth_dev *dev,
	const struct n3k_mgmt_flow_tbl_entry *entry,
	struct n3k_mgmt_flow_tbl_handle **handle);
int rte_pmd_n3k_flow_tbl_get(
	struct rte_eth_dev *dev,
	struct n3k_mgmt_flow_tbl_handle *handle,
	struct n3k_mgmt_flow_tbl_entry *entry);
int rte_pmd_n3k_flow_tbl_del(
	struct rte_eth_dev *dev, struct n3k_mgmt_flow_tbl_handle *handle);
void rte_pmd_n3k_flow_tbl_action_dump(
	const struct n3k_mgmt_flow_tbl_action *action);
void rte_pmd_n3k_flow_tbl_key_dump(
	const struct n3k_mgmt_flow_tbl_key *key);
void rte_pmd_n3k_flow_tbl_entry_dump(
	const struct n3k_mgmt_flow_tbl_entry *entry);
int rte_pmd_n3k_flow_tbl_dump(
	struct rte_eth_dev *dev, struct n3k_mgmt_flow_tbl_handle *handle);

/* Flow statistics */

struct n3k_mgmt_general_flow_stats {
	uint32_t psg_hit_cnt; // number of eml hits (flow matched)
	uint32_t psg_miss_cnt; // number of eml misses (flow didn't match)
	uint32_t def_hit_cnt; // hits on default lookup
	uint32_t def_miss_cnt; // misses on default lookup
	uint32_t emlc_total_entries; // number of flows
};

int rte_pmd_n3k_general_flow_stats_get(struct rte_eth_dev *dev,
	struct n3k_mgmt_general_flow_stats *stats);
int rte_pmd_n3k_general_flow_stats_dump(struct rte_eth_dev *dev);

struct n3k_mgmt_flow_tracker_stats {
	uint64_t bytes_cnt;
	uint64_t packet_cnt;
	uint32_t life_time;
};

int rte_pmd_n3k_flow_tracker_get(
	struct rte_eth_dev *dev,
	struct n3k_mgmt_flow_tbl_handle *handle,
	struct n3k_mgmt_flow_tracker_stats *ft_stats);
int rte_pmd_n3k_flow_tracker_dump(
	struct rte_eth_dev *dev, struct n3k_mgmt_flow_tbl_handle *handle);
void rte_pmd_n3k_flow_tracker_struct_dump(
	const struct n3k_mgmt_flow_tracker_stats *ft_stats);

/* Full flow info */

struct n3k_mgmt_flow_key {
	struct n3k_mgmt_flow_tbl_key key_raw;
	struct n3k_mgmt_vplkp_tbl_res vplkp_raw;
};

struct n3k_mgmt_flow_action {
	struct n3k_mgmt_flow_tbl_action action_raw;
	struct n3k_mgmt_tunnel_entry tunnel;
};

struct n3k_mgmt_flow_entry {
	struct n3k_mgmt_flow_key key;
	struct n3k_mgmt_flow_action action;
};

int rte_pmd_n3k_flow_get(struct rte_eth_dev *dev,
	struct n3k_mgmt_flow_tbl_handle *handle,
	struct n3k_mgmt_flow_entry *flow);

void rte_pmd_n3k_flow_action_dump(const struct n3k_mgmt_flow_action *action);
void rte_pmd_n3k_flow_key_dump(const struct n3k_mgmt_flow_key *key);
void rte_pmd_n3k_flow_entry_dump(const struct n3k_mgmt_flow_entry *entry);
int rte_pmd_n3k_flow_dump(struct rte_eth_dev *dev, struct n3k_mgmt_flow_tbl_handle *handle);

/*
 * Adds given flow into flow table and into some of smac, tunnel and vplkp
 * tables depending on flow type.
 * Updates flow.action.action_raw.encap_action.tunnel_idx to id used to store
 * given tunnel and flow.action.tunnel.tunnel_raw.l2.src_phy_port_id to id used
 * to store given smac in case of ENCAP flows.
 */
int rte_pmd_n3k_flow_add(struct rte_eth_dev *dev, struct n3k_mgmt_flow_entry *flow,
	struct n3k_mgmt_flow_tbl_handle **handle);

int rte_pmd_n3k_flow_remove(struct rte_eth_dev *dev,
	struct n3k_mgmt_flow_tbl_handle *handle,
	const struct n3k_mgmt_flow_entry *flow);

#endif /* _PMD_NET_N3K_FLOW_TBL_MGMT_H_ */
