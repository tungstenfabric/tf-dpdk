/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_conversions.h"
#include "n3k_mgmt_dump.h"
#include "rte_pmd_n3k_vplkp_tbl_mgmt.h"

#include <inttypes.h>

static __thread char str_buff[128];

static void
n3k_mgmt_flow_key_dump_data(
	const struct n3k_mgmt_flow_tbl_key *key,
	const struct n3k_mgmt_vplkp_tbl_res *vplkp);
static void
n3k_mgmt_flow_action_dump_data(
	const struct n3k_mgmt_flow_tbl_action *action,
	const struct n3k_mgmt_tunnel_tbl_entry *tunnel,
	const struct rte_ether_addr *smac);
static void
n3k_mgmt_tunnel_entry_dump_data(
	const struct n3k_mgmt_tunnel_tbl_entry *entry,
	const struct rte_ether_addr *smac,
	const uint16_t *src_port);

static const char *
tunnel_type_to_str(enum n3k_mgmt_tunnel_type type)
{
	switch (type) {
	case N3K_MGMT_TUNNEL_TYPE_MPLSOUDP:
		return "MPLSoUDP";
	case N3K_MGMT_TUNNEL_TYPE_MPLSOGRE:
		return "MPLSoGRE";
	case N3K_MGMT_TUNNEL_TYPE_VXLAN:
		return "VXLAN";
	}
	return "UNKNOWN-TUNNEL-TYPE";
}

static const char *
flow_tbl_l3_type_to_str(enum n3k_mgmt_flow_tbl_l3_type type)
{
	switch (type) {
	case N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4:
		return "ipv4";
	case N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6:
		return "ipv6";
	}
	return "UNKNOWN-L3-TYPE";
}

static const char *
ip_prot_to_str(uint16_t ip_prot)
{
	static __thread char ip_prot_str[32];
	switch (ip_prot) {
	case IPPROTO_UDP:
		return "UDP";
	case IPPROTO_TCP:
		return "TCP";
	}
	sprintf(ip_prot_str, "UNKNOWN-IP-PROTOCOL(0x%" PRIx16 ")", ip_prot);
	return ip_prot_str;
}

static const char *
vport_type_to_str(enum n3k_mgmt_vplkp_tbl_vport_type vport)
{
	switch (vport) {
	case N3K_MGMT_VPLKP_TBL_VPORT_SLOWPATH:
		return "SLOWPATH";
	case N3K_MGMT_VPLKP_TBL_VPORT_L2PACKET:
		return "L2";
	case N3K_MGMT_VPLKP_TBL_VPORT_L3PACKET:
		return "L3";
	}
	return "UNKNOWN-VPORT-TYPE";
}

/* Flows */

