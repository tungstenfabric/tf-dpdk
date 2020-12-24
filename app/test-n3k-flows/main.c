/*
 * By default test-n3k-flows creates all flows with key_port=vf1 and action_port=vf2.
 * To start the app use the following command
 * ./test-n3k-flows -s 0x7 -w 'pci:0000:1d:00.0' -w 'pci:0000:1d:00.2' --vdev  * 'net_n3k0,mgmt=0000:1d:00.0,pf=0000:1d:00.2,vfs=[1,2]'
*/
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

__rte_unused static uint16_t get_port_id_by_name(const char *dev_name)
{
	struct rte_eth_dev *dev;
	dev = rte_eth_dev_allocated(dev_name);
	RTE_ASSERT(dev != NULL);
	return dev->data->port_id;
}

__rte_unused static struct n3k_mgmt_flow_entry
create_local_flow_entry(int id)
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
	key->l4.src_port = 1116 + id;
	key->l4.dst_port = 1117;
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

__rte_unused static struct n3k_mgmt_flow_entry
create_encap_flow_entry(int id)
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
	key->l4.src_port = 2226 + id;
	key->l4.dst_port = 2227;
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

__rte_unused static struct n3k_mgmt_flow_entry
create_vxlan_encap_flow_entry(int id)
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
	key->l4.src_port = 2226 + id;
	key->l4.dst_port = 2227;
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

__rte_unused static struct n3k_mgmt_flow_entry
create_vxlan_decap_flow_entry(int id)
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
	key->l4.src_port = 4446 + id;
	key->l4.dst_port = 4447;
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

__rte_unused static struct n3k_mgmt_flow_entry
create_drop_flow_entry(int id)
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
	key->l4.src_port = 3336 + id;
	key->l4.dst_port = 3337;
	key->port.port_id = get_port_id_by_name(SRC_PORT_NAME);
	key->port.device_id = SRC_DEVICE_ID;
	key->l3.ip_prot = IPPROTO_UDP;

	action->type = N3K_MGMT_FLOW_TBL_ACTION_DROP;

	return flow;
}

__rte_unused static struct n3k_mgmt_flow_entry
create_decap_flow_entry(int id)
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
	key->l4.src_port = 4446 + id;
	key->l4.dst_port = 4447;
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

__rte_unused static struct n3k_mgmt_flow_entry
create_encap_l3_flow_entry(int id)
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
	key->l4.src_port = 5556 + id;
	key->l4.dst_port = 5557;
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

__rte_unused static struct n3k_mgmt_flow_entry
create_decap_l3_flow_entry(int id)
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
	key->l4.src_port = 6666 + id;
	key->l4.dst_port = 6667;
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

__rte_unused static void
set_nat_action(struct n3k_mgmt_flow_tbl_action *action,
	enum n3k_mgmt_flow_tbl_nat_type nat_type,
	uint32_t ip)
{
	action->nat_type = nat_type;
	action->nat.modified_ip = ip;
}

// local = Ether(src="10:11:12:13:14:15", dst="16:17:18:19:1a:1b")/IP(src="1.1.1.6", dst="1.1.1.7")/UDP(sport=1116, dport=1117)
__rte_unused static struct n3k_mgmt_flow_tbl_handle *
add_local_flow(struct rte_eth_dev *dev, int id)
{
	int ret;
	struct n3k_mgmt_flow_tbl_handle *handle;
	struct n3k_mgmt_flow_entry entry;

	entry = create_local_flow_entry(id);

	ret = rte_pmd_n3k_flow_add(dev, &entry, &handle);
	if (ret < 0) {
		rte_exit(EXIT_FAILURE, "Error while adding local flow: %s(%d)\n",
			strerror(-ret), ret);
	}

	return handle;
}

// local_nat = Ether(src="10:11:12:13:14:15", dst="16:17:18:19:1a:1b")/IP(src="1.1.1.6", dst="1.1.1.7")/UDP(sport=1117, dport=1117)
__rte_unused static struct n3k_mgmt_flow_tbl_handle *
add_local_nat_flow(struct rte_eth_dev *dev, int id)
{
	int ret;
	struct n3k_mgmt_flow_tbl_handle *handle;
	struct n3k_mgmt_flow_entry entry;

	entry = create_local_flow_entry(id);

	set_nat_action(&entry.action.action_raw,
			   N3K_MGMT_FLOW_TBL_NAT_TYPE_DNAT,
			   RTE_IPV4(88, 88, 88, 88));

	ret = rte_pmd_n3k_flow_add(dev, &entry, &handle);
	if (ret < 0) {
		rte_exit(EXIT_FAILURE, "Error while adding local flow: %s(%d)\n",
			strerror(-ret), ret);
	}

	return handle;
}

