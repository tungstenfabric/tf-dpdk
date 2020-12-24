/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _N3K_MGMT_INIT_OVS_MGMT_HW_H_
#define _N3K_MGMT_INIT_OVS_MGMT_HW_H_

#include "n3k_mgmt_init_flow.h"

#define REG_MAX_SIZE 256

#define n3k_mgmt_init_insert_field(d, f, v) do { u32 *_d = &(d); \
		u32 _m = GENMASK((f##_W + f##_S - 1), f##_S); \
		*_d = (*_d & ~_m) | (((v) << f##_S) & _m); \
		} while (0)

/**
 * Accommodating timeout values in lesser bits in register
 * E.g. Value 12 will be stored in reg corresponding to N3K_MGMT_INIT_TCP_CT_STATE_1
 * But when used it will be scaled up by factor x 10 seconds to give 120 seconds
 * equivalent to 2 minutes
 */
#define N3K_MGMT_INIT_NON_CT_TIMEOUT		1	/* 1*10 Seconds = 10 Seconds */
#define N3K_MGMT_INIT_TCP_CT_STATE_1		12	/* 12*10 Seconds = 2 Minutes */
#define N3K_MGMT_INIT_TCP_CT_STATE_2		6	/* 6*10 Seconds = 1 Minute */
#define N3K_MGMT_INIT_TCP_CT_STATE_3		60	/* 2 HRS SCALE 60*2 = 5 days */
#define N3K_MGMT_INIT_TCP_CT_STATE_4		12	/* 12*10 Seconds = 2 Minutes */
#define N3K_MGMT_INIT_TCP_CT_STATE_5		6	/* 6*10 Seconds = 1 Minute */
#define N3K_MGMT_INIT_TCP_CT_STATE_6		3	/* 3*10 Seconds = 30 Seconds */
#define N3K_MGMT_INIT_TCP_CT_STATE_7		12	/* 12*10 Seconds = 2 Minutes */
#define N3K_MGMT_INIT_TCP_CT_STATE_8		1	/* 1*10 Seconds = 10 Seconds */
#define N3K_MGMT_INIT_TCP_CT_STATE_9		12	/* 12*10 Seconds = 2 Minutes */
#define N3K_MGMT_INIT_UDP_CT_STATE_INIT		3	/* 3*10 Seconds = 30 Seconds */
#define N3K_MGMT_INIT_UDP_CT_STATE_ESTABLISHED	12	/* 12*10 Seconds = 2 Minutes */
#define N3K_MGMT_INIT_ICMP_CT_STATE_INIT		3	/* 3*10 Seconds = 30 Seconds */
#define N3K_MGMT_INIT_ICMP_CT_STATE_ESTABLISHED	12	/* 12*10 Seconds = 2 Minutes */

#endif //_N3K_MGMT_INIT_OVS_MGMT_HW_H_
