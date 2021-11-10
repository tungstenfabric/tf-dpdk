/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "n3k_mgmt_init_flow.h"
#include "n3k_mgmt_init_port.h"
#include "mgmt/n3k_mgmt_lock.h"
#include "mgmt/n3k_mgmt_log.h"
#include "mgmt/regs/n3k_mgmt_regs.h"

/*
 * This will be defined in the Nios code.
 *
 * TCP state and their associated timeout values. This table is programmable
 * through PI CLI.
 *
#define SECS  * 1
#define MINS  * 60 SECS
#define HOURS * 60 MINS
#define DAYS  * 24 HOURS
static unsigned int tcp_timeouts[CT_TCP_TIMEOUT_MAX] = {
	[CT_TCP_SYN_SENT]       = 2  MINS,
	[CT_TCP_SYN_RECV]       = 60 SECS,
	[CT_TCP_ESTABLISHED]    = 5  DAYS,
	[CT_TCP_FIN_WAIT]       = 2  MINS,
	[CT_TCP_CLOSE_WAIT]     = 60 SECS,
	[CT_TCP_LAST_ACK]       = 30 SECS,
	[CT_TCP_TIME_WAIT]      = 2  MINS,
	[CT_TCP_CLOSE]          = 10 SECS,
	[CT_TCP_SYN_SENT2]      = 2  MINS,
};
 */

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

#define N3K_MGMT_INIT_TIMEOUT_CONFIG_ALL (CT_NONTCP_MAX + 1)

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
	N3K_MGMT_INIT_CT_PTYPE_MAX = 2,
} n3k_mgmt_init_ct_ptype;

#define CHECK_STRUCT_SZ(expr) static char temp_st[(expr) ? 0 : -1] __attribute__((unused));

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

/*
 * All of the connection states CG will support.
 * Total 5bits are reserved in FT module for representing states.
 */
typedef enum _n3k_mgmt_init_ct_tcp_states {
	/* 0 */ CT_TCP_NONE,
	/* 1 */ CT_TCP_SYN_SENT,
	/* 2 */ CT_TCP_SYN_RECV,
	/* 3 */ CT_TCP_ESTABLISHED,
	/* 4 */ CT_TCP_FIN_WAIT,
	/* 5 */ CT_TCP_CLOSE_WAIT,
	/* 6 */ CT_TCP_LAST_ACK,
	/* 7 */ CT_TCP_TIME_WAIT,
	/* 8 */ CT_TCP_CLOSE,
	/* 9 */ CT_TCP_SYN_SENT2,
	/* 10 */ CT_TCP_MAX,
	/* 10 */ CT_TCP_INVALID = CT_TCP_MAX,

	/* 11 */ CT_TCP_IGNORE,      /* Unused */
	/* 12 */ CT_TCP_TIMEOUT_MAX, /* Unused */
} n3k_mgmt_init_ct_tcp_states;

typedef enum _n3k_mgmt_init_ct_nontcp_states {
	/* 13 */ CT_NONTCP_NONE = CT_TCP_TIMEOUT_MAX + 1,     /* Unused */
	/* 13 */ CT_UDP_INIT = CT_NONTCP_NONE,
	/* 14 */ CT_UDP_ESTABLISHED,
	/* 15 */ CT_ICMP_INIT,
	/* 16 */ CT_ICMP_ESTABLISHED,
	/* 17 */ CT_NONTCP_MAX,      /* Unused */
} n3k_mgmt_init_ct_nontcp_states;

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

/*
 * Flags are encoded as follows:
 *         SYN, ACK, FIN, RST
 */
static const uint8_t n3k_mgmt_init_ct_hw_flags[N3K_MGMT_INIT_TCP_FLAGS_MAX] = {
	[N3K_MGMT_INIT_TCP_SYN_SET]    = 0x08, //1000
	[N3K_MGMT_INIT_TCP_SYN_ACK_SET]= 0x0C, //1100
	[N3K_MGMT_INIT_TCP_FIN_SET]    = 0x02, //0010
	[N3K_MGMT_INIT_TCP_FIN_ACK_SET]= 0x06, //0110
	[N3K_MGMT_INIT_TCP_ACK_SET]    = 0x04, //0100
	[N3K_MGMT_INIT_TCP_RST_SET]    = 0x01, //0001
	[N3K_MGMT_INIT_TCP_RST_ACK_SET]= 0x05, //0101
	[N3K_MGMT_INIT_TCP_NONE_SET]   = 0x00, //0000
};

