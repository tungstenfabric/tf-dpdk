/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_rte_flow_parse.h"

#include <rte_common.h>
#include <rte_ether.h>
#include <rte_flow.h>
#include <rte_ip.h>
#include <rte_jhash.h>
#include <rte_mpls.h>
#include <rte_udp.h>

#include "n3k_mgmt_ports.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_rte_flow_parse_print.h"

#include "n3k_mgmt_init/n3k_mgmt_init_mq.h"

struct n3k_mgmt_flow_hash_data {
	uint16_t dport;
	uint16_t sport;
	uint8_t daddr[16];
	uint8_t saddr[16];
};

static int
handle_pattern_port_id(
	const struct rte_flow_item *pattern, struct n3k_mgmt_flow_entry *flow)
{
	int ret;
	const struct rte_flow_item_port_id *port_id_spec =
		(const struct rte_flow_item_port_id *)pattern->spec;

	flow->key.key_raw.port.port_id = port_id_spec->id;

	ret = n3k_vdev_dpdk_port_to_vdev_id(
		port_id_spec->id, &flow->key.key_raw.port.device_id);
	if (ret < 0) {
		N3K_MGMT_LOG(FLOW_PARSE, ERR,
			"Failed to translate vdev dpdk port %" PRIu32
			" to vdev device id", port_id_spec->id);
		return ret;
	}

	return 0;
}

static int
handle_pattern_inner_eth(
	const struct rte_flow_item *pattern, struct n3k_mgmt_flow_entry *flow)
{
	const struct rte_flow_item_eth *eth_spec =
		(const struct rte_flow_item_eth *)pattern->spec;

	struct n3k_mgmt_flow_tbl_key *key = &flow->key.key_raw;

	key->use_l2_key = true;
	memcpy(
		key->l2.src_mac.addr_bytes,
		eth_spec->src.addr_bytes,
		RTE_ETHER_ADDR_LEN
	);
	memcpy(
		key->l2.dst_mac.addr_bytes,
		eth_spec->dst.addr_bytes,
		RTE_ETHER_ADDR_LEN
	);

	return 0;
}

static int
handle_pattern_inner_eth_ingress(
	const struct rte_flow_item *pattern, struct n3k_mgmt_flow_entry *flow)
{
	flow->key.vplkp_raw.vport = N3K_MGMT_VPLKP_TBL_VPORT_L2PACKET;
	return handle_pattern_inner_eth(pattern, flow);
}

static int
handle_pattern_ipv4(
	const struct rte_flow_item *pattern, struct n3k_mgmt_flow_tbl_l3 *l3_key)
{
	const struct rte_flow_item_ipv4 *ipv4_spec =
		(const struct rte_flow_item_ipv4 *)pattern->spec;

	l3_key->type = N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4;
	l3_key->ipv4.src_ip = rte_bswap32(ipv4_spec->hdr.src_addr);
	l3_key->ipv4.dst_ip = rte_bswap32(ipv4_spec->hdr.dst_addr);
	l3_key->ip_prot = ipv4_spec->hdr.next_proto_id;

	return 0;
}

static int
handle_pattern_inner_ipv4(
	const struct rte_flow_item *pattern, struct n3k_mgmt_flow_entry *flow)
{
	return handle_pattern_ipv4(pattern, &flow->key.key_raw.l3);
}

static int
handle_pattern_ipv6(
	const struct rte_flow_item *pattern, struct n3k_mgmt_flow_tbl_l3 *l3_key)
{
	const struct rte_flow_item_ipv6 *ipv6_spec =
		(const struct rte_flow_item_ipv6 *)pattern->spec;

	l3_key->type = N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6;

	rte_memcpy(l3_key->ipv6.src_ip, ipv6_spec->hdr.src_addr, 16);
	rte_memcpy(l3_key->ipv6.dst_ip, ipv6_spec->hdr.dst_addr, 16);

	l3_key->ip_prot = ipv6_spec->hdr.proto;
	return 0;
}

static int
handle_pattern_inner_ipv6(
	const struct rte_flow_item *pattern, struct n3k_mgmt_flow_entry *flow)
{
	return handle_pattern_ipv6(pattern, &flow->key.key_raw.l3);
}

static int
handle_pattern_inner_udp(
	const struct rte_flow_item *pattern, struct n3k_mgmt_flow_entry *flow)
{
	uint8_t proto_id = flow->key.key_raw.l3.ip_prot;
	if (proto_id != IPPROTO_UDP) {
		N3K_MGMT_LOG(FLOW_PARSE, ERR,
			"The UDP pattern does not match previously parsed proto id: %d",
			proto_id
		);
		return -EINVAL;
	}

	const struct rte_flow_item_udp *udp_spec =
		(const struct rte_flow_item_udp *)pattern->spec;

	struct n3k_mgmt_flow_tbl_l4 *l4_key = &flow->key.key_raw.l4;
	l4_key->src_port = rte_bswap16(udp_spec->hdr.src_port);
	l4_key->dst_port = rte_bswap16(udp_spec->hdr.dst_port);

	return 0;
}

static int
handle_pattern_inner_tcp(
	const struct rte_flow_item *pattern, struct n3k_mgmt_flow_entry *flow)
{
	uint8_t proto_id = flow->key.key_raw.l3.ip_prot;
	if (proto_id != IPPROTO_TCP) {
		N3K_MGMT_LOG(FLOW_PARSE,
			ERR,
			"The TCP pattern does not match previously parsed proto id: %d",
			proto_id
		);
		return -EINVAL;
	}

	const struct rte_flow_item_tcp *tcp_spec =
		(const struct rte_flow_item_tcp *)pattern->spec;

	struct n3k_mgmt_flow_tbl_l4 *l4_key = &flow->key.key_raw.l4;
	l4_key->src_port = rte_bswap16(tcp_spec->hdr.src_port);
	l4_key->dst_port = rte_bswap16(tcp_spec->hdr.dst_port);

	return 0;
}

static int
handle_pattern_outer_eth(
	const struct rte_flow_item *pattern, struct n3k_mgmt_flow_entry *flow)
{
	RTE_SET_USED(pattern);

	struct n3k_mgmt_flow_tbl_key *key = &flow->key.key_raw;
	key->use_tunnel_key = true;

	return 0;
}

static int
handle_pattern_outer_ipv4(
	const struct rte_flow_item *pattern, struct n3k_mgmt_flow_entry *flow)
{
	return handle_pattern_ipv4(pattern, &flow->key.key_raw.tunnel.outer_l3);
}

