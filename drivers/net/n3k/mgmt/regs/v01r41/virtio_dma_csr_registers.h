#ifndef  _MKS_RDL_VERSION_                                                  
#define  _MKS_RDL_VERSION_      0x400C0008
#endif   // _MKS_RDL_VERSION_                                               
#ifndef  _MKS_RDL_VERSION_MASK_                                             
#define  _MKS_RDL_VERSION_MASK_ 0x0FFFFFFF                                  
#endif   // _MKS_RDL_VERSION_MASK_                                          
#ifndef  _MKS_RDL_BOARD_MASK_                                               
#define  _MKS_RDL_BOARD_MASK_   0xF0000000                                  
#endif   // _MKS_RDL_BOARD_MASK_                                            
/* ************************************************************************* 
 *
 * $Id: //depot/tmbu/eng/rdldk/main/genhdr/code_generator.py#3 $
 * Date: 2020-12-02 10:39
 *
 * **************************************************************************
 * intel_start_of_header
 *
 * INTEL CONFIDENTIAL
 * Copyright 2017 Intel Corporation
 *
 * The source code contained or described herein and all documents related
 * to the source code ("Material") are owned by Intel Corporation or
 * its suppliers or licensors. Title to the Material remains with Intel
 * Corporation or its suppliers and licensors. The Material contains trade
 * secrets and proprietary and confidential information of Intel or its
 * suppliers and licensors. The Material is protected by worldwide copyright
 * and trade secret laws and treaty provisions. No part of the Material
 * may be used, copied, reproduced, modified, published, uploaded, posted,
 * transmitted, distributed, or disclosed in any way without Intel's prior
 * express written permission.
 *
 * No license under any patent, copyright, trade secret or other intellectual
 * property right is granted to or conferred upon you by disclosure or
 * delivery of the Materials, either expressly, by implication, inducement,
 * estoppel or otherwise. Any license under such intellectual property
 * rights must be express and approved by Intel in writing.
 *
 * intel_end_of_header
 * ************************************************************************
 */

#ifndef _VIRTIO_DMA_CSR_REGISTERS_H_
#define _VIRTIO_DMA_CSR_REGISTERS_H_

/* Virtio DMA Submodules */

/*
Definitions:
- Bit positions are numbered from 0 with LSB in position 0.
- Byte ordering is little endian: LSB at lowest address.
- Register width is width of register in bits.
- Access width is width of read/write access to register, in bits. 
  Often access width == register width in which case the register can be 
  read/written in a single access. 
  If access width is less than register width then multiple accesses
  are needed to read/write the register. Access words are numbered from 0 by
  their offset in bytes from the register address. So for access width 32 
  access words are numbered 0, 4, 8, 12 etc. up to the size of the register.
  
Defines for address map:
  map_REGWIDTH              Default width of register in bits
  map_ACCESSWIDTH           Default width of access word in bits
  map_ACCESSTYPE            Default access type. Access types defined below

Defines for register reg:
  map_reg                   Address of register. Only if non-table register.
  map_reg(ix)               Address of table register. Only if table with 1 index
  map_reg(ix1, ix2)         Address of table register. Only if table with 2 indexes
  map_reg_MAX_INDEX         Maximum last (ix/ix2) index in table. Only if table.
  map_reg_ix1_MAX_INDEX     Maximum but-last (ix1) index in table. Only if table.
  map_reg_REGWIDTH          Width of register in bits. 
                            Only if different from map_REGWIDTH
  map_reg_ACCESSWIDTH       Width of access word in bits. 
                            Only if different from map_ACCESSWIDTH
  
Defines for field:
  map_reg_field_ACCESSTYPE  Access type. Only if different from map_ACCESSTYPE
  map_reg_field_WIDTH       Width of field in bits.
                            May be larger than access width
  map_reg_field_SHIFT       Bit position in register of LSB of field.
                            Only if register width == access width
  map_reg_field_SHIFT_n     Bit position, in access word n, of LSB of field.
                            Only if register width != access width
  map_reg_field_MASK        Bit mask of field in register.
                            Only if access width == register width.
                            Only if mask is 64 bits or less
  map_reg_field_MASK_n      Bit mask of field in access word n containing LSB of 
                            field.
                            Only if access width != register width.
                            Only if mask is 64 bits or less
  map_reg_field             map_reg_field_SHIFT. Only if 1-bit field.
                            Only if register width == access width
  map_reg_field_n           map_reg_field_SHIFT_n. Only if 1-bit field
                            Only if register width != access width
*/

#ifndef GENHDR_ACCESSTYPE
#define GENHDR_ACCESSTYPE               
#define GENHDR_ACCESSTYPE_RW            0 /* Read / Write */
#define GENHDR_ACCESSTYPE_RWA           1 /* Read / Write Atomic. Read/write low address first, then high address. */
#define GENHDR_ACCESSTYPE_RO            2 /* Read Only */
#define GENHDR_ACCESSTYPE_ROA           3 /* Read Only Atomic. Read low address first, then high address */
#define GENHDR_ACCESSTYPE_RWE           4 /* Read / Write Exclusive. Read-modify-write not needed for field update. */
#define GENHDR_ACCESSTYPE_W1C           5 /* Write 1 to Clear (on individual bits) */
#define GENHDR_ACCESSTYPE_WO            6 /* Write Only */
#endif /* GENHDR_ACCESSTYPE */

#define VIRTIO_DMA_CSR_ACCESSTYPE       GENHDR_ACCESSTYPE_RW /*  Default access type. Access types defined above. */
#define VIRTIO_DMA_CSR_REGWIDTH         32 /* Default width of register in bits */
#define VIRTIO_DMA_CSR_ACCESSWIDTH      32 /* Default width of access word in bit */


/* Virtio Config QID Offset ROM, one entry per PVF */
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM(vic_qoffset_rom_ix)      (0x00000000U + ((vic_qoffset_rom_ix) * 4))
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_MAX_INDEX                0x7FU
/* Is this VF enabled. */
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_VF_ENABLE_ACCESSTYPE     GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_VF_ENABLE_SHIFT          0
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_VF_ENABLE_WIDTH          1
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_VF_ENABLE_MASK           (0x1U << VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_VF_ENABLE_SHIFT)

/* reserved. */
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_RSV_1_ACCESSTYPE         GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_RSV_1_SHIFT              1
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_RSV_1_WIDTH              2
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_RSV_1_MASK               (0x3U << VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_RSV_1_SHIFT)

/* Is this a VF or a PF. */
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_ISVF_ACCESSTYPE          GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_ISVF_SHIFT               3
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_ISVF_WIDTH               1
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_ISVF_MASK                (0x1U << VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_ISVF_SHIFT)

/* Max. VQs for the VF. */
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_VQS_ACCESSTYPE           GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_VQS_SHIFT                4
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_VQS_WIDTH                12
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_VQS_MASK                 (0xFFFU << VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_VQS_SHIFT)

/* QID Offset Value for the PVF. */
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_Q_OFFSET_ACCESSTYPE      GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_Q_OFFSET_SHIFT           16
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_Q_OFFSET_WIDTH           12
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_Q_OFFSET_MASK            (0xFFFU << VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_Q_OFFSET_SHIFT)

/* reserved. */
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_RSV_2_ACCESSTYPE         GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_RSV_2_SHIFT              28
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_RSV_2_WIDTH              4
#define VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_RSV_2_MASK               (0xFU << VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_RSV_2_SHIFT)

