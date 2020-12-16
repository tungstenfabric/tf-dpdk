/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "ifc/ifc_port.h"
#include "n3k_mgmt_bitwise.h"
#include "n3k_mgmt_dump.h"
#include "n3k_mgmt_flow_tbl.h"
#include "n3k_mgmt_flow_tracker.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_regs_ctrl.h"
#include "n3k_mgmt_lock.h"

#include <stdint.h>
#include <unistd.h>

#define N3K_MGMT_MIN_STATS_ID 20

// TODO(n3k): Enums copy pasted from ifc code - needs some refactor
//            Also move full flow tracker initialization here
typedef enum {
	IFC_FT_TYPE_NON_CT,
	IFC_FT_TYPE_CT,
	IFC_FT_DIR_RSP,
} ifc_ft_conn_type;

typedef enum _ifc_ct_states {
	/* 0 */ IFC_TCP_CONNTRACK_NONE,
	/* 1 */ IFC_TCP_CONNTRACK_SYN_SENT,
	/* 2 */ IFC_TCP_CONNTRACK_SYN_RECV,
	/* 3 */ IFC_TCP_CONNTRACK_ESTABLISHED,
	/* 4 */ IFC_TCP_CONNTRACK_FIN_WAIT,
	/* 5 */ IFC_TCP_CONNTRACK_CLOSE_WAIT,
	/* 6 */ IFC_TCP_CONNTRACK_LAST_ACK,
	/* 7 */ IFC_TCP_CONNTRACK_TIME_WAIT,
	/* 8 */ IFC_TCP_CONNTRACK_CLOSE,
	/* 9 */ IFC_TCP_CONNTRACK_SYN_SENT2,
	/* 10 */ IFC_TCP_CONNTRACK_MAX,

	/* 11 */ IFC_TCP_CONNTRACK_IGNORE,      /* Unused */
	/* 12 */ IFC_TCP_CONNTRACK_TIMEOUT_MAX, /* Unused */

	/* 13 */ IFC_NONTCP_CONNTRACK_NONE,     /* Unused */
	/* 13 */ IFC_UDP_CONNTRACK_INIT = IFC_NONTCP_CONNTRACK_NONE,
	/* 14 */ IFC_UDP_CONNTRACK_ESTABLISHED,
	/* 15 */ IFC_ICMP_CONNTRACK_INIT,
	/* 16 */ IFC_ICMP_CONNTRACK_ESTABLISHED,
	/* 17 */ IFC_NONTCP_CONNTRACK_MAX,      /* Unused */

} ifc_ct_states;

int n3k_mgmt_flow_tracker_init(struct n3k_mgmt_hw *hw)
{
	MKS_FLOW_TRACKER_FT_STAT_t ft_stat = {{0}};
	int ret;

	ret = n3k_mgmt_poll_ctrl_reg_with_result(hw, N3K_MGMT_CTRL_REG_FT_STAT,
		&ft_stat.val);
	if (ret < 0)
		return ret;
	if (ft_stat.field.fatal_error)
		return -EFAULT;

	return 0;
}

static int exec_flow_tracker_cmd(struct n3k_mgmt_hw* hw, uint32_t stats_id,
	enum MKS_FLOW_TRACKER_FT_COMMAND_OP_TYPE type)
{
	MKS_FLOW_TRACKER_FT_COMMAND_t ft_comm = {{0}};
	ft_comm.field.index = stats_id;
	ft_comm.field.operation = type;
	csr_write(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE, MKS_FLOW_TRACKER_FT_COMMAND,
		ft_comm.val);

	return n3k_mgmt_poll_ctrl_reg(hw, N3K_MGMT_CTRL_REG_FT_COMMAND);
}

void n3k_mgmt_flow_tracker_enable_fill_regs(struct n3k_mgmt_hw* hw,
	struct flow_tracker_data_regs *ft_regs, uint8_t ip_prot)
{
	uint32_t init_time = 0;
	memset(ft_regs, 0, sizeof(*ft_regs));

	ft_regs->data_7.field.valid = 1;
	init_time = csr_read(hw, MKS_CHIP_MKS_LKUP_BASE,
		MKS_LKUP_FT_TIMESTAMP_0_CPY);
	ft_regs->data_7.field.timestamp = BITS_RANGE32(init_time, 0, 24);

	ft_regs->data_7.field.conn_type = IFC_FT_TYPE_NON_CT;
	ft_regs->data_7.field.state = IFC_TCP_CONNTRACK_NONE;
	if (ip_prot == IPPROTO_TCP) {
		// TODO(n3k): Add support for different stages of TCP connection
		ft_regs->data_7.field.conn_type = IFC_FT_TYPE_CT;
		ft_regs->data_7.field.state = IFC_TCP_CONNTRACK_ESTABLISHED;
	} else if (ip_prot == IPPROTO_UDP) {
		ft_regs->data_7.field.conn_type = IFC_FT_TYPE_CT;
		ft_regs->data_7.field.state = IFC_UDP_CONNTRACK_INIT;
	}
}

inline uint32_t
n3k_mgmt_flow_tracker_get_min_stats_id(__rte_unused struct n3k_mgmt_hw* hw)
{
	return N3K_MGMT_MIN_STATS_ID;
}

inline uint32_t
n3k_mgmt_flow_id_to_stats_id(struct n3k_mgmt_hw* hw, uint32_t flow_id)
{
	return flow_id + n3k_mgmt_flow_tracker_get_min_stats_id(hw) -
		n3k_mgmt_flow_tbl_get_min_flow_id(hw);
}

