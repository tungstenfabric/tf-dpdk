/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_bitwise.h"
#include "n3k_mgmt_flr_pkts.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_ports.h"
#include "n3k_mgmt_regs.h"

#include "../rte_pmd_n3k_malloc.h"

static int
get_interface_id(uint32_t start_reg_addr, uint32_t end_reg_addr)
{
	if (MKS_CHIP_LKUP_BASE <= start_reg_addr &&
		start_reg_addr < MKS_CHIP_LKUP_BASE + MKS_CHIP_LKUP_SIZE &&
		MKS_CHIP_LKUP_BASE <= end_reg_addr &&
		end_reg_addr < MKS_CHIP_LKUP_BASE + MKS_CHIP_LKUP_SIZE)
		return 0;
	else if (MKS_CHIP_MKS_FLOW_TRACKER_BASE <= start_reg_addr &&
		start_reg_addr < MKS_CHIP_MKS_FLOW_TRACKER_BASE + MKS_CHIP_MKS_FLOW_TRACKER_SIZE &&
		MKS_CHIP_MKS_FLOW_TRACKER_BASE <= end_reg_addr &&
		end_reg_addr < MKS_CHIP_MKS_FLOW_TRACKER_BASE + MKS_CHIP_MKS_FLOW_TRACKER_SIZE)
		return 1;
	return -EINVAL;
}

static void
fill_block_header(struct n3k_mgmt_flr_block_header *block_hdr,
	uint16_t block_data_size,
	uint32_t start_reg_addr,
	enum n3k_mgmt_flr_block_header_cmd_type cmd_type)
{
	uint32_t end_reg_addr = start_reg_addr + block_data_size - 4;
	int interface_id = get_interface_id(start_reg_addr, end_reg_addr);

	// TODO(n3k): should return error instead of assert
	RTE_ASSERT(interface_id >= 0 && "invalid address");
	RTE_ASSERT(block_data_size % 4 == 0);
	RTE_ASSERT(start_reg_addr % 4 == 0);

	memset(block_hdr, 0, sizeof(*block_hdr));

	block_hdr->burst_cnt = block_data_size / 4 - 1;
	block_hdr->length = rte_cpu_to_be_16(sizeof(*block_hdr) + block_data_size);
	block_hdr->cmd_desc = rte_cpu_to_be_32((cmd_type << 31) |
		BITS_RANGE32(interface_id, 0, 4) << 26 |
		BITS_RANGE32(start_reg_addr, 0, 26));
}

inline static void *
get_pkt_data_ptr(struct rte_mbuf *pkt, uint16_t offset)
{
	RTE_ASSERT(pkt->data_len > offset);
	return ((uint8_t *)pkt->buf_addr + pkt->data_off + offset);
}

inline static struct n3k_mgmt_flr_config_header *
get_config_header_ptr(struct rte_mbuf *pkt)
{
	return get_pkt_data_ptr(pkt, sizeof(struct rte_ether_hdr));
}

inline static void
add_blocks_number(struct rte_mbuf *pkt, uint8_t num)
{
	struct n3k_mgmt_flr_config_header *config_hdr = get_config_header_ptr(pkt);
	config_hdr->blocks_num += num;
}

inline static void
add_payload_length(struct rte_mbuf *pkt, uint16_t len)
{
	struct n3k_mgmt_flr_config_header *config_hdr = get_config_header_ptr(pkt);
	RTE_ASSERT(rte_be_to_cpu_16(config_hdr->payload_len) + len <=
		N3K_MGMT_FLR_CONFIG_HEADER_MAX_PAYLOAD_LEN);
	config_hdr->payload_len = rte_cpu_to_be_16(
		rte_be_to_cpu_16(config_hdr->payload_len) + len);
}

inline static void
subtract_blocks_number(struct rte_mbuf *pkt, uint8_t num)
{
	struct n3k_mgmt_flr_config_header *config_hdr = get_config_header_ptr(pkt);
	config_hdr->blocks_num -= num;
}