const char *n3k_mgmt_init_ct_state_name[] = {
	[CT_TCP_NONE]         = "        CT_TCP_NONE",
	[CT_TCP_SYN_SENT]     = "    CT_TCP_SYN_SENT",
	[CT_TCP_SYN_RECV]     = "    CT_TCP_SYN_RECV",
	[CT_TCP_ESTABLISHED]  = " CT_TCP_ESTABLISHED",
	[CT_TCP_FIN_WAIT]     = "    CT_TCP_FIN_WAIT",
	[CT_TCP_CLOSE_WAIT]   = "  CT_TCP_CLOSE_WAIT",
	[CT_TCP_LAST_ACK]     = "    CT_TCP_LAST_ACK",
	[CT_TCP_TIME_WAIT]    = "   CT_TCP_TIME_WAIT",
	[CT_TCP_CLOSE]        = "       CT_TCP_CLOSE",
	[CT_TCP_SYN_SENT2]    = "   CT_TCP_SYN_SENT2",
	[CT_TCP_INVALID]      = "     CT_TCP_INVALID",
	[CT_TCP_IGNORE]       = "      CT_TCP_IGNORE",
	[CT_UDP_INIT]         = "        CT_UDP_INIT",
	[CT_UDP_ESTABLISHED]  = " CT_UDP_ESTABLISHED",
	[CT_ICMP_INIT]        = "       CT_ICMP_INIT",
	[CT_ICMP_ESTABLISHED] = "CT_ICMP_ESTABLISHED",
};

const char *n3k_mgmt_init_ct_flag_name[] = {
	[N3K_MGMT_INIT_TCP_SYN_SET]    = "    SYN",
	[N3K_MGMT_INIT_TCP_SYN_ACK_SET]= "SYN+ACK",
	[N3K_MGMT_INIT_TCP_FIN_SET]    = "    FIN",
	[N3K_MGMT_INIT_TCP_FIN_ACK_SET]= "FIN+ACK",
	[N3K_MGMT_INIT_TCP_ACK_SET]    = "    ACK",
	[N3K_MGMT_INIT_TCP_RST_SET]    = "    RST",
	[N3K_MGMT_INIT_TCP_RST_ACK_SET]= "RST+ACK",
	[N3K_MGMT_INIT_TCP_NONE_SET]   = "   NONE",
	[N3K_MGMT_INIT_TCP_FLAGS_MAX]  = "    MAX",
};

const char *n3k_mgmt_init_ct_ptype_name[] = {
	[N3K_MGMT_INIT_CT_PTYPE_REQ] = " REQUEST",
	[N3K_MGMT_INIT_CT_PTYPE_RSP] = "RESPONSE",
};

struct ct_tcp_transitions_desc {
	const uint8_t response[N3K_MGMT_INIT_TCP_FLAGS_MAX][CT_TCP_MAX];
	const uint8_t request[N3K_MGMT_INIT_TCP_FLAGS_MAX][CT_TCP_MAX];
};

/*
 * TCP transition table for both ORIGINAL and REPLY direction.
 */
