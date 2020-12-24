/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "../../drivers/net/n3k/mgmt/n3k_mgmt_ethdev.h"
#include "../../drivers/net/n3k/mgmt/n3k_mgmt_flow_tbl_handles.h"
#include "../../drivers/net/n3k/mgmt/n3k_mgmt_flr_command.h"
#include "../../drivers/net/n3k/mgmt/n3k_mgmt_flr_flow_tracker.h"
#include "../../drivers/net/n3k/mgmt/n3k_mgmt_flr_tbl.h"

#include <rte_hexdump.h>
#include <rte_pmd_n3k_flow_tbl_mgmt.h>
#include <rte_pmd_n3k_smac_tbl_mgmt.h>
#include <rte_pmd_n3k_tunnel_tbl_mgmt.h>
#include <rte_pmd_n3k_vplkp_tbl_mgmt.h>

#include <rte_cycles.h>
#include <rte_eal.h>
#include <rte_ethdev.h>
#include <rte_ethdev_driver.h>
#include <rte_ether.h>
#include <rte_lcore.h>
#include <rte_mbuf.h>

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define SRC_PORT_NAME "net_n3k0_vf1"
#define DST_PORT_NAME "net_n3k0_vf2"
#define DST_MIRROR_PORT_NAME "net_n3k0_vf3"

const struct n3k_vdev_dev_id SRC_DEVICE_ID = (struct n3k_vdev_dev_id) {
	.type = N3K_VDEV_DEV_TYPE_VF,
	.id = 1,
};
const struct n3k_vdev_dev_id DST_DEVICE_ID = (struct n3k_vdev_dev_id) {
	.type = N3K_VDEV_DEV_TYPE_VF,
	.id = 2,
};
const struct n3k_vdev_dev_id DST_MIRROR_DEVICE_ID = (struct n3k_vdev_dev_id) {
	.type = N3K_VDEV_DEV_TYPE_VF,
	.id = 3,
};

#define UDP_PORT_MPLS 0x19eb
#define UDP_PORT_VXLAN 0x12b5

#define RX_RING_SIZE 1024
#define TX_RING_SIZE 1024

#define NUM_MBUFS 8191
#define MBUF_CACHE_SIZE 250
#define BURST_SIZE 32

static const struct rte_eth_conf port_conf_default = {
	.rxmode = {
		.max_rx_pkt_len = RTE_ETHER_MAX_LEN,
	},
};

/* basicfwd.c: Basic DPDK skeleton forwarding example. */

/*
 * Initializes a given port using global settings and with the RX buffers
 * coming from the mbuf_pool passed as a parameter.
 */
static inline int
port_init(uint16_t port, struct rte_mempool *mbuf_pool)
{
	struct rte_eth_conf port_conf = port_conf_default;
	const uint16_t rx_rings = 1, tx_rings = 1;
	uint16_t nb_rxd = RX_RING_SIZE;
	uint16_t nb_txd = TX_RING_SIZE;
	int retval;
	uint16_t q;
	struct rte_eth_dev_info dev_info;
	struct rte_eth_txconf txconf;

	if (!rte_eth_dev_is_valid_port(port))
		return -1;

	rte_eth_dev_info_get(port, &dev_info);
	if (dev_info.tx_offload_capa & DEV_TX_OFFLOAD_MBUF_FAST_FREE)
		port_conf.txmode.offloads |=
			DEV_TX_OFFLOAD_MBUF_FAST_FREE;

	/* Configure the Ethernet device. */
	retval = rte_eth_dev_configure(port, rx_rings, tx_rings, &port_conf);
	if (retval != 0)
		return retval;

	retval = rte_eth_dev_adjust_nb_rx_tx_desc(port, &nb_rxd, &nb_txd);
	if (retval != 0)
		return retval;

	/* Allocate and set up 1 RX queue per Ethernet port. */
	for (q = 0; q < rx_rings; q++) {
		retval = rte_eth_rx_queue_setup(port, q, nb_rxd,
				rte_eth_dev_socket_id(port), NULL, mbuf_pool);
		if (retval < 0)
			return retval;
	}

	txconf = dev_info.default_txconf;
	txconf.offloads = port_conf.txmode.offloads;
	/* Allocate and set up 1 TX queue per Ethernet port. */
	for (q = 0; q < tx_rings; q++) {
		retval = rte_eth_tx_queue_setup(port, q, nb_txd,
				rte_eth_dev_socket_id(port), &txconf);
		if (retval < 0)
			return retval;
	}

	/* Start the Ethernet port. */
	retval = rte_eth_dev_start(port);
	if (retval < 0)
		return retval;

	/* Display the port MAC address. */
	struct rte_ether_addr addr;
	rte_eth_macaddr_get(port, &addr);
	printf("Port %u MAC: %02" PRIx8 " %02" PRIx8 " %02" PRIx8
			   " %02" PRIx8 " %02" PRIx8 " %02" PRIx8 "\n",
			port,
			addr.addr_bytes[0], addr.addr_bytes[1],
			addr.addr_bytes[2], addr.addr_bytes[3],
			addr.addr_bytes[4], addr.addr_bytes[5]);

	/* Enable RX in promiscuous mode for the Ethernet device. */
	rte_eth_promiscuous_enable(port);

	return 0;
}

