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
 * Date: 2020-12-02 10:38
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

#ifndef _MKS_TXQ_REGISTERS_H_
#define _MKS_TXQ_REGISTERS_H_

/* OVS Transmit Queue */

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

#define MKS_TXQ_ACCESSTYPE              GENHDR_ACCESSTYPE_RW /*  Default access type. Access types defined above. */
#define MKS_TXQ_REGWIDTH                32 /* Default width of register in bits */
#define MKS_TXQ_ACCESSWIDTH             32 /* Default width of access word in bit */


/* General purpose RW register intended for debug use */
#define MKS_TXQ_TXQ_SCRATCH                     0x00000000U
/* Scratch data. */
#define MKS_TXQ_TXQ_SCRATCH_DATA_SHIFT          0
#define MKS_TXQ_TXQ_SCRATCH_DATA_WIDTH          32
#define MKS_TXQ_TXQ_SCRATCH_DATA_MASK           (0xFFFFFFFFU << MKS_TXQ_TXQ_SCRATCH_DATA_SHIFT)

#ifdef GENHDR_STRUCT
/* General purpose RW register intended for debug use */
typedef union {
  struct {
    uint32_t data : 32; /* Scratch data. */
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_SCRATCH_t;
#endif /* GENHDR_STRUCT */

/*
 * TxQ Flow Control Command. This register is used to access the Flow Control
 * water mark can counter memories
 */
#define MKS_TXQ_TXQ_FC_CMD_CTL                                  0x00000004U
/*
 * Read enable: S/W sets this bit to initiate read cycle. H/W clears once read
 * cycle is done. 
 */
#define MKS_TXQ_TXQ_FC_CMD_CTL_RD_ENABLE_SHIFT                  0
#define MKS_TXQ_TXQ_FC_CMD_CTL_RD_ENABLE_WIDTH                  1
#define MKS_TXQ_TXQ_FC_CMD_CTL_RD_ENABLE_MASK                   (0x1U << MKS_TXQ_TXQ_FC_CMD_CTL_RD_ENABLE_SHIFT)

/*
 * Write enable: S/W sets this bit to initiate write cycle. H/W clears once
 * write cycle is done. 
 */
#define MKS_TXQ_TXQ_FC_CMD_CTL_WR_ENABLE_SHIFT                  1
#define MKS_TXQ_TXQ_FC_CMD_CTL_WR_ENABLE_WIDTH                  1
#define MKS_TXQ_TXQ_FC_CMD_CTL_WR_ENABLE_MASK                   (0x1U << MKS_TXQ_TXQ_FC_CMD_CTL_WR_ENABLE_SHIFT)

/* Block ID. */
#define MKS_TXQ_TXQ_FC_CMD_CTL_BLK_ID_SHIFT                     4
#define MKS_TXQ_TXQ_FC_CMD_CTL_BLK_ID_WIDTH                     4
#define MKS_TXQ_TXQ_FC_CMD_CTL_BLK_ID_MASK                      (0xFU << MKS_TXQ_TXQ_FC_CMD_CTL_BLK_ID_SHIFT)

/* Port ID . */
#define MKS_TXQ_TXQ_FC_CMD_CTL_PORT_ID_SHIFT                    8
#define MKS_TXQ_TXQ_FC_CMD_CTL_PORT_ID_WIDTH                    2
#define MKS_TXQ_TXQ_FC_CMD_CTL_PORT_ID_MASK                     (0x3U << MKS_TXQ_TXQ_FC_CMD_CTL_PORT_ID_SHIFT)

/* Priority ID . */
#define MKS_TXQ_TXQ_FC_CMD_CTL_PRIORITY_ID_SHIFT                10
#define MKS_TXQ_TXQ_FC_CMD_CTL_PRIORITY_ID_WIDTH                2
#define MKS_TXQ_TXQ_FC_CMD_CTL_PRIORITY_ID_MASK                 (0x3U << MKS_TXQ_TXQ_FC_CMD_CTL_PRIORITY_ID_SHIFT)

/*
 * Setting this bit will enable the stalling condition at the igress interfaces.
 * One bit per port. [0]: MAC0, [1]: MAC1, [2]: ARM: [3]: VirtIo Since MAC0,
 * MAC1, and ARM can not be stalled at the ingress interface therefore we should
 * not set bit [2:0] .
 */
#define MKS_TXQ_TXQ_FC_CMD_CTL_CFG_STALL_PORT_EN_SHIFT          12
#define MKS_TXQ_TXQ_FC_CMD_CTL_CFG_STALL_PORT_EN_WIDTH          4
#define MKS_TXQ_TXQ_FC_CMD_CTL_CFG_STALL_PORT_EN_MASK           (0xFU << MKS_TXQ_TXQ_FC_CMD_CTL_CFG_STALL_PORT_EN_SHIFT)

/*
 * Global stall threshold. When the PM occupancy greater than this
 *  // AD FIXED 
 *  03.23.2018 threshold asserts flow control at the Ingress VirtIo interface.
 */
#define MKS_TXQ_TXQ_FC_CMD_CTL_GLOBAL_STALL_THR_SHIFT           16
#define MKS_TXQ_TXQ_FC_CMD_CTL_GLOBAL_STALL_THR_WIDTH           12
#define MKS_TXQ_TXQ_FC_CMD_CTL_GLOBAL_STALL_THR_MASK            (0xFFFU << MKS_TXQ_TXQ_FC_CMD_CTL_GLOBAL_STALL_THR_SHIFT)

/*
 * Global stall threshold enable. Setting this bit will enable global stall at
 * the Ingress VirtIo interface.
 */
#define MKS_TXQ_TXQ_FC_CMD_CTL_CFG_GLOBAL_STALL_EN_SHIFT        28
#define MKS_TXQ_TXQ_FC_CMD_CTL_CFG_GLOBAL_STALL_EN_WIDTH        1
#define MKS_TXQ_TXQ_FC_CMD_CTL_CFG_GLOBAL_STALL_EN_MASK         (0x1U << MKS_TXQ_TXQ_FC_CMD_CTL_CFG_GLOBAL_STALL_EN_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * TxQ Flow Control Command. This register is used to access the Flow Control
 * water mark can counter memories
 */
typedef union {
  struct {
    uint32_t rd_enable : 1;             /* Read enable: S/W sets this bit to initiate read cycle. H/W clears once read
                                           cycle is done. */
    uint32_t wr_enable : 1;             /* Write enable: S/W sets this bit to initiate write cycle. H/W clears once
                                           write cycle is done. */
    uint32_t : 2;
    uint32_t blk_id : 4;                /* Block ID. */
    uint32_t port_id : 2;               /* Port ID . */
    uint32_t priority_id : 2;           /* Priority ID . */
    uint32_t cfg_stall_port_en : 4;     /* Setting this bit will enable the stalling condition at the igress interfaces.
                                           One bit per port. [0]: MAC0, [1]: MAC1, [2]: ARM: [3]: VirtIo Since MAC0,
                                           MAC1, and ARM can not be stalled at the ingress interface therefore we should
                                           not set bit [2:0] . */
    uint32_t global_stall_thr : 12;     /* Global stall threshold. When the PM occupancy greater than this
                                           // AD FIXED
                                           03.23.2018 threshold asserts flow control at the Ingress VirtIo interface. */
    uint32_t cfg_global_stall_en : 1;   /* Global stall threshold enable. Setting this bit will enable global stall at
                                           the Ingress VirtIo interface. */
    uint32_t : 3;
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_FC_CMD_CTL_t;
#endif /* GENHDR_STRUCT */

#define MKS_TXQ_TXQ_FC_CMD_CTL_BLK_ID_ING_STALL_WM              0x0U /* Ingress Stall Water Mark, 12 WM (per priority/rx_port). */
#define MKS_TXQ_TXQ_FC_CMD_CTL_BLK_ID_ING_DROP_WM               0x1U /* Ingress Drop Water Mark, 12 WM (per priority/rx_port. */
#define MKS_TXQ_TXQ_FC_CMD_CTL_BLK_ID_EGR_WM                    0x2U /* Egress Water Mark, 12 WM (per priority/tx_port). */
#define MKS_TXQ_TXQ_FC_CMD_CTL_BLK_ID_EGR_CLS_WM                0x3U /* Egress Class Water Mark, 3 WM (per priority). */
#define MKS_TXQ_TXQ_FC_CMD_CTL_BLK_ID_INGR_STL_OC               0x4U /* Ingress Stall Useage, 12 counters (priority/rx_port). */
#define MKS_TXQ_TXQ_FC_CMD_CTL_BLK_ID_EGR_OCC                   0x5U /* Egress Useage, 12 counters (priority/tx_port). */
#define MKS_TXQ_TXQ_FC_CMD_CTL_BLK_ID_EGR_CLAS_OC               0x6U /* Egress Class Useage, 3 counters (priority). */
#define MKS_TXQ_TXQ_FC_CMD_CTL_BLK_ID_ING_STL_DCNT              0x7U /* Ingress Stall Drop Count, 12 counters (priority/rx_port). */
#define MKS_TXQ_TXQ_FC_CMD_CTL_BLK_ID_EGR_DROP_CNT              0x8U /* Egress Drop Count, 12 counters (priority/tx_port). */

#define MKS_TXQ_TXQ_FC_CMD_CTL_PORT_ID_PORT_0                   0x0U /* MAC0 port. */
#define MKS_TXQ_TXQ_FC_CMD_CTL_PORT_ID_PORT_1                   0x1U /* MAC1 port. */
#define MKS_TXQ_TXQ_FC_CMD_CTL_PORT_ID_PORT_2                   0x2U /* ARM port. */
#define MKS_TXQ_TXQ_FC_CMD_CTL_PORT_ID_PORT_3                   0x3U /* VirtIO port. */

#define MKS_TXQ_TXQ_FC_CMD_CTL_CLASS_ID_CLASS_LOW               0x0U /* Low priority. */
#define MKS_TXQ_TXQ_FC_CMD_CTL_CLASS_ID_CLASS_MED               0x1U /* Medimum priority. */
#define MKS_TXQ_TXQ_FC_CMD_CTL_CLASS_ID_CLASS_HI                0x2U /* High priority. */

/*
 * This register contains bit [31:0] of FC command read data or FC command write
 * data
 */
#define MKS_TXQ_TXQ_FC_DATA                             0x00000008U
/*
 * This register contains bit [31:0] of FC command read data or FC command write
 * data .
 */
#define MKS_TXQ_TXQ_FC_DATA_FC_DATA_REG_SHIFT           0
#define MKS_TXQ_TXQ_FC_DATA_FC_DATA_REG_WIDTH           32
#define MKS_TXQ_TXQ_FC_DATA_FC_DATA_REG_MASK            (0xFFFFFFFFU << MKS_TXQ_TXQ_FC_DATA_FC_DATA_REG_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * This register contains bit [31:0] of FC command read data or FC command write
 * data
 */
typedef union {
  struct {
    uint32_t fc_data_reg : 32;  /* This register contains bit [31:0] of FC command read data or FC command write
                                   data . */
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_FC_DATA_t;
#endif /* GENHDR_STRUCT */

/*
 * TxQueue packet counter: Counts number of pkts transmitted to TxSch.
 * TXQ_PKT_CNT = TXQ_EGRPORT_PKT_CNT[0] + TXQ_EGRPORT_PKT_CNT[1] +
 * TXQ_EGRPORT_PKT_CNT[2] + TXQ_EGRPORT_PKT_CNT[3] 
 */
#define MKS_TXQ_TXQ_PKT_CNT                     0x0000000CU
/* TxQueue packet counter. */
#define MKS_TXQ_TXQ_PKT_CNT_CNT_SHIFT           0
#define MKS_TXQ_TXQ_PKT_CNT_CNT_WIDTH           32
#define MKS_TXQ_TXQ_PKT_CNT_CNT_MASK            (0xFFFFFFFFU << MKS_TXQ_TXQ_PKT_CNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * TxQueue packet counter: Counts number of pkts transmitted to TxSch.
 * TXQ_PKT_CNT = TXQ_EGRPORT_PKT_CNT[0] + TXQ_EGRPORT_PKT_CNT[1] +
 * TXQ_EGRPORT_PKT_CNT[2] + TXQ_EGRPORT_PKT_CNT[3] 
 */
typedef union {
  struct {
    uint32_t cnt : 32;  /* TxQueue packet counter. */
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_PKT_CNT_t;
#endif /* GENHDR_STRUCT */

/*
 * TxQueue 2 PM drop counter: Counts number packets drop due TxQ Flow Control or
 * Default Lookup drop. TXQ_2PM_DROP_CNT = TXQ_LKUP_DROP_CNT +
 * TXQ_INGRFC_DROP_CNT + TXQ_EGRFC_DROP_CNT 
 */
#define MKS_TXQ_TXQ_2_PM_DROP_CNT               0x00000010U
/* Total number of packet being dropped. */
#define MKS_TXQ_TXQ_2_PM_DROP_CNT_CNT_SHIFT     0
#define MKS_TXQ_TXQ_2_PM_DROP_CNT_CNT_WIDTH     32
#define MKS_TXQ_TXQ_2_PM_DROP_CNT_CNT_MASK      (0xFFFFFFFFU << MKS_TXQ_TXQ_2_PM_DROP_CNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * TxQueue 2 PM drop counter: Counts number packets drop due TxQ Flow Control or
 * Default Lookup drop. TXQ_2PM_DROP_CNT = TXQ_LKUP_DROP_CNT +
 * TXQ_INGRFC_DROP_CNT + TXQ_EGRFC_DROP_CNT 
 */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Total number of packet being dropped. */
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_2_PM_DROP_CNT_t;
#endif /* GENHDR_STRUCT */

/* TxQ Flow control status */
#define MKS_TXQ_TXQ_FC_STAT                             0x00000014U
/*
 * Indicates the ingress drop status per priority per rx port.
 *  [3:0]: Low Priority {VirtIo, ARM, MAC1, MAC0}.
 *  [7:4]: Med Priority {VirtIo, ARM, MAC1, MAC0}.
 *  [11:8]: Hi Priority {VirtIo, ARM, MAC1, MAC0}. 
 */
#define MKS_TXQ_TXQ_FC_STAT_INGR_DROP_STAT_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define MKS_TXQ_TXQ_FC_STAT_INGR_DROP_STAT_SHIFT        0
#define MKS_TXQ_TXQ_FC_STAT_INGR_DROP_STAT_WIDTH        12
#define MKS_TXQ_TXQ_FC_STAT_INGR_DROP_STAT_MASK         (0xFFFU << MKS_TXQ_TXQ_FC_STAT_INGR_DROP_STAT_SHIFT)

/* Indicates the stall status of the tx port [3:0]: {VirtIo, ARM, MAC1, MAC0. */
#define MKS_TXQ_TXQ_FC_STAT_EGR_STALL_STAT_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define MKS_TXQ_TXQ_FC_STAT_EGR_STALL_STAT_SHIFT        12
#define MKS_TXQ_TXQ_FC_STAT_EGR_STALL_STAT_WIDTH        4
#define MKS_TXQ_TXQ_FC_STAT_EGR_STALL_STAT_MASK         (0xFU << MKS_TXQ_TXQ_FC_STAT_EGR_STALL_STAT_SHIFT)

/*
 * Indicates the egress drop status per priority per tx port.
 *  [3:0]: Low Priority {VirtIo, ARM, MAC1, MAC0}.
 *  [7:4]: Med Priority {VirtIo, ARM, MAC1, MAC0}.
 *  [11:8]: Hi Priority {VirtIo, ARM, MAC1, MAC0}. 
 */
#define MKS_TXQ_TXQ_FC_STAT_EGR_DROP_STAT_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define MKS_TXQ_TXQ_FC_STAT_EGR_DROP_STAT_SHIFT         16
#define MKS_TXQ_TXQ_FC_STAT_EGR_DROP_STAT_WIDTH         12
#define MKS_TXQ_TXQ_FC_STAT_EGR_DROP_STAT_MASK          (0xFFFU << MKS_TXQ_TXQ_FC_STAT_EGR_DROP_STAT_SHIFT)

#ifdef GENHDR_STRUCT
/* TxQ Flow control status */
typedef union {
  struct {
    uint32_t ingr_drop_stat : 12;       /* Indicates the ingress drop status per priority per rx port.
                                           [3:0]: Low Priority {VirtIo, ARM, MAC1, MAC0}.
                                           [7:4]: Med Priority {VirtIo, ARM, MAC1, MAC0}.
                                           [11:8]: Hi Priority {VirtIo, ARM, MAC1, MAC0}. */
    uint32_t egr_stall_stat : 4;        /* Indicates the stall status of the tx port [3:0]: {VirtIo, ARM, MAC1, MAC0. */
    uint32_t egr_drop_stat : 12;        /* Indicates the egress drop status per priority per tx port.
                                           [3:0]: Low Priority {VirtIo, ARM, MAC1, MAC0}.
                                           [7:4]: Med Priority {VirtIo, ARM, MAC1, MAC0}.
                                           [11:8]: Hi Priority {VirtIo, ARM, MAC1, MAC0}. */
    uint32_t : 4;
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_FC_STAT_t;
#endif /* GENHDR_STRUCT */

/* Queue 0 and 1 usage */
#define MKS_TXQ_TXQ_QUSAGE_0_1                          0x00000018U
/*
 * Queue0 useage (MAC0 low priority). Indicates number of segments are 
 *  being used. 
 */
#define MKS_TXQ_TXQ_QUSAGE_0_1_Q_USAGE_0_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_TXQ_TXQ_QUSAGE_0_1_Q_USAGE_0_SHIFT          0
#define MKS_TXQ_TXQ_QUSAGE_0_1_Q_USAGE_0_WIDTH          12
#define MKS_TXQ_TXQ_QUSAGE_0_1_Q_USAGE_0_MASK           (0xFFFU << MKS_TXQ_TXQ_QUSAGE_0_1_Q_USAGE_0_SHIFT)

/*
 * Queue1 useage (MAC1 low priority). Indicates number of segments are 
 *  being used. 
 */
#define MKS_TXQ_TXQ_QUSAGE_0_1_Q_USAGE_1_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_TXQ_TXQ_QUSAGE_0_1_Q_USAGE_1_SHIFT          16
#define MKS_TXQ_TXQ_QUSAGE_0_1_Q_USAGE_1_WIDTH          12
#define MKS_TXQ_TXQ_QUSAGE_0_1_Q_USAGE_1_MASK           (0xFFFU << MKS_TXQ_TXQ_QUSAGE_0_1_Q_USAGE_1_SHIFT)

#ifdef GENHDR_STRUCT
/* Queue 0 and 1 usage */
typedef union {
  struct {
    uint32_t q_usage_0 : 12;    /* Queue0 useage (MAC0 low priority). Indicates number of segments are
                                   being used. */
    uint32_t : 4;
    uint32_t q_usage_1 : 12;    /* Queue1 useage (MAC1 low priority). Indicates number of segments are
                                   being used. */
    uint32_t : 4;
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_QUSAGE_0_1_t;
#endif /* GENHDR_STRUCT */

/* Queue 2 and 3 usage */
#define MKS_TXQ_TXQ_QUSAGE_2_3                          0x0000001CU
/*
 * Queue2 useage (ARM low priority). Indicates number of segments are 
 *  being used. 
 */
#define MKS_TXQ_TXQ_QUSAGE_2_3_Q_USAGE_2_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_TXQ_TXQ_QUSAGE_2_3_Q_USAGE_2_SHIFT          0
#define MKS_TXQ_TXQ_QUSAGE_2_3_Q_USAGE_2_WIDTH          12
#define MKS_TXQ_TXQ_QUSAGE_2_3_Q_USAGE_2_MASK           (0xFFFU << MKS_TXQ_TXQ_QUSAGE_2_3_Q_USAGE_2_SHIFT)

/*
 * Queue3 useage (VirtIo low priority). Indicates number of segments are 
 *  being used. 
 */
#define MKS_TXQ_TXQ_QUSAGE_2_3_Q_USAGE_3_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_TXQ_TXQ_QUSAGE_2_3_Q_USAGE_3_SHIFT          16
#define MKS_TXQ_TXQ_QUSAGE_2_3_Q_USAGE_3_WIDTH          12
#define MKS_TXQ_TXQ_QUSAGE_2_3_Q_USAGE_3_MASK           (0xFFFU << MKS_TXQ_TXQ_QUSAGE_2_3_Q_USAGE_3_SHIFT)

#ifdef GENHDR_STRUCT
/* Queue 2 and 3 usage */
typedef union {
  struct {
    uint32_t q_usage_2 : 12;    /* Queue2 useage (ARM low priority). Indicates number of segments are
                                   being used. */
    uint32_t : 4;
    uint32_t q_usage_3 : 12;    /* Queue3 useage (VirtIo low priority). Indicates number of segments are
                                   being used. */
    uint32_t : 4;
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_QUSAGE_2_3_t;
#endif /* GENHDR_STRUCT */

/* Queue 4 and 5 usage */
#define MKS_TXQ_TXQ_QUSAGE_4_5                          0x00000020U
/*
 * Queue4 useage (MAC0 high priority). Indicates number of segments are 
 *  being used. 
 */
#define MKS_TXQ_TXQ_QUSAGE_4_5_Q_USAGE_4_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_TXQ_TXQ_QUSAGE_4_5_Q_USAGE_4_SHIFT          0
#define MKS_TXQ_TXQ_QUSAGE_4_5_Q_USAGE_4_WIDTH          12
#define MKS_TXQ_TXQ_QUSAGE_4_5_Q_USAGE_4_MASK           (0xFFFU << MKS_TXQ_TXQ_QUSAGE_4_5_Q_USAGE_4_SHIFT)

/*
 * Queue5 useage (MAC1 high priority). Indicates number of segments are 
 *  being used. 
 */
#define MKS_TXQ_TXQ_QUSAGE_4_5_Q_USAGE_5_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_TXQ_TXQ_QUSAGE_4_5_Q_USAGE_5_SHIFT          16
#define MKS_TXQ_TXQ_QUSAGE_4_5_Q_USAGE_5_WIDTH          12
#define MKS_TXQ_TXQ_QUSAGE_4_5_Q_USAGE_5_MASK           (0xFFFU << MKS_TXQ_TXQ_QUSAGE_4_5_Q_USAGE_5_SHIFT)

#ifdef GENHDR_STRUCT
/* Queue 4 and 5 usage */
typedef union {
  struct {
    uint32_t q_usage_4 : 12;    /* Queue4 useage (MAC0 high priority). Indicates number of segments are
                                   being used. */
    uint32_t : 4;
    uint32_t q_usage_5 : 12;    /* Queue5 useage (MAC1 high priority). Indicates number of segments are
                                   being used. */
    uint32_t : 4;
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_QUSAGE_4_5_t;
#endif /* GENHDR_STRUCT */

/* Queue 6 and 7 usage */
#define MKS_TXQ_TXQ_QUSAGE_6_7                          0x00000024U
/*
 * Queue6 useage (ARM high priority). Indicates number of segments are 
 *  being used. 
 */
#define MKS_TXQ_TXQ_QUSAGE_6_7_Q_USAGE_6_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_TXQ_TXQ_QUSAGE_6_7_Q_USAGE_6_SHIFT          0
#define MKS_TXQ_TXQ_QUSAGE_6_7_Q_USAGE_6_WIDTH          12
#define MKS_TXQ_TXQ_QUSAGE_6_7_Q_USAGE_6_MASK           (0xFFFU << MKS_TXQ_TXQ_QUSAGE_6_7_Q_USAGE_6_SHIFT)

/*
 * Queue7 useage (VirtIo high priority). Indicates number of segments are 
 *  being used. 
 */
#define MKS_TXQ_TXQ_QUSAGE_6_7_Q_USAGE_7_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_TXQ_TXQ_QUSAGE_6_7_Q_USAGE_7_SHIFT          16
#define MKS_TXQ_TXQ_QUSAGE_6_7_Q_USAGE_7_WIDTH          12
#define MKS_TXQ_TXQ_QUSAGE_6_7_Q_USAGE_7_MASK           (0xFFFU << MKS_TXQ_TXQ_QUSAGE_6_7_Q_USAGE_7_SHIFT)

#ifdef GENHDR_STRUCT
/* Queue 6 and 7 usage */
typedef union {
  struct {
    uint32_t q_usage_6 : 12;    /* Queue6 useage (ARM high priority). Indicates number of segments are
                                   being used. */
    uint32_t : 4;
    uint32_t q_usage_7 : 12;    /* Queue7 useage (VirtIo high priority). Indicates number of segments are
                                   being used. */
    uint32_t : 4;
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_QUSAGE_6_7_t;
#endif /* GENHDR_STRUCT */

/* Queue 0 and 1 max usage */
#define MKS_TXQ_TXQ_QUSAGE_MAX_0_1                              0x00000028U
/*
 * Queue0 useage (Mac0 low priority). Indicates max number of segments are 
 *  being used. 
 */
#define MKS_TXQ_TXQ_QUSAGE_MAX_0_1_Q_USAGE_MAX_0_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_TXQ_TXQ_QUSAGE_MAX_0_1_Q_USAGE_MAX_0_SHIFT          0
#define MKS_TXQ_TXQ_QUSAGE_MAX_0_1_Q_USAGE_MAX_0_WIDTH          12
#define MKS_TXQ_TXQ_QUSAGE_MAX_0_1_Q_USAGE_MAX_0_MASK           (0xFFFU << MKS_TXQ_TXQ_QUSAGE_MAX_0_1_Q_USAGE_MAX_0_SHIFT)

/*
 * Queue1 useage (Mac1 low priority). Indicates max number of segments are 
 *  being used. 
 */
#define MKS_TXQ_TXQ_QUSAGE_MAX_0_1_Q_USAGE_MAX_1_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_TXQ_TXQ_QUSAGE_MAX_0_1_Q_USAGE_MAX_1_SHIFT          16
#define MKS_TXQ_TXQ_QUSAGE_MAX_0_1_Q_USAGE_MAX_1_WIDTH          12
#define MKS_TXQ_TXQ_QUSAGE_MAX_0_1_Q_USAGE_MAX_1_MASK           (0xFFFU << MKS_TXQ_TXQ_QUSAGE_MAX_0_1_Q_USAGE_MAX_1_SHIFT)

#ifdef GENHDR_STRUCT
/* Queue 0 and 1 max usage */
typedef union {
  struct {
    uint32_t q_usage_max_0 : 12;        /* Queue0 useage (Mac0 low priority). Indicates max number of segments are
                                           being used. */
    uint32_t : 4;
    uint32_t q_usage_max_1 : 12;        /* Queue1 useage (Mac1 low priority). Indicates max number of segments are
                                           being used. */
    uint32_t : 4;
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_QUSAGE_MAX_0_1_t;
#endif /* GENHDR_STRUCT */

/* Queue 2 and 2 max usage */
#define MKS_TXQ_TXQ_QUSAGE_MAX_2_3                              0x0000002CU
/*
 * Queue2 useage (Arm low priority). Indicates max number of segments are 
 *  being used. 
 */
#define MKS_TXQ_TXQ_QUSAGE_MAX_2_3_Q_USAGE_MAX_2_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_TXQ_TXQ_QUSAGE_MAX_2_3_Q_USAGE_MAX_2_SHIFT          0
#define MKS_TXQ_TXQ_QUSAGE_MAX_2_3_Q_USAGE_MAX_2_WIDTH          12
#define MKS_TXQ_TXQ_QUSAGE_MAX_2_3_Q_USAGE_MAX_2_MASK           (0xFFFU << MKS_TXQ_TXQ_QUSAGE_MAX_2_3_Q_USAGE_MAX_2_SHIFT)

/*
 * Queue3 useage (VirtIo low priority). Indicates max number of segments are 
 *  being used. 
 */
#define MKS_TXQ_TXQ_QUSAGE_MAX_2_3_Q_USAGE_MAX_3_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_TXQ_TXQ_QUSAGE_MAX_2_3_Q_USAGE_MAX_3_SHIFT          16
#define MKS_TXQ_TXQ_QUSAGE_MAX_2_3_Q_USAGE_MAX_3_WIDTH          12
#define MKS_TXQ_TXQ_QUSAGE_MAX_2_3_Q_USAGE_MAX_3_MASK           (0xFFFU << MKS_TXQ_TXQ_QUSAGE_MAX_2_3_Q_USAGE_MAX_3_SHIFT)

#ifdef GENHDR_STRUCT
/* Queue 2 and 2 max usage */
typedef union {
  struct {
    uint32_t q_usage_max_2 : 12;        /* Queue2 useage (Arm low priority). Indicates max number of segments are
                                           being used. */
    uint32_t : 4;
    uint32_t q_usage_max_3 : 12;        /* Queue3 useage (VirtIo low priority). Indicates max number of segments are
                                           being used. */
    uint32_t : 4;
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_QUSAGE_MAX_2_3_t;
#endif /* GENHDR_STRUCT */

/* Queue 4 and 5 max usage */
#define MKS_TXQ_TXQ_QUSAGE_MAX_4_5                              0x00000030U
/*
 * Queue4 useage (Mac0 high priority). Indicates max number of segments are 
 *  being used. 
 */
#define MKS_TXQ_TXQ_QUSAGE_MAX_4_5_Q_USAGE_MAX_4_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_TXQ_TXQ_QUSAGE_MAX_4_5_Q_USAGE_MAX_4_SHIFT          0
#define MKS_TXQ_TXQ_QUSAGE_MAX_4_5_Q_USAGE_MAX_4_WIDTH          12
#define MKS_TXQ_TXQ_QUSAGE_MAX_4_5_Q_USAGE_MAX_4_MASK           (0xFFFU << MKS_TXQ_TXQ_QUSAGE_MAX_4_5_Q_USAGE_MAX_4_SHIFT)

/*
 * Queue5 useage (Mac1 high priority). Indicates max number of segments are 
 *  being used. 
 */
#define MKS_TXQ_TXQ_QUSAGE_MAX_4_5_Q_USAGE_MAX_5_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_TXQ_TXQ_QUSAGE_MAX_4_5_Q_USAGE_MAX_5_SHIFT          16
#define MKS_TXQ_TXQ_QUSAGE_MAX_4_5_Q_USAGE_MAX_5_WIDTH          12
#define MKS_TXQ_TXQ_QUSAGE_MAX_4_5_Q_USAGE_MAX_5_MASK           (0xFFFU << MKS_TXQ_TXQ_QUSAGE_MAX_4_5_Q_USAGE_MAX_5_SHIFT)

#ifdef GENHDR_STRUCT
/* Queue 4 and 5 max usage */
typedef union {
  struct {
    uint32_t q_usage_max_4 : 12;        /* Queue4 useage (Mac0 high priority). Indicates max number of segments are
                                           being used. */
    uint32_t : 4;
    uint32_t q_usage_max_5 : 12;        /* Queue5 useage (Mac1 high priority). Indicates max number of segments are
                                           being used. */
    uint32_t : 4;
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_QUSAGE_MAX_4_5_t;
#endif /* GENHDR_STRUCT */

/* Queue 6 and 7 max usage */
#define MKS_TXQ_TXQ_QUSAGE_MAX_6_7                              0x00000034U
/*
 * Queue6 useage (Arm high priority). Indicates max number of segments are 
 *  being used. 
 */
#define MKS_TXQ_TXQ_QUSAGE_MAX_6_7_Q_USAGE_MAX_6_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_TXQ_TXQ_QUSAGE_MAX_6_7_Q_USAGE_MAX_6_SHIFT          0
#define MKS_TXQ_TXQ_QUSAGE_MAX_6_7_Q_USAGE_MAX_6_WIDTH          12
#define MKS_TXQ_TXQ_QUSAGE_MAX_6_7_Q_USAGE_MAX_6_MASK           (0xFFFU << MKS_TXQ_TXQ_QUSAGE_MAX_6_7_Q_USAGE_MAX_6_SHIFT)

/*
 * Queue7 useage (VirtIo high priority). Indicates max number of segments are 
 *  being used. 
 */
#define MKS_TXQ_TXQ_QUSAGE_MAX_6_7_Q_USAGE_MAX_7_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_TXQ_TXQ_QUSAGE_MAX_6_7_Q_USAGE_MAX_7_SHIFT          16
#define MKS_TXQ_TXQ_QUSAGE_MAX_6_7_Q_USAGE_MAX_7_WIDTH          12
#define MKS_TXQ_TXQ_QUSAGE_MAX_6_7_Q_USAGE_MAX_7_MASK           (0xFFFU << MKS_TXQ_TXQ_QUSAGE_MAX_6_7_Q_USAGE_MAX_7_SHIFT)

#ifdef GENHDR_STRUCT
/* Queue 6 and 7 max usage */
typedef union {
  struct {
    uint32_t q_usage_max_6 : 12;        /* Queue6 useage (Arm high priority). Indicates max number of segments are
                                           being used. */
    uint32_t : 4;
    uint32_t q_usage_max_7 : 12;        /* Queue7 useage (VirtIo high priority). Indicates max number of segments are
                                           being used. */
    uint32_t : 4;
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_QUSAGE_MAX_6_7_t;
#endif /* GENHDR_STRUCT */

/* Flow Control Configuration */
#define MKS_TXQ_TXQ_FC_CFG                                      0x00000038U
/* Enable PLD FIFO full to stall VirtIo ingress interface. */
#define MKS_TXQ_TXQ_FC_CFG_CFG_PLD_FIF_STL_EN_SHIFT             0
#define MKS_TXQ_TXQ_FC_CFG_CFG_PLD_FIF_STL_EN_WIDTH             1
#define MKS_TXQ_TXQ_FC_CFG_CFG_PLD_FIF_STL_EN_MASK              (0x1U << MKS_TXQ_TXQ_FC_CFG_CFG_PLD_FIF_STL_EN_SHIFT)

/* Disable ingress drop. */
#define MKS_TXQ_TXQ_FC_CFG_CFG_DIS_IGR_DRP_EN_SHIFT             1
#define MKS_TXQ_TXQ_FC_CFG_CFG_DIS_IGR_DRP_EN_WIDTH             1
#define MKS_TXQ_TXQ_FC_CFG_CFG_DIS_IGR_DRP_EN_MASK              (0x1U << MKS_TXQ_TXQ_FC_CFG_CFG_DIS_IGR_DRP_EN_SHIFT)

/* Disable egress drop. */
#define MKS_TXQ_TXQ_FC_CFG_CFG_DIS_EGR_DRP_EN_SHIFT             2
#define MKS_TXQ_TXQ_FC_CFG_CFG_DIS_EGR_DRP_EN_WIDTH             1
#define MKS_TXQ_TXQ_FC_CFG_CFG_DIS_EGR_DRP_EN_MASK              (0x1U << MKS_TXQ_TXQ_FC_CFG_CFG_DIS_EGR_DRP_EN_SHIFT)

/* Disable ingress stall. */
#define MKS_TXQ_TXQ_FC_CFG_CFG_DIS_IGR_STLL_EN_SHIFT            3
#define MKS_TXQ_TXQ_FC_CFG_CFG_DIS_IGR_STLL_EN_WIDTH            1
#define MKS_TXQ_TXQ_FC_CFG_CFG_DIS_IGR_STLL_EN_MASK             (0x1U << MKS_TXQ_TXQ_FC_CFG_CFG_DIS_IGR_STLL_EN_SHIFT)

/*
 * Setting this bit will enable the dropping condition at the igress interfaces.
 * One bit per port. [0]: MAC0, [1]: MAC1, [2]: ARM: [3]: VirtIo.
 */
#define MKS_TXQ_TXQ_FC_CFG_CFG_IG_DR_PRT_EN_SHIFT               4
#define MKS_TXQ_TXQ_FC_CFG_CFG_IG_DR_PRT_EN_WIDTH               4
#define MKS_TXQ_TXQ_FC_CFG_CFG_IG_DR_PRT_EN_MASK                (0xFU << MKS_TXQ_TXQ_FC_CFG_CFG_IG_DR_PRT_EN_SHIFT)

#ifdef GENHDR_STRUCT
/* Flow Control Configuration */
typedef union {
  struct {
    uint32_t cfg_pld_fif_stl_en : 1;    /* Enable PLD FIFO full to stall VirtIo ingress interface. */
    uint32_t cfg_dis_igr_drp_en : 1;    /* Disable ingress drop. */
    uint32_t cfg_dis_egr_drp_en : 1;    /* Disable egress drop. */
    uint32_t cfg_dis_igr_stll_en : 1;   /* Disable ingress stall. */
    uint32_t cfg_ig_dr_prt_en : 4;      /* Setting this bit will enable the dropping condition at the igress interfaces.
                                           One bit per port. [0]: MAC0, [1]: MAC1, [2]: ARM: [3]: VirtIo. */
    uint32_t : 24;
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_FC_CFG_t;
#endif /* GENHDR_STRUCT */

/*
 * Port packet counter: Counts number of egress packets TxQ have transmitted to
 * TxSch. One counter per egress port
 */
#define MKS_TXQ_TXQ_EPRT_PKTCNT_0               0x0000003CU
/* Port packet counter. */
#define MKS_TXQ_TXQ_EPRT_PKTCNT_0_CNT_SHIFT     0
#define MKS_TXQ_TXQ_EPRT_PKTCNT_0_CNT_WIDTH     32
#define MKS_TXQ_TXQ_EPRT_PKTCNT_0_CNT_MASK      (0xFFFFFFFFU << MKS_TXQ_TXQ_EPRT_PKTCNT_0_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Port packet counter: Counts number of egress packets TxQ have transmitted to
 * TxSch. One counter per egress port
 */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Port packet counter. */
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_EPRT_PKTCNT_0_t;
#endif /* GENHDR_STRUCT */

/*
 * Port packet counter: Counts number of egress packets TxQ have transmitted to
 * TxSch. One counter per egress port
 */
#define MKS_TXQ_TXQ_EPRT_PKTCNT_1               0x00000040U
/* Port packet counter. */
#define MKS_TXQ_TXQ_EPRT_PKTCNT_1_CNT_SHIFT     0
#define MKS_TXQ_TXQ_EPRT_PKTCNT_1_CNT_WIDTH     32
#define MKS_TXQ_TXQ_EPRT_PKTCNT_1_CNT_MASK      (0xFFFFFFFFU << MKS_TXQ_TXQ_EPRT_PKTCNT_1_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Port packet counter: Counts number of egress packets TxQ have transmitted to
 * TxSch. One counter per egress port
 */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Port packet counter. */
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_EPRT_PKTCNT_1_t;
#endif /* GENHDR_STRUCT */

/*
 * Port packet counter: Counts number of egress packets TxQ have transmitted to
 * TxSch. One counter per egress port
 */
#define MKS_TXQ_TXQ_EPRT_PKTCNT_2               0x00000044U
/* Port packet counter. */
#define MKS_TXQ_TXQ_EPRT_PKTCNT_2_CNT_SHIFT     0
#define MKS_TXQ_TXQ_EPRT_PKTCNT_2_CNT_WIDTH     32
#define MKS_TXQ_TXQ_EPRT_PKTCNT_2_CNT_MASK      (0xFFFFFFFFU << MKS_TXQ_TXQ_EPRT_PKTCNT_2_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Port packet counter: Counts number of egress packets TxQ have transmitted to
 * TxSch. One counter per egress port
 */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Port packet counter. */
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_EPRT_PKTCNT_2_t;
#endif /* GENHDR_STRUCT */

/*
 * Port packet counter: Counts number of egress packets TxQ have transmitted to
 * TxSch. One counter per egress port
 */
#define MKS_TXQ_TXQ_EPRT_PKTCNT_3               0x00000048U
/* Port packet counter. */
#define MKS_TXQ_TXQ_EPRT_PKTCNT_3_CNT_SHIFT     0
#define MKS_TXQ_TXQ_EPRT_PKTCNT_3_CNT_WIDTH     32
#define MKS_TXQ_TXQ_EPRT_PKTCNT_3_CNT_MASK      (0xFFFFFFFFU << MKS_TXQ_TXQ_EPRT_PKTCNT_3_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Port packet counter: Counts number of egress packets TxQ have transmitted to
 * TxSch. One counter per egress port
 */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Port packet counter. */
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_EPRT_PKTCNT_3_t;
#endif /* GENHDR_STRUCT */

/* Lkup drop counter */
#define MKS_TXQ_TXQ_LKUP_DROP_CNT               0x0000004CU
/* Counts number of drop packets due to lookup drop indication. */
#define MKS_TXQ_TXQ_LKUP_DROP_CNT_CNT_SHIFT     0
#define MKS_TXQ_TXQ_LKUP_DROP_CNT_CNT_WIDTH     32
#define MKS_TXQ_TXQ_LKUP_DROP_CNT_CNT_MASK      (0xFFFFFFFFU << MKS_TXQ_TXQ_LKUP_DROP_CNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Lkup drop counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number of drop packets due to lookup drop indication. */
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_LKUP_DROP_CNT_t;
#endif /* GENHDR_STRUCT */

/* Ingress FC drop counter */
#define MKS_TXQ_TXQ_INGRFC_DROP_CNT                     0x00000050U
/* Counts number of drop packets due ingress FC drop indication. */
#define MKS_TXQ_TXQ_INGRFC_DROP_CNT_CNT_SHIFT           0
#define MKS_TXQ_TXQ_INGRFC_DROP_CNT_CNT_WIDTH           32
#define MKS_TXQ_TXQ_INGRFC_DROP_CNT_CNT_MASK            (0xFFFFFFFFU << MKS_TXQ_TXQ_INGRFC_DROP_CNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Ingress FC drop counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number of drop packets due ingress FC drop indication. */
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_INGRFC_DROP_CNT_t;
#endif /* GENHDR_STRUCT */

/* Egress FC drop counter */
#define MKS_TXQ_TXQ_EGRFC_DROP_CNT                      0x00000054U
/* Counts number of drop packets due egress FC drop indication. */
#define MKS_TXQ_TXQ_EGRFC_DROP_CNT_CNT_SHIFT            0
#define MKS_TXQ_TXQ_EGRFC_DROP_CNT_CNT_WIDTH            32
#define MKS_TXQ_TXQ_EGRFC_DROP_CNT_CNT_MASK             (0xFFFFFFFFU << MKS_TXQ_TXQ_EGRFC_DROP_CNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Egress FC drop counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number of drop packets due egress FC drop indication. */
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_EGRFC_DROP_CNT_t;
#endif /* GENHDR_STRUCT */

/* VirtIo Stall cycle counter */
#define MKS_TXQ_TXQ_VIOSTALL_CYC_CNT                    0x00000058U
/* Counts number of cycles Virtio is stalling. */
#define MKS_TXQ_TXQ_VIOSTALL_CYC_CNT_CNT_SHIFT          0
#define MKS_TXQ_TXQ_VIOSTALL_CYC_CNT_CNT_WIDTH          32
#define MKS_TXQ_TXQ_VIOSTALL_CYC_CNT_CNT_MASK           (0xFFFFFFFFU << MKS_TXQ_TXQ_VIOSTALL_CYC_CNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* VirtIo Stall cycle counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number of cycles Virtio is stalling. */
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_VIOSTALL_CYC_CNT_t;
#endif /* GENHDR_STRUCT */

/* PLD fifo Stall cycle counter */
#define MKS_TXQ_TXQ_PLDSTLL_CCNT                0x0000005CU
/* Counts number of cycles PLD Fifo is stalling. */
#define MKS_TXQ_TXQ_PLDSTLL_CCNT_CNT_SHIFT      0
#define MKS_TXQ_TXQ_PLDSTLL_CCNT_CNT_WIDTH      32
#define MKS_TXQ_TXQ_PLDSTLL_CCNT_CNT_MASK       (0xFFFFFFFFU << MKS_TXQ_TXQ_PLDSTLL_CCNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* PLD fifo Stall cycle counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number of cycles PLD Fifo is stalling. */
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_PLDSTLL_CCNT_t;
#endif /* GENHDR_STRUCT */

/* Rx-Mirror Lkup Drop Counter */
#define MKS_TXQ_TXQ_RMIR_LU_DRP_CNT                     0x00000060U
/* Counts number of packets are dropped due to lookup rx-mirror. */
#define MKS_TXQ_TXQ_RMIR_LU_DRP_CNT_CNT_SHIFT           0
#define MKS_TXQ_TXQ_RMIR_LU_DRP_CNT_CNT_WIDTH           32
#define MKS_TXQ_TXQ_RMIR_LU_DRP_CNT_CNT_MASK            (0xFFFFFFFFU << MKS_TXQ_TXQ_RMIR_LU_DRP_CNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx-Mirror Lkup Drop Counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number of packets are dropped due to lookup rx-mirror. */
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_RMIR_LU_DRP_CNT_t;
#endif /* GENHDR_STRUCT */

/*
 * Rx-Mirror Port packet counter: Counts number rx-mirror packets txq has
 * transmitted to pld_fifo. One counter per port
 */
#define MKS_TXQ_TXQ_RMIR_PKTCNT_0               0x00000064U
/* Port packet counter. */
#define MKS_TXQ_TXQ_RMIR_PKTCNT_0_CNT_SHIFT     0
#define MKS_TXQ_TXQ_RMIR_PKTCNT_0_CNT_WIDTH     32
#define MKS_TXQ_TXQ_RMIR_PKTCNT_0_CNT_MASK      (0xFFFFFFFFU << MKS_TXQ_TXQ_RMIR_PKTCNT_0_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Rx-Mirror Port packet counter: Counts number rx-mirror packets txq has
 * transmitted to pld_fifo. One counter per port
 */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Port packet counter. */
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_RMIR_PKTCNT_0_t;
#endif /* GENHDR_STRUCT */

/*
 * Rx-Mirror Port packet counter: Counts number rx-mirror packets txq has
 * transmitted to pld_fifo. One counter per port
 */
#define MKS_TXQ_TXQ_RMIR_PKTCNT_1               0x00000068U
/* Port packet counter. */
#define MKS_TXQ_TXQ_RMIR_PKTCNT_1_CNT_SHIFT     0
#define MKS_TXQ_TXQ_RMIR_PKTCNT_1_CNT_WIDTH     32
#define MKS_TXQ_TXQ_RMIR_PKTCNT_1_CNT_MASK      (0xFFFFFFFFU << MKS_TXQ_TXQ_RMIR_PKTCNT_1_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Rx-Mirror Port packet counter: Counts number rx-mirror packets txq has
 * transmitted to pld_fifo. One counter per port
 */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Port packet counter. */
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_RMIR_PKTCNT_1_t;
#endif /* GENHDR_STRUCT */

/*
 * Rx-Mirror Port packet counter: Counts number rx-mirror packets txq has
 * transmitted to pld_fifo. One counter per port
 */
#define MKS_TXQ_TXQ_RMIR_PKTCNT_2               0x0000006CU
/* Port packet counter. */
#define MKS_TXQ_TXQ_RMIR_PKTCNT_2_CNT_SHIFT     0
#define MKS_TXQ_TXQ_RMIR_PKTCNT_2_CNT_WIDTH     32
#define MKS_TXQ_TXQ_RMIR_PKTCNT_2_CNT_MASK      (0xFFFFFFFFU << MKS_TXQ_TXQ_RMIR_PKTCNT_2_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Rx-Mirror Port packet counter: Counts number rx-mirror packets txq has
 * transmitted to pld_fifo. One counter per port
 */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Port packet counter. */
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_RMIR_PKTCNT_2_t;
#endif /* GENHDR_STRUCT */

/*
 * Rx-Mirror Port packet counter: Counts number rx-mirror packets txq has
 * transmitted to pld_fifo. One counter per port
 */
#define MKS_TXQ_TXQ_RMIR_PKTCNT_3               0x00000070U
/* Port packet counter. */
#define MKS_TXQ_TXQ_RMIR_PKTCNT_3_CNT_SHIFT     0
#define MKS_TXQ_TXQ_RMIR_PKTCNT_3_CNT_WIDTH     32
#define MKS_TXQ_TXQ_RMIR_PKTCNT_3_CNT_MASK      (0xFFFFFFFFU << MKS_TXQ_TXQ_RMIR_PKTCNT_3_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Rx-Mirror Port packet counter: Counts number rx-mirror packets txq has
 * transmitted to pld_fifo. One counter per port
 */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Port packet counter. */
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_RMIR_PKTCNT_3_t;
#endif /* GENHDR_STRUCT */

/*
 * Tx-Mirror Port packet counter: Counts number tx-mirror packets txq has
 * transmitted to txSch. One counter per port
 */
#define MKS_TXQ_TXQ_TMIR_PKTCNT_0               0x00000074U
/* Port packet counter. */
#define MKS_TXQ_TXQ_TMIR_PKTCNT_0_CNT_SHIFT     0
#define MKS_TXQ_TXQ_TMIR_PKTCNT_0_CNT_WIDTH     32
#define MKS_TXQ_TXQ_TMIR_PKTCNT_0_CNT_MASK      (0xFFFFFFFFU << MKS_TXQ_TXQ_TMIR_PKTCNT_0_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Tx-Mirror Port packet counter: Counts number tx-mirror packets txq has
 * transmitted to txSch. One counter per port
 */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Port packet counter. */
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_TMIR_PKTCNT_0_t;
#endif /* GENHDR_STRUCT */

/*
 * Tx-Mirror Port packet counter: Counts number tx-mirror packets txq has
 * transmitted to txSch. One counter per port
 */
#define MKS_TXQ_TXQ_TMIR_PKTCNT_1               0x00000078U
/* Port packet counter. */
#define MKS_TXQ_TXQ_TMIR_PKTCNT_1_CNT_SHIFT     0
#define MKS_TXQ_TXQ_TMIR_PKTCNT_1_CNT_WIDTH     32
#define MKS_TXQ_TXQ_TMIR_PKTCNT_1_CNT_MASK      (0xFFFFFFFFU << MKS_TXQ_TXQ_TMIR_PKTCNT_1_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Tx-Mirror Port packet counter: Counts number tx-mirror packets txq has
 * transmitted to txSch. One counter per port
 */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Port packet counter. */
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_TMIR_PKTCNT_1_t;
#endif /* GENHDR_STRUCT */

/*
 * Tx-Mirror Port packet counter: Counts number tx-mirror packets txq has
 * transmitted to txSch. One counter per port
 */
#define MKS_TXQ_TXQ_TMIR_PKTCNT_2               0x0000007CU
/* Port packet counter. */
#define MKS_TXQ_TXQ_TMIR_PKTCNT_2_CNT_SHIFT     0
#define MKS_TXQ_TXQ_TMIR_PKTCNT_2_CNT_WIDTH     32
#define MKS_TXQ_TXQ_TMIR_PKTCNT_2_CNT_MASK      (0xFFFFFFFFU << MKS_TXQ_TXQ_TMIR_PKTCNT_2_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Tx-Mirror Port packet counter: Counts number tx-mirror packets txq has
 * transmitted to txSch. One counter per port
 */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Port packet counter. */
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_TMIR_PKTCNT_2_t;
#endif /* GENHDR_STRUCT */

/*
 * Tx-Mirror Port packet counter: Counts number tx-mirror packets txq has
 * transmitted to txSch. One counter per port
 */
#define MKS_TXQ_TXQ_TMIR_PKTCNT_3               0x00000080U
/* Port packet counter. */
#define MKS_TXQ_TXQ_TMIR_PKTCNT_3_CNT_SHIFT     0
#define MKS_TXQ_TXQ_TMIR_PKTCNT_3_CNT_WIDTH     32
#define MKS_TXQ_TXQ_TMIR_PKTCNT_3_CNT_MASK      (0xFFFFFFFFU << MKS_TXQ_TXQ_TMIR_PKTCNT_3_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Tx-Mirror Port packet counter: Counts number tx-mirror packets txq has
 * transmitted to txSch. One counter per port
 */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Port packet counter. */
  } field;
  uint32_t val;
} MKS_TXQ_TXQ_TMIR_PKTCNT_3_t;
#endif /* GENHDR_STRUCT */

#endif /* _MKS_TXQ_REGISTERS_H_ */