static const struct ct_tcp_transitions_desc ct_tcp_transition = {
	.request = {
		[N3K_MGMT_INIT_TCP_SYN_SET] = {
			[CT_TCP_NONE] = CT_TCP_SYN_SENT,
			[CT_TCP_SYN_SENT] = CT_TCP_SYN_SENT,
			[CT_TCP_SYN_RECV] = CT_TCP_SYN_RECV,
			[CT_TCP_ESTABLISHED] = CT_TCP_ESTABLISHED,
			[CT_TCP_FIN_WAIT] = CT_TCP_FIN_WAIT,
			[CT_TCP_CLOSE_WAIT] = CT_TCP_CLOSE_WAIT,
			[CT_TCP_LAST_ACK] = CT_TCP_LAST_ACK,
			[CT_TCP_TIME_WAIT] = CT_TCP_SYN_SENT,
			[CT_TCP_CLOSE] = CT_TCP_SYN_SENT,
			[CT_TCP_SYN_SENT2] = CT_TCP_SYN_SENT2
		},
		[N3K_MGMT_INIT_TCP_SYN_ACK_SET] = {
			[CT_TCP_NONE] = CT_TCP_NONE,
			[CT_TCP_SYN_SENT] = CT_TCP_SYN_SENT,
			[CT_TCP_SYN_RECV] = CT_TCP_SYN_RECV,
			[CT_TCP_ESTABLISHED] = CT_TCP_ESTABLISHED,
			[CT_TCP_FIN_WAIT] = CT_TCP_FIN_WAIT,
			[CT_TCP_CLOSE_WAIT] = CT_TCP_CLOSE_WAIT,
			[CT_TCP_LAST_ACK] = CT_TCP_INVALID,
			[CT_TCP_TIME_WAIT] = CT_TCP_TIME_WAIT,
			[CT_TCP_CLOSE] = CT_TCP_CLOSE,
			[CT_TCP_SYN_SENT2] = CT_TCP_SYN_RECV
		},
		[N3K_MGMT_INIT_TCP_FIN_SET] = {
			[CT_TCP_NONE] = CT_TCP_NONE,
			[CT_TCP_SYN_SENT] = CT_TCP_SYN_SENT,
			[CT_TCP_SYN_RECV] = CT_TCP_FIN_WAIT,
			[CT_TCP_ESTABLISHED] = CT_TCP_FIN_WAIT,
			[CT_TCP_FIN_WAIT] = CT_TCP_LAST_ACK,
			[CT_TCP_CLOSE_WAIT] = CT_TCP_LAST_ACK,
			[CT_TCP_LAST_ACK] = CT_TCP_LAST_ACK,
			[CT_TCP_TIME_WAIT] = CT_TCP_TIME_WAIT,
			[CT_TCP_CLOSE] = CT_TCP_CLOSE,
			[CT_TCP_SYN_SENT2] = CT_TCP_SYN_SENT2
		},
		[N3K_MGMT_INIT_TCP_FIN_ACK_SET] = {
			[CT_TCP_NONE] = CT_TCP_NONE,
			[CT_TCP_SYN_SENT] = CT_TCP_SYN_SENT,
			[CT_TCP_SYN_RECV] = CT_TCP_FIN_WAIT,
			[CT_TCP_ESTABLISHED] = CT_TCP_FIN_WAIT,
			[CT_TCP_FIN_WAIT] = CT_TCP_LAST_ACK,
			[CT_TCP_CLOSE_WAIT] = CT_TCP_LAST_ACK,
			[CT_TCP_LAST_ACK] = CT_TCP_LAST_ACK,
			[CT_TCP_TIME_WAIT] = CT_TCP_TIME_WAIT,
			[CT_TCP_CLOSE] = CT_TCP_CLOSE,
			[CT_TCP_SYN_SENT2] = CT_TCP_SYN_SENT2
		},
		[N3K_MGMT_INIT_TCP_ACK_SET] = {
			[CT_TCP_NONE] = CT_TCP_ESTABLISHED,
			[CT_TCP_SYN_SENT] = CT_TCP_SYN_SENT,
			[CT_TCP_SYN_RECV] = CT_TCP_ESTABLISHED,
			[CT_TCP_ESTABLISHED] = CT_TCP_ESTABLISHED,
			[CT_TCP_FIN_WAIT] = CT_TCP_CLOSE_WAIT,
			[CT_TCP_CLOSE_WAIT] = CT_TCP_CLOSE_WAIT,
			[CT_TCP_LAST_ACK] = CT_TCP_TIME_WAIT,
			[CT_TCP_TIME_WAIT] = CT_TCP_TIME_WAIT,
			[CT_TCP_CLOSE] = CT_TCP_CLOSE,
			[CT_TCP_SYN_SENT2] = CT_TCP_SYN_SENT2
		},
		[N3K_MGMT_INIT_TCP_RST_SET] = {
			[CT_TCP_NONE] = CT_TCP_NONE,
			[CT_TCP_SYN_SENT] = CT_TCP_CLOSE,
			[CT_TCP_SYN_RECV] = CT_TCP_CLOSE,
			[CT_TCP_ESTABLISHED] = CT_TCP_CLOSE,
			[CT_TCP_FIN_WAIT] = CT_TCP_CLOSE,
			[CT_TCP_CLOSE_WAIT] = CT_TCP_CLOSE,
			[CT_TCP_LAST_ACK] = CT_TCP_CLOSE,
			[CT_TCP_TIME_WAIT] = CT_TCP_CLOSE,
			[CT_TCP_CLOSE] = CT_TCP_CLOSE,
			[CT_TCP_SYN_SENT2] = CT_TCP_CLOSE
		},
		[N3K_MGMT_INIT_TCP_RST_ACK_SET] = {
			[CT_TCP_NONE] = CT_TCP_NONE,
			[CT_TCP_SYN_SENT] = CT_TCP_CLOSE,
			[CT_TCP_SYN_RECV] = CT_TCP_CLOSE,
			[CT_TCP_ESTABLISHED] = CT_TCP_CLOSE,
			[CT_TCP_FIN_WAIT] = CT_TCP_CLOSE,
			[CT_TCP_CLOSE_WAIT] = CT_TCP_CLOSE,
			[CT_TCP_LAST_ACK] = CT_TCP_CLOSE,
			[CT_TCP_TIME_WAIT] = CT_TCP_CLOSE,
			[CT_TCP_CLOSE] = CT_TCP_CLOSE,
			[CT_TCP_SYN_SENT2] = CT_TCP_CLOSE
		},
		[N3K_MGMT_INIT_TCP_NONE_SET] = {
			[CT_TCP_NONE] = CT_TCP_NONE,
			[CT_TCP_SYN_SENT] = CT_TCP_SYN_SENT,
			[CT_TCP_SYN_RECV] = CT_TCP_SYN_RECV,
			[CT_TCP_ESTABLISHED] = CT_TCP_ESTABLISHED,
			[CT_TCP_FIN_WAIT] = CT_TCP_FIN_WAIT,
			[CT_TCP_CLOSE_WAIT] = CT_TCP_CLOSE_WAIT,
			[CT_TCP_LAST_ACK] = CT_TCP_LAST_ACK,
			[CT_TCP_TIME_WAIT] = CT_TCP_TIME_WAIT,
			[CT_TCP_CLOSE] = CT_TCP_CLOSE,
			[CT_TCP_SYN_SENT2] = CT_TCP_SYN_SENT2
		}
	},
	.response = {
		[N3K_MGMT_INIT_TCP_SYN_SET] = {
			[CT_TCP_NONE] = CT_TCP_NONE,
			[CT_TCP_SYN_SENT] = CT_TCP_SYN_SENT2,
			[CT_TCP_SYN_RECV] = CT_TCP_SYN_RECV,
			[CT_TCP_ESTABLISHED] = CT_TCP_ESTABLISHED,
			[CT_TCP_FIN_WAIT] = CT_TCP_FIN_WAIT,
			[CT_TCP_CLOSE_WAIT] = CT_TCP_CLOSE_WAIT,
			[CT_TCP_LAST_ACK] = CT_TCP_LAST_ACK,
			[CT_TCP_TIME_WAIT] = CT_TCP_SYN_SENT,
			[CT_TCP_CLOSE] = CT_TCP_CLOSE,
			[CT_TCP_SYN_SENT2] = CT_TCP_SYN_SENT2
		},
		[N3K_MGMT_INIT_TCP_SYN_ACK_SET] = {
			[CT_TCP_NONE] = CT_TCP_NONE,
			[CT_TCP_SYN_SENT] = CT_TCP_SYN_RECV,
			[CT_TCP_SYN_RECV] = CT_TCP_SYN_RECV,
			[CT_TCP_ESTABLISHED] = CT_TCP_ESTABLISHED,
			[CT_TCP_FIN_WAIT] = CT_TCP_FIN_WAIT,
			[CT_TCP_CLOSE_WAIT] = CT_TCP_CLOSE_WAIT,
			[CT_TCP_LAST_ACK] = CT_TCP_LAST_ACK,
			[CT_TCP_TIME_WAIT] = CT_TCP_TIME_WAIT,
			[CT_TCP_CLOSE] = CT_TCP_CLOSE,
			[CT_TCP_SYN_SENT2] = CT_TCP_SYN_RECV
		},
		[N3K_MGMT_INIT_TCP_FIN_SET] = {
			[CT_TCP_NONE] = CT_TCP_NONE,
			[CT_TCP_SYN_SENT] = CT_TCP_SYN_SENT,
			[CT_TCP_SYN_RECV] = CT_TCP_FIN_WAIT,
			[CT_TCP_ESTABLISHED] = CT_TCP_FIN_WAIT,
			[CT_TCP_FIN_WAIT] = CT_TCP_LAST_ACK,
			[CT_TCP_CLOSE_WAIT] = CT_TCP_LAST_ACK,
			[CT_TCP_LAST_ACK] = CT_TCP_LAST_ACK,
			[CT_TCP_TIME_WAIT] = CT_TCP_TIME_WAIT,
			[CT_TCP_CLOSE] = CT_TCP_CLOSE,
			[CT_TCP_SYN_SENT2] = CT_TCP_SYN_SENT2
		},
		[N3K_MGMT_INIT_TCP_FIN_ACK_SET] = {
			[CT_TCP_NONE] = CT_TCP_NONE,
			[CT_TCP_SYN_SENT] = CT_TCP_SYN_SENT,
			[CT_TCP_SYN_RECV] = CT_TCP_FIN_WAIT,
			[CT_TCP_ESTABLISHED] = CT_TCP_FIN_WAIT,
			[CT_TCP_FIN_WAIT] = CT_TCP_LAST_ACK,
			[CT_TCP_CLOSE_WAIT] = CT_TCP_LAST_ACK,
			[CT_TCP_LAST_ACK] = CT_TCP_LAST_ACK,
			[CT_TCP_TIME_WAIT] = CT_TCP_TIME_WAIT,
			[CT_TCP_CLOSE] = CT_TCP_CLOSE,
			[CT_TCP_SYN_SENT2] = CT_TCP_SYN_SENT2
		},
		[N3K_MGMT_INIT_TCP_ACK_SET] = {
			[CT_TCP_NONE] = CT_TCP_NONE,
			[CT_TCP_SYN_SENT] = CT_TCP_SYN_SENT,
			[CT_TCP_SYN_RECV] = CT_TCP_SYN_RECV,
			[CT_TCP_ESTABLISHED] = CT_TCP_ESTABLISHED,
			[CT_TCP_FIN_WAIT] = CT_TCP_CLOSE_WAIT,
			[CT_TCP_CLOSE_WAIT] = CT_TCP_CLOSE_WAIT,
			[CT_TCP_LAST_ACK] = CT_TCP_TIME_WAIT,
			[CT_TCP_TIME_WAIT] = CT_TCP_TIME_WAIT,
			[CT_TCP_CLOSE] = CT_TCP_CLOSE,
			[CT_TCP_SYN_SENT2] = CT_TCP_SYN_SENT2
		},
		[N3K_MGMT_INIT_TCP_RST_SET] = {
			[CT_TCP_NONE] = CT_TCP_NONE,
			[CT_TCP_SYN_SENT] = CT_TCP_CLOSE,
			[CT_TCP_SYN_RECV] = CT_TCP_CLOSE,
			[CT_TCP_ESTABLISHED] = CT_TCP_CLOSE,
			[CT_TCP_FIN_WAIT] = CT_TCP_CLOSE,
			[CT_TCP_CLOSE_WAIT] = CT_TCP_CLOSE,
			[CT_TCP_LAST_ACK] = CT_TCP_CLOSE,
			[CT_TCP_TIME_WAIT] = CT_TCP_CLOSE,
			[CT_TCP_CLOSE] = CT_TCP_CLOSE,
			[CT_TCP_SYN_SENT2] = CT_TCP_CLOSE
		},
		[N3K_MGMT_INIT_TCP_RST_ACK_SET] = {
			[CT_TCP_NONE] = CT_TCP_NONE,
			[CT_TCP_SYN_SENT] = CT_TCP_CLOSE,
			[CT_TCP_SYN_RECV] = CT_TCP_CLOSE,
			[CT_TCP_ESTABLISHED] = CT_TCP_CLOSE,
			[CT_TCP_FIN_WAIT] = CT_TCP_CLOSE,
			[CT_TCP_CLOSE_WAIT] = CT_TCP_CLOSE,
			[CT_TCP_LAST_ACK] = CT_TCP_CLOSE,
			[CT_TCP_TIME_WAIT] = CT_TCP_CLOSE,
			[CT_TCP_CLOSE] = CT_TCP_CLOSE,
			[CT_TCP_SYN_SENT2] = CT_TCP_CLOSE
		},
		[N3K_MGMT_INIT_TCP_NONE_SET] = {
			[CT_TCP_NONE] = CT_TCP_NONE,
			[CT_TCP_SYN_SENT] = CT_TCP_SYN_SENT,
			[CT_TCP_SYN_RECV] = CT_TCP_SYN_RECV,
			[CT_TCP_ESTABLISHED] = CT_TCP_ESTABLISHED,
			[CT_TCP_FIN_WAIT] = CT_TCP_FIN_WAIT,
			[CT_TCP_CLOSE_WAIT] = CT_TCP_CLOSE_WAIT,
			[CT_TCP_LAST_ACK] = CT_TCP_LAST_ACK,
			[CT_TCP_TIME_WAIT] = CT_TCP_TIME_WAIT,
			[CT_TCP_CLOSE] = CT_TCP_CLOSE,
			[CT_TCP_SYN_SENT2] = CT_TCP_SYN_SENT2
		}
	}
};