static int
handle_pattern_outer_udp(
	const struct rte_flow_item *pattern, struct n3k_mgmt_flow_entry *flow)
{
	RTE_SET_USED(pattern);

	struct n3k_mgmt_flow_tbl_key_tunnel *tunnel = &flow->key.key_raw.tunnel;
	uint8_t proto_id = tunnel->outer_l3.ip_prot;
	if (proto_id != IPPROTO_UDP) {
		N3K_MGMT_LOG(FLOW_PARSE, ERR,
			"The UDP pattern does not match previously parsed proto id: %d",
			proto_id
		);
		return -EINVAL;
	}

	return 0;
}

static int
handle_pattern_vxlan(
	const struct rte_flow_item *pattern, struct n3k_mgmt_flow_entry *flow)
{
	const struct rte_flow_item_vxlan *vxlan_spec =
		(const struct rte_flow_item_vxlan *)pattern->spec;


	struct n3k_mgmt_flow_tbl_key_tunnel *tunnel = &flow->key.key_raw.tunnel;

	tunnel->vxlan.vni = vxlan_spec->vni[2] +
		(vxlan_spec->vni[1] << 8) +
		(vxlan_spec->vni[0] << 16);
	tunnel->type = N3K_MGMT_TUNNEL_TYPE_VXLAN;

	return 0;
}

static int
handle_pattern_mpls(
	const struct rte_flow_item *pattern, struct n3k_mgmt_flow_entry *flow)
{
	const struct rte_flow_item_mpls *mpls_spec =
		(const struct rte_flow_item_mpls *)pattern->spec;

	struct n3k_mgmt_vplkp_tbl_res *vplkp = &flow->key.vplkp_raw;
	struct n3k_mgmt_flow_tbl_key_tunnel *tunnel = &flow->key.key_raw.tunnel;

	rte_be32_t mpls_be_value = 0;
	rte_memcpy(
		&mpls_be_value, mpls_spec->label_tc_s, sizeof(mpls_spec->label_tc_s));
	rte_le32_t mpls_value = rte_bswap32(mpls_be_value);

	tunnel->mpls_udp.label = mpls_value >> 12;
	tunnel->mpls_udp.tos = (mpls_value >> 9) & 0x7;
	tunnel->type = N3K_MGMT_TUNNEL_TYPE_MPLSOUDP;

	/* Assume it is L3 here. It will be overwritten in ETH parser. */
	vplkp->vport = N3K_MGMT_VPLKP_TBL_VPORT_L3PACKET;

	return 0;
}

static int
handle_action_set_ipv4_src(
	const struct rte_flow_action *action, struct n3k_mgmt_flow_entry *flow)
{
	const struct rte_flow_action_set_ipv4 *action_set_ip =
		(const struct rte_flow_action_set_ipv4 *)action->conf;

	struct n3k_mgmt_flow_tbl_action *n3k_action =
		&flow->action.action_raw;

	if (flow->key.key_raw.l3.type != N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4) {
		N3K_MGMT_LOG(FLOW_PARSE, ERR,
				"Can't use IPv4 actions with different then IPv4 key");
		return -EINVAL;
	}

	n3k_action->nat_type = N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT;
	n3k_action->nat.modified_ip = rte_bswap32(action_set_ip->ipv4_addr);
	return 0;
}

static int
handle_action_set_ipv4_dst(
	const struct rte_flow_action *action, struct n3k_mgmt_flow_entry *flow)
{
	const struct rte_flow_action_set_ipv4 *action_set_ip =
		(const struct rte_flow_action_set_ipv4 *)action->conf;

	struct n3k_mgmt_flow_tbl_action *n3k_action =
		&flow->action.action_raw;

	if (flow->key.key_raw.l3.type != N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4) {
		N3K_MGMT_LOG(FLOW_PARSE, ERR,
				"Can't use IPv4 actions with different then IPv4 key");
		return -EINVAL;
	}

	n3k_action->nat_type = N3K_MGMT_FLOW_TBL_NAT_TYPE_DNAT;
	n3k_action->nat.modified_ip = rte_bswap32(action_set_ip->ipv4_addr);
	return 0;
}

static int
handle_action_set_mac_src(
	const struct rte_flow_action *action, struct n3k_mgmt_flow_entry *flow)
{
	const struct rte_flow_action_set_mac *action_set_smac =
		(const struct rte_flow_action_set_mac *)action->conf;

	struct n3k_mgmt_flow_tbl_action_local *local_act =
		&flow->action.action_raw.local_action;

	local_act->modify_l2 = true;
	rte_memcpy(
		local_act->modified_l2.src_mac.addr_bytes,
		action_set_smac->mac_addr,
		RTE_ETHER_ADDR_LEN
	);

	return 0;
}

static int
handle_action_set_mac_dst(
	const struct rte_flow_action *action, struct n3k_mgmt_flow_entry *flow)
{
	const struct rte_flow_action_set_mac *action_set_dmac =
		(const struct rte_flow_action_set_mac *)action->conf;

	struct n3k_mgmt_flow_tbl_action_local *local_act =
		&flow->action.action_raw.local_action;

	local_act->modify_l2 = true;
	rte_memcpy(
		local_act->modified_l2.dst_mac.addr_bytes,
		action_set_dmac->mac_addr,
		RTE_ETHER_ADDR_LEN
	);

	return 0;
}

static int
handle_action_local_dec_ttl(
	const struct rte_flow_action *action, struct n3k_mgmt_flow_entry *flow)
{
	RTE_SET_USED(action);

	flow->action.action_raw.local_action.decr_ttl = true;
	return 0;
}

static int
handle_action_egress_dec_ttl(
	const struct rte_flow_action *action, struct n3k_mgmt_flow_entry *flow)
{
	RTE_SET_USED(action);

	flow->action.action_raw.encap_action.decr_ttl = true;
	return 0;
}

static int
handle_action_raw_encap_l2(
	const struct rte_flow_action *action, struct n3k_mgmt_flow_entry *flow)
{
	const struct rte_flow_action_raw_encap *action_raw_encap =
		(const struct rte_flow_action_raw_encap *)action->conf;

	if (flow->key.key_raw.use_l2_key) {
		N3K_MGMT_LOG(FLOW_PARSE, ERR,
			"Cannot add inner eth heder when already present");
		return -EINVAL;
	}

	if (action_raw_encap->preserve != NULL) {
		N3K_MGMT_LOG(FLOW_PARSE, ERR,
			"Bit masks to preserve are not supported");
		return -ENOTSUP;
	}

	if (action_raw_encap->size != sizeof(struct rte_ether_hdr)) {
		N3K_MGMT_LOG(FLOW_PARSE, ERR,
			"Invalid L2 encap data length: %" PRIuPTR ". "
			" Should be: %" PRIuPTR ".",
			action_raw_encap->size,
			sizeof(struct rte_ether_hdr)
		);
		return -EINVAL;
	}

	const struct rte_ether_hdr *eth_hdr =
		(const struct rte_ether_hdr *)action_raw_encap->data;

	struct n3k_mgmt_flow_tbl_action_decap *act =
		&flow->action.action_raw.decap_action;
	act->insert_l2 = true;
	act->inserted_l2.src_mac = eth_hdr->s_addr;
	act->inserted_l2.dst_mac = eth_hdr->d_addr;

	return 0;
}

