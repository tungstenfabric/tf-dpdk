/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_init_mq_hw_access.h"
#include "n3k_mgmt_init_type.h"
#include "n3k_mgmt_init_port.h"

#include "mgmt/regs/n3k_mgmt_regs.h"

#include <linux/virtio_config.h>
#include <linux/virtio_net.h>

#define upper_32_bits(n) ((u32)(((n) >> 16) >> 16))
#define lower_32_bits(n) ((u32)(n))

int xlate_ram_cur_off;

void
n3k_mgmt_init_hw_mq_enable(struct n3k_mgmt_hw *hw)
{
	// read modify write
	MKS_VIOSHM_VIOSHM_CSR_CMD_t vioshm_csr_cmd;

	vioshm_csr_cmd.val = csr_read(hw, MKS_CHIP_MKS_VIOSHM_BASE,
		MKS_VIOSHM_VIOSHM_CSR_CMD);

	vioshm_csr_cmd.field.cfg_qid_en = 1;

	csr_write(hw, MKS_CHIP_MKS_VIOSHM_BASE,
		MKS_VIOSHM_VIOSHM_CSR_CMD, vioshm_csr_cmd.val);

}

void
n3k_mgmt_init_hw_max_vq_pairs_set(struct n3k_mgmt_hw *hw, u8 idx, u16 vqp)
{
	VIRTIO_DMA_CSR_VDC_ROM2_t vdc_rom2 = { .val = 0 };

	vdc_rom2.field.rom_2b = vqp > 1 ? vqp - 1 : 1;

	/* VF0 Interface MTU should be more than 1500 */
	if (idx == 0)
		vdc_rom2.field.rom_2a = 2048; // MTU
	else
		vdc_rom2.field.rom_2a = 1500; // MTU

	csr_write(hw, MKS_CHIP_VIRTIO_DMA_CSR_BASE,
		VIRTIO_DMA_CSR_VDC_ROM2(idx), vdc_rom2.val);

}

void
n3k_mgmt_init_hw_virtio_features_set(struct n3k_mgmt_hw *hw, u8 idx, bool is_mq)
{
	u64 features = 0;
	VIRTIO_DMA_CSR_VDC_ROM4_t vdc_rom4 = { .val = 0 };
	VIRTIO_DMA_CSR_VDC_ROM5_t vdc_rom5 = { .val = 0 };

	features |= BIT_ULL(VIRTIO_NET_F_MAC);
	if (is_mq) {
		features |= BIT_ULL(VIRTIO_NET_F_MQ);
		features |= BIT_ULL(VIRTIO_NET_F_CTRL_VQ);
	}
	features |= BIT_ULL(VIRTIO_NET_F_MRG_RXBUF);
	features |= BIT_ULL(VIRTIO_NET_F_MTU);
	features |= BIT_ULL(VIRTIO_F_ANY_LAYOUT);
	features |= BIT_ULL(VIRTIO_F_VERSION_1);
	features |= BIT_ULL(VIRTIO_F_IOMMU_PLATFORM);

	// Device Features LSW
	vdc_rom4.val = lower_32_bits(features);

	csr_write(hw, MKS_CHIP_VIRTIO_DMA_CSR_BASE,
		VIRTIO_DMA_CSR_VDC_ROM4(idx), vdc_rom4.val);

	// Device Features MSW
	vdc_rom5.val = upper_32_bits(features);

	csr_write(hw, MKS_CHIP_VIRTIO_DMA_CSR_BASE,
		VIRTIO_DMA_CSR_VDC_ROM5(idx), vdc_rom5.val);

}

void
n3k_mgmt_init_hw_vqs_set(struct n3k_mgmt_hw *hw, u8 idx, u16 vqs)
{
	VIRTIO_DMA_CSR_VDC_ROM6_t vdc_rom6 = { .val = 0 };

	vdc_rom6.field.rom_6a = vqs > 1 ? vqs * 2 - 1 : 2;

	csr_write(hw, MKS_CHIP_VIRTIO_DMA_CSR_BASE,
		VIRTIO_DMA_CSR_VDC_ROM6(idx), vdc_rom6.val);

}

void
n3k_mgmt_init_hw_vf_map_set(struct n3k_mgmt_hw *hw, u8 idx, u8 is_pf, u16 vqs, u32 offset)
{
	VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_t vic_qoffset_rom = { .val = 0 };

	vic_qoffset_rom.field.vf_enable = 1;
	vic_qoffset_rom.field.isvf = is_pf ? 0 : 1;
	vic_qoffset_rom.field.vqs = vqs;
	vic_qoffset_rom.field.q_offset = offset;

	csr_write(hw, MKS_CHIP_VIRTIO_DMA_CSR_BASE,
			VIRTIO_DMA_CSR_VIC_QOFFSET_ROM(idx),
			vic_qoffset_rom.val);
}

static void
n3k_mgmt_init_hw_msix_map_set(struct n3k_mgmt_hw *hw, u16 idx, VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG_t *msix)
{
	int tx_rx;

	for (tx_rx = 0; tx_rx < 2; ++tx_rx) {
		u16 msix_id = (idx << 1) + tx_rx;

		csr_write(hw, MKS_CHIP_VIRTIO_DMA_CSR_BASE,
			VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG(msix_id),
			msix->val);
	}
}