typedef struct n3k_mgmt_flow_entry (*create_flow_entry_t)(
	uint16_t src_port, uint16_t dst_port);

static uint16_t
get_port_id_by_name(const char *dev_name)
{
	struct rte_eth_dev *dev;
	dev = rte_eth_dev_allocated(dev_name);
	RTE_ASSERT(dev != NULL);
	return dev->data->port_id;
}

static struct n3k_mgmt_flow_entry
create_local_flow_entry(uint16_t src_port, uint16_t dst_port)
{
	struct n3k_mgmt_flow_entry flow;
	memset(&flow, 0, sizeof(flow));
	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;

	struct rte_ether_addr key_smac = {
		.addr_bytes = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15}
	};
	struct rte_ether_addr key_dmac = {
		.addr_bytes = {0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b}
	};

	key->use_tunnel_key = false;
	key->use_l2_key = true;
	key->l2.src_mac = key_smac;
	key->l2.dst_mac = key_dmac;
	key->l3.type = N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4;
	key->l3.ipv4.src_ip = RTE_IPV4(1, 1, 1, 6);
	key->l3.ipv4.dst_ip = RTE_IPV4(1, 1, 1, 7);
	key->l4.src_port = src_port;
	key->l4.dst_port = dst_port;
	key->port.port_id = get_port_id_by_name(SRC_PORT_NAME);
	key->port.device_id = SRC_DEVICE_ID;
	key->l3.ip_prot = IPPROTO_UDP;

	struct rte_ether_addr action_smac = {
		.addr_bytes = {0x15, 0x14, 0x13, 0x12, 0x11, 0x10}
	};
	struct rte_ether_addr action_dmac = {
		.addr_bytes = {0x1b, 0x1a, 0x19, 0x18, 0x17, 0x16}
	};

	action->port.port_id = get_port_id_by_name(DST_PORT_NAME);
	action->port.device_id = DST_DEVICE_ID;
	action->type = N3K_MGMT_FLOW_TBL_ACTION_LOCAL;
	action->local_action.decr_ttl = true;
	action->local_action.modify_l2 = true;
	action->local_action.modified_l2.src_mac = action_smac;
	action->local_action.modified_l2.dst_mac = action_dmac;

	return flow;
}

static struct n3k_mgmt_flow_entry
create_local_mirror_flow_entry(uint16_t src_port, uint16_t dst_port)
{
	struct n3k_mgmt_flow_entry flow;
	memset(&flow, 0, sizeof(flow));
	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;

	struct rte_ether_addr key_smac = {
		.addr_bytes = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15}
	};
	struct rte_ether_addr key_dmac = {
		.addr_bytes = {0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b}
	};

	key->use_tunnel_key = false;
	key->use_l2_key = true;
	key->l2.src_mac = key_smac;
	key->l2.dst_mac = key_dmac;
	key->l3.type = N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4;
	key->l3.ipv4.src_ip = RTE_IPV4(1, 1, 1, 6);
	key->l3.ipv4.dst_ip = RTE_IPV4(1, 1, 1, 7);
	key->l4.src_port = src_port;
	key->l4.dst_port = dst_port;
	key->port.port_id = get_port_id_by_name(SRC_PORT_NAME);
	key->port.device_id = SRC_DEVICE_ID;
	key->l3.ip_prot = IPPROTO_UDP;

	action->port.port_id = get_port_id_by_name(DST_PORT_NAME);
	action->port.device_id = DST_DEVICE_ID;
	action->type = N3K_MGMT_FLOW_TBL_ACTION_LOCAL;

	action->use_mirroring = true;
	action->mirror.mode = N3K_MGMT_FLOW_TBL_MIRROR_MODE_ORIGINAL;
	action->mirror.port.port_id = get_port_id_by_name(DST_MIRROR_PORT_NAME);
	action->mirror.port.device_id = DST_MIRROR_DEVICE_ID;

	return flow;
}

