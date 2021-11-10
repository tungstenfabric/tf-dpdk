/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "n3k-ut.h"

#include "mgmt/n3k_mgmt_dump.h"

static const uint8_t IPv6_ADDR_1[IPv6_ADDR_LEN] = { 0x11, 0x22, 0x33, 0x44,
													0x12, 0x23, 0x34, 0x45,
													0x13, 0x24, 0x35, 0x46,
													0x14, 0x25, 0x36, 0x47 };

static const char IPv6_ADDR_1_STR[] = "1122:3344:1223:3445:1324:3546:1425:3647";

static const uint8_t IPv6_ADDR_2[IPv6_ADDR_LEN] = { 0x01, 0x22, 0x00, 0x00,
													0x00, 0x00, 0x04, 0x05,
													0x03, 0x04, 0x05, 0x06,
													0x04, 0x05, 0x06, 0x07 };

static const char IPv6_ADDR_2_STR[] = "122::405:304:506:405:607";

static const uint8_t IPv6_ADDR_3[IPv6_ADDR_LEN] = { 0x00, 0x00, 0x00, 0x00,
													0x00, 0x00, 0x00, 0x00,
													0x00, 0x00, 0xFF, 0xFF,
													0xC0, 0xA8, 0x00, 0x01 };

static const char IPv6_ADDR_3_STR[] = "::ffff:192.168.0.1";

static const uint8_t IPv6_ADDR_4[IPv6_ADDR_LEN] = { 0x10, 0x00, 0x00, 0x00,
													0x00, 0x00, 0x20, 0x00,
													0x00, 0x00, 0x00, 0xFF,
													0x0C, 0xA8, 0x00, 0x01 };

static const char IPv6_ADDR_4_STR[] = "1000::2000:0:ff:ca8:1";

static void
test_ipv6_str_conversion(void **state __attribute__((unused)))
{
	char buf[40];
	n3k_mgmt_convert_ipv6_to_str(buf, IPv6_ADDR_1);
	assert_memory_equal(buf, IPv6_ADDR_1_STR, sizeof(IPv6_ADDR_1_STR));
}

static void
test_ipv6_str_zeros_to_colons(void **state __attribute__((unused)))
{
	char buf[40];
	n3k_mgmt_convert_ipv6_to_str(buf, IPv6_ADDR_2);
	assert_memory_equal(buf, IPv6_ADDR_2_STR, sizeof(IPv6_ADDR_2_STR));
}

static void
test_ipv6_str_ipv4_representation(void **state __attribute__((unused)))
{
	char buf[40];
	n3k_mgmt_convert_ipv6_to_str(buf, IPv6_ADDR_3);
	assert_memory_equal(buf, IPv6_ADDR_3_STR, sizeof(IPv6_ADDR_3_STR));
}

static void
test_ipv6_str_two_zeros_group(void **state __attribute__((unused)))
{
	char buf[40];
	n3k_mgmt_convert_ipv6_to_str(buf, IPv6_ADDR_4);
	assert_memory_equal(buf, IPv6_ADDR_4_STR, sizeof(IPv6_ADDR_4_STR));
}

const struct CMUnitTest convert_ipv_to_str_tests[] = {
	cmocka_unit_test(test_ipv6_str_conversion),
	cmocka_unit_test(test_ipv6_str_zeros_to_colons),
	cmocka_unit_test(test_ipv6_str_ipv4_representation),
	cmocka_unit_test(test_ipv6_str_two_zeros_group),
};

int test_convert_ipv6_to_str(void)
{
	return cmocka_run_group_tests_name(
		"pmd_ipv6_to_str_conversion_tests", convert_ipv_to_str_tests, NULL, NULL);
}
