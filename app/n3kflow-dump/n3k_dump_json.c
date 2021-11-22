/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_dump_json.h"
#include "n3k_dump_utils.h"

#include "../../drivers/net/n3k/mgmt/n3k_mgmt_dump.h"
#include <rte_pmd_n3k_port.h>

#include <json-c/json.h>

char str_buff[128];

static const char *tunnel_type_to_str(enum n3k_mgmt_tunnel_type type)
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

static const char *vlan_type_to_str(enum n3k_mgmt_flow_tbl_vlan_tag_type type)
{
	switch (type) {
	case N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_NONE:
		return "none";
	case N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_INSERT:
		return "insert";
	case N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_MOD:
		return "modify";
	case N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_STRIP:
		return "strip";
	}
	return "unknown-type";
}

static const char *flow_tbl_l3_type_to_str(enum n3k_mgmt_flow_tbl_l3_type type)
{
	switch (type) {
	case N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4:
		return "ipv4";
	case N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6:
		return "ipv6";
	}
	return "UNKNOWN-L3-TYPE";
}

static const char *ip_prot_to_str(char *buff, uint16_t ip_prot)
{
	switch (ip_prot) {
	case IPPROTO_UDP:
		sprintf(buff, "UDP");
		break;
	case IPPROTO_TCP:
		sprintf(buff, "TCP");
		break;
	default:
		sprintf(buff, "UNKNOWN-IP-PROTOCOL(0x%" PRIx16 ")", ip_prot);
		break;
	}
	return buff;
}

static const char *vport_type_to_str(enum n3k_mgmt_vplkp_tbl_vport_type vport)
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

static void json_flow_tracker_dump_data(json_object *json_obj,
	const struct n3k_mgmt_flow_tracker_stats *ft_stats)
{
	json_object_object_add(json_obj, "bytes_cnt",
		json_object_new_int64(ft_stats->bytes_cnt));
	json_object_object_add(json_obj, "packet_cnt",
		json_object_new_int64(ft_stats->packet_cnt));
	json_object_object_add(json_obj, "lifetime",
		json_object_new_double((double)ft_stats->life_time / 10.0));
}

static void json_flow_tracker_dump(json_object *json_obj,
	const struct n3k_mgmt_flow_tracker_stats *ft_stats)
{
	json_object *json_ft = json_object_new_object();
	json_object_object_add(json_obj, "flow-stats", json_ft);

	json_flow_tracker_dump_data(json_ft, ft_stats);
}

static void json_tunnel_dump_data(
	json_object *json_obj,
	const struct n3k_mgmt_tunnel_tbl_entry *tunnel,
	const struct rte_ether_addr *smac,
	const uint16_t *src_port)
{
	json_object_object_add(
		json_obj, "type",
		json_object_new_string(tunnel_type_to_str(tunnel->type)));

	// L2
	json_object *json_flow_tunnel_l2 = json_object_new_object();
	json_object_object_add(json_obj, "eth", json_flow_tunnel_l2);
	if (smac) {
		json_object_object_add(
			json_flow_tunnel_l2, "smac",
			json_object_new_string(
				n3k_dump_convert_mac_to_str(str_buff, *smac)));
	} else {
		json_object_object_add(
			json_flow_tunnel_l2, "smac_id",
			json_object_new_int(tunnel->l2.src_phy_port_id));
	}
	json_object_object_add(
		json_flow_tunnel_l2, "dmac",
		json_object_new_string(
			n3k_dump_convert_mac_to_str(str_buff, tunnel->l2.dst_mac)));

	// L3
	json_object *json_flow_tunnel_l3 = json_object_new_object();
	// TODO(n3k): use eth type to print l3 type
	json_object_object_add(json_obj, "ipv4", json_flow_tunnel_l3);
	json_object_object_add(
		json_flow_tunnel_l3, "src",
		json_object_new_string(
			n3k_dump_convert_ip_to_str(str_buff, tunnel->l3.src_ip)));
	json_object_object_add(
		json_flow_tunnel_l3, "dst",
		json_object_new_string(
			n3k_dump_convert_ip_to_str(str_buff, tunnel->l3.dst_ip)));
	json_object_object_add(
		json_flow_tunnel_l3, "tos",
		json_object_new_int(tunnel->l3.tos));
	json_object_object_add(
		json_flow_tunnel_l3, "ttl",
		json_object_new_int(tunnel->l3.ttl));

	// L4
	json_object *json_flow_tunnel_l4 = json_object_new_object();
	json_object_object_add(
		json_obj, ip_prot_to_str(str_buff, tunnel->l3.ip_prot),
		json_flow_tunnel_l4);
	json_object_object_add(
		json_flow_tunnel_l4, "dport",
		json_object_new_int(tunnel->l4.dst_port));
	if (src_port) {
		json_object_object_add(
			json_flow_tunnel_l4, "sport",
			json_object_new_int(*src_port));
	}

	if (tunnel->type == N3K_MGMT_TUNNEL_TYPE_VXLAN) {
		json_object *json_flow_tunnel_vxlan = json_object_new_object();
		json_object_object_add(json_obj, "VXLAN", json_flow_tunnel_vxlan);
		json_object_object_add(
			json_flow_tunnel_vxlan, "vni",
			json_object_new_int(tunnel->vxlan.vni));
	}
}