inline static void
subtract_payload_length(struct rte_mbuf *pkt, uint16_t len)
{
	struct n3k_mgmt_flr_config_header *config_hdr = get_config_header_ptr(pkt);
	RTE_ASSERT(rte_be_to_cpu_16(config_hdr->payload_len) >= len);
	config_hdr->payload_len = rte_cpu_to_be_16(
		rte_be_to_cpu_16(config_hdr->payload_len) - len);
}

int
n3k_mgmt_flr_add_write_block(struct rte_mbuf *pkt, uint32_t start_reg_addr,
	const void *regs, size_t regs_size)
{
	uint16_t allocated_space;
	char *added_data_ptr;
	struct n3k_mgmt_flr_block_header *hdr_ptr;
	void *data_ptr;

	N3K_MGMT_FLR_PKTS_LOG(DEBUG,
		"Adding write block to packet: start_reg_addr=%" PRIx32 ", regs_size=%zu",
		start_reg_addr, regs_size);

	allocated_space = sizeof(struct n3k_mgmt_flr_block_header) + regs_size;
	added_data_ptr = rte_pktmbuf_append(pkt, allocated_space);
	if (added_data_ptr == NULL) {
		N3K_MGMT_FLR_PKTS_LOG(ERR, "Error in resizing packet");
		return -EINVAL;
	}
	hdr_ptr = (struct n3k_mgmt_flr_block_header *)added_data_ptr;
	data_ptr = added_data_ptr + sizeof(struct n3k_mgmt_flr_block_header);

	fill_block_header(hdr_ptr, regs_size, start_reg_addr,
		N3K_MGMT_FLR_BLOCK_HEADER_CMD_TYPE_WRITE);
	memcpy_bswap32(data_ptr, regs, regs_size);

	add_payload_length(pkt, allocated_space);
	add_blocks_number(pkt, 1);
	return 0;
}

int
n3k_mgmt_flr_add_read_block(struct rte_mbuf *pkt, uint32_t start_reg_addr,
	size_t regs_size)
{
	uint16_t allocated_space;
	char *added_data_ptr;
	struct n3k_mgmt_flr_block_header *hdr_ptr;

	N3K_MGMT_FLR_PKTS_LOG(DEBUG,
		"Adding read block to packet: start_reg_addr=%" PRIx32 ", regs_size=%zu",
		start_reg_addr, regs_size);

	allocated_space = sizeof(struct n3k_mgmt_flr_block_header) + regs_size;
	added_data_ptr = rte_pktmbuf_append(pkt, allocated_space);
	if (added_data_ptr == NULL) {
		N3K_MGMT_FLR_PKTS_LOG(ERR, "Error in resizing packet");
		return -EINVAL;
	}
	hdr_ptr = (struct n3k_mgmt_flr_block_header *)added_data_ptr;

	fill_block_header(hdr_ptr, regs_size, start_reg_addr,
		N3K_MGMT_FLR_BLOCK_HEADER_CMD_TYPE_READ);

	add_payload_length(pkt, allocated_space);
	add_blocks_number(pkt, 1);
	return 0;
}

int
n3k_mgmt_flr_add_icv(struct rte_mbuf *pkt)
{
	RTE_SET_USED(pkt);
	const uint8_t icv_size = 16;
	char *added_data_ptr;

	N3K_MGMT_FLR_PKTS_LOG(DEBUG, "Adding icv to packet");

	added_data_ptr = rte_pktmbuf_append(pkt, icv_size);
	if (added_data_ptr == NULL) {
		N3K_MGMT_FLR_PKTS_LOG(ERR, "Error in resizing packet");
		return -EINVAL;
	}

	// TODO(n3k): Add icv data
	memset(added_data_ptr, 0xff, icv_size);

	return 0;
}

int
n3k_mgmt_flr_remove_block(struct rte_mbuf *pkt, size_t data_size)
{
	uint16_t trimmed_space = data_size + sizeof(struct n3k_mgmt_flr_block_header);
	int ret;

	ret = rte_pktmbuf_trim(pkt, trimmed_space);
	if (ret == -1) {
		N3K_MGMT_FLR_PKTS_LOG(ERR, "Error in resizing packet");
		return -EINVAL;
	}

	subtract_payload_length(pkt, trimmed_space);
	subtract_blocks_number(pkt, 1);
	return 0;
}

