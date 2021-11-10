/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_bitwise.h"
#include "n3k_mgmt_conversions.h"
#include "n3k_mgmt_dump.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_lock.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_smac_tbl.h"
#include "n3k_mgmt_init/n3k_mgmt_init_port.h"
#include "regs/n3k_mgmt_regs.h"

#include <errno.h>
#include <rte_byteorder.h>
#include <rte_debug.h>
#include <rte_ether.h>
#include <stdint.h>

typedef MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_0_t SMAC_TBL_REG_LO_t;
typedef MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_0_t SMAC_TBL_REG_HI_t;

struct smac_reg_addr {
	uintptr_t lo;
	uintptr_t hi;
};

static void port_id_to_smac_tbl_reg(uint8_t port_id, struct smac_reg_addr *addr)
{
	addr->lo = MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_0 +
		port_id * (sizeof(SMAC_TBL_REG_LO_t) + sizeof(SMAC_TBL_REG_HI_t));
	addr->hi = addr->lo + sizeof(SMAC_TBL_REG_LO_t);
}

int n3k_mgmt_smac_tbl_set(struct n3k_mgmt_hw *hw, uint8_t port_id,
	const struct rte_ether_addr *addr)
{
	struct smac_reg_addr reg_addr;
	SMAC_TBL_REG_LO_t smac_lo_reg = {{0}};
	SMAC_TBL_REG_HI_t smac_hi_reg = {{0}};
	uint64_t smac_addr;

	if (port_id >= 16) {
		N3K_MGMT_LOG(SMAC, ERR, "Invalid smac_id=%" PRIu8, port_id);
		return -EINVAL;
	}

	port_id_to_smac_tbl_reg(port_id, &reg_addr);

	smac_addr = n3k_mgmt_convert_rte_ether_addr_to_uint(*addr);
	smac_lo_reg.field.smac_addr_lo = BITS_RANGE64(smac_addr, 0, 32);
	smac_hi_reg.field.smac_addr_hi = BITS_RANGE64(smac_addr, 32, 16);

	if (N3K_MGMT_SHOULD_LOG(REGS, DEBUG)) {
		N3K_MGMT_LOG(REGS, DEBUG, "Setting SMAC: smac_id=%" PRIu8 " regs", port_id);
		printf("MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_%u_t = 0x%.8" PRIx32 ", 0b%s\n",
			port_id, smac_lo_reg.val, n3k_mgmt_convert_uint_to_bits_str(smac_lo_reg.val, 32));
		printf("MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_%u_t = 0x%.8" PRIx32 ", 0b%s\n",
			port_id, smac_hi_reg.val, n3k_mgmt_convert_uint_to_bits_str(smac_hi_reg.val, 32));
	}

	if (N3K_MGMT_SHOULD_LOG(SMAC, DEBUG)) {
		N3K_MGMT_LOG(SMAC, DEBUG, "Setting SMAC: smac_id=%" PRIu8, port_id);
		n3k_mgmt_smac_tbl_addr_dump(addr);
	}

	n3k_mgmt_lock(hw);

	csr_write(hw, MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE, reg_addr.lo,
		smac_lo_reg.val);
	csr_write(hw, MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE, reg_addr.hi,
		smac_hi_reg.val);

	n3k_mgmt_unlock(hw);

	N3K_MGMT_LOG(SMAC, INFO, "SMAC %" PRIu8 " set successfully", port_id);

	return 0;
}

int n3k_mgmt_smac_tbl_get(struct n3k_mgmt_hw *hw, uint8_t port_id,
	struct rte_ether_addr *addr)
{
	struct smac_reg_addr reg_addr;
	SMAC_TBL_REG_LO_t smac_lo_reg = {{0}};
	SMAC_TBL_REG_HI_t smac_hi_reg = {{0}};
	uint64_t smac_addr = 0;

	if (port_id >= 16) {
		N3K_MGMT_LOG(SMAC, ERR, "Invalid smac_id=%" PRIu8, port_id);
		return -EINVAL;
	}

	port_id_to_smac_tbl_reg(port_id, &reg_addr);

	n3k_mgmt_lock(hw);

	smac_lo_reg.val = csr_read(hw, MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE, reg_addr.lo);
	smac_hi_reg.val = csr_read(hw, MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE, reg_addr.hi);

	n3k_mgmt_unlock(hw);

	smac_addr = smac_lo_reg.val | ((uint64_t)smac_hi_reg.val << 32);
	*addr = n3k_mgmt_convert_uint_to_rte_ether_addr(smac_addr);

	if (N3K_MGMT_SHOULD_LOG(SMAC, DEBUG)) {
		N3K_MGMT_LOG(SMAC, DEBUG, "Read SMAC: smac_id=%" PRIu8, port_id);
		n3k_mgmt_smac_tbl_addr_dump(addr);
	}

	return 0;
}

int n3k_mgmt_smac_tbl_dump(struct n3k_mgmt_hw *hw, uint8_t port_id)
{
	struct rte_ether_addr addr;
	int ret;

	ret = n3k_mgmt_smac_tbl_get(hw, port_id, &addr);
	if (ret < 0)
		return ret;

	n3k_mgmt_smac_tbl_addr_dump(&addr);
	return 0;
}