static int
handle_action_raw_decap_l2(
	const struct rte_flow_action *action, struct n3k_mgmt_flow_entry *flow)
{
	const struct rte_flow_action_raw_decap *action_raw_decap =
		(const struct rte_flow_action_raw_decap *)action->conf;

	if (action_raw_decap->size != sizeof(struct rte_ether_hdr)) {
		N3K_MGMT_LOG(FLOW_PARSE, ERR,
			"Invalid decap length for L2 header: %" PRIuPTR ". "
			"Should be: %" PRIuPTR ".",
			action_raw_decap->size,
			sizeof(struct rte_ether_hdr)
		);
		return -EINVAL;
	}

	flow->action.action_raw.encap_action.strip_l2 = true;
	return 0;
}

static int
handle_action_vxlan_encap(
	const struct rte_flow_action *action, struct n3k_mgmt_flow_entry *flow)
{
	const struct rte_flow_action_vxlan_encap *action_vxlan_encap =
		(const struct rte_flow_action_vxlan_encap *)action->conf;

	const struct rte_flow_item_eth *eth_spec =
		(const struct rte_flow_item_eth *)
		action_vxlan_encap->definition[0].spec;

	const struct rte_flow_item_ipv4 *ipv4_spec =
		(const struct rte_flow_item_ipv4 *)
		action_vxlan_encap->definition[1].spec;

	const struct rte_flow_item_udp *udp_spec =
		(const struct rte_flow_item_udp *)
		action_vxlan_encap->definition[2].spec;

	const struct rte_flow_item_vxlan *vxlan_spec =
		(const struct rte_flow_item_vxlan *)
		action_vxlan_encap->definition[3].spec;

	struct n3k_mgmt_flow_tbl_action *act = &flow->action.action_raw;
	act->type = N3K_MGMT_FLOW_TBL_ACTION_ENCAP;
	act->encap_action.type = N3K_MGMT_TUNNEL_TYPE_VXLAN;

	struct n3k_mgmt_flow_tbl_action_tunnel_vxlan *vxlan = &act->encap_action.vxlan;
	vxlan->src_port = rte_bswap16(udp_spec->hdr.src_port);

	struct rte_ether_addr *smac = &flow->action.tunnel.smac_raw;
	*smac = eth_spec->src;

	struct n3k_mgmt_tunnel_tbl_entry *tunnel =
		&flow->action.tunnel.tunnel_raw;
	tunnel->type = N3K_MGMT_TUNNEL_TYPE_VXLAN;
	tunnel->l2.dst_mac = eth_spec->dst;
	tunnel->l2.eth_type = rte_bswap16(eth_spec->type);

	tunnel->l3.src_ip = rte_bswap32(ipv4_spec->hdr.src_addr);
	tunnel->l3.dst_ip = rte_bswap32(ipv4_spec->hdr.dst_addr);
	tunnel->l3.ip_prot = ipv4_spec->hdr.next_proto_id;
	tunnel->l3.ttl = ipv4_spec->hdr.time_to_live;
	tunnel->l3.tos = ipv4_spec->hdr.type_of_service;

	tunnel->l4.dst_port = rte_bswap16(udp_spec->hdr.dst_port);
	tunnel->vxlan.vni = vxlan_spec->vni[2] + (vxlan_spec->vni[1] << 8) + (vxlan_spec->vni[0] << 16);

	if (tunnel->l2.eth_type != RTE_ETHER_TYPE_IPV4) {
		N3K_MGMT_LOG(FLOW_PARSE, ERR,
			"L2 protocols other than IPv4 are not supported for encapsulation");
		return -ENOTSUP;
	}

	if (tunnel->l3.ip_prot != IPPROTO_UDP) {
		N3K_MGMT_LOG(FLOW_PARSE, ERR,
			"Invalid L3 protocol for encapsulation %d. Only UDP is supported.",
			tunnel->l3.ip_prot
		);
		return -ENOTSUP;
	}

	return 0;
}

static int
handle_action_vxlan_decap(
	__rte_unused const struct rte_flow_action *action, struct n3k_mgmt_flow_entry *flow)
{
	struct n3k_mgmt_flow_tbl_action *act = &flow->action.action_raw;
	act->type = N3K_MGMT_FLOW_TBL_ACTION_DECAP;
	act->decap_action.type = N3K_MGMT_TUNNEL_TYPE_VXLAN;
	act->decap_action.insert_l2 = false;
	return 0;
}

