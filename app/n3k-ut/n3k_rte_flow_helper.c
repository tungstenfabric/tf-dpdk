/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_rte_flow_helper.h"

#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_mpls.h>
#include <rte_udp.h>

enum n3k_rte_flow_encap_pattern {
	ENCAP_ETH,
	ENCAP_IPV4,
	ENCAP_UDP,
	ENCAP_VXLAN,
	ENCAP_END
};

enum { MAX_ITEMS_COUNT = 128 };

static struct rte_flow_item_port_id port_id_spec;
static struct rte_flow_item_eth outer_eth_spec;
static struct rte_flow_item_ipv4 outer_ipv4_spec;
static struct rte_flow_item_ipv4 outer_ipv4_vxlan_spec;
static struct rte_flow_item_udp outer_udp_spec;
static struct rte_flow_item_vxlan vxlan_spec;
static struct rte_flow_item_mpls mpls_spec;
static struct rte_flow_item_eth inner_eth_spec;
static struct rte_flow_item_ipv4 inner_ipv4_spec;
static struct rte_flow_item_ipv6 inner_ipv6_spec;
static struct rte_flow_item_udp inner_udp_spec;
static struct rte_flow_item_tcp inner_tcp_spec;

static struct rte_flow_item_eth outer_eth_mask = {
	.type = -1,
	.src = {
		.addr_bytes = {
			0xff, 0xff, 0xff,
			0xff, 0xff, 0xff,
		},
	},
	.dst = {
		.addr_bytes = {
			0xff, 0xff, 0xff,
			0xff, 0xff, 0xff,
		},
	},
};
static struct rte_flow_item_ipv4 outer_ipv4_vxlan_mask = {
	.hdr = {
		.src_addr = -1,
		.dst_addr = -1,
		.next_proto_id = -1,
		.time_to_live = -1,
		.type_of_service = -1,
	},
};
static struct rte_flow_item_udp outer_udp_mask = {
	.hdr = {
		.src_port = -1,
		.dst_port = -1,
	},
};
static struct rte_flow_item_vxlan vxlan_mask = {
	.vni = "\xff\xff\xff",
};

struct rte_flow_item pattern[MAX_ITEMS_COUNT + 1] = {
	[MAX_ITEMS_COUNT] = {
		.type = RTE_FLOW_ITEM_TYPE_END,
		.spec = NULL,
	},
};

static uint8_t raw_decap_data[2048];

static struct rte_flow_action_port_id port_id_action_conf;
static struct rte_flow_action_set_ipv4 set_ipv4_src_action_conf;
static struct rte_flow_action_set_ipv4 set_ipv4_dst_action_conf;
static struct rte_flow_action_set_mac set_smac_action_conf;
static struct rte_flow_action_set_mac set_dmac_action_conf;
static struct rte_flow_action_raw_encap raw_encap_action_conf;
static struct rte_flow_action_raw_decap raw_decap_action_conf = {
	.data = raw_decap_data,
};
static struct rte_flow_action_mirror mirror_conf;

static struct rte_flow_item encap_pattern[] = {
	[ENCAP_ETH] = {
		.type = RTE_FLOW_ITEM_TYPE_ETH,
		.spec = &outer_eth_spec,
		.mask = &outer_eth_mask,
		.last = NULL,
	},
	[ENCAP_IPV4] = {
		.type = RTE_FLOW_ITEM_TYPE_IPV4,
		.spec = &outer_ipv4_vxlan_spec,
		.mask = &outer_ipv4_vxlan_mask,
		.last = NULL,
	},
	[ENCAP_UDP] = {
		.type = RTE_FLOW_ITEM_TYPE_UDP,
		.spec = &outer_udp_spec,
		.mask = &outer_udp_mask,
		.last = NULL,
	},
	[ENCAP_VXLAN] = {
		.type = RTE_FLOW_ITEM_TYPE_VXLAN,
		.spec = &vxlan_spec,
		.mask = &vxlan_mask,
		.last = NULL,
	},
	[ENCAP_END] = {
		.type = RTE_FLOW_ITEM_TYPE_END,
		.spec = NULL,
		.mask = NULL,
		.last = NULL,
	},
};