static void
n3k_mgmt_flow_key_dump_data(
	const struct n3k_mgmt_flow_tbl_key *key,
	__rte_unused const struct n3k_mgmt_vplkp_tbl_res *vplkp)
{
	char ipv6[40];

	printf("in_port(%" PRIu16 ",%s)", key->port.port_id,
		rte_pmd_n3k_vdev_id_to_str(str_buff, key->port.device_id));

	if (key->use_tunnel_key) {
		printf(",tunnel(");
		printf("%s(src=" N3K_MGMT_CONVERT_IPV4_FORMAT
			",dst=" N3K_MGMT_CONVERT_IPV4_FORMAT ")",
			flow_tbl_l3_type_to_str(key->tunnel.outer_l3.type),
			N3K_MGMT_CONVERT_IPV4_VALUE(key->tunnel.outer_l3.ipv4.src_ip),
			N3K_MGMT_CONVERT_IPV4_VALUE(key->tunnel.outer_l3.ipv4.dst_ip));
		printf(",%s(", tunnel_type_to_str(key->tunnel.type));
		switch (key->tunnel.type) {
		case N3K_MGMT_TUNNEL_TYPE_MPLSOUDP:
			printf("label=%" PRIu32 ",tos=%" PRIu32,
				key->tunnel.mpls_udp.label,
				key->tunnel.mpls_udp.tos);
			// TODO(n3k): print vlpkp res
			break;
		case N3K_MGMT_TUNNEL_TYPE_MPLSOGRE:
			// TODO(n3k): implement along with the rest of MPLSoGRE functionality
			// TODO(n3k): print vlpkp res
			break;
		case N3K_MGMT_TUNNEL_TYPE_VXLAN:
			printf("vni=%d", key->tunnel.vxlan.vni);
			break;
		}
		printf("))");
	}

	if (key->use_l2_key) {
		printf(",eth(src=" N3K_MGMT_CONVERT_MAC_FORMAT
			",dst=" N3K_MGMT_CONVERT_MAC_FORMAT ")",
			N3K_MGMT_CONVERT_MAC_VALUE(key->l2.src_mac),
			N3K_MGMT_CONVERT_MAC_VALUE(key->l2.dst_mac));
	}

	switch (key->l3.type) {
		case N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4:
			printf(",%s(src=" N3K_MGMT_CONVERT_IPV4_FORMAT
				",dst=" N3K_MGMT_CONVERT_IPV4_FORMAT ")",
				flow_tbl_l3_type_to_str(key->l3.type),
				N3K_MGMT_CONVERT_IPV4_VALUE(key->l3.ipv4.src_ip),
				N3K_MGMT_CONVERT_IPV4_VALUE(key->l3.ipv4.dst_ip));
			break;

		case N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6:
			printf(",%s(src=%s", flow_tbl_l3_type_to_str(key->l3.type),
				n3k_mgmt_convert_ipv6_to_str(ipv6, key->l3.ipv6.src_ip));
			printf(",dst=%s)", n3k_mgmt_convert_ipv6_to_str(ipv6, key->l3.ipv6.dst_ip));
			break;
	}

	printf(",%s(sport=%" PRIu16 ",dport=%" PRIu16 ")",
		ip_prot_to_str(key->l3.ip_prot),
		key->l4.src_port, key->l4.dst_port);
}

void
n3k_mgmt_flow_tbl_key_dump(const struct n3k_mgmt_flow_tbl_key *key)
{
	printf(">key: ");
	n3k_mgmt_flow_key_dump_data(key, NULL);
	printf("\n");
}

static void
n3k_mgmt_flow_action_nat_dump_data(
	const struct n3k_mgmt_flow_tbl_action *action
)
{
	printf("%s", (action->nat_type == N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT)
				? ",NAT(src_ip="
				: ",NAT(dst_ip=");
	printf(N3K_MGMT_CONVERT_IPV4_FORMAT ")",
			N3K_MGMT_CONVERT_IPV4_VALUE(action->nat.modified_ip));
}