static struct n3k_mgmt_flow_entry
create_encap_flow_entry(uint16_t src_port, uint16_t dst_port)
{
	struct n3k_mgmt_flow_entry flow;
	memset(&flow, 0, sizeof(flow));
	struct rte_ether_addr *smac = &flow.action.tunnel.smac_raw;
	struct n3k_mgmt_tunnel_tbl_entry *tunnel = &flow.action.tunnel.tunnel_raw;
	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;

	*smac = (struct rte_ether_addr) {
		.addr_bytes = {0x25, 0x24, 0x23, 0x22, 0x21, 0x20}
	};

	tunnel->type = N3K_MGMT_TUNNEL_TYPE_MPLSOUDP;
	tunnel->l2.dst_mac = (struct rte_ether_addr) {
		.addr_bytes = {0x2b, 0x2a, 0x29, 0x28, 0x27, 0x26}
	};
	tunnel->l2.eth_type = RTE_ETHER_TYPE_IPV4;
	tunnel->l3.src_ip = RTE_IPV4(2, 2, 2, 8);
	tunnel->l3.dst_ip = RTE_IPV4(2, 2, 2, 9);
	tunnel->l3.ip_prot = IPPROTO_UDP;
	tunnel->l3.ttl = 67;
	tunnel->l3.tos = 0;
	tunnel->l4.dst_port = UDP_PORT_MPLS;

	key->use_tunnel_key = false;
	key->use_l2_key = true;
	key->l2.src_mac = (struct rte_ether_addr) {
		.addr_bytes = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25}
	};
	key->l2.dst_mac = (struct rte_ether_addr) {
		.addr_bytes = {0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b}
	};
	key->l3.type = N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4;
	key->l3.ipv4.src_ip = RTE_IPV4(2, 2, 2, 6);
	key->l3.ipv4.dst_ip = RTE_IPV4(2, 2, 2, 7);
	key->l4.src_port = src_port;
	key->l4.dst_port = dst_port;
	key->port.port_id = get_port_id_by_name(SRC_PORT_NAME);
	key->port.device_id = SRC_DEVICE_ID;
	key->l3.ip_prot = IPPROTO_UDP;

	action->port.port_id = get_port_id_by_name(DST_PORT_NAME);
	action->port.device_id = DST_DEVICE_ID;
	action->type = N3K_MGMT_FLOW_TBL_ACTION_ENCAP;
	action->encap_action.type = N3K_MGMT_TUNNEL_TYPE_MPLSOUDP;
	action->encap_action.mpls_udp.src_port = 2228;
	action->encap_action.mpls_udp.label = 12345;
	action->encap_action.mpls_udp.tos = 3;
	action->encap_action.decr_ttl = true;

	return flow;
}

static struct n3k_mgmt_flow_entry
create_encap_vxlan_flow_entry(uint16_t src_port, uint16_t dst_port)
{
	struct n3k_mgmt_flow_entry flow;
	memset(&flow, 0, sizeof(flow));
	struct rte_ether_addr *smac = &flow.action.tunnel.smac_raw;
	struct n3k_mgmt_tunnel_tbl_entry *tunnel = &flow.action.tunnel.tunnel_raw;
	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;

	*smac = (struct rte_ether_addr) {
		.addr_bytes = {0x25, 0x24, 0x23, 0x22, 0x21, 0x20}
	};

	tunnel->type = N3K_MGMT_TUNNEL_TYPE_VXLAN;
	tunnel->l2.dst_mac = (struct rte_ether_addr) {
		.addr_bytes = {0x2b, 0x2a, 0x29, 0x28, 0x27, 0x26}
	};
	tunnel->l2.eth_type = RTE_ETHER_TYPE_IPV4;
	tunnel->l3.src_ip = RTE_IPV4(2, 2, 2, 8);
	tunnel->l3.dst_ip = RTE_IPV4(2, 2, 2, 9);
	tunnel->l3.ip_prot = IPPROTO_UDP;
	tunnel->l3.ttl = 67;
	tunnel->l3.tos = 0;
	tunnel->l4.dst_port = UDP_PORT_VXLAN;
	tunnel->vxlan.vni = 123;

	key->use_tunnel_key = false;
	key->use_l2_key = true;
	key->l2.src_mac = (struct rte_ether_addr) {
		.addr_bytes = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25}
	};
	key->l2.dst_mac = (struct rte_ether_addr) {
		.addr_bytes = {0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b}
	};
	key->l3.type = N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4;
	key->l3.ipv4.src_ip = RTE_IPV4(2, 2, 2, 6);
	key->l3.ipv4.dst_ip = RTE_IPV4(2, 2, 2, 7);
	key->l4.src_port = src_port;
	key->l4.dst_port = dst_port;
	key->port.port_id = get_port_id_by_name(SRC_PORT_NAME);
	key->port.device_id = SRC_DEVICE_ID;
	key->l3.ip_prot = IPPROTO_UDP;

	action->port.port_id = get_port_id_by_name(DST_PORT_NAME);
	action->port.device_id = DST_DEVICE_ID;
	action->type = N3K_MGMT_FLOW_TBL_ACTION_ENCAP;
	action->encap_action.type = N3K_MGMT_TUNNEL_TYPE_VXLAN;
	action->encap_action.vxlan.src_port = 2221;
	action->encap_action.decr_ttl = true;

	return flow;
}

