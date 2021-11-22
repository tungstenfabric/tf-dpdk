/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "mgmt/n3k_mgmt_rte_flow_parse.h"

#include "n3k-ut.h"
#include "n3k_rte_flow_helper.h"

static struct n3k_vdev_dev_id SRC_DEVICE = {
	.type = N3K_VDEV_DEV_TYPE_VF,
	.id = 1,
};
static struct n3k_vdev_dev_id DST_DEVICE = {
	.type = N3K_VDEV_DEV_TYPE_VF,
	.id = 2
};
static struct n3k_vdev_dev_id DST_DEVICE_MIRROR = {
	.type = N3K_VDEV_DEV_TYPE_VF,
	.id = 3
};

static const rte_le32_t SRC_PORT_ID = 7;
static const rte_le32_t DST_PORT_ID = 11;
static const rte_le32_t DST_PORT_ID_MIRROR = 13;

static const struct rte_ether_addr SRC_MAC = {
	.addr_bytes = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x33 }
};

static const struct rte_ether_addr DST_MAC = {
	.addr_bytes = { 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff }
};

#define MOD_PCP 3
#define PCP 5
#define MOD_VID 65
#define VID 192
static const rte_le16_t VLAN_PCP = PCP;
static const rte_le16_t VLAN_VID = VID;
static const rte_le16_t VLAN_TCI = VID | (PCP << 13);
static const rte_le16_t MOD_VLAN_PCP = MOD_PCP;
static const rte_le16_t MOD_VLAN_VID = MOD_VID;
static const rte_le16_t MOD_VLAN_TCI = MOD_VID | (MOD_PCP << 13);

static const rte_le32_t SRC_IP = RTE_IPV4(1, 2, 3, 4);
static const rte_le32_t DST_IP = RTE_IPV4(10, 11, 12, 13);

static const uint8_t SRC_IP_IPv6[IPv6_ADDR_LEN] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
	0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x00, 0x0f, 0x0e };

static const uint8_t DST_IP_IPv6[IPv6_ADDR_LEN] = { 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
	0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x10, 0x1f, 0x1e };

static const rte_le32_t NEW_SRC_IP = RTE_IPV4(88, 88, 88, 88);
static const rte_le32_t NEW_DST_IP = RTE_IPV4(77, 77, 77, 77);

static const rte_le16_t SRC_PORT = 1234;
static const rte_le16_t DST_PORT = 5678;

static const struct rte_ether_addr NEW_SRC_MAC = {
	.addr_bytes = { 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6 }
};

static const struct rte_ether_addr NEW_DST_MAC = {
	.addr_bytes = { 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6 }
};

static const struct rte_ether_addr OUTER_SRC_MAC = {
	.addr_bytes = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x99 }
};

static const struct rte_ether_addr OUTER_DST_MAC = {
	.addr_bytes = { 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0x99 }
};

static const struct rte_ether_addr ROUTER_MAC = {
	.addr_bytes = { 0x99, 0x88, 0x77, 0x66, 0x55, 0x44 }
};

static const rte_le32_t OUTER_SRC_IP = RTE_IPV4(110, 120, 130, 140);
static const rte_le32_t OUTER_DST_IP = RTE_IPV4(210, 220, 230, 240);
static const uint8_t OUTER_TTL = 67;
static const uint8_t DEFAULT_TTL = 64;
static const uint8_t OUTER_TOS = 3;

static const rte_le16_t OUTER_SRC_PORT = 43210;
static const rte_le16_t OUTER_DST_PORT = 6635;

static const rte_le32_t VXLAN_VNI = 0x859;

static const rte_le32_t MPLS_LABEL = 0xABCDE;
static const uint8_t MPLS_TC = 2;

static int
flow_conversion_setup(void **state __rte_unused)
{
	set_device_id(SRC_PORT_ID, SRC_DEVICE);
	set_device_id(DST_PORT_ID, DST_DEVICE);
	set_device_id(DST_PORT_ID_MIRROR, DST_DEVICE_MIRROR);
	return 0;
}

static int
flow_conversion_teardown(void **state __rte_unused)
{
	clear_device_ids();
	return 0;
}

static void
assert_inner_key_matches(struct n3k_mgmt_flow_tbl_key *key, uint8_t proto,
		bool use_l2, enum n3k_mgmt_flow_tbl_l3_type expected_ip_version)
{
	if (use_l2) {
		assert_true(key->use_l2_key);
		assert_memory_equal(&key->l2.src_mac, &SRC_MAC, RTE_ETHER_ADDR_LEN);
		assert_memory_equal(&key->l2.dst_mac, &DST_MAC, RTE_ETHER_ADDR_LEN);
	} else {
		assert_false(key->use_l2_key);
	}

	assert_int_equal(key->l3.type, expected_ip_version);

	switch(expected_ip_version) {
		case N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4:
			assert_int_equal(key->l3.ipv4.src_ip, SRC_IP);
			assert_int_equal(key->l3.ipv4.dst_ip, DST_IP);
			break;

		case N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6:
			assert_memory_equal(key->l3.ipv6.src_ip, SRC_IP_IPv6, IPv6_ADDR_LEN);
			assert_memory_equal(key->l3.ipv6.dst_ip, DST_IP_IPv6, IPv6_ADDR_LEN);
			break;
	}

	assert_int_equal(key->l3.ip_prot, proto);
	assert_int_equal(key->l4.src_port, SRC_PORT);
	assert_int_equal(key->l4.dst_port, DST_PORT);
}

static void
assert_outer_key_matches(struct n3k_mgmt_flow_key *key,
	bool use_l2,
	enum n3k_mgmt_tunnel_type tunnel_type)
{
	struct n3k_mgmt_vplkp_tbl_res *vplkp = &key->vplkp_raw;
	struct n3k_mgmt_flow_tbl_key_tunnel *tunnel = &key->key_raw.tunnel;

