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

#ifndef _MKS_VIOSHM_REGISTERS_H_
#define _MKS_VIOSHM_REGISTERS_H_

/* OVS Virtio Shim */

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

#define MKS_VIOSHM_ACCESSTYPE           GENHDR_ACCESSTYPE_RW /*  Default access type. Access types defined above. */
#define MKS_VIOSHM_REGWIDTH             32 /* Default width of register in bits */
#define MKS_VIOSHM_ACCESSWIDTH          32 /* Default width of access word in bit */


/* General purpose RW register intended for debug use */
#define MKS_VIOSHM_VIOSHM_SCRATCH                       0x00000000U
/* Scratch data. */
#define MKS_VIOSHM_VIOSHM_SCRATCH_DATA_SHIFT            0
#define MKS_VIOSHM_VIOSHM_SCRATCH_DATA_WIDTH            32
#define MKS_VIOSHM_VIOSHM_SCRATCH_DATA_MASK             (0xFFFFFFFFU << MKS_VIOSHM_VIOSHM_SCRATCH_DATA_SHIFT)

#ifdef GENHDR_STRUCT
/* General purpose RW register intended for debug use */
typedef union {
  struct {
    uint32_t data : 32; /* Scratch data. */
  } field;
  uint32_t val;
} MKS_VIOSHM_VIOSHM_SCRATCH_t;
#endif /* GENHDR_STRUCT */

/* Virtio Shim to OVS packet counter */
#define MKS_VIOSHM_VIOSHM_V2_O_PKTCNT                   0x00000004U
/* Counts number of packet VirtIo Shim transmits to OVS. */
#define MKS_VIOSHM_VIOSHM_V2_O_PKTCNT_CNT_SHIFT         0
#define MKS_VIOSHM_VIOSHM_V2_O_PKTCNT_CNT_WIDTH         32
#define MKS_VIOSHM_VIOSHM_V2_O_PKTCNT_CNT_MASK          (0xFFFFFFFFU << MKS_VIOSHM_VIOSHM_V2_O_PKTCNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Virtio Shim to OVS packet counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number of packet VirtIo Shim transmits to OVS. */
  } field;
  uint32_t val;
} MKS_VIOSHM_VIOSHM_V2_O_PKTCNT_t;
#endif /* GENHDR_STRUCT */

/* OVS to VirtIo shim packet counter */
#define MKS_VIOSHM_VIOSHM_O2_V_PKTCNT                   0x00000008U
/* Counts number of packet VirtIo Shim receives from OVS. */
#define MKS_VIOSHM_VIOSHM_O2_V_PKTCNT_CNT_SHIFT         0
#define MKS_VIOSHM_VIOSHM_O2_V_PKTCNT_CNT_WIDTH         32
#define MKS_VIOSHM_VIOSHM_O2_V_PKTCNT_CNT_MASK          (0xFFFFFFFFU << MKS_VIOSHM_VIOSHM_O2_V_PKTCNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* OVS to VirtIo shim packet counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number of packet VirtIo Shim receives from OVS. */
  } field;
  uint32_t val;
} MKS_VIOSHM_VIOSHM_O2_V_PKTCNT_t;
#endif /* GENHDR_STRUCT */

/* DMA to Virtio Shim packet counter */
#define MKS_VIOSHM_VIOSHM_D2_V_PKTCNT                   0x0000000CU
/* Counts number of packet VirtIo Shim receives from DMA. */
#define MKS_VIOSHM_VIOSHM_D2_V_PKTCNT_CNT_SHIFT         0
#define MKS_VIOSHM_VIOSHM_D2_V_PKTCNT_CNT_WIDTH         32
#define MKS_VIOSHM_VIOSHM_D2_V_PKTCNT_CNT_MASK          (0xFFFFFFFFU << MKS_VIOSHM_VIOSHM_D2_V_PKTCNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* DMA to Virtio Shim packet counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number of packet VirtIo Shim receives from DMA. */
  } field;
  uint32_t val;
} MKS_VIOSHM_VIOSHM_D2_V_PKTCNT_t;
#endif /* GENHDR_STRUCT */

