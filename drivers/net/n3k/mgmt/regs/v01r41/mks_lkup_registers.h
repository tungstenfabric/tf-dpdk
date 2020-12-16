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

#ifndef _MKS_LKUP_REGISTERS_H_
#define _MKS_LKUP_REGISTERS_H_

/* OVS Lookups */

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

#define MKS_LKUP_ACCESSTYPE             GENHDR_ACCESSTYPE_RW /*  Default access type. Access types defined above. */
#define MKS_LKUP_REGWIDTH               32 /* Default width of register in bits */
#define MKS_LKUP_ACCESSWIDTH            32 /* Default width of access word in bit */


/* OVS status */
#define MKS_LKUP_LKUP_GLOBAL_STAT                               0x00000000U
/* TxSch initialization is done. */
#define MKS_LKUP_LKUP_GLOBAL_STAT_TX_SCH_INIT_DONE_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_GLOBAL_STAT_TX_SCH_INIT_DONE_SHIFT        0
#define MKS_LKUP_LKUP_GLOBAL_STAT_TX_SCH_INIT_DONE_WIDTH        1
#define MKS_LKUP_LKUP_GLOBAL_STAT_TX_SCH_INIT_DONE_MASK         (0x1U << MKS_LKUP_LKUP_GLOBAL_STAT_TX_SCH_INIT_DONE_SHIFT)

/* TxQ initialization is done. */
#define MKS_LKUP_LKUP_GLOBAL_STAT_TX_QINIT_DONE_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_GLOBAL_STAT_TX_QINIT_DONE_SHIFT           1
#define MKS_LKUP_LKUP_GLOBAL_STAT_TX_QINIT_DONE_WIDTH           1
#define MKS_LKUP_LKUP_GLOBAL_STAT_TX_QINIT_DONE_MASK            (0x1U << MKS_LKUP_LKUP_GLOBAL_STAT_TX_QINIT_DONE_SHIFT)

/* PM initialization is done. */
#define MKS_LKUP_LKUP_GLOBAL_STAT_PM_INIT_DONE_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_GLOBAL_STAT_PM_INIT_DONE_SHIFT            2
#define MKS_LKUP_LKUP_GLOBAL_STAT_PM_INIT_DONE_WIDTH            1
#define MKS_LKUP_LKUP_GLOBAL_STAT_PM_INIT_DONE_MASK             (0x1U << MKS_LKUP_LKUP_GLOBAL_STAT_PM_INIT_DONE_SHIFT)

/* IFC initialization is done. */
#define MKS_LKUP_LKUP_GLOBAL_STAT_IFC_INIT_DONE_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_GLOBAL_STAT_IFC_INIT_DONE_SHIFT           3
#define MKS_LKUP_LKUP_GLOBAL_STAT_IFC_INIT_DONE_WIDTH           1
#define MKS_LKUP_LKUP_GLOBAL_STAT_IFC_INIT_DONE_MASK            (0x1U << MKS_LKUP_LKUP_GLOBAL_STAT_IFC_INIT_DONE_SHIFT)

/* Lookup initialization is done. */
#define MKS_LKUP_LKUP_GLOBAL_STAT_LKUP_INIT_DONE_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_GLOBAL_STAT_LKUP_INIT_DONE_SHIFT          4
#define MKS_LKUP_LKUP_GLOBAL_STAT_LKUP_INIT_DONE_WIDTH          1
#define MKS_LKUP_LKUP_GLOBAL_STAT_LKUP_INIT_DONE_MASK           (0x1U << MKS_LKUP_LKUP_GLOBAL_STAT_LKUP_INIT_DONE_SHIFT)

/* PCS is ready for Eth0. */
#define MKS_LKUP_LKUP_GLOBAL_STAT_PCS_RDY_ETH_0_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_GLOBAL_STAT_PCS_RDY_ETH_0_SHIFT           5
#define MKS_LKUP_LKUP_GLOBAL_STAT_PCS_RDY_ETH_0_WIDTH           1
#define MKS_LKUP_LKUP_GLOBAL_STAT_PCS_RDY_ETH_0_MASK            (0x1U << MKS_LKUP_LKUP_GLOBAL_STAT_PCS_RDY_ETH_0_SHIFT)

/* PCS is ready for Eth1. */
#define MKS_LKUP_LKUP_GLOBAL_STAT_PCS_RDY_ETH_1_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_GLOBAL_STAT_PCS_RDY_ETH_1_SHIFT           6
#define MKS_LKUP_LKUP_GLOBAL_STAT_PCS_RDY_ETH_1_WIDTH           1
#define MKS_LKUP_LKUP_GLOBAL_STAT_PCS_RDY_ETH_1_MASK            (0x1U << MKS_LKUP_LKUP_GLOBAL_STAT_PCS_RDY_ETH_1_SHIFT)

/* EM Lookup Alarm Status. */
#define MKS_LKUP_LKUP_GLOBAL_STAT_LKUP_ALARM_ACCESSTYPE         GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_GLOBAL_STAT_LKUP_ALARM_SHIFT              7
#define MKS_LKUP_LKUP_GLOBAL_STAT_LKUP_ALARM_WIDTH              1
#define MKS_LKUP_LKUP_GLOBAL_STAT_LKUP_ALARM_MASK               (0x1U << MKS_LKUP_LKUP_GLOBAL_STAT_LKUP_ALARM_SHIFT)

