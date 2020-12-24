/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "flr_sim/n3k_mgmt_flr_sim_init.h"
#include "n3k_mgmt_init/n3k_mgmt_init_flow.h"
#include "n3k_mgmt_init/n3k_mgmt_init_mq.h"
#include "n3k_mgmt_init/n3k_mgmt_init_port.h"
#include "n3k_mgmt_args.h"
#include "n3k_mgmt_ct.h"
#include "n3k_mgmt_ethdev.h"
#include "n3k_mgmt_flow_id_allocator.h"
#include "n3k_mgmt_flow_tbl_handles.h"
#include "n3k_mgmt_flow_tracker.h"
#include "n3k_mgmt_flr_ack_dispatcher.h"
#include "n3k_mgmt_flr_cmd_queue.h"
#include "n3k_mgmt_flr_command.h"
#include "n3k_mgmt_flr_dispatcher.h"
#include "n3k_mgmt_handle_mapping.h"
#include "n3k_mgmt_init.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_nios.h"
#include "n3k_mgmt_recirc_id_allocator.h"
#include "n3k_mgmt_regs.h"
#include "n3k_mgmt_regs_ctrl.h"
#include "n3k_mgmt_smac_tbl_cache.h"
#include "n3k_mgmt_tunnel_tbl_cache.h"
#include "n3k_mgmt_vplkp_tbl.h"
#include "n3k_mgmt_vplkp_tbl_cache.h"

#include <stdbool.h>

static void set_reg_bit(struct n3k_mgmt_hw *hw, uint32_t base, uint32_t offset,
	uint8_t shift, uint8_t val)
{
	RTE_ASSERT(val <= 1);
	uint32_t pars_enable_reg;
	pars_enable_reg = csr_read(hw, base, offset);
	pars_enable_reg = (pars_enable_reg & ~(1 << shift)) | val << shift;
	csr_write(hw, base, offset, pars_enable_reg);
}

static void set_pars_enable_bit(struct n3k_mgmt_hw *hw, uint8_t shift, uint8_t val)
{
	return set_reg_bit(hw, MKS_CHIP_MKS_PARS_BASE, MKS_PARS_PARS_ENABLE_CFG, shift, val);
}

static void n3k_mgmt_pars_init_disable_all(struct n3k_mgmt_hw *hw)
{
	N3K_MGMT_LOG(INFO, "Disabling all PARS options");
	csr_write(hw, MKS_CHIP_MKS_PARS_BASE, MKS_PARS_PARS_ENABLE_CFG, 0);
}

static void n3k_mgmt_pars_init_disable_ptsubpt(struct n3k_mgmt_hw *hw)
{
	N3K_MGMT_LOG(INFO, "Disabling PARS option: PTSUBPT");
	set_pars_enable_bit(hw, MKS_PARS_PARS_ENABLE_CFG_ENABLE_PTSUBPT_SHIFT, 0);
}

static void n3k_mgmt_pars_init_enable_mpls_udp(struct n3k_mgmt_hw *hw)
{
	N3K_MGMT_LOG(INFO, "Enabling PARS option: MPLSoUDP");
	set_pars_enable_bit(hw, MKS_PARS_PARS_ENABLE_CFG_ENABLE_MPLSO_UDP_SHIFT, 1);
}

static void n3k_mgmt_pars_init_enable_mpls_gre(struct n3k_mgmt_hw *hw)
{
	N3K_MGMT_LOG(INFO, "Enabling PARS option: MPLSoGRE");
	set_pars_enable_bit(hw, MKS_PARS_PARS_ENABLE_CFG_ENABLE_MPLSO_GRE_SHIFT, 1);
}

static void n3k_mgmt_pars_init_enable_eml_bypass_for_fin_rst_tcp(struct n3k_mgmt_hw *hw)
{
	N3K_MGMT_LOG(INFO, "Enabling PARS option: Bypass EML for FIN/RST TCP packets");
	set_pars_enable_bit(hw, MKS_PARS_PARS_ENABLE_CFG_EN_BYP_EMLKUP_TCP_SHIFT, 1);
}

static void n3k_mgmt_pars_init_disable_qid(struct n3k_mgmt_hw *hw)
{
	N3K_MGMT_LOG(INFO, "Disabling PARS option: QID");
	set_pars_enable_bit(hw, MKS_PARS_PARS_ENABLE_CFG_ENABLE_QID_SHIFT, 0);
}

