// SPDX-License-Identifier: GPL-2.0
/* Copyright (c) 2018, Intel Corporation. */

#include "ifc_osdep.h"
#include "ifc_api_lag_callback.h"
#include "ifc_flow.h"
#include "ifc_port.h"

/* HW LAG Specific Constants */
/* MAC0 And MAC1 PID */
#define HW_LAG_MAC0_PID	0x65
#define HW_LAG_MAC1_PID	0x66

/* LAG Logical PID Used In Parser Block
 * Same Value Used Even In Modifier Block
 * LG PID
 */
#define HW_LAGPID_CFG 0x37    /* PID 55 */

/* LAGPID Port And SubPort Default Values */
#define HW_LAG_PID_PORT	0x0
#define HW_LAG_PID_SUBPORT 0x37

/* LAG Enable */
void lag_enable(struct n3k_mgmt_hw *hw)
{
	MKS_PARS_PARS_0_MTCHVID_CFG_t match_vid_0_cfg = {};
	MKS_PARS_PARS_1_MTCHVID_CFG_t match_vid_1_cfg = {};
	MKS_PARS_PARS_LAGVID_CFG_t lagvid_cfg = {};
	MKS_PARS_PARS_ENABLE_CFG_t pars_enable_cfg = {};
	MKS_LKUP_LKUP_CFG_REG_t lkup_cfg = {};

	/* Parser Block LAG Configuration */
	match_vid_0_cfg.field.matchvid = HW_LAG_MAC0_PID;
	csr_write(hw, MKS_CHIP_MKS_PARS_BASE, MKS_PARS_PARS_0_MTCHVID_CFG, match_vid_0_cfg.val);

	match_vid_1_cfg.field.matchvid = HW_LAG_MAC1_PID;
	csr_write(hw, MKS_CHIP_MKS_PARS_BASE, MKS_PARS_PARS_1_MTCHVID_CFG, match_vid_1_cfg.val);

	lagvid_cfg.field.lagvid = HW_LAGPID_CFG;
	csr_write(hw, MKS_CHIP_MKS_PARS_BASE, MKS_PARS_PARS_LAGVID_CFG, lagvid_cfg.val);

	/* Setting Enable LAG Filed */
	pars_enable_cfg.val = csr_read(hw, MKS_CHIP_MKS_PARS_BASE, MKS_PARS_PARS_ENABLE_CFG);

	pars_enable_cfg.field.enable_lag = 1;
	pars_enable_cfg.field.enable_ptsubpt = 0;
	csr_write(hw, MKS_CHIP_MKS_PARS_BASE, MKS_PARS_PARS_ENABLE_CFG, pars_enable_cfg.val);

	/* LKUP Block LAG Configuration */
	lkup_cfg.val = csr_read(hw, MKS_CHIP_MKS_LKUP_BASE, MKS_LKUP_LKUP_CFG_REG);

	/* cfgVidLag */
	lkup_cfg.field.cfg_vid_lag = HW_LAGPID_CFG;

	/* LAG Egress Ports Config - vLagTxPortId0/1/2/3 */
	lkup_cfg.field.v_lag_tx_port_id_0 = MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_0;
	lkup_cfg.field.v_lag_tx_port_id_1 = MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_0;
	lkup_cfg.field.v_lag_tx_port_id_2 = MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_1;
	lkup_cfg.field.v_lag_tx_port_id_3 = MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_1;

	/* cfgVidLagEn - Enabling LAG On MAC0 And MAC1 */
	lkup_cfg.field.cfg_vid_lag_en = 1;

	/* cfgVidLagSel - Select which vid to compare with cfgVidLag */
	lkup_cfg.field.cfg_vid_lag_sel = MKS_LKUP_LKUP_CFG_REG_VID_SEL_RESULT_VID;

	/* cfgLdSameSeed */
	lkup_cfg.field.cfg_ld_same_seed = 1;

	csr_write(hw, MKS_CHIP_MKS_LKUP_BASE, MKS_LKUP_LKUP_CFG_REG, lkup_cfg.val);
}

void lag_disable(struct n3k_mgmt_hw *hw)
{
	MKS_PARS_PARS_0_MTCHVID_CFG_t match_vid_0_cfg = {};
	MKS_PARS_PARS_1_MTCHVID_CFG_t match_vid_1_cfg = {};
	MKS_PARS_PARS_LAGVID_CFG_t lagvid_cfg = {};
	MKS_PARS_PARS_ENABLE_CFG_t pars_enable_cfg = {};
	MKS_LKUP_LKUP_CFG_REG_t lkup_cfg = {};

	/* Parser Block LAG Configuration */
	match_vid_0_cfg.field.matchvid = 0;
	csr_write(hw, MKS_CHIP_MKS_PARS_BASE, MKS_PARS_PARS_0_MTCHVID_CFG, match_vid_0_cfg.val);

	match_vid_1_cfg.field.matchvid = 0;
	csr_write(hw, MKS_CHIP_MKS_PARS_BASE, MKS_PARS_PARS_1_MTCHVID_CFG, match_vid_1_cfg.val);

	lagvid_cfg.field.lagvid = 0;
	csr_write(hw, MKS_CHIP_MKS_PARS_BASE, MKS_PARS_PARS_LAGVID_CFG, lagvid_cfg.val);

	/* Setting Enable LAG Filed */
	pars_enable_cfg.val = csr_read(hw, MKS_CHIP_MKS_PARS_BASE, MKS_PARS_PARS_ENABLE_CFG);

	pars_enable_cfg.field.enable_lag = 0;
	pars_enable_cfg.field.enable_ptsubpt = 0;
	csr_write(hw, MKS_CHIP_MKS_PARS_BASE, MKS_PARS_PARS_ENABLE_CFG, pars_enable_cfg.val);

	/* LKUP Block LAG Configuration */
	lkup_cfg.val = csr_read(hw, MKS_CHIP_MKS_LKUP_BASE, MKS_LKUP_LKUP_CFG_REG);

	/* cfgVidLag */
	lkup_cfg.field.cfg_vid_lag = 0;

	/* cfgVidLagEn - Disabling LAG On MAC0 And MAC1 */
	lkup_cfg.field.cfg_vid_lag_en = 0;

	/* cfgLdSameSeed */
	lkup_cfg.field.cfg_ld_same_seed = 0;

	csr_write(hw, MKS_CHIP_MKS_LKUP_BASE, MKS_LKUP_LKUP_CFG_REG, lkup_cfg.val);
}
