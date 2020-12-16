/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_VDEV_IFC_H_
#define _PMD_NET_N3K_VDEV_IFC_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <rte_mbuf.h>

#include "../mgmt/n3k_mgmt_flr_pkts.h"
#include "../n3k.h"
#include "n3k_vdev_device.h"

struct n3k_vdev_ifc_header {
	uint16_t eth_type;
	uint16_t port_id;
	uint16_t queue_id;
	uint16_t mbl_entry;
	uint16_t cookie;
	uint16_t meta;
	uint8_t code;
	uint8_t epoch;
	uint8_t reserved[6];
};

enum { N3K_VDEV_ETH_HEADER_SIZE = 14 };
enum {
	N3K_VDEV_ETH_TYPE_SIZE =
		sizeof(((struct n3k_vdev_ifc_header *)NULL)->eth_type)
};

enum { N3K_VDEV_IFC_HEADER_SIZE = sizeof(struct n3k_vdev_ifc_header) };
enum {
	N3K_VDEV_IFC_HEADER_OFFSET =
		N3K_VDEV_ETH_HEADER_SIZE - N3K_VDEV_ETH_TYPE_SIZE
};

enum { N3K_VDEV_IFC_ETH_TYPE = 0x8770 };
enum { N3K_VDEV_IFC_PHY_PORT_ID_BASE = 101 };
enum { N3K_VDEV_IFC_VF_PORT_ID_BASE = 128 };

enum { N3K_VDEV_MBUF_DEFAULT_DATAROOM = 10000 };

enum n3k_vdev_ifc_packet_type {
	N3K_VDEV_PKT_TYPE_INVALID,
	N3K_VDEV_PKT_TYPE_REGULAR,
	N3K_VDEV_PKT_TYPE_IFC,
	N3K_VDEV_PKT_TYPE_FLR,
};

static inline uint16_t
n3k_vdev_dev_to_ifc_port_id(const struct rte_eth_dev *dev)
{
	const struct n3k_vdev_dev_internal *dev_internal = dev->data->dev_private;
	const uint16_t port_offset = dev_internal->type == N3K_VDEV_DEV_TYPE_PHY ?
		N3K_VDEV_IFC_PHY_PORT_ID_BASE :
		N3K_VDEV_IFC_VF_PORT_ID_BASE - N3K_VF_PORT_MIN_ID;

	return dev_internal->id + port_offset;
}

static inline void
n3k_vdev_ifc_port_id_to_dev(
	uint16_t port_id, enum n3k_vdev_dev_type *dev_type, uint16_t *dev_id)
{
	if (port_id >= N3K_VDEV_IFC_VF_PORT_ID_BASE) {
		*dev_type = N3K_VDEV_DEV_TYPE_VF;
		*dev_id = port_id - N3K_VDEV_IFC_VF_PORT_ID_BASE + N3K_VF_PORT_MIN_ID;
	} else {
		*dev_type = N3K_VDEV_DEV_TYPE_PHY;
		*dev_id = port_id - N3K_VDEV_IFC_PHY_PORT_ID_BASE;
	}
}

static inline void
n3k_vdev_ifc_header_init(
	struct n3k_vdev_ifc_header *ifc_header, const struct rte_eth_dev *dev)
{
	const uint16_t port_id = n3k_vdev_dev_to_ifc_port_id(dev);

	memset(ifc_header, 0, sizeof(*ifc_header));
	ifc_header->eth_type = rte_cpu_to_be_16(N3K_VDEV_IFC_ETH_TYPE);
	ifc_header->port_id = rte_cpu_to_be_16(port_id);
	// TODO(n3k): Consistent queues across n3k and n3kflow.
	ifc_header->queue_id = rte_cpu_to_be_16(0);
}

static inline uint16_t
n3k_vdev_ifc_packet_valid_parse_port_id(const struct rte_mbuf *pkt)
{
	const struct n3k_vdev_ifc_header *ifc_header;
	char buffer[N3K_VDEV_IFC_HEADER_SIZE];

	ifc_header = rte_pktmbuf_read(
		pkt, N3K_VDEV_IFC_HEADER_OFFSET, N3K_VDEV_IFC_HEADER_SIZE, buffer);

	return rte_be_to_cpu_16(ifc_header->port_id);
}

static inline bool
n3k_vdev_ifc_is_port_id_valid(uint16_t port_id)
{
	if (port_id < N3K_VDEV_IFC_PHY_PORT_ID_BASE)
		return false;

	if (port_id < N3K_VDEV_IFC_PHY_PORT_ID_BASE + N3K_PHY_PORTS_COUNT)
		return true;

	if (port_id < N3K_VDEV_IFC_VF_PORT_ID_BASE)
		return false;

	if (port_id < N3K_VDEV_IFC_VF_PORT_ID_BASE + N3K_VF_PORTS_MAX_COUNT)
		return true;

	return false;
}


// TODO(n3k): Refactor, use consistent queues across n3k and n3kflow.
static inline uint16_t
n3k_vdev_ifc_packet_parse_queue_id(const struct rte_mbuf *pkt)
{
	const struct n3k_vdev_ifc_header *ifc_header;
	char buffer[N3K_VDEV_IFC_HEADER_SIZE] = { 0 };

	ifc_header = rte_pktmbuf_read(
		pkt, N3K_VDEV_IFC_HEADER_OFFSET, N3K_VDEV_IFC_HEADER_SIZE, buffer);

	return rte_be_to_cpu_16(ifc_header->queue_id);
}

static inline enum n3k_vdev_ifc_packet_type
n3k_vdev_ifc_packet_parse_type(const struct rte_mbuf *pkt)
{
	uint16_t port_id;
	const uint16_t *eth_type;
	char buffer[N3K_VDEV_ETH_TYPE_SIZE];

	// TODO(n3k): some refactor is needed here I think
	if (n3k_mgmt_flr_is_config_pkt(pkt))
		return N3K_VDEV_PKT_TYPE_FLR;

	if (rte_pktmbuf_pkt_len(pkt) < N3K_VDEV_ETH_HEADER_SIZE)
		return N3K_VDEV_PKT_TYPE_INVALID;

	eth_type = rte_pktmbuf_read(
		pkt, N3K_VDEV_IFC_HEADER_OFFSET, N3K_VDEV_ETH_TYPE_SIZE, buffer);

	if (rte_be_to_cpu_16(*eth_type) != N3K_VDEV_IFC_ETH_TYPE)
		return N3K_VDEV_PKT_TYPE_REGULAR;

	if (rte_pktmbuf_pkt_len(pkt) < N3K_VDEV_ETH_HEADER_SIZE + N3K_VDEV_IFC_HEADER_SIZE)
		return N3K_VDEV_PKT_TYPE_INVALID;

	/* TODO: Handle stats packets */

	port_id = n3k_vdev_ifc_packet_valid_parse_port_id(pkt);
	if (!n3k_vdev_ifc_is_port_id_valid(port_id))
		return N3K_VDEV_PKT_TYPE_INVALID;