void
n3k_mgmt_init_hw_queue_map_set(struct n3k_mgmt_hw *hw, struct n3k_mgmt_init_vq *vqs)
{
	struct n3k_mgmt_init_vq *it;
	u8 size = vqs->size;
	int vq_idx = 0;
	VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_t vic_qmapping_rom = { .val = 0 };
	VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG_t msix_qid_xlate_cfg = {
		.val = 0
	};
	u32 thr;

	vic_qmapping_rom.field.q_enable = 1;
	thr = 6;	/* 16K irq/s */
	thr |= BIT(VIRTIO_DMA_CSR_TX_THROTTLE_CFG_THROTTLE_EN_SHIFT);

	for (it = vqs; it != vqs + size; ++it) {
		csr_write(hw, MKS_CHIP_VIRTIO_DMA_CSR_BASE,
			VIRTIO_DMA_CSR_TX_THROTTLE_CFG(it->id),
			thr);
		csr_write(hw, MKS_CHIP_VIRTIO_DMA_CSR_BASE,
			VIRTIO_DMA_CSR_RX_THROTTLE_CFG(it->id),
			thr);

		vic_qmapping_rom.field.is_ctlq = it->is_ctrl;
		vic_qmapping_rom.field.isvf = !it->is_pf;
		vic_qmapping_rom.field.vq = vq_idx;
		vic_qmapping_rom.field.vf = it->vf;
		vic_qmapping_rom.field.pf = it->pf;

		csr_write(hw, MKS_CHIP_VIRTIO_DMA_CSR_BASE,
			VIRTIO_DMA_CSR_VIC_QMAPPING_ROM(it->id),
			vic_qmapping_rom.val);

		msix_qid_xlate_cfg.field.pf = it->pf;
		msix_qid_xlate_cfg.field.vf = it->vf;
		msix_qid_xlate_cfg.field.vf_active = 1;

		n3k_mgmt_init_hw_msix_map_set(hw, it->id, &msix_qid_xlate_cfg);

		++vq_idx;
	}
}

void
n3k_mgmt_init_hw_pvf_xlate_set(struct n3k_mgmt_hw *hw, struct n3k_mgmt_init_vq *vqs)
{
	u16 start;
	u16 entries = 0;

	VIRTIO_DMA_CSR_MSIX_PF_XLATE_CFG_t msix_pf_xlate_ram = { .val = 0 };
	VIRTIO_DMA_CSR_MSIX_VF_XLATE_CFG_t msix_vf_xlate_ram = { .val = 0 };

	if (vqs->is_pf)
		xlate_ram_cur_off = 0;

	start = xlate_ram_cur_off;

	if (vqs->size == 1)
		entries = 2 * vqs->size + 1;

	if (vqs->size > 1)
		entries = 2 * vqs->size - 1;

	xlate_ram_cur_off += entries;

	if (vqs->is_pf) {
		msix_pf_xlate_ram.field.start_entry = start;
		msix_pf_xlate_ram.field.entries = entries;

		csr_write(hw, MKS_CHIP_VIRTIO_DMA_CSR_BASE,
			VIRTIO_DMA_CSR_MSIX_PF_XLATE_CFG(vqs->pf),
			msix_pf_xlate_ram.val);
	} else {
		msix_vf_xlate_ram.field.start_entry = start;
		msix_vf_xlate_ram.field.entries = entries;

		csr_write(hw, MKS_CHIP_VIRTIO_DMA_CSR_BASE,
			VIRTIO_DMA_CSR_MSIX_VF_XLATE_CFG(vqs->vf),
			msix_vf_xlate_ram.val);
	}

}

void
n3k_mgmt_init_hw_pvf_xlate_unset(struct n3k_mgmt_hw *hw, struct n3k_mgmt_init_vq *vqs)
{
	if (vqs->is_pf)
		csr_write(hw, MKS_CHIP_VIRTIO_DMA_CSR_BASE,
			VIRTIO_DMA_CSR_MSIX_PF_XLATE_CFG(vqs->pf),
			0);
	else
		csr_write(hw, MKS_CHIP_VIRTIO_DMA_CSR_BASE,
			VIRTIO_DMA_CSR_MSIX_VF_XLATE_CFG(vqs->vf),
			0);
}

void
n3k_mgmt_init_hw_vf_unmap(struct n3k_mgmt_hw *hw, u8 idx)
{
	VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_t vic_qoffset_rom = { .val = 0 };

	vic_qoffset_rom.field.vf_enable = 0;
	vic_qoffset_rom.field.vqs = 1;
	vic_qoffset_rom.field.q_offset = 0x1ff;

	csr_write(hw, MKS_CHIP_VIRTIO_DMA_CSR_BASE,
		VIRTIO_DMA_CSR_VIC_QOFFSET_ROM(idx),
		vic_qoffset_rom.val);
}

void
n3k_mgmt_init_hw_queue_unmap(struct n3k_mgmt_hw *hw, struct n3k_mgmt_init_vq *vqs)
{
	struct n3k_mgmt_init_vq *it;
	u8 size = vqs->size;
	int vq_idx = 0;

	VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG_t msix_qid_xlate_cfg = {
		.val = 0
	};

	for (it = vqs; it != vqs + size; ++it) {
		csr_write(hw, MKS_CHIP_VIRTIO_DMA_CSR_BASE,
			VIRTIO_DMA_CSR_VIC_QMAPPING_ROM(it->id), 0);

		n3k_mgmt_init_hw_msix_map_set(hw, it->id, &msix_qid_xlate_cfg);

		++vq_idx;
	}
}