int
n3k_mgmt_flr_init_pkt(struct rte_mbuf *pkt, uint16_t pkt_id)
{
	struct rte_ether_hdr *eth_hdr;
	struct n3k_mgmt_flr_config_header *flr_hdr;
	char *added_data_ptr = rte_pktmbuf_append(pkt, sizeof(*eth_hdr) + sizeof(*flr_hdr));

	N3K_MGMT_FLR_PKTS_LOG(DEBUG, "Initializing new packet: pkt_id=%" PRIu16, pkt_id);

	if (added_data_ptr == NULL) {
		N3K_MGMT_FLR_PKTS_LOG(ERR, "Error in resizing packet");
		return -EINVAL;
	}
	eth_hdr = (struct rte_ether_hdr *)added_data_ptr;
	flr_hdr = (struct n3k_mgmt_flr_config_header *)(added_data_ptr + sizeof(*eth_hdr));

	memset(added_data_ptr, 0, sizeof(*eth_hdr) + sizeof(*flr_hdr));

	eth_hdr->ether_type = rte_cpu_to_be_16(N3K_MGMT_FLR_CONFIG_HEADER_ETHER_TYPE);
	eth_hdr->s_addr = (struct rte_ether_addr) {
		.addr_bytes = {0x02, 0x02, 0x03, 0x04, 0x05, 0x06}
	};
	eth_hdr->d_addr = (struct rte_ether_addr) {
		.addr_bytes = {0x12, 0x12, 0x13, 0x14, 0x15, 0x16}
	};

	flr_hdr->blocks_num = -1;
	flr_hdr->meta_type = N3K_MGMT_FLR_CONFIG_HEADER_META_TYPE_CONFIG_PKT;
	flr_hdr->pkt_type = N3K_MGMT_FLR_CONFIG_HEADER_PKT_TYPE_REQUEST;
	flr_hdr->id = rte_cpu_to_be_16(pkt_id);

	return 0;
}

inline static struct n3k_mgmt_flr_config_header *
get_config_header_ptr_raw(void *pkt_data)
{
	return (struct n3k_mgmt_flr_config_header *)
		((uint8_t *)pkt_data + sizeof(struct rte_ether_hdr));
}

inline static struct n3k_mgmt_flr_block_header *
get_block_header_ptr_raw(void *pkt_data, size_t offset)
{
	return (struct n3k_mgmt_flr_block_header *)((uint8_t *)pkt_data + offset);
}

int n3k_mgmt_flr_parse_config_pkt_raw(void *pkt_data, uint16_t pkt_size,
	struct n3k_mgmt_flr_parsed_pkt *parsed_pkt)
{
	struct n3k_mgmt_flr_config_header *flr_hdr;
	const uint16_t hdrs_size = sizeof(struct rte_ether_hdr) + sizeof(*flr_hdr);

	if (pkt_size < hdrs_size) {
		N3K_MGMT_FLR_PKTS_LOG(ERR,
			"Packet is too small. Probably not a configuration packet.");
		return -EINVAL;
	}

	flr_hdr = get_config_header_ptr_raw(pkt_data);

	if (flr_hdr->blocks_num > N3K_MGMT_FLR_CONFIG_HEADER_MAX_BLOCKS_NUM) {
		N3K_MGMT_FLR_PKTS_LOG(ERR, "Invalid number of blocks.");
		return -EINVAL;
	}

	uint16_t payload_len = rte_be_to_cpu_16(flr_hdr->payload_len);
	if (payload_len > N3K_MGMT_FLR_CONFIG_HEADER_MAX_PAYLOAD_LEN) {
		N3K_MGMT_FLR_PKTS_LOG(ERR, "Invalid payload length.");
		return -EINVAL;
	}

	memset(parsed_pkt, 0, sizeof(*parsed_pkt));