	assert_int_equal(tunnel->outer_l3.type, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_int_equal(tunnel->outer_l3.ipv4.src_ip, OUTER_SRC_IP);
	assert_int_equal(tunnel->outer_l3.ipv4.dst_ip, OUTER_DST_IP);
	assert_int_equal(tunnel->outer_l3.ip_prot, IPPROTO_UDP);

	if (tunnel_type == N3K_MGMT_TUNNEL_TYPE_MPLSOUDP) {
		assert_int_equal(tunnel->type, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
		assert_int_equal(tunnel->mpls_udp.label, MPLS_LABEL);
		assert_int_equal(tunnel->mpls_udp.tos, MPLS_TC);
	}
	else if (tunnel_type == N3K_MGMT_TUNNEL_TYPE_VXLAN) {
		assert_int_equal(tunnel->type, tunnel_type);
		assert_int_equal(tunnel->vxlan.vni, VXLAN_VNI);
	}

	assert_int_equal(vplkp->vport, use_l2 ?
		N3K_MGMT_VPLKP_TBL_VPORT_L2PACKET : N3K_MGMT_VPLKP_TBL_VPORT_L3PACKET);
}

static void
assert_encap_action_matches(struct n3k_mgmt_flow_action *action,
	bool is_l3,
	enum n3k_mgmt_tunnel_type tunnel_type)
{
	struct n3k_mgmt_flow_tbl_action *action_raw = &action->action_raw;

	if (is_l3) {
		assert_true(action_raw->encap_action.strip_l2);
		assert_true(action_raw->encap_action.decr_ttl);
	} else {
		assert_false(action_raw->encap_action.strip_l2);
		assert_false(action_raw->encap_action.decr_ttl);
	}

	struct n3k_mgmt_tunnel_tbl_entry *tunnel = &action->tunnel.tunnel_raw;
	struct rte_ether_addr *smac = &action->tunnel.smac_raw;

	if(tunnel_type == N3K_MGMT_TUNNEL_TYPE_MPLSOUDP) {
		assert_int_equal(action_raw->encap_action.type,
				 N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
		assert_int_equal(action_raw->encap_action.mpls_udp.src_port, OUTER_SRC_PORT);
		assert_int_equal(action_raw->encap_action.mpls_udp.label, MPLS_LABEL);
		assert_int_equal(action_raw->encap_action.mpls_udp.tos, MPLS_TC);
		assert_int_equal(tunnel->l3.ttl, OUTER_TTL);
		assert_int_equal(tunnel->type, tunnel_type);
	}
	else if (tunnel_type == N3K_MGMT_TUNNEL_TYPE_VXLAN) {
		assert_int_equal(action_raw->encap_action.type,
				 N3K_MGMT_TUNNEL_TYPE_VXLAN);
		assert_int_equal(action_raw->encap_action.vxlan.src_port,
				 OUTER_SRC_PORT);
		assert_int_equal(tunnel->vxlan.vni, VXLAN_VNI);

		assert_int_equal(tunnel->l3.ttl, DEFAULT_TTL);
		assert_int_equal(tunnel->l3.tos, OUTER_TOS);
		assert_int_equal(tunnel->type, tunnel_type);
	}

	assert_memory_equal(smac, &OUTER_SRC_MAC, RTE_ETHER_ADDR_LEN);
	assert_memory_equal(
		&tunnel->l2.dst_mac, &OUTER_DST_MAC, RTE_ETHER_ADDR_LEN);
	assert_int_equal(tunnel->l2.eth_type, RTE_ETHER_TYPE_IPV4);

	assert_int_equal(tunnel->l3.src_ip, OUTER_SRC_IP);
	assert_int_equal(tunnel->l3.dst_ip, OUTER_DST_IP);
	assert_int_equal(tunnel->l3.tos, OUTER_TOS);
	assert_int_equal(tunnel->l3.ip_prot, IPPROTO_UDP);
	assert_int_equal(tunnel->l4.dst_port, OUTER_DST_PORT);
}

static void
assert_decap_action_matches(
	struct n3k_mgmt_flow_action *action, bool insert_l2)
{
	struct n3k_mgmt_flow_tbl_action *action_raw = &action->action_raw;

	assert_int_equal(
		action_raw->decap_action.type, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	if (insert_l2) {
		assert_true(action_raw->decap_action.insert_l2);
		struct n3k_mgmt_flow_tbl_l2 *l2 = &action_raw->decap_action.inserted_l2;
		assert_memory_equal(&l2->src_mac, &ROUTER_MAC, RTE_ETHER_ADDR_LEN);
		assert_memory_equal(&l2->dst_mac, &DST_MAC, RTE_ETHER_ADDR_LEN);
	} else {
		assert_false(action_raw->decap_action.insert_l2);
	}
}

static void
assert_decap_vxlan_action_matches(
	struct n3k_mgmt_flow_action *action)
{
	struct n3k_mgmt_flow_tbl_action *action_raw = &action->action_raw;

	assert_int_equal(
		action_raw->decap_action.type, N3K_MGMT_TUNNEL_TYPE_VXLAN);

	assert_false(action_raw->decap_action.insert_l2);
}

static void
test_local_udp_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_local_udp_l2_insert_vlan_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_push_vlan();
	set_action_set_vlan_vid(VLAN_VID);
	set_action_set_vlan_pcp(VLAN_PCP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_INSERT);
	assert_int_equal(action->vlan.tci, VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_local_udp_l2_modify_vlan_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_set_vlan_vid(MOD_VLAN_VID);
	set_action_set_vlan_pcp(MOD_VLAN_PCP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_MOD);
	assert_int_equal(action->vlan.tci, MOD_VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);

	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_local_udp_l2_strip_vlan_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_pop_vlan();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_STRIP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_local_udp_l2_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);

	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_local_udp_vlan_l2_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);

	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}


static void
test_local_snat_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_ipv4_src(NEW_SRC_IP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT);
	assert_int_equal(action->nat.modified_ip, NEW_SRC_IP);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_local_snat_insert_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_push_vlan();
	set_action_set_vlan_vid(VLAN_VID);
	set_action_set_vlan_pcp(VLAN_PCP);
	set_action_ipv4_src(NEW_SRC_IP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_INSERT);
	assert_int_equal(action->vlan.tci, VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT);
	assert_int_equal(action->nat.modified_ip, NEW_SRC_IP);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_local_snat_modify_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_set_vlan_vid(MOD_VLAN_VID);
	set_action_set_vlan_pcp(MOD_VLAN_PCP);
	set_action_ipv4_src(NEW_SRC_IP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_MOD);
	assert_int_equal(action->vlan.tci, MOD_VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT);
	assert_int_equal(action->nat.modified_ip, NEW_SRC_IP);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_local_snat_strip_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_pop_vlan();
	set_action_ipv4_src(NEW_SRC_IP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_STRIP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT);
	assert_int_equal(action->nat.modified_ip, NEW_SRC_IP);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_local_dnat_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_ipv4_dst(NEW_DST_IP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_DNAT);
	assert_int_equal(action->nat.modified_ip, NEW_DST_IP);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_local_udp_l3_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_set_mac_src(NEW_SRC_MAC);
	set_action_set_mac_dst(NEW_DST_MAC);
	set_action_dec_ttl();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_true(action->local_action.modify_l2);
	assert_true(action->local_action.decr_ttl);

	struct n3k_mgmt_flow_tbl_l2 *new_l2 = &action->local_action.modified_l2;
	assert_memory_equal(
		new_l2->src_mac.addr_bytes, NEW_SRC_MAC.addr_bytes, RTE_ETHER_ADDR_LEN);
	assert_memory_equal(
		new_l2->dst_mac.addr_bytes, NEW_DST_MAC.addr_bytes, RTE_ETHER_ADDR_LEN);
}

static void
test_local_udp_insert_vlan_l3_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_set_mac_src(NEW_SRC_MAC);
	set_action_set_mac_dst(NEW_DST_MAC);
	set_action_dec_ttl();

	set_action_push_vlan();
	set_action_set_vlan_vid(VLAN_VID);
	set_action_set_vlan_pcp(VLAN_PCP);

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_INSERT);
	assert_int_equal(action->vlan.tci, VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_true(action->local_action.modify_l2);
	assert_true(action->local_action.decr_ttl);

	struct n3k_mgmt_flow_tbl_l2 *new_l2 = &action->local_action.modified_l2;
	assert_memory_equal(
		new_l2->src_mac.addr_bytes, NEW_SRC_MAC.addr_bytes, RTE_ETHER_ADDR_LEN);
	assert_memory_equal(
		new_l2->dst_mac.addr_bytes, NEW_DST_MAC.addr_bytes, RTE_ETHER_ADDR_LEN);
}

static void
test_local_udp_modify_vlan_l3_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_set_mac_src(NEW_SRC_MAC);
	set_action_set_mac_dst(NEW_DST_MAC);
	set_action_dec_ttl();

	set_action_set_vlan_vid(MOD_VLAN_VID);
	set_action_set_vlan_pcp(MOD_VLAN_PCP);

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_MOD);
	assert_int_equal(action->vlan.tci, MOD_VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_true(action->local_action.modify_l2);
	assert_true(action->local_action.decr_ttl);

	struct n3k_mgmt_flow_tbl_l2 *new_l2 = &action->local_action.modified_l2;
	assert_memory_equal(
		new_l2->src_mac.addr_bytes, NEW_SRC_MAC.addr_bytes, RTE_ETHER_ADDR_LEN);
	assert_memory_equal(
		new_l2->dst_mac.addr_bytes, NEW_DST_MAC.addr_bytes, RTE_ETHER_ADDR_LEN);
}

static void
test_local_udp_strip_vlan_l3_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_set_mac_src(NEW_SRC_MAC);
	set_action_set_mac_dst(NEW_DST_MAC);
	set_action_dec_ttl();

	set_action_pop_vlan();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_STRIP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_true(action->local_action.modify_l2);
	assert_true(action->local_action.decr_ttl);

	struct n3k_mgmt_flow_tbl_l2 *new_l2 = &action->local_action.modified_l2;
	assert_memory_equal(
		new_l2->src_mac.addr_bytes, NEW_SRC_MAC.addr_bytes, RTE_ETHER_ADDR_LEN);
	assert_memory_equal(
		new_l2->dst_mac.addr_bytes, NEW_DST_MAC.addr_bytes, RTE_ETHER_ADDR_LEN);
}

static void
test_local_udp_l3_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_set_mac_src(NEW_SRC_MAC);
	set_action_set_mac_dst(NEW_DST_MAC);
	set_action_dec_ttl();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_true(action->local_action.modify_l2);
	assert_true(action->local_action.decr_ttl);

	struct n3k_mgmt_flow_tbl_l2 *new_l2 = &action->local_action.modified_l2;
	assert_memory_equal(
		new_l2->src_mac.addr_bytes, NEW_SRC_MAC.addr_bytes, RTE_ETHER_ADDR_LEN);
	assert_memory_equal(
		new_l2->dst_mac.addr_bytes, NEW_DST_MAC.addr_bytes, RTE_ETHER_ADDR_LEN);
}