#ifdef GENHDR_STRUCT
/* Virtio Config QID Offset ROM, one entry per PVF */
typedef union {
  struct {
    uint32_t vf_enable : 1;     /* Is this VF enabled. */
    uint32_t rsv_1 : 2;         /* reserved. */
    uint32_t isvf : 1;          /* Is this a VF or a PF. */
    uint32_t vqs : 12;          /* Max. VQs for the VF. */
    uint32_t q_offset : 12;     /* QID Offset Value for the PVF. */
    uint32_t rsv_2 : 4;         /* reserved. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VIC_QOFFSET_ROM_t;
#endif /* GENHDR_STRUCT */

/* Virtio Driver Features word 0 */
#define VIRTIO_DMA_CSR_VDF_FEATURES_0(virtio_config_driver_features_ram_ix) (0x00004000U + ((virtio_config_driver_features_ram_ix) * 8))
#define VIRTIO_DMA_CSR_VDF_FEATURES_0_VIRTIO_CONFIG_DRIVER_FEATURES_RAM_MAX_INDEX 0x7F
/*
 * Virtio Driver Feature Bits 0 (bits 31:0), as defined in Virtio 1.0
 * specification. The values held here are those that the Virtio software has
 * negotiated with the device.
 */
#define VIRTIO_DMA_CSR_VDF_FEATURES_0_DRIVER_FEATURES_0_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VDF_FEATURES_0_DRIVER_FEATURES_0_SHIFT           0
#define VIRTIO_DMA_CSR_VDF_FEATURES_0_DRIVER_FEATURES_0_WIDTH           32
#define VIRTIO_DMA_CSR_VDF_FEATURES_0_DRIVER_FEATURES_0_MASK            (0xFFFFFFFFU << VIRTIO_DMA_CSR_VDF_FEATURES_0_DRIVER_FEATURES_0_SHIFT)

#ifdef GENHDR_STRUCT
/* Virtio Driver Features word 0 */
typedef union {
  struct {
    uint32_t driver_features_0 : 32;    /* Virtio Driver Feature Bits 0 (bits 31:0), as defined in Virtio 1.0
                                           specification. The values held here are those that the Virtio software has
                                           negotiated with the device. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VDF_FEATURES_0_t;
#endif /* GENHDR_STRUCT */

/* Virtio Driver Features word 1 */
#define VIRTIO_DMA_CSR_VDF_FEATURES_1(virtio_config_driver_features_ram_ix) (0x00004004U + ((virtio_config_driver_features_ram_ix) * 8))
#define VIRTIO_DMA_CSR_VDF_FEATURES_1_VIRTIO_CONFIG_DRIVER_FEATURES_RAM_MAX_INDEX 0x7F
/*
 * Virtio Driver Feature Bits 1 (bits 63:32), as defined in Virtio 1.0
 * specification. The values held here are those that the Virtio software has
 * negotiated with the device.
 */
#define VIRTIO_DMA_CSR_VDF_FEATURES_1_DRIVER_FEATURES_1_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VDF_FEATURES_1_DRIVER_FEATURES_1_SHIFT           0
#define VIRTIO_DMA_CSR_VDF_FEATURES_1_DRIVER_FEATURES_1_WIDTH           32
#define VIRTIO_DMA_CSR_VDF_FEATURES_1_DRIVER_FEATURES_1_MASK            (0xFFFFFFFFU << VIRTIO_DMA_CSR_VDF_FEATURES_1_DRIVER_FEATURES_1_SHIFT)

#ifdef GENHDR_STRUCT
/* Virtio Driver Features word 1 */
typedef union {
  struct {
    uint32_t driver_features_1 : 32;    /* Virtio Driver Feature Bits 1 (bits 63:32), as defined in Virtio 1.0
                                           specification. The values held here are those that the Virtio software has
                                           negotiated with the device. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VDF_FEATURES_1_t;
#endif /* GENHDR_STRUCT */

/* Status associated with a PVF */
#define VIRTIO_DMA_CSR_VIC_STATUS_RAM(vic_status_ram_ix)                (0x00005000U + ((vic_status_ram_ix) * 4))
#define VIRTIO_DMA_CSR_VIC_STATUS_RAM_MAX_INDEX                         0x7FU
/*
 * Current number of active VQ pairs for the PVF. Active pairs are pairs that
 * have been enabled by use of Ctrl messages. Resets to 1.
 */
#define VIRTIO_DMA_CSR_VIC_STATUS_RAM_NUM_ACTIVE_VQ_PAIRS_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VIC_STATUS_RAM_NUM_ACTIVE_VQ_PAIRS_SHIFT         0
#define VIRTIO_DMA_CSR_VIC_STATUS_RAM_NUM_ACTIVE_VQ_PAIRS_WIDTH         16
#define VIRTIO_DMA_CSR_VIC_STATUS_RAM_NUM_ACTIVE_VQ_PAIRS_MASK          (0xFFFFU << VIRTIO_DMA_CSR_VIC_STATUS_RAM_NUM_ACTIVE_VQ_PAIRS_SHIFT)

/*
 * Device status for the PVF from the Virtio Negotiation. This value is the
 * Virtio Device Status as defined in the Virtio 1.0 specification.
 */
#define VIRTIO_DMA_CSR_VIC_STATUS_RAM_DEVICE_STATUS_ACCESSTYPE          GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VIC_STATUS_RAM_DEVICE_STATUS_SHIFT               24
#define VIRTIO_DMA_CSR_VIC_STATUS_RAM_DEVICE_STATUS_WIDTH               8
#define VIRTIO_DMA_CSR_VIC_STATUS_RAM_DEVICE_STATUS_MASK                (0xFFU << VIRTIO_DMA_CSR_VIC_STATUS_RAM_DEVICE_STATUS_SHIFT)

#ifdef GENHDR_STRUCT
/* Status associated with a PVF */
typedef union {
  struct {
    uint32_t num_active_vq_pairs : 16;  /* Current number of active VQ pairs for the PVF. Active pairs are pairs that
                                           have been enabled by use of Ctrl messages. Resets to 1. */
    uint32_t : 8;
    uint32_t device_status : 8;         /* Device status for the PVF from the Virtio Negotiation. This value is the
                                           Virtio Device Status as defined in the Virtio 1.0 specification. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VIC_STATUS_RAM_t;
#endif /* GENHDR_STRUCT */

/* Virtio Config QID Mapping ROM, one entry per QID */
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM(vic_qmapping_rom_ix)    (0x00008000U + ((vic_qmapping_rom_ix) * 4))
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_MAX_INDEX               0x1FFU
/* Enable Queue. */
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_Q_ENABLE_ACCESSTYPE     GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_Q_ENABLE_SHIFT          0
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_Q_ENABLE_WIDTH          1
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_Q_ENABLE_MASK           (0x1U << VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_Q_ENABLE_SHIFT)

/* reserved. */
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_RSV_1_ACCESSTYPE        GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_RSV_1_SHIFT             1
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_RSV_1_WIDTH             1
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_RSV_1_MASK              (0x1U << VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_RSV_1_SHIFT)

/* Is this a control queue. */
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_IS_CTLQ_ACCESSTYPE      GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_IS_CTLQ_SHIFT           2
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_IS_CTLQ_WIDTH           1
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_IS_CTLQ_MASK            (0x1U << VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_IS_CTLQ_SHIFT)

/* Is this a VF. */
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_ISVF_ACCESSTYPE         GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_ISVF_SHIFT              3
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_ISVF_WIDTH              1
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_ISVF_MASK               (0x1U << VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_ISVF_SHIFT)

/* VQ pair id. */
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_VQ_ACCESSTYPE           GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_VQ_SHIFT                4
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_VQ_WIDTH                12
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_VQ_MASK                 (0xFFFU << VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_VQ_SHIFT)

/* VF. */
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_VF_ACCESSTYPE           GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_VF_SHIFT                16
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_VF_WIDTH                12
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_VF_MASK                 (0xFFFU << VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_VF_SHIFT)

/* PF. The actual number of PFs available is set by the NUM_PFS parameter. */
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_PF_ACCESSTYPE           GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_PF_SHIFT                28
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_PF_WIDTH                3
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_PF_MASK                 (0x7U << VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_PF_SHIFT)

/* reserved. */
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_RSV_31_ACCESSTYPE       GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_RSV_31_SHIFT            31
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_RSV_31_WIDTH            1
#define VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_RSV_31_MASK             (0x1U << VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_RSV_31_SHIFT)

#ifdef GENHDR_STRUCT
/* Virtio Config QID Mapping ROM, one entry per QID */
typedef union {
  struct {
    uint32_t q_enable : 1;      /* Enable Queue. */
    uint32_t rsv_1 : 1;         /* reserved. */
    uint32_t is_ctlq : 1;       /* Is this a control queue. */
    uint32_t isvf : 1;          /* Is this a VF. */
    uint32_t vq : 12;           /* VQ pair id. */
    uint32_t vf : 12;           /* VF. */
    uint32_t pf : 3;            /* PF. The actual number of PFs available is set by the NUM_PFS parameter. */
    uint32_t rsv_31 : 1;        /* reserved. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VIC_QMAPPING_ROM_t;
#endif /* GENHDR_STRUCT */

/* Virtio Device Config ROM word 0 */
#define VIRTIO_DMA_CSR_VDC_ROM0(virtio_device_config_rom_ix) (0x0000C000U + ((virtio_device_config_rom_ix) * 32))
#define VIRTIO_DMA_CSR_VDC_ROM0_VIRTIO_DEVICE_CONFIG_ROM_MAX_INDEX 0x7F
/* MAC Address bytes [0:3]. */
#define VIRTIO_DMA_CSR_VDC_ROM0_ROM_0_ACCESSTYPE GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VDC_ROM0_ROM_0_SHIFT     0
#define VIRTIO_DMA_CSR_VDC_ROM0_ROM_0_WIDTH     32
#define VIRTIO_DMA_CSR_VDC_ROM0_ROM_0_MASK      (0xFFFFFFFFU << VIRTIO_DMA_CSR_VDC_ROM0_ROM_0_SHIFT)

#ifdef GENHDR_STRUCT
/* Virtio Device Config ROM word 0 */
typedef union {
  struct {
    uint32_t rom_0 : 32;        /* MAC Address bytes [0:3]. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VDC_ROM0_t;
#endif /* GENHDR_STRUCT */

/* Virtio Device Config ROM word 1 */
#define VIRTIO_DMA_CSR_VDC_ROM1(virtio_device_config_rom_ix) (0x0000C004U + ((virtio_device_config_rom_ix) * 32))
#define VIRTIO_DMA_CSR_VDC_ROM1_VIRTIO_DEVICE_CONFIG_ROM_MAX_INDEX 0x7F
/* MAC Address bytes [4:5]. */
#define VIRTIO_DMA_CSR_VDC_ROM1_ROM_1B_ACCESSTYPE       GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VDC_ROM1_ROM_1B_SHIFT            0
#define VIRTIO_DMA_CSR_VDC_ROM1_ROM_1B_WIDTH            16
#define VIRTIO_DMA_CSR_VDC_ROM1_ROM_1B_MASK             (0xFFFFU << VIRTIO_DMA_CSR_VDC_ROM1_ROM_1B_SHIFT)

/* Link Status. */
#define VIRTIO_DMA_CSR_VDC_ROM1_ROM_1A_ACCESSTYPE       GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VDC_ROM1_ROM_1A_SHIFT            16
#define VIRTIO_DMA_CSR_VDC_ROM1_ROM_1A_WIDTH            16
#define VIRTIO_DMA_CSR_VDC_ROM1_ROM_1A_MASK             (0xFFFFU << VIRTIO_DMA_CSR_VDC_ROM1_ROM_1A_SHIFT)

#ifdef GENHDR_STRUCT
/* Virtio Device Config ROM word 1 */
typedef union {
  struct {
    uint32_t rom_1b : 16;       /* MAC Address bytes [4:5]. */
    uint32_t rom_1a : 16;       /* Link Status. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VDC_ROM1_t;
#endif /* GENHDR_STRUCT */

/* Virtio Device Config ROM word 2 */
#define VIRTIO_DMA_CSR_VDC_ROM2(virtio_device_config_rom_ix) (0x0000C008U + ((virtio_device_config_rom_ix) * 32))
#define VIRTIO_DMA_CSR_VDC_ROM2_VIRTIO_DEVICE_CONFIG_ROM_MAX_INDEX 0x7F
/* Max VirtQueue Pairs. */
#define VIRTIO_DMA_CSR_VDC_ROM2_ROM_2B_ACCESSTYPE       GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VDC_ROM2_ROM_2B_SHIFT            0
#define VIRTIO_DMA_CSR_VDC_ROM2_ROM_2B_WIDTH            16
#define VIRTIO_DMA_CSR_VDC_ROM2_ROM_2B_MASK             (0xFFFFU << VIRTIO_DMA_CSR_VDC_ROM2_ROM_2B_SHIFT)

/* MTU. */
#define VIRTIO_DMA_CSR_VDC_ROM2_ROM_2A_ACCESSTYPE       GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VDC_ROM2_ROM_2A_SHIFT            16
#define VIRTIO_DMA_CSR_VDC_ROM2_ROM_2A_WIDTH            16
#define VIRTIO_DMA_CSR_VDC_ROM2_ROM_2A_MASK             (0xFFFFU << VIRTIO_DMA_CSR_VDC_ROM2_ROM_2A_SHIFT)

#ifdef GENHDR_STRUCT
/* Virtio Device Config ROM word 2 */
typedef union {
  struct {
    uint32_t rom_2b : 16;       /* Max VirtQueue Pairs. */
    uint32_t rom_2a : 16;       /* MTU. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VDC_ROM2_t;
#endif /* GENHDR_STRUCT */

/* Virtio Device Config ROM word 3 */
#define VIRTIO_DMA_CSR_VDC_ROM3(virtio_device_config_rom_ix) (0x0000C00CU + ((virtio_device_config_rom_ix) * 32))
#define VIRTIO_DMA_CSR_VDC_ROM3_VIRTIO_DEVICE_CONFIG_ROM_MAX_INDEX 0x7F
/* Reserved. */
#define VIRTIO_DMA_CSR_VDC_ROM3_ROM_3_ACCESSTYPE GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VDC_ROM3_ROM_3_SHIFT     0
#define VIRTIO_DMA_CSR_VDC_ROM3_ROM_3_WIDTH     32
#define VIRTIO_DMA_CSR_VDC_ROM3_ROM_3_MASK      (0xFFFFFFFFU << VIRTIO_DMA_CSR_VDC_ROM3_ROM_3_SHIFT)

#ifdef GENHDR_STRUCT
/* Virtio Device Config ROM word 3 */
typedef union {
  struct {
    uint32_t rom_3 : 32;        /* Reserved. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VDC_ROM3_t;
#endif /* GENHDR_STRUCT */

/* Virtio Device Config ROM word 4 */
#define VIRTIO_DMA_CSR_VDC_ROM4(virtio_device_config_rom_ix) (0x0000C010U + ((virtio_device_config_rom_ix) * 32))
#define VIRTIO_DMA_CSR_VDC_ROM4_VIRTIO_DEVICE_CONFIG_ROM_MAX_INDEX 0x7F
/* Device Features LSW. */
#define VIRTIO_DMA_CSR_VDC_ROM4_ROM_4_ACCESSTYPE GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VDC_ROM4_ROM_4_SHIFT     0
#define VIRTIO_DMA_CSR_VDC_ROM4_ROM_4_WIDTH     32
#define VIRTIO_DMA_CSR_VDC_ROM4_ROM_4_MASK      (0xFFFFFFFFU << VIRTIO_DMA_CSR_VDC_ROM4_ROM_4_SHIFT)

#ifdef GENHDR_STRUCT
/* Virtio Device Config ROM word 4 */
typedef union {
  struct {
    uint32_t rom_4 : 32;        /* Device Features LSW. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VDC_ROM4_t;
#endif /* GENHDR_STRUCT */

/* Virtio Device Config ROM word 5 */
#define VIRTIO_DMA_CSR_VDC_ROM5(virtio_device_config_rom_ix) (0x0000C014U + ((virtio_device_config_rom_ix) * 32))
#define VIRTIO_DMA_CSR_VDC_ROM5_VIRTIO_DEVICE_CONFIG_ROM_MAX_INDEX 0x7F
/* Device Features MSW. */
#define VIRTIO_DMA_CSR_VDC_ROM5_ROM_5_ACCESSTYPE GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VDC_ROM5_ROM_5_SHIFT     0
#define VIRTIO_DMA_CSR_VDC_ROM5_ROM_5_WIDTH     32
#define VIRTIO_DMA_CSR_VDC_ROM5_ROM_5_MASK      (0xFFFFFFFFU << VIRTIO_DMA_CSR_VDC_ROM5_ROM_5_SHIFT)

#ifdef GENHDR_STRUCT
/* Virtio Device Config ROM word 5 */
typedef union {
  struct {
    uint32_t rom_5 : 32;        /* Device Features MSW. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VDC_ROM5_t;
#endif /* GENHDR_STRUCT */

/* Virtio Device Config ROM word 6 */
#define VIRTIO_DMA_CSR_VDC_ROM6(virtio_device_config_rom_ix) (0x0000C018U + ((virtio_device_config_rom_ix) * 32))
#define VIRTIO_DMA_CSR_VDC_ROM6_VIRTIO_DEVICE_CONFIG_ROM_MAX_INDEX 0x7F
/* Reserved. */
#define VIRTIO_DMA_CSR_VDC_ROM6_ROM_6B_ACCESSTYPE       GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VDC_ROM6_ROM_6B_SHIFT            0
#define VIRTIO_DMA_CSR_VDC_ROM6_ROM_6B_WIDTH            16
#define VIRTIO_DMA_CSR_VDC_ROM6_ROM_6B_MASK             (0xFFFFU << VIRTIO_DMA_CSR_VDC_ROM6_ROM_6B_SHIFT)

/* Num Queues. */
#define VIRTIO_DMA_CSR_VDC_ROM6_ROM_6A_ACCESSTYPE       GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VDC_ROM6_ROM_6A_SHIFT            16
#define VIRTIO_DMA_CSR_VDC_ROM6_ROM_6A_WIDTH            16
#define VIRTIO_DMA_CSR_VDC_ROM6_ROM_6A_MASK             (0xFFFFU << VIRTIO_DMA_CSR_VDC_ROM6_ROM_6A_SHIFT)

#ifdef GENHDR_STRUCT
/* Virtio Device Config ROM word 6 */
typedef union {
  struct {
    uint32_t rom_6b : 16;       /* Reserved. */
    uint32_t rom_6a : 16;       /* Num Queues. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VDC_ROM6_t;
#endif /* GENHDR_STRUCT */

/* Virtio Device Config ROM word 7 */
#define VIRTIO_DMA_CSR_VDC_ROM7(virtio_device_config_rom_ix) (0x0000C01CU + ((virtio_device_config_rom_ix) * 32))
#define VIRTIO_DMA_CSR_VDC_ROM7_VIRTIO_DEVICE_CONFIG_ROM_MAX_INDEX 0x7F
/* Reserved. */
#define VIRTIO_DMA_CSR_VDC_ROM7_ROM_7_ACCESSTYPE GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VDC_ROM7_ROM_7_SHIFT     0
#define VIRTIO_DMA_CSR_VDC_ROM7_ROM_7_WIDTH     32
#define VIRTIO_DMA_CSR_VDC_ROM7_ROM_7_MASK      (0xFFFFFFFFU << VIRTIO_DMA_CSR_VDC_ROM7_ROM_7_SHIFT)

#ifdef GENHDR_STRUCT
/* Virtio Device Config ROM word 7 */
typedef union {
  struct {
    uint32_t rom_7 : 32;        /* Reserved. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VDC_ROM7_t;
#endif /* GENHDR_STRUCT */

/* Virtio DMA Version Register */
#define VIRTIO_DMA_CSR_DMA_CFG_VERSION                          0x00010000U
/* Virtio DMA version number. Set in Hardware. */
#define VIRTIO_DMA_CSR_DMA_CFG_VERSION_VERSION_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_DMA_CFG_VERSION_VERSION_SHIFT            0
#define VIRTIO_DMA_CSR_DMA_CFG_VERSION_VERSION_WIDTH            32
#define VIRTIO_DMA_CSR_DMA_CFG_VERSION_VERSION_MASK             (0xFFFFFFFFU << VIRTIO_DMA_CSR_DMA_CFG_VERSION_VERSION_SHIFT)

#ifdef GENHDR_STRUCT
/* Virtio DMA Version Register */
typedef union {
  struct {
    uint32_t version : 32;      /* Virtio DMA version number. Set in Hardware. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_DMA_CFG_VERSION_t;
#endif /* GENHDR_STRUCT */

/* Virtio DMA Scratch Register [DEBUG REG] */
#define VIRTIO_DMA_CSR_DMA_CFG_SCRATCH                          0x00010004U
/*
 * General Purpose Read/Write Scratch register. No functional use in module, for
 * debug purposes. Resets to 0.
 */
#define VIRTIO_DMA_CSR_DMA_CFG_SCRATCH_SCRATCH_SHIFT            0
#define VIRTIO_DMA_CSR_DMA_CFG_SCRATCH_SCRATCH_WIDTH            32
#define VIRTIO_DMA_CSR_DMA_CFG_SCRATCH_SCRATCH_MASK             (0xFFFFFFFFU << VIRTIO_DMA_CSR_DMA_CFG_SCRATCH_SCRATCH_SHIFT)

#ifdef GENHDR_STRUCT
/* Virtio DMA Scratch Register [DEBUG REG] */
typedef union {
  struct {
    uint32_t scratch : 32;      /* General Purpose Read/Write Scratch register. No functional use in module, for
                                   debug purposes. Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_DMA_CFG_SCRATCH_t;
#endif /* GENHDR_STRUCT */

/* Virtio DMA ID ROM */
#define VIRTIO_DMA_CSR_DMA_CFG_IDROM(dma_cfg_idrom_ix)  (0x00010020U + ((dma_cfg_idrom_ix) * 4))
#define VIRTIO_DMA_CSR_DMA_CFG_IDROM_MAX_INDEX          0x1FU
/*
 * This ROM is loaded from version_id.hex and contains hard-coded version
 * information. Contents TBC.
 */
#define VIRTIO_DMA_CSR_DMA_CFG_IDROM_ID_ROM_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_DMA_CFG_IDROM_ID_ROM_SHIFT       0
#define VIRTIO_DMA_CSR_DMA_CFG_IDROM_ID_ROM_WIDTH       32
#define VIRTIO_DMA_CSR_DMA_CFG_IDROM_ID_ROM_MASK        (0xFFFFFFFFU << VIRTIO_DMA_CSR_DMA_CFG_IDROM_ID_ROM_SHIFT)

#ifdef GENHDR_STRUCT
/* Virtio DMA ID ROM */
typedef union {
  struct {
    uint32_t id_rom : 32;       /* This ROM is loaded from version_id.hex and contains hard-coded version
                                   information. Contents TBC. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_DMA_CFG_IDROM_t;
#endif /* GENHDR_STRUCT */

/* MSI-X QID to VF,PF,MSI-X-index translate RAM */
#define VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG(msix_qid_xlate_cfg_ix) (0x00014000U + ((msix_qid_xlate_cfg_ix) * 4))
#define VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG_MAX_INDEX             0x7FFU
/* MSI-X index. */
#define VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG_MSIX_INDEX_SHIFT      0
#define VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG_MSIX_INDEX_WIDTH      12
#define VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG_MSIX_INDEX_MASK       (0xFFFU << VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG_MSIX_INDEX_SHIFT)

/* VF number. */
#define VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG_VF_SHIFT              12
#define VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG_VF_WIDTH              7
#define VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG_VF_MASK               (0x7FU << VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG_VF_SHIFT)

/* VF active. */
#define VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG_VF_ACTIVE_SHIFT       24
#define VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG_VF_ACTIVE_WIDTH       1
#define VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG_VF_ACTIVE_MASK        (0x1U << VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG_VF_ACTIVE_SHIFT)

/* PF number. */
#define VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG_PF_SHIFT              25
#define VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG_PF_WIDTH              3
#define VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG_PF_MASK               (0x7U << VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG_PF_SHIFT)

#ifdef GENHDR_STRUCT
/* MSI-X QID to VF,PF,MSI-X-index translate RAM */
typedef union {
  struct {
    uint32_t msix_index : 12;   /* MSI-X index. */
    uint32_t vf : 7;            /* VF number. */
    uint32_t : 5;
    uint32_t vf_active : 1;     /* VF active. */
    uint32_t pf : 3;            /* PF number. */
    uint32_t : 4;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_MSIX_QID_XLATE_CFG_t;
#endif /* GENHDR_STRUCT */

/* MSI-X VF index translate configuration */
#define VIRTIO_DMA_CSR_MSIX_VF_XLATE_CFG(msix_vf_xlate_cfg_ix)  (0x00016000U + ((msix_vf_xlate_cfg_ix) * 4))
#define VIRTIO_DMA_CSR_MSIX_VF_XLATE_CFG_MAX_INDEX              0x7DU
/* Entries allocated in global MSI-X table. */
#define VIRTIO_DMA_CSR_MSIX_VF_XLATE_CFG_ENTRIES_SHIFT          0
#define VIRTIO_DMA_CSR_MSIX_VF_XLATE_CFG_ENTRIES_WIDTH          12
#define VIRTIO_DMA_CSR_MSIX_VF_XLATE_CFG_ENTRIES_MASK           (0xFFFU << VIRTIO_DMA_CSR_MSIX_VF_XLATE_CFG_ENTRIES_SHIFT)

/* Start entry in global MSI-X table. */
#define VIRTIO_DMA_CSR_MSIX_VF_XLATE_CFG_START_ENTRY_SHIFT      16
#define VIRTIO_DMA_CSR_MSIX_VF_XLATE_CFG_START_ENTRY_WIDTH      12
#define VIRTIO_DMA_CSR_MSIX_VF_XLATE_CFG_START_ENTRY_MASK       (0xFFFU << VIRTIO_DMA_CSR_MSIX_VF_XLATE_CFG_START_ENTRY_SHIFT)

#ifdef GENHDR_STRUCT
/* MSI-X VF index translate configuration */
typedef union {
  struct {
    uint32_t entries : 12;      /* Entries allocated in global MSI-X table. */
    uint32_t : 4;
    uint32_t start_entry : 12;  /* Start entry in global MSI-X table. */
    uint32_t : 4;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_MSIX_VF_XLATE_CFG_t;
#endif /* GENHDR_STRUCT */

/* MSI-X PF index translate configuration */
#define VIRTIO_DMA_CSR_MSIX_PF_XLATE_CFG(msix_pf_xlate_cfg_ix)  (0x00016800U + ((msix_pf_xlate_cfg_ix) * 4))
#define VIRTIO_DMA_CSR_MSIX_PF_XLATE_CFG_MAX_INDEX              0x1U
/* Entries allocated in global MSI-X table. */
#define VIRTIO_DMA_CSR_MSIX_PF_XLATE_CFG_ENTRIES_SHIFT          0
#define VIRTIO_DMA_CSR_MSIX_PF_XLATE_CFG_ENTRIES_WIDTH          12
#define VIRTIO_DMA_CSR_MSIX_PF_XLATE_CFG_ENTRIES_MASK           (0xFFFU << VIRTIO_DMA_CSR_MSIX_PF_XLATE_CFG_ENTRIES_SHIFT)

/* Start entry in global MSI-X table. */
#define VIRTIO_DMA_CSR_MSIX_PF_XLATE_CFG_START_ENTRY_SHIFT      16
#define VIRTIO_DMA_CSR_MSIX_PF_XLATE_CFG_START_ENTRY_WIDTH      12
#define VIRTIO_DMA_CSR_MSIX_PF_XLATE_CFG_START_ENTRY_MASK       (0xFFFU << VIRTIO_DMA_CSR_MSIX_PF_XLATE_CFG_START_ENTRY_SHIFT)

#ifdef GENHDR_STRUCT
/* MSI-X PF index translate configuration */
typedef union {
  struct {
    uint32_t entries : 12;      /* Entries allocated in global MSI-X table. */
    uint32_t : 4;
    uint32_t start_entry : 12;  /* Start entry in global MSI-X table. */
    uint32_t : 4;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_MSIX_PF_XLATE_CFG_t;
#endif /* GENHDR_STRUCT */

/* MSI-X incoming interrupt FIFO levels [DEBUG REG] */
#define VIRTIO_DMA_CSR_MSIX_IN_FIFO(msix_in_fifo_ix)    (0x00016900U + ((msix_in_fifo_ix) * 4))
#define VIRTIO_DMA_CSR_MSIX_IN_FIFO_MAX_INDEX           0x3U
/* Entries used in incoming interrupt FIFO. Depth = 2^11. */
#define VIRTIO_DMA_CSR_MSIX_IN_FIFO_LVL_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_MSIX_IN_FIFO_LVL_SHIFT           0
#define VIRTIO_DMA_CSR_MSIX_IN_FIFO_LVL_WIDTH           12
#define VIRTIO_DMA_CSR_MSIX_IN_FIFO_LVL_MASK            (0xFFFU << VIRTIO_DMA_CSR_MSIX_IN_FIFO_LVL_SHIFT)

#ifdef GENHDR_STRUCT
/* MSI-X incoming interrupt FIFO levels [DEBUG REG] */
typedef union {
  struct {
    uint32_t lvl : 12;  /* Entries used in incoming interrupt FIFO. Depth = 2^11. */
    uint32_t : 20;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_MSIX_IN_FIFO_t;
#endif /* GENHDR_STRUCT */

/* MSI-X outgoing interrupt FIFO level [DEBUG REG] */
#define VIRTIO_DMA_CSR_MSIX_OUT_FIFO                    0x00016910U
/* Entries used in outgoing interrupt FIFO. Depth = 2^5. */
#define VIRTIO_DMA_CSR_MSIX_OUT_FIFO_LVL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_MSIX_OUT_FIFO_LVL_SHIFT          0
#define VIRTIO_DMA_CSR_MSIX_OUT_FIFO_LVL_WIDTH          6
#define VIRTIO_DMA_CSR_MSIX_OUT_FIFO_LVL_MASK           (0x3FU << VIRTIO_DMA_CSR_MSIX_OUT_FIFO_LVL_SHIFT)

#ifdef GENHDR_STRUCT
/* MSI-X outgoing interrupt FIFO level [DEBUG REG] */
typedef union {
  struct {
    uint32_t lvl : 6;   /* Entries used in outgoing interrupt FIFO. Depth = 2^5. */
    uint32_t : 26;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_MSIX_OUT_FIFO_t;
#endif /* GENHDR_STRUCT */

/* MSI-X write request FIFO level [DEBUG REG] */
#define VIRTIO_DMA_CSR_MSIX_WRREQ_FIFO                  0x00016914U
/* Entries used in write request FIFO. Depth = 2^4. */
#define VIRTIO_DMA_CSR_MSIX_WRREQ_FIFO_LVL_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_MSIX_WRREQ_FIFO_LVL_SHIFT        0
#define VIRTIO_DMA_CSR_MSIX_WRREQ_FIFO_LVL_WIDTH        5
#define VIRTIO_DMA_CSR_MSIX_WRREQ_FIFO_LVL_MASK         (0x1FU << VIRTIO_DMA_CSR_MSIX_WRREQ_FIFO_LVL_SHIFT)

#ifdef GENHDR_STRUCT
/* MSI-X write request FIFO level [DEBUG REG] */
typedef union {
  struct {
    uint32_t lvl : 5;   /* Entries used in write request FIFO. Depth = 2^4. */
    uint32_t : 27;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_MSIX_WRREQ_FIFO_t;
#endif /* GENHDR_STRUCT */

/* MSI-X incoming interrupt maximum FIFO levels [DEBUG REG] */
#define VIRTIO_DMA_CSR_MSIX_IN_FIFO_MAX(msix_in_fifo_max_ix) (0x00016920U + ((msix_in_fifo_max_ix) * 4))
#define VIRTIO_DMA_CSR_MSIX_IN_FIFO_MAX_MAX_INDEX       0x3U
/* Maximum entries used in incoming interrupt FIFO for this interrupt channel. */
#define VIRTIO_DMA_CSR_MSIX_IN_FIFO_MAX_LVL_ACCESSTYPE  GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_MSIX_IN_FIFO_MAX_LVL_SHIFT       0
#define VIRTIO_DMA_CSR_MSIX_IN_FIFO_MAX_LVL_WIDTH       12
#define VIRTIO_DMA_CSR_MSIX_IN_FIFO_MAX_LVL_MASK        (0xFFFU << VIRTIO_DMA_CSR_MSIX_IN_FIFO_MAX_LVL_SHIFT)

#ifdef GENHDR_STRUCT
/* MSI-X incoming interrupt maximum FIFO levels [DEBUG REG] */
typedef union {
  struct {
    uint32_t lvl : 12;  /* Maximum entries used in incoming interrupt FIFO for this interrupt channel. */
    uint32_t : 20;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_MSIX_IN_FIFO_MAX_t;
#endif /* GENHDR_STRUCT */

/* MSI-X outgoing interrupt maximum FIFO level [DEBUG REG] */
#define VIRTIO_DMA_CSR_MSIX_OUT_FIFO_MAX                0x00016930U
/* Maximum entries used in outgoing interrupt FIFO. */
#define VIRTIO_DMA_CSR_MSIX_OUT_FIFO_MAX_LVL_ACCESSTYPE GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_MSIX_OUT_FIFO_MAX_LVL_SHIFT      0
#define VIRTIO_DMA_CSR_MSIX_OUT_FIFO_MAX_LVL_WIDTH      6
#define VIRTIO_DMA_CSR_MSIX_OUT_FIFO_MAX_LVL_MASK       (0x3FU << VIRTIO_DMA_CSR_MSIX_OUT_FIFO_MAX_LVL_SHIFT)

#ifdef GENHDR_STRUCT
/* MSI-X outgoing interrupt maximum FIFO level [DEBUG REG] */
typedef union {
  struct {
    uint32_t lvl : 6;   /* Maximum entries used in outgoing interrupt FIFO. */
    uint32_t : 26;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_MSIX_OUT_FIFO_MAX_t;
#endif /* GENHDR_STRUCT */

/* MSI-X write request maximum FIFO level [DEBUG REG] */
#define VIRTIO_DMA_CSR_MSIX_WRREQ_FIFO_MAX                      0x00016934U
/* Maximum entries used in write request FIFO. */
#define VIRTIO_DMA_CSR_MSIX_WRREQ_FIFO_MAX_LVL_ACCESSTYPE       GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_MSIX_WRREQ_FIFO_MAX_LVL_SHIFT            0
#define VIRTIO_DMA_CSR_MSIX_WRREQ_FIFO_MAX_LVL_WIDTH            5
#define VIRTIO_DMA_CSR_MSIX_WRREQ_FIFO_MAX_LVL_MASK             (0x1FU << VIRTIO_DMA_CSR_MSIX_WRREQ_FIFO_MAX_LVL_SHIFT)

#ifdef GENHDR_STRUCT
/* MSI-X write request maximum FIFO level [DEBUG REG] */
typedef union {
  struct {
    uint32_t lvl : 5;   /* Maximum entries used in write request FIFO. */
    uint32_t : 27;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_MSIX_WRREQ_FIFO_MAX_t;
#endif /* GENHDR_STRUCT */

/* MSI-X FIFO overflow hold status [DEBUG REG] */
#define VIRTIO_DMA_CSR_MSIX_OFLW_HOLD                           0x00016938U
/* Overflow in incoming interrupt FIFO ch[3:0]. */
#define VIRTIO_DMA_CSR_MSIX_OFLW_HOLD_IN_ACCESSTYPE             GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_MSIX_OFLW_HOLD_IN_SHIFT                  0
#define VIRTIO_DMA_CSR_MSIX_OFLW_HOLD_IN_WIDTH                  4
#define VIRTIO_DMA_CSR_MSIX_OFLW_HOLD_IN_MASK                   (0xFU << VIRTIO_DMA_CSR_MSIX_OFLW_HOLD_IN_SHIFT)

/* Overflow in outgoing interrupt FIFO. */
#define VIRTIO_DMA_CSR_MSIX_OFLW_HOLD_OUT_ACCESSTYPE            GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_MSIX_OFLW_HOLD_OUT_SHIFT                 4
#define VIRTIO_DMA_CSR_MSIX_OFLW_HOLD_OUT_WIDTH                 1
#define VIRTIO_DMA_CSR_MSIX_OFLW_HOLD_OUT_MASK                  (0x1U << VIRTIO_DMA_CSR_MSIX_OFLW_HOLD_OUT_SHIFT)

/* Overflow in write request FIFO. */
#define VIRTIO_DMA_CSR_MSIX_OFLW_HOLD_WRREQ_ACCESSTYPE          GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_MSIX_OFLW_HOLD_WRREQ_SHIFT               5
#define VIRTIO_DMA_CSR_MSIX_OFLW_HOLD_WRREQ_WIDTH               1
#define VIRTIO_DMA_CSR_MSIX_OFLW_HOLD_WRREQ_MASK                (0x1U << VIRTIO_DMA_CSR_MSIX_OFLW_HOLD_WRREQ_SHIFT)

/* SOP/EOP protocol error on wr_req input from DMA. */
#define VIRTIO_DMA_CSR_MSIX_OFLW_HOLD_PROT_ERR_ACCESSTYPE       GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_MSIX_OFLW_HOLD_PROT_ERR_SHIFT            6
#define VIRTIO_DMA_CSR_MSIX_OFLW_HOLD_PROT_ERR_WIDTH            1
#define VIRTIO_DMA_CSR_MSIX_OFLW_HOLD_PROT_ERR_MASK             (0x1U << VIRTIO_DMA_CSR_MSIX_OFLW_HOLD_PROT_ERR_SHIFT)

#ifdef GENHDR_STRUCT
/* MSI-X FIFO overflow hold status [DEBUG REG] */
typedef union {
  struct {
    uint32_t in : 4;            /* Overflow in incoming interrupt FIFO ch[3:0]. */
    uint32_t out : 1;           /* Overflow in outgoing interrupt FIFO. */
    uint32_t wrreq : 1;         /* Overflow in write request FIFO. */
    uint32_t prot_err : 1;      /* SOP/EOP protocol error on wr_req input from DMA. */
    uint32_t : 25;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_MSIX_OFLW_HOLD_t;
#endif /* GENHDR_STRUCT */

/* MSI-X out of range address hold status [DEBUG REG] */
#define VIRTIO_DMA_CSR_MSIX_OOR_HOLD                            0x0001693CU
/* Address range violation on PCIE_CSR AVMM interface. */
#define VIRTIO_DMA_CSR_MSIX_OOR_HOLD_AVMM_CSR_ACCESSTYPE        GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_MSIX_OOR_HOLD_AVMM_CSR_SHIFT             0
#define VIRTIO_DMA_CSR_MSIX_OOR_HOLD_AVMM_CSR_WIDTH             1
#define VIRTIO_DMA_CSR_MSIX_OOR_HOLD_AVMM_CSR_MASK              (0x1U << VIRTIO_DMA_CSR_MSIX_OOR_HOLD_AVMM_CSR_SHIFT)

/* Address range violation on interrupt lookup in MSI-X table. */
#define VIRTIO_DMA_CSR_MSIX_OOR_HOLD_INT_LKUP_ACCESSTYPE        GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_MSIX_OOR_HOLD_INT_LKUP_SHIFT             1
#define VIRTIO_DMA_CSR_MSIX_OOR_HOLD_INT_LKUP_WIDTH             1
#define VIRTIO_DMA_CSR_MSIX_OOR_HOLD_INT_LKUP_MASK              (0x1U << VIRTIO_DMA_CSR_MSIX_OOR_HOLD_INT_LKUP_SHIFT)

/* Address range violation on MSI-X table AVMM wr interface. */
#define VIRTIO_DMA_CSR_MSIX_OOR_HOLD_AVMM_MTAB_W_ACCESSTYPE     GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_MSIX_OOR_HOLD_AVMM_MTAB_W_SHIFT          2
#define VIRTIO_DMA_CSR_MSIX_OOR_HOLD_AVMM_MTAB_W_WIDTH          1
#define VIRTIO_DMA_CSR_MSIX_OOR_HOLD_AVMM_MTAB_W_MASK           (0x1U << VIRTIO_DMA_CSR_MSIX_OOR_HOLD_AVMM_MTAB_W_SHIFT)

/* Address range violation on MSI-X table AVMM rd interface. */
#define VIRTIO_DMA_CSR_MSIX_OOR_HOLD_AVMM_MTAB_R_ACCESSTYPE     GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_MSIX_OOR_HOLD_AVMM_MTAB_R_SHIFT          3
#define VIRTIO_DMA_CSR_MSIX_OOR_HOLD_AVMM_MTAB_R_WIDTH          1
#define VIRTIO_DMA_CSR_MSIX_OOR_HOLD_AVMM_MTAB_R_MASK           (0x1U << VIRTIO_DMA_CSR_MSIX_OOR_HOLD_AVMM_MTAB_R_SHIFT)

#ifdef GENHDR_STRUCT
/* MSI-X out of range address hold status [DEBUG REG] */
typedef union {
  struct {
    uint32_t avmm_csr : 1;      /* Address range violation on PCIE_CSR AVMM interface. */
    uint32_t int_lkup : 1;      /* Address range violation on interrupt lookup in MSI-X table. */
    uint32_t avmm_mtab_w : 1;   /* Address range violation on MSI-X table AVMM wr interface. */
    uint32_t avmm_mtab_r : 1;   /* Address range violation on MSI-X table AVMM rd interface. */
    uint32_t : 28;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_MSIX_OOR_HOLD_t;
#endif /* GENHDR_STRUCT */

/* HIP Interface utilization rates [DEBUG REG] */
#define VIRTIO_DMA_CSR_TLP_ARATE_HIP                    0x00017000U
/* Percentage of cycles (xxx.y%) where HIP consumes TLPs from TLP Adaptor. */
#define VIRTIO_DMA_CSR_TLP_ARATE_HIP_TLP_2HIP_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TLP_ARATE_HIP_TLP_2HIP_SHIFT     0
#define VIRTIO_DMA_CSR_TLP_ARATE_HIP_TLP_2HIP_WIDTH     10
#define VIRTIO_DMA_CSR_TLP_ARATE_HIP_TLP_2HIP_MASK      (0x3FFU << VIRTIO_DMA_CSR_TLP_ARATE_HIP_TLP_2HIP_SHIFT)

/* Percentage of cycles (xxx.y%) where TLP Adaptor consumes TLPs from HIP. */
#define VIRTIO_DMA_CSR_TLP_ARATE_HIP_HIP_2TLP_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TLP_ARATE_HIP_HIP_2TLP_SHIFT     10
#define VIRTIO_DMA_CSR_TLP_ARATE_HIP_HIP_2TLP_WIDTH     10
#define VIRTIO_DMA_CSR_TLP_ARATE_HIP_HIP_2TLP_MASK      (0x3FFU << VIRTIO_DMA_CSR_TLP_ARATE_HIP_HIP_2TLP_SHIFT)

#ifdef GENHDR_STRUCT
/* HIP Interface utilization rates [DEBUG REG] */
typedef union {
  struct {
    uint32_t tlp_2hip : 10;     /* Percentage of cycles (xxx.y%) where HIP consumes TLPs from TLP Adaptor. */
    uint32_t hip_2tlp : 10;     /* Percentage of cycles (xxx.y%) where TLP Adaptor consumes TLPs from HIP. */
    uint32_t : 12;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TLP_ARATE_HIP_t;
#endif /* GENHDR_STRUCT */

/* DMA Interface utilization rates [DEBUG REG] */
#define VIRTIO_DMA_CSR_TLP_ARATE_DMA                            0x00017004U
/*
 * Percentage of cycles (xxx.y%) where TLP Adaptor consumes write requests from
 * DMA.
 */
#define VIRTIO_DMA_CSR_TLP_ARATE_DMA_DMA_WR_2TLP_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TLP_ARATE_DMA_DMA_WR_2TLP_SHIFT          0
#define VIRTIO_DMA_CSR_TLP_ARATE_DMA_DMA_WR_2TLP_WIDTH          10
#define VIRTIO_DMA_CSR_TLP_ARATE_DMA_DMA_WR_2TLP_MASK           (0x3FFU << VIRTIO_DMA_CSR_TLP_ARATE_DMA_DMA_WR_2TLP_SHIFT)

/*
 * Percentage of cycles (xxx.y%) where TLP Adaptor consumes read requests from
 * DMA.
 */
#define VIRTIO_DMA_CSR_TLP_ARATE_DMA_DMA_RD_2TLP_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TLP_ARATE_DMA_DMA_RD_2TLP_SHIFT          10
#define VIRTIO_DMA_CSR_TLP_ARATE_DMA_DMA_RD_2TLP_WIDTH          10
#define VIRTIO_DMA_CSR_TLP_ARATE_DMA_DMA_RD_2TLP_MASK           (0x3FFU << VIRTIO_DMA_CSR_TLP_ARATE_DMA_DMA_RD_2TLP_SHIFT)

/*
 * Percentage of cycles (xxx.y%) where TLP Adaptor sends read response data to
 * the DMA.
 */
#define VIRTIO_DMA_CSR_TLP_ARATE_DMA_TLP_2DMA_RD_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TLP_ARATE_DMA_TLP_2DMA_RD_SHIFT          20
#define VIRTIO_DMA_CSR_TLP_ARATE_DMA_TLP_2DMA_RD_WIDTH          10
#define VIRTIO_DMA_CSR_TLP_ARATE_DMA_TLP_2DMA_RD_MASK           (0x3FFU << VIRTIO_DMA_CSR_TLP_ARATE_DMA_TLP_2DMA_RD_SHIFT)

#ifdef GENHDR_STRUCT
/* DMA Interface utilization rates [DEBUG REG] */
typedef union {
  struct {
    uint32_t dma_wr_2tlp : 10;  /* Percentage of cycles (xxx.y%) where TLP Adaptor consumes write requests from
                                   DMA. */
    uint32_t dma_rd_2tlp : 10;  /* Percentage of cycles (xxx.y%) where TLP Adaptor consumes read requests from
                                   DMA. */
    uint32_t tlp_2dma_rd : 10;  /* Percentage of cycles (xxx.y%) where TLP Adaptor sends read response data to
                                   the DMA. */
    uint32_t : 2;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TLP_ARATE_DMA_t;
#endif /* GENHDR_STRUCT */

/* HIP Interface stalled rates [DEBUG REG] */
#define VIRTIO_DMA_CSR_TLP_SRATE_HIP                    0x00017008U
/* Percentage of cycles (xxx.y%) where HIP blocks TLPs from TLP Adaptor. */
#define VIRTIO_DMA_CSR_TLP_SRATE_HIP_TLP_2HIP_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TLP_SRATE_HIP_TLP_2HIP_SHIFT     0
#define VIRTIO_DMA_CSR_TLP_SRATE_HIP_TLP_2HIP_WIDTH     10
#define VIRTIO_DMA_CSR_TLP_SRATE_HIP_TLP_2HIP_MASK      (0x3FFU << VIRTIO_DMA_CSR_TLP_SRATE_HIP_TLP_2HIP_SHIFT)

/* Percentage of cycles (xxx.y%) where TLP Adaptor blocks TLPs from HIP. */
#define VIRTIO_DMA_CSR_TLP_SRATE_HIP_HIP_2TLP_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TLP_SRATE_HIP_HIP_2TLP_SHIFT     10
#define VIRTIO_DMA_CSR_TLP_SRATE_HIP_HIP_2TLP_WIDTH     10
#define VIRTIO_DMA_CSR_TLP_SRATE_HIP_HIP_2TLP_MASK      (0x3FFU << VIRTIO_DMA_CSR_TLP_SRATE_HIP_HIP_2TLP_SHIFT)

#ifdef GENHDR_STRUCT
/* HIP Interface stalled rates [DEBUG REG] */
typedef union {
  struct {
    uint32_t tlp_2hip : 10;     /* Percentage of cycles (xxx.y%) where HIP blocks TLPs from TLP Adaptor. */
    uint32_t hip_2tlp : 10;     /* Percentage of cycles (xxx.y%) where TLP Adaptor blocks TLPs from HIP. */
    uint32_t : 12;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TLP_SRATE_HIP_t;
#endif /* GENHDR_STRUCT */

/* DMA Interface stalled rates [DEBUG REG] */
#define VIRTIO_DMA_CSR_TLP_SRATE_DMA                            0x0001700CU
/*
 * Percentage of cycles (xxx.y%) where TLP Adaptor blocks write requests from
 * DMA.
 */
#define VIRTIO_DMA_CSR_TLP_SRATE_DMA_DMA_WR_2TLP_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TLP_SRATE_DMA_DMA_WR_2TLP_SHIFT          0
#define VIRTIO_DMA_CSR_TLP_SRATE_DMA_DMA_WR_2TLP_WIDTH          10
#define VIRTIO_DMA_CSR_TLP_SRATE_DMA_DMA_WR_2TLP_MASK           (0x3FFU << VIRTIO_DMA_CSR_TLP_SRATE_DMA_DMA_WR_2TLP_SHIFT)

/*
 * Percentage of cycles (xxx.y%) where TLP Adaptor blocks read requests from
 * DMA.
 */
#define VIRTIO_DMA_CSR_TLP_SRATE_DMA_DMA_RD_2TLP_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TLP_SRATE_DMA_DMA_RD_2TLP_SHIFT          10
#define VIRTIO_DMA_CSR_TLP_SRATE_DMA_DMA_RD_2TLP_WIDTH          10
#define VIRTIO_DMA_CSR_TLP_SRATE_DMA_DMA_RD_2TLP_MASK           (0x3FFU << VIRTIO_DMA_CSR_TLP_SRATE_DMA_DMA_RD_2TLP_SHIFT)

#ifdef GENHDR_STRUCT
/* DMA Interface stalled rates [DEBUG REG] */
typedef union {
  struct {
    uint32_t dma_wr_2tlp : 10;  /* Percentage of cycles (xxx.y%) where TLP Adaptor blocks write requests from
                                   DMA. */
    uint32_t dma_rd_2tlp : 10;  /* Percentage of cycles (xxx.y%) where TLP Adaptor blocks read requests from
                                   DMA. */
    uint32_t : 12;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TLP_SRATE_DMA_t;
#endif /* GENHDR_STRUCT */

/* Lack of credits rates [DEBUG REG] */
#define VIRTIO_DMA_CSR_TLP_SRATE_CRED                   0x00017010U
/* Percentage of cycles (xxx.y%) with lack of credits for posted TLPs (writes). */
#define VIRTIO_DMA_CSR_TLP_SRATE_CRED_NP_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TLP_SRATE_CRED_NP_SHIFT          0
#define VIRTIO_DMA_CSR_TLP_SRATE_CRED_NP_WIDTH          10
#define VIRTIO_DMA_CSR_TLP_SRATE_CRED_NP_MASK           (0x3FFU << VIRTIO_DMA_CSR_TLP_SRATE_CRED_NP_SHIFT)

/*
 * Percentage of cycles (xxx.y%) with lack of credits for non-posted TLPs
 * (reads).
 */
#define VIRTIO_DMA_CSR_TLP_SRATE_CRED_P_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TLP_SRATE_CRED_P_SHIFT           10
#define VIRTIO_DMA_CSR_TLP_SRATE_CRED_P_WIDTH           10
#define VIRTIO_DMA_CSR_TLP_SRATE_CRED_P_MASK            (0x3FFU << VIRTIO_DMA_CSR_TLP_SRATE_CRED_P_SHIFT)

/*
 * Percentage of cycles (xxx.y%) with lack of credits for completions (read data
 * back to host).
 */
#define VIRTIO_DMA_CSR_TLP_SRATE_CRED_CPL_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TLP_SRATE_CRED_CPL_SHIFT         20
#define VIRTIO_DMA_CSR_TLP_SRATE_CRED_CPL_WIDTH         10
#define VIRTIO_DMA_CSR_TLP_SRATE_CRED_CPL_MASK          (0x3FFU << VIRTIO_DMA_CSR_TLP_SRATE_CRED_CPL_SHIFT)

#ifdef GENHDR_STRUCT
/* Lack of credits rates [DEBUG REG] */
typedef union {
  struct {
    uint32_t np : 10;   /* Percentage of cycles (xxx.y%) with lack of credits for posted TLPs (writes). */
    uint32_t p : 10;    /* Percentage of cycles (xxx.y%) with lack of credits for non-posted TLPs
                           (reads). */
    uint32_t cpl : 10;  /* Percentage of cycles (xxx.y%) with lack of credits for completions (read data
                           back to host). */
    uint32_t : 2;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TLP_SRATE_CRED_t;
#endif /* GENHDR_STRUCT */

/* Lack of tags rate [DEBUG REG] */
#define VIRTIO_DMA_CSR_TLP_SRATE_TAG                    0x00017014U
/* Percentage of cycles (xxx.y%) with lack of tags for read TLPs to HIP. */
#define VIRTIO_DMA_CSR_TLP_SRATE_TAG_RATE_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TLP_SRATE_TAG_RATE_SHIFT         0
#define VIRTIO_DMA_CSR_TLP_SRATE_TAG_RATE_WIDTH         10
#define VIRTIO_DMA_CSR_TLP_SRATE_TAG_RATE_MASK          (0x3FFU << VIRTIO_DMA_CSR_TLP_SRATE_TAG_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Lack of tags rate [DEBUG REG] */
typedef union {
  struct {
    uint32_t rate : 10; /* Percentage of cycles (xxx.y%) with lack of tags for read TLPs to HIP. */
    uint32_t : 22;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TLP_SRATE_TAG_t;
#endif /* GENHDR_STRUCT */

/* Number of read tags available [DEBUG REG] */
#define VIRTIO_DMA_CSR_TLP_TAG_LVL                      0x00017018U
/* Number of tags available for read requests to host. */
#define VIRTIO_DMA_CSR_TLP_TAG_LVL_LVL_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TLP_TAG_LVL_LVL_SHIFT            0
#define VIRTIO_DMA_CSR_TLP_TAG_LVL_LVL_WIDTH            9
#define VIRTIO_DMA_CSR_TLP_TAG_LVL_LVL_MASK             (0x1FFU << VIRTIO_DMA_CSR_TLP_TAG_LVL_LVL_SHIFT)

#ifdef GENHDR_STRUCT
/* Number of read tags available [DEBUG REG] */
typedef union {
  struct {
    uint32_t lvl : 9;   /* Number of tags available for read requests to host. */
    uint32_t : 23;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TLP_TAG_LVL_t;
#endif /* GENHDR_STRUCT */

/* Non posted credits available (Rd to host) [DEBUG REG] */
#define VIRTIO_DMA_CSR_TLP_CRED_NP                      0x0001701CU
/* Available header credits. */
#define VIRTIO_DMA_CSR_TLP_CRED_NP_HDR_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TLP_CRED_NP_HDR_SHIFT            0
#define VIRTIO_DMA_CSR_TLP_CRED_NP_HDR_WIDTH            8
#define VIRTIO_DMA_CSR_TLP_CRED_NP_HDR_MASK             (0xFFU << VIRTIO_DMA_CSR_TLP_CRED_NP_HDR_SHIFT)

#ifdef GENHDR_STRUCT
/* Non posted credits available (Rd to host) [DEBUG REG] */
typedef union {
  struct {
    uint32_t hdr : 8;   /* Available header credits. */
    uint32_t : 24;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TLP_CRED_NP_t;
#endif /* GENHDR_STRUCT */

/* Posted credits available (Wr to host) [DEBUG REG] */
#define VIRTIO_DMA_CSR_TLP_CRED_P                       0x00017020U
/* Available header credits. */
#define VIRTIO_DMA_CSR_TLP_CRED_P_HDR_ACCESSTYPE        GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TLP_CRED_P_HDR_SHIFT             0
#define VIRTIO_DMA_CSR_TLP_CRED_P_HDR_WIDTH             8
#define VIRTIO_DMA_CSR_TLP_CRED_P_HDR_MASK              (0xFFU << VIRTIO_DMA_CSR_TLP_CRED_P_HDR_SHIFT)

/* Available data credits. */
#define VIRTIO_DMA_CSR_TLP_CRED_P_DATA_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TLP_CRED_P_DATA_SHIFT            16
#define VIRTIO_DMA_CSR_TLP_CRED_P_DATA_WIDTH            12
#define VIRTIO_DMA_CSR_TLP_CRED_P_DATA_MASK             (0xFFFU << VIRTIO_DMA_CSR_TLP_CRED_P_DATA_SHIFT)

#ifdef GENHDR_STRUCT
/* Posted credits available (Wr to host) [DEBUG REG] */
typedef union {
  struct {
    uint32_t hdr : 8;   /* Available header credits. */
    uint32_t : 8;
    uint32_t data : 12; /* Available data credits. */
    uint32_t : 4;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TLP_CRED_P_t;
#endif /* GENHDR_STRUCT */

/* Completion credits available (Cpl to host)[DEBUG REG]  */
#define VIRTIO_DMA_CSR_TLP_CRED_CPL                     0x00017024U
/* Available header credits. */
#define VIRTIO_DMA_CSR_TLP_CRED_CPL_HDR_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TLP_CRED_CPL_HDR_SHIFT           0
#define VIRTIO_DMA_CSR_TLP_CRED_CPL_HDR_WIDTH           8
#define VIRTIO_DMA_CSR_TLP_CRED_CPL_HDR_MASK            (0xFFU << VIRTIO_DMA_CSR_TLP_CRED_CPL_HDR_SHIFT)

/* Available data credits. */
#define VIRTIO_DMA_CSR_TLP_CRED_CPL_DATA_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TLP_CRED_CPL_DATA_SHIFT          16
#define VIRTIO_DMA_CSR_TLP_CRED_CPL_DATA_WIDTH          12
#define VIRTIO_DMA_CSR_TLP_CRED_CPL_DATA_MASK           (0xFFFU << VIRTIO_DMA_CSR_TLP_CRED_CPL_DATA_SHIFT)

#ifdef GENHDR_STRUCT
/* Completion credits available (Cpl to host)[DEBUG REG]  */
typedef union {
  struct {
    uint32_t hdr : 8;   /* Available header credits. */
    uint32_t : 8;
    uint32_t data : 12; /* Available data credits. */
    uint32_t : 4;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TLP_CRED_CPL_t;
#endif /* GENHDR_STRUCT */

/* Warnings and critical errors sticky bits [DEBUG REG] */
#define VIRTIO_DMA_CSR_TLP_HOLD                                 0x00017028U
/* No read tags available for reads towards host. */
#define VIRTIO_DMA_CSR_TLP_HOLD_NO_RD_TAGS_ACCESSTYPE           GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_NO_RD_TAGS_SHIFT                0
#define VIRTIO_DMA_CSR_TLP_HOLD_NO_RD_TAGS_WIDTH                1
#define VIRTIO_DMA_CSR_TLP_HOLD_NO_RD_TAGS_MASK                 (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_NO_RD_TAGS_SHIFT)

/* No posted credits available for writes towards host. */
#define VIRTIO_DMA_CSR_TLP_HOLD_NO_WR_CRED_ACCESSTYPE           GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_NO_WR_CRED_SHIFT                1
#define VIRTIO_DMA_CSR_TLP_HOLD_NO_WR_CRED_WIDTH                1
#define VIRTIO_DMA_CSR_TLP_HOLD_NO_WR_CRED_MASK                 (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_NO_WR_CRED_SHIFT)

/* No non-posted credits available for reads towards host. */
#define VIRTIO_DMA_CSR_TLP_HOLD_NO_RD_CRED_ACCESSTYPE           GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_NO_RD_CRED_SHIFT                2
#define VIRTIO_DMA_CSR_TLP_HOLD_NO_RD_CRED_WIDTH                1
#define VIRTIO_DMA_CSR_TLP_HOLD_NO_RD_CRED_MASK                 (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_NO_RD_CRED_SHIFT)

/* No completion credits available for completions towards host. */
#define VIRTIO_DMA_CSR_TLP_HOLD_NO_CPL_CRED_ACCESSTYPE          GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_NO_CPL_CRED_SHIFT               3
#define VIRTIO_DMA_CSR_TLP_HOLD_NO_CPL_CRED_WIDTH               1
#define VIRTIO_DMA_CSR_TLP_HOLD_NO_CPL_CRED_MASK                (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_NO_CPL_CRED_SHIFT)

/* Overflow detected in DMA write request FIFO. */
#define VIRTIO_DMA_CSR_TLP_HOLD_WR_REQ_FIFO_OFLW_ACCESSTYPE     GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_WR_REQ_FIFO_OFLW_SHIFT          4
#define VIRTIO_DMA_CSR_TLP_HOLD_WR_REQ_FIFO_OFLW_WIDTH          1
#define VIRTIO_DMA_CSR_TLP_HOLD_WR_REQ_FIFO_OFLW_MASK           (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_WR_REQ_FIFO_OFLW_SHIFT)

/* Overflow detected in DMA write TLP FIFO. */
#define VIRTIO_DMA_CSR_TLP_HOLD_WR_TLP_FIFO_OFLW_ACCESSTYPE     GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_WR_TLP_FIFO_OFLW_SHIFT          5
#define VIRTIO_DMA_CSR_TLP_HOLD_WR_TLP_FIFO_OFLW_WIDTH          1
#define VIRTIO_DMA_CSR_TLP_HOLD_WR_TLP_FIFO_OFLW_MASK           (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_WR_TLP_FIFO_OFLW_SHIFT)

/* Overflow detected in DMA read request FIFO. */
#define VIRTIO_DMA_CSR_TLP_HOLD_RD_REQ_FIFO_OFLW_ACCESSTYPE     GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_RD_REQ_FIFO_OFLW_SHIFT          6
#define VIRTIO_DMA_CSR_TLP_HOLD_RD_REQ_FIFO_OFLW_WIDTH          1
#define VIRTIO_DMA_CSR_TLP_HOLD_RD_REQ_FIFO_OFLW_MASK           (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_RD_REQ_FIFO_OFLW_SHIFT)

/* Overflow detected in DMA read response FIFO. */
#define VIRTIO_DMA_CSR_TLP_HOLD_RD_RSP_FIFO_OFLW_ACCESSTYPE     GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_RD_RSP_FIFO_OFLW_SHIFT          7
#define VIRTIO_DMA_CSR_TLP_HOLD_RD_RSP_FIFO_OFLW_WIDTH          1
#define VIRTIO_DMA_CSR_TLP_HOLD_RD_RSP_FIFO_OFLW_MASK           (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_RD_RSP_FIFO_OFLW_SHIFT)

/* SOP/EOP protocol error detected on DMA write request interface. */
#define VIRTIO_DMA_CSR_TLP_HOLD_WR_REQ_PROT_ERR_ACCESSTYPE      GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_WR_REQ_PROT_ERR_SHIFT           8
#define VIRTIO_DMA_CSR_TLP_HOLD_WR_REQ_PROT_ERR_WIDTH           1
#define VIRTIO_DMA_CSR_TLP_HOLD_WR_REQ_PROT_ERR_MASK            (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_WR_REQ_PROT_ERR_SHIFT)

/* Overflow detected in TLP Adaptor to HIP interface FIFO. */
#define VIRTIO_DMA_CSR_TLP_HOLD_HIP_IF_FIFO_OFLW_ACCESSTYPE     GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_HIP_IF_FIFO_OFLW_SHIFT          9
#define VIRTIO_DMA_CSR_TLP_HOLD_HIP_IF_FIFO_OFLW_WIDTH          1
#define VIRTIO_DMA_CSR_TLP_HOLD_HIP_IF_FIFO_OFLW_MASK           (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_HIP_IF_FIFO_OFLW_SHIFT)

/* Overflow detected in AVMM request FIFO. */
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_REQ_FIFO_OFLW_ACCESSTYPE     GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_REQ_FIFO_OFLW_SHIFT          10
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_REQ_FIFO_OFLW_WIDTH          1
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_REQ_FIFO_OFLW_MASK           (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_AV_REQ_FIFO_OFLW_SHIFT)

/* Overflow detected in AVMM response control FIFO. */
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_CTL_FIFO_OFLW_ACCESSTYPE     GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_CTL_FIFO_OFLW_SHIFT          11
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_CTL_FIFO_OFLW_WIDTH          1
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_CTL_FIFO_OFLW_MASK           (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_AV_CTL_FIFO_OFLW_SHIFT)

/* Overflow detected in AVMM response TLP FIFO. */
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_TLP_FIFO_OFLW_ACCESSTYPE     GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_TLP_FIFO_OFLW_SHIFT          12
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_TLP_FIFO_OFLW_WIDTH          1
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_TLP_FIFO_OFLW_MASK           (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_AV_TLP_FIFO_OFLW_SHIFT)

/* Bad read request received from host (extends beyond one 4 byte word). */
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_BAD_RD_REQ_ACCESSTYPE        GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_BAD_RD_REQ_SHIFT             13
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_BAD_RD_REQ_WIDTH             1
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_BAD_RD_REQ_MASK              (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_AV_BAD_RD_REQ_SHIFT)

/* Bad write request received from host (extends beyond one 4 byte word). */
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_BAD_WR_REQ_ACCESSTYPE        GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_BAD_WR_REQ_SHIFT             14
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_BAD_WR_REQ_WIDTH             1
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_BAD_WR_REQ_MASK              (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_AV_BAD_WR_REQ_SHIFT)

/* Unexpected read response received (no read issued to core for response). */
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_RD_RSP_ERR_ACCESSTYPE        GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_RD_RSP_ERR_SHIFT             15
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_RD_RSP_ERR_WIDTH             1
#define VIRTIO_DMA_CSR_TLP_HOLD_AV_RD_RSP_ERR_MASK              (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_AV_RD_RSP_ERR_SHIFT)

/* Overflow detected in read tag FIFO. */
#define VIRTIO_DMA_CSR_TLP_HOLD_TAG_FIFO_OFLW_ACCESSTYPE        GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_TAG_FIFO_OFLW_SHIFT             16
#define VIRTIO_DMA_CSR_TLP_HOLD_TAG_FIFO_OFLW_WIDTH             1
#define VIRTIO_DMA_CSR_TLP_HOLD_TAG_FIFO_OFLW_MASK              (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_TAG_FIFO_OFLW_SHIFT)

/* Completion received from host with status=UR (Unsupported Request). */
#define VIRTIO_DMA_CSR_TLP_HOLD_CPL_STAT_UR_ACCESSTYPE          GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_CPL_STAT_UR_SHIFT               17
#define VIRTIO_DMA_CSR_TLP_HOLD_CPL_STAT_UR_WIDTH               1
#define VIRTIO_DMA_CSR_TLP_HOLD_CPL_STAT_UR_MASK                (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_CPL_STAT_UR_SHIFT)

/* Completion received from host with status=CA (Completion Abort). */
#define VIRTIO_DMA_CSR_TLP_HOLD_CPL_STAT_CA_ACCESSTYPE          GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_CPL_STAT_CA_SHIFT               18
#define VIRTIO_DMA_CSR_TLP_HOLD_CPL_STAT_CA_WIDTH               1
#define VIRTIO_DMA_CSR_TLP_HOLD_CPL_STAT_CA_MASK                (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_CPL_STAT_CA_SHIFT)

/* Completion received from host with other bad status=(!OK && !UR && !CA). */
#define VIRTIO_DMA_CSR_TLP_HOLD_CPL_STAT_BAD_ACCESSTYPE         GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_CPL_STAT_BAD_SHIFT              19
#define VIRTIO_DMA_CSR_TLP_HOLD_CPL_STAT_BAD_WIDTH              1
#define VIRTIO_DMA_CSR_TLP_HOLD_CPL_STAT_BAD_MASK               (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_CPL_STAT_BAD_SHIFT)

/* Completion received from host with poisoned data (EP). */
#define VIRTIO_DMA_CSR_TLP_HOLD_CPL_STAT_EP_ACCESSTYPE          GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_CPL_STAT_EP_SHIFT               20
#define VIRTIO_DMA_CSR_TLP_HOLD_CPL_STAT_EP_WIDTH               1
#define VIRTIO_DMA_CSR_TLP_HOLD_CPL_STAT_EP_MASK                (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_CPL_STAT_EP_SHIFT)

/* Out Of Order completion received. */
#define VIRTIO_DMA_CSR_TLP_HOLD_CPL_OOO_ACCESSTYPE              GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_CPL_OOO_SHIFT                   21
#define VIRTIO_DMA_CSR_TLP_HOLD_CPL_OOO_WIDTH                   1
#define VIRTIO_DMA_CSR_TLP_HOLD_CPL_OOO_MASK                    (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_CPL_OOO_SHIFT)

/*
 * A write to a debug register was attempted where not all byte enables was
 * asserted. Write operation ignored.
 */
#define VIRTIO_DMA_CSR_TLP_HOLD_ILL_REG_WR_ACCESSTYPE           GENHDR_ACCESSTYPE_W1C
#define VIRTIO_DMA_CSR_TLP_HOLD_ILL_REG_WR_SHIFT                22
#define VIRTIO_DMA_CSR_TLP_HOLD_ILL_REG_WR_WIDTH                1
#define VIRTIO_DMA_CSR_TLP_HOLD_ILL_REG_WR_MASK                 (0x1U << VIRTIO_DMA_CSR_TLP_HOLD_ILL_REG_WR_SHIFT)

#ifdef GENHDR_STRUCT
/* Warnings and critical errors sticky bits [DEBUG REG] */
typedef union {
  struct {
    uint32_t no_rd_tags : 1;            /* No read tags available for reads towards host. */
    uint32_t no_wr_cred : 1;            /* No posted credits available for writes towards host. */
    uint32_t no_rd_cred : 1;            /* No non-posted credits available for reads towards host. */
    uint32_t no_cpl_cred : 1;           /* No completion credits available for completions towards host. */
    uint32_t wr_req_fifo_oflw : 1;      /* Overflow detected in DMA write request FIFO. */
    uint32_t wr_tlp_fifo_oflw : 1;      /* Overflow detected in DMA write TLP FIFO. */
    uint32_t rd_req_fifo_oflw : 1;      /* Overflow detected in DMA read request FIFO. */
    uint32_t rd_rsp_fifo_oflw : 1;      /* Overflow detected in DMA read response FIFO. */
    uint32_t wr_req_prot_err : 1;       /* SOP/EOP protocol error detected on DMA write request interface. */
    uint32_t hip_if_fifo_oflw : 1;      /* Overflow detected in TLP Adaptor to HIP interface FIFO. */
    uint32_t av_req_fifo_oflw : 1;      /* Overflow detected in AVMM request FIFO. */
    uint32_t av_ctl_fifo_oflw : 1;      /* Overflow detected in AVMM response control FIFO. */
    uint32_t av_tlp_fifo_oflw : 1;      /* Overflow detected in AVMM response TLP FIFO. */
    uint32_t av_bad_rd_req : 1;         /* Bad read request received from host (extends beyond one 4 byte word). */
    uint32_t av_bad_wr_req : 1;         /* Bad write request received from host (extends beyond one 4 byte word). */
    uint32_t av_rd_rsp_err : 1;         /* Unexpected read response received (no read issued to core for response). */
    uint32_t tag_fifo_oflw : 1;         /* Overflow detected in read tag FIFO. */
    uint32_t cpl_stat_ur : 1;           /* Completion received from host with status=UR (Unsupported Request). */
    uint32_t cpl_stat_ca : 1;           /* Completion received from host with status=CA (Completion Abort). */
    uint32_t cpl_stat_bad : 1;          /* Completion received from host with other bad status=(!OK && !UR && !CA). */
    uint32_t cpl_stat_ep : 1;           /* Completion received from host with poisoned data (EP). */
    uint32_t cpl_ooo : 1;               /* Out Of Order completion received. */
    uint32_t ill_reg_wr : 1;            /* A write to a debug register was attempted where not all byte enables was
                                           asserted. Write operation ignored. */
    uint32_t : 9;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TLP_HOLD_t;
#endif /* GENHDR_STRUCT */

/* TLP Adaptor configuration [DEBUG REG] */
#define VIRTIO_DMA_CSR_TLP_CFG                          0x0001702CU
/*
 * Minimum number of write request TLPs for which credits are available in order
 * include writes in arbitration.
 */
#define VIRTIO_DMA_CSR_TLP_CFG_WR_TLP_CRED_SHIFT        0
#define VIRTIO_DMA_CSR_TLP_CFG_WR_TLP_CRED_WIDTH        3
#define VIRTIO_DMA_CSR_TLP_CFG_WR_TLP_CRED_MASK         (0x7U << VIRTIO_DMA_CSR_TLP_CFG_WR_TLP_CRED_SHIFT)

/*
 * Minimum number of read request TLPs for which credits are available in order
 * include reads in arbitration.
 */
#define VIRTIO_DMA_CSR_TLP_CFG_RD_TLP_CRED_SHIFT        3
#define VIRTIO_DMA_CSR_TLP_CFG_RD_TLP_CRED_WIDTH        3
#define VIRTIO_DMA_CSR_TLP_CFG_RD_TLP_CRED_MASK         (0x7U << VIRTIO_DMA_CSR_TLP_CFG_RD_TLP_CRED_SHIFT)

/*
 * Minimum number of completion TLPs for which credits are available in order
 * include completions in arbitration.
 */
#define VIRTIO_DMA_CSR_TLP_CFG_CPL_TLP_CRED_SHIFT       6
#define VIRTIO_DMA_CSR_TLP_CFG_CPL_TLP_CRED_WIDTH       3
#define VIRTIO_DMA_CSR_TLP_CFG_CPL_TLP_CRED_MASK        (0x7U << VIRTIO_DMA_CSR_TLP_CFG_CPL_TLP_CRED_SHIFT)

#ifdef GENHDR_STRUCT
/* TLP Adaptor configuration [DEBUG REG] */
typedef union {
  struct {
    uint32_t wr_tlp_cred : 3;   /* Minimum number of write request TLPs for which credits are available in order
                                   include writes in arbitration. */
    uint32_t rd_tlp_cred : 3;   /* Minimum number of read request TLPs for which credits are available in order
                                   include reads in arbitration. */
    uint32_t cpl_tlp_cred : 3;  /* Minimum number of completion TLPs for which credits are available in order
                                   include completions in arbitration. */
    uint32_t : 23;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TLP_CFG_t;
#endif /* GENHDR_STRUCT */

/* PCIE P Header Credits Space [DEBUG REG] */
#define VIRTIO_DMA_CSR_PCR_PHDR_STAT                            0x00018000U
/* P Header Credit level. Resets to 0. */
#define VIRTIO_DMA_CSR_PCR_PHDR_STAT_CURRENT_LEVEL_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCR_PHDR_STAT_CURRENT_LEVEL_SHIFT        0
#define VIRTIO_DMA_CSR_PCR_PHDR_STAT_CURRENT_LEVEL_WIDTH        10
#define VIRTIO_DMA_CSR_PCR_PHDR_STAT_CURRENT_LEVEL_MASK         (0x3FFU << VIRTIO_DMA_CSR_PCR_PHDR_STAT_CURRENT_LEVEL_SHIFT)

/* Minimum P Header Credit level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_PCR_PHDR_STAT_MIN_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCR_PHDR_STAT_MIN_SHIFT                  10
#define VIRTIO_DMA_CSR_PCR_PHDR_STAT_MIN_WIDTH                  10
#define VIRTIO_DMA_CSR_PCR_PHDR_STAT_MIN_MASK                   (0x3FFU << VIRTIO_DMA_CSR_PCR_PHDR_STAT_MIN_SHIFT)

/*
 * Current P Header Credit Rate when traffic being sent (not latched). Resets to
 * 0.
 */
#define VIRTIO_DMA_CSR_PCR_PHDR_STAT_RATE_ACCESSTYPE            GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCR_PHDR_STAT_RATE_SHIFT                 22
#define VIRTIO_DMA_CSR_PCR_PHDR_STAT_RATE_WIDTH                 10
#define VIRTIO_DMA_CSR_PCR_PHDR_STAT_RATE_MASK                  (0x3FFU << VIRTIO_DMA_CSR_PCR_PHDR_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* PCIE P Header Credits Space [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* P Header Credit level. Resets to 0. */
    uint32_t min : 10;                  /* Minimum P Header Credit level (latched). Resets to 0. */
    uint32_t : 2;
    uint32_t rate : 10;                 /* Current P Header Credit Rate when traffic being sent (not latched). Resets to
                                           0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_PCR_PHDR_STAT_t;
#endif /* GENHDR_STRUCT */

/* PCIe Credit Monitor Statistics Reset [DEBUG REG] */
#define VIRTIO_DMA_CSR_PCR_STATS_RST                            0x00018000U
/* Write 1 to reset all registers in this block. */
#define VIRTIO_DMA_CSR_PCR_STATS_RST_RESET_STATS_ACCESSTYPE     GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_PCR_STATS_RST_RESET_STATS_SHIFT          0
#define VIRTIO_DMA_CSR_PCR_STATS_RST_RESET_STATS_WIDTH          1
#define VIRTIO_DMA_CSR_PCR_STATS_RST_RESET_STATS_MASK           (0x1U << VIRTIO_DMA_CSR_PCR_STATS_RST_RESET_STATS_SHIFT)

#ifdef GENHDR_STRUCT
/* PCIe Credit Monitor Statistics Reset [DEBUG REG] */
typedef union {
  struct {
    uint32_t reset_stats : 1;   /* Write 1 to reset all registers in this block. */
    uint32_t : 31;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_PCR_STATS_RST_t;
#endif /* GENHDR_STRUCT */

/* PCIE P Data Credits Space [DEBUG REG] */
#define VIRTIO_DMA_CSR_PCR_PDAT_STAT                            0x00018004U
/* P Data Credit level. Resets to 0. */
#define VIRTIO_DMA_CSR_PCR_PDAT_STAT_CURRENT_LEVEL_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCR_PDAT_STAT_CURRENT_LEVEL_SHIFT        0
#define VIRTIO_DMA_CSR_PCR_PDAT_STAT_CURRENT_LEVEL_WIDTH        10
#define VIRTIO_DMA_CSR_PCR_PDAT_STAT_CURRENT_LEVEL_MASK         (0x3FFU << VIRTIO_DMA_CSR_PCR_PDAT_STAT_CURRENT_LEVEL_SHIFT)

/* Minimum P Data Credit level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_PCR_PDAT_STAT_MIN_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCR_PDAT_STAT_MIN_SHIFT                  10
#define VIRTIO_DMA_CSR_PCR_PDAT_STAT_MIN_WIDTH                  10
#define VIRTIO_DMA_CSR_PCR_PDAT_STAT_MIN_MASK                   (0x3FFU << VIRTIO_DMA_CSR_PCR_PDAT_STAT_MIN_SHIFT)

/*
 * Current P Data Credit Rate when traffic being stalled (not latched). Resets
 * to 0.
 */
#define VIRTIO_DMA_CSR_PCR_PDAT_STAT_RATE_ACCESSTYPE            GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCR_PDAT_STAT_RATE_SHIFT                 22
#define VIRTIO_DMA_CSR_PCR_PDAT_STAT_RATE_WIDTH                 10
#define VIRTIO_DMA_CSR_PCR_PDAT_STAT_RATE_MASK                  (0x3FFU << VIRTIO_DMA_CSR_PCR_PDAT_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* PCIE P Data Credits Space [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* P Data Credit level. Resets to 0. */
    uint32_t min : 10;                  /* Minimum P Data Credit level (latched). Resets to 0. */
    uint32_t : 2;
    uint32_t rate : 10;                 /* Current P Data Credit Rate when traffic being stalled (not latched). Resets
                                           to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_PCR_PDAT_STAT_t;
#endif /* GENHDR_STRUCT */

/* PCIE NP Header Credits Space [DEBUG REG] */
#define VIRTIO_DMA_CSR_PCR_NPHDR_STAT                           0x00018008U
/* P Header Credit level. Resets to 0. */
#define VIRTIO_DMA_CSR_PCR_NPHDR_STAT_CURRENT_LEVEL_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCR_NPHDR_STAT_CURRENT_LEVEL_SHIFT       0
#define VIRTIO_DMA_CSR_PCR_NPHDR_STAT_CURRENT_LEVEL_WIDTH       10
#define VIRTIO_DMA_CSR_PCR_NPHDR_STAT_CURRENT_LEVEL_MASK        (0x3FFU << VIRTIO_DMA_CSR_PCR_NPHDR_STAT_CURRENT_LEVEL_SHIFT)

/* Minimum P Header Credit level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_PCR_NPHDR_STAT_MIN_ACCESSTYPE            GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCR_NPHDR_STAT_MIN_SHIFT                 10
#define VIRTIO_DMA_CSR_PCR_NPHDR_STAT_MIN_WIDTH                 10
#define VIRTIO_DMA_CSR_PCR_NPHDR_STAT_MIN_MASK                  (0x3FFU << VIRTIO_DMA_CSR_PCR_NPHDR_STAT_MIN_SHIFT)

/* Current P Header Credit Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_PCR_NPHDR_STAT_RATE_ACCESSTYPE           GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCR_NPHDR_STAT_RATE_SHIFT                22
#define VIRTIO_DMA_CSR_PCR_NPHDR_STAT_RATE_WIDTH                10
#define VIRTIO_DMA_CSR_PCR_NPHDR_STAT_RATE_MASK                 (0x3FFU << VIRTIO_DMA_CSR_PCR_NPHDR_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* PCIE NP Header Credits Space [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* P Header Credit level. Resets to 0. */
    uint32_t min : 10;                  /* Minimum P Header Credit level (latched). Resets to 0. */
    uint32_t : 2;
    uint32_t rate : 10;                 /* Current P Header Credit Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_PCR_NPHDR_STAT_t;
#endif /* GENHDR_STRUCT */

/* PCIE NP Data Credits Space [DEBUG REG] */
#define VIRTIO_DMA_CSR_PCR_NPDAT_STAT                           0x0001800CU
/* P Data Credit level. Resets to 0. */
#define VIRTIO_DMA_CSR_PCR_NPDAT_STAT_CURRENT_LEVEL_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCR_NPDAT_STAT_CURRENT_LEVEL_SHIFT       0
#define VIRTIO_DMA_CSR_PCR_NPDAT_STAT_CURRENT_LEVEL_WIDTH       10
#define VIRTIO_DMA_CSR_PCR_NPDAT_STAT_CURRENT_LEVEL_MASK        (0x3FFU << VIRTIO_DMA_CSR_PCR_NPDAT_STAT_CURRENT_LEVEL_SHIFT)

/* Minimum P Data Credit level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_PCR_NPDAT_STAT_MIN_ACCESSTYPE            GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCR_NPDAT_STAT_MIN_SHIFT                 10
#define VIRTIO_DMA_CSR_PCR_NPDAT_STAT_MIN_WIDTH                 10
#define VIRTIO_DMA_CSR_PCR_NPDAT_STAT_MIN_MASK                  (0x3FFU << VIRTIO_DMA_CSR_PCR_NPDAT_STAT_MIN_SHIFT)

/* Current P Data Credit Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_PCR_NPDAT_STAT_RATE_ACCESSTYPE           GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCR_NPDAT_STAT_RATE_SHIFT                22
#define VIRTIO_DMA_CSR_PCR_NPDAT_STAT_RATE_WIDTH                10
#define VIRTIO_DMA_CSR_PCR_NPDAT_STAT_RATE_MASK                 (0x3FFU << VIRTIO_DMA_CSR_PCR_NPDAT_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* PCIE NP Data Credits Space [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* P Data Credit level. Resets to 0. */
    uint32_t min : 10;                  /* Minimum P Data Credit level (latched). Resets to 0. */
    uint32_t : 2;
    uint32_t rate : 10;                 /* Current P Data Credit Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_PCR_NPDAT_STAT_t;
#endif /* GENHDR_STRUCT */

/* PCIE Completion Header Credits Space [DEBUG REG] */
#define VIRTIO_DMA_CSR_PCR_COMPH_STAT                           0x00018010U
/* Completion Header Credit level. Resets to 0. */
#define VIRTIO_DMA_CSR_PCR_COMPH_STAT_CURRENT_LEVEL_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCR_COMPH_STAT_CURRENT_LEVEL_SHIFT       0
#define VIRTIO_DMA_CSR_PCR_COMPH_STAT_CURRENT_LEVEL_WIDTH       10
#define VIRTIO_DMA_CSR_PCR_COMPH_STAT_CURRENT_LEVEL_MASK        (0x3FFU << VIRTIO_DMA_CSR_PCR_COMPH_STAT_CURRENT_LEVEL_SHIFT)

/* Minimum Completion Header Credit level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_PCR_COMPH_STAT_MIN_ACCESSTYPE            GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCR_COMPH_STAT_MIN_SHIFT                 10
#define VIRTIO_DMA_CSR_PCR_COMPH_STAT_MIN_WIDTH                 10
#define VIRTIO_DMA_CSR_PCR_COMPH_STAT_MIN_MASK                  (0x3FFU << VIRTIO_DMA_CSR_PCR_COMPH_STAT_MIN_SHIFT)

/*
 * Current Completion Header Credit Rate when traffic being sent (not latched).
 * Resets to 0.
 */
#define VIRTIO_DMA_CSR_PCR_COMPH_STAT_RATE_ACCESSTYPE           GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCR_COMPH_STAT_RATE_SHIFT                22
#define VIRTIO_DMA_CSR_PCR_COMPH_STAT_RATE_WIDTH                10
#define VIRTIO_DMA_CSR_PCR_COMPH_STAT_RATE_MASK                 (0x3FFU << VIRTIO_DMA_CSR_PCR_COMPH_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* PCIE Completion Header Credits Space [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Completion Header Credit level. Resets to 0. */
    uint32_t min : 10;                  /* Minimum Completion Header Credit level (latched). Resets to 0. */
    uint32_t : 2;
    uint32_t rate : 10;                 /* Current Completion Header Credit Rate when traffic being sent (not latched).
                                           Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_PCR_COMPH_STAT_t;
#endif /* GENHDR_STRUCT */

/* PCIE Completion Data Credits Space [DEBUG REG] */
#define VIRTIO_DMA_CSR_PCR_COMPD_STAT                           0x00018014U
/* Completion Data Credit level. Resets to 0. */
#define VIRTIO_DMA_CSR_PCR_COMPD_STAT_CURRENT_LEVEL_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCR_COMPD_STAT_CURRENT_LEVEL_SHIFT       0
#define VIRTIO_DMA_CSR_PCR_COMPD_STAT_CURRENT_LEVEL_WIDTH       10
#define VIRTIO_DMA_CSR_PCR_COMPD_STAT_CURRENT_LEVEL_MASK        (0x3FFU << VIRTIO_DMA_CSR_PCR_COMPD_STAT_CURRENT_LEVEL_SHIFT)

/* Minimum Completion Data Credit level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_PCR_COMPD_STAT_MIN_ACCESSTYPE            GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCR_COMPD_STAT_MIN_SHIFT                 10
#define VIRTIO_DMA_CSR_PCR_COMPD_STAT_MIN_WIDTH                 10
#define VIRTIO_DMA_CSR_PCR_COMPD_STAT_MIN_MASK                  (0x3FFU << VIRTIO_DMA_CSR_PCR_COMPD_STAT_MIN_SHIFT)

/*
 * Current Completion Data Credit Rate when traffic being stalled (not latched).
 * Resets to 0.
 */
#define VIRTIO_DMA_CSR_PCR_COMPD_STAT_RATE_ACCESSTYPE           GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCR_COMPD_STAT_RATE_SHIFT                22
#define VIRTIO_DMA_CSR_PCR_COMPD_STAT_RATE_WIDTH                10
#define VIRTIO_DMA_CSR_PCR_COMPD_STAT_RATE_MASK                 (0x3FFU << VIRTIO_DMA_CSR_PCR_COMPD_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* PCIE Completion Data Credits Space [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Completion Data Credit level. Resets to 0. */
    uint32_t min : 10;                  /* Minimum Completion Data Credit level (latched). Resets to 0. */
    uint32_t : 2;
    uint32_t rate : 10;                 /* Current Completion Data Credit Rate when traffic being stalled (not latched).
                                           Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_PCR_COMPD_STAT_t;
#endif /* GENHDR_STRUCT */

/* PCIE Reorder In Order Count [DEBUG REG] */
#define VIRTIO_DMA_CSR_PCM_INORDER_STAT                 0x0001A000U
/*
 * Count of the number of PCIe transactions received in order from host. Resets
 * to 0.
 */
#define VIRTIO_DMA_CSR_PCM_INORDER_STAT_COUNT_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCM_INORDER_STAT_COUNT_SHIFT     0
#define VIRTIO_DMA_CSR_PCM_INORDER_STAT_COUNT_WIDTH     32
#define VIRTIO_DMA_CSR_PCM_INORDER_STAT_COUNT_MASK      (0xFFFFFFFFU << VIRTIO_DMA_CSR_PCM_INORDER_STAT_COUNT_SHIFT)

#ifdef GENHDR_STRUCT
/* PCIE Reorder In Order Count [DEBUG REG] */
typedef union {
  struct {
    uint32_t count : 32;        /* Count of the number of PCIe transactions received in order from host. Resets
                                   to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_PCM_INORDER_STAT_t;
#endif /* GENHDR_STRUCT */

/* Core Monitor Statistics Reset [DEBUG REG] */
#define VIRTIO_DMA_CSR_PCM_STATS_RST                            0x0001A000U
/* Write 1 to reset all status registers in this block. */
#define VIRTIO_DMA_CSR_PCM_STATS_RST_RESET_STATS_ACCESSTYPE     GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_PCM_STATS_RST_RESET_STATS_SHIFT          0
#define VIRTIO_DMA_CSR_PCM_STATS_RST_RESET_STATS_WIDTH          1
#define VIRTIO_DMA_CSR_PCM_STATS_RST_RESET_STATS_MASK           (0x1U << VIRTIO_DMA_CSR_PCM_STATS_RST_RESET_STATS_SHIFT)

#ifdef GENHDR_STRUCT
/* Core Monitor Statistics Reset [DEBUG REG] */
typedef union {
  struct {
    uint32_t reset_stats : 1;   /* Write 1 to reset all status registers in this block. */
    uint32_t : 31;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_PCM_STATS_RST_t;
#endif /* GENHDR_STRUCT */

/* PCIE Reorder Out of Order Count [DEBUG REG] */
#define VIRTIO_DMA_CSR_PCM_OUTORDER_STAT                                0x0001A004U
/*
 * Count of the number of PCIe transactions received out of order from host.
 * Resets to 0.
 */
#define VIRTIO_DMA_CSR_PCM_OUTORDER_STAT_OUT_ORDER_CNT_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCM_OUTORDER_STAT_OUT_ORDER_CNT_SHIFT            0
#define VIRTIO_DMA_CSR_PCM_OUTORDER_STAT_OUT_ORDER_CNT_WIDTH            16
#define VIRTIO_DMA_CSR_PCM_OUTORDER_STAT_OUT_ORDER_CNT_MASK             (0xFFFFU << VIRTIO_DMA_CSR_PCM_OUTORDER_STAT_OUT_ORDER_CNT_SHIFT)

/* Count of the number of PCIe errors received from host. Resets to 0. */
#define VIRTIO_DMA_CSR_PCM_OUTORDER_STAT_PCIE_ERR_CNT_ACCESSTYPE        GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCM_OUTORDER_STAT_PCIE_ERR_CNT_SHIFT             16
#define VIRTIO_DMA_CSR_PCM_OUTORDER_STAT_PCIE_ERR_CNT_WIDTH             16
#define VIRTIO_DMA_CSR_PCM_OUTORDER_STAT_PCIE_ERR_CNT_MASK              (0xFFFFU << VIRTIO_DMA_CSR_PCM_OUTORDER_STAT_PCIE_ERR_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* PCIE Reorder Out of Order Count [DEBUG REG] */
typedef union {
  struct {
    uint32_t out_order_cnt : 16;        /* Count of the number of PCIe transactions received out of order from host.
                                           Resets to 0. */
    uint32_t pcie_err_cnt : 16;         /* Count of the number of PCIe errors received from host. Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_PCM_OUTORDER_STAT_t;
#endif /* GENHDR_STRUCT */

/* PCIe Monitor WrReq Interface Achieved Rate Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_PCM_WRREQ_ARATE_STAT                     0x0001A008U
/*
 * Current WrReq Active Rate indicated as a percentage (not latched). Resets to
 * 0.
 */
#define VIRTIO_DMA_CSR_PCM_WRREQ_ARATE_STAT_RATE_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCM_WRREQ_ARATE_STAT_RATE_SHIFT          22
#define VIRTIO_DMA_CSR_PCM_WRREQ_ARATE_STAT_RATE_WIDTH          10
#define VIRTIO_DMA_CSR_PCM_WRREQ_ARATE_STAT_RATE_MASK           (0x3FFU << VIRTIO_DMA_CSR_PCM_WRREQ_ARATE_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* PCIe Monitor WrReq Interface Achieved Rate Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t : 22;
    uint32_t rate : 10; /* Current WrReq Active Rate indicated as a percentage (not latched). Resets to
                           0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_PCM_WRREQ_ARATE_STAT_t;
#endif /* GENHDR_STRUCT */

/* PCIe Monitor WrReq Interface Stalled Rate Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_PCM_WRREQ_SRATE_STAT                     0x0001A00CU
/*
 * Current WrReq Stalled Rate indicated as a percentage (not latched). Resets to
 * 0.
 */
#define VIRTIO_DMA_CSR_PCM_WRREQ_SRATE_STAT_RATE_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCM_WRREQ_SRATE_STAT_RATE_SHIFT          22
#define VIRTIO_DMA_CSR_PCM_WRREQ_SRATE_STAT_RATE_WIDTH          10
#define VIRTIO_DMA_CSR_PCM_WRREQ_SRATE_STAT_RATE_MASK           (0x3FFU << VIRTIO_DMA_CSR_PCM_WRREQ_SRATE_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* PCIe Monitor WrReq Interface Stalled Rate Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t : 22;
    uint32_t rate : 10; /* Current WrReq Stalled Rate indicated as a percentage (not latched). Resets to
                           0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_PCM_WRREQ_SRATE_STAT_t;
#endif /* GENHDR_STRUCT */

/* PCIe Monitor RdReq Interface Achieved Rate Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_PCM_RDREQ_ARATE_STAT                     0x0001A010U
/*
 * Current RdReq Active Rate indicated as a percentage (not latched). Resets to
 * 0.
 */
#define VIRTIO_DMA_CSR_PCM_RDREQ_ARATE_STAT_RATE_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCM_RDREQ_ARATE_STAT_RATE_SHIFT          22
#define VIRTIO_DMA_CSR_PCM_RDREQ_ARATE_STAT_RATE_WIDTH          10
#define VIRTIO_DMA_CSR_PCM_RDREQ_ARATE_STAT_RATE_MASK           (0x3FFU << VIRTIO_DMA_CSR_PCM_RDREQ_ARATE_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* PCIe Monitor RdReq Interface Achieved Rate Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t : 22;
    uint32_t rate : 10; /* Current RdReq Active Rate indicated as a percentage (not latched). Resets to
                           0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_PCM_RDREQ_ARATE_STAT_t;
#endif /* GENHDR_STRUCT */

/* PCIe Monitor RdReq Interface Stalled Rate Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_PCM_RDREQ_SRATE_STAT                     0x0001A014U
/*
 * Current RdReq Stalled Rate indicated as a percentage (not latched). Resets to
 * 0.
 */
#define VIRTIO_DMA_CSR_PCM_RDREQ_SRATE_STAT_RATE_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCM_RDREQ_SRATE_STAT_RATE_SHIFT          22
#define VIRTIO_DMA_CSR_PCM_RDREQ_SRATE_STAT_RATE_WIDTH          10
#define VIRTIO_DMA_CSR_PCM_RDREQ_SRATE_STAT_RATE_MASK           (0x3FFU << VIRTIO_DMA_CSR_PCM_RDREQ_SRATE_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* PCIe Monitor RdReq Interface Stalled Rate Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t : 22;
    uint32_t rate : 10; /* Current RdReq Stalled Rate indicated as a percentage (not latched). Resets to
                           0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_PCM_RDREQ_SRATE_STAT_t;
#endif /* GENHDR_STRUCT */

/* PCIe Monitor RdRsp Interface Achieved Rate Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_PCM_RDRSP_ARATE_STAT                     0x0001A018U
/*
 * Current RdRsp Active Rate indicated as a percentage (not latched). Resets to
 * 0.
 */
#define VIRTIO_DMA_CSR_PCM_RDRSP_ARATE_STAT_RATE_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCM_RDRSP_ARATE_STAT_RATE_SHIFT          22
#define VIRTIO_DMA_CSR_PCM_RDRSP_ARATE_STAT_RATE_WIDTH          10
#define VIRTIO_DMA_CSR_PCM_RDRSP_ARATE_STAT_RATE_MASK           (0x3FFU << VIRTIO_DMA_CSR_PCM_RDRSP_ARATE_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* PCIe Monitor RdRsp Interface Achieved Rate Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t : 22;
    uint32_t rate : 10; /* Current RdRsp Active Rate indicated as a percentage (not latched). Resets to
                           0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_PCM_RDRSP_ARATE_STAT_t;
#endif /* GENHDR_STRUCT */

/* PCIe Monitor RdRsp Interface Stalled Rate Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_PCM_RDRSP_SRATE_STAT                     0x0001A01CU
/*
 * Current RdRsp Stalled Rate indicated as a percentage (not latched). Resets to
 * 0.
 */
#define VIRTIO_DMA_CSR_PCM_RDRSP_SRATE_STAT_RATE_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_PCM_RDRSP_SRATE_STAT_RATE_SHIFT          22
#define VIRTIO_DMA_CSR_PCM_RDRSP_SRATE_STAT_RATE_WIDTH          10
#define VIRTIO_DMA_CSR_PCM_RDRSP_SRATE_STAT_RATE_MASK           (0x3FFU << VIRTIO_DMA_CSR_PCM_RDRSP_SRATE_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* PCIe Monitor RdRsp Interface Stalled Rate Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t : 22;
    uint32_t rate : 10; /* Current RdRsp Stalled Rate indicated as a percentage (not latched). Resets to
                           0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_PCM_RDRSP_SRATE_STAT_t;
#endif /* GENHDR_STRUCT */

/* Configuration Table for TX Irq Throttle Functions */
#define VIRTIO_DMA_CSR_TX_THROTTLE_CFG(tx_throttle_cfg_ix)      (0x0001C000U + ((tx_throttle_cfg_ix) * 4))
#define VIRTIO_DMA_CSR_TX_THROTTLE_CFG_MAX_INDEX                0x1FFU
/*
 * Sets the approximate maximum rate of interrupts generated for the queue. For
 * value n, approximate maximum rate is
 *  0 : 1Mirq/s
 *  1 : 500Kirq/s
 *  2 : 250Kirq/s
 *  3 : 125Kirq/s
 *  4 : 64Kirq/s
 *  5 : 32Kirq/s
 *  6 : 16Kirq/s
 *  7 : 8Kirq/s
 *  8 : 4Kirq/s
 *  9 : 2Kirq/s
 *  10 : 1Kirq/s
 *  11 : 500irq/s
 *  12 : 250irq/s
 *  13 : 125irq/s
 *  14 : 64irq/s
 *  15 : 32irq/s. 
 */
#define VIRTIO_DMA_CSR_TX_THROTTLE_CFG_RATE_ACCESSTYPE          GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_TX_THROTTLE_CFG_RATE_SHIFT               0
#define VIRTIO_DMA_CSR_TX_THROTTLE_CFG_RATE_WIDTH               4
#define VIRTIO_DMA_CSR_TX_THROTTLE_CFG_RATE_MASK                (0xFU << VIRTIO_DMA_CSR_TX_THROTTLE_CFG_RATE_SHIFT)

/*
 * When set, all IRQs are suppressed. When clear .throttleEn determines
 * behaviour.
 */
#define VIRTIO_DMA_CSR_TX_THROTTLE_CFG_IRQ_DISABLE_ACCESSTYPE   GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_TX_THROTTLE_CFG_IRQ_DISABLE_SHIFT        6
#define VIRTIO_DMA_CSR_TX_THROTTLE_CFG_IRQ_DISABLE_WIDTH        1
#define VIRTIO_DMA_CSR_TX_THROTTLE_CFG_IRQ_DISABLE_MASK         (0x1U << VIRTIO_DMA_CSR_TX_THROTTLE_CFG_IRQ_DISABLE_SHIFT)

/*
 * When set, throttling is enabled. When clear all IRQs would be transmitted
 * unless .irqDisable is set.
 */
#define VIRTIO_DMA_CSR_TX_THROTTLE_CFG_THROTTLE_EN_ACCESSTYPE   GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_TX_THROTTLE_CFG_THROTTLE_EN_SHIFT        7
#define VIRTIO_DMA_CSR_TX_THROTTLE_CFG_THROTTLE_EN_WIDTH        1
#define VIRTIO_DMA_CSR_TX_THROTTLE_CFG_THROTTLE_EN_MASK         (0x1U << VIRTIO_DMA_CSR_TX_THROTTLE_CFG_THROTTLE_EN_SHIFT)

#ifdef GENHDR_STRUCT
/* Configuration Table for TX Irq Throttle Functions */
typedef union {
  struct {
    uint32_t rate : 4;          /* Sets the approximate maximum rate of interrupts generated for the queue. For
                                   value n, approximate maximum rate is
                                   0 : 1Mirq/s
                                   1 : 500Kirq/s
                                   2 : 250Kirq/s
                                   3 : 125Kirq/s
                                   4 : 64Kirq/s
                                   5 : 32Kirq/s
                                   6 : 16Kirq/s
                                   7 : 8Kirq/s
                                   8 : 4Kirq/s
                                   9 : 2Kirq/s
                                   10 : 1Kirq/s
                                   11 : 500irq/s
                                   12 : 250irq/s
                                   13 : 125irq/s
                                   14 : 64irq/s
                                   15 : 32irq/s. */
    uint32_t : 2;
    uint32_t irq_disable : 1;   /* When set, all IRQs are suppressed. When clear .throttleEn determines
                                   behaviour. */
    uint32_t throttle_en : 1;   /* When set, throttling is enabled. When clear all IRQs would be transmitted
                                   unless .irqDisable is set. */
    uint32_t : 24;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TX_THROTTLE_CFG_t;
#endif /* GENHDR_STRUCT */

/* Configuration Table for RX Irq Throttle Functions */
#define VIRTIO_DMA_CSR_RX_THROTTLE_CFG(rx_throttle_cfg_ix)      (0x0001D000U + ((rx_throttle_cfg_ix) * 4))
#define VIRTIO_DMA_CSR_RX_THROTTLE_CFG_MAX_INDEX                0x1FFU
/*
 * Sets the approximate maximum rate of interrupts generated for the queue. For
 * value n, approximate maximum rate is
 *  0 : 1Mirq/s
 *  1 : 500Kirq/s
 *  2 : 250Kirq/s
 *  3 : 125Kirq/s
 *  4 : 64Kirq/s
 *  5 : 32Kirq/s
 *  6 : 16Kirq/s
 *  7 : 8Kirq/s
 *  8 : 4Kirq/s
 *  9 : 2Kirq/s
 *  10 : 1Kirq/s
 *  11 : 500irq/s
 *  12 : 250irq/s
 *  13 : 125irq/s
 *  14 : 64irq/s
 *  15 : 32irq/s. 
 */
#define VIRTIO_DMA_CSR_RX_THROTTLE_CFG_RATE_ACCESSTYPE          GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_RX_THROTTLE_CFG_RATE_SHIFT               0
#define VIRTIO_DMA_CSR_RX_THROTTLE_CFG_RATE_WIDTH               4
#define VIRTIO_DMA_CSR_RX_THROTTLE_CFG_RATE_MASK                (0xFU << VIRTIO_DMA_CSR_RX_THROTTLE_CFG_RATE_SHIFT)

/*
 * When set, all IRQs are suppressed. When clear .throttleEn determines
 * behaviour.
 */
#define VIRTIO_DMA_CSR_RX_THROTTLE_CFG_IRQ_DISABLE_ACCESSTYPE   GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_RX_THROTTLE_CFG_IRQ_DISABLE_SHIFT        6
#define VIRTIO_DMA_CSR_RX_THROTTLE_CFG_IRQ_DISABLE_WIDTH        1
#define VIRTIO_DMA_CSR_RX_THROTTLE_CFG_IRQ_DISABLE_MASK         (0x1U << VIRTIO_DMA_CSR_RX_THROTTLE_CFG_IRQ_DISABLE_SHIFT)

/*
 * When set, throttling is enabled. When clear all IRQs would be transmitted
 * unless .irqDisable is set.
 */
#define VIRTIO_DMA_CSR_RX_THROTTLE_CFG_THROTTLE_EN_ACCESSTYPE   GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_RX_THROTTLE_CFG_THROTTLE_EN_SHIFT        7
#define VIRTIO_DMA_CSR_RX_THROTTLE_CFG_THROTTLE_EN_WIDTH        1
#define VIRTIO_DMA_CSR_RX_THROTTLE_CFG_THROTTLE_EN_MASK         (0x1U << VIRTIO_DMA_CSR_RX_THROTTLE_CFG_THROTTLE_EN_SHIFT)

#ifdef GENHDR_STRUCT
/* Configuration Table for RX Irq Throttle Functions */
typedef union {
  struct {
    uint32_t rate : 4;          /* Sets the approximate maximum rate of interrupts generated for the queue. For
                                   value n, approximate maximum rate is
                                   0 : 1Mirq/s
                                   1 : 500Kirq/s
                                   2 : 250Kirq/s
                                   3 : 125Kirq/s
                                   4 : 64Kirq/s
                                   5 : 32Kirq/s
                                   6 : 16Kirq/s
                                   7 : 8Kirq/s
                                   8 : 4Kirq/s
                                   9 : 2Kirq/s
                                   10 : 1Kirq/s
                                   11 : 500irq/s
                                   12 : 250irq/s
                                   13 : 125irq/s
                                   14 : 64irq/s
                                   15 : 32irq/s. */
    uint32_t : 2;
    uint32_t irq_disable : 1;   /* When set, all IRQs are suppressed. When clear .throttleEn determines
                                   behaviour. */
    uint32_t throttle_en : 1;   /* When set, throttling is enabled. When clear all IRQs would be transmitted
                                   unless .irqDisable is set. */
    uint32_t : 24;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RX_THROTTLE_CFG_t;
#endif /* GENHDR_STRUCT */

/*
 * Indication of the amount of space in the slot. Difference between credit
 * response and current slot adaptor pointer. Single FC only. [DEBUG REG]
 */
#define VIRTIO_DMA_CSR_SLA_TX_SPACE_STAT                                0x0001F000U
/* Current Tx space level. Resets to 0. */
#define VIRTIO_DMA_CSR_SLA_TX_SPACE_STAT_CURRENT_LEVEL_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_SLA_TX_SPACE_STAT_CURRENT_LEVEL_SHIFT            0
#define VIRTIO_DMA_CSR_SLA_TX_SPACE_STAT_CURRENT_LEVEL_WIDTH            10
#define VIRTIO_DMA_CSR_SLA_TX_SPACE_STAT_CURRENT_LEVEL_MASK             (0x3FFU << VIRTIO_DMA_CSR_SLA_TX_SPACE_STAT_CURRENT_LEVEL_SHIFT)

/* Minimum Tx space level (latched). Resets to 10'h3ff. */
#define VIRTIO_DMA_CSR_SLA_TX_SPACE_STAT_MIN_ACCESSTYPE                 GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_SLA_TX_SPACE_STAT_MIN_SHIFT                      10
#define VIRTIO_DMA_CSR_SLA_TX_SPACE_STAT_MIN_WIDTH                      10
#define VIRTIO_DMA_CSR_SLA_TX_SPACE_STAT_MIN_MASK                       (0x3FFU << VIRTIO_DMA_CSR_SLA_TX_SPACE_STAT_MIN_SHIFT)

/* Current Tx credit Rate (Tx bandwidth) (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_SLA_TX_SPACE_STAT_RATE_ACCESSTYPE                GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_SLA_TX_SPACE_STAT_RATE_SHIFT                     22
#define VIRTIO_DMA_CSR_SLA_TX_SPACE_STAT_RATE_WIDTH                     10
#define VIRTIO_DMA_CSR_SLA_TX_SPACE_STAT_RATE_MASK                      (0x3FFU << VIRTIO_DMA_CSR_SLA_TX_SPACE_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Indication of the amount of space in the slot. Difference between credit
 * response and current slot adaptor pointer. Single FC only. [DEBUG REG]
 */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Current Tx space level. Resets to 0. */
    uint32_t min : 10;                  /* Minimum Tx space level (latched). Resets to 10'h3ff. */
    uint32_t : 2;
    uint32_t rate : 10;                 /* Current Tx credit Rate (Tx bandwidth) (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_SLA_TX_SPACE_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Fetch Descriptor Statistics Reset [DEBUG REG] */
#define VIRTIO_DMA_CSR_SLA_STATS_RST                            0x0001F000U
/* Write 1 to reset all registers in this block. */
#define VIRTIO_DMA_CSR_SLA_STATS_RST_RESET_STATS_ACCESSTYPE     GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_SLA_STATS_RST_RESET_STATS_SHIFT          0
#define VIRTIO_DMA_CSR_SLA_STATS_RST_RESET_STATS_WIDTH          1
#define VIRTIO_DMA_CSR_SLA_STATS_RST_RESET_STATS_MASK           (0x1U << VIRTIO_DMA_CSR_SLA_STATS_RST_RESET_STATS_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Fetch Descriptor Statistics Reset [DEBUG REG] */
typedef union {
  struct {
    uint32_t reset_stats : 1;   /* Write 1 to reset all registers in this block. */
    uint32_t : 31;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_SLA_STATS_RST_t;
#endif /* GENHDR_STRUCT */

/*
 * Indication of the amount of Rx data in the slot. Difference between credit
 * response and current slot adaptor pointer. Single FC only. [DEBUG REG]
 */
#define VIRTIO_DMA_CSR_SLA_RX_LEVEL_STAT                                0x0001F004U
/* Current Rx data level. Resets to 0. */
#define VIRTIO_DMA_CSR_SLA_RX_LEVEL_STAT_CURRENT_LEVEL_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_SLA_RX_LEVEL_STAT_CURRENT_LEVEL_SHIFT            0
#define VIRTIO_DMA_CSR_SLA_RX_LEVEL_STAT_CURRENT_LEVEL_WIDTH            10
#define VIRTIO_DMA_CSR_SLA_RX_LEVEL_STAT_CURRENT_LEVEL_MASK             (0x3FFU << VIRTIO_DMA_CSR_SLA_RX_LEVEL_STAT_CURRENT_LEVEL_SHIFT)

/* Max Rx data level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_SLA_RX_LEVEL_STAT_MAX_ACCESSTYPE                 GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_SLA_RX_LEVEL_STAT_MAX_SHIFT                      10
#define VIRTIO_DMA_CSR_SLA_RX_LEVEL_STAT_MAX_WIDTH                      10
#define VIRTIO_DMA_CSR_SLA_RX_LEVEL_STAT_MAX_MASK                       (0x3FFU << VIRTIO_DMA_CSR_SLA_RX_LEVEL_STAT_MAX_SHIFT)

/* Current Rx credit Rate (Rx bandwidth) (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_SLA_RX_LEVEL_STAT_RATE_ACCESSTYPE                GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_SLA_RX_LEVEL_STAT_RATE_SHIFT                     22
#define VIRTIO_DMA_CSR_SLA_RX_LEVEL_STAT_RATE_WIDTH                     10
#define VIRTIO_DMA_CSR_SLA_RX_LEVEL_STAT_RATE_MASK                      (0x3FFU << VIRTIO_DMA_CSR_SLA_RX_LEVEL_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Indication of the amount of Rx data in the slot. Difference between credit
 * response and current slot adaptor pointer. Single FC only. [DEBUG REG]
 */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Current Rx data level. Resets to 0. */
    uint32_t max : 10;                  /* Max Rx data level (latched). Resets to 0. */
    uint32_t : 2;
    uint32_t rate : 10;                 /* Current Rx credit Rate (Rx bandwidth) (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_SLA_RX_LEVEL_STAT_t;
#endif /* GENHDR_STRUCT */

/*
 * Rx Packet Dropped Packet Count. These are packets that are not mapped to a
 * queue
 */
#define VIRTIO_DMA_CSR_SLA_RX_DRP_CNT_STAT                              0x0001F014U
/* Number of Dropped Packets. Resets to 0. */
#define VIRTIO_DMA_CSR_SLA_RX_DRP_CNT_STAT_DROPPED_CNT_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_SLA_RX_DRP_CNT_STAT_DROPPED_CNT_SHIFT            0
#define VIRTIO_DMA_CSR_SLA_RX_DRP_CNT_STAT_DROPPED_CNT_WIDTH            32
#define VIRTIO_DMA_CSR_SLA_RX_DRP_CNT_STAT_DROPPED_CNT_MASK             (0xFFFFFFFFU << VIRTIO_DMA_CSR_SLA_RX_DRP_CNT_STAT_DROPPED_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Rx Packet Dropped Packet Count. These are packets that are not mapped to a
 * queue
 */
typedef union {
  struct {
    uint32_t dropped_cnt : 32;  /* Number of Dropped Packets. Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_SLA_RX_DRP_CNT_STAT_t;
#endif /* GENHDR_STRUCT */

/* Slot Adaptor Tx Packet Integrity Error Indicators */
#define VIRTIO_DMA_CSR_SLA_TX_ERROR                             0x0001F018U
/*
 * Slot Adaptor has identified a missing EOP, ie has seen consecutive SOPs.
 * Resets to 0.
 */
#define VIRTIO_DMA_CSR_SLA_TX_ERROR_TX_MISSING_EOP_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_SLA_TX_ERROR_TX_MISSING_EOP_SHIFT        0
#define VIRTIO_DMA_CSR_SLA_TX_ERROR_TX_MISSING_EOP_WIDTH        1
#define VIRTIO_DMA_CSR_SLA_TX_ERROR_TX_MISSING_EOP_MASK         (0x1U << VIRTIO_DMA_CSR_SLA_TX_ERROR_TX_MISSING_EOP_SHIFT)

/*
 * Slot Adaptor has identified a missing SOP, ie has seen consecutive EOPs.
 * Resets to 0.
 */
#define VIRTIO_DMA_CSR_SLA_TX_ERROR_TX_MISSING_SOP_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_SLA_TX_ERROR_TX_MISSING_SOP_SHIFT        1
#define VIRTIO_DMA_CSR_SLA_TX_ERROR_TX_MISSING_SOP_WIDTH        1
#define VIRTIO_DMA_CSR_SLA_TX_ERROR_TX_MISSING_SOP_MASK         (0x1U << VIRTIO_DMA_CSR_SLA_TX_ERROR_TX_MISSING_SOP_SHIFT)

/*
 * Slot Adaptor has identified a change in QID during a packet, ie between SOP
 * and EOP. Resets to 0.
 */
#define VIRTIO_DMA_CSR_SLA_TX_ERROR_TX_QIDERROR_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_SLA_TX_ERROR_TX_QIDERROR_SHIFT           2
#define VIRTIO_DMA_CSR_SLA_TX_ERROR_TX_QIDERROR_WIDTH           1
#define VIRTIO_DMA_CSR_SLA_TX_ERROR_TX_QIDERROR_MASK            (0x1U << VIRTIO_DMA_CSR_SLA_TX_ERROR_TX_QIDERROR_SHIFT)

#ifdef GENHDR_STRUCT
/* Slot Adaptor Tx Packet Integrity Error Indicators */
typedef union {
  struct {
    uint32_t tx_missing_eop : 1;        /* Slot Adaptor has identified a missing EOP, ie has seen consecutive SOPs.
                                           Resets to 0. */
    uint32_t tx_missing_sop : 1;        /* Slot Adaptor has identified a missing SOP, ie has seen consecutive EOPs.
                                           Resets to 0. */
    uint32_t tx_qiderror : 1;           /* Slot Adaptor has identified a change in QID during a packet, ie between SOP
                                           and EOP. Resets to 0. */
    uint32_t : 29;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_SLA_TX_ERROR_t;
#endif /* GENHDR_STRUCT */

/* Slot Adaptor Rx Packet Integrity Error Indicators */
#define VIRTIO_DMA_CSR_SLA_RX_ERROR                             0x0001F01CU
/*
 * Slot Adaptor has identified a missing EOP, ie has seen consecutive SOPs.
 * Resets to 0.
 */
#define VIRTIO_DMA_CSR_SLA_RX_ERROR_RX_MISSING_EOP_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_SLA_RX_ERROR_RX_MISSING_EOP_SHIFT        0
#define VIRTIO_DMA_CSR_SLA_RX_ERROR_RX_MISSING_EOP_WIDTH        1
#define VIRTIO_DMA_CSR_SLA_RX_ERROR_RX_MISSING_EOP_MASK         (0x1U << VIRTIO_DMA_CSR_SLA_RX_ERROR_RX_MISSING_EOP_SHIFT)

/*
 * Slot Adaptor has identified a missing SOP, ie has seen consecutive EOPs.
 * Resets to 0.
 */
#define VIRTIO_DMA_CSR_SLA_RX_ERROR_RX_MISSING_SOP_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_SLA_RX_ERROR_RX_MISSING_SOP_SHIFT        1
#define VIRTIO_DMA_CSR_SLA_RX_ERROR_RX_MISSING_SOP_WIDTH        1
#define VIRTIO_DMA_CSR_SLA_RX_ERROR_RX_MISSING_SOP_MASK         (0x1U << VIRTIO_DMA_CSR_SLA_RX_ERROR_RX_MISSING_SOP_SHIFT)

/*
 * Slot Adaptor has identified a change in QID during a packet, ie between SOP
 * and EOP. Resets to 0.
 */
#define VIRTIO_DMA_CSR_SLA_RX_ERROR_RX_QIDERROR_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_SLA_RX_ERROR_RX_QIDERROR_SHIFT           2
#define VIRTIO_DMA_CSR_SLA_RX_ERROR_RX_QIDERROR_WIDTH           1
#define VIRTIO_DMA_CSR_SLA_RX_ERROR_RX_QIDERROR_MASK            (0x1U << VIRTIO_DMA_CSR_SLA_RX_ERROR_RX_QIDERROR_SHIFT)

#ifdef GENHDR_STRUCT
/* Slot Adaptor Rx Packet Integrity Error Indicators */
typedef union {
  struct {
    uint32_t rx_missing_eop : 1;        /* Slot Adaptor has identified a missing EOP, ie has seen consecutive SOPs.
                                           Resets to 0. */
    uint32_t rx_missing_sop : 1;        /* Slot Adaptor has identified a missing SOP, ie has seen consecutive EOPs.
                                           Resets to 0. */
    uint32_t rx_qiderror : 1;           /* Slot Adaptor has identified a change in QID during a packet, ie between SOP
                                           and EOP. Resets to 0. */
    uint32_t : 29;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_SLA_RX_ERROR_t;
#endif /* GENHDR_STRUCT */

/* Mapping Table for Shaper Group assignment for each virtqueue */
#define VIRTIO_DMA_CSR_TPS_MAP_TBL_CFG(tps_map_tbl_cfg_ix)      (0x00020000U + ((tps_map_tbl_cfg_ix) * 4))
#define VIRTIO_DMA_CSR_TPS_MAP_TBL_CFG_MAX_INDEX                0x1FFU
/*
 * Shaper Group to which virtqueue is assigned to (min=0, max=2043 depends on
 * hardware configuration).
 */
#define VIRTIO_DMA_CSR_TPS_MAP_TBL_CFG_SHP_GRP_SHIFT            0
#define VIRTIO_DMA_CSR_TPS_MAP_TBL_CFG_SHP_GRP_WIDTH            11
#define VIRTIO_DMA_CSR_TPS_MAP_TBL_CFG_SHP_GRP_MASK             (0x7FFU << VIRTIO_DMA_CSR_TPS_MAP_TBL_CFG_SHP_GRP_SHIFT)

/*
 * Weight to use for queue in scheduling bandwidth relative to other virtqueues
 * (min=1, max=255).
 */
#define VIRTIO_DMA_CSR_TPS_MAP_TBL_CFG_WEIGHT_SHIFT             12
#define VIRTIO_DMA_CSR_TPS_MAP_TBL_CFG_WEIGHT_WIDTH             8
#define VIRTIO_DMA_CSR_TPS_MAP_TBL_CFG_WEIGHT_MASK              (0xFFU << VIRTIO_DMA_CSR_TPS_MAP_TBL_CFG_WEIGHT_SHIFT)

#ifdef GENHDR_STRUCT
/* Mapping Table for Shaper Group assignment for each virtqueue */
typedef union {
  struct {
    uint32_t shp_grp : 11;      /* Shaper Group to which virtqueue is assigned to (min=0, max=2043 depends on
                                   hardware configuration). */
    uint32_t : 1;
    uint32_t weight : 8;        /* Weight to use for queue in scheduling bandwidth relative to other virtqueues
                                   (min=1, max=255). */
    uint32_t : 12;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_MAP_TBL_CFG_t;
#endif /* GENHDR_STRUCT */

/* Bit-Rate Shaper configuration for each group */
#define VIRTIO_DMA_CSR_TPS_SHAPER_CFG(tps_shaper_cfg_ix)        (0x00022000U + ((tps_shaper_cfg_ix) * 4))
#define VIRTIO_DMA_CSR_TPS_SHAPER_CFG_MAX_INDEX                 0x1FFU
/* Maximum level (units of 10240bytes) of shaper counter (min=1, max=2^13 - 1). */
#define VIRTIO_DMA_CSR_TPS_SHAPER_CFG_MAX_LEVEL_SHIFT           0
#define VIRTIO_DMA_CSR_TPS_SHAPER_CFG_MAX_LEVEL_WIDTH           13
#define VIRTIO_DMA_CSR_TPS_SHAPER_CFG_MAX_LEVEL_MASK            (0x1FFFU << VIRTIO_DMA_CSR_TPS_SHAPER_CFG_MAX_LEVEL_SHIFT)

/*
 * Maximum Bandwidth in multiples of 50Mbps (i.e. Bandwidth in Mbps = maxbw *
 * 50).
 */
#define VIRTIO_DMA_CSR_TPS_SHAPER_CFG_MAXBW_SHIFT               16
#define VIRTIO_DMA_CSR_TPS_SHAPER_CFG_MAXBW_WIDTH               10
#define VIRTIO_DMA_CSR_TPS_SHAPER_CFG_MAXBW_MASK                (0x3FFU << VIRTIO_DMA_CSR_TPS_SHAPER_CFG_MAXBW_SHIFT)

/* 1 - Enable / 0 - Disable Shaper. */
#define VIRTIO_DMA_CSR_TPS_SHAPER_CFG_EN_SHIFT                  31
#define VIRTIO_DMA_CSR_TPS_SHAPER_CFG_EN_WIDTH                  1
#define VIRTIO_DMA_CSR_TPS_SHAPER_CFG_EN_MASK                   (0x1U << VIRTIO_DMA_CSR_TPS_SHAPER_CFG_EN_SHIFT)

#ifdef GENHDR_STRUCT
/* Bit-Rate Shaper configuration for each group */
typedef union {
  struct {
    uint32_t max_level : 13;    /* Maximum level (units of 10240bytes) of shaper counter (min=1, max=2^13 - 1). */
    uint32_t : 3;
    uint32_t maxbw : 10;        /* Maximum Bandwidth in multiples of 50Mbps (i.e. Bandwidth in Mbps = maxbw *
                                   50). */
    uint32_t : 5;
    uint32_t en : 1;            /* 1 - Enable / 0 - Disable Shaper. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_SHAPER_CFG_t;
#endif /* GENHDR_STRUCT */

/* Packet-Rate Shaper configuration for each group */
#define VIRTIO_DMA_CSR_TPS_SHAPER_PPS_CFG(tps_shaper_pps_cfg_ix) (0x00024000U + ((tps_shaper_pps_cfg_ix) * 4))
#define VIRTIO_DMA_CSR_TPS_SHAPER_PPS_CFG_MAX_INDEX             0x1FFU
/*
 * Maximum level of shaper counter (scaled value : min=1, max=2^13 - 1) ALWAYS
 * SET TO MAXIMUM VALUE.
 */
#define VIRTIO_DMA_CSR_TPS_SHAPER_PPS_CFG_MAX_LEVEL_SHIFT       0
#define VIRTIO_DMA_CSR_TPS_SHAPER_PPS_CFG_MAX_LEVEL_WIDTH       13
#define VIRTIO_DMA_CSR_TPS_SHAPER_PPS_CFG_MAX_LEVEL_MASK        (0x1FFFU << VIRTIO_DMA_CSR_TPS_SHAPER_PPS_CFG_MAX_LEVEL_SHIFT)

/*
 * Maximum Packet Rate in multiples of 20,000pps (i.e. Packet Rate in kpps =
 * maxpr * 20).
 */
#define VIRTIO_DMA_CSR_TPS_SHAPER_PPS_CFG_MAXPR_SHIFT           16
#define VIRTIO_DMA_CSR_TPS_SHAPER_PPS_CFG_MAXPR_WIDTH           10
#define VIRTIO_DMA_CSR_TPS_SHAPER_PPS_CFG_MAXPR_MASK            (0x3FFU << VIRTIO_DMA_CSR_TPS_SHAPER_PPS_CFG_MAXPR_SHIFT)

/* 1 - Enable / 0 - Disable Packet Rate Shaper. */
#define VIRTIO_DMA_CSR_TPS_SHAPER_PPS_CFG_EN_SHIFT              31
#define VIRTIO_DMA_CSR_TPS_SHAPER_PPS_CFG_EN_WIDTH              1
#define VIRTIO_DMA_CSR_TPS_SHAPER_PPS_CFG_EN_MASK               (0x1U << VIRTIO_DMA_CSR_TPS_SHAPER_PPS_CFG_EN_SHIFT)

#ifdef GENHDR_STRUCT
/* Packet-Rate Shaper configuration for each group */
typedef union {
  struct {
    uint32_t max_level : 13;    /* Maximum level of shaper counter (scaled value : min=1, max=2^13 - 1) ALWAYS
                                   SET TO MAXIMUM VALUE. */
    uint32_t : 3;
    uint32_t maxpr : 10;        /* Maximum Packet Rate in multiples of 20,000pps (i.e. Packet Rate in kpps =
                                   maxpr * 20). */
    uint32_t : 5;
    uint32_t en : 1;            /* 1 - Enable / 0 - Disable Packet Rate Shaper. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_SHAPER_PPS_CFG_t;
#endif /* GENHDR_STRUCT */

/* Shaper Group State1 Table (NOTE: only up to max of first 128 groups) */
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT(tps_grp_state1_stat_ix)      (0x00026000U + ((tps_grp_state1_stat_ix) * 4))
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_MAX_INDEX                    0x7FU
/* Queue Active List FIFO (for Shaper Group) read pointer. */
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_ACT_LIST_RPTR_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_ACT_LIST_RPTR_SHIFT        0
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_ACT_LIST_RPTR_WIDTH        7
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_ACT_LIST_RPTR_MASK         (0x7FU << VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_ACT_LIST_RPTR_SHIFT)

/* Queue Active List FIFO (for Shaper Group) write pointer. */
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_ACT_LIST_WPTR_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_ACT_LIST_WPTR_SHIFT        7
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_ACT_LIST_WPTR_WIDTH        7
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_ACT_LIST_WPTR_MASK         (0x7FU << VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_ACT_LIST_WPTR_SHIFT)

/* Queue Active List FIFO (for Shaper Group) number of entries. */
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_ACT_LIST_CNT_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_ACT_LIST_CNT_SHIFT         14
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_ACT_LIST_CNT_WIDTH         7
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_ACT_LIST_CNT_MASK          (0x7FU << VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_ACT_LIST_CNT_SHIFT)

/* Queue Wait List FIFO (for Shaper Group) has available entries. */
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_WAIT_LIST_AVL_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_WAIT_LIST_AVL_SHIFT        21
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_WAIT_LIST_AVL_WIDTH        1
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_WAIT_LIST_AVL_MASK         (0x1U << VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_WAIT_LIST_AVL_SHIFT)

/* Queue Active List FIFO (for Shaper Group) has available entries. */
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_ACT_LIST_AVL_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_ACT_LIST_AVL_SHIFT         22
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_ACT_LIST_AVL_WIDTH         1
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_ACT_LIST_AVL_MASK          (0x1U << VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_Q_ACT_LIST_AVL_SHIFT)

/* Shaper indicates that maximum bandwidth exceeded. */
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_SHP_FAIL_ACCESSTYPE          GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_SHP_FAIL_SHIFT               23
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_SHP_FAIL_WIDTH               1
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_SHP_FAIL_MASK                (0x1U << VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_SHP_FAIL_SHIFT)

/* Group is in Wait List FIFO. */
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_IN_GRP_WAIT_LIST_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_IN_GRP_WAIT_LIST_SHIFT       24
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_IN_GRP_WAIT_LIST_WIDTH       1
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_IN_GRP_WAIT_LIST_MASK        (0x1U << VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_IN_GRP_WAIT_LIST_SHIFT)

/* Group is in Active List FIFO. */
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_IN_GRP_ACT_LIST_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_IN_GRP_ACT_LIST_SHIFT        25
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_IN_GRP_ACT_LIST_WIDTH        1
#define VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_IN_GRP_ACT_LIST_MASK         (0x1U << VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_IN_GRP_ACT_LIST_SHIFT)

#ifdef GENHDR_STRUCT
/* Shaper Group State1 Table (NOTE: only up to max of first 128 groups) */
typedef union {
  struct {
    uint32_t q_act_list_rptr : 7;       /* Queue Active List FIFO (for Shaper Group) read pointer. */
    uint32_t q_act_list_wptr : 7;       /* Queue Active List FIFO (for Shaper Group) write pointer. */
    uint32_t q_act_list_cnt : 7;        /* Queue Active List FIFO (for Shaper Group) number of entries. */
    uint32_t q_wait_list_avl : 1;       /* Queue Wait List FIFO (for Shaper Group) has available entries. */
    uint32_t q_act_list_avl : 1;        /* Queue Active List FIFO (for Shaper Group) has available entries. */
    uint32_t shp_fail : 1;              /* Shaper indicates that maximum bandwidth exceeded. */
    uint32_t in_grp_wait_list : 1;      /* Group is in Wait List FIFO. */
    uint32_t in_grp_act_list : 1;       /* Group is in Active List FIFO. */
    uint32_t : 6;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_GRP_STATE1_STAT_t;
#endif /* GENHDR_STRUCT */

/* Shaper Group State2 Table (NOTE: only up to max of first 128 groups) */
#define VIRTIO_DMA_CSR_TPS_GRP_STATE2_STAT(tps_grp_state2_stat_ix)      (0x00026200U + ((tps_grp_state2_stat_ix) * 4))
#define VIRTIO_DMA_CSR_TPS_GRP_STATE2_STAT_MAX_INDEX                    0x7FU
/*
 * Last write pointer when queue added to Queue Wait List FIFO (for Shaper
 * Group).
 */
#define VIRTIO_DMA_CSR_TPS_GRP_STATE2_STAT_Q_WAIT_LIST_WPTR_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_GRP_STATE2_STAT_Q_WAIT_LIST_WPTR_SHIFT       0
#define VIRTIO_DMA_CSR_TPS_GRP_STATE2_STAT_Q_WAIT_LIST_WPTR_WIDTH       7
#define VIRTIO_DMA_CSR_TPS_GRP_STATE2_STAT_Q_WAIT_LIST_WPTR_MASK        (0x7FU << VIRTIO_DMA_CSR_TPS_GRP_STATE2_STAT_Q_WAIT_LIST_WPTR_SHIFT)

/* Last write pointer valid. */
#define VIRTIO_DMA_CSR_TPS_GRP_STATE2_STAT_Q_WAIT_LIST_WPTR_VAL_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_GRP_STATE2_STAT_Q_WAIT_LIST_WPTR_VAL_SHIFT   7
#define VIRTIO_DMA_CSR_TPS_GRP_STATE2_STAT_Q_WAIT_LIST_WPTR_VAL_WIDTH   1
#define VIRTIO_DMA_CSR_TPS_GRP_STATE2_STAT_Q_WAIT_LIST_WPTR_VAL_MASK    (0x1U << VIRTIO_DMA_CSR_TPS_GRP_STATE2_STAT_Q_WAIT_LIST_WPTR_VAL_SHIFT)

/*
 * Write pointer of Queue Wait List FIFO (for Shaper Group) when Shaper Group
 * added to Group Wait List.
 */
#define VIRTIO_DMA_CSR_TPS_GRP_STATE2_STAT_Q_WAIT_LIST_WPTR_GRP_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_GRP_STATE2_STAT_Q_WAIT_LIST_WPTR_GRP_SHIFT   8
#define VIRTIO_DMA_CSR_TPS_GRP_STATE2_STAT_Q_WAIT_LIST_WPTR_GRP_WIDTH   7
#define VIRTIO_DMA_CSR_TPS_GRP_STATE2_STAT_Q_WAIT_LIST_WPTR_GRP_MASK    (0x7FU << VIRTIO_DMA_CSR_TPS_GRP_STATE2_STAT_Q_WAIT_LIST_WPTR_GRP_SHIFT)

#ifdef GENHDR_STRUCT
/* Shaper Group State2 Table (NOTE: only up to max of first 128 groups) */
typedef union {
  struct {
    uint32_t q_wait_list_wptr : 7;      /* Last write pointer when queue added to Queue Wait List FIFO (for Shaper
                                           Group). */
    uint32_t q_wait_list_wptr_val : 1;  /* Last write pointer valid. */
    uint32_t q_wait_list_wptr_grp : 7;  /* Write pointer of Queue Wait List FIFO (for Shaper Group) when Shaper Group
                                           added to Group Wait List. */
    uint32_t : 17;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_GRP_STATE2_STAT_t;
#endif /* GENHDR_STRUCT */

/* Shaper Group State3 Table (NOTE: only up to max of first 128 groups) */
#define VIRTIO_DMA_CSR_TPS_GRP_STATE3_STAT(tps_grp_state3_stat_ix)      (0x00026400U + ((tps_grp_state3_stat_ix) * 4))
#define VIRTIO_DMA_CSR_TPS_GRP_STATE3_STAT_MAX_INDEX                    0x7FU
/* Queue Wait List FIFO (for Shaper Group) read pointer. */
#define VIRTIO_DMA_CSR_TPS_GRP_STATE3_STAT_Q_WAIT_LIST_RPTR_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_GRP_STATE3_STAT_Q_WAIT_LIST_RPTR_SHIFT       0
#define VIRTIO_DMA_CSR_TPS_GRP_STATE3_STAT_Q_WAIT_LIST_RPTR_WIDTH       7
#define VIRTIO_DMA_CSR_TPS_GRP_STATE3_STAT_Q_WAIT_LIST_RPTR_MASK        (0x7FU << VIRTIO_DMA_CSR_TPS_GRP_STATE3_STAT_Q_WAIT_LIST_RPTR_SHIFT)

/* Queue Wait List FIFO (for Shaper Group) write pointer. */
#define VIRTIO_DMA_CSR_TPS_GRP_STATE3_STAT_Q_WAIT_LIST_WPTR_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_GRP_STATE3_STAT_Q_WAIT_LIST_WPTR_SHIFT       7
#define VIRTIO_DMA_CSR_TPS_GRP_STATE3_STAT_Q_WAIT_LIST_WPTR_WIDTH       7
#define VIRTIO_DMA_CSR_TPS_GRP_STATE3_STAT_Q_WAIT_LIST_WPTR_MASK        (0x7FU << VIRTIO_DMA_CSR_TPS_GRP_STATE3_STAT_Q_WAIT_LIST_WPTR_SHIFT)

/* Queue Wait List FIFO (for Shaper Group) number of entries. */
#define VIRTIO_DMA_CSR_TPS_GRP_STATE3_STAT_Q_WAIT_LIST_CNT_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_GRP_STATE3_STAT_Q_WAIT_LIST_CNT_SHIFT        14
#define VIRTIO_DMA_CSR_TPS_GRP_STATE3_STAT_Q_WAIT_LIST_CNT_WIDTH        7
#define VIRTIO_DMA_CSR_TPS_GRP_STATE3_STAT_Q_WAIT_LIST_CNT_MASK         (0x7FU << VIRTIO_DMA_CSR_TPS_GRP_STATE3_STAT_Q_WAIT_LIST_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Shaper Group State3 Table (NOTE: only up to max of first 128 groups) */
typedef union {
  struct {
    uint32_t q_wait_list_rptr : 7;      /* Queue Wait List FIFO (for Shaper Group) read pointer. */
    uint32_t q_wait_list_wptr : 7;      /* Queue Wait List FIFO (for Shaper Group) write pointer. */
    uint32_t q_wait_list_cnt : 7;       /* Queue Wait List FIFO (for Shaper Group) number of entries. */
    uint32_t : 11;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_GRP_STATE3_STAT_t;
#endif /* GENHDR_STRUCT */

/*
 * Queue State1 Table (NOTE: only up to max of first 128 queues and only reading
 * 32bits of state)
 */
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT(tps_qid_state1_stat_ix)      (0x00026600U + ((tps_qid_state1_stat_ix) * 4))
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_MAX_INDEX                    0x7FU
/* Shaper Group ID (NOTE: Only TOP 4 bits -> missing bottom 2 bits). */
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_SHP_GRP_ACCESSTYPE           GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_SHP_GRP_SHIFT                0
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_SHP_GRP_WIDTH                4
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_SHP_GRP_MASK                 (0xFU << VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_SHP_GRP_SHIFT)

/*
 * Configured Weight to use for queue in scheduling bandwidth relative to other
 * virtqueues (min=1, max=255).
 */
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_CFG_WEIGHT_ACCESSTYPE        GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_CFG_WEIGHT_SHIFT             4
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_CFG_WEIGHT_WIDTH             8
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_CFG_WEIGHT_MASK              (0xFFU << VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_CFG_WEIGHT_SHIFT)

/* Number of available packets. */
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_PKT_AVL_CNT_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_PKT_AVL_CNT_SHIFT            12
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_PKT_AVL_CNT_WIDTH            16
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_PKT_AVL_CNT_MASK             (0xFFFFU << VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_PKT_AVL_CNT_SHIFT)

/* Allocate new round of credits. */
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_ALLOC_CREDIT_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_ALLOC_CREDIT_SHIFT           28
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_ALLOC_CREDIT_WIDTH           1
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_ALLOC_CREDIT_MASK            (0x1U << VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_ALLOC_CREDIT_SHIFT)

/* Current Queue being reset. */
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_QID_RST_FLAG_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_QID_RST_FLAG_SHIFT           29
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_QID_RST_FLAG_WIDTH           1
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_QID_RST_FLAG_MASK            (0x1U << VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_QID_RST_FLAG_SHIFT)

/* Queue is in Wait List FIFO. */
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_IN_QID_WAIT_LIST_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_IN_QID_WAIT_LIST_SHIFT       30
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_IN_QID_WAIT_LIST_WIDTH       1
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_IN_QID_WAIT_LIST_MASK        (0x1U << VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_IN_QID_WAIT_LIST_SHIFT)

/* Queue is in Active List FIFO. */
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_IN_QID_ACT_LIST_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_IN_QID_ACT_LIST_SHIFT        31
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_IN_QID_ACT_LIST_WIDTH        1
#define VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_IN_QID_ACT_LIST_MASK         (0x1U << VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_IN_QID_ACT_LIST_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Queue State1 Table (NOTE: only up to max of first 128 queues and only reading
 * 32bits of state)
 */
typedef union {
  struct {
    uint32_t shp_grp : 4;               /* Shaper Group ID (NOTE: Only TOP 4 bits -> missing bottom 2 bits). */
    uint32_t cfg_weight : 8;            /* Configured Weight to use for queue in scheduling bandwidth relative to other
                                           virtqueues (min=1, max=255). */
    uint32_t pkt_avl_cnt : 16;          /* Number of available packets. */
    uint32_t alloc_credit : 1;          /* Allocate new round of credits. */
    uint32_t qid_rst_flag : 1;          /* Current Queue being reset. */
    uint32_t in_qid_wait_list : 1;      /* Queue is in Wait List FIFO. */
    uint32_t in_qid_act_list : 1;       /* Queue is in Active List FIFO. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_QID_STATE1_STAT_t;
#endif /* GENHDR_STRUCT */

/*
 * Queue State2 Table (NOTE: only up to max of first 128 queues and only reading
 * 32bits of state)
 */
#define VIRTIO_DMA_CSR_TPS_QID_STATE2_STAT(tps_qid_state2_stat_ix)      (0x00026800U + ((tps_qid_state2_stat_ix) * 4))
#define VIRTIO_DMA_CSR_TPS_QID_STATE2_STAT_MAX_INDEX                    0x7FU
/* Signed number of bytes representing current credit level of queue. */
#define VIRTIO_DMA_CSR_TPS_QID_STATE2_STAT_CREDIT_LEVEL_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_QID_STATE2_STAT_CREDIT_LEVEL_SHIFT           0
#define VIRTIO_DMA_CSR_TPS_QID_STATE2_STAT_CREDIT_LEVEL_WIDTH           32
#define VIRTIO_DMA_CSR_TPS_QID_STATE2_STAT_CREDIT_LEVEL_MASK            (0xFFFFFFFFU << VIRTIO_DMA_CSR_TPS_QID_STATE2_STAT_CREDIT_LEVEL_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Queue State2 Table (NOTE: only up to max of first 128 queues and only reading
 * 32bits of state)
 */
typedef union {
  struct {
    uint32_t credit_level : 32; /* Signed number of bytes representing current credit level of queue. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_QID_STATE2_STAT_t;
#endif /* GENHDR_STRUCT */

/*
 * Shaper State1 Table (NOTE: only up to max of first 128 queues and only
 * reading 32bits of state)
 */
#define VIRTIO_DMA_CSR_TPS_SHP_STATE1_STAT(tps_shp_state1_stat_ix)      (0x00026A00U + ((tps_shp_state1_stat_ix) * 4))
#define VIRTIO_DMA_CSR_TPS_SHP_STATE1_STAT_MAX_INDEX                    0x7FU
/* Shaper level (24.2 signed fixed point format) in multiples of 10 bytes. */
#define VIRTIO_DMA_CSR_TPS_SHP_STATE1_STAT_SHAPER_LEVEL_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_SHP_STATE1_STAT_SHAPER_LEVEL_SHIFT           0
#define VIRTIO_DMA_CSR_TPS_SHP_STATE1_STAT_SHAPER_LEVEL_WIDTH           32
#define VIRTIO_DMA_CSR_TPS_SHP_STATE1_STAT_SHAPER_LEVEL_MASK            (0xFFFFFFFFU << VIRTIO_DMA_CSR_TPS_SHP_STATE1_STAT_SHAPER_LEVEL_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Shaper State1 Table (NOTE: only up to max of first 128 queues and only
 * reading 32bits of state)
 */
typedef union {
  struct {
    uint32_t shaper_level : 32; /* Shaper level (24.2 signed fixed point format) in multiples of 10 bytes. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_SHP_STATE1_STAT_t;
#endif /* GENHDR_STRUCT */

/*
 * Shaper State2 Table (NOTE: only up to max of first 128 queues and only
 * reading 32bits of state)
 */
#define VIRTIO_DMA_CSR_TPS_SHP_STATE2_STAT(tps_shp_state2_stat_ix) (0x00026C00U + ((tps_shp_state2_stat_ix) * 4))
#define VIRTIO_DMA_CSR_TPS_SHP_STATE2_STAT_MAX_INDEX            0x7FU
/* Timestamp at which shaper level was last updated. */
#define VIRTIO_DMA_CSR_TPS_SHP_STATE2_STAT_TIMESTAMP_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_SHP_STATE2_STAT_TIMESTAMP_SHIFT      0
#define VIRTIO_DMA_CSR_TPS_SHP_STATE2_STAT_TIMESTAMP_WIDTH      32
#define VIRTIO_DMA_CSR_TPS_SHP_STATE2_STAT_TIMESTAMP_MASK       (0xFFFFFFFFU << VIRTIO_DMA_CSR_TPS_SHP_STATE2_STAT_TIMESTAMP_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Shaper State2 Table (NOTE: only up to max of first 128 queues and only
 * reading 32bits of state)
 */
typedef union {
  struct {
    uint32_t timestamp : 32;    /* Timestamp at which shaper level was last updated. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_SHP_STATE2_STAT_t;
#endif /* GENHDR_STRUCT */

/* Clear Debug Statistics Registers */
#define VIRTIO_DMA_CSR_TPS_DBG_CLR_CFG                          0x00026E00U
/*
 * Write 16'hFF to clear registers. Ensure write 16'h00 to register after clear
 * operation.
 */
#define VIRTIO_DMA_CSR_TPS_DBG_CLR_CFG_CLR_STATS_SHIFT          0
#define VIRTIO_DMA_CSR_TPS_DBG_CLR_CFG_CLR_STATS_WIDTH          16
#define VIRTIO_DMA_CSR_TPS_DBG_CLR_CFG_CLR_STATS_MASK           (0xFFFFU << VIRTIO_DMA_CSR_TPS_DBG_CLR_CFG_CLR_STATS_SHIFT)

#ifdef GENHDR_STRUCT
/* Clear Debug Statistics Registers */
typedef union {
  struct {
    uint32_t clr_stats : 16;    /* Write 16'hFF to clear registers. Ensure write 16'h00 to register after clear
                                   operation. */
    uint32_t : 16;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_DBG_CLR_CFG_t;
#endif /* GENHDR_STRUCT */

/* Initialise module status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TPS_INIT_STAT                    0x00026E04U
/* Status of module. */
#define VIRTIO_DMA_CSR_TPS_INIT_STAT_STATUS_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_INIT_STAT_STATUS_SHIFT       0
#define VIRTIO_DMA_CSR_TPS_INIT_STAT_STATUS_WIDTH       32
#define VIRTIO_DMA_CSR_TPS_INIT_STAT_STATUS_MASK        (0xFFFFFFFFU << VIRTIO_DMA_CSR_TPS_INIT_STAT_STATUS_SHIFT)

#ifdef GENHDR_STRUCT
/* Initialise module status [DEBUG REG] */
typedef union {
  struct {
    uint32_t status : 32;       /* Status of module. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_INIT_STAT_t;
#endif /* GENHDR_STRUCT */

/* Indication of last DMA queueu ID restarted (reset) */
#define VIRTIO_DMA_CSR_TPS_QID_RST_STAT                         0x00026E08U
/* DMA queue ID of last ring reset. */
#define VIRTIO_DMA_CSR_TPS_QID_RST_STAT_QUEUE_ID_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_QID_RST_STAT_QUEUE_ID_SHIFT          0
#define VIRTIO_DMA_CSR_TPS_QID_RST_STAT_QUEUE_ID_WIDTH          9
#define VIRTIO_DMA_CSR_TPS_QID_RST_STAT_QUEUE_ID_MASK           (0x1FFU << VIRTIO_DMA_CSR_TPS_QID_RST_STAT_QUEUE_ID_SHIFT)

/* DMA queue ID field valid. */
#define VIRTIO_DMA_CSR_TPS_QID_RST_STAT_RST_DONE_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_QID_RST_STAT_RST_DONE_SHIFT          15
#define VIRTIO_DMA_CSR_TPS_QID_RST_STAT_RST_DONE_WIDTH          1
#define VIRTIO_DMA_CSR_TPS_QID_RST_STAT_RST_DONE_MASK           (0x1U << VIRTIO_DMA_CSR_TPS_QID_RST_STAT_RST_DONE_SHIFT)

#ifdef GENHDR_STRUCT
/* Indication of last DMA queueu ID restarted (reset) */
typedef union {
  struct {
    uint32_t queue_id : 9;      /* DMA queue ID of last ring reset. */
    uint32_t : 6;
    uint32_t rst_done : 1;      /* DMA queue ID field valid. */
    uint32_t : 16;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_QID_RST_STAT_t;
#endif /* GENHDR_STRUCT */

/* Packet available module status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TPS_PKT_AVL_STAT                         0x00026E0CU
/* Status of module. */
#define VIRTIO_DMA_CSR_TPS_PKT_AVL_STAT_STATUS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_PKT_AVL_STAT_STATUS_SHIFT            0
#define VIRTIO_DMA_CSR_TPS_PKT_AVL_STAT_STATUS_WIDTH            32
#define VIRTIO_DMA_CSR_TPS_PKT_AVL_STAT_STATUS_MASK             (0xFFFFFFFFU << VIRTIO_DMA_CSR_TPS_PKT_AVL_STAT_STATUS_SHIFT)

#ifdef GENHDR_STRUCT
/* Packet available module status [DEBUG REG] */
typedef union {
  struct {
    uint32_t status : 32;       /* Status of module. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_PKT_AVL_STAT_t;
#endif /* GENHDR_STRUCT */

/* Actual Packet Length module status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TPS_PKT_LEN_STAT                         0x00026E10U
/* Status of module. */
#define VIRTIO_DMA_CSR_TPS_PKT_LEN_STAT_STATUS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_PKT_LEN_STAT_STATUS_SHIFT            0
#define VIRTIO_DMA_CSR_TPS_PKT_LEN_STAT_STATUS_WIDTH            32
#define VIRTIO_DMA_CSR_TPS_PKT_LEN_STAT_STATUS_MASK             (0xFFFFFFFFU << VIRTIO_DMA_CSR_TPS_PKT_LEN_STAT_STATUS_SHIFT)

#ifdef GENHDR_STRUCT
/* Actual Packet Length module status [DEBUG REG] */
typedef union {
  struct {
    uint32_t status : 32;       /* Status of module. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_PKT_LEN_STAT_t;
#endif /* GENHDR_STRUCT */

/* Throttle Notify module status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TPS_THR_NFY_STAT                         0x00026E14U
/* Status of module. */
#define VIRTIO_DMA_CSR_TPS_THR_NFY_STAT_STATUS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_THR_NFY_STAT_STATUS_SHIFT            0
#define VIRTIO_DMA_CSR_TPS_THR_NFY_STAT_STATUS_WIDTH            32
#define VIRTIO_DMA_CSR_TPS_THR_NFY_STAT_STATUS_MASK             (0xFFFFFFFFU << VIRTIO_DMA_CSR_TPS_THR_NFY_STAT_STATUS_SHIFT)

#ifdef GENHDR_STRUCT
/* Throttle Notify module status [DEBUG REG] */
typedef union {
  struct {
    uint32_t status : 32;       /* Status of module. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_THR_NFY_STAT_t;
#endif /* GENHDR_STRUCT */

/* Packet Available Enqueue Scheduler module status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TPS_PKT_ESCH_STAT                        0x00026E18U
/* Status of module. */
#define VIRTIO_DMA_CSR_TPS_PKT_ESCH_STAT_STATUS_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_PKT_ESCH_STAT_STATUS_SHIFT           0
#define VIRTIO_DMA_CSR_TPS_PKT_ESCH_STAT_STATUS_WIDTH           32
#define VIRTIO_DMA_CSR_TPS_PKT_ESCH_STAT_STATUS_MASK            (0xFFFFFFFFU << VIRTIO_DMA_CSR_TPS_PKT_ESCH_STAT_STATUS_SHIFT)

#ifdef GENHDR_STRUCT
/* Packet Available Enqueue Scheduler module status [DEBUG REG] */
typedef union {
  struct {
    uint32_t status : 32;       /* Status of module. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_PKT_ESCH_STAT_t;
#endif /* GENHDR_STRUCT */

/* Packet Length Adjust Enqueue Scheduler module status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TPS_LEN_ESCH_STAT                        0x00026E1CU
/* Status of module. */
#define VIRTIO_DMA_CSR_TPS_LEN_ESCH_STAT_STATUS_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_LEN_ESCH_STAT_STATUS_SHIFT           0
#define VIRTIO_DMA_CSR_TPS_LEN_ESCH_STAT_STATUS_WIDTH           32
#define VIRTIO_DMA_CSR_TPS_LEN_ESCH_STAT_STATUS_MASK            (0xFFFFFFFFU << VIRTIO_DMA_CSR_TPS_LEN_ESCH_STAT_STATUS_SHIFT)

#ifdef GENHDR_STRUCT
/* Packet Length Adjust Enqueue Scheduler module status [DEBUG REG] */
typedef union {
  struct {
    uint32_t status : 32;       /* Status of module. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_LEN_ESCH_STAT_t;
#endif /* GENHDR_STRUCT */

/* Throttle Off Enqueue Scheduler module status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TPS_THR_ESCH_STAT                        0x00026E20U
/* Status of module. */
#define VIRTIO_DMA_CSR_TPS_THR_ESCH_STAT_STATUS_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_THR_ESCH_STAT_STATUS_SHIFT           0
#define VIRTIO_DMA_CSR_TPS_THR_ESCH_STAT_STATUS_WIDTH           32
#define VIRTIO_DMA_CSR_TPS_THR_ESCH_STAT_STATUS_MASK            (0xFFFFFFFFU << VIRTIO_DMA_CSR_TPS_THR_ESCH_STAT_STATUS_SHIFT)

#ifdef GENHDR_STRUCT
/* Throttle Off Enqueue Scheduler module status [DEBUG REG] */
typedef union {
  struct {
    uint32_t status : 32;       /* Status of module. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_THR_ESCH_STAT_t;
#endif /* GENHDR_STRUCT */

/* Shaper Enqueue Scheduler module status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TPS_SHP_ESCH_STAT                        0x00026E24U
/* Status of module. */
#define VIRTIO_DMA_CSR_TPS_SHP_ESCH_STAT_STATUS_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_SHP_ESCH_STAT_STATUS_SHIFT           0
#define VIRTIO_DMA_CSR_TPS_SHP_ESCH_STAT_STATUS_WIDTH           32
#define VIRTIO_DMA_CSR_TPS_SHP_ESCH_STAT_STATUS_MASK            (0xFFFFFFFFU << VIRTIO_DMA_CSR_TPS_SHP_ESCH_STAT_STATUS_SHIFT)

#ifdef GENHDR_STRUCT
/* Shaper Enqueue Scheduler module status [DEBUG REG] */
typedef union {
  struct {
    uint32_t status : 32;       /* Status of module. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_SHP_ESCH_STAT_t;
#endif /* GENHDR_STRUCT */

/* Dequeue Scheduler module status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TPS_PKT_DSCH_STAT                        0x00026E28U
/* Status of module. */
#define VIRTIO_DMA_CSR_TPS_PKT_DSCH_STAT_STATUS_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_PKT_DSCH_STAT_STATUS_SHIFT           0
#define VIRTIO_DMA_CSR_TPS_PKT_DSCH_STAT_STATUS_WIDTH           32
#define VIRTIO_DMA_CSR_TPS_PKT_DSCH_STAT_STATUS_MASK            (0xFFFFFFFFU << VIRTIO_DMA_CSR_TPS_PKT_DSCH_STAT_STATUS_SHIFT)

#ifdef GENHDR_STRUCT
/* Dequeue Scheduler module status [DEBUG REG] */
typedef union {
  struct {
    uint32_t status : 32;       /* Status of module. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_PKT_DSCH_STAT_t;
#endif /* GENHDR_STRUCT */

/* Memory Mux Demux module status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TPS_MUX_STAT                     0x00026E2CU
/* Status of module. */
#define VIRTIO_DMA_CSR_TPS_MUX_STAT_STATUS_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_MUX_STAT_STATUS_SHIFT        0
#define VIRTIO_DMA_CSR_TPS_MUX_STAT_STATUS_WIDTH        32
#define VIRTIO_DMA_CSR_TPS_MUX_STAT_STATUS_MASK         (0xFFFFFFFFU << VIRTIO_DMA_CSR_TPS_MUX_STAT_STATUS_SHIFT)

#ifdef GENHDR_STRUCT
/* Memory Mux Demux module status [DEBUG REG] */
typedef union {
  struct {
    uint32_t status : 32;       /* Status of module. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_MUX_STAT_t;
#endif /* GENHDR_STRUCT */

/* Packet Available output Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT                              0x00026E30U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_CURRENT_LEVEL_SHIFT          0
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_CURRENT_LEVEL_WIDTH          10
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_CURRENT_LEVEL_MASK           (0x3FFU << VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_MAX_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_MAX_SHIFT                    10
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_MAX_WIDTH                    10
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_MAX_MASK                     (0x3FFU << VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_FULL_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_FULL_SHIFT                   20
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_FULL_WIDTH                   1
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_FULL_MASK                    (0x1U << VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_FIFO_OVERFLOW_SHIFT          21
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_FIFO_OVERFLOW_WIDTH          1
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_FIFO_OVERFLOW_MASK           (0x1U << VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_RATE_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_RATE_SHIFT                   22
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_RATE_WIDTH                   10
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_RATE_MASK                    (0x3FFU << VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Packet Available output Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_P_AVL_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Packet Available output Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO2_STAT                     0x00026E34U
/* Total count of entries written. Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO2_STAT_COUNT_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO2_STAT_COUNT_SHIFT         0
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO2_STAT_COUNT_WIDTH         32
#define VIRTIO_DMA_CSR_TPS_P_AVL_FIFO2_STAT_COUNT_MASK          (0xFFFFFFFFU << VIRTIO_DMA_CSR_TPS_P_AVL_FIFO2_STAT_COUNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Packet Available output Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t count : 32;        /* Total count of entries written. Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_P_AVL_FIFO2_STAT_t;
#endif /* GENHDR_STRUCT */

/* Packet Actual Length output Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT                              0x00026E38U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_CURRENT_LEVEL_SHIFT          0
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_CURRENT_LEVEL_WIDTH          10
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_CURRENT_LEVEL_MASK           (0x3FFU << VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_MAX_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_MAX_SHIFT                    10
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_MAX_WIDTH                    10
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_MAX_MASK                     (0x3FFU << VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_FULL_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_FULL_SHIFT                   20
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_FULL_WIDTH                   1
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_FULL_MASK                    (0x1U << VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_FIFO_OVERFLOW_SHIFT          21
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_FIFO_OVERFLOW_WIDTH          1
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_FIFO_OVERFLOW_MASK           (0x1U << VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_RATE_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_RATE_SHIFT                   22
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_RATE_WIDTH                   10
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_RATE_MASK                    (0x3FFU << VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Packet Actual Length output Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_P_LEN_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Packet Actual Length output Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO2_STAT                     0x00026E3CU
/* Total count of entries written. Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO2_STAT_COUNT_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO2_STAT_COUNT_SHIFT         0
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO2_STAT_COUNT_WIDTH         32
#define VIRTIO_DMA_CSR_TPS_P_LEN_FIFO2_STAT_COUNT_MASK          (0xFFFFFFFFU << VIRTIO_DMA_CSR_TPS_P_LEN_FIFO2_STAT_COUNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Packet Actual Length output Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t count : 32;        /* Total count of entries written. Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_P_LEN_FIFO2_STAT_t;
#endif /* GENHDR_STRUCT */

/* Throttle Notify output Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT                              0x00026E40U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_CURRENT_LEVEL_SHIFT          0
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_CURRENT_LEVEL_WIDTH          10
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_CURRENT_LEVEL_MASK           (0x3FFU << VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_MAX_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_MAX_SHIFT                    10
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_MAX_WIDTH                    10
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_MAX_MASK                     (0x3FFU << VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_FULL_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_FULL_SHIFT                   20
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_FULL_WIDTH                   1
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_FULL_MASK                    (0x1U << VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_FIFO_OVERFLOW_SHIFT          21
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_FIFO_OVERFLOW_WIDTH          1
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_FIFO_OVERFLOW_MASK           (0x1U << VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_RATE_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_RATE_SHIFT                   22
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_RATE_WIDTH                   10
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_RATE_MASK                    (0x3FFU << VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Throttle Notify output Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_T_NFY_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Throttle Notify output Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO2_STAT                     0x00026E44U
/* Total count of entries written. Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO2_STAT_COUNT_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO2_STAT_COUNT_SHIFT         0
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO2_STAT_COUNT_WIDTH         32
#define VIRTIO_DMA_CSR_TPS_T_NFY_FIFO2_STAT_COUNT_MASK          (0xFFFFFFFFU << VIRTIO_DMA_CSR_TPS_T_NFY_FIFO2_STAT_COUNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Throttle Notify output Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t count : 32;        /* Total count of entries written. Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_T_NFY_FIFO2_STAT_t;
#endif /* GENHDR_STRUCT */

/* Dequeue Scheduler output Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT                                0x00026E48U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_CURRENT_LEVEL_SHIFT            0
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_CURRENT_LEVEL_WIDTH            10
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_CURRENT_LEVEL_MASK             (0x3FFU << VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_MAX_ACCESSTYPE                 GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_MAX_SHIFT                      10
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_MAX_WIDTH                      10
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_MAX_MASK                       (0x3FFU << VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_FULL_ACCESSTYPE                GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_FULL_SHIFT                     20
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_FULL_WIDTH                     1
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_FULL_MASK                      (0x1U << VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_FIFO_OVERFLOW_SHIFT            21
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_FIFO_OVERFLOW_WIDTH            1
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_FIFO_OVERFLOW_MASK             (0x1U << VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_RATE_ACCESSTYPE                GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_RATE_SHIFT                     22
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_RATE_WIDTH                     10
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_RATE_MASK                      (0x3FFU << VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Dequeue Scheduler output Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_DEQ_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Dequeue Scheduler output Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO2_STAT                       0x00026E4CU
/* Total count of entries written. Resets to 0. */
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO2_STAT_COUNT_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO2_STAT_COUNT_SHIFT           0
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO2_STAT_COUNT_WIDTH           32
#define VIRTIO_DMA_CSR_TPS_DEQ_FIFO2_STAT_COUNT_MASK            (0xFFFFFFFFU << VIRTIO_DMA_CSR_TPS_DEQ_FIFO2_STAT_COUNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Dequeue Scheduler output Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t count : 32;        /* Total count of entries written. Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_DEQ_FIFO2_STAT_t;
#endif /* GENHDR_STRUCT */

/* Control Register [DEBUG REG] */
#define VIRTIO_DMA_CSR_TPS_DBG_CNTL_CFG                                 0x00026E70U
/* Number of bytes a token is worth. Minimum value 64B. */
#define VIRTIO_DMA_CSR_TPS_DBG_CNTL_CFG_TOKEN_BYTES_SHIFT               0
#define VIRTIO_DMA_CSR_TPS_DBG_CNTL_CFG_TOKEN_BYTES_WIDTH               12
#define VIRTIO_DMA_CSR_TPS_DBG_CNTL_CFG_TOKEN_BYTES_MASK                (0xFFFU << VIRTIO_DMA_CSR_TPS_DBG_CNTL_CFG_TOKEN_BYTES_SHIFT)

/*
 * Value of 1 disables pkt length feedback : effectively turns scheduler from
 * being bit based scheduler to pkt based scheduler.
 */
#define VIRTIO_DMA_CSR_TPS_DBG_CNTL_CFG_PKT_LEN_FB_DIS_SHIFT            15
#define VIRTIO_DMA_CSR_TPS_DBG_CNTL_CFG_PKT_LEN_FB_DIS_WIDTH            1
#define VIRTIO_DMA_CSR_TPS_DBG_CNTL_CFG_PKT_LEN_FB_DIS_MASK             (0x1U << VIRTIO_DMA_CSR_TPS_DBG_CNTL_CFG_PKT_LEN_FB_DIS_SHIFT)

/*
 * Packet Length correction i.e. Number of bytes to add to actual packet length
 * (signed twos complement number -128 to 127).
 */
#define VIRTIO_DMA_CSR_TPS_DBG_CNTL_CFG_PKT_LEN_CORR_SHIFT              16
#define VIRTIO_DMA_CSR_TPS_DBG_CNTL_CFG_PKT_LEN_CORR_WIDTH              8
#define VIRTIO_DMA_CSR_TPS_DBG_CNTL_CFG_PKT_LEN_CORR_MASK               (0xFFU << VIRTIO_DMA_CSR_TPS_DBG_CNTL_CFG_PKT_LEN_CORR_SHIFT)

/*
 * 0 : scheduler attempts to output 2 requests for same virtqueue before moving
 * onto next virtqueue. 1: scheduler attempts to interleave requests for
 * dfferent rings. DEBUG: seems to have effect on overall system performance.
 */
#define VIRTIO_DMA_CSR_TPS_DBG_CNTL_CFG_SCH_OP_MIX_SHIFT                31
#define VIRTIO_DMA_CSR_TPS_DBG_CNTL_CFG_SCH_OP_MIX_WIDTH                1
#define VIRTIO_DMA_CSR_TPS_DBG_CNTL_CFG_SCH_OP_MIX_MASK                 (0x1U << VIRTIO_DMA_CSR_TPS_DBG_CNTL_CFG_SCH_OP_MIX_SHIFT)

#ifdef GENHDR_STRUCT
/* Control Register [DEBUG REG] */
typedef union {
  struct {
    uint32_t token_bytes : 12;          /* Number of bytes a token is worth. Minimum value 64B. */
    uint32_t : 3;
    uint32_t pkt_len_fb_dis : 1;        /* Value of 1 disables pkt length feedback : effectively turns scheduler from
                                           being bit based scheduler to pkt based scheduler. */
    uint32_t pkt_len_corr : 8;          /* Packet Length correction i.e. Number of bytes to add to actual packet length
                                           (signed twos complement number -128 to 127). */
    uint32_t : 7;
    uint32_t sch_op_mix : 1;            /* 0 : scheduler attempts to output 2 requests for same virtqueue before moving
                                           onto next virtqueue. 1: scheduler attempts to interleave requests for
                                           dfferent rings. DEBUG: seems to have effect on overall system performance. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_DBG_CNTL_CFG_t;
#endif /* GENHDR_STRUCT */

/*
 * Shaper State3 Table (NOTE: only up to max of first 128 queues and only
 * reading 32bits of state)
 */
#define VIRTIO_DMA_CSR_TPS_SHP_STATE3_STAT(tps_shp_state3_stat_ix)      (0x00027000U + ((tps_shp_state3_stat_ix) * 4))
#define VIRTIO_DMA_CSR_TPS_SHP_STATE3_STAT_MAX_INDEX                    0x7FU
/* Pps Shaper level (24.2 signed fixed point format) in multiples of 10 bytes. */
#define VIRTIO_DMA_CSR_TPS_SHP_STATE3_STAT_SHAPER_PPS_LEVEL_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TPS_SHP_STATE3_STAT_SHAPER_PPS_LEVEL_SHIFT       0
#define VIRTIO_DMA_CSR_TPS_SHP_STATE3_STAT_SHAPER_PPS_LEVEL_WIDTH       32
#define VIRTIO_DMA_CSR_TPS_SHP_STATE3_STAT_SHAPER_PPS_LEVEL_MASK        (0xFFFFFFFFU << VIRTIO_DMA_CSR_TPS_SHP_STATE3_STAT_SHAPER_PPS_LEVEL_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Shaper State3 Table (NOTE: only up to max of first 128 queues and only
 * reading 32bits of state)
 */
typedef union {
  struct {
    uint32_t shaper_pps_level : 32;     /* Pps Shaper level (24.2 signed fixed point format) in multiples of 10 bytes. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TPS_SHP_STATE3_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Available Index Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT                              0x00028000U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_CURRENT_LEVEL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_CURRENT_LEVEL_SHIFT          0
#define VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_CURRENT_LEVEL_WIDTH          10
#define VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_CURRENT_LEVEL_MASK           (0x3FFU << VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_MAX_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_MAX_SHIFT                    10
#define VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_MAX_WIDTH                    10
#define VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_MAX_MASK                     (0x3FFU << VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_FULL_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_FULL_SHIFT                   20
#define VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_FULL_WIDTH                   1
#define VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_FULL_MASK                    (0x1U << VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_FIFO_OVERFLOW_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_FIFO_OVERFLOW_SHIFT          21
#define VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_FIFO_OVERFLOW_WIDTH          1
#define VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_FIFO_OVERFLOW_MASK           (0x1U << VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_RATE_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_RATE_SHIFT                   22
#define VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_RATE_WIDTH                   10
#define VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_RATE_MASK                    (0x3FFU << VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Available Index Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VA_TX_AVAILIDX_STAT_t;
#endif /* GENHDR_STRUCT */

/* Statistics Reset [DEBUG REG] */
#define VIRTIO_DMA_CSR_VA_STATS_RST                             0x00028000U
/* Write 1 to reset all stats registers in this block. */
#define VIRTIO_DMA_CSR_VA_STATS_RST_RESET_STATS_ACCESSTYPE      GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_VA_STATS_RST_RESET_STATS_SHIFT           0
#define VIRTIO_DMA_CSR_VA_STATS_RST_RESET_STATS_WIDTH           1
#define VIRTIO_DMA_CSR_VA_STATS_RST_RESET_STATS_MASK            (0x1U << VIRTIO_DMA_CSR_VA_STATS_RST_RESET_STATS_SHIFT)

#ifdef GENHDR_STRUCT
/* Statistics Reset [DEBUG REG] */
typedef union {
  struct {
    uint32_t reset_stats : 1;   /* Write 1 to reset all stats registers in this block. */
    uint32_t : 31;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VA_STATS_RST_t;
#endif /* GENHDR_STRUCT */

/* Tx Ring Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_VA_TX_RING_STAT                          0x00028008U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_RING_STAT_CURRENT_LEVEL_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_RING_STAT_CURRENT_LEVEL_SHIFT      0
#define VIRTIO_DMA_CSR_VA_TX_RING_STAT_CURRENT_LEVEL_WIDTH      10
#define VIRTIO_DMA_CSR_VA_TX_RING_STAT_CURRENT_LEVEL_MASK       (0x3FFU << VIRTIO_DMA_CSR_VA_TX_RING_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_RING_STAT_MAX_ACCESSTYPE           GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_RING_STAT_MAX_SHIFT                10
#define VIRTIO_DMA_CSR_VA_TX_RING_STAT_MAX_WIDTH                10
#define VIRTIO_DMA_CSR_VA_TX_RING_STAT_MAX_MASK                 (0x3FFU << VIRTIO_DMA_CSR_VA_TX_RING_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_RING_STAT_FULL_ACCESSTYPE          GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_RING_STAT_FULL_SHIFT               20
#define VIRTIO_DMA_CSR_VA_TX_RING_STAT_FULL_WIDTH               1
#define VIRTIO_DMA_CSR_VA_TX_RING_STAT_FULL_MASK                (0x1U << VIRTIO_DMA_CSR_VA_TX_RING_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_RING_STAT_FIFO_OVERFLOW_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_RING_STAT_FIFO_OVERFLOW_SHIFT      21
#define VIRTIO_DMA_CSR_VA_TX_RING_STAT_FIFO_OVERFLOW_WIDTH      1
#define VIRTIO_DMA_CSR_VA_TX_RING_STAT_FIFO_OVERFLOW_MASK       (0x1U << VIRTIO_DMA_CSR_VA_TX_RING_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_RING_STAT_RATE_ACCESSTYPE          GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_RING_STAT_RATE_SHIFT               22
#define VIRTIO_DMA_CSR_VA_TX_RING_STAT_RATE_WIDTH               10
#define VIRTIO_DMA_CSR_VA_TX_RING_STAT_RATE_MASK                (0x3FFU << VIRTIO_DMA_CSR_VA_TX_RING_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Ring Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VA_TX_RING_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Ring CLP Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT                               0x0002800CU
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_CURRENT_LEVEL_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_CURRENT_LEVEL_SHIFT           0
#define VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_CURRENT_LEVEL_WIDTH           10
#define VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_CURRENT_LEVEL_MASK            (0x3FFU << VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_MAX_ACCESSTYPE                GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_MAX_SHIFT                     10
#define VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_MAX_WIDTH                     10
#define VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_MAX_MASK                      (0x3FFU << VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_FULL_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_FULL_SHIFT                    20
#define VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_FULL_WIDTH                    1
#define VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_FULL_MASK                     (0x1U << VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_FIFO_OVERFLOW_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_FIFO_OVERFLOW_SHIFT           21
#define VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_FIFO_OVERFLOW_WIDTH           1
#define VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_FIFO_OVERFLOW_MASK            (0x1U << VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_RATE_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_RATE_SHIFT                    22
#define VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_RATE_WIDTH                    10
#define VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_RATE_MASK                     (0x3FFU << VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Ring CLP Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VA_TX_RINGCLP_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Descriptor Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_VA_TX_DESC_STAT                          0x00028010U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_DESC_STAT_CURRENT_LEVEL_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_DESC_STAT_CURRENT_LEVEL_SHIFT      0
#define VIRTIO_DMA_CSR_VA_TX_DESC_STAT_CURRENT_LEVEL_WIDTH      10
#define VIRTIO_DMA_CSR_VA_TX_DESC_STAT_CURRENT_LEVEL_MASK       (0x3FFU << VIRTIO_DMA_CSR_VA_TX_DESC_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_DESC_STAT_MAX_ACCESSTYPE           GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_DESC_STAT_MAX_SHIFT                10
#define VIRTIO_DMA_CSR_VA_TX_DESC_STAT_MAX_WIDTH                10
#define VIRTIO_DMA_CSR_VA_TX_DESC_STAT_MAX_MASK                 (0x3FFU << VIRTIO_DMA_CSR_VA_TX_DESC_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_DESC_STAT_FULL_ACCESSTYPE          GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_DESC_STAT_FULL_SHIFT               20
#define VIRTIO_DMA_CSR_VA_TX_DESC_STAT_FULL_WIDTH               1
#define VIRTIO_DMA_CSR_VA_TX_DESC_STAT_FULL_MASK                (0x1U << VIRTIO_DMA_CSR_VA_TX_DESC_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_DESC_STAT_FIFO_OVERFLOW_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_DESC_STAT_FIFO_OVERFLOW_SHIFT      21
#define VIRTIO_DMA_CSR_VA_TX_DESC_STAT_FIFO_OVERFLOW_WIDTH      1
#define VIRTIO_DMA_CSR_VA_TX_DESC_STAT_FIFO_OVERFLOW_MASK       (0x1U << VIRTIO_DMA_CSR_VA_TX_DESC_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_DESC_STAT_RATE_ACCESSTYPE          GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_DESC_STAT_RATE_SHIFT               22
#define VIRTIO_DMA_CSR_VA_TX_DESC_STAT_RATE_WIDTH               10
#define VIRTIO_DMA_CSR_VA_TX_DESC_STAT_RATE_MASK                (0x3FFU << VIRTIO_DMA_CSR_VA_TX_DESC_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Descriptor Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VA_TX_DESC_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Packet Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_VA_TX_PKT_STAT                           0x00028014U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_PKT_STAT_CURRENT_LEVEL_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_PKT_STAT_CURRENT_LEVEL_SHIFT       0
#define VIRTIO_DMA_CSR_VA_TX_PKT_STAT_CURRENT_LEVEL_WIDTH       10
#define VIRTIO_DMA_CSR_VA_TX_PKT_STAT_CURRENT_LEVEL_MASK        (0x3FFU << VIRTIO_DMA_CSR_VA_TX_PKT_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_PKT_STAT_MAX_ACCESSTYPE            GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_PKT_STAT_MAX_SHIFT                 10
#define VIRTIO_DMA_CSR_VA_TX_PKT_STAT_MAX_WIDTH                 10
#define VIRTIO_DMA_CSR_VA_TX_PKT_STAT_MAX_MASK                  (0x3FFU << VIRTIO_DMA_CSR_VA_TX_PKT_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_PKT_STAT_FULL_ACCESSTYPE           GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_PKT_STAT_FULL_SHIFT                20
#define VIRTIO_DMA_CSR_VA_TX_PKT_STAT_FULL_WIDTH                1
#define VIRTIO_DMA_CSR_VA_TX_PKT_STAT_FULL_MASK                 (0x1U << VIRTIO_DMA_CSR_VA_TX_PKT_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_PKT_STAT_FIFO_OVERFLOW_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_PKT_STAT_FIFO_OVERFLOW_SHIFT       21
#define VIRTIO_DMA_CSR_VA_TX_PKT_STAT_FIFO_OVERFLOW_WIDTH       1
#define VIRTIO_DMA_CSR_VA_TX_PKT_STAT_FIFO_OVERFLOW_MASK        (0x1U << VIRTIO_DMA_CSR_VA_TX_PKT_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_PKT_STAT_RATE_ACCESSTYPE           GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_PKT_STAT_RATE_SHIFT                22
#define VIRTIO_DMA_CSR_VA_TX_PKT_STAT_RATE_WIDTH                10
#define VIRTIO_DMA_CSR_VA_TX_PKT_STAT_RATE_MASK                 (0x3FFU << VIRTIO_DMA_CSR_VA_TX_PKT_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Packet Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VA_TX_PKT_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Update Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT                                0x00028018U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_CURRENT_LEVEL_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_CURRENT_LEVEL_SHIFT            0
#define VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_CURRENT_LEVEL_WIDTH            10
#define VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_CURRENT_LEVEL_MASK             (0x3FFU << VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_MAX_ACCESSTYPE                 GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_MAX_SHIFT                      10
#define VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_MAX_WIDTH                      10
#define VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_MAX_MASK                       (0x3FFU << VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_FULL_ACCESSTYPE                GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_FULL_SHIFT                     20
#define VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_FULL_WIDTH                     1
#define VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_FULL_MASK                      (0x1U << VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_FIFO_OVERFLOW_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_FIFO_OVERFLOW_SHIFT            21
#define VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_FIFO_OVERFLOW_WIDTH            1
#define VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_FIFO_OVERFLOW_MASK             (0x1U << VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_RATE_ACCESSTYPE                GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_RATE_SHIFT                     22
#define VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_RATE_WIDTH                     10
#define VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_RATE_MASK                      (0x3FFU << VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Update Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VA_TX_UPDATE_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Free Tag Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_VA_FREETAG_STAT                          0x0002801CU
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_VA_FREETAG_STAT_CURRENT_LEVEL_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_FREETAG_STAT_CURRENT_LEVEL_SHIFT      0
#define VIRTIO_DMA_CSR_VA_FREETAG_STAT_CURRENT_LEVEL_WIDTH      10
#define VIRTIO_DMA_CSR_VA_FREETAG_STAT_CURRENT_LEVEL_MASK       (0x3FFU << VIRTIO_DMA_CSR_VA_FREETAG_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Min fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_FREETAG_STAT_MIN_ACCESSTYPE           GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_FREETAG_STAT_MIN_SHIFT                10
#define VIRTIO_DMA_CSR_VA_FREETAG_STAT_MIN_WIDTH                10
#define VIRTIO_DMA_CSR_VA_FREETAG_STAT_MIN_MASK                 (0x3FFU << VIRTIO_DMA_CSR_VA_FREETAG_STAT_MIN_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_FREETAG_STAT_RATE_ACCESSTYPE          GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_FREETAG_STAT_RATE_SHIFT               22
#define VIRTIO_DMA_CSR_VA_FREETAG_STAT_RATE_WIDTH               10
#define VIRTIO_DMA_CSR_VA_FREETAG_STAT_RATE_MASK                (0x3FFU << VIRTIO_DMA_CSR_VA_FREETAG_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Free Tag Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t min : 10;                  /* Fifo Min fill level (latched). Resets to 0. */
    uint32_t : 2;
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VA_FREETAG_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Available Index Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT                              0x0002C000U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_CURRENT_LEVEL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_CURRENT_LEVEL_SHIFT          0
#define VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_CURRENT_LEVEL_WIDTH          10
#define VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_CURRENT_LEVEL_MASK           (0x3FFU << VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_MAX_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_MAX_SHIFT                    10
#define VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_MAX_WIDTH                    10
#define VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_MAX_MASK                     (0x3FFU << VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_FULL_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_FULL_SHIFT                   20
#define VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_FULL_WIDTH                   1
#define VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_FULL_MASK                    (0x1U << VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_FIFO_OVERFLOW_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_FIFO_OVERFLOW_SHIFT          21
#define VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_FIFO_OVERFLOW_WIDTH          1
#define VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_FIFO_OVERFLOW_MASK           (0x1U << VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_RATE_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_RATE_SHIFT                   22
#define VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_RATE_WIDTH                   10
#define VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_RATE_MASK                    (0x3FFU << VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Available Index Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VA_RX_AVAILIDX_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Ring Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_VA_RX_RING_STAT                          0x0002C008U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_RING_STAT_CURRENT_LEVEL_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_RING_STAT_CURRENT_LEVEL_SHIFT      0
#define VIRTIO_DMA_CSR_VA_RX_RING_STAT_CURRENT_LEVEL_WIDTH      10
#define VIRTIO_DMA_CSR_VA_RX_RING_STAT_CURRENT_LEVEL_MASK       (0x3FFU << VIRTIO_DMA_CSR_VA_RX_RING_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_RING_STAT_MAX_ACCESSTYPE           GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_RING_STAT_MAX_SHIFT                10
#define VIRTIO_DMA_CSR_VA_RX_RING_STAT_MAX_WIDTH                10
#define VIRTIO_DMA_CSR_VA_RX_RING_STAT_MAX_MASK                 (0x3FFU << VIRTIO_DMA_CSR_VA_RX_RING_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_RING_STAT_FULL_ACCESSTYPE          GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_RING_STAT_FULL_SHIFT               20
#define VIRTIO_DMA_CSR_VA_RX_RING_STAT_FULL_WIDTH               1
#define VIRTIO_DMA_CSR_VA_RX_RING_STAT_FULL_MASK                (0x1U << VIRTIO_DMA_CSR_VA_RX_RING_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_RING_STAT_FIFO_OVERFLOW_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_RING_STAT_FIFO_OVERFLOW_SHIFT      21
#define VIRTIO_DMA_CSR_VA_RX_RING_STAT_FIFO_OVERFLOW_WIDTH      1
#define VIRTIO_DMA_CSR_VA_RX_RING_STAT_FIFO_OVERFLOW_MASK       (0x1U << VIRTIO_DMA_CSR_VA_RX_RING_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_RING_STAT_RATE_ACCESSTYPE          GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_RING_STAT_RATE_SHIFT               22
#define VIRTIO_DMA_CSR_VA_RX_RING_STAT_RATE_WIDTH               10
#define VIRTIO_DMA_CSR_VA_RX_RING_STAT_RATE_MASK                (0x3FFU << VIRTIO_DMA_CSR_VA_RX_RING_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Ring Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VA_RX_RING_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Ring CLP Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT                               0x0002C00CU
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_CURRENT_LEVEL_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_CURRENT_LEVEL_SHIFT           0
#define VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_CURRENT_LEVEL_WIDTH           10
#define VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_CURRENT_LEVEL_MASK            (0x3FFU << VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_MAX_ACCESSTYPE                GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_MAX_SHIFT                     10
#define VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_MAX_WIDTH                     10
#define VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_MAX_MASK                      (0x3FFU << VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_FULL_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_FULL_SHIFT                    20
#define VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_FULL_WIDTH                    1
#define VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_FULL_MASK                     (0x1U << VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_FIFO_OVERFLOW_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_FIFO_OVERFLOW_SHIFT           21
#define VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_FIFO_OVERFLOW_WIDTH           1
#define VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_FIFO_OVERFLOW_MASK            (0x1U << VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_RATE_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_RATE_SHIFT                    22
#define VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_RATE_WIDTH                    10
#define VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_RATE_MASK                     (0x3FFU << VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Ring CLP Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VA_RX_RINGCLP_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Descriptor Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_VA_RX_DESC_STAT                          0x0002C010U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_DESC_STAT_CURRENT_LEVEL_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_DESC_STAT_CURRENT_LEVEL_SHIFT      0
#define VIRTIO_DMA_CSR_VA_RX_DESC_STAT_CURRENT_LEVEL_WIDTH      10
#define VIRTIO_DMA_CSR_VA_RX_DESC_STAT_CURRENT_LEVEL_MASK       (0x3FFU << VIRTIO_DMA_CSR_VA_RX_DESC_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_DESC_STAT_MAX_ACCESSTYPE           GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_DESC_STAT_MAX_SHIFT                10
#define VIRTIO_DMA_CSR_VA_RX_DESC_STAT_MAX_WIDTH                10
#define VIRTIO_DMA_CSR_VA_RX_DESC_STAT_MAX_MASK                 (0x3FFU << VIRTIO_DMA_CSR_VA_RX_DESC_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_DESC_STAT_FULL_ACCESSTYPE          GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_DESC_STAT_FULL_SHIFT               20
#define VIRTIO_DMA_CSR_VA_RX_DESC_STAT_FULL_WIDTH               1
#define VIRTIO_DMA_CSR_VA_RX_DESC_STAT_FULL_MASK                (0x1U << VIRTIO_DMA_CSR_VA_RX_DESC_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_DESC_STAT_FIFO_OVERFLOW_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_DESC_STAT_FIFO_OVERFLOW_SHIFT      21
#define VIRTIO_DMA_CSR_VA_RX_DESC_STAT_FIFO_OVERFLOW_WIDTH      1
#define VIRTIO_DMA_CSR_VA_RX_DESC_STAT_FIFO_OVERFLOW_MASK       (0x1U << VIRTIO_DMA_CSR_VA_RX_DESC_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_DESC_STAT_RATE_ACCESSTYPE          GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_DESC_STAT_RATE_SHIFT               22
#define VIRTIO_DMA_CSR_VA_RX_DESC_STAT_RATE_WIDTH               10
#define VIRTIO_DMA_CSR_VA_RX_DESC_STAT_RATE_MASK                (0x3FFU << VIRTIO_DMA_CSR_VA_RX_DESC_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Descriptor Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VA_RX_DESC_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Update Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT                                0x0002C018U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_CURRENT_LEVEL_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_CURRENT_LEVEL_SHIFT            0
#define VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_CURRENT_LEVEL_WIDTH            10
#define VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_CURRENT_LEVEL_MASK             (0x3FFU << VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_MAX_ACCESSTYPE                 GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_MAX_SHIFT                      10
#define VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_MAX_WIDTH                      10
#define VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_MAX_MASK                       (0x3FFU << VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_FULL_ACCESSTYPE                GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_FULL_SHIFT                     20
#define VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_FULL_WIDTH                     1
#define VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_FULL_MASK                      (0x1U << VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_FIFO_OVERFLOW_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_FIFO_OVERFLOW_SHIFT            21
#define VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_FIFO_OVERFLOW_WIDTH            1
#define VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_FIFO_OVERFLOW_MASK             (0x1U << VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_RATE_ACCESSTYPE                GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_RATE_SHIFT                     22
#define VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_RATE_WIDTH                     10
#define VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_RATE_MASK                      (0x3FFU << VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Update Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_VA_RX_UPDATE_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Index Multibuffer VF0 Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT                             0x00030000U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_CURRENT_LEVEL_SHIFT         0
#define VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_CURRENT_LEVEL_WIDTH         10
#define VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_CURRENT_LEVEL_MASK          (0x3FFU << VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_MAX_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_MAX_SHIFT                   10
#define VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_MAX_WIDTH                   10
#define VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_MAX_MASK                    (0x3FFU << VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_FULL_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_FULL_SHIFT                  20
#define VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_FULL_WIDTH                  1
#define VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_FULL_MASK                   (0x1U << VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_FIFO_OVERFLOW_SHIFT         21
#define VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_FIFO_OVERFLOW_WIDTH         1
#define VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_FIFO_OVERFLOW_MASK          (0x1U << VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_RATE_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_RATE_SHIFT                  22
#define VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_RATE_WIDTH                  10
#define VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_RATE_MASK                   (0x3FFU << VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Index Multibuffer VF0 Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFI_MB_VF0_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Index Statistics Reset [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFI_STATS_RST                            0x00030000U
/* Write 1 to reset all stats registers in this block. */
#define VIRTIO_DMA_CSR_TFI_STATS_RST_RESET_STATS_ACCESSTYPE     GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_TFI_STATS_RST_RESET_STATS_SHIFT          0
#define VIRTIO_DMA_CSR_TFI_STATS_RST_RESET_STATS_WIDTH          1
#define VIRTIO_DMA_CSR_TFI_STATS_RST_RESET_STATS_MASK           (0x1U << VIRTIO_DMA_CSR_TFI_STATS_RST_RESET_STATS_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Index Statistics Reset [DEBUG REG] */
typedef union {
  struct {
    uint32_t reset_stats : 1;   /* Write 1 to reset all stats registers in this block. */
    uint32_t : 31;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFI_STATS_RST_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Index Multibuffer VF1 Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT                             0x00030004U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_CURRENT_LEVEL_SHIFT         0
#define VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_CURRENT_LEVEL_WIDTH         10
#define VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_CURRENT_LEVEL_MASK          (0x3FFU << VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_MAX_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_MAX_SHIFT                   10
#define VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_MAX_WIDTH                   10
#define VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_MAX_MASK                    (0x3FFU << VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_FULL_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_FULL_SHIFT                  20
#define VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_FULL_WIDTH                  1
#define VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_FULL_MASK                   (0x1U << VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_FIFO_OVERFLOW_SHIFT         21
#define VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_FIFO_OVERFLOW_WIDTH         1
#define VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_FIFO_OVERFLOW_MASK          (0x1U << VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_RATE_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_RATE_SHIFT                  22
#define VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_RATE_WIDTH                  10
#define VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_RATE_MASK                   (0x3FFU << VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Index Multibuffer VF1 Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFI_MB_VF1_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Index Interrupt Suppression */
#define VIRTIO_DMA_CSR_TFI_IRQ_SUPPRESS                                 0x00030004U
/* Disable Interrupt Suppression when set. */
#define VIRTIO_DMA_CSR_TFI_IRQ_SUPPRESS_IRQ_SUPR_DISABLE_ACCESSTYPE     GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_TFI_IRQ_SUPPRESS_IRQ_SUPR_DISABLE_SHIFT          0
#define VIRTIO_DMA_CSR_TFI_IRQ_SUPPRESS_IRQ_SUPR_DISABLE_WIDTH          1
#define VIRTIO_DMA_CSR_TFI_IRQ_SUPPRESS_IRQ_SUPR_DISABLE_MASK           (0x1U << VIRTIO_DMA_CSR_TFI_IRQ_SUPPRESS_IRQ_SUPR_DISABLE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Index Interrupt Suppression */
typedef union {
  struct {
    uint32_t irq_supr_disable : 1;      /* Disable Interrupt Suppression when set. */
    uint32_t : 31;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFI_IRQ_SUPPRESS_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Index Multibuffer VF2 Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT                             0x00030008U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_CURRENT_LEVEL_SHIFT         0
#define VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_CURRENT_LEVEL_WIDTH         10
#define VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_CURRENT_LEVEL_MASK          (0x3FFU << VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_MAX_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_MAX_SHIFT                   10
#define VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_MAX_WIDTH                   10
#define VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_MAX_MASK                    (0x3FFU << VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_FULL_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_FULL_SHIFT                  20
#define VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_FULL_WIDTH                  1
#define VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_FULL_MASK                   (0x1U << VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_FIFO_OVERFLOW_SHIFT         21
#define VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_FIFO_OVERFLOW_WIDTH         1
#define VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_FIFO_OVERFLOW_MASK          (0x1U << VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_RATE_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_RATE_SHIFT                  22
#define VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_RATE_WIDTH                  10
#define VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_RATE_MASK                   (0x3FFU << VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Index Multibuffer VF2 Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFI_MB_VF2_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Index Notify Kick [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK                          0x00030008U
/* Notify Kick for a QID. */
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_NOTIFY_ENABLE_ACCESSTYPE GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_NOTIFY_ENABLE_SHIFT      0
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_NOTIFY_ENABLE_WIDTH      1
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_NOTIFY_ENABLE_MASK       (0x1U << VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_NOTIFY_ENABLE_SHIFT)

/* Reserved. */
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_RSVD_0_ACCESSTYPE        GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_RSVD_0_SHIFT             1
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_RSVD_0_WIDTH             3
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_RSVD_0_MASK              (0x7U << VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_RSVD_0_SHIFT)

/* QID number to be notified. */
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_QID_ACCESSTYPE           GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_QID_SHIFT                4
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_QID_WIDTH                10
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_QID_MASK                 (0x3FFU << VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_QID_SHIFT)

/* Reserved. */
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_RSVD_1_ACCESSTYPE        GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_RSVD_1_SHIFT             14
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_RSVD_1_WIDTH             2
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_RSVD_1_MASK              (0x3U << VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_RSVD_1_SHIFT)

/* Polling Rate (cycles). */
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_POLLING_RATE_ACCESSTYPE  GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_POLLING_RATE_SHIFT       16
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_POLLING_RATE_WIDTH       12
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_POLLING_RATE_MASK        (0xFFFU << VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_POLLING_RATE_SHIFT)

/* Reserved. */
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_RSVD_2_ACCESSTYPE        GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_RSVD_2_SHIFT             28
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_RSVD_2_WIDTH             3
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_RSVD_2_MASK              (0x7U << VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_RSVD_2_SHIFT)

/* Enable h/w AvailIndex Polling. */
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_POLLING_ENABLE_ACCESSTYPE GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_POLLING_ENABLE_SHIFT     31
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_POLLING_ENABLE_WIDTH     1
#define VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_POLLING_ENABLE_MASK      (0x1U << VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_POLLING_ENABLE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Index Notify Kick [DEBUG REG] */
typedef union {
  struct {
    uint32_t notify_enable : 1;         /* Notify Kick for a QID. */
    uint32_t rsvd_0 : 3;                /* Reserved. */
    uint32_t qid : 10;                  /* QID number to be notified. */
    uint32_t rsvd_1 : 2;                /* Reserved. */
    uint32_t polling_rate : 12;         /* Polling Rate (cycles). */
    uint32_t rsvd_2 : 3;                /* Reserved. */
    uint32_t polling_enable : 1;        /* Enable h/w AvailIndex Polling. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFI_NOTIFY_KICK_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Index Multibuffer VF3 Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT                             0x0003000CU
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_CURRENT_LEVEL_SHIFT         0
#define VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_CURRENT_LEVEL_WIDTH         10
#define VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_CURRENT_LEVEL_MASK          (0x3FFU << VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_MAX_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_MAX_SHIFT                   10
#define VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_MAX_WIDTH                   10
#define VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_MAX_MASK                    (0x3FFU << VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_FULL_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_FULL_SHIFT                  20
#define VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_FULL_WIDTH                  1
#define VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_FULL_MASK                   (0x1U << VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_FIFO_OVERFLOW_SHIFT         21
#define VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_FIFO_OVERFLOW_WIDTH         1
#define VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_FIFO_OVERFLOW_MASK          (0x1U << VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_RATE_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_RATE_SHIFT                  22
#define VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_RATE_WIDTH                  10
#define VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_RATE_MASK                   (0x3FFU << VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Index Multibuffer VF3 Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFI_MB_VF3_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Index Notify Suppression [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFI_NFY_SUPPRESS                                 0x0003000CU
/*
 * Write 1 to Disable Notify Suppression Flag setting when performing used ring
 * updates.
 */
#define VIRTIO_DMA_CSR_TFI_NFY_SUPPRESS_NFY_SUPR_DIS_ACCESSTYPE         GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_TFI_NFY_SUPPRESS_NFY_SUPR_DIS_SHIFT              0
#define VIRTIO_DMA_CSR_TFI_NFY_SUPPRESS_NFY_SUPR_DIS_WIDTH              1
#define VIRTIO_DMA_CSR_TFI_NFY_SUPPRESS_NFY_SUPR_DIS_MASK               (0x1U << VIRTIO_DMA_CSR_TFI_NFY_SUPPRESS_NFY_SUPR_DIS_SHIFT)

/*
 * Manually set the Notify Suppression Flag when performing used ring updates.
 * Note that the host may not send any notifies until this bit is cleared.
 */
#define VIRTIO_DMA_CSR_TFI_NFY_SUPPRESS_FORCE_NFY_SUPR_ACCESSTYPE       GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_TFI_NFY_SUPPRESS_FORCE_NFY_SUPR_SHIFT            1
#define VIRTIO_DMA_CSR_TFI_NFY_SUPPRESS_FORCE_NFY_SUPR_WIDTH            1
#define VIRTIO_DMA_CSR_TFI_NFY_SUPPRESS_FORCE_NFY_SUPR_MASK             (0x1U << VIRTIO_DMA_CSR_TFI_NFY_SUPPRESS_FORCE_NFY_SUPR_SHIFT)

/* Notify Threshold. Functionality TBC. */
#define VIRTIO_DMA_CSR_TFI_NFY_SUPPRESS_NFY_THRESHOLD_ACCESSTYPE        GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_TFI_NFY_SUPPRESS_NFY_THRESHOLD_SHIFT             5
#define VIRTIO_DMA_CSR_TFI_NFY_SUPPRESS_NFY_THRESHOLD_WIDTH             16
#define VIRTIO_DMA_CSR_TFI_NFY_SUPPRESS_NFY_THRESHOLD_MASK              (0xFFFFU << VIRTIO_DMA_CSR_TFI_NFY_SUPPRESS_NFY_THRESHOLD_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Index Notify Suppression [DEBUG REG] */
typedef union {
  struct {
    uint32_t nfy_supr_dis : 1;          /* Write 1 to Disable Notify Suppression Flag setting when performing used ring
                                           updates. */
    uint32_t force_nfy_supr : 1;        /* Manually set the Notify Suppression Flag when performing used ring updates.
                                           Note that the host may not send any notifies until this bit is cleared. */
    uint32_t : 3;
    uint32_t nfy_threshold : 16;        /* Notify Threshold. Functionality TBC. */
    uint32_t : 11;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFI_NFY_SUPPRESS_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Index VF0 Tail Pointer [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFI_TPTR_VF0_STAT                        0x00030010U
/* Current Software Tail Pointer. Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_TPTR_VF0_STAT_SW_TAIL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_TPTR_VF0_STAT_SW_TAIL_SHIFT          0
#define VIRTIO_DMA_CSR_TFI_TPTR_VF0_STAT_SW_TAIL_WIDTH          16
#define VIRTIO_DMA_CSR_TFI_TPTR_VF0_STAT_SW_TAIL_MASK           (0xFFFFU << VIRTIO_DMA_CSR_TFI_TPTR_VF0_STAT_SW_TAIL_SHIFT)

/* Current Hardware Tail Pointer. Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_TPTR_VF0_STAT_HW_TAIL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_TPTR_VF0_STAT_HW_TAIL_SHIFT          16
#define VIRTIO_DMA_CSR_TFI_TPTR_VF0_STAT_HW_TAIL_WIDTH          16
#define VIRTIO_DMA_CSR_TFI_TPTR_VF0_STAT_HW_TAIL_MASK           (0xFFFFU << VIRTIO_DMA_CSR_TFI_TPTR_VF0_STAT_HW_TAIL_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Index VF0 Tail Pointer [DEBUG REG] */
typedef union {
  struct {
    uint32_t sw_tail : 16;      /* Current Software Tail Pointer. Resets to 0. */
    uint32_t hw_tail : 16;      /* Current Hardware Tail Pointer. Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFI_TPTR_VF0_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Index VF1 Tail Pointer [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFI_TPTR_VF1_STAT                        0x00030014U
/* Current Software Tail Pointer. Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_TPTR_VF1_STAT_SW_TAIL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_TPTR_VF1_STAT_SW_TAIL_SHIFT          0
#define VIRTIO_DMA_CSR_TFI_TPTR_VF1_STAT_SW_TAIL_WIDTH          16
#define VIRTIO_DMA_CSR_TFI_TPTR_VF1_STAT_SW_TAIL_MASK           (0xFFFFU << VIRTIO_DMA_CSR_TFI_TPTR_VF1_STAT_SW_TAIL_SHIFT)

/* Current Hardware Tail Pointer. Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_TPTR_VF1_STAT_HW_TAIL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_TPTR_VF1_STAT_HW_TAIL_SHIFT          16
#define VIRTIO_DMA_CSR_TFI_TPTR_VF1_STAT_HW_TAIL_WIDTH          16
#define VIRTIO_DMA_CSR_TFI_TPTR_VF1_STAT_HW_TAIL_MASK           (0xFFFFU << VIRTIO_DMA_CSR_TFI_TPTR_VF1_STAT_HW_TAIL_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Index VF1 Tail Pointer [DEBUG REG] */
typedef union {
  struct {
    uint32_t sw_tail : 16;      /* Current Software Tail Pointer. Resets to 0. */
    uint32_t hw_tail : 16;      /* Current Hardware Tail Pointer. Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFI_TPTR_VF1_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Index VF2 Tail Pointer [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFI_TPTR_VF2_STAT                        0x00030018U
/* Current Software Tail Pointer. Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_TPTR_VF2_STAT_SW_TAIL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_TPTR_VF2_STAT_SW_TAIL_SHIFT          0
#define VIRTIO_DMA_CSR_TFI_TPTR_VF2_STAT_SW_TAIL_WIDTH          16
#define VIRTIO_DMA_CSR_TFI_TPTR_VF2_STAT_SW_TAIL_MASK           (0xFFFFU << VIRTIO_DMA_CSR_TFI_TPTR_VF2_STAT_SW_TAIL_SHIFT)

/* Current Hardware Tail Pointer. Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_TPTR_VF2_STAT_HW_TAIL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_TPTR_VF2_STAT_HW_TAIL_SHIFT          16
#define VIRTIO_DMA_CSR_TFI_TPTR_VF2_STAT_HW_TAIL_WIDTH          16
#define VIRTIO_DMA_CSR_TFI_TPTR_VF2_STAT_HW_TAIL_MASK           (0xFFFFU << VIRTIO_DMA_CSR_TFI_TPTR_VF2_STAT_HW_TAIL_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Index VF2 Tail Pointer [DEBUG REG] */
typedef union {
  struct {
    uint32_t sw_tail : 16;      /* Current Software Tail Pointer. Resets to 0. */
    uint32_t hw_tail : 16;      /* Current Hardware Tail Pointer. Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFI_TPTR_VF2_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Index VF3 Tail Pointer [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFI_TPTR_VF3_STAT                        0x0003001CU
/* Current Software Tail Pointer. Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_TPTR_VF3_STAT_SW_TAIL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_TPTR_VF3_STAT_SW_TAIL_SHIFT          0
#define VIRTIO_DMA_CSR_TFI_TPTR_VF3_STAT_SW_TAIL_WIDTH          16
#define VIRTIO_DMA_CSR_TFI_TPTR_VF3_STAT_SW_TAIL_MASK           (0xFFFFU << VIRTIO_DMA_CSR_TFI_TPTR_VF3_STAT_SW_TAIL_SHIFT)

/* Current Hardware Tail Pointer. Resets to 0. */
#define VIRTIO_DMA_CSR_TFI_TPTR_VF3_STAT_HW_TAIL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFI_TPTR_VF3_STAT_HW_TAIL_SHIFT          16
#define VIRTIO_DMA_CSR_TFI_TPTR_VF3_STAT_HW_TAIL_WIDTH          16
#define VIRTIO_DMA_CSR_TFI_TPTR_VF3_STAT_HW_TAIL_MASK           (0xFFFFU << VIRTIO_DMA_CSR_TFI_TPTR_VF3_STAT_HW_TAIL_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Index VF3 Tail Pointer [DEBUG REG] */
typedef union {
  struct {
    uint32_t sw_tail : 16;      /* Current Software Tail Pointer. Resets to 0. */
    uint32_t hw_tail : 16;      /* Current Hardware Tail Pointer. Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFI_TPTR_VF3_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Ring Buffer Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFR_BUFFER_STAT                          0x00032000U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TFR_BUFFER_STAT_CURRENT_LEVEL_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFR_BUFFER_STAT_CURRENT_LEVEL_SHIFT      0
#define VIRTIO_DMA_CSR_TFR_BUFFER_STAT_CURRENT_LEVEL_WIDTH      10
#define VIRTIO_DMA_CSR_TFR_BUFFER_STAT_CURRENT_LEVEL_MASK       (0x3FFU << VIRTIO_DMA_CSR_TFR_BUFFER_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFR_BUFFER_STAT_MAX_ACCESSTYPE           GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFR_BUFFER_STAT_MAX_SHIFT                10
#define VIRTIO_DMA_CSR_TFR_BUFFER_STAT_MAX_WIDTH                10
#define VIRTIO_DMA_CSR_TFR_BUFFER_STAT_MAX_MASK                 (0x3FFU << VIRTIO_DMA_CSR_TFR_BUFFER_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFR_BUFFER_STAT_FULL_ACCESSTYPE          GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFR_BUFFER_STAT_FULL_SHIFT               20
#define VIRTIO_DMA_CSR_TFR_BUFFER_STAT_FULL_WIDTH               1
#define VIRTIO_DMA_CSR_TFR_BUFFER_STAT_FULL_MASK                (0x1U << VIRTIO_DMA_CSR_TFR_BUFFER_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFR_BUFFER_STAT_FIFO_OVERFLOW_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFR_BUFFER_STAT_FIFO_OVERFLOW_SHIFT      21
#define VIRTIO_DMA_CSR_TFR_BUFFER_STAT_FIFO_OVERFLOW_WIDTH      1
#define VIRTIO_DMA_CSR_TFR_BUFFER_STAT_FIFO_OVERFLOW_MASK       (0x1U << VIRTIO_DMA_CSR_TFR_BUFFER_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFR_BUFFER_STAT_RATE_ACCESSTYPE          GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFR_BUFFER_STAT_RATE_SHIFT               22
#define VIRTIO_DMA_CSR_TFR_BUFFER_STAT_RATE_WIDTH               10
#define VIRTIO_DMA_CSR_TFR_BUFFER_STAT_RATE_MASK                (0x3FFU << VIRTIO_DMA_CSR_TFR_BUFFER_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Ring Buffer Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFR_BUFFER_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Ring Statistics Reset */
#define VIRTIO_DMA_CSR_TFR_STATS_RST                            0x00032000U
/* Write 1 to reset all stats registers in this block. */
#define VIRTIO_DMA_CSR_TFR_STATS_RST_RESET_STATS_ACCESSTYPE     GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_TFR_STATS_RST_RESET_STATS_SHIFT          0
#define VIRTIO_DMA_CSR_TFR_STATS_RST_RESET_STATS_WIDTH          1
#define VIRTIO_DMA_CSR_TFR_STATS_RST_RESET_STATS_MASK           (0x1U << VIRTIO_DMA_CSR_TFR_STATS_RST_RESET_STATS_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Ring Statistics Reset */
typedef union {
  struct {
    uint32_t reset_stats : 1;   /* Write 1 to reset all stats registers in this block. */
    uint32_t : 31;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFR_STATS_RST_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Ring Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFR_FIFO_STAT                            0x00032004U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TFR_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFR_FIFO_STAT_CURRENT_LEVEL_SHIFT        0
#define VIRTIO_DMA_CSR_TFR_FIFO_STAT_CURRENT_LEVEL_WIDTH        10
#define VIRTIO_DMA_CSR_TFR_FIFO_STAT_CURRENT_LEVEL_MASK         (0x3FFU << VIRTIO_DMA_CSR_TFR_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFR_FIFO_STAT_MAX_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFR_FIFO_STAT_MAX_SHIFT                  10
#define VIRTIO_DMA_CSR_TFR_FIFO_STAT_MAX_WIDTH                  10
#define VIRTIO_DMA_CSR_TFR_FIFO_STAT_MAX_MASK                   (0x3FFU << VIRTIO_DMA_CSR_TFR_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFR_FIFO_STAT_FULL_ACCESSTYPE            GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFR_FIFO_STAT_FULL_SHIFT                 20
#define VIRTIO_DMA_CSR_TFR_FIFO_STAT_FULL_WIDTH                 1
#define VIRTIO_DMA_CSR_TFR_FIFO_STAT_FULL_MASK                  (0x1U << VIRTIO_DMA_CSR_TFR_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFR_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFR_FIFO_STAT_FIFO_OVERFLOW_SHIFT        21
#define VIRTIO_DMA_CSR_TFR_FIFO_STAT_FIFO_OVERFLOW_WIDTH        1
#define VIRTIO_DMA_CSR_TFR_FIFO_STAT_FIFO_OVERFLOW_MASK         (0x1U << VIRTIO_DMA_CSR_TFR_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFR_FIFO_STAT_RATE_ACCESSTYPE            GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFR_FIFO_STAT_RATE_SHIFT                 22
#define VIRTIO_DMA_CSR_TFR_FIFO_STAT_RATE_WIDTH                 10
#define VIRTIO_DMA_CSR_TFR_FIFO_STAT_RATE_MASK                  (0x3FFU << VIRTIO_DMA_CSR_TFR_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Ring Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFR_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Ring Underflow Count Status */
#define VIRTIO_DMA_CSR_TFR_UFLOW_STAT                           0x00032014U
/* Underflow Count. Resets to 0. */
#define VIRTIO_DMA_CSR_TFR_UFLOW_STAT_UNDER_FLOW_CNT_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFR_UFLOW_STAT_UNDER_FLOW_CNT_SHIFT      0
#define VIRTIO_DMA_CSR_TFR_UFLOW_STAT_UNDER_FLOW_CNT_WIDTH      32
#define VIRTIO_DMA_CSR_TFR_UFLOW_STAT_UNDER_FLOW_CNT_MASK       (0xFFFFFFFFU << VIRTIO_DMA_CSR_TFR_UFLOW_STAT_UNDER_FLOW_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Ring Underflow Count Status */
typedef union {
  struct {
    uint32_t under_flow_cnt : 32;       /* Underflow Count. Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFR_UFLOW_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Ring Overflow Count Status */
#define VIRTIO_DMA_CSR_TFR_OFLOW_STAT                           0x00032018U
/* Overflow Count. Resets to 0. */
#define VIRTIO_DMA_CSR_TFR_OFLOW_STAT_OVER_FLOW_CNT_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFR_OFLOW_STAT_OVER_FLOW_CNT_SHIFT       0
#define VIRTIO_DMA_CSR_TFR_OFLOW_STAT_OVER_FLOW_CNT_WIDTH       32
#define VIRTIO_DMA_CSR_TFR_OFLOW_STAT_OVER_FLOW_CNT_MASK        (0xFFFFFFFFU << VIRTIO_DMA_CSR_TFR_OFLOW_STAT_OVER_FLOW_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Ring Overflow Count Status */
typedef union {
  struct {
    uint32_t over_flow_cnt : 32;        /* Overflow Count. Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFR_OFLOW_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Ring Error Status */
#define VIRTIO_DMA_CSR_TFR_ERROR_STAT                   0x0003201CU
/* Rx Fetch Ring Overflow occurred. Resets to 0. */
#define VIRTIO_DMA_CSR_TFR_ERROR_STAT_O_FLOW_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFR_ERROR_STAT_O_FLOW_SHIFT      0
#define VIRTIO_DMA_CSR_TFR_ERROR_STAT_O_FLOW_WIDTH      1
#define VIRTIO_DMA_CSR_TFR_ERROR_STAT_O_FLOW_MASK       (0x1U << VIRTIO_DMA_CSR_TFR_ERROR_STAT_O_FLOW_SHIFT)

/* Rx Fetch Ring Underflow occurred. Resets to 0. */
#define VIRTIO_DMA_CSR_TFR_ERROR_STAT_U_FLOW_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFR_ERROR_STAT_U_FLOW_SHIFT      1
#define VIRTIO_DMA_CSR_TFR_ERROR_STAT_U_FLOW_WIDTH      1
#define VIRTIO_DMA_CSR_TFR_ERROR_STAT_U_FLOW_MASK       (0x1U << VIRTIO_DMA_CSR_TFR_ERROR_STAT_U_FLOW_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Ring Error Status */
typedef union {
  struct {
    uint32_t o_flow : 1;        /* Rx Fetch Ring Overflow occurred. Resets to 0. */
    uint32_t u_flow : 1;        /* Rx Fetch Ring Underflow occurred. Resets to 0. */
    uint32_t : 30;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFR_ERROR_STAT_t;
#endif /* GENHDR_STRUCT */

/* Ring Command Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT                               0x00034000U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_CURRENT_LEVEL_SHIFT           0
#define VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_CURRENT_LEVEL_WIDTH           10
#define VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_CURRENT_LEVEL_MASK            (0x3FFU << VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_MAX_ACCESSTYPE                GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_MAX_SHIFT                     10
#define VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_MAX_WIDTH                     10
#define VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_MAX_MASK                      (0x3FFU << VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_FULL_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_FULL_SHIFT                    20
#define VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_FULL_WIDTH                    1
#define VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_FULL_MASK                     (0x1U << VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_FIFO_OVERFLOW_SHIFT           21
#define VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_FIFO_OVERFLOW_WIDTH           1
#define VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_FIFO_OVERFLOW_MASK            (0x1U << VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_RATE_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_RATE_SHIFT                    22
#define VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_RATE_WIDTH                    10
#define VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_RATE_MASK                     (0x3FFU << VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Ring Command Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFD_RCMD_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Descriptor Statistics Reset [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFD_STATS_RST                            0x00034000U
/* Write 1 to reset all stats registers in this block. */
#define VIRTIO_DMA_CSR_TFD_STATS_RST_RESET_STATS_ACCESSTYPE     GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_TFD_STATS_RST_RESET_STATS_SHIFT          0
#define VIRTIO_DMA_CSR_TFD_STATS_RST_RESET_STATS_WIDTH          1
#define VIRTIO_DMA_CSR_TFD_STATS_RST_RESET_STATS_MASK           (0x1U << VIRTIO_DMA_CSR_TFD_STATS_RST_RESET_STATS_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Descriptor Statistics Reset [DEBUG REG] */
typedef union {
  struct {
    uint32_t reset_stats : 1;   /* Write 1 to reset all stats registers in this block. */
    uint32_t : 31;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFD_STATS_RST_t;
#endif /* GENHDR_STRUCT */

/* Descriptor Command Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT                               0x00034004U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_CURRENT_LEVEL_SHIFT           0
#define VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_CURRENT_LEVEL_WIDTH           10
#define VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_CURRENT_LEVEL_MASK            (0x3FFU << VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_MAX_ACCESSTYPE                GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_MAX_SHIFT                     10
#define VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_MAX_WIDTH                     10
#define VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_MAX_MASK                      (0x3FFU << VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_FULL_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_FULL_SHIFT                    20
#define VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_FULL_WIDTH                    1
#define VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_FULL_MASK                     (0x1U << VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_FIFO_OVERFLOW_SHIFT           21
#define VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_FIFO_OVERFLOW_WIDTH           1
#define VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_FIFO_OVERFLOW_MASK            (0x1U << VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_RATE_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_RATE_SHIFT                    22
#define VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_RATE_WIDTH                    10
#define VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_RATE_MASK                     (0x3FFU << VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Descriptor Command Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFD_DCMD_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx fetch Descriptor Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFD_FIFO_STAT                            0x00034008U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TFD_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFD_FIFO_STAT_CURRENT_LEVEL_SHIFT        0
#define VIRTIO_DMA_CSR_TFD_FIFO_STAT_CURRENT_LEVEL_WIDTH        10
#define VIRTIO_DMA_CSR_TFD_FIFO_STAT_CURRENT_LEVEL_MASK         (0x3FFU << VIRTIO_DMA_CSR_TFD_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFD_FIFO_STAT_MAX_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFD_FIFO_STAT_MAX_SHIFT                  10
#define VIRTIO_DMA_CSR_TFD_FIFO_STAT_MAX_WIDTH                  10
#define VIRTIO_DMA_CSR_TFD_FIFO_STAT_MAX_MASK                   (0x3FFU << VIRTIO_DMA_CSR_TFD_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFD_FIFO_STAT_FULL_ACCESSTYPE            GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFD_FIFO_STAT_FULL_SHIFT                 20
#define VIRTIO_DMA_CSR_TFD_FIFO_STAT_FULL_WIDTH                 1
#define VIRTIO_DMA_CSR_TFD_FIFO_STAT_FULL_MASK                  (0x1U << VIRTIO_DMA_CSR_TFD_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFD_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFD_FIFO_STAT_FIFO_OVERFLOW_SHIFT        21
#define VIRTIO_DMA_CSR_TFD_FIFO_STAT_FIFO_OVERFLOW_WIDTH        1
#define VIRTIO_DMA_CSR_TFD_FIFO_STAT_FIFO_OVERFLOW_MASK         (0x1U << VIRTIO_DMA_CSR_TFD_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFD_FIFO_STAT_RATE_ACCESSTYPE            GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFD_FIFO_STAT_RATE_SHIFT                 22
#define VIRTIO_DMA_CSR_TFD_FIFO_STAT_RATE_WIDTH                 10
#define VIRTIO_DMA_CSR_TFD_FIFO_STAT_RATE_MASK                  (0x3FFU << VIRTIO_DMA_CSR_TFD_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx fetch Descriptor Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFD_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx fetch Descriptor Free DB Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT                             0x0003400CU
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_CURRENT_LEVEL_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_CURRENT_LEVEL_SHIFT         0
#define VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_CURRENT_LEVEL_WIDTH         10
#define VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_CURRENT_LEVEL_MASK          (0x3FFU << VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Min fill level (latched). Resets to 10'h3ff. */
#define VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_MIN_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_MIN_SHIFT                   10
#define VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_MIN_WIDTH                   10
#define VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_MIN_MASK                    (0x3FFU << VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_MIN_SHIFT)

/* Fifo has been empty, or nearly empty (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_EMPTY_ACCESSTYPE            GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_EMPTY_SHIFT                 20
#define VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_EMPTY_WIDTH                 1
#define VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_EMPTY_MASK                  (0x1U << VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_EMPTY_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_FIFO_OVERFLOW_SHIFT         21
#define VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_FIFO_OVERFLOW_WIDTH         1
#define VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_FIFO_OVERFLOW_MASK          (0x1U << VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_RATE_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_RATE_SHIFT                  22
#define VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_RATE_WIDTH                  10
#define VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_RATE_MASK                   (0x3FFU << VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx fetch Descriptor Free DB Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t min : 10;                  /* Fifo Min fill level (latched). Resets to 10'h3ff. */
    uint32_t empty : 1;                 /* Fifo has been empty, or nearly empty (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFD_FREE_DBFIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Packet Descriptor Command Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT                                0x00036000U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_CURRENT_LEVEL_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_CURRENT_LEVEL_SHIFT            0
#define VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_CURRENT_LEVEL_WIDTH            10
#define VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_CURRENT_LEVEL_MASK             (0x3FFU << VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_MAX_ACCESSTYPE                 GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_MAX_SHIFT                      10
#define VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_MAX_WIDTH                      10
#define VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_MAX_MASK                       (0x3FFU << VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_FULL_ACCESSTYPE                GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_FULL_SHIFT                     20
#define VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_FULL_WIDTH                     1
#define VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_FULL_MASK                      (0x1U << VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_FIFO_OVERFLOW_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_FIFO_OVERFLOW_SHIFT            21
#define VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_FIFO_OVERFLOW_WIDTH            1
#define VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_FIFO_OVERFLOW_MASK             (0x1U << VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_RATE_ACCESSTYPE                GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_RATE_SHIFT                     22
#define VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_RATE_WIDTH                     10
#define VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_RATE_MASK                      (0x3FFU << VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Packet Descriptor Command Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFP_DESC_CMD_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Packet Statistics Reset */
#define VIRTIO_DMA_CSR_TFP_STATS_RST                            0x00036000U
/* Write 1 to reset all stats registers in this block. */
#define VIRTIO_DMA_CSR_TFP_STATS_RST_RESET_STATS_ACCESSTYPE     GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_TFP_STATS_RST_RESET_STATS_SHIFT          0
#define VIRTIO_DMA_CSR_TFP_STATS_RST_RESET_STATS_WIDTH          1
#define VIRTIO_DMA_CSR_TFP_STATS_RST_RESET_STATS_MASK           (0x1U << VIRTIO_DMA_CSR_TFP_STATS_RST_RESET_STATS_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Packet Statistics Reset */
typedef union {
  struct {
    uint32_t reset_stats : 1;   /* Write 1 to reset all stats registers in this block. */
    uint32_t : 31;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFP_STATS_RST_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Packet Descriptor Info Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT                               0x00036004U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_CURRENT_LEVEL_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_CURRENT_LEVEL_SHIFT           0
#define VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_CURRENT_LEVEL_WIDTH           10
#define VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_CURRENT_LEVEL_MASK            (0x3FFU << VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_MAX_ACCESSTYPE                GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_MAX_SHIFT                     10
#define VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_MAX_WIDTH                     10
#define VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_MAX_MASK                      (0x3FFU << VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_FULL_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_FULL_SHIFT                    20
#define VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_FULL_WIDTH                    1
#define VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_FULL_MASK                     (0x1U << VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_FIFO_OVERFLOW_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_FIFO_OVERFLOW_SHIFT           21
#define VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_FIFO_OVERFLOW_WIDTH           1
#define VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_FIFO_OVERFLOW_MASK            (0x1U << VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_RATE_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_RATE_SHIFT                    22
#define VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_RATE_WIDTH                    10
#define VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_RATE_MASK                     (0x3FFU << VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Packet Descriptor Info Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFP_DESC_INFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Packet Test Mode [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFP_TEST_MODE                            0x00036004U
/*
 * Write 1 to enable Virtio 0.9.5 mode. This allows 0.9.5 support to be enabled
 * without having to negotiate features. Global setting. Test mode only.
 */
#define VIRTIO_DMA_CSR_TFP_TEST_MODE_V_95_TESTMODE_ACCESSTYPE   GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_TFP_TEST_MODE_V_95_TESTMODE_SHIFT        0
#define VIRTIO_DMA_CSR_TFP_TEST_MODE_V_95_TESTMODE_WIDTH        1
#define VIRTIO_DMA_CSR_TFP_TEST_MODE_V_95_TESTMODE_MASK         (0x1U << VIRTIO_DMA_CSR_TFP_TEST_MODE_V_95_TESTMODE_SHIFT)

/*
 * Write 1 to enable Virtio 0.9.5 notify suppression feature in Virtio 1.0 mode.
 * This means that during used ring updates only IDX values are updated and flag
 * values are not written. Global setting. Test mode only.
 */
#define VIRTIO_DMA_CSR_TFP_TEST_MODE_V_10_FLAG_SUPR_ACCESSTYPE  GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_TFP_TEST_MODE_V_10_FLAG_SUPR_SHIFT       1
#define VIRTIO_DMA_CSR_TFP_TEST_MODE_V_10_FLAG_SUPR_WIDTH       1
#define VIRTIO_DMA_CSR_TFP_TEST_MODE_V_10_FLAG_SUPR_MASK        (0x1U << VIRTIO_DMA_CSR_TFP_TEST_MODE_V_10_FLAG_SUPR_SHIFT)

/*
 * Write 1 to disable Virtio 0.9.5 notify suppression feature. By default all
 * 0.9.5 rings will not update Flags during used ring updates. Setting this
 * value to 1, Flags and IDX will be written by the DMA. Global setting. Test
 * mode only.
 */
#define VIRTIO_DMA_CSR_TFP_TEST_MODE_V_95_SUPR_DIS_ACCESSTYPE   GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_TFP_TEST_MODE_V_95_SUPR_DIS_SHIFT        2
#define VIRTIO_DMA_CSR_TFP_TEST_MODE_V_95_SUPR_DIS_WIDTH        1
#define VIRTIO_DMA_CSR_TFP_TEST_MODE_V_95_SUPR_DIS_MASK         (0x1U << VIRTIO_DMA_CSR_TFP_TEST_MODE_V_95_SUPR_DIS_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Packet Test Mode [DEBUG REG] */
typedef union {
  struct {
    uint32_t v_95_testmode : 1;         /* Write 1 to enable Virtio 0.9.5 mode. This allows 0.9.5 support to be enabled
                                           without having to negotiate features. Global setting. Test mode only. */
    uint32_t v_10_flag_supr : 1;        /* Write 1 to enable Virtio 0.9.5 notify suppression feature in Virtio 1.0 mode.
                                           This means that during used ring updates only IDX values are updated and flag
                                           values are not written. Global setting. Test mode only. */
    uint32_t v_95_supr_dis : 1;         /* Write 1 to disable Virtio 0.9.5 notify suppression feature. By default all
                                           0.9.5 rings will not update Flags during used ring updates. Setting this
                                           value to 1, Flags and IDX will be written by the DMA. Global setting. Test
                                           mode only. */
    uint32_t : 29;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFP_TEST_MODE_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Packet Virtqueue Packet Response Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT                              0x0003600CU
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_CURRENT_LEVEL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_CURRENT_LEVEL_SHIFT          0
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_CURRENT_LEVEL_WIDTH          10
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_CURRENT_LEVEL_MASK           (0x3FFU << VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_MAX_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_MAX_SHIFT                    10
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_MAX_WIDTH                    10
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_MAX_MASK                     (0x3FFU << VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_FULL_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_FULL_SHIFT                   20
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_FULL_WIDTH                   1
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_FULL_MASK                    (0x1U << VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_FIFO_OVERFLOW_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_FIFO_OVERFLOW_SHIFT          21
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_FIFO_OVERFLOW_WIDTH          1
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_FIFO_OVERFLOW_MASK           (0x1U << VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_RATE_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_RATE_SHIFT                   22
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_RATE_WIDTH                   10
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_RATE_MASK                    (0x3FFU << VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Packet Virtqueue Packet Response Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFP_VQ_PKT_RSP_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Packet Virtqueue Packet Head Info Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT                             0x00036010U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_CURRENT_LEVEL_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_CURRENT_LEVEL_SHIFT         0
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_CURRENT_LEVEL_WIDTH         10
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_CURRENT_LEVEL_MASK          (0x3FFU << VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_MAX_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_MAX_SHIFT                   10
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_MAX_WIDTH                   10
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_MAX_MASK                    (0x3FFU << VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_FULL_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_FULL_SHIFT                  20
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_FULL_WIDTH                  1
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_FULL_MASK                   (0x1U << VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_FIFO_OVERFLOW_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_FIFO_OVERFLOW_SHIFT         21
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_FIFO_OVERFLOW_WIDTH         1
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_FIFO_OVERFLOW_MASK          (0x1U << VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_RATE_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_RATE_SHIFT                  22
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_RATE_WIDTH                  10
#define VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_RATE_MASK                   (0x3FFU << VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Packet Virtqueue Packet Head Info Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFP_VQ_PKT_HEAD_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Packet Comb Pkt Head Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT                              0x00036028U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_CURRENT_LEVEL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_CURRENT_LEVEL_SHIFT          0
#define VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_CURRENT_LEVEL_WIDTH          10
#define VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_CURRENT_LEVEL_MASK           (0x3FFU << VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_MAX_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_MAX_SHIFT                    10
#define VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_MAX_WIDTH                    10
#define VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_MAX_MASK                     (0x3FFU << VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_FULL_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_FULL_SHIFT                   20
#define VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_FULL_WIDTH                   1
#define VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_FULL_MASK                    (0x1U << VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_FIFO_OVERFLOW_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_FIFO_OVERFLOW_SHIFT          21
#define VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_FIFO_OVERFLOW_WIDTH          1
#define VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_FIFO_OVERFLOW_MASK           (0x1U << VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_RATE_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_RATE_SHIFT                   22
#define VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_RATE_WIDTH                   10
#define VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_RATE_MASK                    (0x3FFU << VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Packet Comb Pkt Head Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFP_COMB_PKT_H_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Packet Comb Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFP_COMB_STAT                            0x0003602CU
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_COMB_STAT_CURRENT_LEVEL_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_COMB_STAT_CURRENT_LEVEL_SHIFT        0
#define VIRTIO_DMA_CSR_TFP_COMB_STAT_CURRENT_LEVEL_WIDTH        10
#define VIRTIO_DMA_CSR_TFP_COMB_STAT_CURRENT_LEVEL_MASK         (0x3FFU << VIRTIO_DMA_CSR_TFP_COMB_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_COMB_STAT_MAX_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_COMB_STAT_MAX_SHIFT                  10
#define VIRTIO_DMA_CSR_TFP_COMB_STAT_MAX_WIDTH                  10
#define VIRTIO_DMA_CSR_TFP_COMB_STAT_MAX_MASK                   (0x3FFU << VIRTIO_DMA_CSR_TFP_COMB_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_COMB_STAT_FULL_ACCESSTYPE            GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_COMB_STAT_FULL_SHIFT                 20
#define VIRTIO_DMA_CSR_TFP_COMB_STAT_FULL_WIDTH                 1
#define VIRTIO_DMA_CSR_TFP_COMB_STAT_FULL_MASK                  (0x1U << VIRTIO_DMA_CSR_TFP_COMB_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_COMB_STAT_FIFO_OVERFLOW_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_COMB_STAT_FIFO_OVERFLOW_SHIFT        21
#define VIRTIO_DMA_CSR_TFP_COMB_STAT_FIFO_OVERFLOW_WIDTH        1
#define VIRTIO_DMA_CSR_TFP_COMB_STAT_FIFO_OVERFLOW_MASK         (0x1U << VIRTIO_DMA_CSR_TFP_COMB_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_COMB_STAT_RATE_ACCESSTYPE            GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_COMB_STAT_RATE_SHIFT                 22
#define VIRTIO_DMA_CSR_TFP_COMB_STAT_RATE_WIDTH                 10
#define VIRTIO_DMA_CSR_TFP_COMB_STAT_RATE_MASK                  (0x3FFU << VIRTIO_DMA_CSR_TFP_COMB_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Packet Comb Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFP_COMB_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Packet Egress Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFP_EGRESS_STAT                          0x00036030U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_EGRESS_STAT_CURRENT_LEVEL_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_EGRESS_STAT_CURRENT_LEVEL_SHIFT      0
#define VIRTIO_DMA_CSR_TFP_EGRESS_STAT_CURRENT_LEVEL_WIDTH      10
#define VIRTIO_DMA_CSR_TFP_EGRESS_STAT_CURRENT_LEVEL_MASK       (0x3FFU << VIRTIO_DMA_CSR_TFP_EGRESS_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_EGRESS_STAT_MAX_ACCESSTYPE           GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_EGRESS_STAT_MAX_SHIFT                10
#define VIRTIO_DMA_CSR_TFP_EGRESS_STAT_MAX_WIDTH                10
#define VIRTIO_DMA_CSR_TFP_EGRESS_STAT_MAX_MASK                 (0x3FFU << VIRTIO_DMA_CSR_TFP_EGRESS_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_EGRESS_STAT_FULL_ACCESSTYPE          GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_EGRESS_STAT_FULL_SHIFT               20
#define VIRTIO_DMA_CSR_TFP_EGRESS_STAT_FULL_WIDTH               1
#define VIRTIO_DMA_CSR_TFP_EGRESS_STAT_FULL_MASK                (0x1U << VIRTIO_DMA_CSR_TFP_EGRESS_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_EGRESS_STAT_FIFO_OVERFLOW_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_EGRESS_STAT_FIFO_OVERFLOW_SHIFT      21
#define VIRTIO_DMA_CSR_TFP_EGRESS_STAT_FIFO_OVERFLOW_WIDTH      1
#define VIRTIO_DMA_CSR_TFP_EGRESS_STAT_FIFO_OVERFLOW_MASK       (0x1U << VIRTIO_DMA_CSR_TFP_EGRESS_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_EGRESS_STAT_RATE_ACCESSTYPE          GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_EGRESS_STAT_RATE_SHIFT               22
#define VIRTIO_DMA_CSR_TFP_EGRESS_STAT_RATE_WIDTH               10
#define VIRTIO_DMA_CSR_TFP_EGRESS_STAT_RATE_MASK                (0x3FFU << VIRTIO_DMA_CSR_TFP_EGRESS_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Packet Egress Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFP_EGRESS_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Packet Virtq Update Req Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT                              0x00036034U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_CURRENT_LEVEL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_CURRENT_LEVEL_SHIFT          0
#define VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_CURRENT_LEVEL_WIDTH          10
#define VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_CURRENT_LEVEL_MASK           (0x3FFU << VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_MAX_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_MAX_SHIFT                    10
#define VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_MAX_WIDTH                    10
#define VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_MAX_MASK                     (0x3FFU << VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_FULL_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_FULL_SHIFT                   20
#define VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_FULL_WIDTH                   1
#define VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_FULL_MASK                    (0x1U << VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_FIFO_OVERFLOW_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_FIFO_OVERFLOW_SHIFT          21
#define VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_FIFO_OVERFLOW_WIDTH          1
#define VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_FIFO_OVERFLOW_MASK           (0x1U << VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_RATE_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_RATE_SHIFT                   22
#define VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_RATE_WIDTH                   10
#define VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_RATE_MASK                    (0x3FFU << VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Packet Virtq Update Req Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFP_VQ_UPD_REQ_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Packet Update Cmd Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT                              0x00036038U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_CURRENT_LEVEL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_CURRENT_LEVEL_SHIFT          0
#define VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_CURRENT_LEVEL_WIDTH          10
#define VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_CURRENT_LEVEL_MASK           (0x3FFU << VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_MAX_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_MAX_SHIFT                    10
#define VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_MAX_WIDTH                    10
#define VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_MAX_MASK                     (0x3FFU << VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_FULL_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_FULL_SHIFT                   20
#define VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_FULL_WIDTH                   1
#define VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_FULL_MASK                    (0x1U << VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_FIFO_OVERFLOW_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_FIFO_OVERFLOW_SHIFT          21
#define VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_FIFO_OVERFLOW_WIDTH          1
#define VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_FIFO_OVERFLOW_MASK           (0x1U << VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_RATE_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_RATE_SHIFT                   22
#define VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_RATE_WIDTH                   10
#define VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_RATE_MASK                    (0x3FFU << VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Packet Update Cmd Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFP_UPDATE_CMD_STAT_t;
#endif /* GENHDR_STRUCT */

/* Tx Fetch Packet Error Status */
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT                                   0x0003603CU
/* Virtqueue Tx Packet Response Fifo full. Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_VQ_TX_PKT_RSP_FULL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_VQ_TX_PKT_RSP_FULL_SHIFT          0
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_VQ_TX_PKT_RSP_FULL_WIDTH          1
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_VQ_TX_PKT_RSP_FULL_MASK           (0x1U << VIRTIO_DMA_CSR_TFP_ERROR_STAT_VQ_TX_PKT_RSP_FULL_SHIFT)

/*
 * Virtqueue Tx Packet Head Fifo QID did not match QID for the descriptor being
 * processed. Resets to 0.
 */
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_VQ_TX_PKT_HEAD_QID_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_VQ_TX_PKT_HEAD_QID_SHIFT          6
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_VQ_TX_PKT_HEAD_QID_WIDTH          1
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_VQ_TX_PKT_HEAD_QID_MASK           (0x1U << VIRTIO_DMA_CSR_TFP_ERROR_STAT_VQ_TX_PKT_HEAD_QID_SHIFT)

/*
 * Virtqueue Tx Packet Head Fifo Packet length did not match expected length
 * from the descriptor being processed. Resets to 0.
 */
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_VQ_TX_PKT_HEAD_LEN_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_VQ_TX_PKT_HEAD_LEN_SHIFT          7
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_VQ_TX_PKT_HEAD_LEN_WIDTH          1
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_VQ_TX_PKT_HEAD_LEN_MASK           (0x1U << VIRTIO_DMA_CSR_TFP_ERROR_STAT_VQ_TX_PKT_HEAD_LEN_SHIFT)

/* Error indication from pkt_comb module. Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_PKT_COMB_ERR_ACCESSTYPE           GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_PKT_COMB_ERR_SHIFT                8
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_PKT_COMB_ERR_WIDTH                2
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_PKT_COMB_ERR_MASK                 (0x3U << VIRTIO_DMA_CSR_TFP_ERROR_STAT_PKT_COMB_ERR_SHIFT)

/* Used Ring Update error. Resets to 0. */
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_USED_RING_UPD_ERR_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_USED_RING_UPD_ERR_SHIFT           10
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_USED_RING_UPD_ERR_WIDTH           1
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_USED_RING_UPD_ERR_MASK            (0x1U << VIRTIO_DMA_CSR_TFP_ERROR_STAT_USED_RING_UPD_ERR_SHIFT)

/*
 * Packet Integrity Errors. Resets to 0.
 * Bit 11 indicates Missing EOP. 
 * Bit 12 Indicates Missing SOP. 
 * Bit 13 indicates Unexpected QID.
 */
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_PKT_INTEGRITY_ACCESSTYPE          GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_PKT_INTEGRITY_SHIFT               11
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_PKT_INTEGRITY_WIDTH               3
#define VIRTIO_DMA_CSR_TFP_ERROR_STAT_PKT_INTEGRITY_MASK                (0x7U << VIRTIO_DMA_CSR_TFP_ERROR_STAT_PKT_INTEGRITY_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx Fetch Packet Error Status */
typedef union {
  struct {
    uint32_t vq_tx_pkt_rsp_full : 1;    /* Virtqueue Tx Packet Response Fifo full. Resets to 0. */
    uint32_t : 5;
    uint32_t vq_tx_pkt_head_qid : 1;    /* Virtqueue Tx Packet Head Fifo QID did not match QID for the descriptor being
                                           processed. Resets to 0. */
    uint32_t vq_tx_pkt_head_len : 1;    /* Virtqueue Tx Packet Head Fifo Packet length did not match expected length
                                           from the descriptor being processed. Resets to 0. */
    uint32_t pkt_comb_err : 2;          /* Error indication from pkt_comb module. Resets to 0. */
    uint32_t used_ring_upd_err : 1;     /* Used Ring Update error. Resets to 0. */
    uint32_t pkt_integrity : 3;         /* Packet Integrity Errors. Resets to 0.
                                           Bit 11 indicates Missing EOP.
                                           Bit 12 Indicates Missing SOP.
                                           Bit 13 indicates Unexpected QID. */
    uint32_t : 18;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_TFP_ERROR_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Fetch Index Multibuffer VF0 Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT                             0x00038000U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_CURRENT_LEVEL_SHIFT         0
#define VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_CURRENT_LEVEL_WIDTH         10
#define VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_CURRENT_LEVEL_MASK          (0x3FFU << VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_MAX_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_MAX_SHIFT                   10
#define VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_MAX_WIDTH                   10
#define VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_MAX_MASK                    (0x3FFU << VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_FULL_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_FULL_SHIFT                  20
#define VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_FULL_WIDTH                  1
#define VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_FULL_MASK                   (0x1U << VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_FIFO_OVERFLOW_SHIFT         21
#define VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_FIFO_OVERFLOW_WIDTH         1
#define VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_FIFO_OVERFLOW_MASK          (0x1U << VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_RATE_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_RATE_SHIFT                  22
#define VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_RATE_WIDTH                  10
#define VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_RATE_MASK                   (0x3FFU << VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Fetch Index Multibuffer VF0 Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFI_MB_VF0_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Fetch Index Statistics Reset [DEBUG REG] */
#define VIRTIO_DMA_CSR_RFI_STATS_RST                            0x00038000U
/* Write 1 to reset all stats registers in this block. */
#define VIRTIO_DMA_CSR_RFI_STATS_RST_RESET_STATS_ACCESSTYPE     GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_RFI_STATS_RST_RESET_STATS_SHIFT          0
#define VIRTIO_DMA_CSR_RFI_STATS_RST_RESET_STATS_WIDTH          1
#define VIRTIO_DMA_CSR_RFI_STATS_RST_RESET_STATS_MASK           (0x1U << VIRTIO_DMA_CSR_RFI_STATS_RST_RESET_STATS_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Fetch Index Statistics Reset [DEBUG REG] */
typedef union {
  struct {
    uint32_t reset_stats : 1;   /* Write 1 to reset all stats registers in this block. */
    uint32_t : 31;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFI_STATS_RST_t;
#endif /* GENHDR_STRUCT */

/* Rx Fetch Index Multibuffer VF1 Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT                             0x00038004U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_CURRENT_LEVEL_SHIFT         0
#define VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_CURRENT_LEVEL_WIDTH         10
#define VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_CURRENT_LEVEL_MASK          (0x3FFU << VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_MAX_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_MAX_SHIFT                   10
#define VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_MAX_WIDTH                   10
#define VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_MAX_MASK                    (0x3FFU << VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_FULL_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_FULL_SHIFT                  20
#define VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_FULL_WIDTH                  1
#define VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_FULL_MASK                   (0x1U << VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_FIFO_OVERFLOW_SHIFT         21
#define VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_FIFO_OVERFLOW_WIDTH         1
#define VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_FIFO_OVERFLOW_MASK          (0x1U << VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_RATE_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_RATE_SHIFT                  22
#define VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_RATE_WIDTH                  10
#define VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_RATE_MASK                   (0x3FFU << VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Fetch Index Multibuffer VF1 Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFI_MB_VF1_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Fetch Index Interrupt Suppression */
#define VIRTIO_DMA_CSR_RFI_IRQ_SUPPRESS                                 0x00038004U
/* Disable Interrupt Suppression when set. */
#define VIRTIO_DMA_CSR_RFI_IRQ_SUPPRESS_IRQ_SUPR_DISABLE_ACCESSTYPE     GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_RFI_IRQ_SUPPRESS_IRQ_SUPR_DISABLE_SHIFT          0
#define VIRTIO_DMA_CSR_RFI_IRQ_SUPPRESS_IRQ_SUPR_DISABLE_WIDTH          1
#define VIRTIO_DMA_CSR_RFI_IRQ_SUPPRESS_IRQ_SUPR_DISABLE_MASK           (0x1U << VIRTIO_DMA_CSR_RFI_IRQ_SUPPRESS_IRQ_SUPR_DISABLE_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Fetch Index Interrupt Suppression */
typedef union {
  struct {
    uint32_t irq_supr_disable : 1;      /* Disable Interrupt Suppression when set. */
    uint32_t : 31;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFI_IRQ_SUPPRESS_t;
#endif /* GENHDR_STRUCT */

/* Rx Fetch Index Multibuffer VF2 Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT                             0x00038008U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_CURRENT_LEVEL_SHIFT         0
#define VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_CURRENT_LEVEL_WIDTH         10
#define VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_CURRENT_LEVEL_MASK          (0x3FFU << VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_MAX_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_MAX_SHIFT                   10
#define VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_MAX_WIDTH                   10
#define VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_MAX_MASK                    (0x3FFU << VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_FULL_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_FULL_SHIFT                  20
#define VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_FULL_WIDTH                  1
#define VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_FULL_MASK                   (0x1U << VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_FIFO_OVERFLOW_SHIFT         21
#define VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_FIFO_OVERFLOW_WIDTH         1
#define VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_FIFO_OVERFLOW_MASK          (0x1U << VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_RATE_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_RATE_SHIFT                  22
#define VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_RATE_WIDTH                  10
#define VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_RATE_MASK                   (0x3FFU << VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Fetch Index Multibuffer VF2 Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFI_MB_VF2_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Fetch Index Notify Kick [DEBUG REG] */
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK                          0x00038008U
/* Notify Kick for a QID. */
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_NOTIFY_ENABLE_ACCESSTYPE GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_NOTIFY_ENABLE_SHIFT      0
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_NOTIFY_ENABLE_WIDTH      1
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_NOTIFY_ENABLE_MASK       (0x1U << VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_NOTIFY_ENABLE_SHIFT)

/* Reserved. */
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_RSVD_0_ACCESSTYPE        GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_RSVD_0_SHIFT             1
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_RSVD_0_WIDTH             3
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_RSVD_0_MASK              (0x7U << VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_RSVD_0_SHIFT)

/* QID number to be notified. */
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_QID_ACCESSTYPE           GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_QID_SHIFT                4
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_QID_WIDTH                10
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_QID_MASK                 (0x3FFU << VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_QID_SHIFT)

/* Reserved. */
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_RSVD_1_ACCESSTYPE        GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_RSVD_1_SHIFT             14
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_RSVD_1_WIDTH             2
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_RSVD_1_MASK              (0x3U << VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_RSVD_1_SHIFT)

/* Polling Rate (cycles). */
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_POLLING_RATE_ACCESSTYPE  GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_POLLING_RATE_SHIFT       16
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_POLLING_RATE_WIDTH       12
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_POLLING_RATE_MASK        (0xFFFU << VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_POLLING_RATE_SHIFT)

/* Reserved. */
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_RSVD_2_ACCESSTYPE        GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_RSVD_2_SHIFT             28
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_RSVD_2_WIDTH             3
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_RSVD_2_MASK              (0x7U << VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_RSVD_2_SHIFT)

/* Enable h/w AvailIndex Polling. */
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_POLLING_ENABLE_ACCESSTYPE GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_POLLING_ENABLE_SHIFT     31
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_POLLING_ENABLE_WIDTH     1
#define VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_POLLING_ENABLE_MASK      (0x1U << VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_POLLING_ENABLE_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Fetch Index Notify Kick [DEBUG REG] */
typedef union {
  struct {
    uint32_t notify_enable : 1;         /* Notify Kick for a QID. */
    uint32_t rsvd_0 : 3;                /* Reserved. */
    uint32_t qid : 10;                  /* QID number to be notified. */
    uint32_t rsvd_1 : 2;                /* Reserved. */
    uint32_t polling_rate : 12;         /* Polling Rate (cycles). */
    uint32_t rsvd_2 : 3;                /* Reserved. */
    uint32_t polling_enable : 1;        /* Enable h/w AvailIndex Polling. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFI_NOTIFY_KICK_t;
#endif /* GENHDR_STRUCT */

/* Rx Fetch Index Multibuffer VF3 Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT                             0x0003800CU
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_CURRENT_LEVEL_SHIFT         0
#define VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_CURRENT_LEVEL_WIDTH         10
#define VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_CURRENT_LEVEL_MASK          (0x3FFU << VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_MAX_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_MAX_SHIFT                   10
#define VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_MAX_WIDTH                   10
#define VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_MAX_MASK                    (0x3FFU << VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_FULL_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_FULL_SHIFT                  20
#define VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_FULL_WIDTH                  1
#define VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_FULL_MASK                   (0x1U << VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_FIFO_OVERFLOW_SHIFT         21
#define VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_FIFO_OVERFLOW_WIDTH         1
#define VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_FIFO_OVERFLOW_MASK          (0x1U << VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_RATE_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_RATE_SHIFT                  22
#define VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_RATE_WIDTH                  10
#define VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_RATE_MASK                   (0x3FFU << VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Fetch Index Multibuffer VF3 Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFI_MB_VF3_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Fetch Index Notify Suppression */
#define VIRTIO_DMA_CSR_RFI_NFY_SUPPRESS                                 0x0003800CU
/* Disable Notify Suppression when set. */
#define VIRTIO_DMA_CSR_RFI_NFY_SUPPRESS_NFY_SUPR_DISABLE_ACCESSTYPE     GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_RFI_NFY_SUPPRESS_NFY_SUPR_DISABLE_SHIFT          0
#define VIRTIO_DMA_CSR_RFI_NFY_SUPPRESS_NFY_SUPR_DISABLE_WIDTH          1
#define VIRTIO_DMA_CSR_RFI_NFY_SUPPRESS_NFY_SUPR_DISABLE_MASK           (0x1U << VIRTIO_DMA_CSR_RFI_NFY_SUPPRESS_NFY_SUPR_DISABLE_SHIFT)

/* Force Notify Suppression when set. */
#define VIRTIO_DMA_CSR_RFI_NFY_SUPPRESS_FORCE_NFY_SUPR_ACCESSTYPE       GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_RFI_NFY_SUPPRESS_FORCE_NFY_SUPR_SHIFT            1
#define VIRTIO_DMA_CSR_RFI_NFY_SUPPRESS_FORCE_NFY_SUPR_WIDTH            1
#define VIRTIO_DMA_CSR_RFI_NFY_SUPPRESS_FORCE_NFY_SUPR_MASK             (0x1U << VIRTIO_DMA_CSR_RFI_NFY_SUPPRESS_FORCE_NFY_SUPR_SHIFT)

/* Notify Threshold. Functionality TBC. */
#define VIRTIO_DMA_CSR_RFI_NFY_SUPPRESS_NFY_THRESHOLD_ACCESSTYPE        GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_RFI_NFY_SUPPRESS_NFY_THRESHOLD_SHIFT             5
#define VIRTIO_DMA_CSR_RFI_NFY_SUPPRESS_NFY_THRESHOLD_WIDTH             16
#define VIRTIO_DMA_CSR_RFI_NFY_SUPPRESS_NFY_THRESHOLD_MASK              (0xFFFFU << VIRTIO_DMA_CSR_RFI_NFY_SUPPRESS_NFY_THRESHOLD_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Fetch Index Notify Suppression */
typedef union {
  struct {
    uint32_t nfy_supr_disable : 1;      /* Disable Notify Suppression when set. */
    uint32_t force_nfy_supr : 1;        /* Force Notify Suppression when set. */
    uint32_t : 3;
    uint32_t nfy_threshold : 16;        /* Notify Threshold. Functionality TBC. */
    uint32_t : 11;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFI_NFY_SUPPRESS_t;
#endif /* GENHDR_STRUCT */

/* Rx Fetch Index VF0 Tail Pointer [DEBUG REG] */
#define VIRTIO_DMA_CSR_RFI_TPTR_VF0_STAT                        0x00038010U
/* Current Software Tail Pointer. Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_TPTR_VF0_STAT_SW_TAIL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_TPTR_VF0_STAT_SW_TAIL_SHIFT          0
#define VIRTIO_DMA_CSR_RFI_TPTR_VF0_STAT_SW_TAIL_WIDTH          16
#define VIRTIO_DMA_CSR_RFI_TPTR_VF0_STAT_SW_TAIL_MASK           (0xFFFFU << VIRTIO_DMA_CSR_RFI_TPTR_VF0_STAT_SW_TAIL_SHIFT)

/* Current Hardware Tail Pointer. Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_TPTR_VF0_STAT_HW_TAIL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_TPTR_VF0_STAT_HW_TAIL_SHIFT          16
#define VIRTIO_DMA_CSR_RFI_TPTR_VF0_STAT_HW_TAIL_WIDTH          16
#define VIRTIO_DMA_CSR_RFI_TPTR_VF0_STAT_HW_TAIL_MASK           (0xFFFFU << VIRTIO_DMA_CSR_RFI_TPTR_VF0_STAT_HW_TAIL_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Fetch Index VF0 Tail Pointer [DEBUG REG] */
typedef union {
  struct {
    uint32_t sw_tail : 16;      /* Current Software Tail Pointer. Resets to 0. */
    uint32_t hw_tail : 16;      /* Current Hardware Tail Pointer. Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFI_TPTR_VF0_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Fetch Index VF1 Tail Pointer [DEBUG REG] */
#define VIRTIO_DMA_CSR_RFI_TPTR_VF1_STAT                        0x00038014U
/* Current Software Tail Pointer. Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_TPTR_VF1_STAT_SW_TAIL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_TPTR_VF1_STAT_SW_TAIL_SHIFT          0
#define VIRTIO_DMA_CSR_RFI_TPTR_VF1_STAT_SW_TAIL_WIDTH          16
#define VIRTIO_DMA_CSR_RFI_TPTR_VF1_STAT_SW_TAIL_MASK           (0xFFFFU << VIRTIO_DMA_CSR_RFI_TPTR_VF1_STAT_SW_TAIL_SHIFT)

/* Current Hardware Tail Pointer. Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_TPTR_VF1_STAT_HW_TAIL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_TPTR_VF1_STAT_HW_TAIL_SHIFT          16
#define VIRTIO_DMA_CSR_RFI_TPTR_VF1_STAT_HW_TAIL_WIDTH          16
#define VIRTIO_DMA_CSR_RFI_TPTR_VF1_STAT_HW_TAIL_MASK           (0xFFFFU << VIRTIO_DMA_CSR_RFI_TPTR_VF1_STAT_HW_TAIL_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Fetch Index VF1 Tail Pointer [DEBUG REG] */
typedef union {
  struct {
    uint32_t sw_tail : 16;      /* Current Software Tail Pointer. Resets to 0. */
    uint32_t hw_tail : 16;      /* Current Hardware Tail Pointer. Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFI_TPTR_VF1_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Fetch Index VF2 Tail Pointer [DEBUG REG] */
#define VIRTIO_DMA_CSR_RFI_TPTR_VF2_STAT                        0x00038018U
/* Current Software Tail Pointer. Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_TPTR_VF2_STAT_SW_TAIL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_TPTR_VF2_STAT_SW_TAIL_SHIFT          0
#define VIRTIO_DMA_CSR_RFI_TPTR_VF2_STAT_SW_TAIL_WIDTH          16
#define VIRTIO_DMA_CSR_RFI_TPTR_VF2_STAT_SW_TAIL_MASK           (0xFFFFU << VIRTIO_DMA_CSR_RFI_TPTR_VF2_STAT_SW_TAIL_SHIFT)

/* Current Hardware Tail Pointer. Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_TPTR_VF2_STAT_HW_TAIL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_TPTR_VF2_STAT_HW_TAIL_SHIFT          16
#define VIRTIO_DMA_CSR_RFI_TPTR_VF2_STAT_HW_TAIL_WIDTH          16
#define VIRTIO_DMA_CSR_RFI_TPTR_VF2_STAT_HW_TAIL_MASK           (0xFFFFU << VIRTIO_DMA_CSR_RFI_TPTR_VF2_STAT_HW_TAIL_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Fetch Index VF2 Tail Pointer [DEBUG REG] */
typedef union {
  struct {
    uint32_t sw_tail : 16;      /* Current Software Tail Pointer. Resets to 0. */
    uint32_t hw_tail : 16;      /* Current Hardware Tail Pointer. Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFI_TPTR_VF2_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Fetch Index VF3 Tail Pointer [DEBUG REG] */
#define VIRTIO_DMA_CSR_RFI_TPTR_VF3_STAT                        0x0003801CU
/* Current Software Tail Pointer. Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_TPTR_VF3_STAT_SW_TAIL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_TPTR_VF3_STAT_SW_TAIL_SHIFT          0
#define VIRTIO_DMA_CSR_RFI_TPTR_VF3_STAT_SW_TAIL_WIDTH          16
#define VIRTIO_DMA_CSR_RFI_TPTR_VF3_STAT_SW_TAIL_MASK           (0xFFFFU << VIRTIO_DMA_CSR_RFI_TPTR_VF3_STAT_SW_TAIL_SHIFT)

/* Current Hardware Tail Pointer. Resets to 0. */
#define VIRTIO_DMA_CSR_RFI_TPTR_VF3_STAT_HW_TAIL_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFI_TPTR_VF3_STAT_HW_TAIL_SHIFT          16
#define VIRTIO_DMA_CSR_RFI_TPTR_VF3_STAT_HW_TAIL_WIDTH          16
#define VIRTIO_DMA_CSR_RFI_TPTR_VF3_STAT_HW_TAIL_MASK           (0xFFFFU << VIRTIO_DMA_CSR_RFI_TPTR_VF3_STAT_HW_TAIL_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Fetch Index VF3 Tail Pointer [DEBUG REG] */
typedef union {
  struct {
    uint32_t sw_tail : 16;      /* Current Software Tail Pointer. Resets to 0. */
    uint32_t hw_tail : 16;      /* Current Hardware Tail Pointer. Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFI_TPTR_VF3_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Fetch Ring Buffer Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_RFR_BUFFER_STAT                          0x0003A000U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_RFR_BUFFER_STAT_CURRENT_LEVEL_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFR_BUFFER_STAT_CURRENT_LEVEL_SHIFT      0
#define VIRTIO_DMA_CSR_RFR_BUFFER_STAT_CURRENT_LEVEL_WIDTH      10
#define VIRTIO_DMA_CSR_RFR_BUFFER_STAT_CURRENT_LEVEL_MASK       (0x3FFU << VIRTIO_DMA_CSR_RFR_BUFFER_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFR_BUFFER_STAT_MAX_ACCESSTYPE           GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFR_BUFFER_STAT_MAX_SHIFT                10
#define VIRTIO_DMA_CSR_RFR_BUFFER_STAT_MAX_WIDTH                10
#define VIRTIO_DMA_CSR_RFR_BUFFER_STAT_MAX_MASK                 (0x3FFU << VIRTIO_DMA_CSR_RFR_BUFFER_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFR_BUFFER_STAT_FULL_ACCESSTYPE          GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFR_BUFFER_STAT_FULL_SHIFT               20
#define VIRTIO_DMA_CSR_RFR_BUFFER_STAT_FULL_WIDTH               1
#define VIRTIO_DMA_CSR_RFR_BUFFER_STAT_FULL_MASK                (0x1U << VIRTIO_DMA_CSR_RFR_BUFFER_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFR_BUFFER_STAT_FIFO_OVERFLOW_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFR_BUFFER_STAT_FIFO_OVERFLOW_SHIFT      21
#define VIRTIO_DMA_CSR_RFR_BUFFER_STAT_FIFO_OVERFLOW_WIDTH      1
#define VIRTIO_DMA_CSR_RFR_BUFFER_STAT_FIFO_OVERFLOW_MASK       (0x1U << VIRTIO_DMA_CSR_RFR_BUFFER_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFR_BUFFER_STAT_RATE_ACCESSTYPE          GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFR_BUFFER_STAT_RATE_SHIFT               22
#define VIRTIO_DMA_CSR_RFR_BUFFER_STAT_RATE_WIDTH               10
#define VIRTIO_DMA_CSR_RFR_BUFFER_STAT_RATE_MASK                (0x3FFU << VIRTIO_DMA_CSR_RFR_BUFFER_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Fetch Ring Buffer Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFR_BUFFER_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Fetch Ring Statistics Reset */
#define VIRTIO_DMA_CSR_RFR_STATS_RST                            0x0003A000U
/* Write 1 to reset all stats registers in this block. */
#define VIRTIO_DMA_CSR_RFR_STATS_RST_RESET_STATS_ACCESSTYPE     GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_RFR_STATS_RST_RESET_STATS_SHIFT          0
#define VIRTIO_DMA_CSR_RFR_STATS_RST_RESET_STATS_WIDTH          1
#define VIRTIO_DMA_CSR_RFR_STATS_RST_RESET_STATS_MASK           (0x1U << VIRTIO_DMA_CSR_RFR_STATS_RST_RESET_STATS_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Fetch Ring Statistics Reset */
typedef union {
  struct {
    uint32_t reset_stats : 1;   /* Write 1 to reset all stats registers in this block. */
    uint32_t : 31;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFR_STATS_RST_t;
#endif /* GENHDR_STRUCT */

/* Rx Fetch Ring Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_RFR_FIFO_STAT                            0x0003A004U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_RFR_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFR_FIFO_STAT_CURRENT_LEVEL_SHIFT        0
#define VIRTIO_DMA_CSR_RFR_FIFO_STAT_CURRENT_LEVEL_WIDTH        10
#define VIRTIO_DMA_CSR_RFR_FIFO_STAT_CURRENT_LEVEL_MASK         (0x3FFU << VIRTIO_DMA_CSR_RFR_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFR_FIFO_STAT_MAX_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFR_FIFO_STAT_MAX_SHIFT                  10
#define VIRTIO_DMA_CSR_RFR_FIFO_STAT_MAX_WIDTH                  10
#define VIRTIO_DMA_CSR_RFR_FIFO_STAT_MAX_MASK                   (0x3FFU << VIRTIO_DMA_CSR_RFR_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFR_FIFO_STAT_FULL_ACCESSTYPE            GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFR_FIFO_STAT_FULL_SHIFT                 20
#define VIRTIO_DMA_CSR_RFR_FIFO_STAT_FULL_WIDTH                 1
#define VIRTIO_DMA_CSR_RFR_FIFO_STAT_FULL_MASK                  (0x1U << VIRTIO_DMA_CSR_RFR_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFR_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFR_FIFO_STAT_FIFO_OVERFLOW_SHIFT        21
#define VIRTIO_DMA_CSR_RFR_FIFO_STAT_FIFO_OVERFLOW_WIDTH        1
#define VIRTIO_DMA_CSR_RFR_FIFO_STAT_FIFO_OVERFLOW_MASK         (0x1U << VIRTIO_DMA_CSR_RFR_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFR_FIFO_STAT_RATE_ACCESSTYPE            GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFR_FIFO_STAT_RATE_SHIFT                 22
#define VIRTIO_DMA_CSR_RFR_FIFO_STAT_RATE_WIDTH                 10
#define VIRTIO_DMA_CSR_RFR_FIFO_STAT_RATE_MASK                  (0x3FFU << VIRTIO_DMA_CSR_RFR_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Fetch Ring Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFR_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Fetch Ring Underflow Count Status */
#define VIRTIO_DMA_CSR_RFR_UFLOW_STAT                           0x0003A014U
/* Underflow Count. Resets to 0. */
#define VIRTIO_DMA_CSR_RFR_UFLOW_STAT_UNDER_FLOW_CNT_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFR_UFLOW_STAT_UNDER_FLOW_CNT_SHIFT      0
#define VIRTIO_DMA_CSR_RFR_UFLOW_STAT_UNDER_FLOW_CNT_WIDTH      32
#define VIRTIO_DMA_CSR_RFR_UFLOW_STAT_UNDER_FLOW_CNT_MASK       (0xFFFFFFFFU << VIRTIO_DMA_CSR_RFR_UFLOW_STAT_UNDER_FLOW_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Fetch Ring Underflow Count Status */
typedef union {
  struct {
    uint32_t under_flow_cnt : 32;       /* Underflow Count. Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFR_UFLOW_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Fetch Ring Overflow Count Status */
#define VIRTIO_DMA_CSR_RFR_OFLOW_STAT                           0x0003A018U
/* Overflow Count. Resets to 0. */
#define VIRTIO_DMA_CSR_RFR_OFLOW_STAT_OVER_FLOW_CNT_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFR_OFLOW_STAT_OVER_FLOW_CNT_SHIFT       0
#define VIRTIO_DMA_CSR_RFR_OFLOW_STAT_OVER_FLOW_CNT_WIDTH       32
#define VIRTIO_DMA_CSR_RFR_OFLOW_STAT_OVER_FLOW_CNT_MASK        (0xFFFFFFFFU << VIRTIO_DMA_CSR_RFR_OFLOW_STAT_OVER_FLOW_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Fetch Ring Overflow Count Status */
typedef union {
  struct {
    uint32_t over_flow_cnt : 32;        /* Overflow Count. Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFR_OFLOW_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Fetch Ring Error Status */
#define VIRTIO_DMA_CSR_RFR_ERROR_STAT                   0x0003A01CU
/* Rx Fetch Ring Overflow occurred. Resets to 0. */
#define VIRTIO_DMA_CSR_RFR_ERROR_STAT_O_FLOW_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFR_ERROR_STAT_O_FLOW_SHIFT      0
#define VIRTIO_DMA_CSR_RFR_ERROR_STAT_O_FLOW_WIDTH      1
#define VIRTIO_DMA_CSR_RFR_ERROR_STAT_O_FLOW_MASK       (0x1U << VIRTIO_DMA_CSR_RFR_ERROR_STAT_O_FLOW_SHIFT)

/* Rx Fetch Ring Underflow occurred. Resets to 0. */
#define VIRTIO_DMA_CSR_RFR_ERROR_STAT_U_FLOW_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFR_ERROR_STAT_U_FLOW_SHIFT      1
#define VIRTIO_DMA_CSR_RFR_ERROR_STAT_U_FLOW_WIDTH      1
#define VIRTIO_DMA_CSR_RFR_ERROR_STAT_U_FLOW_MASK       (0x1U << VIRTIO_DMA_CSR_RFR_ERROR_STAT_U_FLOW_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Fetch Ring Error Status */
typedef union {
  struct {
    uint32_t o_flow : 1;        /* Rx Fetch Ring Overflow occurred. Resets to 0. */
    uint32_t u_flow : 1;        /* Rx Fetch Ring Underflow occurred. Resets to 0. */
    uint32_t : 30;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFR_ERROR_STAT_t;
#endif /* GENHDR_STRUCT */

/* Ring Command Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT                               0x0003C000U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_CURRENT_LEVEL_SHIFT           0
#define VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_CURRENT_LEVEL_WIDTH           10
#define VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_CURRENT_LEVEL_MASK            (0x3FFU << VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_MAX_ACCESSTYPE                GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_MAX_SHIFT                     10
#define VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_MAX_WIDTH                     10
#define VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_MAX_MASK                      (0x3FFU << VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_FULL_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_FULL_SHIFT                    20
#define VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_FULL_WIDTH                    1
#define VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_FULL_MASK                     (0x1U << VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_FIFO_OVERFLOW_SHIFT           21
#define VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_FIFO_OVERFLOW_WIDTH           1
#define VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_FIFO_OVERFLOW_MASK            (0x1U << VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_RATE_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_RATE_SHIFT                    22
#define VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_RATE_WIDTH                    10
#define VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_RATE_MASK                     (0x3FFU << VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Ring Command Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFD_RCMD_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Fetch Descriptor Statistics Reset [DEBUG REG] */
#define VIRTIO_DMA_CSR_RFD_STATS_RST                            0x0003C000U
/* Write 1 to reset all stats registers in this block. */
#define VIRTIO_DMA_CSR_RFD_STATS_RST_RESET_STATS_ACCESSTYPE     GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_RFD_STATS_RST_RESET_STATS_SHIFT          0
#define VIRTIO_DMA_CSR_RFD_STATS_RST_RESET_STATS_WIDTH          1
#define VIRTIO_DMA_CSR_RFD_STATS_RST_RESET_STATS_MASK           (0x1U << VIRTIO_DMA_CSR_RFD_STATS_RST_RESET_STATS_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Fetch Descriptor Statistics Reset [DEBUG REG] */
typedef union {
  struct {
    uint32_t reset_stats : 1;   /* Write 1 to reset all stats registers in this block. */
    uint32_t : 31;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFD_STATS_RST_t;
#endif /* GENHDR_STRUCT */

/* Descriptor Command Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT                               0x0003C004U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_CURRENT_LEVEL_SHIFT           0
#define VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_CURRENT_LEVEL_WIDTH           10
#define VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_CURRENT_LEVEL_MASK            (0x3FFU << VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_MAX_ACCESSTYPE                GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_MAX_SHIFT                     10
#define VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_MAX_WIDTH                     10
#define VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_MAX_MASK                      (0x3FFU << VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_FULL_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_FULL_SHIFT                    20
#define VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_FULL_WIDTH                    1
#define VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_FULL_MASK                     (0x1U << VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_FIFO_OVERFLOW_SHIFT           21
#define VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_FIFO_OVERFLOW_WIDTH           1
#define VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_FIFO_OVERFLOW_MASK            (0x1U << VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_RATE_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_RATE_SHIFT                    22
#define VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_RATE_WIDTH                    10
#define VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_RATE_MASK                     (0x3FFU << VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Descriptor Command Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFD_DCMD_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Fetch Descriptor Packet Termination Control [DEBUG REG] */
#define VIRTIO_DMA_CSR_RFD_PKT_TERM_CFG                                 0x0003C004U
/*
 * Disable Early Termination of Descriptor Chain.
 *  If a packet doesn't use a complete chain, the DMA will terminate fetching of
 * further unused descriptors in the chain. Setting this bit disables that
 * functionality so that each descriptor chain is fully fetched. This is a debug
 * feature.
 */
#define VIRTIO_DMA_CSR_RFD_PKT_TERM_CFG_TERM_EARLY_DIS_ACCESSTYPE       GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_RFD_PKT_TERM_CFG_TERM_EARLY_DIS_SHIFT            0
#define VIRTIO_DMA_CSR_RFD_PKT_TERM_CFG_TERM_EARLY_DIS_WIDTH            1
#define VIRTIO_DMA_CSR_RFD_PKT_TERM_CFG_TERM_EARLY_DIS_MASK             (0x1U << VIRTIO_DMA_CSR_RFD_PKT_TERM_CFG_TERM_EARLY_DIS_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Fetch Descriptor Packet Termination Control [DEBUG REG] */
typedef union {
  struct {
    uint32_t term_early_dis : 1;        /* Disable Early Termination of Descriptor Chain.
                                           If a packet doesn't use a complete chain, the DMA will terminate fetching of
                                           further unused descriptors in the chain. Setting this bit disables that
                                           functionality so that each descriptor chain is fully fetched. This is a debug
                                           feature. */
    uint32_t : 31;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFD_PKT_TERM_CFG_t;
#endif /* GENHDR_STRUCT */

/* Rx fetch Descriptor Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_RFD_FIFO_STAT                            0x0003C008U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_RFD_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFD_FIFO_STAT_CURRENT_LEVEL_SHIFT        0
#define VIRTIO_DMA_CSR_RFD_FIFO_STAT_CURRENT_LEVEL_WIDTH        10
#define VIRTIO_DMA_CSR_RFD_FIFO_STAT_CURRENT_LEVEL_MASK         (0x3FFU << VIRTIO_DMA_CSR_RFD_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFD_FIFO_STAT_MAX_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFD_FIFO_STAT_MAX_SHIFT                  10
#define VIRTIO_DMA_CSR_RFD_FIFO_STAT_MAX_WIDTH                  10
#define VIRTIO_DMA_CSR_RFD_FIFO_STAT_MAX_MASK                   (0x3FFU << VIRTIO_DMA_CSR_RFD_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFD_FIFO_STAT_FULL_ACCESSTYPE            GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFD_FIFO_STAT_FULL_SHIFT                 20
#define VIRTIO_DMA_CSR_RFD_FIFO_STAT_FULL_WIDTH                 1
#define VIRTIO_DMA_CSR_RFD_FIFO_STAT_FULL_MASK                  (0x1U << VIRTIO_DMA_CSR_RFD_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFD_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFD_FIFO_STAT_FIFO_OVERFLOW_SHIFT        21
#define VIRTIO_DMA_CSR_RFD_FIFO_STAT_FIFO_OVERFLOW_WIDTH        1
#define VIRTIO_DMA_CSR_RFD_FIFO_STAT_FIFO_OVERFLOW_MASK         (0x1U << VIRTIO_DMA_CSR_RFD_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFD_FIFO_STAT_RATE_ACCESSTYPE            GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFD_FIFO_STAT_RATE_SHIFT                 22
#define VIRTIO_DMA_CSR_RFD_FIFO_STAT_RATE_WIDTH                 10
#define VIRTIO_DMA_CSR_RFD_FIFO_STAT_RATE_MASK                  (0x3FFU << VIRTIO_DMA_CSR_RFD_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx fetch Descriptor Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFD_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx fetch Descriptor Free DB Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT                             0x0003C00CU
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_CURRENT_LEVEL_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_CURRENT_LEVEL_SHIFT         0
#define VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_CURRENT_LEVEL_WIDTH         10
#define VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_CURRENT_LEVEL_MASK          (0x3FFU << VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Min fill level (latched). Resets to 10'h3ff. */
#define VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_MIN_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_MIN_SHIFT                   10
#define VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_MIN_WIDTH                   10
#define VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_MIN_MASK                    (0x3FFU << VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_MIN_SHIFT)

/* Fifo has been empty, or nearly empty (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_EMPTY_ACCESSTYPE            GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_EMPTY_SHIFT                 20
#define VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_EMPTY_WIDTH                 1
#define VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_EMPTY_MASK                  (0x1U << VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_EMPTY_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_FIFO_OVERFLOW_SHIFT         21
#define VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_FIFO_OVERFLOW_WIDTH         1
#define VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_FIFO_OVERFLOW_MASK          (0x1U << VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_RATE_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_RATE_SHIFT                  22
#define VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_RATE_WIDTH                  10
#define VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_RATE_MASK                   (0x3FFU << VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx fetch Descriptor Free DB Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t min : 10;                  /* Fifo Min fill level (latched). Resets to 10'h3ff. */
    uint32_t empty : 1;                 /* Fifo has been empty, or nearly empty (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RFD_FREE_DBFIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Store Packet Scheduler DQ Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT                             0x0003E000U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_CURRENT_LEVEL_SHIFT         0
#define VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_CURRENT_LEVEL_WIDTH         10
#define VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_CURRENT_LEVEL_MASK          (0x3FFU << VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_MAX_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_MAX_SHIFT                   10
#define VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_MAX_WIDTH                   10
#define VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_MAX_MASK                    (0x3FFU << VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_FULL_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_FULL_SHIFT                  20
#define VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_FULL_WIDTH                  1
#define VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_FULL_MASK                   (0x1U << VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_FIFO_OVERFLOW_SHIFT         21
#define VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_FIFO_OVERFLOW_WIDTH         1
#define VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_FIFO_OVERFLOW_MASK          (0x1U << VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_RATE_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_RATE_SHIFT                  22
#define VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_RATE_WIDTH                  10
#define VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_RATE_MASK                   (0x3FFU << VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Store Packet Scheduler DQ Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RSP_SCH_DQ_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Store Packet Statistics Reset */
#define VIRTIO_DMA_CSR_RSP_STATS_RST                            0x0003E000U
/* Write 1 to reset all stats registers in this block. */
#define VIRTIO_DMA_CSR_RSP_STATS_RST_RESET_STATS_ACCESSTYPE     GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_RSP_STATS_RST_RESET_STATS_SHIFT          0
#define VIRTIO_DMA_CSR_RSP_STATS_RST_RESET_STATS_WIDTH          1
#define VIRTIO_DMA_CSR_RSP_STATS_RST_RESET_STATS_MASK           (0x1U << VIRTIO_DMA_CSR_RSP_STATS_RST_RESET_STATS_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Store Packet Statistics Reset */
typedef union {
  struct {
    uint32_t reset_stats : 1;   /* Write 1 to reset all stats registers in this block. */
    uint32_t : 31;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RSP_STATS_RST_t;
#endif /* GENHDR_STRUCT */

/* Rx Store Packet Data Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_RSP_D_FIFO_STAT                          0x0003E004U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_CURRENT_LEVEL_SHIFT      0
#define VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_CURRENT_LEVEL_WIDTH      10
#define VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_CURRENT_LEVEL_MASK       (0x3FFU << VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_MAX_ACCESSTYPE           GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_MAX_SHIFT                10
#define VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_MAX_WIDTH                10
#define VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_MAX_MASK                 (0x3FFU << VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_FULL_ACCESSTYPE          GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_FULL_SHIFT               20
#define VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_FULL_WIDTH               1
#define VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_FULL_MASK                (0x1U << VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_FIFO_OVERFLOW_SHIFT      21
#define VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_FIFO_OVERFLOW_WIDTH      1
#define VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_FIFO_OVERFLOW_MASK       (0x1U << VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_RATE_ACCESSTYPE          GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_RATE_SHIFT               22
#define VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_RATE_WIDTH               10
#define VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_RATE_MASK                (0x3FFU << VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Store Packet Data Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RSP_D_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Store Packet Test Mode [DEBUG REG] */
#define VIRTIO_DMA_CSR_RSP_TEST_MODE                            0x0003E004U
/*
 * Write 1 to enable Virtio 0.9.5 mode. This allows 0.9.5 support to be enabled
 * without having to negotiate features. Test mode only.
 */
#define VIRTIO_DMA_CSR_RSP_TEST_MODE_V_95_TESTMODE_ACCESSTYPE   GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_RSP_TEST_MODE_V_95_TESTMODE_SHIFT        0
#define VIRTIO_DMA_CSR_RSP_TEST_MODE_V_95_TESTMODE_WIDTH        1
#define VIRTIO_DMA_CSR_RSP_TEST_MODE_V_95_TESTMODE_MASK         (0x1U << VIRTIO_DMA_CSR_RSP_TEST_MODE_V_95_TESTMODE_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Store Packet Test Mode [DEBUG REG] */
typedef union {
  struct {
    uint32_t v_95_testmode : 1;         /* Write 1 to enable Virtio 0.9.5 mode. This allows 0.9.5 support to be enabled
                                           without having to negotiate features. Test mode only. */
    uint32_t : 31;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RSP_TEST_MODE_t;
#endif /* GENHDR_STRUCT */

/* Rx Store Packet WRAM Head Buffer Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT                             0x0003E008U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_CURRENT_LEVEL_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_CURRENT_LEVEL_SHIFT         0
#define VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_CURRENT_LEVEL_WIDTH         10
#define VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_CURRENT_LEVEL_MASK          (0x3FFU << VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_MAX_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_MAX_SHIFT                   10
#define VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_MAX_WIDTH                   10
#define VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_MAX_MASK                    (0x3FFU << VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_FULL_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_FULL_SHIFT                  20
#define VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_FULL_WIDTH                  1
#define VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_FULL_MASK                   (0x1U << VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_FIFO_OVERFLOW_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_FIFO_OVERFLOW_SHIFT         21
#define VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_FIFO_OVERFLOW_WIDTH         1
#define VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_FIFO_OVERFLOW_MASK          (0x1U << VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_RATE_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_RATE_SHIFT                  22
#define VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_RATE_WIDTH                  10
#define VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_RATE_MASK                   (0x3FFU << VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Store Packet WRAM Head Buffer Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RSP_WRAM_HD_BUF_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Store Packet Descriptor Command Buffer Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT                             0x0003E00CU
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_CURRENT_LEVEL_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_CURRENT_LEVEL_SHIFT         0
#define VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_CURRENT_LEVEL_WIDTH         10
#define VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_CURRENT_LEVEL_MASK          (0x3FFU << VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_MAX_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_MAX_SHIFT                   10
#define VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_MAX_WIDTH                   10
#define VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_MAX_MASK                    (0x3FFU << VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_FULL_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_FULL_SHIFT                  20
#define VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_FULL_WIDTH                  1
#define VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_FULL_MASK                   (0x1U << VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_FIFO_OVERFLOW_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_FIFO_OVERFLOW_SHIFT         21
#define VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_FIFO_OVERFLOW_WIDTH         1
#define VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_FIFO_OVERFLOW_MASK          (0x1U << VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_RATE_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_RATE_SHIFT                  22
#define VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_RATE_WIDTH                  10
#define VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_RATE_MASK                   (0x3FFU << VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Store Packet Descriptor Command Buffer Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RSP_DESC_CMDBUF_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Store Packet WRAM Buf Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT                                0x0003E010U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_CURRENT_LEVEL_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_CURRENT_LEVEL_SHIFT            0
#define VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_CURRENT_LEVEL_WIDTH            10
#define VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_CURRENT_LEVEL_MASK             (0x3FFU << VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_MAX_ACCESSTYPE                 GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_MAX_SHIFT                      10
#define VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_MAX_WIDTH                      10
#define VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_MAX_MASK                       (0x3FFU << VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_FULL_ACCESSTYPE                GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_FULL_SHIFT                     20
#define VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_FULL_WIDTH                     1
#define VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_FULL_MASK                      (0x1U << VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_FIFO_OVERFLOW_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_FIFO_OVERFLOW_SHIFT            21
#define VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_FIFO_OVERFLOW_WIDTH            1
#define VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_FIFO_OVERFLOW_MASK             (0x1U << VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_RATE_ACCESSTYPE                GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_RATE_SHIFT                     22
#define VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_RATE_WIDTH                     10
#define VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_RATE_MASK                      (0x3FFU << VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Store Packet WRAM Buf Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RSP_WRAM_BUF_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Store Packet Egress Update Fifo Status [DEBUG REG] */
#define VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT                                0x0003E014U
/* Fifo current fill level. Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_CURRENT_LEVEL_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_CURRENT_LEVEL_SHIFT            0
#define VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_CURRENT_LEVEL_WIDTH            10
#define VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_CURRENT_LEVEL_MASK             (0x3FFU << VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_CURRENT_LEVEL_SHIFT)

/* Fifo Max fill level (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_MAX_ACCESSTYPE                 GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_MAX_SHIFT                      10
#define VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_MAX_WIDTH                      10
#define VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_MAX_MASK                       (0x3FFU << VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_MAX_SHIFT)

/* Fifo has been full (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_FULL_ACCESSTYPE                GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_FULL_SHIFT                     20
#define VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_FULL_WIDTH                     1
#define VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_FULL_MASK                      (0x1U << VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_FULL_SHIFT)

/* Fifo has overflowed when set (latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_FIFO_OVERFLOW_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_FIFO_OVERFLOW_SHIFT            21
#define VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_FIFO_OVERFLOW_WIDTH            1
#define VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_FIFO_OVERFLOW_MASK             (0x1U << VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_FIFO_OVERFLOW_SHIFT)

/* Current Fifo Throughput Rate (not latched). Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_RATE_ACCESSTYPE                GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_RATE_SHIFT                     22
#define VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_RATE_WIDTH                     10
#define VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_RATE_MASK                      (0x3FFU << VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_RATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Store Packet Egress Update Fifo Status [DEBUG REG] */
typedef union {
  struct {
    uint32_t current_level : 10;        /* Fifo current fill level. Resets to 0. */
    uint32_t max : 10;                  /* Fifo Max fill level (latched). Resets to 0. */
    uint32_t full : 1;                  /* Fifo has been full (latched). Resets to 0. */
    uint32_t fifo_overflow : 1;         /* Fifo has overflowed when set (latched). Resets to 0. */
    uint32_t rate : 10;                 /* Current Fifo Throughput Rate (not latched). Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RSP_UPD_FIFO_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Store Packet Error Status */
#define VIRTIO_DMA_CSR_RSP_ERROR_STAT                           0x0003E01CU
/* Rx Store Packet Error value. Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_ERROR_STAT_STORE_PKT_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_ERROR_STAT_STORE_PKT_SHIFT           0
#define VIRTIO_DMA_CSR_RSP_ERROR_STAT_STORE_PKT_WIDTH           3
#define VIRTIO_DMA_CSR_RSP_ERROR_STAT_STORE_PKT_MASK            (0x7U << VIRTIO_DMA_CSR_RSP_ERROR_STAT_STORE_PKT_SHIFT)

/* Rx Index Mismatch Error. Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_ERROR_STAT_INDEX_MISMATCH_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_ERROR_STAT_INDEX_MISMATCH_SHIFT      3
#define VIRTIO_DMA_CSR_RSP_ERROR_STAT_INDEX_MISMATCH_WIDTH      1
#define VIRTIO_DMA_CSR_RSP_ERROR_STAT_INDEX_MISMATCH_MASK       (0x1U << VIRTIO_DMA_CSR_RSP_ERROR_STAT_INDEX_MISMATCH_SHIFT)

/*
 * Rx Packet Integrity Errors. Resets to 0. Bit 7 indicates Unexpected QID. 
 *  Bit 6 Indicates Missing SOP. 
 *  Bit 5 indicates Missing EOP.
 */
#define VIRTIO_DMA_CSR_RSP_ERROR_STAT_PKT_INTEGRITY_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_ERROR_STAT_PKT_INTEGRITY_SHIFT       5
#define VIRTIO_DMA_CSR_RSP_ERROR_STAT_PKT_INTEGRITY_WIDTH       3
#define VIRTIO_DMA_CSR_RSP_ERROR_STAT_PKT_INTEGRITY_MASK        (0x7U << VIRTIO_DMA_CSR_RSP_ERROR_STAT_PKT_INTEGRITY_SHIFT)

/* Rx FIFO Overflow Error. Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_ERROR_STAT_RX_OVERFLOW_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_ERROR_STAT_RX_OVERFLOW_SHIFT         8
#define VIRTIO_DMA_CSR_RSP_ERROR_STAT_RX_OVERFLOW_WIDTH         1
#define VIRTIO_DMA_CSR_RSP_ERROR_STAT_RX_OVERFLOW_MASK          (0x1U << VIRTIO_DMA_CSR_RSP_ERROR_STAT_RX_OVERFLOW_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Store Packet Error Status */
typedef union {
  struct {
    uint32_t store_pkt : 3;             /* Rx Store Packet Error value. Resets to 0. */
    uint32_t index_mismatch : 1;        /* Rx Index Mismatch Error. Resets to 0. */
    uint32_t : 1;
    uint32_t pkt_integrity : 3;         /* Rx Packet Integrity Errors. Resets to 0. Bit 7 indicates Unexpected QID.
                                           Bit 6 Indicates Missing SOP.
                                           Bit 5 indicates Missing EOP. */
    uint32_t rx_overflow : 1;           /* Rx FIFO Overflow Error. Resets to 0. */
    uint32_t : 23;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RSP_ERROR_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Store Packet Dropped Packet Count [DEBUG REG] */
#define VIRTIO_DMA_CSR_RSP_DROP_COUNT_STAT                              0x0003E020U
/* Number of Dropped Packets. Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_DROP_COUNT_STAT_DROPPED_CNT_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_DROP_COUNT_STAT_DROPPED_CNT_SHIFT            0
#define VIRTIO_DMA_CSR_RSP_DROP_COUNT_STAT_DROPPED_CNT_WIDTH            32
#define VIRTIO_DMA_CSR_RSP_DROP_COUNT_STAT_DROPPED_CNT_MASK             (0xFFFFFFFFU << VIRTIO_DMA_CSR_RSP_DROP_COUNT_STAT_DROPPED_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Store Packet Dropped Packet Count [DEBUG REG] */
typedef union {
  struct {
    uint32_t dropped_cnt : 32;  /* Number of Dropped Packets. Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RSP_DROP_COUNT_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Store Packet Counter Reset */
#define VIRTIO_DMA_CSR_RSP_COUNTS_RST                           0x0003E020U
/* Write 1 to reset all counter registers in this block. */
#define VIRTIO_DMA_CSR_RSP_COUNTS_RST_RESET_COUNTS_ACCESSTYPE   GENHDR_ACCESSTYPE_WO
#define VIRTIO_DMA_CSR_RSP_COUNTS_RST_RESET_COUNTS_SHIFT        0
#define VIRTIO_DMA_CSR_RSP_COUNTS_RST_RESET_COUNTS_WIDTH        1
#define VIRTIO_DMA_CSR_RSP_COUNTS_RST_RESET_COUNTS_MASK         (0x1U << VIRTIO_DMA_CSR_RSP_COUNTS_RST_RESET_COUNTS_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Store Packet Counter Reset */
typedef union {
  struct {
    uint32_t reset_counts : 1;  /* Write 1 to reset all counter registers in this block. */
    uint32_t : 31;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RSP_COUNTS_RST_t;
#endif /* GENHDR_STRUCT */

/* Rx Store Packet Oversized Packet Count [DEBUG REG] */
#define VIRTIO_DMA_CSR_RSP_OVRSZD_CNT_STAT                              0x0003E024U
/* Number of Oversized Packets. Resets to 0. */
#define VIRTIO_DMA_CSR_RSP_OVRSZD_CNT_STAT_OVERSIZED_CNT_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_OVRSZD_CNT_STAT_OVERSIZED_CNT_SHIFT          0
#define VIRTIO_DMA_CSR_RSP_OVRSZD_CNT_STAT_OVERSIZED_CNT_WIDTH          32
#define VIRTIO_DMA_CSR_RSP_OVRSZD_CNT_STAT_OVERSIZED_CNT_MASK           (0xFFFFFFFFU << VIRTIO_DMA_CSR_RSP_OVRSZD_CNT_STAT_OVERSIZED_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Store Packet Oversized Packet Count [DEBUG REG] */
typedef union {
  struct {
    uint32_t oversized_cnt : 32;        /* Number of Oversized Packets. Resets to 0. */
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RSP_OVRSZD_CNT_STAT_t;
#endif /* GENHDR_STRUCT */

/* Rx Store Packet Warning Status */
#define VIRTIO_DMA_CSR_RSP_WARNING_STAT                         0x0003E03CU
/*
 * Rx Packet Dropped. This is a warning indication, as packet dropping in Rx is
 * expected in some circumstances.(Non Fatal). Resets to 0.
 */
#define VIRTIO_DMA_CSR_RSP_WARNING_STAT_RX_DROPPED_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_WARNING_STAT_RX_DROPPED_SHIFT        0
#define VIRTIO_DMA_CSR_RSP_WARNING_STAT_RX_DROPPED_WIDTH        1
#define VIRTIO_DMA_CSR_RSP_WARNING_STAT_RX_DROPPED_MASK         (0x1U << VIRTIO_DMA_CSR_RSP_WARNING_STAT_RX_DROPPED_SHIFT)

/*
 * Rx Packet Oversize, longer than descriptor. This is a warning indication, as
 * packet dropping in Rx is expected in some circumstances.(Non Fatal). Resets
 * to 0.
 */
#define VIRTIO_DMA_CSR_RSP_WARNING_STAT_RX_OVERSIZED_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define VIRTIO_DMA_CSR_RSP_WARNING_STAT_RX_OVERSIZED_SHIFT      1
#define VIRTIO_DMA_CSR_RSP_WARNING_STAT_RX_OVERSIZED_WIDTH      1
#define VIRTIO_DMA_CSR_RSP_WARNING_STAT_RX_OVERSIZED_MASK       (0x1U << VIRTIO_DMA_CSR_RSP_WARNING_STAT_RX_OVERSIZED_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx Store Packet Warning Status */
typedef union {
  struct {
    uint32_t rx_dropped : 1;    /* Rx Packet Dropped. This is a warning indication, as packet dropping in Rx is
                                   expected in some circumstances.(Non Fatal). Resets to 0. */
    uint32_t rx_oversized : 1;  /* Rx Packet Oversize, longer than descriptor. This is a warning indication, as
                                   packet dropping in Rx is expected in some circumstances.(Non Fatal). Resets
                                   to 0. */
    uint32_t : 30;
  } field;
  uint32_t val;
} VIRTIO_DMA_CSR_RSP_WARNING_STAT_t;
#endif /* GENHDR_STRUCT */

#endif /* _VIRTIO_DMA_CSR_REGISTERS_H_ */