static void
n3k_mgmt_flow_action_dump_data(
	const struct n3k_mgmt_flow_tbl_action *action,
	const struct n3k_mgmt_tunnel_tbl_entry *tunnel,
	const struct rte_ether_addr *smac)
{
	switch (action->type) {
	case N3K_MGMT_FLOW_TBL_ACTION_LOCAL:
		printf("FORWRD");

		if (action->nat_type != N3K_MGMT_FLOW_TBL_NAT_TYPE_NONE)
			n3k_mgmt_flow_action_nat_dump_data(action);

		if (action->local_action.decr_ttl)
			printf(",decr_ttl()");

		if (action->local_action.modify_l2 ) {
			printf(",set(eth(src=" N3K_MGMT_CONVERT_MAC_FORMAT
				",dst=" N3K_MGMT_CONVERT_MAC_FORMAT "))",
				N3K_MGMT_CONVERT_MAC_VALUE(
					action->local_action.modified_l2.src_mac),
				N3K_MGMT_CONVERT_MAC_VALUE(
					action->local_action.modified_l2.dst_mac));
		}

		printf(",out_port(%" PRIu16 ",%s)", action->port.port_id,
			rte_pmd_n3k_vdev_id_to_str(str_buff, action->port.device_id));

		break;
	case N3K_MGMT_FLOW_TBL_ACTION_ENCAP: {
		bool print_tunnel = tunnel && smac;

		printf("ENCAP");

		if (action->nat_type != N3K_MGMT_FLOW_TBL_NAT_TYPE_NONE)
			n3k_mgmt_flow_action_nat_dump_data(action);

		if (action->encap_action.strip_l2)
			printf(",strip_l2()");

		if (action->encap_action.decr_ttl)
			printf(",decr_ttl()");

		if (!print_tunnel) {
			printf(",tunnel(id=%" PRIu16 ",%s",
				action->encap_action.tunnel_idx,
				tunnel_type_to_str(action->encap_action.type));
		}
		switch (action->encap_action.type) {
		case N3K_MGMT_TUNNEL_TYPE_MPLSOUDP:
			if (print_tunnel) {
				printf(",tunnel(");
				n3k_mgmt_tunnel_entry_dump_data(tunnel,
					smac,
					&action->encap_action.mpls_udp.src_port);
			}
			printf("(label=%" PRIu32 ",tos=%" PRIu32,
				action->encap_action.mpls_udp.label,
				action->encap_action.mpls_udp.tos);
			if (!print_tunnel)
				printf(",sport=%" PRIu32, action->encap_action.mpls_udp.src_port);
			printf("))");
			break;
		case N3K_MGMT_TUNNEL_TYPE_MPLSOGRE:
			// TODO(n3k): implement along with the rest of MPLSoGRE functionality
			break;
		case N3K_MGMT_TUNNEL_TYPE_VXLAN:
			if (print_tunnel) {
				printf(",tunnel(");
				n3k_mgmt_tunnel_entry_dump_data(tunnel,
					smac,
					&action->encap_action.vxlan.src_port);
			} else {
				printf("(sport=%" PRIu32 ")", action->encap_action.vxlan.src_port);
			}
			printf(")");
			break;
		}

		printf(",out_port(%" PRIu16 ",%s)", action->port.port_id,
			rte_pmd_n3k_vdev_id_to_str(str_buff, action->port.device_id));

		break;
	}
	case N3K_MGMT_FLOW_TBL_ACTION_DECAP:
		printf("DECAP(%s)", tunnel_type_to_str(action->decap_action.type));

		if (action->decap_action.insert_l2) {
			printf(",insert(eth(src=" N3K_MGMT_CONVERT_MAC_FORMAT
				",dst=" N3K_MGMT_CONVERT_MAC_FORMAT "))",
				N3K_MGMT_CONVERT_MAC_VALUE(
					action->decap_action.inserted_l2.src_mac),
				N3K_MGMT_CONVERT_MAC_VALUE(
					action->decap_action.inserted_l2.dst_mac));
		}

		if (action->nat_type != N3K_MGMT_FLOW_TBL_NAT_TYPE_NONE)
			n3k_mgmt_flow_action_nat_dump_data(action);

		printf(",out_port(%" PRIu16 ",%s)", action->port.port_id,
			rte_pmd_n3k_vdev_id_to_str(str_buff, action->port.device_id));

		break;
	case N3K_MGMT_FLOW_TBL_ACTION_DROP:
		printf("DROP");
		break;
	default:
		printf("UNKNOWN-ACTION");
		break;
	}

	if (action->type != N3K_MGMT_FLOW_TBL_ACTION_DROP) {
		printf(",queue(%" PRIu16 ")", action->queue);
	}

	if (action->use_mirroring)
		printf(",mirror(%" PRIu16 ",%s,mode=%s)", action->mirror.port.port_id,
			rte_pmd_n3k_vdev_id_to_str(str_buff, action->mirror.port.device_id),
			(action->mirror.mode == N3K_MGMT_FLOW_TBL_MIRROR_MODE_ORIGINAL)
			? "ORIGINAL" : "MODIFIED");
}

void
n3k_mgmt_flow_tbl_action_dump(const struct n3k_mgmt_flow_tbl_action *action)
{
	printf(">action: ");
	n3k_mgmt_flow_action_dump_data(action, NULL, NULL);
	printf("\n");
}

