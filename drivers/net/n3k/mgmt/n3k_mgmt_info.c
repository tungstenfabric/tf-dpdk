/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include <rte_pmd_n3k_mp.h>

#include "n3k_mgmt_info.h"
#include "regs/n3k_mgmt_regs.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_mp_callbacks.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_ethdev.h"

static int
n3k_mgmt_info_get(
	struct n3k_mgmt_hw *hw,
	struct n3k_mgmt_mp_register_request *request,
	uint32_t *value)
{
	int ret = n3k_mgmt_mp_fill_register_response(hw, request, value);
	if (ret) {
		N3K_MGMT_LOG(DEFAULT, ERR, "Register response failed: %d", ret);
		return ret;
	}

	return 0;
}

int
n3k_mgmt_info_fw_version_get(struct rte_eth_dev *dev,
	char *fw_version, size_t fw_size)
{
	union {
		uint32_t raw;
		struct {
			uint8_t revision;
			uint8_t version;
			uint8_t milestone;
			uint8_t ovs:4;
			uint8_t hcl_id:4;
		};
	} build_version;
	union {
		uint32_t raw;
		struct {
			uint8_t day;
			uint8_t month;
			uint16_t year;
		};
	} build_date;
	int ret = -1;
	struct n3k_mgmt_mp_register_request req = {
		.base = MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE,
		.offset = MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_VERSION_REDU,
	};
	struct n3k_mgmt_hw *hw = n3k_mgmt_get_hw_from_dev_private(dev);
	if (hw == NULL) {
		N3K_MGMT_LOG(DEFAULT, ERR, "Management device fetch failed: %d", ret);
		return -ENODEV;
	}

	ret = n3k_mgmt_info_get(hw, &req, &build_version.raw);
	if (ret) {
		N3K_MGMT_LOG(DEFAULT, ERR, "Build version fetch failed: %d", ret);
		return -ENOTSUP;
	}

	req = (struct n3k_mgmt_mp_register_request) {
		.base = MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE,
		.offset = MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_CMPL_DATE_REDU,
	};

	ret = n3k_mgmt_info_get(hw, &req, &build_date.raw);
	if (ret) {
		N3K_MGMT_LOG(DEFAULT, ERR, "Build date fetch failed: %d", ret);
		return -ENOTSUP;
	}

	printf(" \"FPGA_BUILD_VER\": { \"version\": %x, \"revision\": %x, \"milestone\": %x, \"HCL\": %s, \"OVS\": %x },\n",
		build_version.version, build_version.revision, build_version.milestone,
		build_version.hcl_id == 0x0C ? "true" : "false", build_version.ovs);

	printf(" \"FPGA_BUILD_DATE\": { \"day\": %x, \"month\": %x, \"year\": %x },\n",
		build_date.day, build_date.month, build_date.year);

	ret = snprintf(fw_version, fw_size, "%u", build_version.raw);
	if (ret < 0 || (size_t)ret >= fw_size) {
		N3K_MGMT_LOG(DEFAULT, ERR, "Invalid firmware version string size: %d", ret);
		return ret;
	}

	return 0;
}