struct ct_nontcp_transitions_desc {
	const uint8_t response[CT_NONTCP_MAX];
	const uint8_t request[CT_NONTCP_MAX];
};

/*
 * The flags index is ommitted as we are not expecting any TCP flags
 * coming from such packets.
 */
static const struct ct_nontcp_transitions_desc ct_nontcp_transition = {
	.request = {
		[CT_UDP_INIT] = CT_UDP_INIT,
		[CT_UDP_ESTABLISHED] = CT_UDP_ESTABLISHED,
		[CT_ICMP_INIT] = CT_ICMP_INIT,
		[CT_ICMP_ESTABLISHED] = CT_ICMP_ESTABLISHED,
	},
	.response = {
		[CT_UDP_INIT] = CT_UDP_ESTABLISHED,
		[CT_UDP_ESTABLISHED] = CT_UDP_ESTABLISHED,
		[CT_ICMP_INIT] = CT_ICMP_ESTABLISHED,
		[CT_ICMP_ESTABLISHED] = CT_ICMP_ESTABLISHED,
	}
};

static int
n3k_mgmt_init_gen_cmd_check(struct n3k_mgmt_hw* hw, u32 base, u32 offset, u32 busy_off,
	u32 timeout)
{
	uint32_t data;

	/*
	 * Status 0 means that previous command has completed.
	 * Status 1 means that previous command is still pending.
	 */
	data = csr_read(hw, base, offset);

	while((CT_RD_FIELD(data, busy_off, 1)) && (timeout > 0)) {
		timeout --;
		N3K_MGMT_LOG(DEFAULT, DEBUG, "data:0x%x", data);
		data = csr_read(hw, base, offset);
	}

	/*
	 * if timeout, return an error
	 */
	if (timeout == 0) {
		N3K_MGMT_LOG(DEFAULT, ERR, "Timed out");
		return -ETIMEDOUT;
	}

	return 0;
}

