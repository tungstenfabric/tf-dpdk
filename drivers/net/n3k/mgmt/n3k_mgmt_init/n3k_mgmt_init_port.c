/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_init_port.h"
#include "../n3k_mgmt_hw.h"
#include "../n3k_mgmt_log.h"
#include "../n3k_mgmt_regs.h"

static const char *get_reg_base_str(size_t base_addr)
{
	switch (base_addr) {
	case MKS_CHIP_MKS_MGMT_BASE: return "MKS_MGMT";
	case MKS_CHIP_MKS_FLOW_TRACKER_BASE: return "MKS_FLOW_TRACKER";
	case MKS_CHIP_MKS_METERS_BASE: return "MKS_METERS";
	case MKS_CHIP_EM_LKUP_BASE: return "EM_LKUP";
	case MKS_CHIP_LKUP_BASE: return "LKUP";
	case MKS_CHIP_MKS_CHIP_TOP_CPU_REGS_BASE: return "MKS_CHIP_TOP_CPU_REGS";
	case MKS_CHIP_MKS_LKUP_BASE: return "MKS_LKUP";
	case MKS_CHIP_MKS_PM_BASE: return "MKS_PM";
	case MKS_CHIP_MKS_TXQ_BASE: return "MKS_TXQ";
	case MKS_CHIP_MKS_TXSCH_BASE: return "MKS_TXSCH";
	case MKS_CHIP_MKS_PARS_BASE: return "MKS_PARS";
	case MKS_CHIP_MKS_MOD_BASE: return "MKS_MOD";
	case MKS_CHIP_MKS_VIOSHM_BASE: return "MKS_VIOSHM";
	case MKS_CHIP_MKS_FT_FIFO_BASE: return "MKS_FT_FIFO";
	case MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE: return "MKS_VPORTLKUP_TUNNEL_TABLE";
	case MKS_CHIP_VIRTIO_DMA_CSR_BASE: return "VIRTIO_DMA_CSR";
	default: return "UNKNOWN";
	}
}

#define LOG_REGISTER_OP(base, offset, value) \
	N3K_MGMT_REGS_LOG(DEBUG, \
		"reg addr: 0x%04" PRIx32 ".%04" PRIx32 "(base:%s), reg val: 0x%08" PRIx32, \
		(uint32_t)(base + offset) >> 16, (uint32_t)(base + offset) & 0xffff, get_reg_base_str(base), (uint32_t)value)

uint32_t csr_read(struct n3k_mgmt_hw *hw, size_t base_module, size_t offset)
{
	char* addr = (char*)(hw->pci_resource_start);
	uint32_t val = N3K_MGMT_READ_REG32(addr + base_module + offset);
	if (base_module != MKS_CHIP_MKS_MGMT_BASE || offset < 0x2200) // TODO(n3k): tmp fix for nios spam
		LOG_REGISTER_OP(base_module, offset, val);
	return val;
}

void csr_write(struct n3k_mgmt_hw *hw, size_t base_module, size_t offset, uint32_t val)
{
	if (base_module != MKS_CHIP_MKS_MGMT_BASE || offset < 0x2200) // TODO(n3k): tmp fix for nios spam
		LOG_REGISTER_OP(base_module, offset, val);
	char* addr = (char*)(hw->pci_resource_start);
	N3K_MGMT_WRITE_REG32(val, addr + base_module + offset);
}