static void json_flow_key_dump(
	json_object *json_obj,
	const struct n3k_mgmt_flow_tbl_key *key)
{
	json_object *json_flow_key = json_object_new_object();
	json_object_object_add(json_obj, "key", json_flow_key);

	json_object_object_add(json_flow_key, "in_port",
		json_object_new_int(key->port.port_id));
	json_object_object_add(
		json_flow_key, "in_port_name",
		json_object_new_string(
			rte_pmd_n3k_vdev_id_to_str(str_buff, key->port.device_id)));

	// L2
	if (key->use_l2_key) {
		json_object *json_flow_key_l2 = json_object_new_object();
		json_object_object_add(json_flow_key, "eth",
			json_flow_key_l2);
		json_object_object_add(
			json_flow_key_l2, "smac",
			json_object_new_string(
				n3k_dump_convert_mac_to_str(str_buff, key->l2.src_mac)));
		json_object_object_add(
			json_flow_key_l2, "dmac",
			json_object_new_string(
				n3k_dump_convert_mac_to_str(str_buff, key->l2.dst_mac)));

		if (key->l2.vlan_tci != 0) {
			json_object *json_flow_key_vlan = json_object_new_object();
			json_object_object_add(json_flow_key, "vlan",
				json_flow_key_vlan);
			json_object_object_add(
				json_flow_key_vlan, "vid",
					json_object_new_int(key->l2.vlan_tci & 0xFFF));
			json_object_object_add(
				json_flow_key_vlan, "pcp",
					json_object_new_int(key->l2.vlan_tci >> 13));
		}
	}

	// L3
	json_object *json_flow_key_l3 = json_object_new_object();
	json_object_object_add(json_flow_key,
		flow_tbl_l3_type_to_str(key->l3.type),
		json_flow_key_l3);
	switch (key->l3.type) {
	case N3K_MGMT_FLOW_TBL_L3_TYPE_IPV6:
		json_object_object_add(
			json_flow_key_l3, "src",
			json_object_new_string(n3k_mgmt_convert_ipv6_to_str(
				str_buff, key->l3.ipv6.src_ip)));

		json_object_object_add(
			json_flow_key_l3, "dst",
			json_object_new_string(n3k_mgmt_convert_ipv6_to_str(
				str_buff, key->l3.ipv6.dst_ip)));
		break;

	case N3K_MGMT_FLOW_TBL_L3_TYPE_IPV4:
		json_object_object_add(
			json_flow_key_l3, "src",
			json_object_new_string(n3k_dump_convert_ip_to_str(
				str_buff, key->l3.ipv4.src_ip)));

		json_object_object_add(
			json_flow_key_l3, "dst",
			json_object_new_string(n3k_dump_convert_ip_to_str(
				str_buff, key->l3.ipv4.dst_ip)));
		break;
	}

	// L4
	json_object *json_flow_key_l4 = json_object_new_object();
	json_object_object_add(
		json_flow_key,
		ip_prot_to_str(str_buff, key->l3.ip_prot),
		json_flow_key_l4);
	json_object_object_add(
		json_flow_key_l4, "sport",
		json_object_new_int(key->l4.src_port));
	json_object_object_add(
		json_flow_key_l4, "dport",
		json_object_new_int(key->l4.dst_port));

	// tunnel
	if (key->use_tunnel_key) {
		json_object *json_flow_key_tunnel = json_object_new_object();
		json_object *json_flow_key_tunnel_ip = json_object_new_object();
		json_object *json_flow_key_tunnel_hdr = json_object_new_object();

		json_object_object_add(json_flow_key, "tunnel",
			json_flow_key_tunnel);

		// outer L3
		json_object_object_add(json_flow_key_tunnel,
			flow_tbl_l3_type_to_str(key->tunnel.outer_l3.type),
			json_flow_key_tunnel_ip);
		json_object_object_add(
			json_flow_key_tunnel_ip, "src",
			json_object_new_string(n3k_dump_convert_ip_to_str(
				str_buff,
				key->tunnel.outer_l3.ipv4.src_ip)));
		json_object_object_add(
			json_flow_key_tunnel_ip, "dst",
			json_object_new_string(n3k_dump_convert_ip_to_str(
				str_buff,
				key->tunnel.outer_l3.ipv4.dst_ip)));

		// tunnel header
		json_object_object_add(json_flow_key_tunnel,
			tunnel_type_to_str(key->tunnel.type),
			json_flow_key_tunnel_hdr);
		switch (key->tunnel.type) {
		case N3K_MGMT_TUNNEL_TYPE_MPLSOUDP:
			json_object_object_add(json_flow_key_tunnel_hdr, "label",
				json_object_new_int(key->tunnel.mpls_udp.label));
			json_object_object_add(json_flow_key_tunnel_hdr, "tos",
				json_object_new_int(key->tunnel.mpls_udp.tos));
			break;
			// TODO(n3k): print vlpkp res
		case N3K_MGMT_TUNNEL_TYPE_MPLSOGRE:
			// TODO(n3k): implement
			// TODO(n3k): print vlpkp res
			break;
		case N3K_MGMT_TUNNEL_TYPE_VXLAN:
			json_object_object_add(json_flow_key_tunnel_hdr, "vni",
				json_object_new_int(key->tunnel.vxlan.vni));
			break;
		}
	}
}