/* Virtio to DMA packet counter */
#define MKS_VIOSHM_VIOSHM_V2_D_PKTCNT                   0x00000010U
/* Counts number of packet VirtIo shim transmits to DMA. */
#define MKS_VIOSHM_VIOSHM_V2_D_PKTCNT_CNT_SHIFT         0
#define MKS_VIOSHM_VIOSHM_V2_D_PKTCNT_CNT_WIDTH         32
#define MKS_VIOSHM_VIOSHM_V2_D_PKTCNT_CNT_MASK          (0xFFFFFFFFU << MKS_VIOSHM_VIOSHM_V2_D_PKTCNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Virtio to DMA packet counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number of packet VirtIo shim transmits to DMA. */
  } field;
  uint32_t val;
} MKS_VIOSHM_VIOSHM_V2_D_PKTCNT_t;
#endif /* GENHDR_STRUCT */

/* Virtio Shim error status hold */
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD                            0x00000014U
/* Virtio Tx Ring FIFO underflow. */
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_RING_FIFO_UD_ACCESSTYPE GENHDR_ACCESSTYPE_W1C
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_RING_FIFO_UD_SHIFT      0
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_RING_FIFO_UD_WIDTH      1
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_RING_FIFO_UD_MASK       (0x1U << MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_RING_FIFO_UD_SHIFT)

/* Virtio Tx Ring FIFO overflow. */
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_RING_FIFO_OV_ACCESSTYPE GENHDR_ACCESSTYPE_W1C
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_RING_FIFO_OV_SHIFT      1
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_RING_FIFO_OV_WIDTH      1
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_RING_FIFO_OV_MASK       (0x1U << MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_RING_FIFO_OV_SHIFT)

/* Virtio Tx FIFO underflow. */
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_FIFO_UD_ACCESSTYPE      GENHDR_ACCESSTYPE_W1C
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_FIFO_UD_SHIFT           2
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_FIFO_UD_WIDTH           1
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_FIFO_UD_MASK            (0x1U << MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_FIFO_UD_SHIFT)

/* Virtio Tx FIFO overflow. */
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_FIFO_OV_ACCESSTYPE      GENHDR_ACCESSTYPE_W1C
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_FIFO_OV_SHIFT           3
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_FIFO_OV_WIDTH           1
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_FIFO_OV_MASK            (0x1U << MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_FIFO_OV_SHIFT)

/* Virtio Shim Tx Ingress Port ID error. */
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_INGR_PORT_ID_ACCESSTYPE GENHDR_ACCESSTYPE_W1C
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_INGR_PORT_ID_SHIFT      4
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_INGR_PORT_ID_WIDTH      1
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_INGR_PORT_ID_MASK       (0x1U << MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_INGR_PORT_ID_SHIFT)

/* Virtio Shim Tx Ingress Missing Eop error. */
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_INGR_MISS_EOP_ACCESSTYPE GENHDR_ACCESSTYPE_W1C
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_INGR_MISS_EOP_SHIFT     5
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_INGR_MISS_EOP_WIDTH     1
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_INGR_MISS_EOP_MASK      (0x1U << MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_INGR_MISS_EOP_SHIFT)

/* Virtio Shim Tx Ingress Missing Sop error. */
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_INGR_MISS_SOP_ACCESSTYPE GENHDR_ACCESSTYPE_W1C
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_INGR_MISS_SOP_SHIFT     6
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_INGR_MISS_SOP_WIDTH     1
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_INGR_MISS_SOP_MASK      (0x1U << MKS_VIOSHM_VIOSHM_ERROR_HOLD_TX_INGR_MISS_SOP_SHIFT)

/* Virtio Rx FIFO underflow. */
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_FIFO_UD_ACCESSTYPE      GENHDR_ACCESSTYPE_W1C
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_FIFO_UD_SHIFT           8
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_FIFO_UD_WIDTH           1
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_FIFO_UD_MASK            (0x1U << MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_FIFO_UD_SHIFT)

