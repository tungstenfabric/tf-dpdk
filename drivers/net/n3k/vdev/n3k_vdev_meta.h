/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_VDEV_META_H_
#define _PMD_NET_N3K_VDEV_META_H_

#include <rte_ethdev.h>

#include "n3k_vdev_ifc.h"

struct n3k_vdev_metadata {
	union {
		struct {
			uint16_t queue_id;
			uint8_t reserved[2];
		};
		uint32_t data;
	};
};

//TODO: After moving to DPDK 19.11, metadata should be handled by mbuf dynamic field API, not through userdata
static inline void
n3k_vdev_attach_metadata_to_mbuf(struct rte_mbuf *pkt, struct n3k_vdev_metadata *meta) {
	uintptr_t m = (uintptr_t)meta->data;
	pkt->userdata = (void *)m;
}

static inline uint16_t
n3k_vdev_get_queue_id_from_metadata(uint32_t metadata) {