static void n3k_mgmt_pars_init_enable_mirroring(struct n3k_mgmt_hw *hw)
{
	N3K_MGMT_LOG(INFO, "Enabling PARS option: enableMIRROR");
	set_pars_enable_bit(hw, MKS_PARS_PARS_ENABLE_CFG_ENABLE_MIRROR_SHIFT, 1);
}

static void n3k_mgmt_pars_init_enable_newhdr(struct n3k_mgmt_hw *hw)
{
	N3K_MGMT_LOG(INFO, "Enabling PARS option: enableNEWHDR");
	set_pars_enable_bit(hw, MKS_PARS_PARS_ENABLE_CFG_ENABLE_NEWHDR_SHIFT, 1);
}

static void set_mod_stats_cfg_bit(struct n3k_mgmt_hw *hw, uint8_t shift, uint8_t val)
{
	return set_reg_bit(hw, MKS_CHIP_MKS_MOD_BASE, MKS_MOD_MOD_STATS_CFG, shift, val);
}

static void n3k_mgmt_mod_stats_reset(struct n3k_mgmt_hw *hw)
{
	MKS_MOD_MOD_STATS_CFG_t cfg = {
		.val = 0
	};

	// TODO(smartnic): Store default values somewhere?
	cfg.field.gre_version = 0x2;

	N3K_MGMT_LOG(INFO, "Resetting MOD_STATS_CFG options");
	csr_write(hw, MKS_CHIP_MKS_MOD_BASE, MKS_MOD_MOD_STATS_CFG, cfg.val);
}

static void n3k_mgmt_mod_enable_int_port_rep(struct n3k_mgmt_hw *hw)
{
	N3K_MGMT_LOG(INFO, "Enabling MOD_STATS_CFG option: intPortRepEn");
	set_mod_stats_cfg_bit(hw, MKS_MOD_MOD_STATS_CFG_INT_PORT_REP_EN_SHIFT, 1);
}

static void n3k_mgmt_lkup_set_flr_mode(struct n3k_mgmt_hw *hw, bool enable)
{
	MKS_LKUP_FLR_WRPR_CTRL_t flr_cfg = {{0}};
	flr_cfg.field.en_flr_ib_cfg_mode = enable;
	N3K_MGMT_LOG(INFO, "Enabling FLR mode: enFlrIbCfgMode");
	csr_write(hw, MKS_CHIP_MKS_LKUP_BASE, MKS_LKUP_FLR_WRPR_CFG_PROT, 0xed01);
	csr_write(hw, MKS_CHIP_MKS_LKUP_BASE, MKS_LKUP_FLR_WRPR_CTRL, flr_cfg.val);
	csr_write(hw, MKS_CHIP_MKS_LKUP_BASE, MKS_LKUP_FLR_WRPR_CFG_PROT, 0);
}

// static void set_emlc_gen_ctrl_bit(struct n3k_mgmt_hw *hw, uint8_t shift, uint8_t val)
// {
// 	return set_reg_bit(hw, MKS_CHIP_LKUP_BASE, LKUP_EMLC_GEN_CTRL, shift, val);
// }

// static void n3k_mgmt_emlc_gen_ctrl_init_enable_semi_auto_ptr(struct n3k_mgmt_hw *hw)
// {
// 	N3K_MGMT_LOG(INFO, "Enabling EMLC GEN CTRL option: enableSemiAutoPtrMode");
// 	set_emlc_gen_ctrl_bit(hw, 2, 1); // TODO(n3k): fix when new documentation available
// }

static void
n3k_mgmt_txq_fc_cfg_enable_pld_fifo_stall(struct n3k_mgmt_hw *hw)
{
	return set_reg_bit(hw, MKS_CHIP_MKS_TXQ_BASE, MKS_TXQ_TXQ_FC_CFG, 0, 1);
}

static int
flush_emlc_table(struct n3k_mgmt_hw *hw)
{
	LKUP_EMLC_MGMT_CTRL_t emlc_mgmt;
	memset(&emlc_mgmt, 0, sizeof(emlc_mgmt));
	emlc_mgmt.field.req_type = LKUP_EMLC_MGMT_CTRL_REQ_TYPE_FLUSH;
	csr_write(hw, MKS_CHIP_LKUP_BASE, LKUP_EMLC_MGMT_CTRL, emlc_mgmt.val);
	return n3k_mgmt_poll_ctrl_reg(hw, N3K_MGMT_CTRL_REG_LKUP_EMLC_MGMT_CTRL);
}