static int
handle_action_raw_encap(
	const struct rte_flow_action *action, struct n3k_mgmt_flow_entry *flow)
{
	const struct rte_flow_action_raw_encap *action_raw_encap =
		(const struct rte_flow_action_raw_encap *)action->conf;

	if (action_raw_encap->preserve != NULL) {
		N3K_MGMT_LOG(FLOW_PARSE, ERR,
			"Bit masks to preserve are not supported");
		return -ENOTSUP;
	}

	const uint8_t *data = action_raw_encap->data;
	const size_t size = action_raw_encap->size;

	if (data == NULL) {
		N3K_MGMT_LOG(FLOW_PARSE, ERR, "Encap data ptr is not set");
		return -EINVAL;
	}

	const struct rte_ether_hdr *eth_hdr = (const struct rte_ether_hdr *)data;
	data += sizeof(*eth_hdr);

	const struct rte_ipv4_hdr *ipv4_hdr = (const struct rte_ipv4_hdr *)data;
	data += sizeof(*ipv4_hdr);

	const struct rte_udp_hdr *udp_hdr = (const struct rte_udp_hdr *)data;
	data += sizeof(*udp_hdr);

	const struct rte_mpls_hdr *mpls_hdr = (const struct rte_mpls_hdr *)data;
	data += sizeof(*mpls_hdr);

	if ((sizeof(*eth_hdr) + sizeof(*ipv4_hdr) +
			sizeof(*udp_hdr) + sizeof(*mpls_hdr)) != size) {
		N3K_MGMT_LOG(FLOW_PARSE, ERR, "Invalid encap data length");
		return -EINVAL;
	}

	struct n3k_mgmt_flow_tbl_action *act = &flow->action.action_raw;
	act->type = N3K_MGMT_FLOW_TBL_ACTION_ENCAP;
	act->encap_action.type = N3K_MGMT_TUNNEL_TYPE_MPLSOUDP;

	struct n3k_mgmt_flow_tbl_action_encap_mpls_udp *mpls =
		&act->encap_action.mpls_udp;
	mpls->src_port = rte_bswap16(udp_hdr->src_port);
	mpls->label = (rte_bswap16(mpls_hdr->tag_msb) << 4) | mpls_hdr->tag_lsb;
	mpls->tos = mpls_hdr->tc;

	struct rte_ether_addr *smac = &flow->action.tunnel.smac_raw;
	*smac = eth_hdr->s_addr;

	struct n3k_mgmt_tunnel_tbl_entry *tunnel =
		&flow->action.tunnel.tunnel_raw;
	tunnel->type = N3K_MGMT_TUNNEL_TYPE_MPLSOUDP;
	tunnel->l2.dst_mac = eth_hdr->d_addr;
	tunnel->l2.eth_type = rte_bswap16(eth_hdr->ether_type);

	tunnel->l3.src_ip = rte_bswap32(ipv4_hdr->src_addr);
	tunnel->l3.dst_ip = rte_bswap32(ipv4_hdr->dst_addr);
	tunnel->l3.ip_prot = ipv4_hdr->next_proto_id;
	tunnel->l3.tos = ipv4_hdr->type_of_service;
	tunnel->l3.ttl = ipv4_hdr->time_to_live;

	tunnel->l4.dst_port = rte_bswap16(udp_hdr->dst_port);

	if (tunnel->l2.eth_type != RTE_ETHER_TYPE_IPV4) {
		N3K_MGMT_LOG(FLOW_PARSE, ERR,
			"L2 protocols other than IPv4 are not supported for encapsulation");
		return -ENOTSUP;
	}

	if (tunnel->l3.ip_prot != IPPROTO_UDP) {
		N3K_MGMT_LOG(FLOW_PARSE, ERR,
			"Invalid L3 protocol for encapsulation %d. Only UDP is supported.",
			tunnel->l3.ip_prot
		);
		return -ENOTSUP;
	}

	return 0;
}

static int
handle_action_raw_decap(
	const struct rte_flow_action *action, struct n3k_mgmt_flow_entry *flow)
{
	const struct rte_flow_action_raw_decap *action_raw_decap =
		(const struct rte_flow_action_raw_decap *)action->conf;

	const size_t expected_size =
		sizeof(struct rte_ether_hdr) +
		RTE_IPV4_IHL_MULTIPLIER * RTE_IPV4_MIN_IHL +
		sizeof(struct rte_udp_hdr) +
		sizeof(struct rte_mpls_hdr);

	if (action_raw_decap->size != expected_size) {
		N3K_MGMT_LOG(FLOW_PARSE, ERR,
			"Invalid decap length: %" PRIuPTR ". Should be: %" PRIuPTR ".",
			action_raw_decap->size,
			expected_size
		);
		return -EINVAL;
	}

	struct n3k_mgmt_flow_tbl_action *act = &flow->action.action_raw;
	act->type = N3K_MGMT_FLOW_TBL_ACTION_DECAP;
	act->decap_action.type = N3K_MGMT_TUNNEL_TYPE_MPLSOUDP;
	act->decap_action.insert_l2 = false;

	return 0;
}

static int
handle_action_port_id(
	const struct rte_flow_action *action, struct n3k_mgmt_flow_entry *flow)
{
	const struct rte_flow_action_port_id *action_port_id =
		(const struct rte_flow_action_port_id *)action->conf;

	struct n3k_mgmt_flow_tbl_action *act = &flow->action.action_raw;
	act->port.port_id = action_port_id->id;

	return n3k_vdev_dpdk_port_to_vdev_id(act->port.port_id, &act->port.device_id);
}

static int
handle_action_port_id_local(
	const struct rte_flow_action *action, struct n3k_mgmt_flow_entry *flow)
{
	flow->action.action_raw.type = N3K_MGMT_FLOW_TBL_ACTION_LOCAL;
	return handle_action_port_id(action, flow);
}

static int
handle_action_port_id_ingress_l2_check(
	const struct rte_flow_action *action, struct n3k_mgmt_flow_entry *flow)
{
	if (!flow->key.key_raw.use_l2_key) {
		N3K_MGMT_LOG(FLOW_PARSE, ERR,
			"Eth encap missing. Cannot handle L3 packet");
		return -EINVAL;
	}

	return handle_action_port_id(action, flow);
}

static int
handle_action_mirror(
	const struct rte_flow_action *action, struct n3k_mgmt_flow_entry *flow)
{
	const struct rte_flow_action_mirror *mirror =
		(const struct rte_flow_action_mirror *)action->conf;

	flow->action.action_raw.use_mirroring = true;
	flow->action.action_raw.mirror.port.port_id = mirror->port;
	flow->action.action_raw.mirror.mode =
		(enum n3k_mgmt_flow_tbl_mirror_mode)mirror->mirror_modified;
	return n3k_vdev_dpdk_port_to_vdev_id(
		mirror->port, &flow->action.action_raw.mirror.port.device_id);
}

static int
handle_action_drop(
	const struct rte_flow_action *action, struct n3k_mgmt_flow_entry *flow)
{
	RTE_SET_USED(action);

	struct n3k_mgmt_flow_tbl_action *act = &flow->action.action_raw;
	act->type = N3K_MGMT_FLOW_TBL_ACTION_DROP;

	return 0;
}


#define PATTERN_MAX_VALUE (RTE_FLOW_ITEM_TYPE_TAG + 1)
#define ACTION_MAX_VALUE (RTE_FLOW_ACTION_TYPE_MIRROR + 1)

typedef int (*pattern_handler_t)(
	const struct rte_flow_item *pattern, struct n3k_mgmt_flow_entry *flow);

typedef int (*action_handler_t)(
	const struct rte_flow_action *action, struct n3k_mgmt_flow_entry *flow);

struct pattern_transition {
	enum n3k_mgmt_parser_pattern_state next;
	pattern_handler_t handler;
};

struct action_transition {
	enum n3k_mgmt_parser_action_state next;
	action_handler_t handler;
};