static struct rte_flow_action_vxlan_encap vxlan_encap_action_conf = {
	.definition = encap_pattern,
};
static struct rte_flow_action_vxlan_encap vxlan_decap_action_conf;

static uint8_t raw_encap_data[4096];
static size_t raw_encap_size;

struct rte_flow_action actions[MAX_ITEMS_COUNT + 1] = {
	[MAX_ITEMS_COUNT] = {
		.type = RTE_FLOW_ACTION_TYPE_END,
		.conf = NULL,
	},
};

static size_t next_pattern_idx;
static size_t next_action_idx;

struct port_to_device_id {
	bool active;
	uint16_t dpdk_port;
	struct n3k_vdev_dev_id device_id;
};

static struct port_to_device_id devices[RTE_MAX_ETHPORTS];

static inline void
set_next_pattern(enum rte_flow_item_type type, const void *spec)
{
	struct rte_flow_item *next_pattern = &pattern[next_pattern_idx++];
	next_pattern->type = type;
	next_pattern->spec = spec;
}

static inline void
set_next_action(enum rte_flow_action_type type, const void *conf)
{
	struct rte_flow_action *next_action = &actions[next_action_idx++];
	next_action->type = type;
	next_action->conf = conf;
}

void
reset_patterns_and_actions(void)
{
	size_t i;

	for (i = 0; i < MAX_ITEMS_COUNT; ++i) {
		pattern[i].type = RTE_FLOW_ITEM_TYPE_VOID;
		pattern[i].spec = NULL;
	}

	for (i = 0; i < MAX_ITEMS_COUNT; ++i) {
		actions[i].type = RTE_FLOW_ACTION_TYPE_VOID;
		actions[i].conf = NULL;
	}

	next_pattern_idx = 0;
	next_action_idx = 0;

	memset(raw_encap_data, 0, sizeof(raw_encap_data));
	raw_encap_size = 0;
}

void
set_pattern_port_id(rte_le32_t port_id)
{
	port_id_spec = (struct rte_flow_item_port_id) {
		.id = port_id,
	};

	set_next_pattern(RTE_FLOW_ITEM_TYPE_PORT_ID, &port_id_spec);
}

void
set_pattern_outer_eth(
	struct rte_ether_addr src_mac,
	struct rte_ether_addr dst_mac)
{
	outer_eth_spec = (struct rte_flow_item_eth) {
		.src = src_mac,
		.dst = dst_mac,
		.type = RTE_BE16(RTE_ETHER_TYPE_IPV4),
	};

	set_next_pattern(RTE_FLOW_ITEM_TYPE_ETH, &outer_eth_spec);
}

void
set_pattern_outer_ipv4(rte_le32_t src_ip, rte_le32_t dst_ip)
{
	outer_ipv4_spec = (struct rte_flow_item_ipv4) {
		.hdr = {
			.src_addr = RTE_BE32(src_ip),
			.dst_addr = RTE_BE32(dst_ip),
			.next_proto_id = IPPROTO_UDP,
		}
	};

	set_next_pattern(RTE_FLOW_ITEM_TYPE_IPV4, &outer_ipv4_spec);
}

void
set_pattern_outer_udp(rte_le16_t src_port, rte_le16_t dst_port)
{
	outer_udp_spec = (struct rte_flow_item_udp) {
		.hdr = {
			.src_port = RTE_BE16(src_port),
			.dst_port = RTE_BE16(dst_port),
		}
	};

	set_next_pattern(RTE_FLOW_ITEM_TYPE_UDP, &outer_udp_spec);
}

void
set_pattern_vxlan(rte_le32_t vni)
{
	const rte_be32_t vxlan_vni = RTE_BE32(vni << 8);
	memcpy(vxlan_spec.vni, &vxlan_vni, 3);

	set_next_pattern(RTE_FLOW_ITEM_TYPE_VXLAN, &vxlan_spec);
}