/* Virtio Rx FIFO overflow. */
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_FIFO_OV_ACCESSTYPE      GENHDR_ACCESSTYPE_W1C
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_FIFO_OV_SHIFT           9
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_FIFO_OV_WIDTH           1
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_FIFO_OV_MASK            (0x1U << MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_FIFO_OV_SHIFT)

/* Virtio Shim Rx Ingress Port ID error. */
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_INGR_PORT_ID_ACCESSTYPE GENHDR_ACCESSTYPE_W1C
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_INGR_PORT_ID_SHIFT      10
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_INGR_PORT_ID_WIDTH      1
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_INGR_PORT_ID_MASK       (0x1U << MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_INGR_PORT_ID_SHIFT)

/* Virtio Shim Rx Ingress Missing Eop error. */
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_INGR_MISS_EOP_ACCESSTYPE GENHDR_ACCESSTYPE_W1C
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_INGR_MISS_EOP_SHIFT     11
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_INGR_MISS_EOP_WIDTH     1
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_INGR_MISS_EOP_MASK      (0x1U << MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_INGR_MISS_EOP_SHIFT)

/* Virtio Shim Rx Ingress Missing Sop error. */
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_INGR_MISS_SOP_ACCESSTYPE GENHDR_ACCESSTYPE_W1C
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_INGR_MISS_SOP_SHIFT     12
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_INGR_MISS_SOP_WIDTH     1
#define MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_INGR_MISS_SOP_MASK      (0x1U << MKS_VIOSHM_VIOSHM_ERROR_HOLD_RX_INGR_MISS_SOP_SHIFT)

#ifdef GENHDR_STRUCT
/* Virtio Shim error status hold */
typedef union {
  struct {
    uint32_t tx_ring_fifo_ud : 1;       /* Virtio Tx Ring FIFO underflow. */
    uint32_t tx_ring_fifo_ov : 1;       /* Virtio Tx Ring FIFO overflow. */
    uint32_t tx_fifo_ud : 1;            /* Virtio Tx FIFO underflow. */
    uint32_t tx_fifo_ov : 1;            /* Virtio Tx FIFO overflow. */
    uint32_t tx_ingr_port_id : 1;       /* Virtio Shim Tx Ingress Port ID error. */
    uint32_t tx_ingr_miss_eop : 1;      /* Virtio Shim Tx Ingress Missing Eop error. */
    uint32_t tx_ingr_miss_sop : 1;      /* Virtio Shim Tx Ingress Missing Sop error. */
    uint32_t : 1;
    uint32_t rx_fifo_ud : 1;            /* Virtio Rx FIFO underflow. */
    uint32_t rx_fifo_ov : 1;            /* Virtio Rx FIFO overflow. */
    uint32_t rx_ingr_port_id : 1;       /* Virtio Shim Rx Ingress Port ID error. */
    uint32_t rx_ingr_miss_eop : 1;      /* Virtio Shim Rx Ingress Missing Eop error. */
    uint32_t rx_ingr_miss_sop : 1;      /* Virtio Shim Rx Ingress Missing Sop error. */
    uint32_t : 19;
  } field;
  uint32_t val;
} MKS_VIOSHM_VIOSHM_ERROR_HOLD_t;
#endif /* GENHDR_STRUCT */

/* Virtio to Virtio Shim Rx stall counter */
#define MKS_VIOSHM_VIOSHM_RX_STALL_CNT                  0x00000018U
/*
 * Count number of cycles Virtio DMA stalls Virtio SHM. Virtio Shim has data to
 * send but Virtio DMA does not fetch the data.
 */