static int
n3k_mgmt_init_ct_upd_transition_tbl(struct n3k_mgmt_hw *hw, u32 i, u32 new_state, u32 old_state)
{
	MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_t cfg;
	int rc;

	rc = n3k_mgmt_init_gen_cmd_check(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE,
		MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG,
		MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_BUSY_SHIFT,
		MKS_FT_CMD_TOUT);
	if (rc)
		return rc;

	cfg.val = csr_read(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE,
			MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG);
	cfg.field.index = i;
	if (new_state == CT_TCP_ESTABLISHED ||
	    new_state == CT_UDP_ESTABLISHED ||
	    new_state == CT_UDP_INIT)
		cfg.field.dis_tstamp_update = IDLE_TIMEOUT_MODE;
	else if (old_state == new_state)
		cfg.field.dis_tstamp_update = HARD_TIMEOUT_MODE;
	else
		cfg.field.dis_tstamp_update = IDLE_TIMEOUT_MODE;
	cfg.field.new_state = new_state;
	cfg.field.operation = N3K_MGMT_INIT_CT_XTION_TBL_WRITE;

	csr_write(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE,
		MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG,
		cfg.val);
	return 0;
}

static int
n3k_mgmt_init_ct_pgm_state_xtion_table(struct n3k_mgmt_hw *hw)
{
	uint32_t ptype, flag_bits, flag, old_state, i, new_state;
	uint32_t num_valid = 0;
	int rc;

	/*
	 * ptype[0], current_state[4:0], SYN, ACK, FIN, RST
	 *
	 * Here is what index would be composed like.
	 *      ptype: state:  flags
	 *         x   xxxxx   xxxx
	 *  bits   9   87654   3210
	 */
	for (ptype = 0; ptype < N3K_MGMT_INIT_CT_PTYPE_MAX; ptype++) {
		for (flag = N3K_MGMT_INIT_TCP_FLAGS_START;
				flag < N3K_MGMT_INIT_TCP_FLAGS_MAX; flag++) {
			flag_bits = n3k_mgmt_init_ct_hw_flags[flag];
			for (old_state = CT_TCP_NONE; old_state < CT_TCP_MAX;
					old_state++) {
				i = hw_state_table_idx(ptype, old_state,
					flag_bits);
				if (ptype == N3K_MGMT_INIT_CT_PTYPE_REQ)
					new_state = ct_tcp_transition.request[flag][old_state];
				else
					new_state = ct_tcp_transition.response[flag][old_state];

				/*
				 * Write new state at the index calculated.
				 */
				rc = n3k_mgmt_init_ct_upd_transition_tbl(hw, i, new_state,
					old_state);
				if (rc)
					return rc;
				num_valid++;
				N3K_MGMT_LOG(DEFAULT, DEBUG, "%ld:ptype:%s flag:%s old_st:%s new_st:%s",
					(long int) num_valid,
					n3k_mgmt_init_ct_ptype_name[ptype],
					n3k_mgmt_init_ct_flag_name[flag],
					n3k_mgmt_init_ct_state_name[old_state],
					n3k_mgmt_init_ct_state_name[new_state]);
			}
		}
	}
	return 0;
}

