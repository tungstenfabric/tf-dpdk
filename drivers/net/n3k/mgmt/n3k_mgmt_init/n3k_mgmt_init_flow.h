/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _N3K_MGMT_INIT_FLOW_H_
#define _N3K_MGMT_INIT_FLOW_H_

#include <stdbool.h>
#include "n3k_mgmt_init_osdep.h"

#include "n3k_mgmt_init_port.h"

#include "../n3k_mgmt_hw.h"
#include "../n3k_mgmt_regs.h"

#define CT_MASK(width)           ((uint32_t)((1UL << (width)) - 1))
#define CT_SMASK(shift, width)   ((uint32_t)(CT_MASK(width) << (shift)))
#define CT_RD_FIELD(val, shift, width) \
	(((val) & CT_SMASK((shift), (width)) ) >> (shift))

#define MKS_FT_CMD_TOUT 10
#define N3K_MGMT_INIT_FT_FLUSH_CMD_TOUT 100000
#define N3K_MGMT_INIT_LAST_HANDLE_AFTER_LOAD 15

/* Macros for setting timeout mode na value */
#define MIN_TIMEOUT 0
#define MAX_TIMEOUT 63
#define HARD_TIMEOUT_MODE 1
#define IDLE_TIMEOUT_MODE 0

#define N3K_MGMT_INIT_TIMEOUT_CONFIG_ALL (N3K_MGMT_INIT_NONTCP_CONNTRACK_MAX + 1)

typedef enum {
	N3K_MGMT_INIT_FT_OP_READ           = 0,
	N3K_MGMT_INIT_FT_OP_READ_CLEAR     = 1,  // Read and clear the dirty bit.
	N3K_MGMT_INIT_FT_OP_WRITE          = 2,
	N3K_MGMT_INIT_FT_OP_PARTIAL_WRITE  = 3,
	N3K_MGMT_INIT_FT_OP_FLUSH          = 4,
} n3k_mgmt_init_ft_op;

typedef enum {
	N3K_MGMT_INIT_CT_PTYPE_REQ = 0,
	N3K_MGMT_INIT_CT_PTYPE_RSP = 1,
} n3k_mgmt_init_ct_ptype;

#define CHECK_STRUCT_SZ(expr) static char temp_st[(expr) ? 0 : -1] __attribute__((unused));

static inline void n3k_mgmt_init_ft_write_cmd(struct n3k_mgmt_hw *hw, u32 op, u32 idx)
{
	MKS_FLOW_TRACKER_FT_COMMAND_t ftc;

	memset(&ftc, 0, sizeof(ftc));
	ftc.field.operation = (op);
	ftc.field.index = (idx);
	csr_write(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE, MKS_FLOW_TRACKER_FT_COMMAND, ftc.val);
}

int n3k_mgmt_init_ct_upd_transition_tbl(struct n3k_mgmt_hw* hw,
					u32 index, u32 new_state,
					u32 old_state);
int n3k_mgmt_init_gen_cmd_check(struct n3k_mgmt_hw* hw,
				u32 base, u32 reg, u32 busy_off,
				u32 timeout);

int n3k_mgmt_init_ct_pgm_state_xtion_table(struct n3k_mgmt_hw* hw);
int n3k_mgmt_init_ct_pgm_nontcp_xtion_table(struct n3k_mgmt_hw* hw);
int n3k_mgmt_init_ft_init(struct n3k_mgmt_hw* hw);
void n3k_mgmt_init_notify_init_begin(struct n3k_mgmt_hw* hw);
void n3k_mgmt_init_notify_init_done(struct n3k_mgmt_hw* hw);

void n3k_mgmt_init_msk_l4_src0(struct n3k_mgmt_hw* hw);

/*
 * All of the connection states CG will support.
 * Total 5bits are reserved in FT module for representing states.
 */
typedef enum _n3k_mgmt_init_ct_states {
	/* 0 */ N3K_MGMT_INIT_TCP_CONNTRACK_NONE,
	/* 1 */ N3K_MGMT_INIT_TCP_CONNTRACK_SYN_SENT,
	/* 2 */ N3K_MGMT_INIT_TCP_CONNTRACK_SYN_RECV,
	/* 3 */ N3K_MGMT_INIT_TCP_CONNTRACK_ESTABLISHED,
	/* 4 */ N3K_MGMT_INIT_TCP_CONNTRACK_FIN_WAIT,
	/* 5 */ N3K_MGMT_INIT_TCP_CONNTRACK_CLOSE_WAIT,
	/* 6 */ N3K_MGMT_INIT_TCP_CONNTRACK_LAST_ACK,
	/* 7 */ N3K_MGMT_INIT_TCP_CONNTRACK_TIME_WAIT,
	/* 8 */ N3K_MGMT_INIT_TCP_CONNTRACK_CLOSE,
	/* 9 */ N3K_MGMT_INIT_TCP_CONNTRACK_SYN_SENT2,
	/* 10 */ N3K_MGMT_INIT_TCP_CONNTRACK_MAX,

	/* 11 */ N3K_MGMT_INIT_TCP_CONNTRACK_IGNORE,      /* Unused */
	/* 12 */ N3K_MGMT_INIT_TCP_CONNTRACK_TIMEOUT_MAX, /* Unused */

	/* 13 */ N3K_MGMT_INIT_NONTCP_CONNTRACK_NONE,     /* Unused */
	/* 13 */ N3K_MGMT_INIT_UDP_CONNTRACK_INIT = N3K_MGMT_INIT_NONTCP_CONNTRACK_NONE,
	/* 14 */ N3K_MGMT_INIT_UDP_CONNTRACK_ESTABLISHED,
	/* 15 */ N3K_MGMT_INIT_ICMP_CONNTRACK_INIT,
	/* 16 */ N3K_MGMT_INIT_ICMP_CONNTRACK_ESTABLISHED,
	/* 17 */ N3K_MGMT_INIT_NONTCP_CONNTRACK_MAX,      /* Unused */

} n3k_mgmt_init_ct_states;