#define MKS_VIOSHM_VIOSHM_RX_STALL_CNT_CNT_SHIFT        0
#define MKS_VIOSHM_VIOSHM_RX_STALL_CNT_CNT_WIDTH        32
#define MKS_VIOSHM_VIOSHM_RX_STALL_CNT_CNT_MASK         (0xFFFFFFFFU << MKS_VIOSHM_VIOSHM_RX_STALL_CNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Virtio to Virtio Shim Rx stall counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Count number of cycles Virtio DMA stalls Virtio SHM. Virtio Shim has data to
                           send but Virtio DMA does not fetch the data. */
  } field;
  uint32_t val;
} MKS_VIOSHM_VIOSHM_RX_STALL_CNT_t;
#endif /* GENHDR_STRUCT */

/* Virtio Shim Rx Tx not ready counter */
#define MKS_VIOSHM_VIOSHM_TXNOTRDY_CNT                  0x0000001CU
/*
 * Count number of cycles VirIo Shim Rx stalls Modifier. The buffer in Virtio
 * Shim Rx is almost full and it back pressures the Modifier.
 */
#define MKS_VIOSHM_VIOSHM_TXNOTRDY_CNT_CNT_SHIFT        0
#define MKS_VIOSHM_VIOSHM_TXNOTRDY_CNT_CNT_WIDTH        32
#define MKS_VIOSHM_VIOSHM_TXNOTRDY_CNT_CNT_MASK         (0xFFFFFFFFU << MKS_VIOSHM_VIOSHM_TXNOTRDY_CNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Virtio Shim Rx Tx not ready counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Count number of cycles VirIo Shim Rx stalls Modifier. The buffer in Virtio
                           Shim Rx is almost full and it back pressures the Modifier. */
  } field;
  uint32_t val;
} MKS_VIOSHM_VIOSHM_TXNOTRDY_CNT_t;
#endif /* GENHDR_STRUCT */

/* Received FC buffer almost full threshold  */
#define MKS_VIOSHM_VIOSHM_RXFC_AFULL                            0x00000020U
/* received FC buffer almost full threshold. */
#define MKS_VIOSHM_VIOSHM_RXFC_AFULL_RX_FC_AF_THR_SHIFT         0
#define MKS_VIOSHM_VIOSHM_RXFC_AFULL_RX_FC_AF_THR_WIDTH         32
#define MKS_VIOSHM_VIOSHM_RXFC_AFULL_RX_FC_AF_THR_MASK          (0xFFFFFFFFU << MKS_VIOSHM_VIOSHM_RXFC_AFULL_RX_FC_AF_THR_SHIFT)

#ifdef GENHDR_STRUCT
/* Received FC buffer almost full threshold  */
typedef union {
  struct {
    uint32_t rx_fc_af_thr : 32; /* received FC buffer almost full threshold. */
  } field;
  uint32_t val;
} MKS_VIOSHM_VIOSHM_RXFC_AFULL_t;
#endif /* GENHDR_STRUCT */

/* VirtIo Queue ID mapping table access control */
#define MKS_VIOSHM_VIOSHM_CSR_CMD                       0x00000024U
/*
 * Multi Queue feature enable. 
 *  1: Enable Multi Queue feature. 
 *  0: Disable Multi Queue feature.
 */
#define MKS_VIOSHM_VIOSHM_CSR_CMD_CFG_QID_EN_SHIFT      24
#define MKS_VIOSHM_VIOSHM_CSR_CMD_CFG_QID_EN_WIDTH      1
#define MKS_VIOSHM_VIOSHM_CSR_CMD_CFG_QID_EN_MASK       (0x1U << MKS_VIOSHM_VIOSHM_CSR_CMD_CFG_QID_EN_SHIFT)

#ifdef GENHDR_STRUCT
/* VirtIo Queue ID mapping table access control */
typedef union {
  struct {
    uint32_t : 24;
    uint32_t cfg_qid_en : 1;    /* Multi Queue feature enable.
                                   1: Enable Multi Queue feature.
                                   0: Disable Multi Queue feature. */
    uint32_t : 7;
  } field;
  uint32_t val;
} MKS_VIOSHM_VIOSHM_CSR_CMD_t;
#endif /* GENHDR_STRUCT */

#endif /* _MKS_VIOSHM_REGISTERS_H_ */

