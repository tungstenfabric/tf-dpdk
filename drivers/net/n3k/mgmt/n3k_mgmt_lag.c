/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_ethdev.h"
#include "n3k_mgmt_init/n3k_mgmt_init_port.h"
#include "n3k_mgmt_lag.h"
#include "n3k_mgmt_log.h"
#include "regs/n3k_mgmt_regs.h"

#include <rte_alarm.h>
#include <rte_eth_bond.h>
#include <rte_eth_bond_8023ad.h>
#include <rte_pmd_n3k_malloc.h>

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

#define LACP_REFRESH 50000

enum lag_lacp_ports_state {
	LAG_LACP_DISABLED = 0,
	LAG_LACP_PHY0_ENABLED = 1 << 0,
	LAG_LACP_PHY1_ENABLED = 1 << 1,
};

struct n3k_mgmt_lag_internal {
	bool is_enabled;
	bool is_bond_port_id_set;
	uint16_t bond_port_id;
	int cur_lacp_state;
};

static const char*
lacp_state_to_str(int state)
{
	switch (state) {
	case LAG_LACP_DISABLED:
		return "DISABLED";
	case LAG_LACP_PHY0_ENABLED:
		return "PHY_0 ENABLED";
	case LAG_LACP_PHY1_ENABLED:
		return "PHY_1 ENABLED";
	case LAG_LACP_PHY0_ENABLED | LAG_LACP_PHY1_ENABLED:
		return "PHY_01 ENABLED";
	}
	return "UNKNOWN";
}

int
n3k_mgmt_lag_init(struct n3k_mgmt_hw *hw)
{
	N3K_MGMT_LOG(LAG, INFO, "Initializing LAG structure");

	if (hw->lag_internal) {
		N3K_MGMT_LOG(LAG, ERR, "LAG structure already initialized");
		return -EINVAL;
	}

	hw->lag_internal = n3k_zmalloc("n3k_lag_internal", sizeof(*hw->lag_internal), 0);
	if (!hw->lag_internal) {
		N3K_MGMT_LOG(LAG, ERR, "Failed to allocate LAG structure");
		return -ENOMEM;
	}

	hw->lag_internal->cur_lacp_state = LAG_LACP_DISABLED;

	return 0;
}

static void
lag_set_lacp_state(struct n3k_mgmt_hw *hw, int state)
{
	MKS_LKUP_LKUP_CFG_REG_t lkup_cfg = {};

	if (state == hw->lag_internal->cur_lacp_state)
		return;

	N3K_MGMT_LOG(LAG, INFO, "LACP STATE CHANGED: %s", lacp_state_to_str(state));
	hw->lag_internal->cur_lacp_state = state;

	lkup_cfg.val = csr_read(hw, MKS_CHIP_MKS_LKUP_BASE, MKS_LKUP_LKUP_CFG_REG);

	const uint32_t port01 = state & LAG_LACP_PHY0_ENABLED ?
		MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_0 : MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_1;

	const uint32_t port23 = state & LAG_LACP_PHY1_ENABLED ?
		MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_1 : MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_0;

	lkup_cfg.field.v_lag_tx_port_id_0 = port01;
	lkup_cfg.field.v_lag_tx_port_id_1 = port01;
	lkup_cfg.field.v_lag_tx_port_id_2 = port23;
	lkup_cfg.field.v_lag_tx_port_id_3 = port23;

	csr_write(hw, MKS_CHIP_MKS_LKUP_BASE, MKS_LKUP_LKUP_CFG_REG, lkup_cfg.val);
}

static void
n3k_mgmt_lag_lacp_check(void *arg)
{
	struct n3k_mgmt_hw *hw = (struct n3k_mgmt_hw *)arg;

	if (!hw->lag_internal->is_enabled)
		return;

	RTE_BUILD_BUG_ON(N3K_PHY_PORTS_COUNT != 2);

	uint16_t phy0_port_id = hw->phy_representors[0]->data->port_id;
	uint16_t phy1_port_id = hw->phy_representors[1]->data->port_id;
	int bond_port_id = n3k_mgmt_get_lag_port_id(hw);
	if (bond_port_id < 0) // bond not available yet
		goto restart_callback;

	int phy0_collect = rte_eth_bond_8023ad_collect_get(bond_port_id, phy0_port_id);
	int phy1_collect = rte_eth_bond_8023ad_collect_get(bond_port_id, phy1_port_id);
	int phy0_distrib = rte_eth_bond_8023ad_distrib_get(bond_port_id, phy0_port_id);
	int phy1_distrib = rte_eth_bond_8023ad_distrib_get(bond_port_id, phy1_port_id);

	if (phy0_collect < 0 || phy1_collect < 0 || phy0_distrib < 0 || phy1_distrib < 0) {
		N3K_MGMT_LOG(LAG, DEBUG, "Couldn't read bond states: %d %d %d %d",
			phy0_collect, phy1_collect, phy0_distrib, phy1_distrib);
		goto restart_callback;
	}

	int state = LAG_LACP_DISABLED;

	if (phy0_collect && phy0_distrib)
		state |= LAG_LACP_PHY0_ENABLED;

	if (phy1_collect && phy1_distrib)
		state |= LAG_LACP_PHY1_ENABLED;

	lag_set_lacp_state(hw, state);

restart_callback:
	if (rte_eal_alarm_set(LACP_REFRESH, n3k_mgmt_lag_lacp_check, hw) < 0)
		N3K_MGMT_LOG(LAG, CRIT, "Failed to set lag refresh alarm");
}

void
n3k_mgmt_lag_enable(struct n3k_mgmt_hw *hw)
{
	if (hw->lag_internal->is_enabled)
		return;

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

	hw->lag_internal->is_enabled = true;

	if (rte_eal_alarm_set(LACP_REFRESH, n3k_mgmt_lag_lacp_check, hw) < 0)
		N3K_MGMT_LOG(LAG, CRIT, "Failed to set lag refresh alarm");
}

void
n3k_mgmt_lag_disable(struct n3k_mgmt_hw *hw)
{
	if (!hw->lag_internal->is_enabled)
		return;

	rte_eal_alarm_cancel(n3k_mgmt_lag_lacp_check, hw);

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

	hw->lag_internal->is_enabled = false;
}

int
n3k_mgmt_is_port_valid_lag_bond(uint16_t port_id)
{
	int i;
	uint16_t slaves[RTE_MAX_ETHPORTS];

	int count = rte_eth_bond_slaves_get(port_id, slaves, RTE_MAX_ETHPORTS);
	if (count != 2)
		return -ENOENT;

	for (i = 0; i < count; i++) {
		struct rte_eth_dev *repr = &rte_eth_devices[slaves[i]];
		if (!n3k_vdev_is_valid_n3k_phy_repr(repr))
			return -ENODEV;
	}
	return 0;
}

static void
try_save_lag_port_id(struct n3k_mgmt_hw *hw)
{
	uint16_t port_id;

	if (!hw->lag_internal->is_enabled || hw->lag_internal->is_bond_port_id_set)
		return;

	RTE_ETH_FOREACH_DEV(port_id) {
		if (n3k_mgmt_is_port_valid_lag_bond(port_id) < 0)
			continue;

		hw->lag_internal->bond_port_id = port_id;
		hw->lag_internal->is_bond_port_id_set = true;
		return;
	}
}

int
n3k_mgmt_get_lag_port_id(struct n3k_mgmt_hw *hw)
{
	try_save_lag_port_id(hw);
	return hw->lag_internal->is_bond_port_id_set ? hw->lag_internal->bond_port_id : -ENODEV;
}
