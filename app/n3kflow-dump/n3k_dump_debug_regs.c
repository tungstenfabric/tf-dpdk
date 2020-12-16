/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "../../drivers/net/n3k/mgmt/n3k_mgmt_regs.h"

#include <rte_common.h>

#include "n3k_dump_debug_regs.h"

const struct regs_description debug_regs[] = {
	{
		.base = MKS_CHIP_MKS_MOD_BASE,
		.offset = MKS_MOD_MOD_STATS_CFG,
		.name = "MOD_STATS_CFG",
	},
	{
		.base = MKS_CHIP_MKS_LKUP_BASE,
		.offset = MKS_LKUP_LKUP_CFG_REG,
		.name = "MKS_LKUP_LKUP_CFG_REG",
	},
	{
		.base = MKS_CHIP_MKS_LKUP_BASE,
		.offset = MKS_LKUP_LKUP_DEF_HIT_CNT,
		.name = "MKS_LKUP_LKUP_DEF_HIT_CNT",
	},
	{
		.base = MKS_CHIP_MKS_LKUP_BASE,
		.offset = MKS_LKUP_LKUP_DEF_MISS_CNT,
		.name = "MKS_LKUP_LKUP_DEF_MISS_CNT",
	},
	{
		.base = MKS_CHIP_MKS_LKUP_BASE,
		.offset = MKS_LKUP_LKUP_PSG_HIT_CNT,
		.name = "MKS_LKUP_LKUP_PSG_HIT_CNT",
	},
	{
		.base = MKS_CHIP_MKS_LKUP_BASE,
		.offset = MKS_LKUP_LKUP_PSG_MISS_CNT,
		.name = "MKS_LKUP_LKUP_PSG_MISS_CNT",
	},
	{
		.base = MKS_CHIP_MKS_LKUP_BASE,
		.offset = MKS_LKUP_LKUP_PSG_ERR_CNT,
		.name = "MKS_LKUP_LKUP_PSG_ERR_CNT",
	},
	{
		.base = MKS_CHIP_MKS_LKUP_BASE,
		.offset = MKS_LKUP_LKUP_SRCH_DROP_CNT,
		.name = "MKS_LKUP_LKUP_SRCH_DROP_CNT",
	},
	{
		.base = MKS_CHIP_MKS_LKUP_BASE,
		.offset = MKS_LKUP_LKUP_MISS_DROP_CNT,
		.name = "MKS_LKUP_LKUP_MISS_DROP_CNT",
	},
	{
		.base = MKS_CHIP_MKS_LKUP_BASE,
		.offset = MKS_LKUP_LKUP_CMD_DRP_CNT,
		.name = "MKS_LKUP_LKUP_CMD_DRP_CNT",
	},
	{
		.base = MKS_CHIP_MKS_LKUP_BASE,
		.offset = MKS_LKUP_LKUP_PARSERR_DRP_CNT,
		.name = "MKS_LKUP_LKUP_PARSERR_DRP_CNT",
	},
	{
		.base = MKS_CHIP_MKS_LKUP_BASE,
		.offset = MKS_LKUP_LKUP_PROG_SP,
		.name = "MKS_LKUP_LKUP_PROG_SP",
	},
	{
		.base = MKS_CHIP_MKS_LKUP_BASE,
		.offset = MKS_LKUP_LKUP_TTL_ERR_DRP_CNT,
		.name = "MKS_LKUP_LKUP_TTL_ERR_DRP_CNT",
	},
	{
		.base = MKS_CHIP_LKUP_BASE,
		.offset = LKUP_EMLC_TOTAL_ENTRIES,
		.name = "EMLC_TOTAL_ENTRIES",
	},
	{
		.base = MKS_CHIP_MKS_PARS_BASE,
		.offset = MKS_PARS_PARS_RX_ERR_CNT,
		.name = "MKS_PARS_PARS_RX_ERR_CNT",
	},
	{
		.base = MKS_CHIP_MKS_PARS_BASE,
		.offset = MKS_PARS_PARS_PORT0_CNT,
		.name = "MKS_PARS_PARS_PORT0_CNT",
	},
	{
		.base = MKS_CHIP_MKS_PARS_BASE,
		.offset = MKS_PARS_PARS_PORT1_CNT,
		.name = "MKS_PARS_PARS_PORT1_CNT",
	},
	{
		.base = MKS_CHIP_MKS_PARS_BASE,
		.offset = MKS_PARS_PARS_PORT2_CNT,
		.name = "MKS_PARS_PARS_PORT2_CNT",
	},
	{
		.base = MKS_CHIP_MKS_PARS_BASE,
		.offset = MKS_PARS_PARS_PORT3_CNT,
		.name = "MKS_PARS_PARS_PORT3_CNT",
	},
	{
		.base = MKS_CHIP_MKS_PARS_BASE,
		.offset = MKS_PARS_PARS_ENABLE_CFG,
		.name = "MKS_PARS_PARS_ENABLE_CFG",
	},
	{
		.base = MKS_CHIP_MKS_PARS_BASE,
		.offset = MKS_PARS_PARS_ILLGPKT_ERR_CNT,
		.name = "MKS_PARS_PARS_ILLGPKT_ERR_CNT",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_DROP_STATUS0,
		.name = "MKS_FT_FIFO_FT_FIFO_DROP_STATUS0",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_DROP_STATUS1,
		.name = "MKS_FT_FIFO_FT_FIFO_DROP_STATUS1",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_DROP_STATUS2,
		.name = "MKS_FT_FIFO_FT_FIFO_DROP_STATUS2",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_DROP_STATUS3,
		.name = "MKS_FT_FIFO_FT_FIFO_DROP_STATUS3",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_DROP_STATUS4,
		.name = "MKS_FT_FIFO_FT_FIFO_DROP_STATUS4",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_DROP_STATUS5,
		.name = "MKS_FT_FIFO_FT_FIFO_DROP_STATUS5",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_DROP_STATUS6,
		.name = "MKS_FT_FIFO_FT_FIFO_DROP_STATUS6",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_DROP_STATUS7,
		.name = "MKS_FT_FIFO_FT_FIFO_DROP_STATUS7",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_INPKT0_CNT,
		.name = "MKS_FT_FIFO_FT_FIFO_INPKT0_CNT",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_INPKT1_CNT,
		.name = "MKS_FT_FIFO_FT_FIFO_INPKT1_CNT",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_INPKT2_CNT,
		.name = "MKS_FT_FIFO_FT_FIFO_INPKT2_CNT",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_INPKT3_CNT,
		.name = "MKS_FT_FIFO_FT_FIFO_INPKT3_CNT",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_INPKT4_CNT,
		.name = "MKS_FT_FIFO_FT_FIFO_INPKT4_CNT",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_INPKT5_CNT,
		.name = "MKS_FT_FIFO_FT_FIFO_INPKT5_CNT",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_INPKT6_CNT,
		.name = "MKS_FT_FIFO_FT_FIFO_INPKT6_CNT",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_INPKT7_CNT,
		.name = "MKS_FT_FIFO_FT_FIFO_INPKT7_CNT",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_DROP_ERR0,
		.name = "MKS_FT_FIFO_FT_FIFO_DROP_ERR0",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_DROP_ERR1,
		.name = "MKS_FT_FIFO_FT_FIFO_DROP_ERR1",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_DROP_ERR2,
		.name = "MKS_FT_FIFO_FT_FIFO_DROP_ERR2",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_DROP_ERR3,
		.name = "MKS_FT_FIFO_FT_FIFO_DROP_ERR3",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_DROP_ERR4,
		.name = "MKS_FT_FIFO_FT_FIFO_DROP_ERR4",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_DROP_ERR5,
		.name = "MKS_FT_FIFO_FT_FIFO_DROP_ERR5",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_DROP_ERR6,
		.name = "MKS_FT_FIFO_FT_FIFO_DROP_ERR6",
	},
	{
		.base = MKS_CHIP_MKS_FT_FIFO_BASE,
		.offset = MKS_FT_FIFO_FT_FIFO_DROP_ERR7,
		.name = "MKS_FT_FIFO_FT_FIFO_DROP_ERR7",
	},
	// wtf
	// {
	// 	.base = MKS_CHIP_MKS_TXQ_BASE,
	// 	.offset = MKS_TXQ_TXQ_EPRT_PKTCNT_0,
	// 	.name = "MKS_TXQ_TXQ_EPRT_PKTCNT_0",
	// },
	// {
	// 	.base = MKS_CHIP_MKS_TXQ_BASE,
	// 	.offset = MKS_TXQ_TXQ_EPRT_PKTCNT_1,
	// 	.name = "MKS_TXQ_TXQ_EPRT_PKTCNT_1",
	// },
	// {
	// 	.base = MKS_CHIP_MKS_TXQ_BASE,
	// 	.offset = MKS_TXQ_TXQ_EPRT_PKTCNT_2,
	// 	.name = "MKS_TXQ_TXQ_EPRT_PKTCNT_2",
	// },
	// {
	// 	.base = MKS_CHIP_MKS_TXQ_BASE,
	// 	.offset = MKS_TXQ_TXQ_EPRT_PKTCNT_3,
	// 	.name = "MKS_TXQ_TXQ_EPRT_PKTCNT_3",
	// },
	{
		.base = MKS_CHIP_MKS_TXQ_BASE,
		.offset = MKS_TXQ_TXQ_LKUP_DROP_CNT,
		.name = "MKS_TXQ_TXQ_LKUP_DROP_CNT",
	},
	{
		.base = MKS_CHIP_MKS_TXQ_BASE,
		.offset = MKS_TXQ_TXQ_INGRFC_DROP_CNT,
		.name = "MKS_TXQ_TXQ_INGRFC_DROP_CNT",
	},
	{
		.base = MKS_CHIP_MKS_TXQ_BASE,
		.offset = MKS_TXQ_TXQ_EGRFC_DROP_CNT,
		.name = "MKS_TXQ_TXQ_EGRFC_DROP_CNT",
	},
	{
		.base = MKS_CHIP_MKS_TXSCH_BASE,
		.offset = MKS_TXSCH_TXSCH_PKT_CNT,
		.name = "MKS_TXSCH_TXSCH_PKT_CNT",
	},
	{
		.base = MKS_CHIP_MKS_MOD_BASE,
		.offset = MKS_MOD_MOD_PORT_PKT_CNT_0,
		.name = "MKS_MOD_MOD_PORT_PKT_CNT_0",
	},
	{
		.base = MKS_CHIP_MKS_MOD_BASE,
		.offset = MKS_MOD_MOD_PORT_PKT_CNT_1,
		.name = "MKS_MOD_MOD_PORT_PKT_CNT_1",
	},
	{
		.base = MKS_CHIP_MKS_MOD_BASE,
		.offset = MKS_MOD_MOD_PORT_PKT_CNT_2,
		.name = "MKS_MOD_MOD_PORT_PKT_CNT_2",
	},
	{
		.base = MKS_CHIP_MKS_MOD_BASE,
		.offset = MKS_MOD_MOD_PORT_PKT_CNT_3,
		.name = "MKS_MOD_MOD_PORT_PKT_CNT_3",
	},
	{
		.base = MKS_CHIP_MKS_VIOSHM_BASE,
		.offset = MKS_VIOSHM_VIOSHM_V2_O_PKTCNT,
		.name = "MKS_VIOSHM_VIOSHM_V2_O_PKTCNT",
	},
	{
		.base = MKS_CHIP_MKS_VIOSHM_BASE,
		.offset = MKS_VIOSHM_VIOSHM_O2_V_PKTCNT,
		.name = "MKS_VIOSHM_VIOSHM_O2_V_PKTCNT",
	},
	{
		.base = MKS_CHIP_MKS_VIOSHM_BASE,
		.offset = MKS_VIOSHM_VIOSHM_D2_V_PKTCNT,
		.name = "MKS_VIOSHM_VIOSHM_D2_V_PKTCNT",
	},
	{
		.base = MKS_CHIP_MKS_VIOSHM_BASE,
		.offset = MKS_VIOSHM_VIOSHM_V2_D_PKTCNT,
		.name = "MKS_VIOSHM_VIOSHM_V2_D_PKTCNT",
	},
	{
		.base = MKS_CHIP_MKS_VIOSHM_BASE,
		.offset = MKS_VIOSHM_VIOSHM_RX_STALL_CNT,
		.name = "MKS_VIOSHM_VIOSHM_RX_STALL_CNT",
	},
	{
		.base = MKS_CHIP_MKS_VIOSHM_BASE,
		.offset = MKS_VIOSHM_VIOSHM_TXNOTRDY_CNT,
		.name = "MKS_VIOSHM_VIOSHM_TXNOTRDY_CNT",
	},
	{
		.base = MKS_CHIP_MKS_VIOSHM_BASE,
		.offset = MKS_VIOSHM_VIOSHM_RXFC_AFULL,
		.name = "MKS_VIOSHM_VIOSHM_RXFC_AFULL",
	},
	{
		.base = MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE,
		.offset = MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_0,
		.name = "MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_0",
	},
	{
		.base = MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE,
		.offset = MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0,
		.name = "MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0",