static void
test_local_snat_l3_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_set_mac_src(NEW_SRC_MAC);
	set_action_set_mac_dst(NEW_DST_MAC);
	set_action_dec_ttl();
	set_action_ipv4_src(NEW_SRC_IP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_true(action->local_action.modify_l2);
	assert_true(action->local_action.decr_ttl);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT);
	assert_int_equal(action->nat.modified_ip, NEW_SRC_IP);

	struct n3k_mgmt_flow_tbl_l2 *new_l2 = &action->local_action.modified_l2;
	assert_memory_equal(
		new_l2->src_mac.addr_bytes, NEW_SRC_MAC.addr_bytes, RTE_ETHER_ADDR_LEN);
	assert_memory_equal(
		new_l2->dst_mac.addr_bytes, NEW_DST_MAC.addr_bytes, RTE_ETHER_ADDR_LEN);
}

static void
test_local_dnat_l3_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_set_mac_src(NEW_SRC_MAC);
	set_action_set_mac_dst(NEW_DST_MAC);
	set_action_dec_ttl();
	set_action_ipv4_dst(NEW_DST_IP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_true(action->local_action.modify_l2);
	assert_true(action->local_action.decr_ttl);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_DNAT);
	assert_int_equal(action->nat.modified_ip, NEW_DST_IP);

	struct n3k_mgmt_flow_tbl_l2 *new_l2 = &action->local_action.modified_l2;
	assert_memory_equal(
		new_l2->src_mac.addr_bytes, NEW_SRC_MAC.addr_bytes, RTE_ETHER_ADDR_LEN);
	assert_memory_equal(
		new_l2->dst_mac.addr_bytes, NEW_DST_MAC.addr_bytes, RTE_ETHER_ADDR_LEN);
}

static void
test_local_tcp_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_local_tcp_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_local_tcp_l2_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_local_tcp_vlan_l2_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_local_tcp_l3_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_set_mac_src(NEW_SRC_MAC);
	set_action_set_mac_dst(NEW_DST_MAC);
	set_action_dec_ttl();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_true(action->local_action.modify_l2);
	assert_true(action->local_action.decr_ttl);

	struct n3k_mgmt_flow_tbl_l2 *new_l2 = &action->local_action.modified_l2;
	assert_memory_equal(
		new_l2->src_mac.addr_bytes, NEW_SRC_MAC.addr_bytes, RTE_ETHER_ADDR_LEN);
	assert_memory_equal(
		new_l2->dst_mac.addr_bytes, NEW_DST_MAC.addr_bytes, RTE_ETHER_ADDR_LEN);
}

static void
test_local_tcp_l3_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_set_mac_src(NEW_SRC_MAC);
	set_action_set_mac_dst(NEW_DST_MAC);
	set_action_dec_ttl();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_true(action->local_action.modify_l2);
	assert_true(action->local_action.decr_ttl);

	struct n3k_mgmt_flow_tbl_l2 *new_l2 = &action->local_action.modified_l2;
	assert_memory_equal(
		new_l2->src_mac.addr_bytes, NEW_SRC_MAC.addr_bytes, RTE_ETHER_ADDR_LEN);
	assert_memory_equal(
		new_l2->dst_mac.addr_bytes, NEW_DST_MAC.addr_bytes, RTE_ETHER_ADDR_LEN);
}

static void
test_local_udp_drop(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_drop();

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DROP);
}

static void
test_local_udp_vlan_drop(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_drop();

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DROP);
}

static void
test_local_udp_drop_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_drop();

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DROP);
}

static void
test_local_udp_vlan_drop_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_drop();

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DROP);
}

static void
test_local_tcp_drop(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_drop();

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DROP);
}

static void
test_local_tcp_vlan_drop(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_drop();

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DROP);
}

static void
test_local_tcp_drop_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_drop();

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DROP);
}

static void
test_local_tcp_vlan_drop_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_drop();

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DROP);
}

static void
test_local_l2_mirror_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_port_id(DST_PORT_ID);
	set_action_mirror(DST_PORT_ID_MIRROR, 0);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->mirror.mode, 0);
	assert_int_equal(action->mirror.port.port_id, DST_PORT_ID_MIRROR);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_local_insert_vlan_l2_mirror_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_push_vlan();
	set_action_set_vlan_vid(VLAN_VID);
	set_action_set_vlan_pcp(VLAN_PCP);
	set_action_port_id(DST_PORT_ID);
	set_action_mirror(DST_PORT_ID_MIRROR, 0);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_INSERT);
	assert_int_equal(action->vlan.tci, VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->mirror.mode, 0);
	assert_int_equal(action->mirror.port.port_id, DST_PORT_ID_MIRROR);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_local_modify_vlan_l2_mirror_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_set_vlan_vid(MOD_VLAN_VID);
	set_action_set_vlan_pcp(MOD_VLAN_PCP);
	set_action_port_id(DST_PORT_ID);
	set_action_mirror(DST_PORT_ID_MIRROR, 0);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_MOD);
	assert_int_equal(action->vlan.tci, MOD_VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->mirror.mode, 0);
	assert_int_equal(action->mirror.port.port_id, DST_PORT_ID_MIRROR);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_local_strip_vlan_l2_mirror_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_pop_vlan();
	set_action_port_id(DST_PORT_ID);
	set_action_mirror(DST_PORT_ID_MIRROR, 0);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_STRIP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->mirror.mode, 0);
	assert_int_equal(action->mirror.port.port_id, DST_PORT_ID_MIRROR);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_local_l2_mirror_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_port_id(DST_PORT_ID);
	set_action_mirror(DST_PORT_ID_MIRROR, 0);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->mirror.mode, 0);
	assert_int_equal(action->mirror.port.port_id, DST_PORT_ID_MIRROR);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_local_l3_mirror_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_set_mac_src(NEW_SRC_MAC);
	set_action_set_mac_dst(NEW_DST_MAC);
	set_action_dec_ttl();
	set_action_port_id(DST_PORT_ID);
	set_action_mirror(DST_PORT_ID_MIRROR, 0);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->mirror.mode, 0);
	assert_int_equal(action->mirror.port.port_id, DST_PORT_ID_MIRROR);

	assert_true(action->local_action.modify_l2);
	assert_true(action->local_action.decr_ttl);

	struct n3k_mgmt_flow_tbl_l2 *new_l2 = &action->local_action.modified_l2;
	assert_memory_equal(
		new_l2->src_mac.addr_bytes, NEW_SRC_MAC.addr_bytes, RTE_ETHER_ADDR_LEN);
	assert_memory_equal(
		new_l2->dst_mac.addr_bytes, NEW_DST_MAC.addr_bytes, RTE_ETHER_ADDR_LEN);
}

static void
test_local_l3_mirror_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_set_mac_src(NEW_SRC_MAC);
	set_action_set_mac_dst(NEW_DST_MAC);
	set_action_dec_ttl();
	set_action_port_id(DST_PORT_ID);
	set_action_mirror(DST_PORT_ID_MIRROR, 0);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->mirror.mode, 0);
	assert_int_equal(action->mirror.port.port_id, DST_PORT_ID_MIRROR);

	assert_true(action->local_action.modify_l2);
	assert_true(action->local_action.decr_ttl);

	struct n3k_mgmt_flow_tbl_l2 *new_l2 = &action->local_action.modified_l2;
	assert_memory_equal(
		new_l2->src_mac.addr_bytes, NEW_SRC_MAC.addr_bytes, RTE_ETHER_ADDR_LEN);
	assert_memory_equal(
		new_l2->dst_mac.addr_bytes, NEW_DST_MAC.addr_bytes, RTE_ETHER_ADDR_LEN);
}

static void
test_egress_mplsoudp_udp_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_egress_mplsoudp_udp_insert_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_push_vlan();
	set_action_set_vlan_vid(VLAN_VID);
	set_action_set_vlan_pcp(VLAN_PCP);

	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_INSERT);
	assert_int_equal(action->vlan.tci, VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_egress_mplsoudp_udp_modify_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_set_vlan_vid(MOD_VLAN_VID);
	set_action_set_vlan_pcp(MOD_VLAN_PCP);

	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_MOD);
	assert_int_equal(action->vlan.tci, MOD_VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_egress_mplsoudp_udp_strip_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_pop_vlan();

	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_STRIP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_egress_mplsoudp_udp_l2_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_egress_mplsoudp_udp_vlan_l2_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}