static struct n3k_mgmt_flow_entry
create_drop_flow_entry(uint16_t src_port, uint16_t dst_port)
{
	struct n3k_mgmt_flow_entry flow;
	memset(&flow, 0, sizeof(flow));
	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;

	key->use_tunnel_key = false;
	key->use_l2_key = true;
	key->l2.src_mac = (struct rte_ether_addr) {
		.addr_bytes = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35}
	};
	key->l2.dst_mac = (struct rte_ether_addr) {
		.addr_bytes = {0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b}
	};
	key->l3.type = N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4;
	key->l3.ipv4.src_ip = RTE_IPV4(3, 3, 3, 6);
	key->l3.ipv4.dst_ip = RTE_IPV4(3, 3, 3, 7);
	key->l4.src_port = src_port;
	key->l4.dst_port = dst_port;
	key->port.port_id = get_port_id_by_name(SRC_PORT_NAME);
	key->port.device_id = SRC_DEVICE_ID;
	key->l3.ip_prot = IPPROTO_UDP;

	action->type = N3K_MGMT_FLOW_TBL_ACTION_DROP;

	return flow;
}

static struct n3k_mgmt_flow_entry
create_decap_flow_entry(uint16_t src_port, uint16_t dst_port)
{
	struct n3k_mgmt_flow_entry flow;
	memset(&flow, 0, sizeof(flow));
	struct n3k_mgmt_vplkp_tbl_res *vplkp = &flow.key.vplkp_raw;
	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;

	uint32_t vplkp_key = 54321;
	vplkp->keymask = 0;
	vplkp->vport = N3K_MGMT_VPLKP_TBL_VPORT_L2PACKET;

	key->use_l2_key = true;
	key->l2.src_mac = (struct rte_ether_addr) {
		.addr_bytes = {0x40, 0x41, 0x42, 0x43, 0x44, 0x45}
	};
	key->l2.dst_mac = (struct rte_ether_addr) {
		.addr_bytes = {0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b}
	};
	key->l3.type = N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4;
	key->l3.ipv4.src_ip = RTE_IPV4(4, 4, 4, 6);
	key->l3.ipv4.dst_ip = RTE_IPV4(4, 4, 4, 7);
	key->l3.ip_prot = IPPROTO_UDP;
	key->l4.src_port = src_port;
	key->l4.dst_port = dst_port;
	key->port.port_id = get_port_id_by_name(SRC_PORT_NAME);
	key->port.device_id = SRC_DEVICE_ID;
	key->use_tunnel_key = true;
	key->tunnel.type = N3K_MGMT_TUNNEL_TYPE_MPLSOUDP;
	key->tunnel.outer_l3.type = N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4;
	key->tunnel.outer_l3.ipv4.src_ip = RTE_IPV4(4, 4, 4, 8);
	key->tunnel.outer_l3.ipv4.dst_ip = RTE_IPV4(4, 4, 4, 9);
	key->tunnel.outer_l3.ip_prot = IPPROTO_UDP;
	key->tunnel.mpls_udp.label = vplkp_key;
	key->tunnel.mpls_udp.tos = 3;

	action->type = N3K_MGMT_FLOW_TBL_ACTION_DECAP;
	action->port.port_id = get_port_id_by_name(DST_PORT_NAME);
	action->port.device_id = DST_DEVICE_ID;
	action->decap_action.type = N3K_MGMT_TUNNEL_TYPE_MPLSOUDP;

	return flow;
}

static struct n3k_mgmt_flow_entry
create_decap_vxlan_flow_entry(uint16_t src_port, uint16_t dst_port)
{
	struct n3k_mgmt_flow_entry flow;
	memset(&flow, 0, sizeof(flow));
	struct n3k_mgmt_vplkp_tbl_res *vplkp = &flow.key.vplkp_raw;
	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;

	vplkp->keymask = 0;
	vplkp->vport = N3K_MGMT_VPLKP_TBL_VPORT_L2PACKET;

	key->use_l2_key = true;
	key->l2.src_mac = (struct rte_ether_addr) {
		.addr_bytes = {0x40, 0x41, 0x42, 0x43, 0x44, 0x45}
	};
	key->l2.dst_mac = (struct rte_ether_addr) {
		.addr_bytes = {0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b}
	};
	key->l3.type = N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4;
	key->l3.ipv4.src_ip = RTE_IPV4(4, 4, 4, 6);
	key->l3.ipv4.dst_ip = RTE_IPV4(4, 4, 4, 7);
	key->l3.ip_prot = IPPROTO_UDP;
	key->l4.src_port = src_port;
	key->l4.dst_port = dst_port;
	key->port.port_id = get_port_id_by_name(SRC_PORT_NAME);
	key->port.device_id = SRC_DEVICE_ID;
	key->use_tunnel_key = true;
	key->tunnel.type = N3K_MGMT_TUNNEL_TYPE_VXLAN;
	key->tunnel.outer_l3.type = N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4;
	key->tunnel.outer_l3.ipv4.src_ip = RTE_IPV4(4, 4, 4, 8);
	key->tunnel.outer_l3.ipv4.dst_ip = RTE_IPV4(4, 4, 4, 9);
	key->tunnel.outer_l3.ip_prot = IPPROTO_UDP;
	key->tunnel.vxlan.vni = 333;

	action->type = N3K_MGMT_FLOW_TBL_ACTION_DECAP;
	action->port.port_id = get_port_id_by_name(DST_PORT_NAME);
	action->port.device_id = DST_DEVICE_ID;
	action->decap_action.type = N3K_MGMT_TUNNEL_TYPE_VXLAN;

	return flow;
}

