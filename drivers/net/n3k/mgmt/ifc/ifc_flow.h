/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (c) 2018, Intel Corporation. */

#ifndef _IFC_FLOW_H_
#define _IFC_FLOW_H_

#include <stdbool.h>
#include "ifc_osdep.h"

#include "ifc_port.h"

#include "../n3k_mgmt_hw.h"
#include "../n3k_mgmt_regs.h"

#define CT_MASK(width)           ((uint32_t)((1UL << (width)) - 1))
#define CT_SMASK(shift, width)   ((uint32_t)(CT_MASK(width) << (shift)))
#define CT_RD_FIELD(val, shift, width) \
	(((val) & CT_SMASK((shift), (width)) ) >> (shift))

#define MKS_FT_CMD_TOUT 10
#define IFC_FT_FLUSH_CMD_TOUT 100000
#define IFC_LAST_HANDLE_AFTER_LOAD 15

/* Macros for setting timeout mode na value */
#define MIN_TIMEOUT 0
#define MAX_TIMEOUT 63
#define HARD_TIMEOUT_MODE 1
#define IDLE_TIMEOUT_MODE 0

#define IFC_TIMEOUT_CONFIG_ALL (IFC_NONTCP_CONNTRACK_MAX + 1)

typedef enum {
	IFC_FT_OP_READ           = 0,
	IFC_FT_OP_READ_CLEAR     = 1,  // Read and clear the dirty bit.
	IFC_FT_OP_WRITE          = 2,
	IFC_FT_OP_PARTIAL_WRITE  = 3,
	IFC_FT_OP_FLUSH          = 4,
} ifc_ft_op;

typedef enum {
	IFC_CT_PTYPE_REQ = 0,
	IFC_CT_PTYPE_RSP = 1,
} ifc_ct_ptype;

#define CHECK_STRUCT_SZ(expr) static char temp_st[(expr) ? 0 : -1] __attribute__((unused));

static inline void ifc_ft_write_cmd(struct n3k_mgmt_hw *hw, u32 op, u32 idx)
{
	MKS_FLOW_TRACKER_FT_COMMAND_t ftc;

	memset(&ftc, 0, sizeof(ftc));
	ftc.field.operation = (op);
	ftc.field.index = (idx);
	csr_write(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE, MKS_FLOW_TRACKER_FT_COMMAND, ftc.val);
}

int ifc_ct_upd_transition_tbl(struct n3k_mgmt_hw* hw,
					u32 index, u32 new_state,
					u32 old_state);
int ifc_gen_cmd_check(struct n3k_mgmt_hw* hw,
				u32 base, u32 reg, u32 busy_off,
				u32 timeout);

int ifc_ct_pgm_state_xtion_table(struct n3k_mgmt_hw* hw);
int ifc_ct_pgm_nontcp_xtion_table(struct n3k_mgmt_hw* hw);
int ifc_ft_init(struct n3k_mgmt_hw* hw);
void ifc_notify_init_begin(struct n3k_mgmt_hw* hw);
void ifc_notify_init_done(struct n3k_mgmt_hw* hw);

void ifc_msk_l4_src0(struct n3k_mgmt_hw* hw);

/*
 * All of the connection states CG will support.
 * Total 5bits are reserved in FT module for representing states.
 */
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

/*
 * Shorthand for TCP state names.
 */
#define sNO IFC_TCP_CONNTRACK_NONE
#define sSS IFC_TCP_CONNTRACK_SYN_SENT
#define sSR IFC_TCP_CONNTRACK_SYN_RECV
#define sES IFC_TCP_CONNTRACK_ESTABLISHED
#define sFW IFC_TCP_CONNTRACK_FIN_WAIT
#define sCW IFC_TCP_CONNTRACK_CLOSE_WAIT
#define sLA IFC_TCP_CONNTRACK_LAST_ACK
#define sTW IFC_TCP_CONNTRACK_TIME_WAIT
#define sCL IFC_TCP_CONNTRACK_CLOSE
#define sS2 IFC_TCP_CONNTRACK_SYN_SENT2
#define sIV IFC_TCP_CONNTRACK_MAX
#define sIG IFC_TCP_CONNTRACK_IGNORE

#define sUI IFC_UDP_CONNTRACK_INIT
#define sUE IFC_UDP_CONNTRACK_ESTABLISHED
#define sII IFC_ICMP_CONNTRACK_INIT
#define sIE IFC_ICMP_CONNTRACK_ESTABLISHED

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
static unsigned int tcp_timeouts[IFC_TCP_CONNTRACK_TIMEOUT_MAX] = {
	[IFC_TCP_CONNTRACK_SYN_SENT]       = 2  MINS,
	[IFC_TCP_CONNTRACK_SYN_RECV]       = 60 SECS,
	[IFC_TCP_CONNTRACK_ESTABLISHED]    = 5  DAYS,
	[IFC_TCP_CONNTRACK_FIN_WAIT]       = 2  MINS,
	[IFC_TCP_CONNTRACK_CLOSE_WAIT]     = 60 SECS,
	[IFC_TCP_CONNTRACK_LAST_ACK]       = 30 SECS,
	[IFC_TCP_CONNTRACK_TIME_WAIT]      = 2  MINS,
	[IFC_TCP_CONNTRACK_CLOSE]          = 10 SECS,
	[IFC_TCP_CONNTRACK_SYN_SENT2]      = 2  MINS,
};
 */

typedef enum _ifc_flow_ptype {
	IFC_FLOW_PTYPE_REQ  = 0,
	IFC_FLOW_PTYPE_RSP  = 1,
} ifc_flow_ptype;

/*
 * What TCP flags are set from RST/SYN/FIN/ACK.
 * The values in the next array, ifc_ct_hw_flags, are order dependent
 * on this enum. So make sure to udpate both of the following
 * definitions so that they are in sync.
 */
enum tcp_bit_set {
	IFC_TCP_FLAGS_START,
	IFC_TCP_SYN_SET = IFC_TCP_FLAGS_START,
	IFC_TCP_SYN_ACK_SET,
	IFC_TCP_FIN_SET,
	IFC_TCP_FIN_ACK_SET,
	IFC_TCP_ACK_SET,
	IFC_TCP_RST_SET,
	IFC_TCP_RST_ACK_SET,
	IFC_TCP_NONE_SET, // Last flag
	IFC_TCP_FLAGS_MAX
};

#define IFC_CT_PTYPE_OFFSET   (9)

#define IFC_CT_STATE_OFFSET (4)

#define hw_state_table_idx(ptype, state, flag)                            \
					((ptype) << IFC_CT_PTYPE_OFFSET | \
					 (state) << IFC_CT_STATE_OFFSET | \
					 (flag))

/*
 * HW Operation commands for the state transition table configuration.
 */
enum {
	IFC_CT_XTION_TBL_READ = 0,
	IFC_CT_XTION_TBL_WRITE,
};

/*
 * Bits to provide information to Nios from IFC PM.
 */
#define IFC_CT_TRACKING_MODE_BIT 0
#define IFC_CT_INIT_STATUS_BIT   1

#endif //#ifndef _IFC_FLOW_H_
