/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2019 Intel Corporation
 */

#ifndef TEST_CRYPTODEV_MIXED_TEST_VECTORS_H_
#define TEST_CRYPTODEV_MIXED_TEST_VECTORS_H_

struct mixed_cipher_auth_test_data {

	enum rte_crypto_auth_algorithm auth_algo;

	struct {
		uint8_t data[64];
		unsigned int len; /* length must be in Bytes */
	} auth_key;

	struct {
		uint8_t data[64];
		unsigned int len; /* length must be in Bytes */
	} auth_iv;

	struct {
		unsigned int len_bits;
		unsigned int offset_bits;
	} auth;

	enum rte_crypto_cipher_algorithm cipher_algo;

	struct {
		uint8_t data[64];
		unsigned int len; /* length must be in Bytes */
	} cipher_key;

	struct {
		uint8_t data[64] __rte_aligned(16);
		unsigned int len; /* length must be in Bytes */
	} cipher_iv;

	struct {
		unsigned int len_bits;
		unsigned int offset_bits;
	} cipher;

	struct {
		uint8_t data[1024];
		unsigned int len_bits;
	} plaintext;

	struct {
		uint8_t data[1024];
		unsigned int len_bits;
	} ciphertext;

	struct {
		uint8_t data[64];
		unsigned int len; /* length must be in Bytes */
		unsigned int offset; /* offset must be in Bytes */
	} digest_enc;

	struct {
		unsigned int len_bits;
	} validDataLen;

	struct {
		unsigned int len_bits;
	} validCipherLen;

	struct {
		unsigned int len_bits;
	} validAuthLen;
};

struct mixed_cipher_auth_test_data auth_aes_cmac_cipher_aes_ctr_test_case_1 = {
	.auth_algo = RTE_CRYPTO_AUTH_AES_CMAC,
	.auth_key = {
		.data = {
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
			0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
		},
		.len = 16,
	},
	.auth_iv = {
		.data = {
			 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
			 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		},
		.len = 16,
	},
	.auth = {
		.len_bits = 124 << 3,
		.offset_bits = 0,
	},
	.cipher_algo = RTE_CRYPTO_CIPHER_AES_CTR,
	.cipher_key = {
		.data = {
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
			0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
		},
		.len = 16,
	},
	.cipher_iv = {
		.data = {
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		},
		.len = 16,
	},
	.cipher = {
		.len_bits = 126 << 3,
		.offset_bits = 2 << 3,
	},
	.plaintext = {
		.data = {
			0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A,
			0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A,
			0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A,
			0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A,
			0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A,
			0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A,
			0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A,
			0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A,
			0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A,
			0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A,
			0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A,
			0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A,
			0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A,
			0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A,
			0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A,
			0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A,
		},
		.len_bits = 128 << 3,
	},
	.ciphertext = {
		.data = {
			0x5A, 0x5A, 0xC9, 0x79, 0xCA, 0xCB, 0xDF, 0x96,
			0x97, 0xA5, 0xB0, 0xFC, 0x56, 0xE9, 0xC4, 0xB4,
			0x32, 0x5F, 0x55, 0x59, 0xCF, 0x1A, 0xA5, 0xB5,
			0x63, 0xAC, 0x94, 0x87, 0x6F, 0xFB, 0x5A, 0x54,
			0x4E, 0xE2, 0x2B, 0x32, 0x04, 0x0C, 0xDF, 0x7D,
			0x58, 0x73, 0xCE, 0x64, 0x71, 0xEA, 0x31, 0x19,
			0xE1, 0x37, 0x9E, 0x89, 0x15, 0x1F, 0xDE, 0x99,
			0x17, 0x80, 0x2A, 0x95, 0xAB, 0x37, 0xDF, 0x01,
			0x7F, 0x3B, 0x17, 0x07, 0xFA, 0x93, 0xC9, 0xC8,
			0x6A, 0x1C, 0xE2, 0x34, 0x2F, 0x33, 0x59, 0xE4,
			0xE2, 0x56, 0x1A, 0x23, 0xCC, 0xCE, 0x29, 0x33,
			0xC7, 0xAF, 0x92, 0x28, 0x86, 0x5B, 0x91, 0x5B,
			0x0E, 0x4E, 0xFB, 0x3B, 0x9D, 0x9F, 0xDF, 0x16,
			0x79, 0x90, 0x86, 0xBA, 0x70, 0xD9, 0xF7, 0x9B,
			0xEC, 0x46, 0xA9, 0x98, 0x5A, 0xF1, 0x5A, 0x05,
			0x3E, 0xD9, 0xE5, 0x03, 0x5D, 0x41, 0xB4, 0xDF
		},
		.len_bits = 128 << 3,
	},
	.digest_enc = {
		.data = {
			0x5D, 0x41, 0xB4, 0xDF,
		},
		.len = 4,
		.offset = 124,
	},
	.validDataLen = {
		.len_bits = 128 << 3,
	},
	.validCipherLen = {
		.len_bits = 126 << 3,
	},
	.validAuthLen = {
		.len_bits = 124 << 3,
	}
};

#endif /* TEST_CRYPTODEV_MIXED_TEST_VECTORS_H_ */