static struct n3k_mgmt_flow_entry
create_encap_l3_flow_entry(uint16_t src_port, uint16_t dst_port)
{
	struct n3k_mgmt_flow_entry flow;
	memset(&flow, 0, sizeof(flow));
	struct rte_ether_addr *smac = &flow.action.tunnel.smac_raw;
	struct n3k_mgmt_tunnel_tbl_entry *tunnel = &flow.action.tunnel.tunnel_raw;
	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;

	*smac = (struct rte_ether_addr) {
		.addr_bytes = {0x55, 0x54, 0x53, 0x52, 0x51, 0x50}
	};

	tunnel->type = N3K_MGMT_TUNNEL_TYPE_MPLSOUDP;
	tunnel->l2.dst_mac = (struct rte_ether_addr) {
		.addr_bytes = {0x5b, 0x5a, 0x59, 0x58, 0x57, 0x56}
	};
	tunnel->l2.eth_type = RTE_ETHER_TYPE_IPV4;
	tunnel->l3.src_ip = RTE_IPV4(5, 5, 5, 8);
	tunnel->l3.dst_ip = RTE_IPV4(5, 5, 5, 9);
	tunnel->l3.ip_prot = IPPROTO_UDP;
	tunnel->l3.ttl = 67;
	tunnel->l3.tos = 0;
	tunnel->l4.dst_port = UDP_PORT_MPLS;

	key->use_tunnel_key = false;
	key->use_l2_key = true;
	key->l2.src_mac = (struct rte_ether_addr) {
		.addr_bytes = {0x50, 0x51, 0x52, 0x53, 0x54, 0x55}
	};
	key->l2.dst_mac = (struct rte_ether_addr) {
		.addr_bytes = {0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b}
	};
	key->l3.type = N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4;
	key->l3.ipv4.src_ip = RTE_IPV4(5, 5, 5, 6);
	key->l3.ipv4.dst_ip = RTE_IPV4(5, 5, 5, 7);
	key->l4.src_port = src_port;
	key->l4.dst_port = dst_port;
	key->port.port_id = get_port_id_by_name(SRC_PORT_NAME);
	key->port.device_id = SRC_DEVICE_ID;
	key->l3.ip_prot = IPPROTO_UDP;

	action->port.port_id = get_port_id_by_name(DST_PORT_NAME);
	action->port.device_id = DST_DEVICE_ID;
	action->type = N3K_MGMT_FLOW_TBL_ACTION_ENCAP;
	action->encap_action.type = N3K_MGMT_TUNNEL_TYPE_MPLSOUDP;
	action->encap_action.mpls_udp.src_port = 5558;
	action->encap_action.mpls_udp.label = 12346;
	action->encap_action.mpls_udp.tos = 3;
	action->encap_action.decr_ttl = true;
	action->encap_action.strip_l2 = true;

	return flow;
}