static int
n3k_mgmt_init_ct_pgm_nontcp_xtion_table(struct n3k_mgmt_hw *hw)
{
	uint32_t ptype, flag_bits, flag, old_state, i, new_state;
	uint32_t num_valid = 0;
	int rc;

	/*
	 * ptype[0], current_state[4:0], SYN, ACK, FIN, RST
	 *
	 * Here is what index would be composed like.
	 *      ptype: state:  flags
	 *         x   xxxxx   xxxx
	 *  bits   9   87654   3210
	 */
	flag = N3K_MGMT_INIT_TCP_NONE_SET;
	flag_bits = n3k_mgmt_init_ct_hw_flags[flag];

	N3K_MGMT_LOG(DEFAULT, DEBUG, "sizeof nontcp tbl:%lu", sizeof(ct_nontcp_transition));

	for (ptype = 0; ptype < N3K_MGMT_INIT_CT_PTYPE_MAX; ptype++) {
		for (old_state = CT_NONTCP_NONE; old_state < CT_NONTCP_MAX;
				old_state++) {
			i = hw_state_table_idx(ptype, old_state, flag_bits);
			if (ptype == N3K_MGMT_INIT_CT_PTYPE_REQ)
				new_state = ct_nontcp_transition.request[old_state];
			else
				new_state = ct_nontcp_transition.response[old_state];

			/*
			 * Write new state at the index calculated.
			 */
			rc = n3k_mgmt_init_ct_upd_transition_tbl(hw, i, new_state, old_state);
			if (rc)
				return rc;

			num_valid++;
			N3K_MGMT_LOG(DEFAULT, DEBUG, "nontcp %ld:ptype:%s flag:%s old:%s new:%s",
				(long int) num_valid,
				n3k_mgmt_init_ct_ptype_name[ptype],
				n3k_mgmt_init_ct_flag_name[flag],
				n3k_mgmt_init_ct_state_name[old_state],
				n3k_mgmt_init_ct_state_name[new_state]);
		}
	}

	return 0;
}