void
n3k_mgmt_flow_tbl_entry_dump(const struct n3k_mgmt_flow_tbl_entry *entry)
{
	printf(">key: ");
	n3k_mgmt_flow_key_dump_data(&entry->key, NULL);
	printf("\n action: ");
	n3k_mgmt_flow_action_dump_data(&entry->action, NULL, NULL);
	printf("\n");
}

void
n3k_mgmt_flow_key_dump(const struct n3k_mgmt_flow_key *key)
{
	printf(">key: ");
	n3k_mgmt_flow_key_dump_data(&key->key_raw, &key->vplkp_raw);
	printf("\n");
}

void
n3k_mgmt_flow_action_dump(const struct n3k_mgmt_flow_action *action)
{
	printf(">action: ");
	n3k_mgmt_flow_action_dump_data(&action->action_raw,
		&action->tunnel.tunnel_raw,
		&action->tunnel.smac_raw);
	printf("\n");
}

void
n3k_mgmt_flow_entry_dump(const struct n3k_mgmt_flow_entry *entry)
{
	printf(">key: ");
	n3k_mgmt_flow_key_dump_data(&entry->key.key_raw, &entry->key.vplkp_raw);
	printf("\n action: ");
	n3k_mgmt_flow_action_dump_data(&entry->action.action_raw,
		&entry->action.tunnel.tunnel_raw,
		&entry->action.tunnel.smac_raw);
	printf("\n");
}

void
n3k_mgmt_general_flow_stats_struct_dump(
	const struct n3k_mgmt_general_flow_stats *gf_stats)
{
	printf("psg_hit_cnt=%" PRIu32 "\n", gf_stats->psg_hit_cnt);
	printf("psg_miss_cnt=%" PRIu32 "\n", gf_stats->psg_miss_cnt);
	printf("def_hit_cnt=%" PRIu32 "\n", gf_stats->def_hit_cnt);
	printf("def_miss_cnt=%" PRIu32 "\n", gf_stats->def_miss_cnt);
	printf("emlc_total_entries=%" PRIu32 "\n", gf_stats->emlc_total_entries);
}

static void
n3k_mgmt_flow_tracker_struct_dump_data(
	const struct n3k_mgmt_flow_tracker_stats *ft_stats)
{
	printf("bytes_cnt=%" PRIu64 ",packet_cnt=%" PRIu64 ",life_time=%.3fs",
		ft_stats->bytes_cnt,
		ft_stats->packet_cnt,
		(double)ft_stats->life_time / 10.0);
}

void
n3k_mgmt_flow_tracker_struct_dump(
	const struct n3k_mgmt_flow_tracker_stats *ft_stats)
{
	printf(">flow-stats: ");
	n3k_mgmt_flow_tracker_struct_dump_data(ft_stats);
	printf("\n");
}

/* Tunnels */

static void
n3k_mgmt_tunnel_entry_dump_data(
	const struct n3k_mgmt_tunnel_tbl_entry *entry,
	const struct rte_ether_addr *smac,
	const uint16_t *src_port)
{
	printf("eth(");
	if (smac)
		printf("smac=" N3K_MGMT_CONVERT_MAC_FORMAT,
			N3K_MGMT_CONVERT_MAC_VALUE(*smac));
	else
		printf("smac_id=%" PRIu8 , entry->l2.src_phy_port_id);
	printf(",dmac=" N3K_MGMT_CONVERT_MAC_FORMAT ",eth_type=0x%04" PRIx16 ")",
		N3K_MGMT_CONVERT_MAC_VALUE(entry->l2.dst_mac),
		entry->l2.eth_type);

	// TODO(n3k): use eth type to print l3 type
	printf(",ipv4(src=" N3K_MGMT_CONVERT_IPV4_FORMAT
		",dst=" N3K_MGMT_CONVERT_IPV4_FORMAT
		",tos=%" PRIu8 ",ttl=%" PRIu8 ",ip_proto=%" PRIu8 ")",
		N3K_MGMT_CONVERT_IPV4_VALUE(entry->l3.src_ip),
		N3K_MGMT_CONVERT_IPV4_VALUE(entry->l3.dst_ip),
		entry->l3.tos, entry->l3.ttl, entry->l3.ip_prot);

	printf(",%s(dport=%" PRIu16,
		ip_prot_to_str(entry->l3.ip_prot),
		entry->l4.dst_port);
	if (src_port)
		printf(",sport=%" PRIu16, *src_port);
	printf(")");


	printf(",%s", tunnel_type_to_str(entry->type));
	if (entry->type == N3K_MGMT_TUNNEL_TYPE_VXLAN) {
		printf("(vni=%" PRIu32 ")", entry->vxlan.vni);
	}
}