static struct n3k_mgmt_flow_entry
create_decap_l3_flow_entry(uint16_t src_port, uint16_t dst_port)
{
	struct n3k_mgmt_flow_entry flow;
	memset(&flow, 0, sizeof(flow));
	struct n3k_mgmt_vplkp_tbl_res *vplkp = &flow.key.vplkp_raw;
	struct n3k_mgmt_flow_tbl_key *key = &flow.key.key_raw;
	struct n3k_mgmt_flow_tbl_action *action = &flow.action.action_raw;

	uint32_t vplkp_key = 54322;
	vplkp->keymask = 0;
	vplkp->vport = N3K_MGMT_VPLKP_TBL_VPORT_L3PACKET;

	key->use_l2_key = false;
	key->l3.type = N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4;
	key->l3.ipv4.src_ip = RTE_IPV4(6, 6, 6, 6);
	key->l3.ipv4.dst_ip = RTE_IPV4(6, 6, 6, 7);
	key->l3.ip_prot = IPPROTO_UDP;
	key->l4.src_port = src_port;
	key->l4.dst_port = dst_port;
	key->port.port_id = get_port_id_by_name(SRC_PORT_NAME);
	key->port.device_id = SRC_DEVICE_ID;
	key->use_tunnel_key = true;
	key->tunnel.type = N3K_MGMT_TUNNEL_TYPE_MPLSOUDP;
	key->tunnel.outer_l3.type = N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4;
	key->tunnel.outer_l3.ipv4.src_ip = RTE_IPV4(6, 6, 6, 8);
	key->tunnel.outer_l3.ipv4.dst_ip = RTE_IPV4(6, 6, 6, 9);
	key->tunnel.outer_l3.ip_prot = IPPROTO_UDP;
	key->tunnel.mpls_udp.label = vplkp_key;
	key->tunnel.mpls_udp.tos = 3;

	struct rte_ether_addr inserted_smac = {
		.addr_bytes = {0x65, 0x64, 0x63, 0x62, 0x61, 0x60}
	};
	struct rte_ether_addr inserted_dmac = {
		.addr_bytes = {0x6b, 0x6a, 0x69, 0x68, 0x67, 0x66}
	};

	action->type = N3K_MGMT_FLOW_TBL_ACTION_DECAP;
	action->port.port_id = get_port_id_by_name(DST_PORT_NAME);
	action->port.device_id = DST_DEVICE_ID;
	action->decap_action.type = N3K_MGMT_TUNNEL_TYPE_MPLSOUDP;
	action->decap_action.insert_l2 = true;
	action->decap_action.inserted_l2.src_mac = inserted_smac;
	action->decap_action.inserted_l2.dst_mac = inserted_dmac;

	return flow;
}

enum n3k_flow_entry_type {
	N3K_FLOW_ENTRY_TYPE_LOCAL,
	N3K_FLOW_ENTRY_TYPE_LOCAL_MIRROR,
	N3K_FLOW_ENTRY_TYPE_ENCAP,
	N3K_FLOW_ENTRY_TYPE_ENCAP_VXLAN,
	N3K_FLOW_ENTRY_TYPE_DROP,
	N3K_FLOW_ENTRY_TYPE_DECAP,
	N3K_FLOW_ENTRY_TYPE_DECAP_VXLAN,
	N3K_FLOW_ENTRY_TYPE_ENCAP_L3,
	N3K_FLOW_ENTRY_TYPE_DECAP_L3,
};

struct n3k_mgmt_flow_entry_description *flow_descs;

static void
schedule_flow_add(struct n3k_mgmt_hw *hw, enum n3k_flow_entry_type flow_type,
	uint32_t flow_cnt)
{
	int ret;
	static uint16_t src_port = 0, dst_port = 0;

	create_flow_entry_t create_entry;
	switch (flow_type) {
	case N3K_FLOW_ENTRY_TYPE_LOCAL:
		create_entry = create_local_flow_entry;
		break;
	case N3K_FLOW_ENTRY_TYPE_LOCAL_MIRROR:
		create_entry = create_local_mirror_flow_entry;
		break;
	case N3K_FLOW_ENTRY_TYPE_ENCAP:
		create_entry = create_encap_flow_entry;
		break;
	case N3K_FLOW_ENTRY_TYPE_ENCAP_VXLAN:
		create_entry = create_encap_vxlan_flow_entry;
		break;
	case N3K_FLOW_ENTRY_TYPE_DROP:
		create_entry = create_drop_flow_entry;
		break;
	case N3K_FLOW_ENTRY_TYPE_DECAP:
		create_entry = create_decap_flow_entry;
		break;
	case N3K_FLOW_ENTRY_TYPE_DECAP_VXLAN:
		create_entry = create_decap_vxlan_flow_entry;
		break;
	case N3K_FLOW_ENTRY_TYPE_ENCAP_L3:
		create_entry = create_encap_l3_flow_entry;
		break;
	case N3K_FLOW_ENTRY_TYPE_DECAP_L3:
		create_entry = create_decap_l3_flow_entry;
		break;
	}

	while (flow_cnt--) {
		struct n3k_mgmt_flr_command *cmd;
		struct n3k_mgmt_flow_tbl_handle *handle;
		struct n3k_mgmt_flow_entry flow;

		cmd = n3k_mgmt_flr_command_alloc(hw, N3K_MGMT_FLR_COMMAND_FLOW_CREATE);
		if (cmd == NULL) {
			rte_exit(EXIT_FAILURE,
				"n3k_mgmt_flr_command_alloc returned NULL\n");
		}

		cmd->flow_create.flow = flow = create_entry(src_port, dst_port);

		ret = n3k_mgmt_flr_flow_schedule_add(hw, &cmd->flow_create, &handle);
		if (ret < 0) {
			rte_exit(EXIT_FAILURE,
				"n3k_mgmt_flr_flow_schedule_add failed ret=%d(%s)\n",
				ret, strerror(-ret));
		}
		n3k_mgmt_flow_entry_to_description(handle, &flow,
			&flow_descs[handle->flow_id]);

		++src_port;
		if (src_port == 0)
			++dst_port;
	}
}