typedef struct pattern_transition
	pattern_transitions_t[PATTERN_STATE_MAX][PATTERN_MAX_VALUE];

typedef struct action_transition
	action_transitions_t[ACTION_STATE_MAX][ACTION_MAX_VALUE];

typedef const struct pattern_transition
	(*pattern_transitions_ptr_t)[PATTERN_MAX_VALUE];

typedef const struct action_transition
	(*action_transitions_ptr_t)[ACTION_MAX_VALUE];

static const pattern_transitions_t pattern_transitions_from_local_vm = {
	[PATTERN_STATE_START][RTE_FLOW_ITEM_TYPE_PORT_ID] = {
		PATTERN_STATE_PORT_ID, &handle_pattern_port_id
	},

	[PATTERN_STATE_PORT_ID][RTE_FLOW_ITEM_TYPE_ETH] = {
		PATTERN_STATE_INNER_ETH, &handle_pattern_inner_eth
	},
	[PATTERN_STATE_INNER_ETH][RTE_FLOW_ITEM_TYPE_IPV4] = {
		PATTERN_STATE_INNER_IPV4, &handle_pattern_inner_ipv4,
	},
	[PATTERN_STATE_INNER_ETH][RTE_FLOW_ITEM_TYPE_IPV6] = {
		PATTERN_STATE_INNER_IPV6, &handle_pattern_inner_ipv6,
	},
	[PATTERN_STATE_INNER_IPV4][RTE_FLOW_ITEM_TYPE_UDP] = {
		PATTERN_STATE_INNER_UDP, &handle_pattern_inner_udp
	},
	[PATTERN_STATE_INNER_IPV6][RTE_FLOW_ITEM_TYPE_UDP] = {
		PATTERN_STATE_INNER_UDP, &handle_pattern_inner_udp
	},
	[PATTERN_STATE_INNER_IPV4][RTE_FLOW_ITEM_TYPE_TCP] = {
		PATTERN_STATE_INNER_TCP, &handle_pattern_inner_tcp
	},
	[PATTERN_STATE_INNER_IPV6][RTE_FLOW_ITEM_TYPE_TCP] = {
		PATTERN_STATE_INNER_TCP, &handle_pattern_inner_tcp
	},
	[PATTERN_STATE_INNER_UDP][RTE_FLOW_ITEM_TYPE_END] = {
		PATTERN_STATE_END, NULL
	},
	[PATTERN_STATE_INNER_TCP][RTE_FLOW_ITEM_TYPE_END] = {
		PATTERN_STATE_END, NULL
	},
};

static const pattern_transitions_t pattern_transitions_from_remote_vm = {
	[PATTERN_STATE_START][RTE_FLOW_ITEM_TYPE_PORT_ID] = {
		PATTERN_STATE_PORT_ID, &handle_pattern_port_id
	},

	[PATTERN_STATE_PORT_ID][RTE_FLOW_ITEM_TYPE_ETH] = {
		PATTERN_STATE_OUTER_ETH, &handle_pattern_outer_eth
	},
	[PATTERN_STATE_OUTER_ETH][RTE_FLOW_ITEM_TYPE_IPV4] = {
		PATTERN_STATE_OUTER_IPV4, &handle_pattern_outer_ipv4
	},
	[PATTERN_STATE_OUTER_IPV4][RTE_FLOW_ITEM_TYPE_UDP] = {
		PATTERN_STATE_OUTER_UDP, &handle_pattern_outer_udp
	},
	[PATTERN_STATE_OUTER_UDP][RTE_FLOW_ITEM_TYPE_MPLS] = {
		PATTERN_STATE_MPLS, &handle_pattern_mpls
	},
	[PATTERN_STATE_OUTER_UDP][RTE_FLOW_ITEM_TYPE_VXLAN] = {
		PATTERN_STATE_VXLAN, &handle_pattern_vxlan
	},
	[PATTERN_STATE_VXLAN][RTE_FLOW_ITEM_TYPE_ETH] = {
		PATTERN_STATE_INNER_ETH, &handle_pattern_inner_eth_ingress
	},
	[PATTERN_STATE_MPLS][RTE_FLOW_ITEM_TYPE_ETH] = {
		PATTERN_STATE_INNER_ETH, &handle_pattern_inner_eth_ingress
	},
	[PATTERN_STATE_MPLS][RTE_FLOW_ITEM_TYPE_IPV4] = {
		PATTERN_STATE_INNER_IPV4, &handle_pattern_inner_ipv4
	},
	[PATTERN_STATE_MPLS][RTE_FLOW_ITEM_TYPE_IPV6] = {
		PATTERN_STATE_INNER_IPV6, &handle_pattern_inner_ipv6
	},
	[PATTERN_STATE_INNER_ETH][RTE_FLOW_ITEM_TYPE_IPV4] = {
		PATTERN_STATE_INNER_IPV4, &handle_pattern_inner_ipv4,
	},
	[PATTERN_STATE_INNER_ETH][RTE_FLOW_ITEM_TYPE_IPV6] = {
		PATTERN_STATE_INNER_IPV6, &handle_pattern_inner_ipv6,
	},
	[PATTERN_STATE_INNER_IPV4][RTE_FLOW_ITEM_TYPE_UDP] = {
		PATTERN_STATE_INNER_UDP, &handle_pattern_inner_udp
	},
	[PATTERN_STATE_INNER_IPV6][RTE_FLOW_ITEM_TYPE_UDP] = {
		PATTERN_STATE_INNER_UDP, &handle_pattern_inner_udp
	},
	[PATTERN_STATE_INNER_IPV4][RTE_FLOW_ITEM_TYPE_TCP] = {
		PATTERN_STATE_INNER_TCP, &handle_pattern_inner_tcp
	},
	[PATTERN_STATE_INNER_IPV6][RTE_FLOW_ITEM_TYPE_TCP] = {
		PATTERN_STATE_INNER_TCP, &handle_pattern_inner_tcp
	},
	[PATTERN_STATE_INNER_UDP][RTE_FLOW_ITEM_TYPE_END] = {
		PATTERN_STATE_END, NULL
	},
	[PATTERN_STATE_INNER_TCP][RTE_FLOW_ITEM_TYPE_END] = {
		PATTERN_STATE_END, NULL
	},
};

static const pattern_transitions_ptr_t pattern_transitions[] = {
	[FLOW_TYPE_LOCAL] = pattern_transitions_from_local_vm,
	[FLOW_TYPE_EGRESS] = pattern_transitions_from_local_vm,
	[FLOW_TYPE_INGRESS] = pattern_transitions_from_remote_vm,
};