#ifdef GENHDR_STRUCT
/* OVS status */
typedef union {
  struct {
    uint32_t tx_sch_init_done : 1;      /* TxSch initialization is done. */
    uint32_t tx_qinit_done : 1;         /* TxQ initialization is done. */
    uint32_t pm_init_done : 1;          /* PM initialization is done. */
    uint32_t ifc_init_done : 1;         /* IFC initialization is done. */
    uint32_t lkup_init_done : 1;        /* Lookup initialization is done. */
    uint32_t pcs_rdy_eth_0 : 1;         /* PCS is ready for Eth0. */
    uint32_t pcs_rdy_eth_1 : 1;         /* PCS is ready for Eth1. */
    uint32_t lkup_alarm : 1;            /* EM Lookup Alarm Status. */
    uint32_t : 24;
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_GLOBAL_STAT_t;
#endif /* GENHDR_STRUCT */

/* General purpose RW register intended for debug use */
#define MKS_LKUP_LKUP_SCRATCH                   0x00000004U
/* Temporary hardware debug. Enables specific group of default flows. */
#define MKS_LKUP_LKUP_SCRATCH_DEBUG_SHIFT       0
#define MKS_LKUP_LKUP_SCRATCH_DEBUG_WIDTH       2
#define MKS_LKUP_LKUP_SCRATCH_DEBUG_MASK        (0x3U << MKS_LKUP_LKUP_SCRATCH_DEBUG_SHIFT)

/* A 30-bit scratch R/W register. */
#define MKS_LKUP_LKUP_SCRATCH_SCRATCH_SHIFT     2
#define MKS_LKUP_LKUP_SCRATCH_SCRATCH_WIDTH     30
#define MKS_LKUP_LKUP_SCRATCH_SCRATCH_MASK      (0x3FFFFFFFU << MKS_LKUP_LKUP_SCRATCH_SCRATCH_SHIFT)

#ifdef GENHDR_STRUCT
/* General purpose RW register intended for debug use */
typedef union {
  struct {
    uint32_t debug : 2;         /* Temporary hardware debug. Enables specific group of default flows. */
    uint32_t scratch : 30;      /* A 30-bit scratch R/W register. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_SCRATCH_t;
#endif /* GENHDR_STRUCT */

#define MKS_LKUP_LKUP_SCRATCH_DEF_LKP_OPT_SWDEFAULT 0x0U
#define MKS_LKUP_LKUP_SCRATCH_DEF_LKP_OPT_HWSANJOSE 0x1U
#define MKS_LKUP_LKUP_SCRATCH_DEF_LKP_OPT_HWUNITEDK 0x2U

/* Lookup command register */
#define MKS_LKUP_LKUP_CMD_REG                   0x00000008U
/*
 * Indicates which index of ptd, key or key mask to be accessed when blkId is 0,
 * 1, or 2. Exception Rate index need to accessed when blkId is 8, 9, 10, 12,
 * 13, or 14. 0: EM miss. 1: EM miss slow path MAC. 2: EM miss ARP packet. 3: EM
 * miss ICMP packet. 4: EM miss Broadcast packet. 5: EM miss Multicast packet.
 * 6: EPOCH miss. 7: Connection Track dummy hit. 8: ARM DMAC address hit. 9: IP
 * Fragmentation Hit. 10: EM miss TCP packet.
 */
#define MKS_LKUP_LKUP_CMD_REG_IDX_SHIFT         0
#define MKS_LKUP_LKUP_CMD_REG_IDX_WIDTH         5
#define MKS_LKUP_LKUP_CMD_REG_IDX_MASK          (0x1FU << MKS_LKUP_LKUP_CMD_REG_IDX_SHIFT)

/* Indicates which block to be accessed. */
#define MKS_LKUP_LKUP_CMD_REG_BLK_ID_SHIFT      8
#define MKS_LKUP_LKUP_CMD_REG_BLK_ID_WIDTH      4
#define MKS_LKUP_LKUP_CMD_REG_BLK_ID_MASK       (0xFU << MKS_LKUP_LKUP_CMD_REG_BLK_ID_SHIFT)

/*
 * Set this bit to enable write to ptd, key, or key mask.
 *  SW sets and HW clears when process is done. Once the process is done read
 * lkup data reg to get the read back value.
 */
#define MKS_LKUP_LKUP_CMD_REG_WR_EN_SHIFT       12
#define MKS_LKUP_LKUP_CMD_REG_WR_EN_WIDTH       1
#define MKS_LKUP_LKUP_CMD_REG_WR_EN_MASK        (0x1U << MKS_LKUP_LKUP_CMD_REG_WR_EN_SHIFT)

/*
 * Set this bit to enable read from ptd, key, or key mask.
 *  SW sets and HW clears when process is done. Once the process is done read
 * lkup data reg to get the read back value.
 */
#define MKS_LKUP_LKUP_CMD_REG_RD_EN_SHIFT       13
#define MKS_LKUP_LKUP_CMD_REG_RD_EN_WIDTH       1
#define MKS_LKUP_LKUP_CMD_REG_RD_EN_MASK        (0x1U << MKS_LKUP_LKUP_CMD_REG_RD_EN_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup command register */
typedef union {
  struct {
    uint32_t idx : 5;           /* Indicates which index of ptd, key or key mask to be accessed when blkId is 0,
                                   1, or 2. Exception Rate index need to accessed when blkId is 8, 9, 10, 12,
                                   13, or 14. 0: EM miss. 1: EM miss slow path MAC. 2: EM miss ARP packet. 3: EM
                                   miss ICMP packet. 4: EM miss Broadcast packet. 5: EM miss Multicast packet.
                                   6: EPOCH miss. 7: Connection Track dummy hit. 8: ARM DMAC address hit. 9: IP
                                   Fragmentation Hit. 10: EM miss TCP packet. */
    uint32_t : 3;
    uint32_t blk_id : 4;        /* Indicates which block to be accessed. */
    uint32_t wr_en : 1;         /* Set this bit to enable write to ptd, key, or key mask.
                                   SW sets and HW clears when process is done. Once the process is done read
                                   lkup data reg to get the read back value. */
    uint32_t rd_en : 1;         /* Set this bit to enable read from ptd, key, or key mask.
                                   SW sets and HW clears when process is done. Once the process is done read
                                   lkup data reg to get the read back value. */
    uint32_t : 18;
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_CMD_REG_t;
#endif /* GENHDR_STRUCT */

#define MKS_LKUP_LKUP_CMD_REG_BLK_ID_KEY        0x0U
#define MKS_LKUP_LKUP_CMD_REG_BLK_ID_KEY_MASK   0x1U
#define MKS_LKUP_LKUP_CMD_REG_BLK_ID_PTD        0x2U
#define MKS_LKUP_LKUP_CMD_REG_BLK_ID_NW_EXC_RATE 0x8U
#define MKS_LKUP_LKUP_CMD_REG_BLK_ID_NW_EXC_OCNT 0x9U
#define MKS_LKUP_LKUP_CMD_REG_BLK_ID_NW_EXC_UCNT 0xAU
#define MKS_LKUP_LKUP_CMD_REG_BLK_ID_HST_EXC_RATE 0xCU
#define MKS_LKUP_LKUP_CMD_REG_BLK_ID_HST_EXC_OCNT 0xDU
#define MKS_LKUP_LKUP_CMD_REG_BLK_ID_HST_EXC_UCNT 0xEU

/* Configuration Register for various options such as LAG, bypass EM Lookup etc */
#define MKS_LKUP_LKUP_CFG_REG                                   0x0000000CU
/*
 *  Enables routing of OVS offload to port 2(ARM).
 *  0: Miss rule route to virtio subport=0.
 *  1: Miss rule route to arm.
 */
#define MKS_LKUP_LKUP_CFG_REG_MISS_ROUTE_EN_SHIFT               0
#define MKS_LKUP_LKUP_CFG_REG_MISS_ROUTE_EN_WIDTH               1
#define MKS_LKUP_LKUP_CFG_REG_MISS_ROUTE_EN_MASK                (0x1U << MKS_LKUP_LKUP_CFG_REG_MISS_ROUTE_EN_SHIFT)

/*
 *  Enables bypassing Exact Match Lookup and use default lookup.
 *  0: Use Exact Match Lookup.
 *  1: Use Default Lookup.
 */
#define MKS_LKUP_LKUP_CFG_REG_BYPASS_EMLKUP_SHIFT               1
#define MKS_LKUP_LKUP_CFG_REG_BYPASS_EMLKUP_WIDTH               1
#define MKS_LKUP_LKUP_CFG_REG_BYPASS_EMLKUP_MASK                (0x1U << MKS_LKUP_LKUP_CFG_REG_BYPASS_EMLKUP_SHIFT)

/*
 *  Disable VF0 as high priority.
 *  0: Enable VF0 as high priority queue.
 *  1: Disable VF0 as high priority queue.
 */
#define MKS_LKUP_LKUP_CFG_REG_VFO_PRIORITY_HIGH_SHIFT           2
#define MKS_LKUP_LKUP_CFG_REG_VFO_PRIORITY_HIGH_WIDTH           1
#define MKS_LKUP_LKUP_CFG_REG_VFO_PRIORITY_HIGH_MASK            (0x1U << MKS_LKUP_LKUP_CFG_REG_VFO_PRIORITY_HIGH_SHIFT)

/*
 *  Control for flow tracker bypass.
 *  0: Disable flow tracker bypass.
 *  1: Enable flow tracker bypass.
 */
#define MKS_LKUP_LKUP_CFG_REG_BYPASS_FT_SHIFT                   3
#define MKS_LKUP_LKUP_CFG_REG_BYPASS_FT_WIDTH                   1
#define MKS_LKUP_LKUP_CFG_REG_BYPASS_FT_MASK                    (0x1U << MKS_LKUP_LKUP_CFG_REG_BYPASS_FT_SHIFT)

/*
 *  PLD FIFO pop rate.
 *  0: Invalid 
 *  1: Invalid 
 *  2: Pop 1 every 3 cycle.
 *  3: Pop 1 every 4 cycle.
 *  4: Pop 1 every 5 cycle.
 *  5: Pop 1 every 6 cycle.
 *  6: Pop 1 every 7 cycle.
 *  7: Pop 1 every 8 cycle.
 *  8: Pop 1 every 9 cycle.
 *  9: Pop 1 every 10 cycle. 
 *  10: Pop 1 every 11 cycle.
 *  11: Pop 1 every 12 cycle.
 *  12: Pop 1 every 13 cycle.
 *  13: Pop 1 every 14 cycle.
 *  14: Pop 1 every 15 cycle.
 *  ......
 *  31: Invalid.
 */
#define MKS_LKUP_LKUP_CFG_REG_RATE_CTL_PLD_FIFO_SHIFT           4
#define MKS_LKUP_LKUP_CFG_REG_RATE_CTL_PLD_FIFO_WIDTH           4
#define MKS_LKUP_LKUP_CFG_REG_RATE_CTL_PLD_FIFO_MASK            (0xFU << MKS_LKUP_LKUP_CFG_REG_RATE_CTL_PLD_FIFO_SHIFT)

/*
 * VID LAG compare value. If cfgVidLagEn is set and the cfgVidLag is equal to
 * the VID from Lookup then the incoming packet is a LAG packet. 
 */
#define MKS_LKUP_LKUP_CFG_REG_CFG_VID_LAG_SHIFT                 8
#define MKS_LKUP_LKUP_CFG_REG_CFG_VID_LAG_WIDTH                 8
#define MKS_LKUP_LKUP_CFG_REG_CFG_VID_LAG_MASK                  (0xFFU << MKS_LKUP_LKUP_CFG_REG_CFG_VID_LAG_SHIFT)

/*
 * VID Tx Port id-0. Indicates which of the four ports the packet will be
 * transmitted to. 0: MAC0, 1: MAC1, 2: ARM, 3: VirtIO. 
 */
#define MKS_LKUP_LKUP_CFG_REG_V_LAG_TX_PORT_ID_0_SHIFT          16
#define MKS_LKUP_LKUP_CFG_REG_V_LAG_TX_PORT_ID_0_WIDTH          2
#define MKS_LKUP_LKUP_CFG_REG_V_LAG_TX_PORT_ID_0_MASK           (0x3U << MKS_LKUP_LKUP_CFG_REG_V_LAG_TX_PORT_ID_0_SHIFT)

/*
 * VID Tx Port id-1. Indicates which of the four ports the packet will be
 * transmitted to. 0: MAC0, 1: MAC1, 2: ARM, 3: VirtIO.
 */
#define MKS_LKUP_LKUP_CFG_REG_V_LAG_TX_PORT_ID_1_SHIFT          18
#define MKS_LKUP_LKUP_CFG_REG_V_LAG_TX_PORT_ID_1_WIDTH          2
#define MKS_LKUP_LKUP_CFG_REG_V_LAG_TX_PORT_ID_1_MASK           (0x3U << MKS_LKUP_LKUP_CFG_REG_V_LAG_TX_PORT_ID_1_SHIFT)

/*
 * VID Tx Port id-2. Indicates which of the four ports the packet will be
 * transmitted to. 0: MAC0, 1: MAC1, 2: ARM, 3: VirtIO.
 */
#define MKS_LKUP_LKUP_CFG_REG_V_LAG_TX_PORT_ID_2_SHIFT          20
#define MKS_LKUP_LKUP_CFG_REG_V_LAG_TX_PORT_ID_2_WIDTH          2
#define MKS_LKUP_LKUP_CFG_REG_V_LAG_TX_PORT_ID_2_MASK           (0x3U << MKS_LKUP_LKUP_CFG_REG_V_LAG_TX_PORT_ID_2_SHIFT)

/*
 * VID Tx Port id-3. Indicates which of the four ports the packet will be
 * transmitted to. 0: MAC0, 1: MAC1, 2: ARM, 3: VirtIO.
 */
#define MKS_LKUP_LKUP_CFG_REG_V_LAG_TX_PORT_ID_3_SHIFT          22
#define MKS_LKUP_LKUP_CFG_REG_V_LAG_TX_PORT_ID_3_WIDTH          2
#define MKS_LKUP_LKUP_CFG_REG_V_LAG_TX_PORT_ID_3_MASK           (0x3U << MKS_LKUP_LKUP_CFG_REG_V_LAG_TX_PORT_ID_3_SHIFT)

/*  Vid Enable for LAG. 1: Enable LAG, 0: Disable LAG . */
#define MKS_LKUP_LKUP_CFG_REG_CFG_VID_LAG_EN_SHIFT              24
#define MKS_LKUP_LKUP_CFG_REG_CFG_VID_LAG_EN_WIDTH              1
#define MKS_LKUP_LKUP_CFG_REG_CFG_VID_LAG_EN_MASK               (0x1U << MKS_LKUP_LKUP_CFG_REG_CFG_VID_LAG_EN_SHIFT)

/*
 * Select which vid to compare with cfgVidLag. 0: select txVid 1: select key.vid
 * 2: select rxVid = ptsubpt_to_vid(rx_port, rx_subPort).
 */
#define MKS_LKUP_LKUP_CFG_REG_CFG_VID_LAG_SEL_SHIFT             25
#define MKS_LKUP_LKUP_CFG_REG_CFG_VID_LAG_SEL_WIDTH             2
#define MKS_LKUP_LKUP_CFG_REG_CFG_VID_LAG_SEL_MASK              (0x3U << MKS_LKUP_LKUP_CFG_REG_CFG_VID_LAG_SEL_SHIFT)

/*
 * Miss packet is flagged as low priority. 1: Miss packet is flagged as low
 * priority. 0: Miss packet is flagged as high priority. 
 */
#define MKS_LKUP_LKUP_CFG_REG_CFG_MISS_PKT_LOW_PQ_SHIFT         27
#define MKS_LKUP_LKUP_CFG_REG_CFG_MISS_PKT_LOW_PQ_WIDTH         1
#define MKS_LKUP_LKUP_CFG_REG_CFG_MISS_PKT_LOW_PQ_MASK          (0x1U << MKS_LKUP_LKUP_CFG_REG_CFG_MISS_PKT_LOW_PQ_SHIFT)

/*
 * PldFifo Hash Function Load same seed for every packet. 1: Enable pldfifo hash
 * function to load the same seed for every packet. 0: Enable pldfifo hash
 * function to load the different seed for every packet .
 */
#define MKS_LKUP_LKUP_CFG_REG_CFG_LD_SAME_SEED_SHIFT            28
#define MKS_LKUP_LKUP_CFG_REG_CFG_LD_SAME_SEED_WIDTH            1
#define MKS_LKUP_LKUP_CFG_REG_CFG_LD_SAME_SEED_MASK             (0x1U << MKS_LKUP_LKUP_CFG_REG_CFG_LD_SAME_SEED_SHIFT)

/* 1: Enable TTL error checking. 0: Disable TTL error checking. */
#define MKS_LKUP_LKUP_CFG_REG_CFG_TTL_CHK_EN_SHIFT              29
#define MKS_LKUP_LKUP_CFG_REG_CFG_TTL_CHK_EN_WIDTH              1
#define MKS_LKUP_LKUP_CFG_REG_CFG_TTL_CHK_EN_MASK               (0x1U << MKS_LKUP_LKUP_CFG_REG_CFG_TTL_CHK_EN_SHIFT)

#ifdef GENHDR_STRUCT
/* Configuration Register for various options such as LAG, bypass EM Lookup etc */
typedef union {
  struct {
    uint32_t miss_route_en : 1;         /* Enables routing of OVS offload to port 2(ARM).
                                           0: Miss rule route to virtio subport=0.
                                           1: Miss rule route to arm. */
    uint32_t bypass_emlkup : 1;         /* Enables bypassing Exact Match Lookup and use default lookup.
                                           0: Use Exact Match Lookup.
                                           1: Use Default Lookup. */
    uint32_t vfo_priority_high : 1;     /* Disable VF0 as high priority.
                                           0: Enable VF0 as high priority queue.
                                           1: Disable VF0 as high priority queue. */
    uint32_t bypass_ft : 1;             /* Control for flow tracker bypass.
                                           0: Disable flow tracker bypass.
                                           1: Enable flow tracker bypass. */
    uint32_t rate_ctl_pld_fifo : 4;     /* PLD FIFO pop rate.
                                           0: Invalid
                                           1: Invalid
                                           2: Pop 1 every 3 cycle.
                                           3: Pop 1 every 4 cycle.
                                           4: Pop 1 every 5 cycle.
                                           5: Pop 1 every 6 cycle.
                                           6: Pop 1 every 7 cycle.
                                           7: Pop 1 every 8 cycle.
                                           8: Pop 1 every 9 cycle.
                                           9: Pop 1 every 10 cycle.
                                           10: Pop 1 every 11 cycle.
                                           11: Pop 1 every 12 cycle.
                                           12: Pop 1 every 13 cycle.
                                           13: Pop 1 every 14 cycle.
                                           14: Pop 1 every 15 cycle.
                                           ......
                                           31: Invalid. */
    uint32_t cfg_vid_lag : 8;           /* VID LAG compare value. If cfgVidLagEn is set and the cfgVidLag is equal to
                                           the VID from Lookup then the incoming packet is a LAG packet. */
    uint32_t v_lag_tx_port_id_0 : 2;    /* VID Tx Port id-0. Indicates which of the four ports the packet will be
                                           transmitted to. 0: MAC0, 1: MAC1, 2: ARM, 3: VirtIO. */
    uint32_t v_lag_tx_port_id_1 : 2;    /* VID Tx Port id-1. Indicates which of the four ports the packet will be
                                           transmitted to. 0: MAC0, 1: MAC1, 2: ARM, 3: VirtIO. */
    uint32_t v_lag_tx_port_id_2 : 2;    /* VID Tx Port id-2. Indicates which of the four ports the packet will be
                                           transmitted to. 0: MAC0, 1: MAC1, 2: ARM, 3: VirtIO. */
    uint32_t v_lag_tx_port_id_3 : 2;    /* VID Tx Port id-3. Indicates which of the four ports the packet will be
                                           transmitted to. 0: MAC0, 1: MAC1, 2: ARM, 3: VirtIO. */
    uint32_t cfg_vid_lag_en : 1;        /* Vid Enable for LAG. 1: Enable LAG, 0: Disable LAG . */
    uint32_t cfg_vid_lag_sel : 2;       /* Select which vid to compare with cfgVidLag. 0: select txVid 1: select key.vid
                                           2: select rxVid = ptsubpt_to_vid(rx_port, rx_subPort). */
    uint32_t cfg_miss_pkt_low_pq : 1;   /* Miss packet is flagged as low priority. 1: Miss packet is flagged as low
                                           priority. 0: Miss packet is flagged as high priority. */
    uint32_t cfg_ld_same_seed : 1;      /* PldFifo Hash Function Load same seed for every packet. 1: Enable pldfifo hash
                                           function to load the same seed for every packet. 0: Enable pldfifo hash
                                           function to load the different seed for every packet . */
    uint32_t cfg_ttl_chk_en : 1;        /* 1: Enable TTL error checking. 0: Disable TTL error checking. */
    uint32_t : 2;
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_CFG_REG_t;
#endif /* GENHDR_STRUCT */

#define MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_0                0x0U /* MAC0 port. */
#define MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_1                0x1U /* MAC1 port. */
#define MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_2                0x2U /* ARM port. */
#define MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_3                0x3U /* VirtIO port. */

#define MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_0                0x0U /* MAC0 port. */
#define MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_1                0x1U /* MAC1 port. */
#define MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_2                0x2U /* ARM port. */
#define MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_3                0x3U /* VirtIO port. */

#define MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_0                0x0U /* MAC0 port. */
#define MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_1                0x1U /* MAC1 port. */
#define MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_2                0x2U /* ARM port. */
#define MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_3                0x3U /* VirtIO port. */

#define MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_0                0x0U /* MAC0 port. */
#define MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_1                0x1U /* MAC1 port. */
#define MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_2                0x2U /* ARM port. */
#define MKS_LKUP_LKUP_CFG_REG_LAG_PORT_ID_PORT_3                0x3U /* VirtIO port. */

#define MKS_LKUP_LKUP_CFG_REG_VID_SEL_METADA_VID                0x2U /* rxVid = ptsubpt_to_vid(rx_port, rx_subPort). */
#define MKS_LKUP_LKUP_CFG_REG_VID_SEL_KEY_VID                   0x1U /* key.vid. */
#define MKS_LKUP_LKUP_CFG_REG_VID_SEL_RESULT_VID                0x0U /* txVid. */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_0                0x00000010U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_0_BITS_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_0_BITS_SHIFT     0
#define MKS_LKUP_LKUP_DATA_REG_0_BITS_WIDTH     32
#define MKS_LKUP_LKUP_DATA_REG_0_BITS_MASK      (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_0_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_0_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_1                0x00000014U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_1_BITS_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_1_BITS_SHIFT     0
#define MKS_LKUP_LKUP_DATA_REG_1_BITS_WIDTH     32
#define MKS_LKUP_LKUP_DATA_REG_1_BITS_MASK      (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_1_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_1_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_2                0x00000018U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_2_BITS_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_2_BITS_SHIFT     0
#define MKS_LKUP_LKUP_DATA_REG_2_BITS_WIDTH     32
#define MKS_LKUP_LKUP_DATA_REG_2_BITS_MASK      (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_2_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_2_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_3                0x0000001CU
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_3_BITS_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_3_BITS_SHIFT     0
#define MKS_LKUP_LKUP_DATA_REG_3_BITS_WIDTH     32
#define MKS_LKUP_LKUP_DATA_REG_3_BITS_MASK      (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_3_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_3_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_4                0x00000020U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_4_BITS_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_4_BITS_SHIFT     0
#define MKS_LKUP_LKUP_DATA_REG_4_BITS_WIDTH     32
#define MKS_LKUP_LKUP_DATA_REG_4_BITS_MASK      (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_4_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_4_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_5                0x00000024U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_5_BITS_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_5_BITS_SHIFT     0
#define MKS_LKUP_LKUP_DATA_REG_5_BITS_WIDTH     32
#define MKS_LKUP_LKUP_DATA_REG_5_BITS_MASK      (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_5_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_5_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_6                0x00000028U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_6_BITS_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_6_BITS_SHIFT     0
#define MKS_LKUP_LKUP_DATA_REG_6_BITS_WIDTH     32
#define MKS_LKUP_LKUP_DATA_REG_6_BITS_MASK      (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_6_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_6_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_7                0x0000002CU
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_7_BITS_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_7_BITS_SHIFT     0
#define MKS_LKUP_LKUP_DATA_REG_7_BITS_WIDTH     32
#define MKS_LKUP_LKUP_DATA_REG_7_BITS_MASK      (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_7_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_7_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_8                0x00000030U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_8_BITS_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_8_BITS_SHIFT     0
#define MKS_LKUP_LKUP_DATA_REG_8_BITS_WIDTH     32
#define MKS_LKUP_LKUP_DATA_REG_8_BITS_MASK      (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_8_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_8_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_9                0x00000034U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_9_BITS_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_9_BITS_SHIFT     0
#define MKS_LKUP_LKUP_DATA_REG_9_BITS_WIDTH     32
#define MKS_LKUP_LKUP_DATA_REG_9_BITS_MASK      (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_9_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_9_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_10                       0x00000038U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_10_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_10_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_10_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_10_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_10_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_10_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_11                       0x0000003CU
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_11_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_11_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_11_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_11_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_11_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_11_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_12                       0x00000040U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_12_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_12_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_12_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_12_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_12_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_12_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_13                       0x00000044U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_13_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_13_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_13_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_13_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_13_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_13_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_14                       0x00000048U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_14_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_14_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_14_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_14_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_14_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_14_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_15                       0x0000004CU
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_15_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_15_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_15_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_15_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_15_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_15_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_16                       0x00000050U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_16_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_16_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_16_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_16_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_16_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_16_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_17                       0x00000054U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_17_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_17_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_17_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_17_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_17_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_17_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_18                       0x00000058U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_18_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_18_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_18_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_18_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_18_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_18_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_19                       0x0000005CU
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_19_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_19_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_19_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_19_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_19_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_19_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_20                       0x00000060U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_20_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_20_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_20_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_20_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_20_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_20_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_21                       0x00000064U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_21_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_21_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_21_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_21_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_21_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_21_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_22                       0x00000068U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_22_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_22_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_22_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_22_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_22_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_22_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_23                       0x0000006CU
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_23_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_23_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_23_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_23_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_23_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_23_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_24                       0x00000070U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_24_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_24_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_24_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_24_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_24_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_24_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_25                       0x00000074U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_25_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_25_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_25_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_25_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_25_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_25_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_26                       0x00000078U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_26_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_26_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_26_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_26_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_26_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_26_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_27                       0x0000007CU
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_27_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_27_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_27_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_27_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_27_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_27_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_28                       0x00000080U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_28_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_28_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_28_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_28_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_28_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_28_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_29                       0x00000084U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_29_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_29_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_29_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_29_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_29_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_29_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_30                       0x00000088U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_30_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_30_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_30_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_30_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_30_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_30_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_31                       0x0000008CU
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_31_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_31_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_31_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_31_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_31_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_31_t;
#endif /* GENHDR_STRUCT */

/* Lookup data register */
#define MKS_LKUP_LKUP_DATA_REG_32                       0x00000090U
/* TBD Use as read data register to configure ptd, key, and key mask. */
#define MKS_LKUP_LKUP_DATA_REG_32_BITS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_DATA_REG_32_BITS_SHIFT            0
#define MKS_LKUP_LKUP_DATA_REG_32_BITS_WIDTH            32
#define MKS_LKUP_LKUP_DATA_REG_32_BITS_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DATA_REG_32_BITS_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup data register */
typedef union {
  struct {
    uint32_t bits : 32; /* TBD Use as read data register to configure ptd, key, and key mask. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DATA_REG_32_t;
#endif /* GENHDR_STRUCT */

/* Default hit counter */
#define MKS_LKUP_LKUP_DEF_HIT_CNT               0x00000094U
/* Counts number of times the Default Lookup got a hit. */
#define MKS_LKUP_LKUP_DEF_HIT_CNT_CNT_SHIFT     0
#define MKS_LKUP_LKUP_DEF_HIT_CNT_CNT_WIDTH     32
#define MKS_LKUP_LKUP_DEF_HIT_CNT_CNT_MASK      (0xFFFFFFFFU << MKS_LKUP_LKUP_DEF_HIT_CNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Default hit counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number of times the Default Lookup got a hit. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DEF_HIT_CNT_t;
#endif /* GENHDR_STRUCT */

/* Default miss counter  */
#define MKS_LKUP_LKUP_DEF_MISS_CNT                      0x00000098U
/* Counts number of times the Default Lookup got a miss. */
#define MKS_LKUP_LKUP_DEF_MISS_CNT_CNT_SHIFT            0
#define MKS_LKUP_LKUP_DEF_MISS_CNT_CNT_WIDTH            32
#define MKS_LKUP_LKUP_DEF_MISS_CNT_CNT_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_DEF_MISS_CNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Default miss counter  */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number of times the Default Lookup got a miss. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DEF_MISS_CNT_t;
#endif /* GENHDR_STRUCT */

/* PSG hit counter */
#define MKS_LKUP_LKUP_PSG_HIT_CNT               0x0000009CU
/* Counts number of times the Exact Match Lookup got a hit. */
#define MKS_LKUP_LKUP_PSG_HIT_CNT_CNT_SHIFT     0
#define MKS_LKUP_LKUP_PSG_HIT_CNT_CNT_WIDTH     32
#define MKS_LKUP_LKUP_PSG_HIT_CNT_CNT_MASK      (0xFFFFFFFFU << MKS_LKUP_LKUP_PSG_HIT_CNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* PSG hit counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number of times the Exact Match Lookup got a hit. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_PSG_HIT_CNT_t;
#endif /* GENHDR_STRUCT */

/* PSG miss counter */
#define MKS_LKUP_LKUP_PSG_MISS_CNT                      0x000000A0U
/* Counts number of times the Exact Match Lookup got a miss. */
#define MKS_LKUP_LKUP_PSG_MISS_CNT_CNT_SHIFT            0
#define MKS_LKUP_LKUP_PSG_MISS_CNT_CNT_WIDTH            32
#define MKS_LKUP_LKUP_PSG_MISS_CNT_CNT_MASK             (0xFFFFFFFFU << MKS_LKUP_LKUP_PSG_MISS_CNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* PSG miss counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number of times the Exact Match Lookup got a miss. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_PSG_MISS_CNT_t;
#endif /* GENHDR_STRUCT */

/* PSG error counter */
#define MKS_LKUP_LKUP_PSG_ERR_CNT               0x000000A4U
/*
 * Counts number of times the Exact Match Lookup detects an error. This
 * condition will result in packet drop.
 */
#define MKS_LKUP_LKUP_PSG_ERR_CNT_CNT_SHIFT     0
#define MKS_LKUP_LKUP_PSG_ERR_CNT_CNT_WIDTH     32
#define MKS_LKUP_LKUP_PSG_ERR_CNT_CNT_MASK      (0xFFFFFFFFU << MKS_LKUP_LKUP_PSG_ERR_CNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* PSG error counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number of times the Exact Match Lookup detects an error. This
                           condition will result in packet drop. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_PSG_ERR_CNT_t;
#endif /* GENHDR_STRUCT */

/* Result Action drop counter */
#define MKS_LKUP_LKUP_SRCH_DROP_CNT                     0x000000A8U
/*
 * Counts number of times a drop packet is detected. LKUP_SRCH_DROP_CNT =
 * LKUP_PSG_ERR_CNT + LKUP_MISS_DRP_CNT + LKUP_CMD_DRP_CNT +
 * LKUP_PARSERR_DRP_CNT.
 */
#define MKS_LKUP_LKUP_SRCH_DROP_CNT_CNT_SHIFT           0
#define MKS_LKUP_LKUP_SRCH_DROP_CNT_CNT_WIDTH           32
#define MKS_LKUP_LKUP_SRCH_DROP_CNT_CNT_MASK            (0xFFFFFFFFU << MKS_LKUP_LKUP_SRCH_DROP_CNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Result Action drop counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number of times a drop packet is detected. LKUP_SRCH_DROP_CNT =
                           LKUP_PSG_ERR_CNT + LKUP_MISS_DRP_CNT + LKUP_CMD_DRP_CNT +
                           LKUP_PARSERR_DRP_CNT. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_SRCH_DROP_CNT_t;
#endif /* GENHDR_STRUCT */

/* Lookup miss drop counter */
#define MKS_LKUP_LKUP_MISS_DROP_CNT                     0x000000ACU
/*
 * Counts number of times both EM and Default lookup do not have a hit. This
 * condition will result in drop packet.
 */
#define MKS_LKUP_LKUP_MISS_DROP_CNT_CNT_SHIFT           0
#define MKS_LKUP_LKUP_MISS_DROP_CNT_CNT_WIDTH           32
#define MKS_LKUP_LKUP_MISS_DROP_CNT_CNT_MASK            (0xFFFFFFFFU << MKS_LKUP_LKUP_MISS_DROP_CNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup miss drop counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number of times both EM and Default lookup do not have a hit. This
                           condition will result in drop packet. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_MISS_DROP_CNT_t;
#endif /* GENHDR_STRUCT */

/* Used to acquire access to Semaphore 0 */
#define MKS_LKUP_SEMAPHORE_LOCK0                0x000000B0U
/*
 * Used to acquire access to semaphore lock. If the semaphore 0 unlock register
 * is 0, a read to this CSR would return a 0 and set the semaphore 0 unlock
 * register to 1. Else it would return a 1, indicating the Read did not win the
 * arbitration. 
 */
#define MKS_LKUP_SEMAPHORE_LOCK0_LOCK_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_SEMAPHORE_LOCK0_LOCK_SHIFT     0
#define MKS_LKUP_SEMAPHORE_LOCK0_LOCK_WIDTH     1
#define MKS_LKUP_SEMAPHORE_LOCK0_LOCK_MASK      (0x1U << MKS_LKUP_SEMAPHORE_LOCK0_LOCK_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to acquire access to Semaphore 0 */
typedef union {
  struct {
    uint32_t lock : 1;  /* Used to acquire access to semaphore lock. If the semaphore 0 unlock register
                           is 0, a read to this CSR would return a 0 and set the semaphore 0 unlock
                           register to 1. Else it would return a 1, indicating the Read did not win the
                           arbitration. */
    uint32_t : 31;
  } field;
  uint32_t val;
} MKS_LKUP_SEMAPHORE_LOCK0_t;
#endif /* GENHDR_STRUCT */

/* Semaphore 0 unlock register */
#define MKS_LKUP_SEMAPHORE_UNLOCK0                      0x000000B4U
/* Stores the status of Lock and used to unlock the Semaphore 0.  */
#define MKS_LKUP_SEMAPHORE_UNLOCK0_UNLOCK_SHIFT         0
#define MKS_LKUP_SEMAPHORE_UNLOCK0_UNLOCK_WIDTH         1
#define MKS_LKUP_SEMAPHORE_UNLOCK0_UNLOCK_MASK          (0x1U << MKS_LKUP_SEMAPHORE_UNLOCK0_UNLOCK_SHIFT)

#ifdef GENHDR_STRUCT
/* Semaphore 0 unlock register */
typedef union {
  struct {
    uint32_t unlock : 1;        /* Stores the status of Lock and used to unlock the Semaphore 0. */
    uint32_t : 31;
  } field;
  uint32_t val;
} MKS_LKUP_SEMAPHORE_UNLOCK0_t;
#endif /* GENHDR_STRUCT */

/* Used to acquire access to Semaphore 1 */
#define MKS_LKUP_SEMAPHORE_LOCK1                0x000000B8U
/*
 * Used to acquire access to semaphore lock. If the semaphore 1 unlock register
 * is 0, a read to this CSR would return a 0 and set the semaphore 1 unlock
 * register to 1. Else it would return a 1, indicating the Read did not win the
 * arbitration. 
 */
#define MKS_LKUP_SEMAPHORE_LOCK1_LOCK_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_SEMAPHORE_LOCK1_LOCK_SHIFT     0
#define MKS_LKUP_SEMAPHORE_LOCK1_LOCK_WIDTH     1
#define MKS_LKUP_SEMAPHORE_LOCK1_LOCK_MASK      (0x1U << MKS_LKUP_SEMAPHORE_LOCK1_LOCK_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to acquire access to Semaphore 1 */
typedef union {
  struct {
    uint32_t lock : 1;  /* Used to acquire access to semaphore lock. If the semaphore 1 unlock register
                           is 0, a read to this CSR would return a 0 and set the semaphore 1 unlock
                           register to 1. Else it would return a 1, indicating the Read did not win the
                           arbitration. */
    uint32_t : 31;
  } field;
  uint32_t val;
} MKS_LKUP_SEMAPHORE_LOCK1_t;
#endif /* GENHDR_STRUCT */

/* Semaphore 1 unlock register */
#define MKS_LKUP_SEMAPHORE_UNLOCK1                      0x000000BCU
/* Stores the status of Lock and used to unlock the Semaphore 1.  */
#define MKS_LKUP_SEMAPHORE_UNLOCK1_UNLOCK_SHIFT         0
#define MKS_LKUP_SEMAPHORE_UNLOCK1_UNLOCK_WIDTH         1
#define MKS_LKUP_SEMAPHORE_UNLOCK1_UNLOCK_MASK          (0x1U << MKS_LKUP_SEMAPHORE_UNLOCK1_UNLOCK_SHIFT)

#ifdef GENHDR_STRUCT
/* Semaphore 1 unlock register */
typedef union {
  struct {
    uint32_t unlock : 1;        /* Stores the status of Lock and used to unlock the Semaphore 1. */
    uint32_t : 31;
  } field;
  uint32_t val;
} MKS_LKUP_SEMAPHORE_UNLOCK1_t;
#endif /* GENHDR_STRUCT */

/* Used to acquire access to Semaphore 2 */
#define MKS_LKUP_SEMAPHORE_LOCK2                0x000000C0U
/*
 * Used to acquire access to semaphore lock. If the semaphore 2 unlock register
 * is 0, a read to this CSR would return a 0 and set the semaphore 2 unlock
 * register to 1. Else it would return a 1, indicating the Read did not win the
 * arbitration. 
 */
#define MKS_LKUP_SEMAPHORE_LOCK2_LOCK_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_SEMAPHORE_LOCK2_LOCK_SHIFT     0
#define MKS_LKUP_SEMAPHORE_LOCK2_LOCK_WIDTH     1
#define MKS_LKUP_SEMAPHORE_LOCK2_LOCK_MASK      (0x1U << MKS_LKUP_SEMAPHORE_LOCK2_LOCK_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to acquire access to Semaphore 2 */
typedef union {
  struct {
    uint32_t lock : 1;  /* Used to acquire access to semaphore lock. If the semaphore 2 unlock register
                           is 0, a read to this CSR would return a 0 and set the semaphore 2 unlock
                           register to 1. Else it would return a 1, indicating the Read did not win the
                           arbitration. */
    uint32_t : 31;
  } field;
  uint32_t val;
} MKS_LKUP_SEMAPHORE_LOCK2_t;
#endif /* GENHDR_STRUCT */

/* Semaphore 2 unlock register */
#define MKS_LKUP_SEMAPHORE_UNLOCK2                      0x000000C4U
/* Stores the status of Lock and used to unlock the Semaphore 2.  */
#define MKS_LKUP_SEMAPHORE_UNLOCK2_UNLOCK_SHIFT         0
#define MKS_LKUP_SEMAPHORE_UNLOCK2_UNLOCK_WIDTH         1
#define MKS_LKUP_SEMAPHORE_UNLOCK2_UNLOCK_MASK          (0x1U << MKS_LKUP_SEMAPHORE_UNLOCK2_UNLOCK_SHIFT)

#ifdef GENHDR_STRUCT
/* Semaphore 2 unlock register */
typedef union {
  struct {
    uint32_t unlock : 1;        /* Stores the status of Lock and used to unlock the Semaphore 2. */
    uint32_t : 31;
  } field;
  uint32_t val;
} MKS_LKUP_SEMAPHORE_UNLOCK2_t;
#endif /* GENHDR_STRUCT */

/* Used to acquire access to Semaphore 3 */
#define MKS_LKUP_SEMAPHORE_LOCK3                0x000000C8U
/*
 * Used to acquire access to semaphore lock. If the semaphore 3 unlock register
 * is 0, a read to this CSR would return a 0 and set the semaphore 3 unlock
 * register to 1. Else it would return a 1, indicating the Read did not win the
 * arbitration. 
 */
#define MKS_LKUP_SEMAPHORE_LOCK3_LOCK_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_SEMAPHORE_LOCK3_LOCK_SHIFT     0
#define MKS_LKUP_SEMAPHORE_LOCK3_LOCK_WIDTH     1
#define MKS_LKUP_SEMAPHORE_LOCK3_LOCK_MASK      (0x1U << MKS_LKUP_SEMAPHORE_LOCK3_LOCK_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to acquire access to Semaphore 3 */
typedef union {
  struct {
    uint32_t lock : 1;  /* Used to acquire access to semaphore lock. If the semaphore 3 unlock register
                           is 0, a read to this CSR would return a 0 and set the semaphore 3 unlock
                           register to 1. Else it would return a 1, indicating the Read did not win the
                           arbitration. */
    uint32_t : 31;
  } field;
  uint32_t val;
} MKS_LKUP_SEMAPHORE_LOCK3_t;
#endif /* GENHDR_STRUCT */

/* Semaphore 3 unlock register */
#define MKS_LKUP_SEMAPHORE_UNLOCK3                      0x000000CCU
/* Stores the status of Lock and used to unlock the Semaphore 3.  */
#define MKS_LKUP_SEMAPHORE_UNLOCK3_UNLOCK_SHIFT         0
#define MKS_LKUP_SEMAPHORE_UNLOCK3_UNLOCK_WIDTH         1
#define MKS_LKUP_SEMAPHORE_UNLOCK3_UNLOCK_MASK          (0x1U << MKS_LKUP_SEMAPHORE_UNLOCK3_UNLOCK_SHIFT)

#ifdef GENHDR_STRUCT
/* Semaphore 3 unlock register */
typedef union {
  struct {
    uint32_t unlock : 1;        /* Stores the status of Lock and used to unlock the Semaphore 3. */
    uint32_t : 31;
  } field;
  uint32_t val;
} MKS_LKUP_SEMAPHORE_UNLOCK3_t;
#endif /* GENHDR_STRUCT */

/* Stats Index Compare Register */
#define MKS_LKUP_LKUP_STATIDX_STAT0                     0x000000D0U
/*
 * If the StatsIdx matches this value then increment a count in
 * LKUP_STATIDX_CNT0.
 */
#define MKS_LKUP_LKUP_STATIDX_STAT0_CTL_SHIFT           0
#define MKS_LKUP_LKUP_STATIDX_STAT0_CTL_WIDTH           21
#define MKS_LKUP_LKUP_STATIDX_STAT0_CTL_MASK            (0x1FFFFFU << MKS_LKUP_LKUP_STATIDX_STAT0_CTL_SHIFT)

#ifdef GENHDR_STRUCT
/* Stats Index Compare Register */
typedef union {
  struct {
    uint32_t ctl : 21;  /* If the StatsIdx matches this value then increment a count in
                           LKUP_STATIDX_CNT0. */
    uint32_t : 11;
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_STATIDX_STAT0_t;
#endif /* GENHDR_STRUCT */

/* Stats Index Compare Register */
#define MKS_LKUP_LKUP_STATIDX_STAT1                     0x000000D4U
/*
 * If the StatsIdx matches this value then increment a count in
 * LKUP_STATIDX_CNT1.
 */
#define MKS_LKUP_LKUP_STATIDX_STAT1_CTL_SHIFT           0
#define MKS_LKUP_LKUP_STATIDX_STAT1_CTL_WIDTH           21
#define MKS_LKUP_LKUP_STATIDX_STAT1_CTL_MASK            (0x1FFFFFU << MKS_LKUP_LKUP_STATIDX_STAT1_CTL_SHIFT)

#ifdef GENHDR_STRUCT
/* Stats Index Compare Register */
typedef union {
  struct {
    uint32_t ctl : 21;  /* If the StatsIdx matches this value then increment a count in
                           LKUP_STATIDX_CNT1. */
    uint32_t : 11;
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_STATIDX_STAT1_t;
#endif /* GENHDR_STRUCT */

/* Stats Index Compare Register */
#define MKS_LKUP_LKUP_STATIDX_STAT2                     0x000000D8U
/*
 * If the StatsIdx matches this value then increment a count in
 * LKUP_STATIDX_CNT2.
 */
#define MKS_LKUP_LKUP_STATIDX_STAT2_CTL_SHIFT           0
#define MKS_LKUP_LKUP_STATIDX_STAT2_CTL_WIDTH           21
#define MKS_LKUP_LKUP_STATIDX_STAT2_CTL_MASK            (0x1FFFFFU << MKS_LKUP_LKUP_STATIDX_STAT2_CTL_SHIFT)

#ifdef GENHDR_STRUCT
/* Stats Index Compare Register */
typedef union {
  struct {
    uint32_t ctl : 21;  /* If the StatsIdx matches this value then increment a count in
                           LKUP_STATIDX_CNT2. */
    uint32_t : 11;
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_STATIDX_STAT2_t;
#endif /* GENHDR_STRUCT */

/* Stats Index Compare Register */
#define MKS_LKUP_LKUP_STATIDX_STAT3                     0x000000DCU
/*
 * If the StatsIdx matches this value then increment a count in
 * LKUP_STATIDX_CNT3.
 */
#define MKS_LKUP_LKUP_STATIDX_STAT3_CTL_SHIFT           0
#define MKS_LKUP_LKUP_STATIDX_STAT3_CTL_WIDTH           21
#define MKS_LKUP_LKUP_STATIDX_STAT3_CTL_MASK            (0x1FFFFFU << MKS_LKUP_LKUP_STATIDX_STAT3_CTL_SHIFT)

#ifdef GENHDR_STRUCT
/* Stats Index Compare Register */
typedef union {
  struct {
    uint32_t ctl : 21;  /* If the StatsIdx matches this value then increment a count in
                           LKUP_STATIDX_CNT3. */
    uint32_t : 11;
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_STATIDX_STAT3_t;
#endif /* GENHDR_STRUCT */

/* Stats Index Match Counter */
#define MKS_LKUP_LKUP_STATIDX_CNT0                      0x000000E0U
/* Counts number of times the statIdx matches with LKUP_STATIDX_STAT0. */
#define MKS_LKUP_LKUP_STATIDX_CNT0_PACKETS_SHIFT        0
#define MKS_LKUP_LKUP_STATIDX_CNT0_PACKETS_WIDTH        32
#define MKS_LKUP_LKUP_STATIDX_CNT0_PACKETS_MASK         (0xFFFFFFFFU << MKS_LKUP_LKUP_STATIDX_CNT0_PACKETS_SHIFT)

#ifdef GENHDR_STRUCT
/* Stats Index Match Counter */
typedef union {
  struct {
    uint32_t packets : 32;      /* Counts number of times the statIdx matches with LKUP_STATIDX_STAT0. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_STATIDX_CNT0_t;
#endif /* GENHDR_STRUCT */

/* Stats Index Match Counter */
#define MKS_LKUP_LKUP_STATIDX_CNT1                      0x000000E4U
/* Counts number of times the statIdx matches with LKUP_STATIDX_STAT1. */
#define MKS_LKUP_LKUP_STATIDX_CNT1_PACKETS_SHIFT        0
#define MKS_LKUP_LKUP_STATIDX_CNT1_PACKETS_WIDTH        32
#define MKS_LKUP_LKUP_STATIDX_CNT1_PACKETS_MASK         (0xFFFFFFFFU << MKS_LKUP_LKUP_STATIDX_CNT1_PACKETS_SHIFT)

#ifdef GENHDR_STRUCT
/* Stats Index Match Counter */
typedef union {
  struct {
    uint32_t packets : 32;      /* Counts number of times the statIdx matches with LKUP_STATIDX_STAT1. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_STATIDX_CNT1_t;
#endif /* GENHDR_STRUCT */

/* Stats Index Match Counter */
#define MKS_LKUP_LKUP_STATIDX_CNT2                      0x000000E8U
/* Counts number of times the statIdx matches with LKUP_STATIDX_STAT2. */
#define MKS_LKUP_LKUP_STATIDX_CNT2_PACKETS_SHIFT        0
#define MKS_LKUP_LKUP_STATIDX_CNT2_PACKETS_WIDTH        32
#define MKS_LKUP_LKUP_STATIDX_CNT2_PACKETS_MASK         (0xFFFFFFFFU << MKS_LKUP_LKUP_STATIDX_CNT2_PACKETS_SHIFT)

#ifdef GENHDR_STRUCT
/* Stats Index Match Counter */
typedef union {
  struct {
    uint32_t packets : 32;      /* Counts number of times the statIdx matches with LKUP_STATIDX_STAT2. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_STATIDX_CNT2_t;
#endif /* GENHDR_STRUCT */

/* Stats Index Match Counter */
#define MKS_LKUP_LKUP_STATIDX_CNT3                      0x000000ECU
/* Counts number of times the statIdx matches with LKUP_STATIDX_STAT3. */
#define MKS_LKUP_LKUP_STATIDX_CNT3_PACKETS_SHIFT        0
#define MKS_LKUP_LKUP_STATIDX_CNT3_PACKETS_WIDTH        32
#define MKS_LKUP_LKUP_STATIDX_CNT3_PACKETS_MASK         (0xFFFFFFFFU << MKS_LKUP_LKUP_STATIDX_CNT3_PACKETS_SHIFT)

#ifdef GENHDR_STRUCT
/* Stats Index Match Counter */
typedef union {
  struct {
    uint32_t packets : 32;      /* Counts number of times the statIdx matches with LKUP_STATIDX_STAT3. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_STATIDX_CNT3_t;
#endif /* GENHDR_STRUCT */

/* Action Command drop counter */
#define MKS_LKUP_LKUP_CMD_DRP_CNT               0x000000F0U
/*
 * Counts number of times the action command indicates drop. This condition will
 * result in drop packet.
 */
#define MKS_LKUP_LKUP_CMD_DRP_CNT_CNT_SHIFT     0
#define MKS_LKUP_LKUP_CMD_DRP_CNT_CNT_WIDTH     32
#define MKS_LKUP_LKUP_CMD_DRP_CNT_CNT_MASK      (0xFFFFFFFFU << MKS_LKUP_LKUP_CMD_DRP_CNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Action Command drop counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number of times the action command indicates drop. This condition will
                           result in drop packet. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_CMD_DRP_CNT_t;
#endif /* GENHDR_STRUCT */

/* Parser Error drop counter */
#define MKS_LKUP_LKUP_PARSERR_DRP_CNT                   0x000000F4U
/*
 * Counts number of times parser indicates parser error. This condition will
 * result in drop packet.
 */
#define MKS_LKUP_LKUP_PARSERR_DRP_CNT_CNT_SHIFT         0
#define MKS_LKUP_LKUP_PARSERR_DRP_CNT_CNT_WIDTH         32
#define MKS_LKUP_LKUP_PARSERR_DRP_CNT_CNT_MASK          (0xFFFFFFFFU << MKS_LKUP_LKUP_PARSERR_DRP_CNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Parser Error drop counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number of times parser indicates parser error. This condition will
                           result in drop packet. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_PARSERR_DRP_CNT_t;
#endif /* GENHDR_STRUCT */

/* Ingress EPOCH compare registers */
#define MKS_LKUP_LKUP_EPOCH                             0x000000F8U
/* Ingress EPOCH compare register for MAC0. */
#define MKS_LKUP_LKUP_EPOCH_ING_EPOCH_0_SHIFT           0
#define MKS_LKUP_LKUP_EPOCH_ING_EPOCH_0_WIDTH           8
#define MKS_LKUP_LKUP_EPOCH_ING_EPOCH_0_MASK            (0xFFU << MKS_LKUP_LKUP_EPOCH_ING_EPOCH_0_SHIFT)

/* Ingress EPOCH compare register for MAC1. */
#define MKS_LKUP_LKUP_EPOCH_ING_EPOCH_1_SHIFT           8
#define MKS_LKUP_LKUP_EPOCH_ING_EPOCH_1_WIDTH           8
#define MKS_LKUP_LKUP_EPOCH_ING_EPOCH_1_MASK            (0xFFU << MKS_LKUP_LKUP_EPOCH_ING_EPOCH_1_SHIFT)

/* Ingress EPOCH compare register for ARM. */
#define MKS_LKUP_LKUP_EPOCH_ING_EPOCH_2_SHIFT           16
#define MKS_LKUP_LKUP_EPOCH_ING_EPOCH_2_WIDTH           8
#define MKS_LKUP_LKUP_EPOCH_ING_EPOCH_2_MASK            (0xFFU << MKS_LKUP_LKUP_EPOCH_ING_EPOCH_2_SHIFT)

/* Ingress EPOCH compare register for VIRTIO. */
#define MKS_LKUP_LKUP_EPOCH_ING_EPOCH_3_SHIFT           24
#define MKS_LKUP_LKUP_EPOCH_ING_EPOCH_3_WIDTH           8
#define MKS_LKUP_LKUP_EPOCH_ING_EPOCH_3_MASK            (0xFFU << MKS_LKUP_LKUP_EPOCH_ING_EPOCH_3_SHIFT)

#ifdef GENHDR_STRUCT
/* Ingress EPOCH compare registers */
typedef union {
  struct {
    uint32_t ing_epoch_0 : 8;   /* Ingress EPOCH compare register for MAC0. */
    uint32_t ing_epoch_1 : 8;   /* Ingress EPOCH compare register for MAC1. */
    uint32_t ing_epoch_2 : 8;   /* Ingress EPOCH compare register for ARM. */
    uint32_t ing_epoch_3 : 8;   /* Ingress EPOCH compare register for VIRTIO. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_EPOCH_t;
#endif /* GENHDR_STRUCT */

/* TTL error drop counter */
#define MKS_LKUP_LKUP_TTL_ERR_DRP_CNT                   0x000000FCU
/* Counts number packets dropped due to TTL error. */
#define MKS_LKUP_LKUP_TTL_ERR_DRP_CNT_CNT_SHIFT         0
#define MKS_LKUP_LKUP_TTL_ERR_DRP_CNT_CNT_WIDTH         32
#define MKS_LKUP_LKUP_TTL_ERR_DRP_CNT_CNT_MASK          (0xFFFFFFFFU << MKS_LKUP_LKUP_TTL_ERR_DRP_CNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* TTL error drop counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number packets dropped due to TTL error. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_TTL_ERR_DRP_CNT_t;
#endif /* GENHDR_STRUCT */

/* Lookup exception rate data register */
#define MKS_LKUP_LKUP_CMD_DAT_REG                       0x00000100U
/*
 * Use as write or read data during exception rate access. Refer to section
 * 24.10 of the MKS-Ovs-Uarch.docx for programming sequence.
 */
#define MKS_LKUP_LKUP_CMD_DAT_REG_CMD_DATA_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_LKUP_CMD_DAT_REG_CMD_DATA_SHIFT        0
#define MKS_LKUP_LKUP_CMD_DAT_REG_CMD_DATA_WIDTH        32
#define MKS_LKUP_LKUP_CMD_DAT_REG_CMD_DATA_MASK         (0xFFFFFFFFU << MKS_LKUP_LKUP_CMD_DAT_REG_CMD_DATA_SHIFT)

#ifdef GENHDR_STRUCT
/* Lookup exception rate data register */
typedef union {
  struct {
    uint32_t cmd_data : 32;     /* Use as write or read data during exception rate access. Refer to section
                                   24.10 of the MKS-Ovs-Uarch.docx for programming sequence. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_CMD_DAT_REG_t;
#endif /* GENHDR_STRUCT */

/* Network interface DMAC registers */
#define MKS_LKUP_LKUP_DMAC_NW0                  0x00000104U
/*
 * Network interface DMAC bit [31:0] to be use for matching locally terminate
 * DMAC.
 */
#define MKS_LKUP_LKUP_DMAC_NW0_DATA_SHIFT       0
#define MKS_LKUP_LKUP_DMAC_NW0_DATA_WIDTH       32
#define MKS_LKUP_LKUP_DMAC_NW0_DATA_MASK        (0xFFFFFFFFU << MKS_LKUP_LKUP_DMAC_NW0_DATA_SHIFT)

#ifdef GENHDR_STRUCT
/* Network interface DMAC registers */
typedef union {
  struct {
    uint32_t data : 32; /* Network interface DMAC bit [31:0] to be use for matching locally terminate
                           DMAC. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DMAC_NW0_t;
#endif /* GENHDR_STRUCT */

/* Network interface DMAC registers */
#define MKS_LKUP_LKUP_DMAC_NW1                  0x00000108U
/*
 * Network interface DMAC bit [47:32] to be use for matching locally terminate
 * DMAC.
 *  NOTE: As per existing RTL implementation, this LKUP_DMAC_NW0/1 reg. value is
 * treated as ARM's DMAC (cfgExcRtArmDmac) and is compared with outer DMAC in
 * incoming packet to detect a match (armDmacHit).
 */
#define MKS_LKUP_LKUP_DMAC_NW1_DATA_SHIFT       0
#define MKS_LKUP_LKUP_DMAC_NW1_DATA_WIDTH       16
#define MKS_LKUP_LKUP_DMAC_NW1_DATA_MASK        (0xFFFFU << MKS_LKUP_LKUP_DMAC_NW1_DATA_SHIFT)

#ifdef GENHDR_STRUCT
/* Network interface DMAC registers */
typedef union {
  struct {
    uint32_t data : 16; /* Network interface DMAC bit [47:32] to be use for matching locally terminate
                           DMAC.
                           NOTE: As per existing RTL implementation, this LKUP_DMAC_NW0/1 reg. value is
                           treated as ARM's DMAC (cfgExcRtArmDmac) and is compared with outer DMAC in
                           incoming packet to detect a match (armDmacHit). */
    uint32_t : 16;
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DMAC_NW1_t;
#endif /* GENHDR_STRUCT */

/* Host interface DMAC registers */
#define MKS_LKUP_LKUP_DMAC_HST0                 0x0000010CU
/* Host interface DMAC bit [31:0] to be use for matching locally terminate DMAC. */
#define MKS_LKUP_LKUP_DMAC_HST0_DATA_SHIFT      0
#define MKS_LKUP_LKUP_DMAC_HST0_DATA_WIDTH      32
#define MKS_LKUP_LKUP_DMAC_HST0_DATA_MASK       (0xFFFFFFFFU << MKS_LKUP_LKUP_DMAC_HST0_DATA_SHIFT)

#ifdef GENHDR_STRUCT
/* Host interface DMAC registers */
typedef union {
  struct {
    uint32_t data : 32; /* Host interface DMAC bit [31:0] to be use for matching locally terminate DMAC. */
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DMAC_HST0_t;
#endif /* GENHDR_STRUCT */

/* Host interface DMAC registers */
#define MKS_LKUP_LKUP_DMAC_HST1                 0x00000110U
/*
 * Host interface DMAC bit [47:32] to be use for matching locally terminate
 * DMAC.
 *  NOTE-1: Locally terminated packets are packets that matches the configurable
 * DMAC address of the slow path (i.e. host). The SW can set the PSP using the
 * LKUP_PROG_SP register.
 *  NOTE-2: This LKUP_DMAC_HST0/1 register value will also be used to match SMAC
 * address of slow path for locally originated packets (i.e. packets that the
 * slow path generates).
 */
#define MKS_LKUP_LKUP_DMAC_HST1_DATA_SHIFT      0
#define MKS_LKUP_LKUP_DMAC_HST1_DATA_WIDTH      16
#define MKS_LKUP_LKUP_DMAC_HST1_DATA_MASK       (0xFFFFU << MKS_LKUP_LKUP_DMAC_HST1_DATA_SHIFT)

#ifdef GENHDR_STRUCT
/* Host interface DMAC registers */
typedef union {
  struct {
    uint32_t data : 16; /* Host interface DMAC bit [47:32] to be use for matching locally terminate
                           DMAC.
                           NOTE-1: Locally terminated packets are packets that matches the configurable
                           DMAC address of the slow path (i.e. host). The SW can set the PSP using the
                           LKUP_PROG_SP register.
                           NOTE-2: This LKUP_DMAC_HST0/1 register value will also be used to match SMAC
                           address of slow path for locally originated packets (i.e. packets that the
                           slow path generates). */
    uint32_t : 16;
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_DMAC_HST1_t;
#endif /* GENHDR_STRUCT */

/* Programmable Slow Path */
#define MKS_LKUP_LKUP_PROG_SP                                   0x00000114U
/*
 * Programmable Slow Path (VID): VID=100: MAC0. VID=101: MAC1. VID=102: ARM
 * VID=127 to 255: VIRTIO subPort 0 to 127.
 */
#define MKS_LKUP_LKUP_PROG_SP_PROG_SLOW_PATH_SHIFT              0
#define MKS_LKUP_LKUP_PROG_SP_PROG_SLOW_PATH_WIDTH              12
#define MKS_LKUP_LKUP_PROG_SP_PROG_SLOW_PATH_MASK               (0xFFFU << MKS_LKUP_LKUP_PROG_SP_PROG_SLOW_PATH_SHIFT)

/*
 * 1: Enable programmable Slow Path. 0: Disable progrmmable Slow Path. When this
 * bit is cleared, the slow path will be ARM interface for Bare Metal or Virtio
 * SubPort=0 for Virtual mode.
 */
#define MKS_LKUP_LKUP_PROG_SP_PROG_SLOW_PATH_EN_SHIFT           12
#define MKS_LKUP_LKUP_PROG_SP_PROG_SLOW_PATH_EN_WIDTH           1
#define MKS_LKUP_LKUP_PROG_SP_PROG_SLOW_PATH_EN_MASK            (0x1U << MKS_LKUP_LKUP_PROG_SP_PROG_SLOW_PATH_EN_SHIFT)

#ifdef GENHDR_STRUCT
/* Programmable Slow Path */
typedef union {
  struct {
    uint32_t prog_slow_path : 12;       /* Programmable Slow Path (VID): VID=100: MAC0. VID=101: MAC1. VID=102: ARM
                                           VID=127 to 255: VIRTIO subPort 0 to 127. */
    uint32_t prog_slow_path_en : 1;     /* 1: Enable programmable Slow Path. 0: Disable progrmmable Slow Path. When this
                                           bit is cleared, the slow path will be ARM interface for Bare Metal or Virtio
                                           SubPort=0 for Virtual mode. */
    uint32_t : 19;
  } field;
  uint32_t val;
} MKS_LKUP_LKUP_PROG_SP_t;
#endif /* GENHDR_STRUCT */

/* FLR Wrapper Configuration Protection register */
#define MKS_LKUP_FLR_WRPR_CFG_PROT                              0x00000118U
/*
 * To unlock, write 0x0000_ED01. If any other value is present,then the mode is
 * lock(default mode).
 *  Only if the mode is unlock,the SW is allowed to modify FLR_WRPR_CTRL
 * register.
 *  After updating the FLR_WRPR_CTRL register,the SW should set the mode back to
 * lock.
 */
#define MKS_LKUP_FLR_WRPR_CFG_PROT_FLR_WRPR_CFG_PROT_SHIFT      0
#define MKS_LKUP_FLR_WRPR_CFG_PROT_FLR_WRPR_CFG_PROT_WIDTH      32
#define MKS_LKUP_FLR_WRPR_CFG_PROT_FLR_WRPR_CFG_PROT_MASK       (0xFFFFFFFFU << MKS_LKUP_FLR_WRPR_CFG_PROT_FLR_WRPR_CFG_PROT_SHIFT)

#ifdef GENHDR_STRUCT
/* FLR Wrapper Configuration Protection register */
typedef union {
  struct {
    uint32_t flr_wrpr_cfg_prot : 32;    /* To unlock, write 0x0000_ED01. If any other value is present,then the mode is
                                           lock(default mode).
                                           Only if the mode is unlock,the SW is allowed to modify FLR_WRPR_CTRL
                                           register.
                                           After updating the FLR_WRPR_CTRL register,the SW should set the mode back to
                                           lock. */
  } field;
  uint32_t val;
} MKS_LKUP_FLR_WRPR_CFG_PROT_t;
#endif /* GENHDR_STRUCT */

/* FLR Wrapper control register */
#define MKS_LKUP_FLR_WRPR_CTRL                                  0x0000011CU
/*
 * Used to enable FLR inband configuration mode.This bit value decides which
 * master is granted the exclusive access to EM Lookup(Flow table) and CT
 * table(Flow Tracker) registers,via the corresponding AvMM slave interface of
 * these modules.
 *  If '1',the registers of these 2 modules can be accessed only via FLR inband
 * configuration packets.This mode should be enabled by S/W during normal
 * operation at the start.
 *  If '0',the registers of these 2 modules can be accessed only via CSR's
 * interface.This is legacy mode and it is intended only for debug purpose.
 *  NOTE: Using Default value as 0x0 - just temporarily for backward
 * compatibility with previous versions of S/W.
 */
#define MKS_LKUP_FLR_WRPR_CTRL_EN_FLR_IB_CFG_MODE_SHIFT         0
#define MKS_LKUP_FLR_WRPR_CTRL_EN_FLR_IB_CFG_MODE_WIDTH         1
#define MKS_LKUP_FLR_WRPR_CTRL_EN_FLR_IB_CFG_MODE_MASK          (0x1U << MKS_LKUP_FLR_WRPR_CTRL_EN_FLR_IB_CFG_MODE_SHIFT)

/* Reserved. */
#define MKS_LKUP_FLR_WRPR_CTRL_RESERVED_SHIFT                   1
#define MKS_LKUP_FLR_WRPR_CTRL_RESERVED_WIDTH                   30
#define MKS_LKUP_FLR_WRPR_CTRL_RESERVED_MASK                    (0x3FFFFFFFU << MKS_LKUP_FLR_WRPR_CTRL_RESERVED_SHIFT)

/* Set to 1 to soft reset the FLR IP core. Must be 0 during normal operation. */
#define MKS_LKUP_FLR_WRPR_CTRL_FLR_SOFT_RST_SHIFT               31
#define MKS_LKUP_FLR_WRPR_CTRL_FLR_SOFT_RST_WIDTH               1
#define MKS_LKUP_FLR_WRPR_CTRL_FLR_SOFT_RST_MASK                (0x1U << MKS_LKUP_FLR_WRPR_CTRL_FLR_SOFT_RST_SHIFT)

#ifdef GENHDR_STRUCT
/* FLR Wrapper control register */
typedef union {
  struct {
    uint32_t en_flr_ib_cfg_mode : 1;    /* Used to enable FLR inband configuration mode.This bit value decides which
                                           master is granted the exclusive access to EM Lookup(Flow table) and CT
                                           table(Flow Tracker) registers,via the corresponding AvMM slave interface of
                                           these modules.
                                           If '1',the registers of these 2 modules can be accessed only via FLR inband
                                           configuration packets.This mode should be enabled by S/W during normal
                                           operation at the start.
                                           If '0',the registers of these 2 modules can be accessed only via CSR's
                                           interface.This is legacy mode and it is intended only for debug purpose.
                                           NOTE: Using Default value as 0x0 - just temporarily for backward
                                           compatibility with previous versions of S/W. */
    uint32_t reserved : 30;             /* Reserved. */
    uint32_t flr_soft_rst : 1;          /* Set to 1 to soft reset the FLR IP core. Must be 0 during normal operation. */
  } field;
  uint32_t val;
} MKS_LKUP_FLR_WRPR_CTRL_t;
#endif /* GENHDR_STRUCT */

/* FLR Wrapper Status register */
#define MKS_LKUP_FLR_WRPR_STAT                                  0x00000120U
/* Reserved for future use. */
#define MKS_LKUP_FLR_WRPR_STAT_FLR_WRPR_STATUS_SHIFT            0
#define MKS_LKUP_FLR_WRPR_STAT_FLR_WRPR_STATUS_WIDTH            32
#define MKS_LKUP_FLR_WRPR_STAT_FLR_WRPR_STATUS_MASK             (0xFFFFFFFFU << MKS_LKUP_FLR_WRPR_STAT_FLR_WRPR_STATUS_SHIFT)

#ifdef GENHDR_STRUCT
/* FLR Wrapper Status register */
typedef union {
  struct {
    uint32_t flr_wrpr_status : 32;      /* Reserved for future use. */
  } field;
  uint32_t val;
} MKS_LKUP_FLR_WRPR_STAT_t;
#endif /* GENHDR_STRUCT */

/* Copy of FT_TIMESTAMP_0 register present in Flow tracker */
#define MKS_LKUP_FT_TIMESTAMP_0_CPY                             0x00000124U
/*
 * A copy of FT_TIMESTAMP_0 register present in Flow tracker.It contains lower
 * 32 bits of Timestamp value.
 *  When Flow tracker is accessed in inband mode, SW can read the Timestamp
 * value through CSR access of this register.
 */
#define MKS_LKUP_FT_TIMESTAMP_0_CPY_FT_TSTMP_CPY_L32_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_FT_TIMESTAMP_0_CPY_FT_TSTMP_CPY_L32_SHIFT      0
#define MKS_LKUP_FT_TIMESTAMP_0_CPY_FT_TSTMP_CPY_L32_WIDTH      32
#define MKS_LKUP_FT_TIMESTAMP_0_CPY_FT_TSTMP_CPY_L32_MASK       (0xFFFFFFFFU << MKS_LKUP_FT_TIMESTAMP_0_CPY_FT_TSTMP_CPY_L32_SHIFT)

#ifdef GENHDR_STRUCT
/* Copy of FT_TIMESTAMP_0 register present in Flow tracker */
typedef union {
  struct {
    uint32_t ft_tstmp_cpy_l32 : 32;     /* A copy of FT_TIMESTAMP_0 register present in Flow tracker.It contains lower
                                           32 bits of Timestamp value.
                                           When Flow tracker is accessed in inband mode, SW can read the Timestamp
                                           value through CSR access of this register. */
  } field;
  uint32_t val;
} MKS_LKUP_FT_TIMESTAMP_0_CPY_t;
#endif /* GENHDR_STRUCT */

/* Copy of FT_TIMESTAMP_1 register present in Flow tracker */
#define MKS_LKUP_FT_TIMESTAMP_1_CPY                             0x00000128U
/*
 * A copy of FT_TIMESTAMP_1 register present in Flow tracker.It contains upper
 * 32 bits of Timestamp value.
 *  When Flow tracker is accessed in inband mode, SW can read the Timestamp
 * value through CSR access of this register.
 *  NOTE: The SW should first read the FT_TIMESTAMP_0_CPY register and then the
 * FT_TIMESTAMP_1_CPY register.
 */
#define MKS_LKUP_FT_TIMESTAMP_1_CPY_FT_TSTMP_CPY_U32_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_LKUP_FT_TIMESTAMP_1_CPY_FT_TSTMP_CPY_U32_SHIFT      0
#define MKS_LKUP_FT_TIMESTAMP_1_CPY_FT_TSTMP_CPY_U32_WIDTH      32
#define MKS_LKUP_FT_TIMESTAMP_1_CPY_FT_TSTMP_CPY_U32_MASK       (0xFFFFFFFFU << MKS_LKUP_FT_TIMESTAMP_1_CPY_FT_TSTMP_CPY_U32_SHIFT)

#ifdef GENHDR_STRUCT
/* Copy of FT_TIMESTAMP_1 register present in Flow tracker */
typedef union {
  struct {
    uint32_t ft_tstmp_cpy_u32 : 32;     /* A copy of FT_TIMESTAMP_1 register present in Flow tracker.It contains upper
                                           32 bits of Timestamp value.
                                           When Flow tracker is accessed in inband mode, SW can read the Timestamp
                                           value through CSR access of this register.
                                           NOTE: The SW should first read the FT_TIMESTAMP_0_CPY register and then the
                                           FT_TIMESTAMP_1_CPY register. */
  } field;
  uint32_t val;
} MKS_LKUP_FT_TIMESTAMP_1_CPY_t;
#endif /* GENHDR_STRUCT */

#endif /* _MKS_LKUP_REGISTERS_H_ */