static void
test_egress_mplsoudp_snat_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_ipv4_src(NEW_SRC_IP);

	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT);
	assert_int_equal(action->nat.modified_ip, NEW_SRC_IP);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_egress_mplsoudp_snat_insert_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_push_vlan();
	set_action_set_vlan_vid(VLAN_VID);
	set_action_set_vlan_pcp(VLAN_PCP);
	set_action_ipv4_src(NEW_SRC_IP);

	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_INSERT);
	assert_int_equal(action->vlan.tci, VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT);
	assert_int_equal(action->nat.modified_ip, NEW_SRC_IP);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_egress_mplsoudp_snat_modify_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_set_vlan_vid(MOD_VLAN_VID);
	set_action_set_vlan_pcp(MOD_VLAN_PCP);
	set_action_ipv4_src(NEW_SRC_IP);

	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_MOD);
	assert_int_equal(action->vlan.tci, MOD_VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT);
	assert_int_equal(action->nat.modified_ip, NEW_SRC_IP);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_egress_mplsoudp_snat_strip_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_pop_vlan();
	set_action_ipv4_src(NEW_SRC_IP);

	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_STRIP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT);
	assert_int_equal(action->nat.modified_ip, NEW_SRC_IP);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_egress_mplsoudp_tcp_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_egress_mplsoudp_tcp_l2_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_missing_pattern(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	//set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, -EINVAL);
}

static void
test_missing_pattern_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	//set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, -EINVAL);
}

static void
test_egress_vxlan_udp_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_encap_eth_vxlan(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4_vxlan(OUTER_SRC_IP, OUTER_DST_IP, DEFAULT_TTL, OUTER_TOS);
	set_encap_udp_vxlan(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_vxlan(VXLAN_VNI);
	set_action_vxlan_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_VXLAN);
}

static void
test_egress_vxlan_udp_insert_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_push_vlan();
	set_action_set_vlan_vid(VLAN_VID);
	set_action_set_vlan_pcp(VLAN_PCP);

	set_encap_eth_vxlan(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4_vxlan(OUTER_SRC_IP, OUTER_DST_IP, DEFAULT_TTL, OUTER_TOS);
	set_encap_udp_vxlan(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_vxlan(VXLAN_VNI);
	set_action_vxlan_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_INSERT);
	assert_int_equal(action->vlan.tci, VLAN_TCI);
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_VXLAN);
}

static void
test_egress_vxlan_udp_modify_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_set_vlan_vid(MOD_VLAN_VID);
	set_action_set_vlan_pcp(MOD_VLAN_PCP);

	set_encap_eth_vxlan(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4_vxlan(OUTER_SRC_IP, OUTER_DST_IP, DEFAULT_TTL, OUTER_TOS);
	set_encap_udp_vxlan(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_vxlan(VXLAN_VNI);
	set_action_vxlan_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_MOD);
	assert_int_equal(action->vlan.tci, MOD_VLAN_TCI);
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_VXLAN);
}

static void
test_egress_vxlan_udp_strip_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_pop_vlan();

	set_encap_eth_vxlan(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4_vxlan(OUTER_SRC_IP, OUTER_DST_IP, DEFAULT_TTL, OUTER_TOS);
	set_encap_udp_vxlan(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_vxlan(VXLAN_VNI);
	set_action_vxlan_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_STRIP);
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_VXLAN);
}

static void
test_egress_vxlan_udp_l2_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_encap_eth_vxlan(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4_vxlan(OUTER_SRC_IP, OUTER_DST_IP, DEFAULT_TTL, OUTER_TOS);
	set_encap_udp_vxlan(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_vxlan(VXLAN_VNI);
	set_action_vxlan_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_VXLAN);
}

static void
test_egress_vxlan_snat_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_ipv4_src(NEW_SRC_IP);

	set_encap_eth_vxlan(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4_vxlan(OUTER_SRC_IP, OUTER_DST_IP, DEFAULT_TTL, OUTER_TOS);
	set_encap_udp_vxlan(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_vxlan(VXLAN_VNI);
	set_action_vxlan_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT);
	assert_int_equal(action->nat.modified_ip, NEW_SRC_IP);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_VXLAN);
}

static void
test_egress_vxlan_snat_insert_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_push_vlan();
	set_action_set_vlan_vid(VLAN_VID);
	set_action_set_vlan_pcp(VLAN_PCP);
	set_action_ipv4_src(NEW_SRC_IP);

	set_encap_eth_vxlan(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4_vxlan(OUTER_SRC_IP, OUTER_DST_IP, DEFAULT_TTL, OUTER_TOS);
	set_encap_udp_vxlan(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_vxlan(VXLAN_VNI);
	set_action_vxlan_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_INSERT);
	assert_int_equal(action->vlan.tci, VLAN_TCI);
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT);
	assert_int_equal(action->nat.modified_ip, NEW_SRC_IP);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_VXLAN);
}

static void
test_egress_vxlan_snat_modify_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_set_vlan_vid(MOD_VLAN_VID);
	set_action_set_vlan_pcp(MOD_VLAN_PCP);
	set_action_ipv4_src(NEW_SRC_IP);

	set_encap_eth_vxlan(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4_vxlan(OUTER_SRC_IP, OUTER_DST_IP, DEFAULT_TTL, OUTER_TOS);
	set_encap_udp_vxlan(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_vxlan(VXLAN_VNI);
	set_action_vxlan_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_MOD);
	assert_int_equal(action->vlan.tci, MOD_VLAN_TCI);
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT);
	assert_int_equal(action->nat.modified_ip, NEW_SRC_IP);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_VXLAN);
}

static void
test_egress_vxlan_snat_strip_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_pop_vlan();
	set_action_ipv4_src(NEW_SRC_IP);

	set_encap_eth_vxlan(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4_vxlan(OUTER_SRC_IP, OUTER_DST_IP, DEFAULT_TTL, OUTER_TOS);
	set_encap_udp_vxlan(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_vxlan(VXLAN_VNI);
	set_action_vxlan_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_STRIP);
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT);
	assert_int_equal(action->nat.modified_ip, NEW_SRC_IP);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_VXLAN);
}

static void
test_egress_vxlan_tcp_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_encap_eth_vxlan(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4_vxlan(OUTER_SRC_IP, OUTER_DST_IP, DEFAULT_TTL, OUTER_TOS);
	set_encap_udp_vxlan(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_vxlan(VXLAN_VNI);
	set_action_vxlan_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_VXLAN);
}

static void
test_egress_vxlan_tcp_l2_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_encap_eth_vxlan(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4_vxlan(OUTER_SRC_IP, OUTER_DST_IP, DEFAULT_TTL, OUTER_TOS);
	set_encap_udp_vxlan(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_vxlan(VXLAN_VNI);
	set_action_vxlan_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_VXLAN);
}

static void
test_ingress_vxlan_udp_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_vxlan(VXLAN_VNI);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_decap_vxlan();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_VXLAN);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_decap_vxlan_action_matches(&flow.action);
}

static void
test_ingress_vxlan_udp_insert_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_vxlan(VXLAN_VNI);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_decap_vxlan();
	set_action_push_vlan();
	set_action_set_vlan_vid(VLAN_VID);
	set_action_set_vlan_pcp(VLAN_PCP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_VXLAN);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_INSERT);
	assert_int_equal(action->vlan.tci, VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_decap_vxlan_action_matches(&flow.action);
}

static void
test_ingress_vxlan_udp_modify_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_vxlan(VXLAN_VNI);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_decap_vxlan();
	set_action_set_vlan_vid(MOD_VLAN_VID);
	set_action_set_vlan_pcp(MOD_VLAN_PCP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_VXLAN);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_MOD);
	assert_int_equal(action->vlan.tci, MOD_VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_decap_vxlan_action_matches(&flow.action);
}

static void
test_ingress_vxlan_udp_strip_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_vxlan(VXLAN_VNI);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_decap_vxlan();
	set_action_pop_vlan();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_VXLAN);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_STRIP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_decap_vxlan_action_matches(&flow.action);
}

static void
test_ingress_vxlan_udp_l2_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_vxlan(VXLAN_VNI);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_decap_vxlan();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_VXLAN);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_decap_vxlan_action_matches(&flow.action);
}

static void
test_ingress_vxlan_udp_vlan_l2_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_vxlan(VXLAN_VNI);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_decap_vxlan();
	set_action_pop_vlan();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_VXLAN);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_decap_vxlan_action_matches(&flow.action);
}


static void
test_ingress_vxlan_dnat_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_vxlan(VXLAN_VNI);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_decap_vxlan();
	set_action_ipv4_dst(NEW_DST_IP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_VXLAN);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_DNAT);
	assert_int_equal(action->nat.modified_ip, NEW_DST_IP);

	assert_decap_vxlan_action_matches(&flow.action);
}

