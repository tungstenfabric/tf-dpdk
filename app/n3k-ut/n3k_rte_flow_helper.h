/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _N3K_RTE_FLOW_HELPER_
#define _N3K_RTE_FLOW_HELPER_

#include <rte_ether.h>
#include <rte_flow.h>
#include <rte_pmd_n3k_port.h>
#include <stdbool.h>

extern struct rte_flow_item pattern[];
extern struct rte_flow_action actions[];

void reset_patterns_and_actions(void);

void set_pattern_port_id(rte_le32_t port_id);
void set_pattern_outer_eth(
	struct rte_ether_addr src_mac, struct rte_ether_addr dst_mac);
void set_pattern_outer_ipv4(rte_le32_t src_ip, rte_le32_t dst_ip);
void set_pattern_outer_udp(rte_le16_t src_port, rte_le16_t dst_port);
void set_pattern_vxlan(rte_le32_t vni);
void set_pattern_mpls(rte_le32_t label, uint8_t tc);
void set_pattern_inner_eth(
	struct rte_ether_addr src_mac, struct rte_ether_addr dst_mac);
void set_pattern_inner_ipv4(
	rte_le32_t src_ip, rte_le32_t dst_ip, uint8_t ip_proto);
void set_pattern_inner_ipv6(
	const uint8_t *src_ip, const uint8_t *dst_ip, uint8_t ip_proto);
void set_pattern_inner_udp(rte_le16_t src_port, rte_le16_t dst_port);
void set_pattern_inner_tcp(rte_le16_t src_port, rte_le16_t dst_port);
void set_pattern_void(void);

void set_action_mirror(uint32_t port, uint8_t mirror_modified);
void set_action_set_mac_src(struct rte_ether_addr src_mac);
void set_action_set_mac_dst(struct rte_ether_addr dst_mac);
void set_action_dec_ttl(void);
void set_action_vxlan_encap(void);
void set_action_raw_encap(void);
void set_action_decap_vxlan(void);
void set_action_raw_decap_mpls(void);
void set_action_raw_decap_eth(void);
void set_action_ipv4_src(uint32_t ip);
void set_action_ipv4_dst(uint32_t ip);
void set_action_port_id(rte_le32_t port);
void set_action_drop(void);
void set_action_void(void);

void set_device_id(uint16_t dpdk_port,
	struct n3k_vdev_dev_id device_id);
struct n3k_vdev_dev_id get_device_id(uint16_t dpdk_port);
void clear_device_ids(void);

bool assert_device_id_equal(struct n3k_vdev_dev_id dev1,
	struct n3k_vdev_dev_id dev2);

void set_encap_eth(
	struct rte_ether_addr src_mac, struct rte_ether_addr dst_mac);
void set_encap_ipv4(
	rte_le32_t src_ip, rte_le32_t dst_ip, uint8_t ttl, uint8_t tos);
void set_encap_udp(rte_le16_t src_port, rte_le16_t dst_port);
void set_encap_mpls(rte_le32_t label, uint8_t tc);
void set_encap_eth_vxlan(struct rte_ether_addr src_mac, struct rte_ether_addr dst_mac);
void set_encap_ipv4_vxlan(rte_le32_t src_ip, rte_le32_t dst_ip, uint8_t ttl, uint8_t tos);
void set_encap_udp_vxlan(rte_le16_t src_port, rte_le16_t dst_port);
void set_encap_vxlan(rte_le32_t vni);

#endif /* _N3K_RTE_FLOW_HELPER_ */