int n3k_mgmt_flow_tracker_write_regs(struct n3k_mgmt_hw* hw, uint32_t stats_id,
	const struct flow_tracker_data_regs *ft_regs)
{
	MKS_FLOW_TRACKER_FT_STAT_t ft_stat = {{0}};
	int ret;

	n3k_mgmt_lock(hw);

	ft_stat.val = csr_read(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE, MKS_FLOW_TRACKER_FT_STAT);
	if (ft_stat.field.fatal_error) {
		n3k_mgmt_unlock(hw);
		N3K_MGMT_LOG(ERR, "flow tracker fatal error");
		return -EFAULT;
	}

	n3k_mgmt_program_reg_tbl(hw, N3K_MGMT_REG_TBL_FLOW_TRACKER_DATA, ft_regs);

	RTE_ASSERT(!n3k_mgmt_ctrl_reg_is_busy(hw, N3K_MGMT_CTRL_REG_FT_COMMAND) && "flow tracker busy");

	ret = exec_flow_tracker_cmd(hw, stats_id, MKS_FLOW_TRACKER_FT_COMMAND_WRITE);
	if (ret < 0) {
		n3k_mgmt_unlock(hw);
		N3K_MGMT_LOG(ERR,
			"Error occurred while waiting to complete flow tracker write operation: %d",
			ret);
		return ret;
	}

	n3k_mgmt_unlock(hw);

	return 0;
}

int n3k_mgmt_flow_tracker_enable(struct n3k_mgmt_hw* hw, uint32_t stats_id,
	uint8_t ip_prot)
{
	struct flow_tracker_data_regs ft_regs;
	n3k_mgmt_flow_tracker_enable_fill_regs(hw, &ft_regs, ip_prot);
	return n3k_mgmt_flow_tracker_write_regs(hw, stats_id, &ft_regs);
}

void n3k_mgmt_flow_tracker_disable_fill_regs(struct flow_tracker_data_regs *ft_regs)
{
	memset(ft_regs, 0, sizeof(*ft_regs));
	ft_regs->data_7.field.valid = 0;
}

int n3k_mgmt_flow_tracker_disable(struct n3k_mgmt_hw* hw, uint32_t stats_id)
{
	struct flow_tracker_data_regs ft_regs;
	n3k_mgmt_flow_tracker_disable_fill_regs(&ft_regs);
	return n3k_mgmt_flow_tracker_write_regs(hw, stats_id, &ft_regs);
}

void n3k_mgmt_flow_tracker_fill_stats(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tracker_stats *ft_stats,
	struct flow_tracker_data_regs *ft_regs)
{
	uint32_t curr_time;

	ft_stats->bytes_cnt = ft_regs->data_2.field.byte_count_lsb |
		((uint64_t)ft_regs->data_3.field.byte_count_msb << 32);

	ft_stats->packet_cnt = ft_regs->data_4.field.pkt_count_lsb |
		((uint64_t)ft_regs->data_5.field.pkt_count_msb << 32);

	curr_time = csr_read(hw, MKS_CHIP_MKS_LKUP_BASE,
		MKS_LKUP_FT_TIMESTAMP_0_CPY);
	curr_time = BITS_RANGE32(curr_time, 0, 24);
	ft_stats->life_time = curr_time - ft_regs->data_7.field.timestamp;
}


int n3k_mgmt_flow_tracker_get_by_regs(struct n3k_mgmt_hw *hw,
	uint32_t stats_id, struct flow_tracker_data_regs *ft_regs)
{
	MKS_FLOW_TRACKER_FT_COMMAND_t ft_comm = {{0}};
	int ret;

	n3k_mgmt_lock(hw);

	RTE_ASSERT(!n3k_mgmt_ctrl_reg_is_busy(hw, N3K_MGMT_CTRL_REG_FT_COMMAND) && "flow tracker busy");

	ft_comm.field.index = stats_id;
	ft_comm.field.operation = MKS_FLOW_TRACKER_FT_COMMAND_READ;
	csr_write(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE, MKS_FLOW_TRACKER_FT_COMMAND,
		ft_comm.val);

	ret = n3k_mgmt_poll_ctrl_reg(hw, N3K_MGMT_CTRL_REG_FT_COMMAND);
	if (ret < 0) {
		N3K_MGMT_LOG(ERR,
			"Error occurred while waiting to complete flow tracker read operation: %d",
			ret);
		n3k_mgmt_unlock(hw);
		return ret;
	}
	n3k_mgmt_read_reg_tbl(hw, N3K_MGMT_REG_TBL_FLOW_TRACKER_DATA, ft_regs);

	n3k_mgmt_unlock(hw);
	return 0;
}

int n3k_mgmt_flow_tracker_get(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle,
	struct n3k_mgmt_flow_tracker_stats *ft_stats)
{
	struct flow_tracker_data_regs ft_regs;
	int ret;

	ret = n3k_mgmt_flow_tracker_get_by_regs(hw,
		n3k_mgmt_flow_id_to_stats_id(hw, handle->flow_id), &ft_regs);
	if (ret < 0)
		return ret;
	n3k_mgmt_flow_tracker_fill_stats(hw, ft_stats, &ft_regs);

	return 0;
}

int n3k_mgmt_flow_tracker_dump(struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_flow_tbl_handle *handle)
{
	struct n3k_mgmt_flow_tracker_stats ft_stats;
	int ret;

	ret = n3k_mgmt_flow_tracker_get(hw, handle, &ft_stats);
	if (ret < 0)
		return ret;