void
n3k_mgmt_tunnel_tbl_entry_dump(const struct n3k_mgmt_tunnel_tbl_entry *entry)
{
	printf(">tunnel: ");
	n3k_mgmt_tunnel_entry_dump_data(entry, NULL, NULL);
	printf("\n");
}

void
n3k_mgmt_tunnel_entry_dump(const struct n3k_mgmt_tunnel_entry *entry)
{
	printf(">tunnel: ");
	n3k_mgmt_tunnel_entry_dump_data(&entry->tunnel_raw, &entry->smac_raw, NULL);
	printf("\n");
}

/* Vplkps */

static void
n3k_mgmt_vplkp_tbl_res_dump_data(const struct n3k_mgmt_vplkp_tbl_res *res)
{
	printf("vport=%s,key_mask=%" PRIx32, vport_type_to_str(res->vport), res->keymask);
}

void
n3k_mgmt_vplkp_tbl_res_dump(const struct n3k_mgmt_vplkp_tbl_res *res)
{
	printf(">vplkp-res: ");
	n3k_mgmt_vplkp_tbl_res_dump_data(res);
	printf("\n");
}

/* SMACs */

void
n3k_mgmt_smac_tbl_addr_dump(const struct rte_ether_addr *smac)
{
	printf(">smac: ");
	printf(N3K_MGMT_CONVERT_MAC_FORMAT "\n", N3K_MGMT_CONVERT_MAC_VALUE(*smac));
}

/* Port */

static void
n3k_mgmt_port_stats_struct_dump_data(
	const struct n3k_mgmt_port_stats *port_stats)
{
	printf("in_packets=%" PRIu64 ",in_bytes=%" PRIu64,
		port_stats->ipackets, port_stats->ibytes);
	printf(",out_packets=%" PRIu64 ",out_bytes=%" PRIu64,
		port_stats->opackets, port_stats->obytes);
}

void
n3k_mgmt_port_stats_struct_dump(const struct n3k_mgmt_port_stats *port_stats)
{
	printf(">port-stats: ");
	n3k_mgmt_port_stats_struct_dump_data(port_stats);
	printf("\n");
}

/* MP structs */

void
n3k_mgmt_mp_flow_dump(const struct n3k_mgmt_mp_flow_response *flow)
{
	printf(">flow_id=%" PRIu32 ",", flow->flow_id);
	printf("stats(");
	n3k_mgmt_flow_tracker_struct_dump_data(&flow->ft);
	printf(")\n key: ");
	n3k_mgmt_flow_key_dump_data(&flow->flow.key.key_raw, NULL);
	printf("\n action: ");
	n3k_mgmt_flow_action_dump_data(&flow->flow.action.action_raw,
		&flow->flow.action.tunnel.tunnel_raw,
		&flow->flow.action.tunnel.smac_raw);
	printf("\n");
}

void
n3k_mgmt_mp_flow_raw_dump(const struct n3k_mgmt_mp_flow_raw_response *flow_raw)
{
	printf(">flow_id=%" PRIu32 "\n", flow_raw->flow_id);
	printf(" key: ");
	n3k_mgmt_flow_key_dump_data(&flow_raw->flow.key, NULL);
	printf("\n action: ");
	n3k_mgmt_flow_action_dump_data(&flow_raw->flow.action, NULL, NULL);
	printf("\n");
}