static void
schedule_flow_query(struct n3k_mgmt_hw *hw, uint32_t flow_id_beg,
	uint32_t flow_cnt)
{
	uint32_t flow_id_end = RTE_MIN(flow_id_beg + flow_cnt,
		n3k_mgmt_flow_tbl_get_max_flow_id(hw));
	struct n3k_mgmt_flr_command **cmds;
	uint32_t i;
	int ret;

	cmds = rte_zmalloc("n3k-flr-test-app", sizeof(*cmds) * (flow_id_end - flow_id_beg), 0);

	for (i = flow_id_beg; i < flow_id_end; ++i) {
		if (flow_descs[i].flow_handle == NULL)
			continue;
		struct n3k_mgmt_flr_command *cmd;
		cmd = n3k_mgmt_flr_command_alloc(hw, N3K_MGMT_FLR_COMMAND_FLOW_QUERY);
		if (cmd == NULL) {
			rte_exit(EXIT_FAILURE,
				"n3k_mgmt_flr_command_alloc returned NULL\n");
		}

		cmd->flow_query.handle = flow_descs[i].flow_handle;

		ret = n3k_mgmt_flr_flow_tracker_schedule_get(hw, &cmd->flow_query);
		if (ret) {
			printf("n3k_mgmt_flr_flow_tracker_schedule_get failed ret=%d(%s)\n",
				ret, strerror(-ret));
			n3k_mgmt_flr_command_free(hw, cmd);
		} else {
			cmds[i - flow_id_beg] = cmd;
		}
	}

	for (i = flow_id_beg; i < flow_id_end; ++i) {
		struct n3k_mgmt_flr_command *cmd = cmds[i - flow_id_beg];
		if (cmd == NULL)
			continue;

		while (rte_atomic16_read(&cmd->flow_query.done) == 0)
			rte_delay_ms(1);
		ret = cmd->flow_query.error_code;

		if (ret < 0) {
			printf("Failed to gen flow statistics for flow_id=%" PRIu32 "\n", i);
		} else {
			printf("flow_id=%" PRIu32 " ", i);
			rte_pmd_n3k_flow_tracker_struct_dump(&cmd->flow_query.stats);
		}
		fflush(stdout);

		n3k_mgmt_flr_command_free(hw, cmd);
	}

	rte_free(cmds);
}

static void
schedule_flow_delete(struct n3k_mgmt_hw *hw, uint32_t flow_id_beg,
	uint32_t flow_cnt)
{
	uint32_t flow_id_end = RTE_MIN(flow_id_beg + flow_cnt,
		n3k_mgmt_flow_tbl_get_max_flow_id(hw));
	uint32_t i;
	int ret;

	for (i = flow_id_beg; i < flow_id_end; ++i) {
		if (flow_descs[i].flow_handle == NULL)
			continue;

		struct n3k_mgmt_flr_command *cmd;
		cmd = n3k_mgmt_flr_command_alloc(hw, N3K_MGMT_FLR_COMMAND_FLOW_DESTROY);
		if (cmd == NULL) {
			rte_exit(EXIT_FAILURE,
				"n3k_mgmt_flr_command_alloc returned NULL\n");
		}

		cmd->flow_destroy.desc = flow_descs[i];

		ret = n3k_mgmt_flr_flow_schedule_del(hw, &cmd->flow_destroy);
		if (ret < 0) {
			printf("n3k_mgmt_flr_flow_tracker_schedule_get failed ret=%d(%s)\n",
				ret, strerror(-ret));
		}
		flow_descs[i].flow_handle = NULL;
	}
}

typedef char input_string_buffer[32];
#define IN_STR_BUFF_FMT "%31s"

static int
parse_flow_type(const char *flow_type_str, enum n3k_flow_entry_type *flow_type)
{
	if (strcmp(flow_type_str, "local") == 0)
		return *flow_type = N3K_FLOW_ENTRY_TYPE_LOCAL, 0;
	if (strcmp(flow_type_str, "mirror") == 0)
		return *flow_type = N3K_FLOW_ENTRY_TYPE_LOCAL_MIRROR, 0;
	if (strcmp(flow_type_str, "encap") == 0)
		return *flow_type = N3K_FLOW_ENTRY_TYPE_ENCAP, 0;
	if (strcmp(flow_type_str, "encap_vxlan") == 0)
		return *flow_type = N3K_FLOW_ENTRY_TYPE_ENCAP_VXLAN, 0;
	if (strcmp(flow_type_str, "drop") == 0)
		return *flow_type = N3K_FLOW_ENTRY_TYPE_DROP, 0;
	if (strcmp(flow_type_str, "decap") == 0)
		return *flow_type = N3K_FLOW_ENTRY_TYPE_DECAP, 0;
	if (strcmp(flow_type_str, "decap_vxlan") == 0)
		return *flow_type = N3K_FLOW_ENTRY_TYPE_DECAP_VXLAN, 0;
	if (strcmp(flow_type_str, "encap_l3") == 0)
		return *flow_type = N3K_FLOW_ENTRY_TYPE_ENCAP_L3, 0;
	if (strcmp(flow_type_str, "decap_l3") == 0)
		return *flow_type = N3K_FLOW_ENTRY_TYPE_DECAP_L3, 0;
	return -EINVAL;
}