static void
test_ingress_vxlan_dnat_insert_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_vxlan(VXLAN_VNI);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_decap_vxlan();
	set_action_push_vlan();
	set_action_set_vlan_vid(VLAN_VID);
	set_action_set_vlan_pcp(VLAN_PCP);
	set_action_ipv4_dst(NEW_DST_IP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_VXLAN);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_INSERT);
	assert_int_equal(action->vlan.tci, VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_DNAT);
	assert_int_equal(action->nat.modified_ip, NEW_DST_IP);

	assert_decap_vxlan_action_matches(&flow.action);
}

static void
test_ingress_vxlan_dnat_modify_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_vxlan(VXLAN_VNI);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_decap_vxlan();
	set_action_set_vlan_vid(MOD_VLAN_VID);
	set_action_set_vlan_pcp(MOD_VLAN_PCP);
	set_action_ipv4_dst(NEW_DST_IP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_VXLAN);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_MOD);
	assert_int_equal(action->vlan.tci, MOD_VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_DNAT);
	assert_int_equal(action->nat.modified_ip, NEW_DST_IP);

	assert_decap_vxlan_action_matches(&flow.action);
}

static void
test_ingress_vxlan_dnat_strip_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_vxlan(VXLAN_VNI);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_decap_vxlan();
	set_action_pop_vlan();
	set_action_ipv4_dst(NEW_DST_IP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_VXLAN);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_STRIP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_DNAT);
	assert_int_equal(action->nat.modified_ip, NEW_DST_IP);

	assert_decap_vxlan_action_matches(&flow.action);
}

static void
test_ingress_vxlan_tcp_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_vxlan(VXLAN_VNI);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_decap_vxlan();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_VXLAN);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_decap_vxlan_action_matches(&flow.action);
}

static void
test_ingress_vxlan_tcp_l2_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_vxlan(VXLAN_VNI);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_decap_vxlan();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_VXLAN);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_decap_vxlan_action_matches(&flow.action);
}

static void
test_ingress_vxlan_udp_l2_drop(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_vxlan(VXLAN_VNI);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_drop();

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_VXLAN);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DROP);
}

static void
test_ingress_vxlan_udp_l2_drop_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_vxlan(VXLAN_VNI);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_drop();

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_VXLAN);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DROP);
}

static void
test_ingress_vxlan_tcp_l2_drop(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_vxlan(VXLAN_VNI);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_drop();

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_VXLAN);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DROP);
}

static void
test_ingress_vxlan_tcp_l2_drop_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_vxlan(VXLAN_VNI);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_drop();

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_VXLAN);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DROP);
}

static void
test_ingress_mplsoudp_udp_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_decap_action_matches(&flow.action, false);
}

static void
test_ingress_mplsoudp_udp_insert_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_action_push_vlan();
	set_action_set_vlan_vid(VLAN_VID);
	set_action_set_vlan_pcp(VLAN_PCP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_INSERT);
	assert_int_equal(action->vlan.tci, VLAN_TCI);
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_decap_action_matches(&flow.action, false);
}

static void
test_ingress_mplsoudp_udp_modify_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_action_set_vlan_vid(MOD_VLAN_VID);
	set_action_set_vlan_pcp(MOD_VLAN_PCP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_MOD);
	assert_int_equal(action->vlan.tci, MOD_VLAN_TCI);
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_decap_action_matches(&flow.action, false);
}

static void
test_ingress_mplsoudp_udp_strip_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_action_pop_vlan();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_STRIP);
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_decap_action_matches(&flow.action, false);
}

static void
test_ingress_mplsoudp_udp_l2_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_decap_action_matches(&flow.action, false);
}

static void
test_ingress_mplsoudp_udp_vlan_l2_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_decap_action_matches(&flow.action, false);
}

static void
test_ingress_mplsoudp_dnat_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_action_ipv4_dst(NEW_DST_IP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_DNAT);
	assert_int_equal(action->nat.modified_ip, NEW_DST_IP);

	assert_decap_action_matches(&flow.action, false);
}

static void
test_ingress_mplsoudp_dnat_insert_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_action_push_vlan();
	set_action_set_vlan_vid(VLAN_VID);
	set_action_set_vlan_pcp(VLAN_PCP);
	set_action_ipv4_dst(NEW_DST_IP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_INSERT);
	assert_int_equal(action->vlan.tci, VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_DNAT);
	assert_int_equal(action->nat.modified_ip, NEW_DST_IP);

	assert_decap_action_matches(&flow.action, false);
}

static void
test_ingress_mplsoudp_dnat_modify_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_action_set_vlan_vid(MOD_VLAN_VID);
	set_action_set_vlan_pcp(MOD_VLAN_PCP);
	set_action_ipv4_dst(NEW_DST_IP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_MOD);
	assert_int_equal(action->vlan.tci, MOD_VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_DNAT);
	assert_int_equal(action->nat.modified_ip, NEW_DST_IP);

	assert_decap_action_matches(&flow.action, false);
}

static void
test_ingress_mplsoudp_dnat_strip_vlan_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_action_pop_vlan();
	set_action_ipv4_dst(NEW_DST_IP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_STRIP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_DNAT);
	assert_int_equal(action->nat.modified_ip, NEW_DST_IP);

	assert_decap_action_matches(&flow.action, false);
}

static void
test_ingress_mplsoudp_tcp_l2_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_decap_action_matches(&flow.action, false);
}

static void
test_ingress_mplsoudp_tcp_l2_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_decap_action_matches(&flow.action, false);
}

static void
test_ingress_mplsoudp_udp_l2_drop(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_drop();

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DROP);
}

static void
test_ingress_mplsoudp_udp_l2_drop_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_drop();

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DROP);
}

static void
test_ingress_mplsoudp_tcp_l2_drop(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_drop();

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DROP);
}

static void
test_ingress_mplsoudp_tcp_l2_drop_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_drop();

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DROP);
}

static void
test_egress_mplsoudp_udp_l3_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_eth();
	set_action_dec_ttl();
	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);

	assert_encap_action_matches(&flow.action, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_egress_mplsoudp_udp_strip_vlan_l3_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_pop_vlan();

	set_action_raw_decap_eth();
	set_action_dec_ttl();
	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_STRIP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);

	assert_encap_action_matches(&flow.action, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_egress_mplsoudp_udp_l3_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_eth();
	set_action_dec_ttl();
	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);

	assert_encap_action_matches(&flow.action, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_egress_mplsoudp_snat_l3_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_ipv4_src(NEW_SRC_IP);

	set_action_raw_decap_eth();
	set_action_dec_ttl();
	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT);
	assert_int_equal(action->nat.modified_ip, NEW_SRC_IP);

	assert_encap_action_matches(&flow.action, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_egress_mplsoudp_snat_strip_vlan_l3_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_pop_vlan();

	set_action_ipv4_src(NEW_SRC_IP);

	set_action_raw_decap_eth();
	set_action_dec_ttl();
	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_STRIP);
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT);
	assert_int_equal(action->nat.modified_ip, NEW_SRC_IP);

	assert_encap_action_matches(&flow.action, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_egress_mplsoudp_tcp_l3_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_raw_decap_eth();
	set_action_dec_ttl();
	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);

	assert_encap_action_matches(&flow.action, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_egress_mplsoudp_tcp_l3_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_raw_decap_eth();
	set_action_dec_ttl();
	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);

	assert_encap_action_matches(&flow.action, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_ingress_mplsoudp_udp_l3_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_encap_eth(ROUTER_MAC, DST_MAC);
	set_action_raw_encap();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, false, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);

	assert_decap_action_matches(&flow.action, true);
}

static void
test_ingress_mplsoudp_udp_insert_vlan_l3_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_encap_eth(ROUTER_MAC, DST_MAC);
	set_action_raw_encap();

	set_action_push_vlan();
	set_action_set_vlan_vid(VLAN_VID);
	set_action_set_vlan_pcp(VLAN_PCP);

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, false, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_INSERT);
	assert_int_equal(action->vlan.tci, VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);

	assert_decap_action_matches(&flow.action, true);
}

static void
test_ingress_mplsoudp_udp_modify_vlan_l3_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_encap_eth(ROUTER_MAC, DST_MAC);
	set_action_raw_encap();

	set_action_set_vlan_vid(VLAN_VID);
	set_action_set_vlan_pcp(VLAN_PCP);

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, -EINVAL);
}