int
n3k_mgmt_init_init(struct n3k_mgmt_hw *hw, const struct n3k_mgmt_args *args)
{
	int ret;

	// TODO(n3k): refactor enabling parser options
	n3k_mgmt_pars_init_disable_all(hw);
	n3k_mgmt_pars_init_enable_mpls_udp(hw);
	n3k_mgmt_pars_init_enable_mpls_gre(hw);
	n3k_mgmt_pars_init_enable_newhdr(hw);
	n3k_mgmt_pars_init_enable_mirroring(hw);
	n3k_mgmt_pars_init_disable_qid(hw);
	n3k_mgmt_pars_init_enable_eml_bypass_for_fin_rst_tcp(hw);
	n3k_mgmt_mod_stats_reset(hw);
	n3k_mgmt_mod_enable_int_port_rep(hw);
	n3k_mgmt_txq_fc_cfg_enable_pld_fifo_stall(hw);

	n3k_mgmt_lkup_set_flr_mode(hw, false);

	n3k_mgmt_ct_enable_hw_tracking(hw);
	n3k_mgmt_pars_init_disable_ptsubpt(hw);
	n3k_mgmt_init_msk_l4_src0(hw);

	// if (hw->insert_type == N3K_MGMT_HW_INSERT_TYPE_FLR)
	// 	n3k_mgmt_emlc_gen_ctrl_init_enable_semi_auto_ptr(hw);

	/*
	 * Perform lookup and flow tracker related initialization
	 * here.
	 */
	n3k_mgmt_init_notify_init_begin(hw); // TODO(n3k): what is this

	ret = flush_emlc_table(hw);
	if (ret) {
		N3K_MGMT_LOG(ERR, "Failed to flush the emlc table");
		return -EIO;
	}

	uint32_t eml_version = n3k_mgmt_regs_get_eml_version(hw);
	uint32_t eml_core_module = eml_version & EM_LKUP_EML_VERSION_MODULE_MASK;
	if (eml_core_module != EM_LKUP_EML_VERSION_MODULE_MBL) {
		N3K_MGMT_LOG(ERR,
			"Invalid Exact Match Lookup IP version = %x. MBL required.",
			eml_version);
		return -EINVAL;
	}

	ret = n3k_mgmt_init_ft_init(hw); // TODO(n3k): move to n3k_mgmt_flow_tracker_init
	if (ret) {
		N3K_MGMT_LOG(ERR, "Error during n3k_mgmt_init_ft_init. ret = %d",
			ret);
		return -EFAULT;
	}

	n3k_mgmt_init_notify_init_done(hw);

	n3k_mgmt_init_vq_pool_init();

	// TODO(smartnic): Can this sleep be changed into busy polling?
	rte_delay_ms(500);

	// n3k_mgmt_init_write_timeout_val_to_reg(N3K_MGMT_INIT_UDP_CONNTRACK_INIT, MAX_TIMEOUT);
	// n3k_mgmt_init_write_timeout_val_to_reg(N3K_MGMT_INIT_UDP_CONNTRACK_ESTABLISHED, MAX_TIMEOUT);

	ret = n3k_mgmt_vplkp_tbl_init(hw);
	if (ret) {
		N3K_MGMT_LOG(ERR,
			"n3k_mgmt_vplkp_tbl_init failed. ret = %d",
			ret);
		return ret;
	}
	N3K_MGMT_LOG(INFO, "vportlkup table initalization succeeded");

	ret = n3k_mgmt_flow_tracker_init(hw);
	if (ret) {
		N3K_MGMT_LOG(ERR,
			"n3k_mgmt_flow_tracker_init failed. ret = %d",
			ret);
		return ret;
	}
	N3K_MGMT_LOG(INFO, "flow tracker initalization succeeded");

	ret = n3k_mgmt_flow_tbl_handles_init(hw);
	if (ret) {
		N3K_MGMT_LOG(ERR,
			"n3k_mgmt_flow_tbl_handles_init failed. ret = %d",
			ret);
		return ret;
	}
	N3K_MGMT_LOG(INFO, "flow table handles initalization succeeded");

	// TODO(n3k): remove nios code?
	// ret = n3k_mgmt_nios_handler_register_and_run(hw);
	// if (ret) {
	// 	N3K_MGMT_LOG(ERR, "n3k_mgmt_nios_handler_register_and_run failed");
	// 	return ret;
	// }
	// N3K_MGMT_LOG(INFO, "Nios initalization succeeded");

	ret = n3k_mgmt_flow_id_allocator_init(hw);
	if (ret) {
		N3K_MGMT_LOG(ERR,
			"n3k_mgmt_flow_id_allocator_init failed. ret = %d",
			ret);
		return ret;
	}
	N3K_MGMT_LOG(INFO, "flow id allocator initalization succeeded");

	ret = n3k_mgmt_recirc_id_allocator_init(hw);
	if (ret) {
		N3K_MGMT_LOG(ERR,
			"n3k_mgmt_recirc_id_allocator_init failed. ret = %d",
			ret);
		return ret;
	}
	N3K_MGMT_LOG(INFO, "recirc id allocator initalization succeeded");

	ret = n3k_mgmt_handle_mapping_init(hw);
	if (ret) {
		N3K_MGMT_LOG(ERR,
			"n3k_mgmt_handle_mapping_init failed. ret = %d",
			ret);
		return ret;
	}
	N3K_MGMT_LOG(INFO, "handle mapping initalization succeeded");

	if (hw->insert_type == N3K_MGMT_HW_INSERT_TYPE_FLR_SIM ||
		hw->insert_type == N3K_MGMT_HW_INSERT_TYPE_FLR)
	{
		ret = n3k_mgmt_flr_cmd_queue_init(hw);
		if (ret) {
			N3K_MGMT_LOG(ERR,
				"n3k_mgmt_flr_cmd_queue_init failed. ret = %d",
				ret);
			return ret;
		}
		N3K_MGMT_LOG(INFO, "FLR command queue initalization succeeded");

		ret = n3k_mgmt_flr_dispatcher_init(hw);
		if (ret) {
			N3K_MGMT_LOG(ERR,
				"n3k_mgmt_flr_dispatcher_init failed. ret = %d",
				ret);
			return ret;
		}
		N3K_MGMT_LOG(INFO, "FLR dispatcher initalization succeeded");

		ret = n3k_mgmt_flr_ack_dispatcher_init(hw);
		if (ret) {
			N3K_MGMT_LOG(ERR,
				"n3k_mgmt_flr_ack_dispatcher_init failed. ret = %d",
				ret);
			return ret;
		}
		N3K_MGMT_LOG(INFO, "FLR ACK dispatcher initalization succeeded");

		ret = n3k_mgmt_flr_commands_init(hw);
		if (ret) {
			N3K_MGMT_LOG(ERR,
				"n3k_mgmt_flr_commands_init failed. ret = %d",
				ret);
			return ret;
		}
		N3K_MGMT_LOG(INFO, "FLR commands initalization succeeded");
	}
	if (hw->insert_type == N3K_MGMT_HW_INSERT_TYPE_FLR_SIM) {
		ret = n3k_mgmt_flr_sim_init(hw, args);
		if (ret) {
			N3K_MGMT_LOG(ERR,
				"n3k_mgmt_flr_sim_init failed. ret = %d",
				ret);
			return ret;
		}
		N3K_MGMT_LOG(INFO, "Flr simulator initalization succeeded");
	}

	if (hw->insert_type == N3K_MGMT_HW_INSERT_TYPE_FLR) {
		// Enable configurations through packets
		n3k_mgmt_lkup_set_flr_mode(hw, true);
	}

	// init caches
	ret = n3k_mgmt_tunnel_tbl_cache_init(hw);
	if (ret) {
		N3K_MGMT_LOG(ERR, "n3k_mgmt_tunnel_tbl_cache_init() failed %d", ret);
		return ret;
	}

	ret = n3k_mgmt_smac_tbl_cache_init(hw);
	if (ret) {
		N3K_MGMT_LOG(ERR, "n3k_mgmt_smac_tbl_cache_init() failed %d", ret);
		return ret;
	}

	ret = n3k_mgmt_vplkp_tbl_cache_init(hw);
	if (ret) {
		N3K_MGMT_LOG(ERR, "n3k_mgmt_smac_tbl_cache_init() failed %d", ret);
		return ret;
	}

	// init multi queue
	uint16_t i;
	for (i = 0; i < args->mqs_param_count; ++i) {
		ret = n3k_mgmt_set_vf_mq(hw, i, args->vf_mqs[i]);
		if (ret) {
			// TODO: free all
			N3K_MGMT_LOG(ERR, "Setting MQ for vf%u failed", i);
			return ret;
		}
	}

	if (args->pf_mqs > 0) {
		ret = n3k_mgmt_set_pf_mq(hw, args->pf_mqs);
		// TODO: how to free those queues?
		if (ret) {
			N3K_MGMT_LOG(ERR, "Setting MQ for PF failed");
			return ret;
		}
	}

	return 0;
}
