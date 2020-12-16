// SPDX-License-Identifier: GPL-2.0
/* Copyright (c) 2018, Intel Corporation. */
#include <rte_common.h>
#include "ifc/ifc_port.h"
#include "n3k_mgmt_nios_shm.h"
#include "n3k_mgmt_regs.h"

#define N3K_OCB_FIFO_DESC 0x00000400U /* msgQ descriptors */

#define N3K_SHM_MSG_POISON 0xbadc

#define N3K_SHM_RING_OFFSET MKS_MGMT_ONCHIP_SHARED_BUFFER(N3K_OCB_FIFO_DESC / 4)

static int
n3k_shm_ring_empty(struct n3k_shm_ring *ring)
{
	return ring->head == ring->tail;
}

static void
n3k_mgmt_nios_shm_read_mem(struct n3k_mgmt_hw *hw,
	      uint32_t offset,
	      uint32_t* out,
	      uint32_t len)
{
	uint32_t i;
	for (i = 0; i < len / sizeof(uint32_t); i++) {
		out[i] =
		  csr_read(hw, MKS_CHIP_MKS_MGMT_BASE, offset + i * sizeof(uint32_t));
	}
}

static void
n3k_mgmt_nios_shm_ring_reader(struct n3k_mgmt_hw *hw, uint32_t* msg, uint32_t len)
{
	struct n3k_mgmt_nios_shm_internal* msgq = hw->nios_internal;
	uint32_t rem_len;
	uint32_t rd_len;
	uint32_t c;
	uint32_t x;

	if (msgq->ring.tail < msgq->ring.head) {
		c = msgq->ring.head - msgq->ring.tail;
		x = 0;
	}
	else {
		c = msgq->ring.len - msgq->ring.tail;
		x = msgq->ring.head;
	}

	rd_len = RTE_MIN(c, len);

	n3k_mgmt_nios_shm_read_mem(hw, msgq->ring.base + msgq->ring.tail, msg, rd_len);

	rem_len = RTE_MIN(len - rd_len, x);

	if (rem_len) {
		uint32_t *m = msg + rd_len / sizeof(uint32_t);
		n3k_mgmt_nios_shm_read_mem(hw, msgq->ring.base, m, rem_len);
	}
}

static int
n3k_mgmt_nios_shm_read_msg(struct n3k_mgmt_hw* hw, uint32_t* msg, uint16_t* len)
{
	struct n3k_mgmt_nios_shm_internal* msgq = hw->nios_internal;
	struct n3k_shm_msg_hdr msgh;

	if (*len < sizeof(msgh))
		return -EINVAL;

	if (n3k_shm_ring_empty(&msgq->ring))
		return -ENOENT;

	/* read msg header */
	n3k_mgmt_nios_shm_ring_reader(hw, (uint32_t *)&msgh, sizeof(msgh));

	if (msgh.magic != N3K_SHM_MSG_POISON || msgh.len > *len)
		return -EINVAL;

	/* read msg */
	n3k_mgmt_nios_shm_ring_reader(hw, msg, msgh.len);

	*len = msgh.len;
	msgq->ring.tail = (msgq->ring.tail + *len) & (msgq->ring.len - 1);

	return 0;
}

/**
 * n3k_mgmt_nios_shm_msgq_init() - initialize msg_q structure
 * @hw: pointer to n3k mgmt hw struct
 *
 * returns int
 */
int
n3k_mgmt_nios_shm_msgq_init(struct n3k_mgmt_hw *hw)
{
	struct n3k_mgmt_nios_shm_internal* msgq;

	if (!hw || !hw->pci_resource_start || !hw->nios_internal)
		return -EINVAL;

	msgq = hw->nios_internal;

	n3k_mgmt_nios_shm_read_mem(
	  hw, N3K_SHM_RING_OFFSET, (uint32_t*)&msgq->ring, sizeof(msgq->ring));
	return 0;
}

/**
 * n3k_mgmt_nios_shm_msg_recv() - read one msg from msg_q
 * @hw: pointer to n3k mgmt hw struct
 * @msg: msg buffer where msg data would be copied
 * @len: pointer to length of msg buffer
 *
 * returns int
 */
int
n3k_mgmt_nios_shm_msg_recv(struct n3k_mgmt_hw* hw,
		       uint32_t* msg,
		       uint16_t* len)
{
	struct n3k_mgmt_nios_shm_internal* msgq = hw->nios_internal;
	int ret;

	if (!msgq || !msg)
		return -EINVAL;

	msgq->ring.head =
	  csr_read(hw,
		   MKS_CHIP_MKS_MGMT_BASE,
		   N3K_SHM_RING_OFFSET + offsetof(struct n3k_shm_ring, head));

	ret = n3k_mgmt_nios_shm_read_msg(hw, msg, len);
	if (ret)
		return ret;

	csr_write(hw,
		  MKS_CHIP_MKS_MGMT_BASE,
		  N3K_SHM_RING_OFFSET + offsetof(struct n3k_shm_ring, tail),
		  msgq->ring.tail);
	return 0;
}