static void
test_ingress_mplsoudp_udp_strip_vlan_l3_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_encap_eth(ROUTER_MAC, DST_MAC);
	set_action_raw_encap();

	set_action_push_vlan();
	set_action_set_vlan_vid(VLAN_VID);
	set_action_set_vlan_pcp(VLAN_PCP);

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, -EINVAL);
}

static void
test_ingress_mplsoudp_udp_l3_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_encap_eth(ROUTER_MAC, DST_MAC);
	set_action_raw_encap();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, false, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);
	assert_outer_key_matches(&flow.key, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);

	assert_decap_action_matches(&flow.action, true);
}

static void
test_ingress_mplsoudp_dnat_l3_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_encap_eth(ROUTER_MAC, DST_MAC);
	set_action_raw_encap();
	set_action_ipv4_dst(NEW_DST_IP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, false, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);

	assert_int_equal(action->nat_type, N3K_MGMT_FLOW_TBL_NAT_TYPE_DNAT);
	assert_int_equal(action->nat.modified_ip, NEW_DST_IP);

	assert_decap_action_matches(&flow.action, true);
}


static void
test_ingress_mplsoudp_tcp_l3_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_encap_eth(ROUTER_MAC, DST_MAC);
	set_action_raw_encap();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, false, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);

	assert_decap_action_matches(&flow.action, true);
}

static void
test_ingress_mplsoudp_tcp_l3_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_encap_eth(ROUTER_MAC, DST_MAC);
	set_action_raw_encap();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, false, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);
	assert_outer_key_matches(&flow.key, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);

	assert_decap_action_matches(&flow.action, true);
}

static void
test_ingress_mplsoudp_udp_l3_drop(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_drop();

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, false, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DROP);
}

static void
test_ingress_mplsoudp_udp_l3_drop_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_drop();

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, false, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);
	assert_outer_key_matches(&flow.key, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DROP);
}

static void
test_ingress_mplsoudp_tcp_l3_drop(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_drop();

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, false, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DROP);
}

static void
test_ingress_mplsoudp_tcp_l3_drop_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_tcp(SRC_PORT, DST_PORT);

	set_action_drop();

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, false, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);
	assert_outer_key_matches(&flow.key, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DROP);
}

static void
test_egress_mplsoudp_l2_mirror_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);
	set_action_mirror(DST_PORT_ID_MIRROR, 0);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->mirror.mode, 0);
	assert_int_equal(action->mirror.port.port_id, DST_PORT_ID_MIRROR);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_egress_mplsoudp_insert_vlan_l2_mirror_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_push_vlan();
	set_action_set_vlan_vid(VLAN_VID);
	set_action_set_vlan_pcp(VLAN_PCP);

	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);
	set_action_mirror(DST_PORT_ID_MIRROR, 0);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_INSERT);
	assert_int_equal(action->vlan.tci, VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->mirror.mode, 0);
	assert_int_equal(action->mirror.port.port_id, DST_PORT_ID_MIRROR);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_egress_mplsoudp_modify_vlan_l2_mirror_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_set_vlan_vid(MOD_VLAN_VID);
	set_action_set_vlan_pcp(MOD_VLAN_PCP);

	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);
	set_action_mirror(DST_PORT_ID_MIRROR, 0);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_MOD);
	assert_int_equal(action->vlan.tci, MOD_VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->mirror.mode, 0);
	assert_int_equal(action->mirror.port.port_id, DST_PORT_ID_MIRROR);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_egress_mplsoudp_strip_vlan_l2_mirror_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_pop_vlan();

	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);
	set_action_mirror(DST_PORT_ID_MIRROR, 0);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_STRIP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->mirror.mode, 0);
	assert_int_equal(action->mirror.port.port_id, DST_PORT_ID_MIRROR);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_egress_mplsoudp_l2_mirror_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);
	set_action_mirror(DST_PORT_ID_MIRROR, 0);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->mirror.mode, 0);
	assert_int_equal(action->mirror.port.port_id, DST_PORT_ID_MIRROR);

	assert_encap_action_matches(&flow.action, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_ingress_mplsoudp_l2_mirror_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_action_port_id(DST_PORT_ID);
	set_action_mirror(DST_PORT_ID_MIRROR, 1);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->mirror.mode, 1);
	assert_int_equal(action->mirror.port.port_id, DST_PORT_ID_MIRROR);

	assert_decap_action_matches(&flow.action, false);
}

static void
test_ingress_mplsoudp_insert_vlan_l2_mirror_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_action_push_vlan();
	set_action_set_vlan_vid(VLAN_VID);
	set_action_set_vlan_pcp(VLAN_PCP);
	set_action_port_id(DST_PORT_ID);
	set_action_mirror(DST_PORT_ID_MIRROR, 1);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_INSERT);
	assert_int_equal(action->vlan.tci, VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->mirror.mode, 1);
	assert_int_equal(action->mirror.port.port_id, DST_PORT_ID_MIRROR);

	assert_decap_action_matches(&flow.action, false);
}

static void
test_ingress_mplsoudp_modify_vlan_l2_mirror_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_action_set_vlan_vid(MOD_VLAN_VID);
	set_action_set_vlan_pcp(MOD_VLAN_PCP);
	set_action_port_id(DST_PORT_ID);
	set_action_mirror(DST_PORT_ID_MIRROR, 1);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_MOD);
	assert_int_equal(action->vlan.tci, MOD_VLAN_TCI);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->mirror.mode, 1);
	assert_int_equal(action->mirror.port.port_id, DST_PORT_ID_MIRROR);

	assert_decap_action_matches(&flow.action, false);
}

static void
test_ingress_mplsoudp_strip_vlan_l2_mirror_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_vlan(VLAN_TCI);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_action_pop_vlan();
	set_action_port_id(DST_PORT_ID);
	set_action_mirror(DST_PORT_ID_MIRROR, 1);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->l2.vlan_tci, VLAN_TCI);
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->vlan.type, N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_STRIP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->mirror.mode, 1);
	assert_int_equal(action->mirror.port.port_id, DST_PORT_ID_MIRROR);

	assert_decap_action_matches(&flow.action, false);
}

static void
test_ingress_mplsoudp_l2_mirror_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_action_port_id(DST_PORT_ID);
	set_action_mirror(DST_PORT_ID_MIRROR, 1);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);
	assert_outer_key_matches(&flow.key, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_int_equal(action->mirror.mode, 1);
	assert_int_equal(action->mirror.port.port_id, DST_PORT_ID_MIRROR);

	assert_decap_action_matches(&flow.action, false);
}

static void
test_egress_mplsoudp_l3_mirror_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_eth();
	set_action_dec_ttl();
	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);
	set_action_mirror(DST_PORT_ID_MIRROR, 0);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);

	assert_int_equal(action->mirror.mode, 0);
	assert_int_equal(action->mirror.port.port_id, DST_PORT_ID_MIRROR);

	assert_encap_action_matches(&flow.action, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_egress_mplsoudp_l3_mirror_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_eth();
	set_action_dec_ttl();
	set_encap_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_encap_ipv4(OUTER_SRC_IP, OUTER_DST_IP, OUTER_TTL, OUTER_TOS);
	set_encap_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_encap_mpls(MPLS_LABEL, MPLS_TC);
	set_action_raw_encap();

	set_action_port_id(DST_PORT_ID);
	set_action_mirror(DST_PORT_ID_MIRROR, 0);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_ENCAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);

	assert_int_equal(action->mirror.mode, 0);
	assert_int_equal(action->mirror.port.port_id, DST_PORT_ID_MIRROR);

	assert_encap_action_matches(&flow.action, true, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);
}

static void
test_ingress_mplsoudp_l3_mirror_forward(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_encap_eth(ROUTER_MAC, DST_MAC);
	set_action_raw_encap();
	set_action_port_id(DST_PORT_ID);
	set_action_mirror(DST_PORT_ID_MIRROR, 1);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, false, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);
	assert_outer_key_matches(&flow.key, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);

	assert_int_equal(action->mirror.mode, 1);
	assert_int_equal(action->mirror.port.port_id, DST_PORT_ID_MIRROR);

	assert_decap_action_matches(&flow.action, true);
}

static void
test_ingress_mplsoudp_l3_mirror_forward_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_encap_eth(ROUTER_MAC, DST_MAC);
	set_action_raw_encap();
	set_action_port_id(DST_PORT_ID);
	set_action_mirror(DST_PORT_ID_MIRROR, 1);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_true(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, false, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);
	assert_outer_key_matches(&flow.key, false, N3K_MGMT_TUNNEL_TYPE_MPLSOUDP);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_DECAP);
	assert_int_equal(action->port.port_id, DST_PORT_ID);

	assert_int_equal(action->mirror.mode, 1);
	assert_int_equal(action->mirror.port.port_id, DST_PORT_ID_MIRROR);

	assert_decap_action_matches(&flow.action, true);
}