static const action_transitions_t action_transitions_local = {
	[ACTION_STATE_START][RTE_FLOW_ACTION_TYPE_SET_MAC_SRC] = {
		ACTION_STATE_ROUTING_SMAC, &handle_action_set_mac_src
	},
	[ACTION_STATE_ROUTING_SMAC][RTE_FLOW_ACTION_TYPE_SET_MAC_DST] = {
		ACTION_STATE_ROUTING_SMAC_DMAC, &handle_action_set_mac_dst,
	},
	[ACTION_STATE_ROUTING_SMAC_DMAC][RTE_FLOW_ACTION_TYPE_DEC_TTL] = {
		ACTION_STATE_ROUTING_SMAC_DMAC_TTL, &handle_action_local_dec_ttl
	},
	[ACTION_STATE_ROUTING_SMAC_DMAC_TTL][RTE_FLOW_ACTION_TYPE_PORT_ID] = {
		ACTION_STATE_PORT_ID, &handle_action_port_id_local
	},

	[ACTION_STATE_ROUTING_SMAC_DMAC_TTL][RTE_FLOW_ACTION_TYPE_SET_IPV4_DST] = {
		ACTION_STATE_SET_IPV4_DST, &handle_action_set_ipv4_dst
	},
	[ACTION_STATE_START][RTE_FLOW_ACTION_TYPE_SET_IPV4_DST] = {
		ACTION_STATE_SET_IPV4_DST, &handle_action_set_ipv4_dst
	},
	[ACTION_STATE_SET_IPV4_DST][RTE_FLOW_ACTION_TYPE_PORT_ID] = {
		ACTION_STATE_PORT_ID, &handle_action_port_id_local
	},

	[ACTION_STATE_ROUTING_SMAC_DMAC][RTE_FLOW_ACTION_TYPE_SET_IPV4_SRC] = {
		ACTION_STATE_SET_IPV4_SRC, &handle_action_set_ipv4_src
	},
	[ACTION_STATE_SET_IPV4_SRC][RTE_FLOW_ACTION_TYPE_DEC_TTL] = {
		ACTION_STATE_ROUTING_SMAC_DMAC_TTL, &handle_action_local_dec_ttl
	},
	[ACTION_STATE_START][RTE_FLOW_ACTION_TYPE_SET_IPV4_SRC] = {
		ACTION_STATE_SET_IPV4_SRC, &handle_action_set_ipv4_src
	},
	[ACTION_STATE_SET_IPV4_SRC][RTE_FLOW_ACTION_TYPE_PORT_ID] = {
		ACTION_STATE_PORT_ID, &handle_action_port_id_local
	},

	[ACTION_STATE_START][RTE_FLOW_ACTION_TYPE_PORT_ID] = {
		ACTION_STATE_PORT_ID, &handle_action_port_id_local
	},
	[ACTION_STATE_PORT_ID][RTE_FLOW_ACTION_TYPE_END] = {
		ACTION_STATE_END, NULL
	},

	[ACTION_STATE_PORT_ID][RTE_FLOW_ACTION_TYPE_MIRROR] = {
		ACTION_STATE_MIRROR, &handle_action_mirror
	},
	[ACTION_STATE_MIRROR][RTE_FLOW_ACTION_TYPE_END] = {
		ACTION_STATE_END, NULL
	},

	[ACTION_STATE_START][RTE_FLOW_ACTION_TYPE_DROP] = {
		ACTION_STATE_DROP, &handle_action_drop,
	},
	[ACTION_STATE_DROP][RTE_FLOW_ACTION_TYPE_END] = {
		ACTION_STATE_END, NULL
	},
};

static const action_transitions_t action_transitions_egress = {
	[ACTION_STATE_START][RTE_FLOW_ACTION_TYPE_RAW_DECAP] = {
		ACTION_STATE_RAW_DECAP, &handle_action_raw_decap_l2
	},
	[ACTION_STATE_SET_IPV4_SRC][RTE_FLOW_ACTION_TYPE_RAW_DECAP] = {
		ACTION_STATE_RAW_DECAP, &handle_action_raw_decap_l2
	},
	[ACTION_STATE_RAW_DECAP][RTE_FLOW_ACTION_TYPE_DEC_TTL] = {
		ACTION_STATE_RAW_DEC_TTL, &handle_action_egress_dec_ttl
	},
	[ACTION_STATE_RAW_DEC_TTL][RTE_FLOW_ACTION_TYPE_RAW_ENCAP] = {
		ACTION_STATE_RAW_ENCAP, &handle_action_raw_encap
	},

	[ACTION_STATE_START][RTE_FLOW_ACTION_TYPE_SET_IPV4_SRC] = {
		ACTION_STATE_SET_IPV4_SRC, &handle_action_set_ipv4_src
	},
	[ACTION_STATE_SET_IPV4_SRC][RTE_FLOW_ACTION_TYPE_RAW_ENCAP] = {
		ACTION_STATE_RAW_ENCAP, &handle_action_raw_encap,
	},

	[ACTION_STATE_START][RTE_FLOW_ACTION_TYPE_VXLAN_ENCAP] = {
		ACTION_STATE_ENCAP_VXLAN, &handle_action_vxlan_encap,
	},
	[ACTION_STATE_SET_IPV4_SRC][RTE_FLOW_ACTION_TYPE_VXLAN_ENCAP] = {
		ACTION_STATE_ENCAP_VXLAN, &handle_action_vxlan_encap
	},
	[ACTION_STATE_ENCAP_VXLAN][RTE_FLOW_ACTION_TYPE_PORT_ID] = {
		ACTION_STATE_PORT_ID, &handle_action_port_id,
	},

	[ACTION_STATE_START][RTE_FLOW_ACTION_TYPE_RAW_ENCAP] = {
		ACTION_STATE_RAW_ENCAP, &handle_action_raw_encap,
	},
	[ACTION_STATE_RAW_ENCAP][RTE_FLOW_ACTION_TYPE_PORT_ID] = {
		ACTION_STATE_PORT_ID, &handle_action_port_id,
	},

	[ACTION_STATE_PORT_ID][RTE_FLOW_ACTION_TYPE_MIRROR] = {
		ACTION_STATE_MIRROR, &handle_action_mirror
	},
	[ACTION_STATE_MIRROR][RTE_FLOW_ACTION_TYPE_END] = {
		ACTION_STATE_END, NULL,
	},

	[ACTION_STATE_PORT_ID][RTE_FLOW_ACTION_TYPE_END] = {
		ACTION_STATE_END, NULL,
	},
};