static void json_flow_action_dump(
	json_object *json_obj,
	const struct n3k_mgmt_flow_tbl_action *action,
	const struct n3k_mgmt_tunnel_tbl_entry *tunnel,
	const struct rte_ether_addr *smac)
{

	json_object *json_flow_action = json_object_new_object();
	json_object_object_add(json_obj, "action", json_flow_action);

	switch (action->type) {
	case N3K_MGMT_FLOW_TBL_ACTION_LOCAL:
		json_object_object_add(json_flow_action, "type",
			json_object_new_string("forward"));

		json_object_object_add(
			json_flow_action, "out_port",
			json_object_new_int(action->port.port_id));
		json_object_object_add(
			json_flow_action, "out_port_name",
			json_object_new_string(
				rte_pmd_n3k_vdev_id_to_str(str_buff, action->port.device_id)));

		json_object_object_add(
			json_flow_action, "decr_ttl",
			json_object_new_boolean(action->local_action.decr_ttl));

		if (action->local_action.modify_l2) {
			json_object *json_flow_modified_l2 = json_object_new_object();
			json_object_object_add(json_flow_action, "modified_l2",
				json_flow_modified_l2);
			json_object_object_add(
				json_flow_modified_l2, "smac",
				json_object_new_string(n3k_dump_convert_mac_to_str(
					str_buff,
					action->local_action.modified_l2.src_mac)));
			json_object_object_add(
				json_flow_modified_l2, "dmac",
				json_object_new_string(n3k_dump_convert_mac_to_str(
					str_buff,
					action->local_action.modified_l2.dst_mac)));
		}
		break;
	case N3K_MGMT_FLOW_TBL_ACTION_ENCAP: {
		bool print_tunnel = tunnel && smac;

		json_object_object_add(json_flow_action, "type",
			json_object_new_string("encap"));

		json_object_object_add(
			json_flow_action, "out_port",
			json_object_new_int(action->port.port_id));
		json_object_object_add(
			json_flow_action, "out_port_name",
			json_object_new_string(
				rte_pmd_n3k_vdev_id_to_str(str_buff, action->port.device_id)));

		if (!print_tunnel) {
			json_object_object_add(
				json_flow_action, "tunnel_id",
				json_object_new_int(action->encap_action.tunnel_idx));
		}

		switch (action->encap_action.type) {
		case N3K_MGMT_TUNNEL_TYPE_MPLSOUDP: {
			json_object *json_flow_tunnel = json_object_new_object();
			if (print_tunnel) {
				json_tunnel_dump_data(json_flow_action,
					tunnel,
					smac,
					&action->encap_action.mpls_udp.src_port);
			}
			json_object_object_add(
				json_flow_tunnel, "label",
				json_object_new_int(action->encap_action.mpls_udp.label));
			json_object_object_add(
				json_flow_tunnel, "tos",
				json_object_new_int(action->encap_action.mpls_udp.tos));
			if (!print_tunnel) {
				json_object_object_add(
					json_flow_tunnel, "sport",
					json_object_new_int(
						action->encap_action.mpls_udp.src_port));
			}
			json_object_object_add(
				json_flow_action,
				tunnel_type_to_str(action->encap_action.type),
				json_flow_tunnel);
			}
			break;
		case N3K_MGMT_TUNNEL_TYPE_MPLSOGRE:
			// TODO(n3k): implement
			break;
		case N3K_MGMT_TUNNEL_TYPE_VXLAN:
			if (print_tunnel) {
				json_tunnel_dump_data(json_flow_action,
					tunnel,
					smac,
					&action->encap_action.vxlan.src_port);
			} else {
				json_object *json_flow_tunnel = json_object_new_object();
				json_object_object_add(
					json_flow_action,
					tunnel_type_to_str(action->encap_action.type),
					json_flow_tunnel);
				json_object_object_add(
					json_flow_tunnel, "sport",
					json_object_new_int(
						action->encap_action.vxlan.src_port));
			}
			break;
		}

		json_object_object_add(
			json_flow_action, "decr_ttl",
			json_object_new_boolean(
				action->encap_action.decr_ttl));
		json_object_object_add(
			json_flow_action, "strip_l2",
			json_object_new_boolean(
				action->encap_action.strip_l2));
		break;
	}
	case N3K_MGMT_FLOW_TBL_ACTION_DECAP:
		json_object_object_add(json_flow_action, "type",
			json_object_new_string("decap"));

		json_object_object_add(
			json_flow_action, "out_port",
			json_object_new_int(action->port.port_id));
		json_object_object_add(
			json_flow_action, "out_port_name",
			json_object_new_string(
				rte_pmd_n3k_vdev_id_to_str(str_buff, action->port.device_id)));

		json_object_object_add(
			json_flow_action, "tunnel_type",
			json_object_new_string(
				tunnel_type_to_str(action->decap_action.type)));

		if (action->decap_action.insert_l2) {
			json_object *json_flow_inserted_l2 = json_object_new_object();
			json_object_object_add(json_flow_action, "inserted_l2",
				json_flow_inserted_l2);
			json_object_object_add(
				json_flow_inserted_l2, "smac",
				json_object_new_string(n3k_dump_convert_mac_to_str(
					str_buff,
					action->decap_action.inserted_l2.src_mac)));
			json_object_object_add(
				json_flow_inserted_l2, "dmac",
				json_object_new_string(n3k_dump_convert_mac_to_str(
					str_buff,
					action->decap_action.inserted_l2.dst_mac)));
		}
		break;
	case N3K_MGMT_FLOW_TBL_ACTION_DROP:
		json_object_object_add(json_flow_action, "type",
			json_object_new_string("drop"));
		break;
	}

	if (action->type != N3K_MGMT_FLOW_TBL_ACTION_DROP) {
		json_object_object_add(json_flow_action, "queue",
			json_object_new_int(action->queue));
	}

	if (action->nat_type != N3K_MGMT_FLOW_TBL_NAT_TYPE_NONE) {
		json_object *json_flow_nat = json_object_new_object();
		json_object_object_add(json_flow_action, "NAT", json_flow_nat);
		json_object_object_add(
			json_flow_nat,
			(action->nat_type == N3K_MGMT_FLOW_TBL_NAT_TYPE_SNAT) ? "src_ip" : "dst_ip",
			json_object_new_string(n3k_dump_convert_ip_to_str(
				str_buff, action->nat.modified_ip)));
	}

	if (action->vlan.type != N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_NONE) {
		json_object *json_flow_vlan = json_object_new_object();
		json_object_object_add(json_flow_action, "vlan", json_flow_vlan);
		json_object_object_add(
			json_flow_vlan,
			"type",
			json_object_new_string(vlan_type_to_str(action->vlan.type)));

		if (action->vlan.type != N3K_MGMT_FLOW_TBL_VLAN_TAG_TYPE_STRIP) {
			json_object_object_add(
				json_flow_vlan,
				"vid",
				json_object_new_int(action->vlan.tci & 0xFFF));
			json_object_object_add(
				json_flow_vlan,
				"pcp",
				json_object_new_int(action->vlan.tci >> 13));
		}
	}

	if (action->use_mirroring) {
		json_object *json_flow_mirror = json_object_new_object();
		json_object_object_add(json_flow_action, "mirror", json_flow_mirror);
		json_object_object_add(json_flow_mirror, "port",
						json_object_new_int(action->mirror.port.port_id));
		json_object_object_add(json_flow_mirror, "port_name",
					json_object_new_string(rte_pmd_n3k_vdev_id_to_str(
						str_buff, action->mirror.port.device_id)));
		json_object_object_add(json_flow_mirror, "mode",
					json_object_new_string(
						(action->mirror.mode ==
						N3K_MGMT_FLOW_TBL_MIRROR_MODE_ORIGINAL)
						? "ORIGINAL"
						: "MODIFIED"));
	}
}