// encap_nat = Ether(src="20:21:22:23:24:25", dst="26:27:28:29:2a:2b")/IP(src="2.2.2.6", dst="2.2.2.7")/UDP(sport=2227, dport=2227)
__rte_unused static struct n3k_mgmt_flow_tbl_handle *
add_encap_nat_flow(struct rte_eth_dev *dev, int id)
{
	int ret;
	struct n3k_mgmt_flow_tbl_handle *handle;
	struct n3k_mgmt_flow_entry entry;

	entry = create_encap_flow_entry(id);

	set_nat_action(&entry.action.action_raw,
			   N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT,
			   RTE_IPV4(88, 88, 88, 88));

	ret = rte_pmd_n3k_flow_add(dev, &entry, &handle);
	if (ret < 0) {
		rte_exit(EXIT_FAILURE, "Error while adding local flow: %s(%d)\n",
			strerror(-ret), ret);
	}

	return handle;
}

// decap_nat = Ether()/IP(src="4.4.4.8", dst="4.4.4.9")/UDP(dport=6635)/MPLS(label=54321, cos=3)/Ether(src="40:41:42:43:44:45", dst="46:47:48:49:4a:4b")/IP(src="4.4.4.6", dst="4.4.4.7")/UDP(sport=4447, dport=4447)
__rte_unused static struct n3k_mgmt_flow_tbl_handle *
add_decap_nat_flow(struct rte_eth_dev *dev, int id)
{
	int ret;
	struct n3k_mgmt_flow_tbl_handle *handle;
	struct n3k_mgmt_flow_entry entry;

	entry = create_decap_flow_entry(id);

	set_nat_action(&entry.action.action_raw,
			   N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT,
			   RTE_IPV4(88, 88, 88, 88));

	ret = rte_pmd_n3k_flow_add(dev, &entry, &handle);
	if (ret < 0) {
		rte_exit(EXIT_FAILURE, "Error while adding local flow: %s(%d)\n",
			strerror(-ret), ret);
	}

	return handle;
}

//local = Ether(src="10:11:12:13:14:15", dst="16:17:18:19:1a:1b")/IP(src="1.1.1.6", dst="1.1.1.7")/UDP(sport=1117, dport=1117)
__rte_unused static struct n3k_mgmt_flow_tbl_handle *add_local_mirror_flow(struct rte_eth_dev *dev, int id)
{
	int ret;
	struct n3k_mgmt_flow_tbl_handle *handle;
	struct n3k_mgmt_flow_entry entry;

	entry = create_local_flow_entry(id);

	entry.action.action_raw.use_mirroring = true;
	entry.action.action_raw.mirror.mode = N3K_MGMT_FLOW_TBL_MIRROR_MODE_ORIGINAL;
	entry.action.action_raw.mirror.port.port_id = get_port_id_by_name(DST_MIRROR_PORT_NAME);
	entry.action.action_raw.mirror.port.device_id = DST_MIRROR_DEVICE_ID;

	ret = rte_pmd_n3k_flow_add(dev, &entry, &handle);
	if (ret < 0) {
		rte_exit(EXIT_FAILURE, "Error while adding local flow: %s(%d)\n",
			strerror(-ret), ret);
	}

	return handle;
}

// encap = Ether(src="20:21:22:23:24:25", dst="26:27:28:29:2a:2b")/IP(src="2.2.2.6", dst="2.2.2.7")/UDP(sport=2226, dport=2227)
__rte_unused static struct n3k_mgmt_flow_tbl_handle *
add_encap_flow(struct rte_eth_dev *dev, int id)
{
	int ret;
	struct n3k_mgmt_flow_tbl_handle *handle;
	struct n3k_mgmt_flow_entry entry;

	entry = create_encap_flow_entry(id);

	ret = rte_pmd_n3k_flow_add(dev, &entry, &handle);
	if (ret < 0) {
		rte_exit(EXIT_FAILURE, "Error while adding local flow: %s(%d)\n",
			strerror(-ret), ret);
	}

	return handle;
}

// drop = Ether(src="30:31:32:33:34:35", dst="36:37:38:39:3a:3b")/IP(src="3.3.3.6", dst="3.3.3.7")/UDP(sport=3336, dport=3337)
__rte_unused static struct n3k_mgmt_flow_tbl_handle *
add_drop_flow(struct rte_eth_dev *dev, int id)
{
	int ret;
	struct n3k_mgmt_flow_tbl_handle *handle;
	struct n3k_mgmt_flow_entry entry;

	entry = create_drop_flow_entry(id);

	ret = rte_pmd_n3k_flow_add(dev, &entry, &handle);
	if (ret < 0) {
		rte_exit(EXIT_FAILURE, "Error while adding local flow: %s(%d)\n",
			strerror(-ret), ret);
	}

	return handle;
}