static const action_transitions_t action_transitions_ingress = {
	[ACTION_STATE_START][RTE_FLOW_ACTION_TYPE_RAW_DECAP] = {
		ACTION_STATE_RAW_DECAP, &handle_action_raw_decap,
	},
	[ACTION_STATE_RAW_DECAP][RTE_FLOW_ACTION_TYPE_RAW_ENCAP] = {
		ACTION_STATE_RAW_ENCAP, &handle_action_raw_encap_l2
	},

	[ACTION_STATE_RAW_DECAP][RTE_FLOW_ACTION_TYPE_SET_IPV4_DST] = {
		ACTION_STATE_SET_IPV4_DST, &handle_action_set_ipv4_dst
	},
	[ACTION_STATE_RAW_ENCAP][RTE_FLOW_ACTION_TYPE_SET_IPV4_DST] = {
		ACTION_STATE_SET_IPV4_DST, &handle_action_set_ipv4_dst
	},
	[ACTION_STATE_SET_IPV4_DST][RTE_FLOW_ACTION_TYPE_PORT_ID] = {
		ACTION_STATE_PORT_ID, &handle_action_port_id
	},

	[ACTION_STATE_START][RTE_FLOW_ACTION_TYPE_VXLAN_DECAP] = {
		ACTION_STATE_DECAP_VXLAN, &handle_action_vxlan_decap,
	},
	[ACTION_STATE_DECAP_VXLAN][RTE_FLOW_ACTION_TYPE_PORT_ID] = {
		ACTION_STATE_PORT_ID, &handle_action_port_id_ingress_l2_check,
	},
	[ACTION_STATE_DECAP_VXLAN][RTE_FLOW_ACTION_TYPE_SET_IPV4_DST] = {
		ACTION_STATE_SET_IPV4_DST, &handle_action_set_ipv4_dst,
	},

	[ACTION_STATE_RAW_ENCAP][RTE_FLOW_ACTION_TYPE_PORT_ID] = {
		ACTION_STATE_PORT_ID, &handle_action_port_id
	},
	[ACTION_STATE_RAW_DECAP][RTE_FLOW_ACTION_TYPE_PORT_ID] = {
		ACTION_STATE_PORT_ID, &handle_action_port_id_ingress_l2_check
	},
	[ACTION_STATE_PORT_ID][RTE_FLOW_ACTION_TYPE_END] = {
		ACTION_STATE_END, NULL,
	},

	[ACTION_STATE_PORT_ID][RTE_FLOW_ACTION_TYPE_MIRROR] = {
		ACTION_STATE_MIRROR, &handle_action_mirror
	},
	[ACTION_STATE_MIRROR][RTE_FLOW_ACTION_TYPE_END] = {
		ACTION_STATE_END, NULL,
	},

	[ACTION_STATE_START][RTE_FLOW_ACTION_TYPE_DROP] = {
		ACTION_STATE_DROP, &handle_action_drop,
	},
	[ACTION_STATE_DROP][RTE_FLOW_ACTION_TYPE_END] = {
		ACTION_STATE_END, NULL
	},
};

static const action_transitions_ptr_t action_transitions[] = {
	[FLOW_TYPE_LOCAL] = action_transitions_local,
	[FLOW_TYPE_EGRESS] = action_transitions_egress,
	[FLOW_TYPE_INGRESS] = action_transitions_ingress,
};

static enum n3k_mgmt_parser_flow_type
deduce_flow_type(
	const struct rte_flow_item patterns[],
	const struct rte_flow_action actions[])
{
	size_t i = 0;

	size_t ip_pattern_count = 0;
	for (i = 0; patterns[i].type != RTE_FLOW_ITEM_TYPE_END; ++i)
		if ((patterns[i].type == RTE_FLOW_ITEM_TYPE_IPV4) ||
			(patterns[i].type == RTE_FLOW_ITEM_TYPE_IPV6))
			if (++ip_pattern_count == 2)
				return FLOW_TYPE_INGRESS;

	for (i = 0; actions[i].type != RTE_FLOW_ACTION_TYPE_END; ++i)
		if (actions[i].type == RTE_FLOW_ACTION_TYPE_RAW_ENCAP ||
			actions[i].type == RTE_FLOW_ACTION_TYPE_VXLAN_ENCAP)
			return FLOW_TYPE_EGRESS;

	return FLOW_TYPE_LOCAL;
}

static int
parse_patterns(
	struct n3k_mgmt_flow_entry *flow,
	const struct rte_flow_item patterns[],
	enum n3k_mgmt_parser_flow_type flow_type)
{
	int rc;
	size_t pos;

	enum n3k_mgmt_parser_pattern_state state = PATTERN_STATE_START;
	const pattern_transitions_ptr_t transitions =
		pattern_transitions[flow_type];

	N3K_MGMT_LOG(FLOW_PARSE, DEBUG, "pattern flow type %s", n3k_mgmt_parser_flow_type_names[flow_type]);

	for (pos = 0; state != PATTERN_STATE_END; pos++) {
		enum rte_flow_item_type type = patterns[pos].type;
		N3K_MGMT_LOG(FLOW_PARSE, DEBUG, "pattern pos %zu", pos);
		if (type >= PATTERN_MAX_VALUE) {
			N3K_MGMT_LOG(FLOW_PARSE, ERR,
				"Invalid pattern type %d", type);
			return -EINVAL;
		}

		if (type == RTE_FLOW_ITEM_TYPE_VOID)
			continue;

		const struct pattern_transition *transition =
			&transitions[state][type];
		N3K_MGMT_LOG(FLOW_PARSE, DEBUG, "pattern transition %s -> %s",
			n3k_mgmt_parser_pattern_state_names[state],
			n3k_mgmt_parser_item_type_names[type]);
		if (transition->next == PATTERN_STATE_INVALID) {
			N3K_MGMT_LOG(FLOW_PARSE, ERR, "Unsupported pattern type %s",
				n3k_mgmt_parser_item_type_names[type]);
			return -EINVAL;
		}

		if (transition->handler != NULL) {
			rc = transition->handler(&patterns[pos], flow);
			if (rc) {
				N3K_MGMT_LOG(FLOW_PARSE, ERR,
					"Transition handler failed %s",
					n3k_mgmt_parser_item_type_names[type]);
				return rc;
			}
		}

		state = transition->next;
	}
	N3K_MGMT_LOG(FLOW_PARSE, DEBUG, "pattern parsing ok");
	return 0;
}