void
set_pattern_mpls(rte_le32_t label, uint8_t tc)
{
	rte_be32_t value = RTE_BE32((label << 12) | ((tc & 0x7) << 9) | (1 << 8));
	memcpy(mpls_spec.label_tc_s, &value, sizeof(mpls_spec.label_tc_s));

	set_next_pattern(RTE_FLOW_ITEM_TYPE_MPLS, &mpls_spec);
}

void
set_pattern_inner_eth(
	struct rte_ether_addr src_mac,
	struct rte_ether_addr dst_mac)
{
	inner_eth_spec = (struct rte_flow_item_eth) {
		.src = src_mac,
		.dst = dst_mac,
		.type = RTE_BE16(RTE_ETHER_TYPE_IPV4),
	};

	set_next_pattern(RTE_FLOW_ITEM_TYPE_ETH, &inner_eth_spec);
}

void
set_pattern_inner_ipv4(rte_le32_t src_ip, rte_le32_t dst_ip, uint8_t ip_proto)
{
	inner_ipv4_spec = (struct rte_flow_item_ipv4) {
		.hdr = {
			.src_addr = RTE_BE32(src_ip),
			.dst_addr = RTE_BE32(dst_ip),
			.next_proto_id = ip_proto,
		}
	};

	set_next_pattern(RTE_FLOW_ITEM_TYPE_IPV4, &inner_ipv4_spec);
}

void
set_pattern_inner_ipv6(const uint8_t *src_ip, const uint8_t *dst_ip, uint8_t ip_proto)
{
	inner_ipv6_spec.hdr.proto = ip_proto;
	rte_memcpy(inner_ipv6_spec.hdr.src_addr, src_ip, 16);
	rte_memcpy(inner_ipv6_spec.hdr.dst_addr, dst_ip, 16);
	set_next_pattern(RTE_FLOW_ITEM_TYPE_IPV6, &inner_ipv6_spec);
}

void
set_pattern_inner_udp(rte_le16_t src_port, rte_le16_t dst_port)
{
	inner_udp_spec = (struct rte_flow_item_udp) {
		.hdr = {
			.src_port = RTE_BE16(src_port),
			.dst_port = RTE_BE16(dst_port),
		}
	};

	set_next_pattern(RTE_FLOW_ITEM_TYPE_UDP, &inner_udp_spec);
}

void
set_pattern_inner_tcp(rte_le16_t src_port, rte_le16_t dst_port)
{
	inner_tcp_spec = (struct rte_flow_item_tcp) {
		.hdr = {
			.src_port = RTE_BE16(src_port),
			.dst_port = RTE_BE16(dst_port),
		}
	};

	set_next_pattern(RTE_FLOW_ITEM_TYPE_TCP, &inner_tcp_spec);
}

void
set_pattern_void(void)
{
	set_next_pattern(RTE_FLOW_ITEM_TYPE_VOID, NULL);
}

void
set_action_set_mac_src(struct rte_ether_addr src_mac)
{
	rte_memcpy(
		set_smac_action_conf.mac_addr,
		src_mac.addr_bytes,
		RTE_ETHER_ADDR_LEN
	);

	set_next_action(RTE_FLOW_ACTION_TYPE_SET_MAC_SRC, &set_smac_action_conf);
}

void
set_action_set_mac_dst(struct rte_ether_addr dst_mac)
{
	rte_memcpy(
		set_dmac_action_conf.mac_addr,
		dst_mac.addr_bytes,
		RTE_ETHER_ADDR_LEN
	);

	set_next_action(RTE_FLOW_ACTION_TYPE_SET_MAC_DST, &set_dmac_action_conf);
}

void
set_action_dec_ttl(void)
{
	set_next_action(RTE_FLOW_ACTION_TYPE_DEC_TTL, NULL);
}

