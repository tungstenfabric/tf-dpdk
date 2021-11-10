/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_HW_H_
#define _PMD_NET_N3K_MGMT_HW_H_

#include "common/n3k.h"
#include "n3k_mgmt_osdep.h"

#include <rte_ethdev.h>
#include <rte_pmd_n3k_tunnel_tbl_mgmt.h>

#include <stdbool.h>

#define N3K_MGMT_VENDOR_ID        0x1AF4
#define N3K_MGMT_DEVICE_ID        0x1041
#define N3K_MGMT_SUBSYS_VENDOR_ID 0x8086
#define N3K_MGMT_SUBSYS_DEVICE_ID 0x15FE

#define N3K_MGMT_MAX_QUEUES     1
#define VIRTIO_F_IOMMU_PLATFORM 33

/* Common configuration */
#define N3K_MGMT_PCI_CAP_COMMON_CFG 1
/* Notifications */
#define N3K_MGMT_PCI_CAP_NOTIFY_CFG 2
/* ISR Status */
#define N3K_MGMT_PCI_CAP_ISR_CFG    3
/* Device specific configuration */
#define N3K_MGMT_PCI_CAP_DEVICE_CFG 4
/* PCI configuration access */
#define N3K_MGMT_PCI_CAP_PCI_CFG    5

#define N3K_MGMT_PCI_MAX_RESOURCE   6

#define N3K_MGMT_MAX_MQS_PER_DEVICE 62

struct n3k_mgmt_args;

struct n3k_mgmt_pci_cap {
	u8 cap_vndr;            /* Generic PCI field: PCI_CAP_ID_VNDR */
	u8 cap_next;            /* Generic PCI field: next ptr. */
	u8 cap_len;             /* Generic PCI field: capability length */
	u8 cfg_type;            /* Identifies the structure. */
	u8 bar;                 /* Where to find it. */
	u8 padding[3];          /* Pad to full dword. */
	u32 offset;             /* Offset within bar. */
	u32 length;             /* Length of the structure, in bytes. */
};

struct n3k_mgmt_pci_common_cfg {
	/* About the whole device. */
	u32 device_feature_select;
	u32 device_feature;
	u32 guest_feature_select;
	u32 guest_feature;
	u16 msix_config;
	u16 num_queues;
	u8 device_status;
	u8 config_generation;

	/* About a specific virtqueue. */
	u16 queue_select;
	u16 queue_size;
	u16 queue_msix_vector;
	u16 queue_enable;
	u16 queue_notify_off;
	u32 queue_desc_lo;
	u32 queue_desc_hi;
	u32 queue_avail_lo;
	u32 queue_avail_hi;
	u32 queue_used_lo;
	u32 queue_used_hi;
};

struct n3k_mgmt_net_config {
	u8    mac[6];
	u16   status;
	u16   max_virtqueue_pairs;
} __attribute__((packed));

struct n3k_mgmt_pci_mem_resource {
	u64      phys_addr; /**< Physical address, 0 if not resource. */
	u64      len;       /**< Length of the resource. */
	u8       *addr;     /**< Virtual address, NULL when not mapped. */
};

struct vring_info {
	u64 desc;
	u64 avail;
	u64 used;
	u16 size;
	u16 last_avail_idx;
	u16 last_used_idx;
};

enum n3k_mgmt_hw_insert_type {
	N3K_MGMT_HW_INSERT_TYPE_FLR,
	N3K_MGMT_HW_INSERT_TYPE_FLR_SIM,
	N3K_MGMT_HW_INSERT_TYPE_CSR,
};

struct n3k_mgmt_lag_internal;
struct n3k_mgmt_vplkp_tbl_cache;
struct n3k_mgmt_tunnel_tbl_cache;
struct n3k_mgmt_smac_tbl_cache;
struct n3k_mgmt_flow_tbl_handles_internal;
struct n3k_mgmt_mp_internal;
struct n3k_mgmt_flow_id_allocator_internal;
struct n3k_mgmt_recirc_id_allocator_internal;
struct n3k_mgmt_handle_mapping_internal;
struct n3k_mgmt_flr_cmd_queue;
struct n3k_mgmt_flr_flow_stats_cmd_src;
struct n3k_mgmt_flr_commands_internal;
struct n3k_mgmt_flr_dispatcher_internal;
struct n3k_mgmt_flr_ack_dispatcher_internal;
struct n3k_mgmt_flr_rdy_cmd_hdlr;

struct n3k_mgmt_flr_sim_queues_mgmt_internal;
struct n3k_mgmt_flr_sim_request_handler_internal;

struct n3k_mgmt_hw {
	struct n3k_mgmt_pci_mem_resource mem_resource[N3K_MGMT_PCI_MAX_RESOURCE];

	struct n3k_mgmt_lag_internal *lag_internal;
	struct n3k_mgmt_vplkp_tbl_cache *vplkp_cache;
	struct n3k_mgmt_tunnel_tbl_cache *tunnel_cache;
	struct n3k_mgmt_smac_tbl_cache *smac_cache;

	struct n3k_mgmt_flow_tbl_handles_internal *flow_tbl_handles_internal;
	struct n3k_mgmt_mp_internal *mp_internal;
	struct n3k_mgmt_flow_id_allocator_internal *flow_id_alloc_internal;
	struct n3k_mgmt_flr_cmd_queue *flr_cmd_queue;
	struct n3k_mgmt_flr_flow_stats_cmd_src *flr_flow_stats_cmd_src;
	struct n3k_mgmt_flr_commands_internal *flr_commands_internal;
	struct n3k_mgmt_flr_dispatcher_internal *flr_dispatcher_internal;
	struct n3k_mgmt_flr_ack_dispatcher_internal *flr_ack_dispatcher_internal;
	struct n3k_mgmt_flr_rdy_cmd_hdlr *flr_rdy_cmd_hdlr;

	struct n3k_mgmt_flr_sim_queues_mgmt_internal *flr_sim_queues_mgmt_internal;
	struct n3k_mgmt_flr_sim_request_handler_internal *flr_sim_req_hdl_internal;

	struct n3k_mgmt_handle_mapping_internal *handle_map_internal;
	struct n3k_mgmt_recirc_id_allocator_internal *recirc_id_alloc_internal;

	void* pci_resource_start;

	enum n3k_mgmt_hw_insert_type insert_type;

	struct rte_eth_dev *phy_representors[N3K_PHY_PORTS_COUNT];
	struct rte_eth_dev *vf_representors[N3K_VF_PORTS_MAX_COUNT + N3K_VF_PORT_MIN_ID];

	// TODO(n3k): add vf0 private data somewhere
	struct {
		struct {
			rte_atomic64_t rx_pkts;
			rte_atomic64_t tx_pkts;
		} slow_path, flr;
	} vf0_stats;
};

int n3k_mgmt_init_hw(struct n3k_mgmt_hw *hw, struct rte_pci_device *dev,
	const struct n3k_mgmt_args *args);

#endif /* _PMD_NET_N3K_MGMT_HW_H_ */
