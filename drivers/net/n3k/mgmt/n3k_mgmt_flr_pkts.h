/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_MGMT_FLR_PKTS_H_
#define _PMD_NET_N3K_MGMT_FLR_PKTS_H_

#include <rte_ether.h>
#include <rte_mbuf.h>

#include <stdbool.h>

// Config header

struct n3k_mgmt_flr_config_header {
	uint8_t pkt_type : 1;    // 0 - response, 1 - request
	uint8_t error : 1;
	uint8_t queue_full : 1;
	uint8_t queue_free_entries : 4;
	uint8_t meta_type : 1;
	uint8_t blocks_num : 4;
	uint8_t version : 4;
	uint16_t id;
	uint16_t payload_len;
} __attribute__((packed));

#define N3K_MGMT_FLR_CONFIG_HEADER_ETHER_TYPE 0x8771
#define N3K_MGMT_FLR_CONFIG_HEADER_MAX_BLOCKS_NUM 15
#define N3K_MGMT_FLR_CONFIG_HEADER_MAX_PAYLOAD_LEN 1482

enum n3k_mgmt_flr_config_header_pkt_type {
	N3K_MGMT_FLR_CONFIG_HEADER_PKT_TYPE_RESPONSE = 0,
	N3K_MGMT_FLR_CONFIG_HEADER_PKT_TYPE_REQUEST = 1,
};

enum n3k_mgmt_flr_config_header_meta_type {
	N3K_MGMT_FLR_CONFIG_HEADER_META_TYPE_REGULAR_PKT = 0,
	N3K_MGMT_FLR_CONFIG_HEADER_META_TYPE_CONFIG_PKT = 1,
};

// Block header

struct n3k_mgmt_flr_block_header {
	uint16_t length;
	uint8_t burst_cnt;
	uint8_t error_code;
	uint32_t cmd_desc; // [25:0]  - starting register address
	                   // [29:26] - interface id
	                   // [30]    - is internal module
	                   // [31]    - cmd_type (0 - read, 1 - write)
} __attribute__((packed));

enum n3k_mgmt_flr_block_header_cmd_type {
	N3K_MGMT_FLR_BLOCK_HEADER_CMD_TYPE_READ = 0,
	N3K_MGMT_FLR_BLOCK_HEADER_CMD_TYPE_WRITE = 1,
};

// Packet creation and modification

int n3k_mgmt_flr_add_write_block(struct rte_mbuf *pkt, uint32_t start_reg_addr,
	const void *regs, size_t regs_size);
int n3k_mgmt_flr_add_read_block(struct rte_mbuf *pkt, uint32_t start_reg_addr,
	size_t regs_size);
int n3k_mgmt_flr_add_icv(struct rte_mbuf *pkt);

int n3k_mgmt_flr_remove_block(struct rte_mbuf *pkt, size_t data_size);

int n3k_mgmt_flr_init_pkt(struct rte_mbuf *pkt, uint16_t pkt_id);

// Parsed packets

struct n3k_mgmt_flr_parsed_block {
	enum n3k_mgmt_flr_block_header_cmd_type cmd_type;
	uint8_t error_code;
	uint8_t burst_cnt;
	uint32_t start_reg_addr;
	uint32_t *data;
};

struct n3k_mgmt_flr_parsed_pkt {
	uint16_t id;
	uint8_t blocks_num;
	bool error; // indicates if any of the parsed blocks ended with an error
	enum n3k_mgmt_flr_config_header_pkt_type pkt_type;
	struct n3k_mgmt_flr_parsed_block blocks[N3K_MGMT_FLR_CONFIG_HEADER_MAX_BLOCKS_NUM];
};

/* Parse config packet and store the result in parsed_pkt struct.
 * Pointers to data in n3k_mgmt_flr_parsed_block fields refer to pkt internal
 * buffers. That means parsed_pkt lifetime shouldn't be shorter than pkt
 * lifetime.
 *
 * Returns 0 if packet is properly constructed and successfully parsed.
 * Returns negative value in case of error in parsing.
 */
int n3k_mgmt_flr_parse_config_pkt(struct rte_mbuf *pkt,
	struct n3k_mgmt_flr_parsed_pkt *parsed_pkt);

int n3k_mgmt_flr_parse_config_pkt_raw(void *pkt_data, uint16_t pkt_size,
	struct n3k_mgmt_flr_parsed_pkt *parsed_pkt);


/* Performs only some basic check.
 * Checks if ether_hdr.ether_type == N3K_MGMT_FLR_CONFIG_HEADER_ETHER_TYPE
 */
bool n3k_mgmt_flr_is_config_pkt(const struct rte_mbuf *pkt);

#endif /* _PMD_NET_N3K_MGMT_FLR_PKTS_H_ */
