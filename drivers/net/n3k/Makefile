# SPDX-License-Identifier: BSD-3-Clause
# Copyright(c) HCL TECHNOLOGIES LTD
# Submitted on behalf of a third-party: Intel Corporation, a
# Delaware corporation, having its principal place of business
# at 2200 Mission College Boulevard,
# Santa Clara, California 95052, USA

include $(RTE_SDK)/mk/rte.vars.mk

#
# library name
#
LIB = librte_pmd_n3k.a

CFLAGS += -DALLOW_EXPERIMENTAL_API
CFLAGS += -O3
CFLAGS += $(WERROR_FLAGS)
LDLIBS += -lrte_eal -lrte_mbuf -lrte_mempool -lrte_ring
LDLIBS += -lrte_ethdev -lrte_net -lrte_kvargs
LDLIBS += -lrte_pci -lrte_bus_pci
LDLIBS += -lrte_bus_vdev
LDLIBS += -lrte_vhost

EXPORT_MAP := rte_pmd_n3k_version.map

LIBABIVER := 1

#
# all source are stored in SRCS-y
#
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += vdev/n3k_vdev_args.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += vdev/n3k_vdev_dispatcher.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += vdev/n3k_vdev_flow.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += vdev/n3k_vdev_log.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += vdev/n3k_vdev_pmd.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += vdev/n3k_vdev_device.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += vdev/n3k_vdev_rxtx.c

SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/flr_sim/n3k_mgmt_flr_sim_dispatcher.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/flr_sim/n3k_mgmt_flr_sim_init.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/flr_sim/n3k_mgmt_flr_sim_queues_mgmt.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/flr_sim/n3k_mgmt_flr_sim_request_handler.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_args.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_conversions.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_ct.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_dump.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_ethdev.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_flow_id_allocator.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_flow_tbl_handles.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_flow_tbl.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_flow_tracker.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_flr_ack_dispatcher.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_flr_cmd_queue.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_flr_command.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_flr_dispatcher.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_flr_flow_tbl.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_flr_flow_tracker.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_flr_pkts_blocks.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_flr_pkts.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_flr_read_reg_32.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_flr_tbl.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_general_flow_stats.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_handle_mapping.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_hw.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_init.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_lock.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_log.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_mp_callbacks.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_mp.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_nios_shm.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_nios.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_pmd.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_port_stats.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_ports.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_recirc_id_allocator.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_regs_ctrl.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_regs.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_rte_flow_parse_print.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_rte_flow_parse.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_rte_flow.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_smac_tbl_cache.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_smac_tbl.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_tbl.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_tunnel_tbl_cache.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_tunnel_tbl.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_vplkp_tbl_cache.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/n3k_mgmt_vplkp_tbl.c

SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/ifc/ifc_flow.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/ifc/ifc_api_lag_callback.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/ifc/ifc_mq.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/ifc/ifc_mq_hw_access.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += mgmt/ifc/ifc_port.c

SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += rte_pmd_n3k_flow_tbl_mgmt.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += rte_pmd_n3k_malloc.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += rte_pmd_n3k_mp.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += rte_pmd_n3k_port.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += rte_pmd_n3k_smac_tbl_mgmt.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += rte_pmd_n3k_tunnel_tbl_mgmt.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += rte_pmd_n3k_vplkp_tbl_mgmt.c
SRCS-$(CONFIG_RTE_LIBRTE_N3K_PMD) += rte_pmd_n3k.c

SYMLINK-$(CONFIG_RTE_LIBRTE_N3K_PMD)-include += rte_pmd_n3k_flow_tbl_mgmt.h
SYMLINK-$(CONFIG_RTE_LIBRTE_N3K_PMD)-include += rte_pmd_n3k_mp.h
SYMLINK-$(CONFIG_RTE_LIBRTE_N3K_PMD)-include += rte_pmd_n3k_port.h
SYMLINK-$(CONFIG_RTE_LIBRTE_N3K_PMD)-include += rte_pmd_n3k_smac_tbl_mgmt.h
SYMLINK-$(CONFIG_RTE_LIBRTE_N3K_PMD)-include += rte_pmd_n3k_tunnel_tbl_mgmt.h