void
set_action_vxlan_encap(void)
{
	set_next_action(RTE_FLOW_ACTION_TYPE_VXLAN_ENCAP, &vxlan_encap_action_conf);
}

void
set_action_raw_encap(void)
{
	raw_encap_action_conf = (struct rte_flow_action_raw_encap) {
		.data = raw_encap_data,
		.preserve = NULL,
		.size = raw_encap_size,
	};

	set_next_action(RTE_FLOW_ACTION_TYPE_RAW_ENCAP, &raw_encap_action_conf);
}

void
set_action_decap_vxlan(void)
{
	set_next_action(RTE_FLOW_ACTION_TYPE_VXLAN_DECAP, &vxlan_decap_action_conf);
}

void
set_action_raw_decap_mpls(void)
{
	raw_decap_action_conf = (struct rte_flow_action_raw_decap) {
		.size =
			sizeof(struct rte_ether_hdr) +
			RTE_IPV4_IHL_MULTIPLIER * RTE_IPV4_MIN_IHL +
			sizeof(struct rte_udp_hdr) +
			sizeof(struct rte_mpls_hdr),
	};

	set_next_action(RTE_FLOW_ACTION_TYPE_RAW_DECAP, &raw_decap_action_conf);
}

void
set_action_raw_decap_eth(void)
{
	raw_decap_action_conf = (struct rte_flow_action_raw_decap) {
		.size = sizeof(struct rte_ether_hdr),
	};

	set_next_action(RTE_FLOW_ACTION_TYPE_RAW_DECAP, &raw_decap_action_conf);
}

void
set_action_ipv4_src(uint32_t ip)
{
	set_ipv4_src_action_conf = (struct rte_flow_action_set_ipv4) {
		.ipv4_addr = RTE_BE32(ip),
	};
	set_next_action(RTE_FLOW_ACTION_TYPE_SET_IPV4_SRC, &set_ipv4_src_action_conf);
}

void
set_action_ipv4_dst(uint32_t ip)
{
	set_ipv4_dst_action_conf = (struct rte_flow_action_set_ipv4) {
		.ipv4_addr = RTE_BE32(ip),
	};
	set_next_action(RTE_FLOW_ACTION_TYPE_SET_IPV4_DST, &set_ipv4_dst_action_conf);
}

void
set_action_port_id(rte_le32_t port)
{
	port_id_action_conf = (struct rte_flow_action_port_id) {
		.id = port,
	};

	set_next_action(RTE_FLOW_ACTION_TYPE_PORT_ID, &port_id_action_conf);
}

void
set_action_mirror(uint32_t port, uint8_t mirror_modified)
{
	mirror_conf = (struct rte_flow_action_mirror) {
		.port = port,
		.mirror_modified = mirror_modified,
	};
	set_next_action(RTE_FLOW_ACTION_TYPE_MIRROR, &mirror_conf);
}

void
set_action_drop(void)
{
	set_next_action(RTE_FLOW_ACTION_TYPE_DROP, NULL);
}

void
set_action_void(void)
{
	set_next_action(RTE_FLOW_ACTION_TYPE_VOID, NULL);
}

void
set_device_id(uint16_t dpdk_port,
	struct n3k_vdev_dev_id device_id)
{
	int i;
	for (i = 0; i < RTE_MAX_ETHPORTS; ++i) {
		if (!devices[i].active) {
			devices[i].active = true;
			devices[i].dpdk_port = dpdk_port;
			devices[i].device_id.id = device_id.id;
			devices[i].device_id.type = device_id.type;
			return;
		}
	}
}

struct n3k_vdev_dev_id
get_device_id(uint16_t dpdk_port)
{
	struct n3k_vdev_dev_id ret = {.type = N3K_VDEV_DEV_TYPE_PHY, .id = 0};
	int i;
	for (i = 0; i < RTE_MAX_ETHPORTS; ++i) {
		if (devices[i].active &&
			devices[i].dpdk_port == dpdk_port) {
			ret = devices[i].device_id;
			break;
		}
	}
	return ret;
}