static void
test_missing_action(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	//set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, -EINVAL);
}

static void
test_missing_action_insert_vlan(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_push_vlan();
	// set_action_set_vlan_vid(VLAN_VID);
	set_action_set_vlan_pcp(VLAN_PCP);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, -EINVAL);
}

static void
test_missing_action_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	//set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, -EINVAL);
}

static void
test_nat_not_supported(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_set_mac_src(NEW_SRC_MAC);
	set_action_set_mac_dst(NEW_DST_MAC);
	set_action_ipv4_src(NEW_SRC_IP);
	set_action_dec_ttl();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, -EINVAL);
}

static void
test_mixed_tcp_udp(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, -EINVAL);
}

static void
test_mixed_tcp_udp_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_TCP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, -EINVAL);
}

static void
test_extra_void_pattern(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_void();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_pattern_void();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_extra_void_pattern_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_void();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_pattern_void();

	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_extra_void_action(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_void();
	set_action_port_id(DST_PORT_ID);
	set_action_void();

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_extra_void_action_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_void();
	set_action_port_id(DST_PORT_ID);
	set_action_void();

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, 0);

	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	assert_int_equal(key->port.port_id, SRC_PORT_ID);
	assert_device_id_equal(key->port.device_id, SRC_DEVICE);
	assert_false(key->use_tunnel_key);
	assert_inner_key_matches(key, proto, true, N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6);

	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;
	assert_int_equal(action->type, N3K_MGMT_FLOW_TBL_ACTION_LOCAL);
	assert_int_equal(action->port.port_id, DST_PORT_ID);
	assert_device_id_equal(action->port.device_id, DST_DEVICE);

	assert_false(action->local_action.modify_l2);
	assert_false(action->local_action.decr_ttl);
}

static void
test_missing_routing_action(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	//set_action_dec_ttl();
	set_action_set_mac_dst(NEW_DST_MAC);
	set_action_set_mac_src(NEW_SRC_MAC);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, -EINVAL);
}

static void
test_missing_routing_action_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	//set_action_dec_ttl();
	set_action_set_mac_dst(NEW_DST_MAC);
	set_action_set_mac_src(NEW_SRC_MAC);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, -EINVAL);
}

static void
test_extra_routing_action(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_dec_ttl();
	set_action_dec_ttl();
	set_action_set_mac_dst(NEW_DST_MAC);
	set_action_set_mac_src(NEW_SRC_MAC);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, -EINVAL);
}

static void
test_extra_routing_action_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);
	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_dec_ttl();
	set_action_dec_ttl();
	set_action_set_mac_dst(NEW_DST_MAC);
	set_action_set_mac_src(NEW_SRC_MAC);
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, -EINVAL);
}

static void
test_mixed_l2_l3_ingress(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_encap_eth(ROUTER_MAC, DST_MAC);
	set_action_raw_encap();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, -EINVAL);
}

static void
test_mixed_l2_l3_ingress_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_eth(SRC_MAC, DST_MAC);
	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_encap_eth(ROUTER_MAC, DST_MAC);
	set_action_raw_encap();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, -EINVAL);
}

static void
test_mixed_l3_l2_ingress(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_ipv4(SRC_IP, DST_IP, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, -EINVAL);
}

static void
test_mixed_l3_l2_ingress_ipv6(void **state __rte_unused)
{
	const uint8_t proto = IPPROTO_UDP;

	reset_patterns_and_actions();

	set_pattern_port_id(SRC_PORT_ID);

	set_pattern_outer_eth(OUTER_SRC_MAC, OUTER_DST_MAC);
	set_pattern_outer_ipv4(OUTER_SRC_IP, OUTER_DST_IP);
	set_pattern_outer_udp(OUTER_SRC_PORT, OUTER_DST_PORT);
	set_pattern_mpls(MPLS_LABEL, MPLS_TC);

	set_pattern_inner_ipv6(SRC_IP_IPv6, DST_IP_IPv6, proto);
	set_pattern_inner_udp(SRC_PORT, DST_PORT);

	set_action_raw_decap_mpls();
	set_action_port_id(DST_PORT_ID);

	struct n3k_mgmt_flow_entry flow;
	int rc = n3k_mgmt_rte_flow_to_tables(pattern, actions, &flow);
	assert_int_equal(rc, -EINVAL);
}