static void
n3k_mgmt_init_ft_write_cmd(struct n3k_mgmt_hw *hw, u32 op, u32 idx)
{
	MKS_FLOW_TRACKER_FT_COMMAND_t ftc;

	memset(&ftc, 0, sizeof(ftc));
	ftc.field.operation = (op);
	ftc.field.index = (idx);
	csr_write(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE, MKS_FLOW_TRACKER_FT_COMMAND, ftc.val);
}

/*
 * Function to initialize the stats block. Firmware will need to wait
 * for flow_tracker block to finish initialization before it can begin
 * to interact. In addition, we will need to make all the stats
 * entries invalid by writing 0s.
 */
int
n3k_mgmt_init_ft_init(struct n3k_mgmt_hw* hw)
{
	int rc;
	uint32_t init_status = 0, max_retries = 10;
	MKS_FLOW_TRACKER_FT_COMMAND_t ft_cmd;

	n3k_mgmt_lock(hw);

	init_status = csr_read(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE,
		MKS_FLOW_TRACKER_FT_STAT);
	while (init_status == 0 && max_retries--) {

		rte_delay_ms(100); // Busy wait 100ms
		init_status = csr_read(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE,
			MKS_FLOW_TRACKER_FT_STAT);

		N3K_MGMT_LOG(DEFAULT, DEBUG, "init status:0x%x", init_status);
	}

	if (init_status == 0 && max_retries == 0) {
		N3K_MGMT_LOG(DEFAULT, ERR, "Quitting! Flow Tracker module not ready!!!");
		return -1;
	}

	N3K_MGMT_LOG(DEFAULT, INFO, "version:0x%x init_status:0x%x",
		csr_read(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE,
			MKS_FLOW_TRACKER_FT_VERSION),
		init_status);

	N3K_MGMT_LOG(DEFAULT, DEBUG, "Flushing stats block.");

	/*
	 * Ideally we don't need to verify the busy bit but it is added
	 * just as a safely check.
	 */
	rc = n3k_mgmt_init_gen_cmd_check(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE,
		MKS_FLOW_TRACKER_FT_COMMAND,
		MKS_FLOW_TRACKER_FT_COMMAND_BUSY_SHIFT,
		10);
	if (rc) {
		N3K_MGMT_LOG(DEFAULT, ERR, "failed to verify busy bit of ft %d", rc);
		return rc;
	}

	n3k_mgmt_init_ft_write_cmd(hw, N3K_MGMT_INIT_FT_OP_FLUSH, 0);

	n3k_mgmt_unlock(hw);

	ft_cmd.val = csr_read(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE,
				MKS_FLOW_TRACKER_FT_COMMAND);
	while (ft_cmd.field.busy == 1) {
		/* FT COMMAND Register busy performing previous cmd. */
		rte_delay_ms(100);
		ft_cmd.val = csr_read(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE,
				MKS_FLOW_TRACKER_FT_COMMAND);
	}

	n3k_mgmt_lock(hw);

	/*
	 * Program the CT transition tables (TCP and nonTCP).
	 */
	n3k_mgmt_init_ct_pgm_state_xtion_table(hw);
	n3k_mgmt_init_ct_pgm_nontcp_xtion_table(hw);

	n3k_mgmt_unlock(hw);

	return 0;
}

/*
 * Enable Chicken Bit for Vxlan source port masking.
 *
 * Vxlan is a uni-directional protocol.
 * Meaning, one can’t get the response flow 5 tuple from request 5 tuple.
 * This is a problem for connection tracking, as we pre-populate response flow, when we setup request flow.
 * So, we need a change in parser.  For vxlan, we need the source port to be ignored/wildcarded.
 */
void
n3k_mgmt_init_msk_l4_src0(struct n3k_mgmt_hw* hw)
{
	uint32_t val;

	val = csr_read(hw, MKS_CHIP_MKS_PARS_BASE, MKS_PARS_PARS_MSKKEY_CFG);
	N3K_MGMT_LOG(DEFAULT, DEBUG, "val:0x%x", val);

	val = val | (1 << MKS_PARS_PARS_MSKKEY_CFG_MSK_L4_SRC0_SHIFT);
	N3K_MGMT_LOG(DEFAULT, DEBUG, "modified val:0x%x", val);
	csr_write(hw, MKS_CHIP_MKS_PARS_BASE, MKS_PARS_PARS_MSKKEY_CFG, val);

	N3K_MGMT_LOG(DEFAULT, INFO, "Vxlan L4 SP Masking in on");

	return;
}