void
clear_device_ids(void)
{
	int i;
	for (i = 0; i < RTE_MAX_ETHPORTS; ++i) {
		devices[i].active = false;
	}
}

bool
assert_device_id_equal(struct n3k_vdev_dev_id dev1,
	struct n3k_vdev_dev_id dev2)
{
	return (dev1.id == dev2.id &&
		dev1.type == dev2.type);
}

void
set_encap_eth(struct rte_ether_addr src_mac, struct rte_ether_addr dst_mac)
{
	struct rte_ether_hdr *eth_hdr =
		(struct rte_ether_hdr *)(raw_encap_data + raw_encap_size);
	raw_encap_size += sizeof(*eth_hdr);

	eth_hdr->d_addr = dst_mac;
	eth_hdr->s_addr = src_mac;
	eth_hdr->ether_type = RTE_BE16(RTE_ETHER_TYPE_IPV4);
}

void
set_encap_ipv4(rte_le32_t src_ip, rte_le32_t dst_ip, uint8_t ttl, uint8_t tos)
{
	struct rte_ipv4_hdr *ipv4_hdr =
		(struct rte_ipv4_hdr *)(raw_encap_data + raw_encap_size);
	raw_encap_size += sizeof(*ipv4_hdr);

	ipv4_hdr->src_addr = RTE_BE32(src_ip);
	ipv4_hdr->dst_addr = RTE_BE32(dst_ip);
	ipv4_hdr->next_proto_id = IPPROTO_UDP;
	ipv4_hdr->time_to_live = ttl;
	ipv4_hdr->type_of_service = tos;
}

void
set_encap_udp(rte_le16_t src_port, rte_le16_t dst_port)
{
	struct rte_udp_hdr *udp_hdr =
		(struct rte_udp_hdr *)(raw_encap_data + raw_encap_size);
	raw_encap_size += sizeof(*udp_hdr);

	udp_hdr->src_port = RTE_BE16(src_port);
	udp_hdr->dst_port = RTE_BE16(dst_port);
}

void
set_encap_eth_vxlan(struct rte_ether_addr src_mac, struct rte_ether_addr dst_mac)
{
	outer_eth_spec.dst = dst_mac;
	outer_eth_spec.src = src_mac;
	outer_eth_spec.type = RTE_BE16(RTE_ETHER_TYPE_IPV4);
}

void
set_encap_ipv4_vxlan(rte_le32_t src_ip, rte_le32_t dst_ip, uint8_t ttl, uint8_t tos)
{
	outer_ipv4_vxlan_spec.hdr.src_addr = RTE_BE32(src_ip);
	outer_ipv4_vxlan_spec.hdr.dst_addr = RTE_BE32(dst_ip);
	outer_ipv4_vxlan_spec.hdr.next_proto_id = IPPROTO_UDP;
	outer_ipv4_vxlan_spec.hdr.time_to_live = ttl;
	outer_ipv4_vxlan_spec.hdr.type_of_service = tos;
}

void
set_encap_udp_vxlan(rte_le16_t src_port, rte_le16_t dst_port)
{
	outer_udp_spec.hdr.src_port = RTE_BE16(src_port);
	outer_udp_spec.hdr.dst_port = RTE_BE16(dst_port);
}

void
set_encap_vxlan(rte_le32_t vni)
{
	const rte_be32_t vxlan_vni = RTE_BE32(vni << 8);
	memcpy(vxlan_spec.vni, &vxlan_vni, 3);
}

void
set_encap_mpls(rte_le32_t label, uint8_t tc)
{
	struct rte_mpls_hdr *mpls_hdr =
		(struct rte_mpls_hdr *)(raw_encap_data + raw_encap_size);
	raw_encap_size += sizeof(*mpls_hdr);

	mpls_hdr->tag_msb = RTE_BE16((label >> 4) & 0xffff);
	mpls_hdr->tag_lsb = label & 0xf;
	mpls_hdr->tc = tc;
}