/*
 * Shorthand for TCP state names.
 */
#define sNO N3K_MGMT_INIT_TCP_CONNTRACK_NONE
#define sSS N3K_MGMT_INIT_TCP_CONNTRACK_SYN_SENT
#define sSR N3K_MGMT_INIT_TCP_CONNTRACK_SYN_RECV
#define sES N3K_MGMT_INIT_TCP_CONNTRACK_ESTABLISHED
#define sFW N3K_MGMT_INIT_TCP_CONNTRACK_FIN_WAIT
#define sCW N3K_MGMT_INIT_TCP_CONNTRACK_CLOSE_WAIT
#define sLA N3K_MGMT_INIT_TCP_CONNTRACK_LAST_ACK
#define sTW N3K_MGMT_INIT_TCP_CONNTRACK_TIME_WAIT
#define sCL N3K_MGMT_INIT_TCP_CONNTRACK_CLOSE
#define sS2 N3K_MGMT_INIT_TCP_CONNTRACK_SYN_SENT2
#define sIV N3K_MGMT_INIT_TCP_CONNTRACK_MAX
#define sIG N3K_MGMT_INIT_TCP_CONNTRACK_IGNORE

#define sUI N3K_MGMT_INIT_UDP_CONNTRACK_INIT
#define sUE N3K_MGMT_INIT_UDP_CONNTRACK_ESTABLISHED
#define sII N3K_MGMT_INIT_ICMP_CONNTRACK_INIT
#define sIE N3K_MGMT_INIT_ICMP_CONNTRACK_ESTABLISHED

#define SECS  * 1
#define MINS  * 60 SECS
#define HOURS * 60 MINS
#define DAYS  * 24 HOURS

/*
 * This will be defined in the Nios code.
 *
 * TCP state and their associated timeout values. This table is programmable
 * through PI CLI.
 *
static unsigned int tcp_timeouts[N3K_MGMT_INIT_TCP_CONNTRACK_TIMEOUT_MAX] = {
	[N3K_MGMT_INIT_TCP_CONNTRACK_SYN_SENT]       = 2  MINS,
	[N3K_MGMT_INIT_TCP_CONNTRACK_SYN_RECV]       = 60 SECS,
	[N3K_MGMT_INIT_TCP_CONNTRACK_ESTABLISHED]    = 5  DAYS,
	[N3K_MGMT_INIT_TCP_CONNTRACK_FIN_WAIT]       = 2  MINS,
	[N3K_MGMT_INIT_TCP_CONNTRACK_CLOSE_WAIT]     = 60 SECS,
	[N3K_MGMT_INIT_TCP_CONNTRACK_LAST_ACK]       = 30 SECS,
	[N3K_MGMT_INIT_TCP_CONNTRACK_TIME_WAIT]      = 2  MINS,
	[N3K_MGMT_INIT_TCP_CONNTRACK_CLOSE]          = 10 SECS,
	[N3K_MGMT_INIT_TCP_CONNTRACK_SYN_SENT2]      = 2  MINS,
};
 */

typedef enum _n3k_mgmt_init_flow_ptype {
	N3K_MGMT_INIT_FLOW_PTYPE_REQ  = 0,
	N3K_MGMT_INIT_FLOW_PTYPE_RSP  = 1,
} n3k_mgmt_init_flow_ptype;

/*
 * What TCP flags are set from RST/SYN/FIN/ACK.
 * The values in the next array, n3k_mgmt_init_ct_hw_flags, are order dependent
 * on this enum. So make sure to udpate both of the following
 * definitions so that they are in sync.
 */
enum tcp_bit_set {
	N3K_MGMT_INIT_TCP_FLAGS_START,
	N3K_MGMT_INIT_TCP_SYN_SET = N3K_MGMT_INIT_TCP_FLAGS_START,
	N3K_MGMT_INIT_TCP_SYN_ACK_SET,
	N3K_MGMT_INIT_TCP_FIN_SET,
	N3K_MGMT_INIT_TCP_FIN_ACK_SET,
	N3K_MGMT_INIT_TCP_ACK_SET,
	N3K_MGMT_INIT_TCP_RST_SET,
	N3K_MGMT_INIT_TCP_RST_ACK_SET,
	N3K_MGMT_INIT_TCP_NONE_SET, // Last flag
	N3K_MGMT_INIT_TCP_FLAGS_MAX
};

#define N3K_MGMT_INIT_CT_PTYPE_OFFSET   (9)

#define N3K_MGMT_INIT_CT_STATE_OFFSET (4)

#define hw_state_table_idx(ptype, state, flag)                            \
					((ptype) << N3K_MGMT_INIT_CT_PTYPE_OFFSET | \
					 (state) << N3K_MGMT_INIT_CT_STATE_OFFSET | \
					 (flag))

/*
 * HW Operation commands for the state transition table configuration.
 */
enum {
	N3K_MGMT_INIT_CT_XTION_TBL_READ = 0,
	N3K_MGMT_INIT_CT_XTION_TBL_WRITE,
};

/*
 * Bits to provide information to Nios from N3K_MGMT_INIT PM.
 */
#define N3K_MGMT_INIT_CT_TRACKING_MODE_BIT 0
#define N3K_MGMT_INIT_CT_INIT_STATUS_BIT   1

#endif //#ifndef _N3K_MGMT_INIT_FLOW_H_