static void json_port_dump_data_data(json_object *json_obj,
	const struct n3k_mgmt_port_stats *stats)
{
	json_object_object_add(json_obj, "in_packets",
		json_object_new_int64(stats->ipackets));
	json_object_object_add(json_obj, "in_bytes",
		json_object_new_int64(stats->ibytes));
	json_object_object_add(json_obj, "out_packets",
		json_object_new_int64(stats->opackets));
	json_object_object_add(json_obj, "out_bytes",
		json_object_new_int64(stats->obytes));
}

void n3k_dump_mp_json_flow_dump(
	const struct n3k_mgmt_mp_flow_response *rsp)
{
	json_object *json_flow = json_object_new_object();

	json_object_object_add(json_flow, "flow_id",
		json_object_new_int64(rsp->flow_id));

	json_flow_tracker_dump(json_flow, &rsp->ft);
	json_flow_key_dump(json_flow, &rsp->flow.key.key_raw);
	json_flow_action_dump(json_flow,
		&rsp->flow.action.action_raw,
		&rsp->flow.action.tunnel.tunnel_raw,
		&rsp->flow.action.tunnel.smac_raw);

	printf("%s", json_object_to_json_string(json_flow));
	json_object_put(json_flow);
}

void n3k_dump_mp_json_flow_raw_dump(const struct n3k_mgmt_mp_flow_raw_response *rsp)
{
	json_object *json_flow = json_object_new_object();

	json_object_object_add(json_flow, "flow_id",
		json_object_new_int64(rsp->flow_id));

	json_flow_key_dump(json_flow, &rsp->flow.key);
	json_flow_action_dump(json_flow, &rsp->flow.action, NULL, NULL);

	printf("%s", json_object_to_json_string(json_flow));
	json_object_put(json_flow);
}