	parsed_pkt->error = flr_hdr->error;
	parsed_pkt->pkt_type = flr_hdr->pkt_type;
	parsed_pkt->id = rte_be_to_cpu_16(flr_hdr->id);
	parsed_pkt->blocks_num = flr_hdr->blocks_num + 1;
	uint8_t i;
	uint16_t block_off = hdrs_size;
	for (i = 0; i < parsed_pkt->blocks_num; ++i) {
		struct n3k_mgmt_flr_block_header *block_hdr;
		uint16_t next_block_off;
		uint32_t cmd_desc;
		uint16_t block_length;
		int interface_id;
		uint32_t end_reg_addr;

		if (block_off + sizeof(*block_hdr) > pkt_size) {
			N3K_MGMT_FLR_PKTS_LOG(ERR, "Invalid packet size.");
			return -EINVAL;
		}

		block_hdr = get_block_header_ptr_raw(pkt_data, block_off);
		cmd_desc = rte_be_to_cpu_32(block_hdr->cmd_desc);
		parsed_pkt->blocks[i].cmd_type = cmd_desc >> 31;
		parsed_pkt->blocks[i].start_reg_addr = BITS_RANGE32(cmd_desc, 0, 26);
		interface_id = BITS_RANGE32(cmd_desc, 26, 4);
		parsed_pkt->blocks[i].burst_cnt = block_hdr->burst_cnt + 1;
		end_reg_addr = parsed_pkt->blocks[i].start_reg_addr +
			(parsed_pkt->blocks[i].burst_cnt - 1) * 4;
		parsed_pkt->blocks[i].error_code = block_hdr->error_code;
		block_length = rte_be_to_cpu_16(block_hdr->length);
		next_block_off = block_off + block_length;

		if (next_block_off > pkt_size ||
			next_block_off - hdrs_size > payload_len) {
			N3K_MGMT_FLR_PKTS_LOG(ERR, "Invalid packet or block size.");
			return -EINVAL;
		}

		if (parsed_pkt->blocks[i].start_reg_addr % 4) {
			N3K_MGMT_FLR_PKTS_LOG(ERR, "Start register address not aligned to 4.");
			return -EINVAL;
		}

		if (interface_id != get_interface_id(parsed_pkt->blocks[i].start_reg_addr, end_reg_addr)) {
			N3K_MGMT_FLR_PKTS_LOG(ERR, "Invalid interface id or address range.");
			return -EINVAL;
		}

		if (parsed_pkt->blocks[i].cmd_type != N3K_MGMT_FLR_BLOCK_HEADER_CMD_TYPE_READ &&
			parsed_pkt->blocks[i].cmd_type != N3K_MGMT_FLR_BLOCK_HEADER_CMD_TYPE_WRITE) {
			N3K_MGMT_FLR_PKTS_LOG(ERR, "Invalid command type.");
			return -EINVAL;
		}

		if (parsed_pkt->blocks[i].burst_cnt * 4 + sizeof(*block_hdr) != block_length) {
			N3K_MGMT_FLR_PKTS_LOG(ERR, "Invalid packet data size.");
			return -EINVAL;
		}
		parsed_pkt->blocks[i].data = (uint32_t *)
			((uint8_t *)block_hdr + sizeof(*block_hdr));

		block_off = next_block_off;
	}

	if (block_off - hdrs_size != payload_len) {
		N3K_MGMT_FLR_PKTS_LOG(ERR, "Invalid sum of blocks size.");
		return -EINVAL;
	}

	return 0;
}

int
n3k_mgmt_flr_parse_config_pkt(struct rte_mbuf *pkt,
	struct n3k_mgmt_flr_parsed_pkt *parsed_pkt)
{
	return n3k_mgmt_flr_parse_config_pkt_raw(
		(uint8_t *)pkt->buf_addr + pkt->data_off,
		pkt->data_len, parsed_pkt);
}


bool n3k_mgmt_flr_is_config_pkt(const struct rte_mbuf *pkt)
{
	const struct rte_ether_hdr *eth_hdr;
	if (pkt->data_len < sizeof(struct rte_ether_hdr *))
