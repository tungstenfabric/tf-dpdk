/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (c) 2018, Intel Corporation. */

#ifndef _PMD_NET_N3K_MGMT_NIOS_SHM_H_
#define _PMD_NET_N3K_MGMT_NIOS_SHM_H_

#include "n3k_mgmt_hw.h"
#include <stdint.h>

enum n3k_ocb_msg_type {
	N3K_MSG_FREE_RECIRC,	 /* nios -> ifc */
	N3K_MSG_FREE_HNDL,
	N3K_MSG_FREE_RECIRC_ACK, /* ifc -> nios ack */
	N3K_MSG_FREE_HNDL_ACK,
};

/**
 * struct n3k_shm_msg_hdr - basic msg header format
 * @magic: message header signature
 * @len: length of user message, includes this header and payload
 *
 * User message structure should embed this header at the beginning
 * like below for n3k_shm_msgq_send/read_msg() APIs to work.
 *
 * struct n3k_shm_msg_usr {
 *	struct n3k_shm_msg_hdr hdr;
 *	uint32_t msg_data[];
 * }
 */
struct n3k_shm_msg_hdr {
	uint16_t magic;
	uint16_t msg_type;
	uint16_t len;	/* in bytes, for header + payload */
	uint16_t spare;
};

struct n3k_shm_msg {
	struct n3k_shm_msg_hdr hdr;
	uint32_t data[];
};

/* User APIs */

/**
 * struct n3k_shm_ring  - circular ring
 * @base: base address of ring buffer
 * @len: length of ring buffer (in power of 2)
 * @head: index into ring buffer, maintained by writer
 * @tail: index into ring buffer, maintained by reader
 */
struct n3k_shm_ring {
	uint32_t base;
	uint32_t len;
	uint32_t head;
	uint32_t tail;
};

/**
 * struct n3k_mgmt_nios_shm_internal
 * @ring: circular ring struct
 * @reg_addr: region base address
 * @offset: addr-offset of circular ring struct into the region
 */
struct n3k_mgmt_nios_shm_internal {
	struct n3k_shm_ring ring;
};

int
n3k_mgmt_nios_shm_msgq_init(struct n3k_mgmt_hw *hw);
int
n3k_mgmt_nios_shm_msg_recv(struct n3k_mgmt_hw *hw, uint32_t *msg, uint16_t *max_len);

#endif /* _PMD_NET_N3K_MGMT_NIOS_SHM_H_ */