void n3k_dump_mp_json_tunnel_dump(const struct n3k_mgmt_mp_tunnel_response *rsp)
{
	json_object *json_tunnel = json_object_new_object();

	json_object_object_add(json_tunnel, "tunnel_id",
		json_object_new_int(rsp->tunnel_id));

	json_object_object_add(json_tunnel, "ref_count",
		json_object_new_int(rsp->ref_count));

	json_tunnel_dump_data(json_tunnel,
		&rsp->tunnel.tunnel_raw,
		&rsp->tunnel.smac_raw,
		NULL);

	printf("%s", json_object_to_json_string(json_tunnel));
	json_object_put(json_tunnel);
}

void n3k_dump_mp_json_tunnel_raw_dump(const struct n3k_mgmt_mp_tunnel_raw_response *rsp)
{
	json_object *json_tunnel = json_object_new_object();

	json_object_object_add(json_tunnel, "tunnel_id",
		json_object_new_int(rsp->tunnel_id));

	json_object_object_add(json_tunnel, "ref_count",
		json_object_new_int(rsp->ref_count));

	json_tunnel_dump_data(json_tunnel, &rsp->tunnel, NULL, NULL);

	printf("%s", json_object_to_json_string(json_tunnel));
	json_object_put(json_tunnel);
}