const struct CMUnitTest flow_create_tests[] = {
	cmocka_unit_test(test_local_udp_l2_forward),
	cmocka_unit_test(test_local_udp_l2_insert_vlan_forward),
	cmocka_unit_test(test_local_udp_l2_modify_vlan_forward),
	cmocka_unit_test(test_local_udp_l2_strip_vlan_forward),
	cmocka_unit_test(test_local_udp_l2_forward_ipv6),
	cmocka_unit_test(test_local_udp_vlan_l2_forward_ipv6),
	cmocka_unit_test(test_local_udp_l3_forward),
	cmocka_unit_test(test_local_udp_insert_vlan_l3_forward),
	cmocka_unit_test(test_local_udp_modify_vlan_l3_forward),
	cmocka_unit_test(test_local_udp_strip_vlan_l3_forward),
	cmocka_unit_test(test_local_udp_l3_forward_ipv6),
	cmocka_unit_test(test_local_tcp_l2_forward),
	cmocka_unit_test(test_local_tcp_vlan_l2_forward),
	cmocka_unit_test(test_local_tcp_l2_forward_ipv6),
	cmocka_unit_test(test_local_tcp_vlan_l2_forward_ipv6),
	cmocka_unit_test(test_local_tcp_l3_forward),
	cmocka_unit_test(test_local_tcp_l3_forward_ipv6),
	cmocka_unit_test(test_local_udp_drop),
	cmocka_unit_test(test_local_udp_vlan_drop),
	cmocka_unit_test(test_local_udp_drop_ipv6),
	cmocka_unit_test(test_local_udp_vlan_drop_ipv6),
	cmocka_unit_test(test_local_tcp_drop),
	cmocka_unit_test(test_local_tcp_vlan_drop),
	cmocka_unit_test(test_local_tcp_drop_ipv6),
	cmocka_unit_test(test_local_tcp_vlan_drop_ipv6),

	cmocka_unit_test(test_local_l2_mirror_forward),
	cmocka_unit_test(test_local_insert_vlan_l2_mirror_forward),
	cmocka_unit_test(test_local_modify_vlan_l2_mirror_forward),
	cmocka_unit_test(test_local_strip_vlan_l2_mirror_forward),
	cmocka_unit_test(test_local_l2_mirror_forward_ipv6),
	cmocka_unit_test(test_local_l3_mirror_forward),
	cmocka_unit_test(test_local_l3_mirror_forward_ipv6),

	cmocka_unit_test(test_egress_mplsoudp_udp_l2_forward),
	cmocka_unit_test(test_egress_mplsoudp_udp_insert_vlan_l2_forward),
	cmocka_unit_test(test_egress_mplsoudp_udp_modify_vlan_l2_forward),
	cmocka_unit_test(test_egress_mplsoudp_udp_strip_vlan_l2_forward),
	cmocka_unit_test(test_egress_mplsoudp_udp_l2_forward_ipv6),
	cmocka_unit_test(test_egress_mplsoudp_udp_vlan_l2_forward_ipv6),
	cmocka_unit_test(test_egress_mplsoudp_tcp_l2_forward),
	cmocka_unit_test(test_egress_mplsoudp_tcp_l2_forward_ipv6),

	cmocka_unit_test(test_ingress_mplsoudp_udp_l2_forward),
	cmocka_unit_test(test_ingress_mplsoudp_udp_insert_vlan_l2_forward),
	cmocka_unit_test(test_ingress_mplsoudp_udp_modify_vlan_l2_forward),
	cmocka_unit_test(test_ingress_mplsoudp_udp_strip_vlan_l2_forward),
	cmocka_unit_test(test_ingress_mplsoudp_udp_l2_forward_ipv6),
	cmocka_unit_test(test_ingress_mplsoudp_udp_vlan_l2_forward_ipv6),
	cmocka_unit_test(test_ingress_mplsoudp_tcp_l2_forward),
	cmocka_unit_test(test_ingress_mplsoudp_tcp_l2_forward_ipv6),
	cmocka_unit_test(test_ingress_mplsoudp_udp_l2_drop),
	cmocka_unit_test(test_ingress_mplsoudp_udp_l2_drop_ipv6),
	cmocka_unit_test(test_ingress_mplsoudp_tcp_l2_drop),
	cmocka_unit_test(test_ingress_mplsoudp_tcp_l2_drop_ipv6),

	cmocka_unit_test(test_egress_mplsoudp_udp_l3_forward),
	cmocka_unit_test(test_egress_mplsoudp_udp_strip_vlan_l3_forward),
	cmocka_unit_test(test_egress_mplsoudp_udp_l3_forward_ipv6),
	cmocka_unit_test(test_egress_mplsoudp_tcp_l3_forward),
	cmocka_unit_test(test_egress_mplsoudp_tcp_l3_forward_ipv6),

	cmocka_unit_test(test_ingress_mplsoudp_udp_l3_forward),
	cmocka_unit_test(test_ingress_mplsoudp_udp_insert_vlan_l3_forward),
	cmocka_unit_test(test_ingress_mplsoudp_udp_modify_vlan_l3_forward),
	cmocka_unit_test(test_ingress_mplsoudp_udp_strip_vlan_l3_forward),
	cmocka_unit_test(test_ingress_mplsoudp_udp_l3_forward_ipv6),
	cmocka_unit_test(test_ingress_mplsoudp_tcp_l3_forward),
	cmocka_unit_test(test_ingress_mplsoudp_tcp_l3_forward_ipv6),
	cmocka_unit_test(test_ingress_mplsoudp_udp_l3_drop),
	cmocka_unit_test(test_ingress_mplsoudp_udp_l3_drop_ipv6),
	cmocka_unit_test(test_ingress_mplsoudp_tcp_l3_drop),
	cmocka_unit_test(test_ingress_mplsoudp_tcp_l3_drop_ipv6),

	cmocka_unit_test(test_egress_mplsoudp_l2_mirror_forward),
	cmocka_unit_test(test_egress_mplsoudp_insert_vlan_l2_mirror_forward),
	cmocka_unit_test(test_egress_mplsoudp_modify_vlan_l2_mirror_forward),
	cmocka_unit_test(test_egress_mplsoudp_strip_vlan_l2_mirror_forward),
	cmocka_unit_test(test_egress_mplsoudp_l2_mirror_forward_ipv6),
	cmocka_unit_test(test_ingress_mplsoudp_l2_mirror_forward),
	cmocka_unit_test(test_ingress_mplsoudp_insert_vlan_l2_mirror_forward),
	cmocka_unit_test(test_ingress_mplsoudp_modify_vlan_l2_mirror_forward),
	cmocka_unit_test(test_ingress_mplsoudp_strip_vlan_l2_mirror_forward),
	cmocka_unit_test(test_ingress_mplsoudp_l2_mirror_forward_ipv6),
	cmocka_unit_test(test_egress_mplsoudp_l3_mirror_forward),
	cmocka_unit_test(test_egress_mplsoudp_l3_mirror_forward_ipv6),
	cmocka_unit_test(test_ingress_mplsoudp_l3_mirror_forward),
	cmocka_unit_test(test_ingress_mplsoudp_l3_mirror_forward_ipv6),
	cmocka_unit_test(test_local_snat_l2_forward),
	cmocka_unit_test(test_local_snat_insert_vlan_l2_forward),
	cmocka_unit_test(test_local_snat_modify_vlan_l2_forward),
	cmocka_unit_test(test_local_snat_strip_vlan_l2_forward),
	cmocka_unit_test(test_local_dnat_l2_forward),
	cmocka_unit_test(test_local_snat_l3_forward),
	cmocka_unit_test(test_local_dnat_l3_forward),
	cmocka_unit_test(test_egress_mplsoudp_snat_l2_forward),
	cmocka_unit_test(test_egress_mplsoudp_snat_insert_vlan_l2_forward),
	cmocka_unit_test(test_egress_mplsoudp_snat_modify_vlan_l2_forward),
	cmocka_unit_test(test_egress_mplsoudp_snat_strip_vlan_l2_forward),
	cmocka_unit_test(test_ingress_mplsoudp_dnat_l2_forward),
	cmocka_unit_test(test_ingress_mplsoudp_dnat_insert_vlan_l2_forward),
	cmocka_unit_test(test_ingress_mplsoudp_dnat_modify_vlan_l2_forward),
	cmocka_unit_test(test_ingress_mplsoudp_dnat_strip_vlan_l2_forward),
	cmocka_unit_test(test_egress_mplsoudp_snat_l3_forward),
	cmocka_unit_test(test_egress_mplsoudp_snat_strip_vlan_l3_forward),
	cmocka_unit_test(test_ingress_mplsoudp_dnat_l3_forward),

	cmocka_unit_test(test_missing_pattern),
	cmocka_unit_test(test_missing_pattern_ipv6),
	cmocka_unit_test(test_missing_action),
	cmocka_unit_test(test_missing_action_ipv6),
	cmocka_unit_test(test_missing_action_insert_vlan),
	cmocka_unit_test(test_nat_not_supported),
	cmocka_unit_test(test_mixed_tcp_udp),
	cmocka_unit_test(test_mixed_tcp_udp_ipv6),
	cmocka_unit_test(test_extra_void_pattern),
	cmocka_unit_test(test_extra_void_pattern_ipv6),
	cmocka_unit_test(test_extra_void_action),
	cmocka_unit_test(test_extra_void_action_ipv6),
	cmocka_unit_test(test_missing_routing_action),
	cmocka_unit_test(test_missing_routing_action_ipv6),
	cmocka_unit_test(test_extra_routing_action),
	cmocka_unit_test(test_extra_routing_action_ipv6),
	cmocka_unit_test(test_mixed_l2_l3_ingress),
	cmocka_unit_test(test_mixed_l2_l3_ingress_ipv6),
	cmocka_unit_test(test_mixed_l3_l2_ingress),
	cmocka_unit_test(test_mixed_l3_l2_ingress_ipv6),

	cmocka_unit_test(test_egress_vxlan_udp_l2_forward),
	cmocka_unit_test(test_egress_vxlan_udp_insert_vlan_l2_forward),
	cmocka_unit_test(test_egress_vxlan_udp_modify_vlan_l2_forward),
	cmocka_unit_test(test_egress_vxlan_udp_strip_vlan_l2_forward),
	cmocka_unit_test(test_egress_vxlan_udp_l2_forward_ipv6),
	cmocka_unit_test(test_egress_vxlan_snat_l2_forward),
	cmocka_unit_test(test_egress_vxlan_snat_insert_vlan_l2_forward),
	cmocka_unit_test(test_egress_vxlan_snat_modify_vlan_l2_forward),
	cmocka_unit_test(test_egress_vxlan_snat_strip_vlan_l2_forward),
	cmocka_unit_test(test_egress_vxlan_tcp_l2_forward),
	cmocka_unit_test(test_egress_vxlan_tcp_l2_forward_ipv6),

	cmocka_unit_test(test_ingress_vxlan_udp_l2_forward),
	cmocka_unit_test(test_ingress_vxlan_udp_insert_vlan_l2_forward),
	cmocka_unit_test(test_ingress_vxlan_udp_modify_vlan_l2_forward),
	cmocka_unit_test(test_ingress_vxlan_udp_strip_vlan_l2_forward),
	cmocka_unit_test(test_ingress_vxlan_udp_l2_forward_ipv6),
	cmocka_unit_test(test_ingress_vxlan_udp_vlan_l2_forward_ipv6),
	cmocka_unit_test(test_ingress_vxlan_dnat_l2_forward),
	cmocka_unit_test(test_ingress_vxlan_dnat_insert_vlan_l2_forward),
	cmocka_unit_test(test_ingress_vxlan_dnat_modify_vlan_l2_forward),
	cmocka_unit_test(test_ingress_vxlan_dnat_strip_vlan_l2_forward),
	cmocka_unit_test(test_ingress_vxlan_tcp_l2_forward),
	cmocka_unit_test(test_ingress_vxlan_tcp_l2_forward_ipv6),

	cmocka_unit_test(test_ingress_vxlan_udp_l2_drop),
	cmocka_unit_test(test_ingress_vxlan_udp_l2_drop_ipv6),
	cmocka_unit_test(test_ingress_vxlan_tcp_l2_drop),
	cmocka_unit_test(test_ingress_vxlan_tcp_l2_drop_ipv6),
};

int test_flow_create(void)
{
	return cmocka_run_group_tests_name(
		"pmd_rte_flow_conversion_tests", flow_create_tests,
		flow_conversion_setup, flow_conversion_teardown);
}