// decap = Ether()/IP(src="4.4.4.8", dst="4.4.4.9")/UDP(dport=6635)/MPLS(label=54321, cos=3)/Ether(src="40:41:42:43:44:45", dst="46:47:48:49:4a:4b")/IP(src="4.4.4.6", dst="4.4.4.7")/UDP(sport=4446, dport=4447)
__rte_unused static struct n3k_mgmt_flow_tbl_handle *
add_decap_flow(struct rte_eth_dev *dev, int id)
{
	int ret;
	struct n3k_mgmt_flow_tbl_handle *handle;
	struct n3k_mgmt_flow_entry entry;

	entry = create_decap_flow_entry(id);

	ret = rte_pmd_n3k_flow_add(dev, &entry, &handle);
	if (ret < 0) {
		rte_exit(EXIT_FAILURE, "Error while adding local flow: %s(%d)\n",
			strerror(-ret), ret);
	}

	return handle;
}

// decap = Ether()/IP(src="4.4.4.8", dst="4.4.4.9")/UDP(dport=6635)/MPLS(label=54321, cos=3)/Ether(src="40:41:42:43:44:45", dst="46:47:48:49:4a:4b")/IP(src="4.4.4.6", dst="4.4.4.7")/UDP(sport=4447, dport=4447)
__rte_unused static struct n3k_mgmt_flow_tbl_handle *add_decap_mirror_flow(struct rte_eth_dev *dev, int id)
{
	int ret;
	struct n3k_mgmt_flow_tbl_handle *handle;
	struct n3k_mgmt_flow_entry entry;

	entry = create_decap_flow_entry(id);

	entry.action.action_raw.use_mirroring = true;
	entry.action.action_raw.mirror.mode = N3K_MGMT_FLOW_TBL_MIRROR_MODE_MODIFIED;
	entry.action.action_raw.mirror.port.port_id = get_port_id_by_name(DST_MIRROR_PORT_NAME);
	entry.action.action_raw.mirror.port.device_id = DST_MIRROR_DEVICE_ID;

	ret = rte_pmd_n3k_flow_add(dev, &entry, &handle);
	if (ret < 0) {
		rte_exit(EXIT_FAILURE, "Error while adding local flow: %s(%d)\n",
			strerror(-ret), ret);
	}

	return handle;
}

// encap_l3 = Ether(src="50:51:52:53:54:55", dst="56:57:58:59:5a:5b")/IP(src="5.5.5.6", dst="5.5.5.7")/UDP(sport=5556, dport=5557)
__rte_unused static struct n3k_mgmt_flow_tbl_handle *
add_encap_l3_flow(struct rte_eth_dev *dev, int id)
{
	int ret;
	struct n3k_mgmt_flow_tbl_handle *handle;
	struct n3k_mgmt_flow_entry entry;

	entry = create_encap_l3_flow_entry(id);

	ret = rte_pmd_n3k_flow_add(dev, &entry, &handle);
	if (ret < 0) {
		rte_exit(EXIT_FAILURE, "Error while adding local flow: %s(%d)\n",
			strerror(-ret), ret);
	}

	return handle;
}

// decap_l3 = Ether()/IP(src="6.6.6.8", dst="6.6.6.9")/UDP(dport=6635)/MPLS(label=54322, cos=3)/IP(src="6.6.6.6", dst="6.6.6.7")/UDP(sport=6666, dport=6667)
__rte_unused static struct n3k_mgmt_flow_tbl_handle *
add_decap_l3_flow(struct rte_eth_dev *dev, int id)
{
	int ret;
	struct n3k_mgmt_flow_tbl_handle *handle;
	struct n3k_mgmt_flow_entry entry;

	entry = create_decap_l3_flow_entry(id);

	ret = rte_pmd_n3k_flow_add(dev, &entry, &handle);
	if (ret < 0) {
		rte_exit(EXIT_FAILURE, "Error while adding local flow: %s(%d)\n",
			strerror(-ret), ret);
	}

	return handle;
}

// encap_vxlan = Ether(src="20:21:22:23:24:25", dst="26:27:28:29:2a:2b")/IP(src="2.2.2.6", dst="2.2.2.7")/UDP(sport=2226, dport=2227)
__rte_unused static struct n3k_mgmt_flow_tbl_handle *add_encap_vxlan_flow(struct rte_eth_dev *dev, int id)
{
	int ret;
	struct n3k_mgmt_flow_tbl_handle *handle;
	struct n3k_mgmt_flow_entry entry;

	entry = create_vxlan_encap_flow_entry(id);

	ret = rte_pmd_n3k_flow_add(dev, &entry, &handle);
	if (ret < 0) {
		rte_exit(EXIT_FAILURE, "Error while adding local flow: %s(%d)\n",
			strerror(-ret), ret);
	}

	return handle;
}

