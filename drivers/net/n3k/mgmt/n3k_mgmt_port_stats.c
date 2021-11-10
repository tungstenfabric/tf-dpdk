/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_bitwise.h"
#include "n3k_mgmt_dump.h"
#include "n3k_mgmt_ethdev.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_lock.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_port_stats.h"
#include "n3k_mgmt_ports.h"
#include "n3k_mgmt_regs_ctrl.h"
#include "n3k_mgmt_init/n3k_mgmt_init_port.h"
#include "vdev/n3k_vdev_device.h"
#include "regs/n3k_mgmt_regs.h"

#include <stdint.h>
#include <string.h>
#include <sys/stat.h>

enum query_type {
	QUERY_PACKETS_UNICAST = 0,
	QUERY_PACKETS_MULTICAST = 1,
	QUERY_BYTES = 2,
};

static int
prepare_pars_stats_cfg(
	struct n3k_mgmt_hw *hw, uint16_t port_id,
	MKS_PARS_PARS_STATS_CFG_t *pars_stats, enum query_type type)
{
	uint8_t port, subport;
	int ret;

	memset(pars_stats, 0, sizeof(*pars_stats));
	pars_stats->field.status_rd_enable = 1;
	pars_stats->field.status_blk_id = type;

	ret = n3k_mgmt_dpdk_port_to_hw_port_subport(hw, port_id, &port, &subport);
	if (ret < 0)
		return ret;

	pars_stats->field.status_port_id = port;
	pars_stats->field.status_sub_port_id = subport;

	return 0;
}

static int
exec_pars_stats_cmd(struct n3k_mgmt_hw* hw, uint32_t port_id, enum query_type type)
{
	MKS_PARS_PARS_STATS_CFG_t pars_stats;
	int ret;

	ret = prepare_pars_stats_cfg(hw, port_id, &pars_stats, type);
	if (ret < 0)
		return ret;

	csr_write(hw, MKS_CHIP_MKS_PARS_BASE, MKS_PARS_PARS_STATS_CFG, pars_stats.val);
	return n3k_mgmt_poll_ctrl_reg(hw, N3K_MGMT_CTRL_REG_PARS_STATS_CFG);
}

static uint64_t
read_40_bits(struct n3k_mgmt_hw *hw, uintptr_t regs_base, uintptr_t regs_offset)
{
	uint32_t cnt_low, cnt_high;
	cnt_low = csr_read(hw, regs_base, regs_offset);
	cnt_high = csr_read(hw, regs_base, regs_offset + 4);
	return cnt_low | ((uint64_t)BITS_RANGE32(cnt_high, 0, 8) << 32);
}

static uint64_t
read_pars_stat_count(struct n3k_mgmt_hw *hw)
{
	return read_40_bits(hw, MKS_CHIP_MKS_PARS_BASE, MKS_PARS_PARS_STAT_REG0);
}

static int
read_rx_stats(struct n3k_mgmt_hw *hw, uint16_t port_id, struct n3k_mgmt_port_stats *stats)
{
	int ret;

	ret = exec_pars_stats_cmd(hw, port_id, QUERY_PACKETS_UNICAST);
	if (ret < 0)
		return ret;
	stats->ipackets = read_pars_stat_count(hw);

	ret = exec_pars_stats_cmd(hw, port_id, QUERY_PACKETS_MULTICAST);
	if (ret < 0)
		return ret;
	stats->ipackets += read_pars_stat_count(hw);

	ret = exec_pars_stats_cmd(hw, port_id, QUERY_BYTES);
	if (ret < 0)
		return ret;
	stats->ibytes = read_pars_stat_count(hw);

	return 0;
}

static int
prepare_mod_stats_cfg(struct n3k_mgmt_hw* hw, uint16_t port_id,
	MKS_MOD_MOD_STATS_CFG_t *mod_stats, enum query_type type)
{
	uint8_t mod_port;
	int ret;

	// Read modifier to preserve previous options
	mod_stats->val = csr_read(hw, MKS_CHIP_MKS_MOD_BASE, MKS_MOD_MOD_STATS_CFG);
	mod_stats->field.status_wr_enable = 0;
	mod_stats->field.st_blk_id_rcirc = 0;
	mod_stats->field.status_rd_enable = 1;
	mod_stats->field.status_blk_id = type;

	ret = n3k_mgmt_dpdk_port_to_hw_modifier_port(hw, port_id, &mod_port);
	if (ret < 0)
		return ret;
	mod_stats->field.status_addr = mod_port;

	return 0;
}

static int
exec_mod_stats_cmd(struct n3k_mgmt_hw* hw, uint32_t port_id, enum query_type type)
{
	MKS_MOD_MOD_STATS_CFG_t mod_stats;
	int ret;

	ret = prepare_mod_stats_cfg(hw, port_id, &mod_stats, type);
	if (ret < 0)
		return ret;

	csr_write(hw, MKS_CHIP_MKS_MOD_BASE, MKS_MOD_MOD_STATS_CFG, mod_stats.val);
	return n3k_mgmt_poll_ctrl_reg(hw, N3K_MGMT_CTRL_REG_MOD_STATS_CFG);
}

static uint64_t
read_mod_stat_count(struct n3k_mgmt_hw *hw)
{
	return read_40_bits(hw, MKS_CHIP_MKS_MOD_BASE, MKS_MOD_MOD_STAT_REG0);
}

static int
read_tx_stats(struct n3k_mgmt_hw *hw, uint16_t port_id,
	struct n3k_mgmt_port_stats *stats)
{
	int ret;

	ret = exec_mod_stats_cmd(hw, port_id, QUERY_PACKETS_UNICAST);
	if (ret < 0)
		return ret;
	stats->opackets = read_mod_stat_count(hw);

	ret = exec_mod_stats_cmd(hw, port_id, QUERY_PACKETS_MULTICAST);
	if (ret < 0)
		return ret;
	stats->opackets += read_mod_stat_count(hw);

	ret = exec_mod_stats_cmd(hw, port_id, QUERY_BYTES);
	if (ret < 0)
		return ret;
	stats->obytes = read_mod_stat_count(hw);

	return 0;
}

int
n3k_mgmt_get_vdev_stats(struct rte_eth_dev *dev, struct n3k_mgmt_port_stats *stats)
{
	struct rte_eth_dev *mgmt;
	struct n3k_mgmt_hw *hw;
	int ret;

	ret = n3k_vdev_get_mgmt_dev(dev, &mgmt);
	if (ret < 0)
		return ret;

	hw = n3k_mgmt_get_hw_from_dev_private(mgmt);
	if (hw == NULL)
		return -EINVAL;

	n3k_mgmt_lock(hw);

	ret = read_rx_stats(hw, dev->data->port_id, stats);
	if (ret < 0) {
		N3K_MGMT_LOG(DEFAULT, ERR,
			"Error occurred while while extracting rx stats: %d",
			ret);
		goto release_lock;
	}
	ret = read_tx_stats(hw, dev->data->port_id, stats);
	if (ret < 0) {
		N3K_MGMT_LOG(DEFAULT, ERR,
			"Error occurred while while extracting tx stats: %d",
			ret);
		goto release_lock;
	}

release_lock:
	n3k_mgmt_unlock(hw);
	return ret;
}

int
n3k_mgmt_port_stats_dump(struct rte_eth_dev *dev)
{
	struct n3k_mgmt_port_stats stats;
	int ret;

	ret = n3k_mgmt_get_vdev_stats(dev, &stats);
	if (ret < 0)
		return ret;

	n3k_mgmt_port_stats_struct_dump(&stats);

	return 0;
}