void
n3k_mgmt_mp_tunnel_dump(const struct n3k_mgmt_mp_tunnel_response *tunnel)
{
	printf(">tunnel: id=%" PRIu16 ",ref_count=%" PRIu32 "\n ", tunnel->tunnel_id,
		tunnel->ref_count);
	n3k_mgmt_tunnel_entry_dump_data(&tunnel->tunnel.tunnel_raw,
		&tunnel->tunnel.smac_raw, NULL);
	printf("\n");
}

void
n3k_mgmt_mp_tunnel_raw_dump(const struct n3k_mgmt_mp_tunnel_raw_response *tunnel_raw)
{
	printf(">tunnel: id=%" PRIu16 ",ref_count=%" PRIu32 "\n ",
		tunnel_raw->tunnel_id, tunnel_raw->ref_count);
	n3k_mgmt_tunnel_entry_dump_data(&tunnel_raw->tunnel, NULL, NULL);
	printf("\n");
}

void
n3k_mgmt_mp_smac_dump(const struct n3k_mgmt_mp_smac_response *smac)
{
	printf(">smac: id=%" PRIu8 ",ref_count=%" PRIu32,
		smac->port_id, smac->ref_count);
	printf(",addr=" N3K_MGMT_CONVERT_MAC_FORMAT "\n",
		N3K_MGMT_CONVERT_MAC_VALUE(smac->smac));
}

void
n3k_mgmt_mp_vplkp_dump(const struct n3k_mgmt_mp_vplkp_response *vplkp)
{
	printf(">vplkp: label=%" PRIu32 ",ref_count=%" PRIu32 ",",
		vplkp->label, vplkp->ref_count);
	n3k_mgmt_vplkp_tbl_res_dump_data(&vplkp->res);
	printf("\n");
}

void
n3k_mgmt_mp_flow_tracker_dump(
	const struct n3k_mgmt_mp_flow_tracker_response *flow_tracker)
{
	printf(">flow_id=%" PRIu32 ",", flow_tracker->flow_id);
	printf("stats(");
	n3k_mgmt_flow_tracker_struct_dump_data(&flow_tracker->ft);
	printf(")\n");
}

void
n3k_mgmt_mp_port_dump(const struct n3k_mgmt_mp_port_response *port)
{
	printf(">port: %.*s,stats(", (int)sizeof(port->port_name), port->port_name);
	n3k_mgmt_port_stats_struct_dump_data(&port->stats);
	printf(")\n");
}

void
n3k_mgmt_mp_vf0_stats_dump(const struct n3k_mgmt_mp_vf0_stats_response *stats)
{
	printf("slow_path: in_packets=%" PRIu64 ",out_packets=%" PRIu64
		"\nflr: in_packets=%" PRIu64 ",out_packets=%" PRIu64 "\n",
		stats->slow_path.rx_pkts, stats->slow_path.tx_pkts,
		stats->flr.rx_pkts, stats->flr.tx_pkts);
}

const char *
n3k_mgmt_convert_ipv6_to_str(char *buff, const uint8_t *addr)
{
	int i;
	int skipped = 0;
	char *tmp = buff;

	for (i = 0; i < 8; i++) {
		/* We won't print zeros if this is the first zeros block */
		if ((addr[2*i] == 0x00) && (addr[2*i+1] == 0) && (skipped >= 0)) {
			++skipped;
			continue;
		}

		/* If there was a zero block for a first time, add additional colon */
		if (skipped > 0) {
			*tmp++ = ':';
			skipped = -1;
		}

		/* dividing colon should go only between numbers */
		if (i != 0)
			*tmp++ = ':';

		/* If we have last two words, last word was 0xFFFF and before that only
		 * zeros, this is special case of IPv4 address inside IPv6, format it accordingly */
		if ((i == 6) && (addr[10] == 0xFF) && (addr[11] == 0xFF) && ((tmp - buff) == 7)) {
			sprintf(tmp, "%u.%u.%u.%u", addr[12], addr[13], addr[14], addr[15]);
			break;
		}

		if (addr[2*i] == 0)
			tmp += sprintf(tmp, "%x", addr[2*i+1]);
		else
			tmp += sprintf(tmp, "%x%02x", addr[2*i], addr[2*i+1]);
	}

	return buff;
}