// decap_vxlan = Ether()/IP(src="4.4.4.8", dst="4.4.4.9")/UDP(dport=4789)/VXLAN(vni=333)/Ether(src="40:41:42:43:44:45", dst="46:47:48:49:4a:4b")/IP(src="4.4.4.6", dst="4.4.4.7")/UDP(sport=4446, dport=4447)
__rte_unused static struct n3k_mgmt_flow_tbl_handle *add_decap_vxlan_flow(struct rte_eth_dev *dev, int id)
{
	int ret;
	struct n3k_mgmt_flow_tbl_handle *handle;
	struct n3k_mgmt_flow_entry entry;

	entry = create_vxlan_decap_flow_entry(id);

	ret = rte_pmd_n3k_flow_add(dev, &entry, &handle);
	if (ret < 0) {
		rte_exit(EXIT_FAILURE, "Error while adding local flow: %s(%d)\n",
			strerror(-ret), ret);
	}

	return handle;
}

__rte_unused static void dump_port_stats(uint16_t port_id)
{
	struct rte_eth_stats stats;
	int ret;

	ret = rte_eth_stats_get(port_id, &stats);
	if (ret < 0) {
		rte_exit(EXIT_FAILURE,
			"Failed to get port statistics for port %u. ret = %s(%d)\n",
			port_id, strerror(-ret), ret);
	}

	printf("Stats for port %u\n", port_id);
	printf("ipackets = %lu\n", stats.ipackets);
	printf("ibytes = %lu\n", stats.ibytes);
	printf("opackets = %lu\n", stats.opackets);
	printf("obytes = %lu\n", stats.obytes);
}

__rte_unused static void monitor_flows(struct rte_eth_dev *dev,
	struct n3k_mgmt_flow_tbl_handle **handles, size_t flow_cnt)
{
	uint64_t *prev_packet_cnt = calloc(flow_cnt, sizeof(*prev_packet_cnt));
	int ret;

	while (1) {
		size_t i;
		for (i = 0; i < flow_cnt; ++i) {
			struct n3k_mgmt_flow_tracker_stats cur_stats;
			ret = rte_pmd_n3k_flow_tracker_get(dev, handles[i], &cur_stats);
			if (ret < 0) {
				rte_exit(EXIT_FAILURE,
					"Failed to get statistics for flow %zu. ret = %s(%d)\n",
					i, strerror(-ret), ret);
			}
			if (cur_stats.packet_cnt != prev_packet_cnt[i]) {
				printf("Flow %zu hit.\n", i);
				rte_pmd_n3k_flow_tbl_dump(dev, handles[i]);
				rte_pmd_n3k_flow_tracker_dump(dev, handles[i]);
				printf("\n");
				prev_packet_cnt[i] = cur_stats.packet_cnt;
				dump_port_stats(get_port_id_by_name(DST_PORT_NAME));
				dump_port_stats(get_port_id_by_name(SRC_PORT_NAME));
				dump_port_stats(get_port_id_by_name(DST_MIRROR_PORT_NAME));
			}
		}
		sleep(1);
	}
	free(prev_packet_cnt);
}

__rte_unused static void test_flows(struct rte_eth_dev *dev)
{
	const size_t MAX_FLOW_CNT = 1280;
	struct n3k_mgmt_flow_tbl_handle *handles[MAX_FLOW_CNT];
	size_t flow_cnt = 0;

	handles[flow_cnt++] = add_local_flow(dev, 0);
	handles[flow_cnt++] = add_encap_l3_flow(dev, 0);
	handles[flow_cnt++] = add_decap_l3_flow(dev, 0);
	handles[flow_cnt++] = add_decap_flow(dev, 0);
	handles[flow_cnt++] = add_encap_flow(dev, 0);
	handles[flow_cnt++] = add_drop_flow(dev, 0);
	handles[flow_cnt++] = add_local_mirror_flow(dev, 1);
	handles[flow_cnt++] = add_decap_mirror_flow(dev, 1);
	handles[flow_cnt++] = add_local_nat_flow(dev, 2);
	handles[flow_cnt++] = add_encap_nat_flow(dev, 2);
	handles[flow_cnt++] = add_decap_nat_flow(dev, 2);
	handles[flow_cnt++] = add_encap_vxlan_flow(dev, 3);
	handles[flow_cnt++] = add_decap_vxlan_flow(dev, 3);

	printf("Flows added successfully\n");

	monitor_flows(dev, handles, flow_cnt);
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

	test_flows(&rte_eth_devices[0]);

	return 0;
}