void n3k_dump_mp_json_smac_dump(const struct n3k_mgmt_mp_smac_response *rsp)
{
	json_object *json_smac = json_object_new_object();

	json_object_object_add(json_smac, "port_id",
		json_object_new_int(rsp->port_id));

	json_object_object_add(json_smac, "ref_count",
		json_object_new_int(rsp->ref_count));

	json_object_object_add(
		json_smac, "smac",
		json_object_new_string(
			n3k_dump_convert_mac_to_str(str_buff, rsp->smac)));

	printf("%s", json_object_to_json_string(json_smac));
	json_object_put(json_smac);
}

void n3k_dump_mp_json_vplkp_dump(const struct n3k_mgmt_mp_vplkp_response *rsp)
{
	json_object *json_vplkp = json_object_new_object();

	json_object_object_add(json_vplkp, "label",
		json_object_new_int(rsp->label));
	json_object_object_add(json_vplkp, "keymask",
		json_object_new_int(rsp->res.keymask));
	json_object_object_add(json_vplkp, "vport",
		json_object_new_string(vport_type_to_str(rsp->res.vport)));

	printf("%s", json_object_to_json_string(json_vplkp));
	json_object_put(json_vplkp);
}

void n3k_dump_mp_json_flow_tracker_dump(
	const struct n3k_mgmt_mp_flow_tracker_response *rsp)
{
	json_object *json_ft = json_object_new_object();
	json_object_object_add(json_ft, "flow_id",
		json_object_new_int64(rsp->flow_id));
	json_flow_tracker_dump_data(json_ft, &rsp->ft);

	printf("%s", json_object_to_json_string(json_ft));
	json_object_put(json_ft);
}

void n3k_dump_mp_json_port_dump(const struct n3k_mgmt_mp_port_response *rsp)
{
	json_object *json_port = json_object_new_object();

	json_object_object_add(
		json_port, "name",
		json_object_new_string(rsp->port_name));

	json_port_dump_data_data(json_port, &rsp->stats);

	printf("%s", json_object_to_json_string(json_port));
	json_object_put(json_port);
}

void n3k_dump_mp_json_vf0_stats_dump(const struct n3k_mgmt_mp_vf0_stats_response *rsp)
{
	printf("\"slow_path\": { \"in_packets\": %" PRIu64 ", \"out_packets\": %" PRIu64
		" }, \"flr\": { \"in_packets\": %" PRIu64 ", \"out_packets\": %" PRIu64 " }",
			rsp->slow_path.rx_pkts, rsp->slow_path.tx_pkts,
			rsp->flr.tx_pkts, rsp->flr.tx_pkts);
}