__rte_unused static void
read_cmdline(struct n3k_mgmt_hw *hw)
{
	while (1) {
		input_string_buffer command_type_str;
		int ret;

		ret = scanf(IN_STR_BUFF_FMT, command_type_str);
		if (ret == EOF)
			break;
		if (ret != 1)
			continue;

		if (strcmp(command_type_str, "add") == 0) {
			input_string_buffer flow_type_str;
			uint32_t flow_cnt;
			enum n3k_flow_entry_type flow_type;

			ret = scanf(IN_STR_BUFF_FMT "%" SCNu32,
				flow_type_str, &flow_cnt);
			if (ret == EOF)
				break;
			if (ret != 2)
				continue;
			ret = parse_flow_type(flow_type_str, &flow_type);
			if (ret < 0) {
				printf("Unknown flow type %s\n", flow_type_str);
				fflush(stdout);
				continue;
			}
			schedule_flow_add(hw, flow_type, flow_cnt);
		} else if (strcmp(command_type_str, "query") == 0) {
			uint32_t flow_id_beg, flow_cnt;
			ret = scanf("%" SCNu32 "%" SCNu32, &flow_id_beg, &flow_cnt);
			if (ret == EOF)
				break;
			if (ret != 2)
				continue;
			schedule_flow_query(hw, flow_id_beg, flow_cnt);
		} else if (strcmp(command_type_str, "del") == 0) {
			uint32_t flow_id_beg, flow_cnt;
			ret = scanf("%" SCNu32 "%" SCNu32, &flow_id_beg, &flow_cnt);
			if (ret == EOF)
				break;
			if (ret != 2)
				continue;
			schedule_flow_delete(hw, flow_id_beg, flow_cnt);
		} else {
			printf("Unknown command type %s\n", command_type_str);
			fflush(stdout);
		}
	}
}

/*
 * The main function, which does initialization and calls the per-lcore
 * functions.
 */
int
main(int argc, char *argv[])
{
	struct rte_mempool *mbuf_pool;
	unsigned nb_ports;
	uint16_t portid;

	/* Initialize the Environment Abstraction Layer (EAL). */
	int ret = rte_eal_init(argc, argv);
	if (ret < 0)
		rte_exit(EXIT_FAILURE, "Error with EAL initialization\n");

	argc -= ret;
	argv += ret;

	/* Creates a new mempool in memory to hold the mbufs. */
	nb_ports = rte_eth_dev_count_avail();
	mbuf_pool = rte_pktmbuf_pool_create("MBUF_POOL", NUM_MBUFS * nb_ports,
		MBUF_CACHE_SIZE, 0, RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id());

	if (mbuf_pool == NULL)
		rte_exit(EXIT_FAILURE, "Cannot create mbuf pool\n");

	/* Initialize all ports. */
	RTE_ETH_FOREACH_DEV(portid)
		if (port_init(portid, mbuf_pool) != 0)
			rte_exit(EXIT_FAILURE, "Cannot init port %"PRIu16 "\n",
					portid);

	if (rte_lcore_count() > 1)
		printf("\nWARNING: Too many lcores enabled. Only 1 used.\n");

	if (!n3k_mgmt_is_valid_n3k_mgmt(&rte_eth_devices[0]))
		rte_exit(EXIT_FAILURE, "Failed to init mgmt device.\n");

	enum n3k_mgmt_hw_insert_type insert_type = n3k_mgmt_get_hw_from_dev_private(&rte_eth_devices[0])->insert_type;
	if (insert_type != N3K_MGMT_HW_INSERT_TYPE_FLR && insert_type != N3K_MGMT_HW_INSERT_TYPE_FLR_SIM)
		rte_exit(EXIT_FAILURE, "Invalid insert type specified.\n");

	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(&rte_eth_devices[0]);
	if (hw == NULL)
		rte_exit(EXIT_FAILURE, "Invalid device.\n");

	flow_descs = rte_zmalloc("n3k-flr-test-app", sizeof(*flow_descs) * n3k_mgmt_flow_tbl_get_max_flow_id(hw), 0);
	read_cmdline(hw);
	rte_free(flow_descs);

	return 0;
}