static int
parse_actions(
	struct n3k_mgmt_flow_entry *flow,
	const struct rte_flow_action actions[],
	enum n3k_mgmt_parser_flow_type flow_type)
{
	int rc;
	size_t pos;

	enum n3k_mgmt_parser_action_state state = ACTION_STATE_START;
	const action_transitions_ptr_t transitions =
		action_transitions[flow_type];

	N3K_MGMT_LOG(FLOW_PARSE, DEBUG, "action flow_type %s", n3k_mgmt_parser_flow_type_names[flow_type]);

	for (pos = 0; state != ACTION_STATE_END; pos++) {
		N3K_MGMT_LOG(FLOW_PARSE, DEBUG, "action pos %zu", pos);
		enum rte_flow_action_type type = actions[pos].type;
		if (type >= ACTION_MAX_VALUE) {
			N3K_MGMT_LOG(FLOW_PARSE, ERR,
				"Invalid action type %d", type);
			return -EINVAL;
		}

		if (type == RTE_FLOW_ACTION_TYPE_VOID)
			continue;

		const struct action_transition *transition =
			&transitions[state][type];
		N3K_MGMT_LOG(FLOW_PARSE, DEBUG, "action transition %s -> %s",
			n3k_mgmt_parser_action_state_names[state],
			n3k_mgmt_parser_action_type_names[type]);
		if (transition->next == ACTION_STATE_INVALID) {
			N3K_MGMT_LOG(FLOW_PARSE, ERR, "Unsupported action type %s",
				n3k_mgmt_parser_action_type_names[type]);
			return -EINVAL;
		}

		if (transition->handler != NULL) {
			rc = transition->handler(&actions[pos], flow);
			if (rc) {
				N3K_MGMT_LOG(FLOW_PARSE, ERR,
					"Transition handler failed %s",
					n3k_mgmt_parser_action_type_names[type]);
				return rc;
			}
		}

		state = transition->next;
	}
	N3K_MGMT_LOG(FLOW_PARSE, DEBUG, "action parsing ok");
	return 0;
}

static int
get_target_action_queue_num(struct n3k_mgmt_flow_entry* parsed_flow,
	uint16_t* queues) {
	struct n3k_vdev_dev_id* dev_id =
		&parsed_flow->action.action_raw.port.device_id;
	switch (dev_id->type) {
	case N3K_VDEV_DEV_TYPE_PHY:
	case N3K_VDEV_DEV_TYPE_LAG:
		n3k_mgmt_init_get_pf_mq(dev_id->id, (u8*) queues);
		return 0;
	case N3K_VDEV_DEV_TYPE_VF:
		n3k_mgmt_init_get_vf_mq(0, dev_id->id, (u8*) queues);
		return 0;
	}
	return -EINVAL;
}

static int
calculate_hash(struct n3k_mgmt_flow_entry* parsed_flow, uint16_t* queue)
{
	struct n3k_mgmt_flow_hash_data hash_data = { 0 };
	uint32_t hash;

	switch (parsed_flow->key.key_raw.l3.type) {
		case N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4:
			rte_memcpy(hash_data.daddr, &parsed_flow->key.key_raw.l3.ipv4.dst_ip,
					sizeof(parsed_flow->key.key_raw.l3.ipv4.dst_ip));
			rte_memcpy(hash_data.saddr, &parsed_flow->key.key_raw.l3.ipv4.src_ip,
					sizeof(parsed_flow->key.key_raw.l3.ipv4.dst_ip));
			break;

		case N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6:
			rte_memcpy(hash_data.daddr, &parsed_flow->key.key_raw.l3.ipv6.dst_ip,
					sizeof(parsed_flow->key.key_raw.l3.ipv4.dst_ip));
			rte_memcpy(hash_data.saddr, &parsed_flow->key.key_raw.l3.ipv6.src_ip,
					sizeof(parsed_flow->key.key_raw.l3.ipv4.dst_ip));
			break;

		default:
			N3K_MGMT_LOG(FLOW_PARSE, ERR,
					"Don't know how to calculate hash for l3 key type %d",
					parsed_flow->key.key_raw.l3.type);
			break;
	}

	hash_data.dport = parsed_flow->key.key_raw.l4.dst_port;
	hash_data.sport = parsed_flow->key.key_raw.l4.src_port;

	hash = rte_jhash(&hash_data, sizeof(hash_data), 0);
	uint16_t queues_num = 0;
	int rc = get_target_action_queue_num(parsed_flow, &queues_num);
	if (rc) {
		N3K_MGMT_LOG(FLOW_PARSE, ERR,
			"get_target_action_queue_num failed %d", rc);
		return rc;
	}

	*queue = hash % queues_num;
	return 0;
}

int
n3k_mgmt_rte_flow_to_tables(
	const struct rte_flow_item patterns[],
	const struct rte_flow_action actions[],
	struct n3k_mgmt_flow_entry *flow)
{
	int rc;

	memset(flow, 0, sizeof(*flow));

	const enum n3k_mgmt_parser_flow_type flow_type = deduce_flow_type(patterns, actions);
	if (flow_type == FLOW_TYPE_UNKNOWN)
		return -EINVAL;

	rc = parse_patterns(flow, patterns, flow_type);
	if (rc)
		return rc;

	rc = parse_actions(flow, actions, flow_type);
	if (rc)
		return rc;

	uint16_t queue_id = 0;
	int ret = calculate_hash(flow, &queue_id);
	if (ret) {
		N3K_MGMT_LOG(FLOW_PARSE, ERR,
			"calculating hash for queue id failed %d", ret);
		return ret;
	}
	flow->action.action_raw.queue = queue_id;

	return 0;
}

bool
n3k_mgmt_rte_flow_is_lacp_flow(
	const struct rte_flow_item patterns[],
	__rte_unused const struct rte_flow_action actions[])
{
	if (patterns[0].type == RTE_FLOW_ITEM_TYPE_ETH &&
		patterns[1].type == RTE_FLOW_ITEM_TYPE_END)
	{
		const struct rte_flow_item_eth *eth_item =
			(const struct rte_flow_item_eth *)patterns[0].spec;
		return eth_item->type == RTE_BE16(RTE_ETHER_TYPE_SLOW);
	}
	return false;
}

bool
n3k_mgmt_rte_flow_verify(
	const struct rte_flow_item patterns[],
	const struct rte_flow_action actions[])
{
	struct n3k_mgmt_flow_entry flow;

	// check LACP flow
	if (n3k_mgmt_rte_flow_is_lacp_flow(patterns, actions))
		return true;

	// check normal flow
	return n3k_mgmt_rte_flow_to_tables(patterns, actions, &flow) == 0;
}
