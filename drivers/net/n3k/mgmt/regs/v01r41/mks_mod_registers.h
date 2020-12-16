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

#ifndef _MKS_MOD_REGISTERS_H_
#define _MKS_MOD_REGISTERS_H_

/* OVS Modifier */

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

#define MKS_MOD_ACCESSTYPE              GENHDR_ACCESSTYPE_RW /*  Default access type. Access types defined above. */
#define MKS_MOD_REGWIDTH                32 /* Default width of register in bits */
#define MKS_MOD_ACCESSWIDTH             32 /* Default width of access word in bit */


/* General purpose RW register intended for debug use */
#define MKS_MOD_MOD_SCRATCH                     0x00000000U
/* Scratch data. */
#define MKS_MOD_MOD_SCRATCH_DATA_SHIFT          0
#define MKS_MOD_MOD_SCRATCH_DATA_WIDTH          32
#define MKS_MOD_MOD_SCRATCH_DATA_MASK           (0xFFFFFFFFU << MKS_MOD_MOD_SCRATCH_DATA_SHIFT)

#ifdef GENHDR_STRUCT
/* General purpose RW register intended for debug use */
typedef union {
  struct {
    uint32_t data : 32; /* Scratch data. */
  } field;
  uint32_t val;
} MKS_MOD_MOD_SCRATCH_t;
#endif /* GENHDR_STRUCT */

/* Modifier status counter control */
#define MKS_MOD_MOD_STATS_CFG                                   0x00000004U
/*
 * Status read enable:
 *  SW sets this bit to initiate read port status.
 *  HW clears once all counters are loaded to the holding registers.
 */
#define MKS_MOD_MOD_STATS_CFG_STATUS_RD_ENABLE_SHIFT            0
#define MKS_MOD_MOD_STATS_CFG_STATUS_RD_ENABLE_WIDTH            1
#define MKS_MOD_MOD_STATS_CFG_STATUS_RD_ENABLE_MASK             (0x1U << MKS_MOD_MOD_STATS_CFG_STATUS_RD_ENABLE_SHIFT)

/*
 * Status write enable:
 *  SW sets this bit to initiate read port status.
 *  HW clears once all counters are loaded to the holding registers.
 */
#define MKS_MOD_MOD_STATS_CFG_STATUS_WR_ENABLE_SHIFT            1
#define MKS_MOD_MOD_STATS_CFG_STATUS_WR_ENABLE_WIDTH            1
#define MKS_MOD_MOD_STATS_CFG_STATUS_WR_ENABLE_MASK             (0x1U << MKS_MOD_MOD_STATS_CFG_STATUS_WR_ENABLE_SHIFT)

/* Block ID: */
#define MKS_MOD_MOD_STATS_CFG_STATUS_BLK_ID_SHIFT               2
#define MKS_MOD_MOD_STATS_CFG_STATUS_BLK_ID_WIDTH               2
#define MKS_MOD_MOD_STATS_CFG_STATUS_BLK_ID_MASK                (0x3U << MKS_MOD_MOD_STATS_CFG_STATUS_BLK_ID_SHIFT)

/*
 * Modify Status counter Port ID:
 *  127-0: Virtio subPort.
 *  128: MAC0.
 *  129: MAC1.
 *  130: MGMT.
 *  Ingress Arbiter counter stats: statusBlkId==3 
 *  statusAddr[5:3]: Ingress Arbiter blkId
 *  0: Ingress interface pkt counter. 
 *  1: Ingress good pkt to parser. 
 *  2: Ingress error pkt to parser. 
 *  3: Ingress drop pkt counter. 
 *  4: Ingress number of times missing sop event is detected. This counter is
 * used a debug status only and it should be used as part of the packet counter
 * tracking. 
 *  5: Ingress Number of times missing eop evnet is detected. This counter is
 * used a debug status only and it should be used as part of the packet counter
 * tracking. 
 *  6: Ingress Number of cycles PM drop condition is detected. This counter is
 * used a debug status only and it should be used as part of the packet counter
 * tracking. 
 *  7: Ingress Number of cycles PldFifo drop condition is detected. This counter
 * is used a debug status only and it should be used as part of the packet
 * counter tracking. 
 *  statusAddr[1:0]: Ingress PortId. 
 *  0: MAC0. 
 *  1: MAC1. 
 *  2: ARM. 
 *  3: VIRTIO.
 */
#define MKS_MOD_MOD_STATS_CFG_STATUS_ADDR_SHIFT                 4
#define MKS_MOD_MOD_STATS_CFG_STATUS_ADDR_WIDTH                 8
#define MKS_MOD_MOD_STATS_CFG_STATUS_ADDR_MASK                  (0xFFU << MKS_MOD_MOD_STATS_CFG_STATUS_ADDR_SHIFT)

/* when this bit is set it selects the status relating to the tx-mirror status. */
#define MKS_MOD_MOD_STATS_CFG_ST_BLK_ID_RCIRC_SHIFT             12
#define MKS_MOD_MOD_STATS_CFG_ST_BLK_ID_RCIRC_WIDTH             1
#define MKS_MOD_MOD_STATS_CFG_ST_BLK_ID_RCIRC_MASK              (0x1U << MKS_MOD_MOD_STATS_CFG_ST_BLK_ID_RCIRC_SHIFT)

/*
 * 1: Enable to decrement or modify the ipv4 inner header ttl field in the
 * Modify. 
 *  0: Disable decrement or modify the ipv4 inner header ttl field in the
 * Modify.
 */
#define MKS_MOD_MOD_STATS_CFG_IN_TTL_MOD_EN_SHIFT               27
#define MKS_MOD_MOD_STATS_CFG_IN_TTL_MOD_EN_WIDTH               1
#define MKS_MOD_MOD_STATS_CFG_IN_TTL_MOD_EN_MASK                (0x1U << MKS_MOD_MOD_STATS_CFG_IN_TTL_MOD_EN_SHIFT)

/*
 * 1: Enable Internal Port Representor in the Modify. 
 *  0: Disable Internal Port Representor in the Modify. 
 *  NOTE: As per existing RTL implementation, when the intPortRepEn, insert_vlan
 * and insert_n_set bits are all set to 1, then only the Intel header is
 * inserted as expected for both tunnel and non-tunnel packets. And, when the
 * intPortRepEn, strip_vlan and strip_n_fwd bits are all set to 1, then only the
 * Intel header is stripped as expected for both tunnel and non-tunnel packets.
 */
#define MKS_MOD_MOD_STATS_CFG_INT_PORT_REP_EN_SHIFT             28
#define MKS_MOD_MOD_STATS_CFG_INT_PORT_REP_EN_WIDTH             1
#define MKS_MOD_MOD_STATS_CFG_INT_PORT_REP_EN_MASK              (0x1U << MKS_MOD_MOD_STATS_CFG_INT_PORT_REP_EN_SHIFT)

/* Set GRE version during Encap. */
#define MKS_MOD_MOD_STATS_CFG_GRE_VERSION_SHIFT                 29
#define MKS_MOD_MOD_STATS_CFG_GRE_VERSION_WIDTH                 3
#define MKS_MOD_MOD_STATS_CFG_GRE_VERSION_MASK                  (0x7U << MKS_MOD_MOD_STATS_CFG_GRE_VERSION_SHIFT)

#ifdef GENHDR_STRUCT
/* Modifier status counter control */
typedef union {
  struct {
    uint32_t status_rd_enable : 1;      /* Status read enable:
                                           SW sets this bit to initiate read port status.
                                           HW clears once all counters are loaded to the holding registers. */
    uint32_t status_wr_enable : 1;      /* Status write enable:
                                           SW sets this bit to initiate read port status.
                                           HW clears once all counters are loaded to the holding registers. */
    uint32_t status_blk_id : 2;         /* Block ID: */
    uint32_t status_addr : 8;           /* Modify Status counter Port ID:
                                           127-0: Virtio subPort.
                                           128: MAC0.
                                           129: MAC1.
                                           130: MGMT.
                                           Ingress Arbiter counter stats: statusBlkId==3
                                           statusAddr[5:3]: Ingress Arbiter blkId
                                           0: Ingress interface pkt counter.
                                           1: Ingress good pkt to parser.
                                           2: Ingress error pkt to parser.
                                           3: Ingress drop pkt counter.
                                           4: Ingress number of times missing sop event is detected. This counter is
                                           used a debug status only and it should be used as part of the packet counter
                                           tracking.
                                           5: Ingress Number of times missing eop evnet is detected. This counter is
                                           used a debug status only and it should be used as part of the packet counter
                                           tracking.
                                           6: Ingress Number of cycles PM drop condition is detected. This counter is
                                           used a debug status only and it should be used as part of the packet counter
                                           tracking.
                                           7: Ingress Number of cycles PldFifo drop condition is detected. This counter
                                           is used a debug status only and it should be used as part of the packet
                                           counter tracking.
                                           statusAddr[1:0]: Ingress PortId.
                                           0: MAC0.
                                           1: MAC1.
                                           2: ARM.
                                           3: VIRTIO. */
    uint32_t st_blk_id_rcirc : 1;       /* when this bit is set it selects the status relating to the tx-mirror status. */
    uint32_t : 14;
    uint32_t in_ttl_mod_en : 1;         /* 1: Enable to decrement or modify the ipv4 inner header ttl field in the
                                           Modify.
                                           0: Disable decrement or modify the ipv4 inner header ttl field in the
                                           Modify. */
    uint32_t int_port_rep_en : 1;       /* 1: Enable Internal Port Representor in the Modify.
                                           0: Disable Internal Port Representor in the Modify.
                                           NOTE: As per existing RTL implementation, when the intPortRepEn, insert_vlan
                                           and insert_n_set bits are all set to 1, then only the Intel header is
                                           inserted as expected for both tunnel and non-tunnel packets. And, when the
                                           intPortRepEn, strip_vlan and strip_n_fwd bits are all set to 1, then only the
                                           Intel header is stripped as expected for both tunnel and non-tunnel packets. */
    uint32_t gre_version : 3;           /* Set GRE version during Encap. */
  } field;
  uint32_t val;
} MKS_MOD_MOD_STATS_CFG_t;
#endif /* GENHDR_STRUCT */

#define MKS_MOD_MOD_STATS_CFG_STATUS_BLK_ID_RD_UC               0x0U /* Modify Unicast Packet Counter. */
#define MKS_MOD_MOD_STATS_CFG_STATUS_BLK_ID_RD_UMC_BC           0x1U /* Modify Multi/Broadcast Packet Counter. */
#define MKS_MOD_MOD_STATS_CFG_STATUS_BLK_ID_RD_BCNT             0x2U /* Modify Byte Count. */
#define MKS_MOD_MOD_STATS_CFG_STATUS_BLK_ID_ARBCNT              0x3U /* Ingress Arbiter counter stats. */

/* Bits [31:0] of status read/write data */
#define MKS_MOD_MOD_STAT_REG0                           0x00000008U
/* This register contains bit [31:0] of status read data or status write data. */
#define MKS_MOD_MOD_STAT_REG0_BITS_31_0_SHIFT           0
#define MKS_MOD_MOD_STAT_REG0_BITS_31_0_WIDTH           32
#define MKS_MOD_MOD_STAT_REG0_BITS_31_0_MASK            (0xFFFFFFFFU << MKS_MOD_MOD_STAT_REG0_BITS_31_0_SHIFT)

#ifdef GENHDR_STRUCT
/* Bits [31:0] of status read/write data */
typedef union {
  struct {
    uint32_t bits_31_0 : 32;    /* This register contains bit [31:0] of status read data or status write data. */
  } field;
  uint32_t val;
} MKS_MOD_MOD_STAT_REG0_t;
#endif /* GENHDR_STRUCT */

/* Bits [39:32] of status read/write data */
#define MKS_MOD_MOD_STAT_REG1                           0x0000000CU
/* This register contains bit [39:32] of status read data or status write data. */
#define MKS_MOD_MOD_STAT_REG1_BITS_39_32_SHIFT          0
#define MKS_MOD_MOD_STAT_REG1_BITS_39_32_WIDTH          8
#define MKS_MOD_MOD_STAT_REG1_BITS_39_32_MASK           (0xFFU << MKS_MOD_MOD_STAT_REG1_BITS_39_32_SHIFT)

#ifdef GENHDR_STRUCT
/* Bits [39:32] of status read/write data */
typedef union {
  struct {
    uint32_t bits_39_32 : 8;    /* This register contains bit [39:32] of status read data or status write data. */
    uint32_t : 24;
  } field;
  uint32_t val;
} MKS_MOD_MOD_STAT_REG1_t;
#endif /* GENHDR_STRUCT */

/* Modifier error status hold #0 */
#define MKS_MOD_MOD_ERR_HOLD0                                   0x00000010U
/*
 * Output from Parser missing Eop error. Write a one to clear the status bit. 
 *  Bit 0: MAC0.
 *  Bit 1: MAC1.
 *  Bit 2: ARM.
 *  Bit 3 : VirtIO.
 */
#define MKS_MOD_MOD_ERR_HOLD0_PARSER_MISS_EOP_ACCESSTYPE        GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD0_PARSER_MISS_EOP_SHIFT             0
#define MKS_MOD_MOD_ERR_HOLD0_PARSER_MISS_EOP_WIDTH             4
#define MKS_MOD_MOD_ERR_HOLD0_PARSER_MISS_EOP_MASK              (0xFU << MKS_MOD_MOD_ERR_HOLD0_PARSER_MISS_EOP_SHIFT)

/*
 * Output from Parser missing Sop error. Write a one to clear the status bit. 
 *  Bit 0: MAC0.
 *  Bit 1: MAC1.
 *  Bit 2: ARM.
 *  Bit 3 : VirtIO.
 */
#define MKS_MOD_MOD_ERR_HOLD0_PARSER_MISS_SOP_ACCESSTYPE        GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD0_PARSER_MISS_SOP_SHIFT             4
#define MKS_MOD_MOD_ERR_HOLD0_PARSER_MISS_SOP_WIDTH             4
#define MKS_MOD_MOD_ERR_HOLD0_PARSER_MISS_SOP_MASK              (0xFU << MKS_MOD_MOD_ERR_HOLD0_PARSER_MISS_SOP_SHIFT)

/* Packet Alignment Buffer over flow. Write a one to clear the status bit. */
#define MKS_MOD_MOD_ERR_HOLD0_ALN_PORT_OV_ACCESSTYPE            GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD0_ALN_PORT_OV_SHIFT                 12
#define MKS_MOD_MOD_ERR_HOLD0_ALN_PORT_OV_WIDTH                 1
#define MKS_MOD_MOD_ERR_HOLD0_ALN_PORT_OV_MASK                  (0x1U << MKS_MOD_MOD_ERR_HOLD0_ALN_PORT_OV_SHIFT)

/* Packet Alignment Buffer under flow. Write a one to clear the status bit. */
#define MKS_MOD_MOD_ERR_HOLD0_ALN_PORT_UD_ACCESSTYPE            GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD0_ALN_PORT_UD_SHIFT                 13
#define MKS_MOD_MOD_ERR_HOLD0_ALN_PORT_UD_WIDTH                 1
#define MKS_MOD_MOD_ERR_HOLD0_ALN_PORT_UD_MASK                  (0x1U << MKS_MOD_MOD_ERR_HOLD0_ALN_PORT_UD_SHIFT)

/*
 * AVST width adjust 64B32B buffer over flow at Mac0 egress interface.
 *  Write a one to clear the status bit.
 */
#define MKS_MOD_MOD_ERR_HOLD0_EGR_BUF_OVF_PRT_0_ACCESSTYPE      GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD0_EGR_BUF_OVF_PRT_0_SHIFT           14
#define MKS_MOD_MOD_ERR_HOLD0_EGR_BUF_OVF_PRT_0_WIDTH           1
#define MKS_MOD_MOD_ERR_HOLD0_EGR_BUF_OVF_PRT_0_MASK            (0x1U << MKS_MOD_MOD_ERR_HOLD0_EGR_BUF_OVF_PRT_0_SHIFT)

/*
 * AVST width adjust 64B32B buffer over flow at Mac1 egress interface.
 *  Write a one to clear the status bit.
 */
#define MKS_MOD_MOD_ERR_HOLD0_EGR_BUF_OVF_PRT_1_ACCESSTYPE      GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD0_EGR_BUF_OVF_PRT_1_SHIFT           15
#define MKS_MOD_MOD_ERR_HOLD0_EGR_BUF_OVF_PRT_1_WIDTH           1
#define MKS_MOD_MOD_ERR_HOLD0_EGR_BUF_OVF_PRT_1_MASK            (0x1U << MKS_MOD_MOD_ERR_HOLD0_EGR_BUF_OVF_PRT_1_SHIFT)

/*
 * AVST width adjust 64B4B buffer over flow at ARM egress interface.
 *  Write a one to clear the status bit.
 */
#define MKS_MOD_MOD_ERR_HOLD0_EGR_BUF_OVF_PRT_2_ACCESSTYPE      GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD0_EGR_BUF_OVF_PRT_2_SHIFT           16
#define MKS_MOD_MOD_ERR_HOLD0_EGR_BUF_OVF_PRT_2_WIDTH           1
#define MKS_MOD_MOD_ERR_HOLD0_EGR_BUF_OVF_PRT_2_MASK            (0x1U << MKS_MOD_MOD_ERR_HOLD0_EGR_BUF_OVF_PRT_2_SHIFT)

/*
 * Merge Port payload buffer over flow. Write a one to clear the 
 *  status bit.
 */
#define MKS_MOD_MOD_ERR_HOLD0_MERGE_PLD_ON_BUF_OV_ACCESSTYPE    GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD0_MERGE_PLD_ON_BUF_OV_SHIFT         17
#define MKS_MOD_MOD_ERR_HOLD0_MERGE_PLD_ON_BUF_OV_WIDTH         1
#define MKS_MOD_MOD_ERR_HOLD0_MERGE_PLD_ON_BUF_OV_MASK          (0x1U << MKS_MOD_MOD_ERR_HOLD0_MERGE_PLD_ON_BUF_OV_SHIFT)

/*
 * Merge Port header buffer over flow. Write a one to clear the 
 *  status bit.
 */
#define MKS_MOD_MOD_ERR_HOLD0_MERGE_HDR_ON_BUF_OV_ACCESSTYPE    GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD0_MERGE_HDR_ON_BUF_OV_SHIFT         18
#define MKS_MOD_MOD_ERR_HOLD0_MERGE_HDR_ON_BUF_OV_WIDTH         1
#define MKS_MOD_MOD_ERR_HOLD0_MERGE_HDR_ON_BUF_OV_MASK          (0x1U << MKS_MOD_MOD_ERR_HOLD0_MERGE_HDR_ON_BUF_OV_SHIFT)

/*
 * Output from Parser missing Eop error from recirc port. Write a one to clear
 * the status bit. 
 *  Bit 0: MAC0.
 *  Bit 1: MAC1.
 *  Bit 2: ARM.
 *  Bit 3 : VirtIO.
 */
#define MKS_MOD_MOD_ERR_HOLD0_PAR_MISS_EOP_RCR_ACCESSTYPE       GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD0_PAR_MISS_EOP_RCR_SHIFT            20
#define MKS_MOD_MOD_ERR_HOLD0_PAR_MISS_EOP_RCR_WIDTH            4
#define MKS_MOD_MOD_ERR_HOLD0_PAR_MISS_EOP_RCR_MASK             (0xFU << MKS_MOD_MOD_ERR_HOLD0_PAR_MISS_EOP_RCR_SHIFT)

/*
 * Output from Parser missing Sop error from recirc port. Write a one to clear
 * the status bit. 
 *  Bit 0: MAC0.
 *  Bit 1: MAC1.
 *  Bit 2: ARM.
 *  Bit 3 : VirtIO.
 */
#define MKS_MOD_MOD_ERR_HOLD0_PAR_MISS_SOP_RCR_ACCESSTYPE       GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD0_PAR_MISS_SOP_RCR_SHIFT            24
#define MKS_MOD_MOD_ERR_HOLD0_PAR_MISS_SOP_RCR_WIDTH            4
#define MKS_MOD_MOD_ERR_HOLD0_PAR_MISS_SOP_RCR_MASK             (0xFU << MKS_MOD_MOD_ERR_HOLD0_PAR_MISS_SOP_RCR_SHIFT)

#ifdef GENHDR_STRUCT
/* Modifier error status hold #0 */
typedef union {
  struct {
    uint32_t parser_miss_eop : 4;       /* Output from Parser missing Eop error. Write a one to clear the status bit.
                                           Bit 0: MAC0.
                                           Bit 1: MAC1.
                                           Bit 2: ARM.
                                           Bit 3 : VirtIO. */
    uint32_t parser_miss_sop : 4;       /* Output from Parser missing Sop error. Write a one to clear the status bit.
                                           Bit 0: MAC0.
                                           Bit 1: MAC1.
                                           Bit 2: ARM.
                                           Bit 3 : VirtIO. */
    uint32_t : 4;
    uint32_t aln_port_ov : 1;           /* Packet Alignment Buffer over flow. Write a one to clear the status bit. */
    uint32_t aln_port_ud : 1;           /* Packet Alignment Buffer under flow. Write a one to clear the status bit. */
    uint32_t egr_buf_ovf_prt_0 : 1;     /* AVST width adjust 64B32B buffer over flow at Mac0 egress interface.
                                           Write a one to clear the status bit. */
    uint32_t egr_buf_ovf_prt_1 : 1;     /* AVST width adjust 64B32B buffer over flow at Mac1 egress interface.
                                           Write a one to clear the status bit. */
    uint32_t egr_buf_ovf_prt_2 : 1;     /* AVST width adjust 64B4B buffer over flow at ARM egress interface.
                                           Write a one to clear the status bit. */
    uint32_t merge_pld_on_buf_ov : 1;   /* Merge Port payload buffer over flow. Write a one to clear the
                                           status bit. */
    uint32_t merge_hdr_on_buf_ov : 1;   /* Merge Port header buffer over flow. Write a one to clear the
                                           status bit. */
    uint32_t : 1;
    uint32_t par_miss_eop_rcr : 4;      /* Output from Parser missing Eop error from recirc port. Write a one to clear
                                           the status bit.
                                           Bit 0: MAC0.
                                           Bit 1: MAC1.
                                           Bit 2: ARM.
                                           Bit 3 : VirtIO. */
    uint32_t par_miss_sop_rcr : 4;      /* Output from Parser missing Sop error from recirc port. Write a one to clear
                                           the status bit.
                                           Bit 0: MAC0.
                                           Bit 1: MAC1.
                                           Bit 2: ARM.
                                           Bit 3 : VirtIO. */
    uint32_t : 4;
  } field;
  uint32_t val;
} MKS_MOD_MOD_ERR_HOLD0_t;
#endif /* GENHDR_STRUCT */

/* Modifier error status hold #1 */
#define MKS_MOD_MOD_ERR_HOLD1                           0x00000014U
/*
 * Missing Eop Error at Modifier egress interface. Write a one to clear the
 * status bit.
 *  Bit 0: MAC0.
 *  Bit 1: MAC1.
 *  Bit 2: NIOS.
 *  Bit 3 : VirtIO.
 */
#define MKS_MOD_MOD_ERR_HOLD1_EGR_MISS_EOP_ACCESSTYPE   GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD1_EGR_MISS_EOP_SHIFT        0
#define MKS_MOD_MOD_ERR_HOLD1_EGR_MISS_EOP_WIDTH        4
#define MKS_MOD_MOD_ERR_HOLD1_EGR_MISS_EOP_MASK         (0xFU << MKS_MOD_MOD_ERR_HOLD1_EGR_MISS_EOP_SHIFT)

/*
 * Missing Sop Error at Modifier egress interface. Write a one to clear the
 * status bit.
 *  Bit 0: MAC0.
 *  Bit 1: MAC1.
 *  Bit 2: NIOS.
 *  Bit 3 : VirtIO.
 */
#define MKS_MOD_MOD_ERR_HOLD1_EGR_MISS_SOP_ACCESSTYPE   GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD1_EGR_MISS_SOP_SHIFT        4
#define MKS_MOD_MOD_ERR_HOLD1_EGR_MISS_SOP_WIDTH        4
#define MKS_MOD_MOD_ERR_HOLD1_EGR_MISS_SOP_MASK         (0xFU << MKS_MOD_MOD_ERR_HOLD1_EGR_MISS_SOP_SHIFT)

/*
 * Missing Eop Error at the ingress arbiter interface. Write a one to clear the
 * status bit. 
 *  Bit 0: MAC0.
 *  Bit 1: MAC1.
 *  Bit 2: NIOS.
 *  Bit 3 : VirtIO.
 */
#define MKS_MOD_MOD_ERR_HOLD1_INGR_MISS_EOP_ACCESSTYPE  GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD1_INGR_MISS_EOP_SHIFT       12
#define MKS_MOD_MOD_ERR_HOLD1_INGR_MISS_EOP_WIDTH       4
#define MKS_MOD_MOD_ERR_HOLD1_INGR_MISS_EOP_MASK        (0xFU << MKS_MOD_MOD_ERR_HOLD1_INGR_MISS_EOP_SHIFT)

/*
 * Missing Sop Error at the ingress arbiter interface. Write a one to clear the
 * status bit. 
 *  Bit 0: MAC0.
 *  Bit 1: MAC1.
 *  Bit 2: NIOS.
 *  Bit 3 : VirtIO.
 */
#define MKS_MOD_MOD_ERR_HOLD1_INGR_MISS_SOP_ACCESSTYPE  GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD1_INGR_MISS_SOP_SHIFT       16
#define MKS_MOD_MOD_ERR_HOLD1_INGR_MISS_SOP_WIDTH       4
#define MKS_MOD_MOD_ERR_HOLD1_INGR_MISS_SOP_MASK        (0xFU << MKS_MOD_MOD_ERR_HOLD1_INGR_MISS_SOP_SHIFT)

/*
 * Buffer over flow at the ingress arbiter interface. Write a one to clear the
 * status bit. 
 *  Bit 0: MAC0.
 *  Bit 1: MAC1.
 *  Bit 2: ARM.
 *  Bit 3 : VirtIO.
 */
#define MKS_MOD_MOD_ERR_HOLD1_INGR_OV_ERR_ACCESSTYPE    GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD1_INGR_OV_ERR_SHIFT         20
#define MKS_MOD_MOD_ERR_HOLD1_INGR_OV_ERR_WIDTH         4
#define MKS_MOD_MOD_ERR_HOLD1_INGR_OV_ERR_MASK          (0xFU << MKS_MOD_MOD_ERR_HOLD1_INGR_OV_ERR_SHIFT)

/*
 * Transmit buffer overflow. Write a one to clear the status bit. 
 *  Bit 0: MAC0.
 *  Bit 1: MAC1.
 *  Bit 2: NIOS.
 *  Bit 3 : VirtIO.
 */
#define MKS_MOD_MOD_ERR_HOLD1_TX_BUF_OV_ACCESSTYPE      GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD1_TX_BUF_OV_SHIFT           24
#define MKS_MOD_MOD_ERR_HOLD1_TX_BUF_OV_WIDTH           4
#define MKS_MOD_MOD_ERR_HOLD1_TX_BUF_OV_MASK            (0xFU << MKS_MOD_MOD_ERR_HOLD1_TX_BUF_OV_SHIFT)

/*
 * Transmit buffer underflow. Write a one to clear the status bit. 
 *  Bit 0: MAC0.
 *  Bit 1: MAC1.
 *  Bit 2: NIOS.
 *  Bit 3 : VirtIO.
 */
#define MKS_MOD_MOD_ERR_HOLD1_TX_BUF_UD_ACCESSTYPE      GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD1_TX_BUF_UD_SHIFT           28
#define MKS_MOD_MOD_ERR_HOLD1_TX_BUF_UD_WIDTH           4
#define MKS_MOD_MOD_ERR_HOLD1_TX_BUF_UD_MASK            (0xFU << MKS_MOD_MOD_ERR_HOLD1_TX_BUF_UD_SHIFT)

#ifdef GENHDR_STRUCT
/* Modifier error status hold #1 */
typedef union {
  struct {
    uint32_t egr_miss_eop : 4;          /* Missing Eop Error at Modifier egress interface. Write a one to clear the
                                           status bit.
                                           Bit 0: MAC0.
                                           Bit 1: MAC1.
                                           Bit 2: NIOS.
                                           Bit 3 : VirtIO. */
    uint32_t egr_miss_sop : 4;          /* Missing Sop Error at Modifier egress interface. Write a one to clear the
                                           status bit.
                                           Bit 0: MAC0.
                                           Bit 1: MAC1.
                                           Bit 2: NIOS.
                                           Bit 3 : VirtIO. */
    uint32_t : 4;
    uint32_t ingr_miss_eop : 4;         /* Missing Eop Error at the ingress arbiter interface. Write a one to clear the
                                           status bit.
                                           Bit 0: MAC0.
                                           Bit 1: MAC1.
                                           Bit 2: NIOS.
                                           Bit 3 : VirtIO. */
    uint32_t ingr_miss_sop : 4;         /* Missing Sop Error at the ingress arbiter interface. Write a one to clear the
                                           status bit.
                                           Bit 0: MAC0.
                                           Bit 1: MAC1.
                                           Bit 2: NIOS.
                                           Bit 3 : VirtIO. */
    uint32_t ingr_ov_err : 4;           /* Buffer over flow at the ingress arbiter interface. Write a one to clear the
                                           status bit.
                                           Bit 0: MAC0.
                                           Bit 1: MAC1.
                                           Bit 2: ARM.
                                           Bit 3 : VirtIO. */
    uint32_t tx_buf_ov : 4;             /* Transmit buffer overflow. Write a one to clear the status bit.
                                           Bit 0: MAC0.
                                           Bit 1: MAC1.
                                           Bit 2: NIOS.
                                           Bit 3 : VirtIO. */
    uint32_t tx_buf_ud : 4;             /* Transmit buffer underflow. Write a one to clear the status bit.
                                           Bit 0: MAC0.
                                           Bit 1: MAC1.
                                           Bit 2: NIOS.
                                           Bit 3 : VirtIO. */
  } field;
  uint32_t val;
} MKS_MOD_MOD_ERR_HOLD1_t;
#endif /* GENHDR_STRUCT */

/* Port packet counter */
#define MKS_MOD_MOD_PORT_PKT_CNT_0                      0x00000018U
/* Port packet counter. */
#define MKS_MOD_MOD_PORT_PKT_CNT_0_CNT_SHIFT            0
#define MKS_MOD_MOD_PORT_PKT_CNT_0_CNT_WIDTH            32
#define MKS_MOD_MOD_PORT_PKT_CNT_0_CNT_MASK             (0xFFFFFFFFU << MKS_MOD_MOD_PORT_PKT_CNT_0_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Port packet counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Port packet counter. */
  } field;
  uint32_t val;
} MKS_MOD_MOD_PORT_PKT_CNT_0_t;
#endif /* GENHDR_STRUCT */

/* Port packet counter */
#define MKS_MOD_MOD_PORT_PKT_CNT_1                      0x0000001CU
/* Port packet counter. */
#define MKS_MOD_MOD_PORT_PKT_CNT_1_CNT_SHIFT            0
#define MKS_MOD_MOD_PORT_PKT_CNT_1_CNT_WIDTH            32
#define MKS_MOD_MOD_PORT_PKT_CNT_1_CNT_MASK             (0xFFFFFFFFU << MKS_MOD_MOD_PORT_PKT_CNT_1_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Port packet counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Port packet counter. */
  } field;
  uint32_t val;
} MKS_MOD_MOD_PORT_PKT_CNT_1_t;
#endif /* GENHDR_STRUCT */

/* Port packet counter */
#define MKS_MOD_MOD_PORT_PKT_CNT_2                      0x00000020U
/* Port packet counter. */
#define MKS_MOD_MOD_PORT_PKT_CNT_2_CNT_SHIFT            0
#define MKS_MOD_MOD_PORT_PKT_CNT_2_CNT_WIDTH            32
#define MKS_MOD_MOD_PORT_PKT_CNT_2_CNT_MASK             (0xFFFFFFFFU << MKS_MOD_MOD_PORT_PKT_CNT_2_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Port packet counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Port packet counter. */
  } field;
  uint32_t val;
} MKS_MOD_MOD_PORT_PKT_CNT_2_t;
#endif /* GENHDR_STRUCT */

/* Port packet counter */
#define MKS_MOD_MOD_PORT_PKT_CNT_3                      0x00000024U
/* Port packet counter. */
#define MKS_MOD_MOD_PORT_PKT_CNT_3_CNT_SHIFT            0
#define MKS_MOD_MOD_PORT_PKT_CNT_3_CNT_WIDTH            32
#define MKS_MOD_MOD_PORT_PKT_CNT_3_CNT_MASK             (0xFFFFFFFFU << MKS_MOD_MOD_PORT_PKT_CNT_3_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Port packet counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Port packet counter. */
  } field;
  uint32_t val;
} MKS_MOD_MOD_PORT_PKT_CNT_3_t;
#endif /* GENHDR_STRUCT */

/* Configuration Register for Traffic Shaping */
#define MKS_MOD_MOD_SHAPE_CFG_0                                 0x00000028U
/*
 *  Assumption:
 *  Freq = 250 Mhz.
 *  bus_width = 512.
 *  Num of bits for rate shaper = 22.
 *  Unit Rate = (Freq * bus_width) / (2^num_bits_shp_rate).
 *  = 250Mhz * 512 / (2^22).
 *  = 30.517 Kbps.
 *  Unit Pkt Per Second Rate = Freq / (2^num_bits_shp_rate).
 *  = 250 Mhz.
 */
#define MKS_MOD_MOD_SHAPE_CFG_0_CFG_SHP_RATE_SHIFT              0
#define MKS_MOD_MOD_SHAPE_CFG_0_CFG_SHP_RATE_WIDTH              22
#define MKS_MOD_MOD_SHAPE_CFG_0_CFG_SHP_RATE_MASK               (0x3FFFFFU << MKS_MOD_MOD_SHAPE_CFG_0_CFG_SHP_RATE_SHIFT)

/* This config bit enables shaping feature. */
#define MKS_MOD_MOD_SHAPE_CFG_0_RATE_EN_SHIFT                   22
#define MKS_MOD_MOD_SHAPE_CFG_0_RATE_EN_WIDTH                   1
#define MKS_MOD_MOD_SHAPE_CFG_0_RATE_EN_MASK                    (0x1U << MKS_MOD_MOD_SHAPE_CFG_0_RATE_EN_SHIFT)

/*
 *  This config bit configures packet rate.
 *  1: Enable packet shaping (Pps).
 *  0: Enable bit shaping (bps).
 */
#define MKS_MOD_MOD_SHAPE_CFG_0_PKT_RATE_EN_SHIFT               23
#define MKS_MOD_MOD_SHAPE_CFG_0_PKT_RATE_EN_WIDTH               1
#define MKS_MOD_MOD_SHAPE_CFG_0_PKT_RATE_EN_MASK                (0x1U << MKS_MOD_MOD_SHAPE_CFG_0_PKT_RATE_EN_SHIFT)

/*  Extra bytes use for either incrementing or decrementing. */
#define MKS_MOD_MOD_SHAPE_CFG_0_EXTRA_BYTES_SHIFT               24
#define MKS_MOD_MOD_SHAPE_CFG_0_EXTRA_BYTES_WIDTH               6
#define MKS_MOD_MOD_SHAPE_CFG_0_EXTRA_BYTES_MASK                (0x3FU << MKS_MOD_MOD_SHAPE_CFG_0_EXTRA_BYTES_SHIFT)

/*
 *  Increment extra bytes enable. When this bit is set extra bytes in added to
 * the packet length.
 */
#define MKS_MOD_MOD_SHAPE_CFG_0_INC_EXTRA_BYTES_EN_SHIFT        30
#define MKS_MOD_MOD_SHAPE_CFG_0_INC_EXTRA_BYTES_EN_WIDTH        1
#define MKS_MOD_MOD_SHAPE_CFG_0_INC_EXTRA_BYTES_EN_MASK         (0x1U << MKS_MOD_MOD_SHAPE_CFG_0_INC_EXTRA_BYTES_EN_SHIFT)

/*
 *  Decrement extra bytes enable.
 *  When this bit is set extra bytes in subtracted from the packet length.
 */
#define MKS_MOD_MOD_SHAPE_CFG_0_DEC_EXTRA_BYTES_EN_SHIFT        31
#define MKS_MOD_MOD_SHAPE_CFG_0_DEC_EXTRA_BYTES_EN_WIDTH        1
#define MKS_MOD_MOD_SHAPE_CFG_0_DEC_EXTRA_BYTES_EN_MASK         (0x1U << MKS_MOD_MOD_SHAPE_CFG_0_DEC_EXTRA_BYTES_EN_SHIFT)

#ifdef GENHDR_STRUCT
/* Configuration Register for Traffic Shaping */
typedef union {
  struct {
    uint32_t cfg_shp_rate : 22;         /* Assumption:
                                           Freq = 250 Mhz.
                                           bus_width = 512.
                                           Num of bits for rate shaper = 22.
                                           Unit Rate = (Freq * bus_width) / (2^num_bits_shp_rate).
                                           = 250Mhz * 512 / (2^22).
                                           = 30.517 Kbps.
                                           Unit Pkt Per Second Rate = Freq / (2^num_bits_shp_rate).
                                           = 250 Mhz. */
    uint32_t rate_en : 1;               /* This config bit enables shaping feature. */
    uint32_t pkt_rate_en : 1;           /* This config bit configures packet rate.
                                           1: Enable packet shaping (Pps).
                                           0: Enable bit shaping (bps). */
    uint32_t extra_bytes : 6;           /* Extra bytes use for either incrementing or decrementing. */
    uint32_t inc_extra_bytes_en : 1;    /* Increment extra bytes enable. When this bit is set extra bytes in added to
                                           the packet length. */
    uint32_t dec_extra_bytes_en : 1;    /* Decrement extra bytes enable.
                                           When this bit is set extra bytes in subtracted from the packet length. */
  } field;
  uint32_t val;
} MKS_MOD_MOD_SHAPE_CFG_0_t;
#endif /* GENHDR_STRUCT */

/* Configuration Register for Traffic Shaping */
#define MKS_MOD_MOD_SHAPE_CFG_1                                 0x0000002CU
/*
 *  Assumption:
 *  Freq = 250 Mhz.
 *  bus_width = 512.
 *  Num of bits for rate shaper = 22.
 *  Unit Rate = (Freq * bus_width) / (2^num_bits_shp_rate).
 *  = 250Mhz * 512 / (2^22).
 *  = 30.517 Kbps.
 *  Unit Pkt Per Second Rate = Freq / (2^num_bits_shp_rate).
 *  = 250 Mhz.
 */
#define MKS_MOD_MOD_SHAPE_CFG_1_CFG_SHP_RATE_SHIFT              0
#define MKS_MOD_MOD_SHAPE_CFG_1_CFG_SHP_RATE_WIDTH              22
#define MKS_MOD_MOD_SHAPE_CFG_1_CFG_SHP_RATE_MASK               (0x3FFFFFU << MKS_MOD_MOD_SHAPE_CFG_1_CFG_SHP_RATE_SHIFT)

/* This config bit enables shaping feature. */
#define MKS_MOD_MOD_SHAPE_CFG_1_RATE_EN_SHIFT                   22
#define MKS_MOD_MOD_SHAPE_CFG_1_RATE_EN_WIDTH                   1
#define MKS_MOD_MOD_SHAPE_CFG_1_RATE_EN_MASK                    (0x1U << MKS_MOD_MOD_SHAPE_CFG_1_RATE_EN_SHIFT)

/*
 *  This config bit configures packet rate.
 *  1: Enable packet shaping (Pps).
 *  0: Enable bit shaping (bps).
 */
#define MKS_MOD_MOD_SHAPE_CFG_1_PKT_RATE_EN_SHIFT               23
#define MKS_MOD_MOD_SHAPE_CFG_1_PKT_RATE_EN_WIDTH               1
#define MKS_MOD_MOD_SHAPE_CFG_1_PKT_RATE_EN_MASK                (0x1U << MKS_MOD_MOD_SHAPE_CFG_1_PKT_RATE_EN_SHIFT)

/*  Extra bytes use for either incrementing or decrementing. */
#define MKS_MOD_MOD_SHAPE_CFG_1_EXTRA_BYTES_SHIFT               24
#define MKS_MOD_MOD_SHAPE_CFG_1_EXTRA_BYTES_WIDTH               6
#define MKS_MOD_MOD_SHAPE_CFG_1_EXTRA_BYTES_MASK                (0x3FU << MKS_MOD_MOD_SHAPE_CFG_1_EXTRA_BYTES_SHIFT)

/*
 *  Increment extra bytes enable. When this bit is set extra bytes in added to
 * the packet length.
 */
#define MKS_MOD_MOD_SHAPE_CFG_1_INC_EXTRA_BYTES_EN_SHIFT        30
#define MKS_MOD_MOD_SHAPE_CFG_1_INC_EXTRA_BYTES_EN_WIDTH        1
#define MKS_MOD_MOD_SHAPE_CFG_1_INC_EXTRA_BYTES_EN_MASK         (0x1U << MKS_MOD_MOD_SHAPE_CFG_1_INC_EXTRA_BYTES_EN_SHIFT)

/*
 *  Decrement extra bytes enable.
 *  When this bit is set extra bytes in subtracted from the packet length.
 */
#define MKS_MOD_MOD_SHAPE_CFG_1_DEC_EXTRA_BYTES_EN_SHIFT        31
#define MKS_MOD_MOD_SHAPE_CFG_1_DEC_EXTRA_BYTES_EN_WIDTH        1
#define MKS_MOD_MOD_SHAPE_CFG_1_DEC_EXTRA_BYTES_EN_MASK         (0x1U << MKS_MOD_MOD_SHAPE_CFG_1_DEC_EXTRA_BYTES_EN_SHIFT)

#ifdef GENHDR_STRUCT
/* Configuration Register for Traffic Shaping */
typedef union {
  struct {
    uint32_t cfg_shp_rate : 22;         /* Assumption:
                                           Freq = 250 Mhz.
                                           bus_width = 512.
                                           Num of bits for rate shaper = 22.
                                           Unit Rate = (Freq * bus_width) / (2^num_bits_shp_rate).
                                           = 250Mhz * 512 / (2^22).
                                           = 30.517 Kbps.
                                           Unit Pkt Per Second Rate = Freq / (2^num_bits_shp_rate).
                                           = 250 Mhz. */
    uint32_t rate_en : 1;               /* This config bit enables shaping feature. */
    uint32_t pkt_rate_en : 1;           /* This config bit configures packet rate.
                                           1: Enable packet shaping (Pps).
                                           0: Enable bit shaping (bps). */
    uint32_t extra_bytes : 6;           /* Extra bytes use for either incrementing or decrementing. */
    uint32_t inc_extra_bytes_en : 1;    /* Increment extra bytes enable. When this bit is set extra bytes in added to
                                           the packet length. */
    uint32_t dec_extra_bytes_en : 1;    /* Decrement extra bytes enable.
                                           When this bit is set extra bytes in subtracted from the packet length. */
  } field;
  uint32_t val;
} MKS_MOD_MOD_SHAPE_CFG_1_t;
#endif /* GENHDR_STRUCT */

/* Configuration Register for Traffic Shaping */
#define MKS_MOD_MOD_SHAPE_CFG_2                                 0x00000030U
/*
 *  Assumption:
 *  Freq = 250 Mhz.
 *  bus_width = 512.
 *  Num of bits for rate shaper = 22.
 *  Unit Rate = (Freq * bus_width) / (2^num_bits_shp_rate).
 *  = 250Mhz * 512 / (2^22).
 *  = 30.517 Kbps.
 *  Unit Pkt Per Second Rate = Freq / (2^num_bits_shp_rate).
 *  = 250 Mhz.
 */
#define MKS_MOD_MOD_SHAPE_CFG_2_CFG_SHP_RATE_SHIFT              0
#define MKS_MOD_MOD_SHAPE_CFG_2_CFG_SHP_RATE_WIDTH              22
#define MKS_MOD_MOD_SHAPE_CFG_2_CFG_SHP_RATE_MASK               (0x3FFFFFU << MKS_MOD_MOD_SHAPE_CFG_2_CFG_SHP_RATE_SHIFT)

/* This config bit enables shaping feature. */
#define MKS_MOD_MOD_SHAPE_CFG_2_RATE_EN_SHIFT                   22
#define MKS_MOD_MOD_SHAPE_CFG_2_RATE_EN_WIDTH                   1
#define MKS_MOD_MOD_SHAPE_CFG_2_RATE_EN_MASK                    (0x1U << MKS_MOD_MOD_SHAPE_CFG_2_RATE_EN_SHIFT)

/*
 *  This config bit configures packet rate.
 *  1: Enable packet shaping (Pps).
 *  0: Enable bit shaping (bps).
 */
#define MKS_MOD_MOD_SHAPE_CFG_2_PKT_RATE_EN_SHIFT               23
#define MKS_MOD_MOD_SHAPE_CFG_2_PKT_RATE_EN_WIDTH               1
#define MKS_MOD_MOD_SHAPE_CFG_2_PKT_RATE_EN_MASK                (0x1U << MKS_MOD_MOD_SHAPE_CFG_2_PKT_RATE_EN_SHIFT)

/*  Extra bytes use for either incrementing or decrementing. */
#define MKS_MOD_MOD_SHAPE_CFG_2_EXTRA_BYTES_SHIFT               24
#define MKS_MOD_MOD_SHAPE_CFG_2_EXTRA_BYTES_WIDTH               6
#define MKS_MOD_MOD_SHAPE_CFG_2_EXTRA_BYTES_MASK                (0x3FU << MKS_MOD_MOD_SHAPE_CFG_2_EXTRA_BYTES_SHIFT)

/*
 *  Increment extra bytes enable. When this bit is set extra bytes in added to
 * the packet length.
 */
#define MKS_MOD_MOD_SHAPE_CFG_2_INC_EXTRA_BYTES_EN_SHIFT        30
#define MKS_MOD_MOD_SHAPE_CFG_2_INC_EXTRA_BYTES_EN_WIDTH        1
#define MKS_MOD_MOD_SHAPE_CFG_2_INC_EXTRA_BYTES_EN_MASK         (0x1U << MKS_MOD_MOD_SHAPE_CFG_2_INC_EXTRA_BYTES_EN_SHIFT)

/*
 *  Decrement extra bytes enable.
 *  When this bit is set extra bytes in subtracted from the packet length.
 */
#define MKS_MOD_MOD_SHAPE_CFG_2_DEC_EXTRA_BYTES_EN_SHIFT        31
#define MKS_MOD_MOD_SHAPE_CFG_2_DEC_EXTRA_BYTES_EN_WIDTH        1
#define MKS_MOD_MOD_SHAPE_CFG_2_DEC_EXTRA_BYTES_EN_MASK         (0x1U << MKS_MOD_MOD_SHAPE_CFG_2_DEC_EXTRA_BYTES_EN_SHIFT)

#ifdef GENHDR_STRUCT
/* Configuration Register for Traffic Shaping */
typedef union {
  struct {
    uint32_t cfg_shp_rate : 22;         /* Assumption:
                                           Freq = 250 Mhz.
                                           bus_width = 512.
                                           Num of bits for rate shaper = 22.
                                           Unit Rate = (Freq * bus_width) / (2^num_bits_shp_rate).
                                           = 250Mhz * 512 / (2^22).
                                           = 30.517 Kbps.
                                           Unit Pkt Per Second Rate = Freq / (2^num_bits_shp_rate).
                                           = 250 Mhz. */
    uint32_t rate_en : 1;               /* This config bit enables shaping feature. */
    uint32_t pkt_rate_en : 1;           /* This config bit configures packet rate.
                                           1: Enable packet shaping (Pps).
                                           0: Enable bit shaping (bps). */
    uint32_t extra_bytes : 6;           /* Extra bytes use for either incrementing or decrementing. */
    uint32_t inc_extra_bytes_en : 1;    /* Increment extra bytes enable. When this bit is set extra bytes in added to
                                           the packet length. */
    uint32_t dec_extra_bytes_en : 1;    /* Decrement extra bytes enable.
                                           When this bit is set extra bytes in subtracted from the packet length. */
  } field;
  uint32_t val;
} MKS_MOD_MOD_SHAPE_CFG_2_t;
#endif /* GENHDR_STRUCT */

/* Configuration Register for Traffic Shaping */
#define MKS_MOD_MOD_SHAPE_CFG_3                                 0x00000034U
/*
 *  Assumption:
 *  Freq = 250 Mhz.
 *  bus_width = 512.
 *  Num of bits for rate shaper = 22.
 *  Unit Rate = (Freq * bus_width) / (2^num_bits_shp_rate).
 *  = 250Mhz * 512 / (2^22).
 *  = 30.517 Kbps.
 *  Unit Pkt Per Second Rate = Freq / (2^num_bits_shp_rate).
 *  = 250 Mhz.
 */
#define MKS_MOD_MOD_SHAPE_CFG_3_CFG_SHP_RATE_SHIFT              0
#define MKS_MOD_MOD_SHAPE_CFG_3_CFG_SHP_RATE_WIDTH              22
#define MKS_MOD_MOD_SHAPE_CFG_3_CFG_SHP_RATE_MASK               (0x3FFFFFU << MKS_MOD_MOD_SHAPE_CFG_3_CFG_SHP_RATE_SHIFT)

/* This config bit enables shaping feature. */
#define MKS_MOD_MOD_SHAPE_CFG_3_RATE_EN_SHIFT                   22
#define MKS_MOD_MOD_SHAPE_CFG_3_RATE_EN_WIDTH                   1
#define MKS_MOD_MOD_SHAPE_CFG_3_RATE_EN_MASK                    (0x1U << MKS_MOD_MOD_SHAPE_CFG_3_RATE_EN_SHIFT)

/*
 *  This config bit configures packet rate.
 *  1: Enable packet shaping (Pps).
 *  0: Enable bit shaping (bps).
 */
#define MKS_MOD_MOD_SHAPE_CFG_3_PKT_RATE_EN_SHIFT               23
#define MKS_MOD_MOD_SHAPE_CFG_3_PKT_RATE_EN_WIDTH               1
#define MKS_MOD_MOD_SHAPE_CFG_3_PKT_RATE_EN_MASK                (0x1U << MKS_MOD_MOD_SHAPE_CFG_3_PKT_RATE_EN_SHIFT)

/*  Extra bytes use for either incrementing or decrementing. */
#define MKS_MOD_MOD_SHAPE_CFG_3_EXTRA_BYTES_SHIFT               24
#define MKS_MOD_MOD_SHAPE_CFG_3_EXTRA_BYTES_WIDTH               6
#define MKS_MOD_MOD_SHAPE_CFG_3_EXTRA_BYTES_MASK                (0x3FU << MKS_MOD_MOD_SHAPE_CFG_3_EXTRA_BYTES_SHIFT)

/*
 *  Increment extra bytes enable. When this bit is set extra bytes in added to
 * the packet length.
 */
#define MKS_MOD_MOD_SHAPE_CFG_3_INC_EXTRA_BYTES_EN_SHIFT        30
#define MKS_MOD_MOD_SHAPE_CFG_3_INC_EXTRA_BYTES_EN_WIDTH        1
#define MKS_MOD_MOD_SHAPE_CFG_3_INC_EXTRA_BYTES_EN_MASK         (0x1U << MKS_MOD_MOD_SHAPE_CFG_3_INC_EXTRA_BYTES_EN_SHIFT)

/*
 *  Decrement extra bytes enable.
 *  When this bit is set extra bytes in subtracted from the packet length.
 */
#define MKS_MOD_MOD_SHAPE_CFG_3_DEC_EXTRA_BYTES_EN_SHIFT        31
#define MKS_MOD_MOD_SHAPE_CFG_3_DEC_EXTRA_BYTES_EN_WIDTH        1
#define MKS_MOD_MOD_SHAPE_CFG_3_DEC_EXTRA_BYTES_EN_MASK         (0x1U << MKS_MOD_MOD_SHAPE_CFG_3_DEC_EXTRA_BYTES_EN_SHIFT)

#ifdef GENHDR_STRUCT
/* Configuration Register for Traffic Shaping */
typedef union {
  struct {
    uint32_t cfg_shp_rate : 22;         /* Assumption:
                                           Freq = 250 Mhz.
                                           bus_width = 512.
                                           Num of bits for rate shaper = 22.
                                           Unit Rate = (Freq * bus_width) / (2^num_bits_shp_rate).
                                           = 250Mhz * 512 / (2^22).
                                           = 30.517 Kbps.
                                           Unit Pkt Per Second Rate = Freq / (2^num_bits_shp_rate).
                                           = 250 Mhz. */
    uint32_t rate_en : 1;               /* This config bit enables shaping feature. */
    uint32_t pkt_rate_en : 1;           /* This config bit configures packet rate.
                                           1: Enable packet shaping (Pps).
                                           0: Enable bit shaping (bps). */
    uint32_t extra_bytes : 6;           /* Extra bytes use for either incrementing or decrementing. */
    uint32_t inc_extra_bytes_en : 1;    /* Increment extra bytes enable. When this bit is set extra bytes in added to
                                           the packet length. */
    uint32_t dec_extra_bytes_en : 1;    /* Decrement extra bytes enable.
                                           When this bit is set extra bytes in subtracted from the packet length. */
  } field;
  uint32_t val;
} MKS_MOD_MOD_SHAPE_CFG_3_t;
#endif /* GENHDR_STRUCT */

/*
 * Counts number cycle that bus width translator module has data to send but it
 * is being stalled
 */
#define MKS_MOD_MOD_EGR_STLCNT_0                0x00000038U
/*
 *  Counts number cycle that bus width translator module has data 
 *  to send but it is being stalled.
 */
#define MKS_MOD_MOD_EGR_STLCNT_0_CNT_SHIFT      0
#define MKS_MOD_MOD_EGR_STLCNT_0_CNT_WIDTH      32
#define MKS_MOD_MOD_EGR_STLCNT_0_CNT_MASK       (0xFFFFFFFFU << MKS_MOD_MOD_EGR_STLCNT_0_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Counts number cycle that bus width translator module has data to send but it
 * is being stalled
 */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number cycle that bus width translator module has data
                           to send but it is being stalled. */
  } field;
  uint32_t val;
} MKS_MOD_MOD_EGR_STLCNT_0_t;
#endif /* GENHDR_STRUCT */

/*
 * Counts number cycle that bus width translator module has data to send but it
 * is being stalled
 */
#define MKS_MOD_MOD_EGR_STLCNT_1                0x0000003CU
/*
 *  Counts number cycle that bus width translator module has data 
 *  to send but it is being stalled.
 */
#define MKS_MOD_MOD_EGR_STLCNT_1_CNT_SHIFT      0
#define MKS_MOD_MOD_EGR_STLCNT_1_CNT_WIDTH      32
#define MKS_MOD_MOD_EGR_STLCNT_1_CNT_MASK       (0xFFFFFFFFU << MKS_MOD_MOD_EGR_STLCNT_1_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Counts number cycle that bus width translator module has data to send but it
 * is being stalled
 */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number cycle that bus width translator module has data
                           to send but it is being stalled. */
  } field;
  uint32_t val;
} MKS_MOD_MOD_EGR_STLCNT_1_t;
#endif /* GENHDR_STRUCT */

/*
 * Counts number cycle that bus width translator module has data to send but it
 * is being stalled
 */
#define MKS_MOD_MOD_EGR_STLCNT_2                0x00000040U
/*
 *  Counts number cycle that bus width translator module has data 
 *  to send but it is being stalled.
 */
#define MKS_MOD_MOD_EGR_STLCNT_2_CNT_SHIFT      0
#define MKS_MOD_MOD_EGR_STLCNT_2_CNT_WIDTH      32
#define MKS_MOD_MOD_EGR_STLCNT_2_CNT_MASK       (0xFFFFFFFFU << MKS_MOD_MOD_EGR_STLCNT_2_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Counts number cycle that bus width translator module has data to send but it
 * is being stalled
 */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Counts number cycle that bus width translator module has data
                           to send but it is being stalled. */
  } field;
  uint32_t val;
} MKS_MOD_MOD_EGR_STLCNT_2_t;
#endif /* GENHDR_STRUCT */

/*  Tx Buffer Xoff threshold for each port */
#define MKS_MOD_MOD_TXBUF_THR                                   0x00000044U
/*  Tx Buffer Mac 0 Xoff threshold. */
#define MKS_MOD_MOD_TXBUF_THR_TX_BUF_XOFF_THR_0_SHIFT           0
#define MKS_MOD_MOD_TXBUF_THR_TX_BUF_XOFF_THR_0_WIDTH           7
#define MKS_MOD_MOD_TXBUF_THR_TX_BUF_XOFF_THR_0_MASK            (0x7FU << MKS_MOD_MOD_TXBUF_THR_TX_BUF_XOFF_THR_0_SHIFT)

/*  Tx Buffer Mac 1 Xoff threshold. */
#define MKS_MOD_MOD_TXBUF_THR_TX_BUF_XOFF_THR_1_SHIFT           8
#define MKS_MOD_MOD_TXBUF_THR_TX_BUF_XOFF_THR_1_WIDTH           7
#define MKS_MOD_MOD_TXBUF_THR_TX_BUF_XOFF_THR_1_MASK            (0x7FU << MKS_MOD_MOD_TXBUF_THR_TX_BUF_XOFF_THR_1_SHIFT)

/*  Tx Buffer Mac 2/Management Xoff threshold. */
#define MKS_MOD_MOD_TXBUF_THR_TX_BUF_XOFF_THR_2_SHIFT           16
#define MKS_MOD_MOD_TXBUF_THR_TX_BUF_XOFF_THR_2_WIDTH           7
#define MKS_MOD_MOD_TXBUF_THR_TX_BUF_XOFF_THR_2_MASK            (0x7FU << MKS_MOD_MOD_TXBUF_THR_TX_BUF_XOFF_THR_2_SHIFT)

/*  Tx Buffer VirtIO Xoff threshold. */
#define MKS_MOD_MOD_TXBUF_THR_TX_BUF_XOFF_THR_3_SHIFT           24
#define MKS_MOD_MOD_TXBUF_THR_TX_BUF_XOFF_THR_3_WIDTH           7
#define MKS_MOD_MOD_TXBUF_THR_TX_BUF_XOFF_THR_3_MASK            (0x7FU << MKS_MOD_MOD_TXBUF_THR_TX_BUF_XOFF_THR_3_SHIFT)

#ifdef GENHDR_STRUCT
/*  Tx Buffer Xoff threshold for each port */
typedef union {
  struct {
    uint32_t tx_buf_xoff_thr_0 : 7;     /* Tx Buffer Mac 0 Xoff threshold. */
    uint32_t : 1;
    uint32_t tx_buf_xoff_thr_1 : 7;     /* Tx Buffer Mac 1 Xoff threshold. */
    uint32_t : 1;
    uint32_t tx_buf_xoff_thr_2 : 7;     /* Tx Buffer Mac 2/Management Xoff threshold. */
    uint32_t : 1;
    uint32_t tx_buf_xoff_thr_3 : 7;     /* Tx Buffer VirtIO Xoff threshold. */
    uint32_t : 1;
  } field;
  uint32_t val;
} MKS_MOD_MOD_TXBUF_THR_t;
#endif /* GENHDR_STRUCT */

/* Tx-Mirror Port packet counter */
#define MKS_MOD_MOD_RECIRC_PKT_CNT_0                    0x00000048U
/* Port packet counter for Tx-Mirror. */
#define MKS_MOD_MOD_RECIRC_PKT_CNT_0_CNT_SHIFT          0
#define MKS_MOD_MOD_RECIRC_PKT_CNT_0_CNT_WIDTH          32
#define MKS_MOD_MOD_RECIRC_PKT_CNT_0_CNT_MASK           (0xFFFFFFFFU << MKS_MOD_MOD_RECIRC_PKT_CNT_0_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx-Mirror Port packet counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Port packet counter for Tx-Mirror. */
  } field;
  uint32_t val;
} MKS_MOD_MOD_RECIRC_PKT_CNT_0_t;
#endif /* GENHDR_STRUCT */

/* Tx-Mirror Port packet counter */
#define MKS_MOD_MOD_RECIRC_PKT_CNT_1                    0x0000004CU
/* Port packet counter for Tx-Mirror. */
#define MKS_MOD_MOD_RECIRC_PKT_CNT_1_CNT_SHIFT          0
#define MKS_MOD_MOD_RECIRC_PKT_CNT_1_CNT_WIDTH          32
#define MKS_MOD_MOD_RECIRC_PKT_CNT_1_CNT_MASK           (0xFFFFFFFFU << MKS_MOD_MOD_RECIRC_PKT_CNT_1_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx-Mirror Port packet counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Port packet counter for Tx-Mirror. */
  } field;
  uint32_t val;
} MKS_MOD_MOD_RECIRC_PKT_CNT_1_t;
#endif /* GENHDR_STRUCT */

/* Tx-Mirror Port packet counter */
#define MKS_MOD_MOD_RECIRC_PKT_CNT_2                    0x00000050U
/* Port packet counter for Tx-Mirror. */
#define MKS_MOD_MOD_RECIRC_PKT_CNT_2_CNT_SHIFT          0
#define MKS_MOD_MOD_RECIRC_PKT_CNT_2_CNT_WIDTH          32
#define MKS_MOD_MOD_RECIRC_PKT_CNT_2_CNT_MASK           (0xFFFFFFFFU << MKS_MOD_MOD_RECIRC_PKT_CNT_2_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx-Mirror Port packet counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Port packet counter for Tx-Mirror. */
  } field;
  uint32_t val;
} MKS_MOD_MOD_RECIRC_PKT_CNT_2_t;
#endif /* GENHDR_STRUCT */

/* Tx-Mirror Port packet counter */
#define MKS_MOD_MOD_RECIRC_PKT_CNT_3                    0x00000054U
/* Port packet counter for Tx-Mirror. */
#define MKS_MOD_MOD_RECIRC_PKT_CNT_3_CNT_SHIFT          0
#define MKS_MOD_MOD_RECIRC_PKT_CNT_3_CNT_WIDTH          32
#define MKS_MOD_MOD_RECIRC_PKT_CNT_3_CNT_MASK           (0xFFFFFFFFU << MKS_MOD_MOD_RECIRC_PKT_CNT_3_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx-Mirror Port packet counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Port packet counter for Tx-Mirror. */
  } field;
  uint32_t val;
} MKS_MOD_MOD_RECIRC_PKT_CNT_3_t;
#endif /* GENHDR_STRUCT */

/* Modifier error status hold #2 */
#define MKS_MOD_MOD_ERR_HOLD2                                   0x00000058U
/*
 * Missing Eop Error at the ingress arbiter interface from recirc interface.
 * Write a one to clear the status bit. 
 *  Bit 0: MAC0.
 *  Bit 1: MAC1.
 *  Bit 2: NIOS.
 *  Bit 3 : VirtIO.
 */
#define MKS_MOD_MOD_ERR_HOLD2_REC_IGR_MISS_EOP_ACCESSTYPE       GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD2_REC_IGR_MISS_EOP_SHIFT            0
#define MKS_MOD_MOD_ERR_HOLD2_REC_IGR_MISS_EOP_WIDTH            4
#define MKS_MOD_MOD_ERR_HOLD2_REC_IGR_MISS_EOP_MASK             (0xFU << MKS_MOD_MOD_ERR_HOLD2_REC_IGR_MISS_EOP_SHIFT)

/*
 * Missing Sop Error at the ingress arbiter interface from recirc interface.
 * Write a one to clear the status bit. 
 *  Bit 0: MAC0.
 *  Bit 1: MAC1.
 *  Bit 2: NIOS.
 *  Bit 3 : VirtIO.
 */
#define MKS_MOD_MOD_ERR_HOLD2_REC_IGR_MISS_SOP_ACCESSTYPE       GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD2_REC_IGR_MISS_SOP_SHIFT            4
#define MKS_MOD_MOD_ERR_HOLD2_REC_IGR_MISS_SOP_WIDTH            4
#define MKS_MOD_MOD_ERR_HOLD2_REC_IGR_MISS_SOP_MASK             (0xFU << MKS_MOD_MOD_ERR_HOLD2_REC_IGR_MISS_SOP_SHIFT)

/*
 * Buffer over flow at the ingress arbiter interface from recirc interface.
 * Write a one to clear the status bit. 
 *  Bit 0: MAC0.
 *  Bit 1: MAC1.
 *  Bit 2: ARM.
 *  Bit 3 : VirtIO.
 */
#define MKS_MOD_MOD_ERR_HOLD2_REC_IGR_OV_ERR_ACCESSTYPE         GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD2_REC_IGR_OV_ERR_SHIFT              8
#define MKS_MOD_MOD_ERR_HOLD2_REC_IGR_OV_ERR_WIDTH              4
#define MKS_MOD_MOD_ERR_HOLD2_REC_IGR_OV_ERR_MASK               (0xFU << MKS_MOD_MOD_ERR_HOLD2_REC_IGR_OV_ERR_SHIFT)

/*
 * Missing Sop Error from ingress arbiter to parser interface. Write a one to
 * clear the status bit. 
 *  Bit 0: MAC0.
 *  Bit 1: MAC1.
 *  Bit 2: NIOS.
 *  Bit 3 : VirtIO.
 */
#define MKS_MOD_MOD_ERR_HOLD2_IGR_ARB_2_PAR_MSOP_ACCESSTYPE     GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD2_IGR_ARB_2_PAR_MSOP_SHIFT          12
#define MKS_MOD_MOD_ERR_HOLD2_IGR_ARB_2_PAR_MSOP_WIDTH          4
#define MKS_MOD_MOD_ERR_HOLD2_IGR_ARB_2_PAR_MSOP_MASK           (0xFU << MKS_MOD_MOD_ERR_HOLD2_IGR_ARB_2_PAR_MSOP_SHIFT)

/*
 * Missing Eop Error from ingress arbiter to parser interface. Write a one to
 * clear the status bit. 
 *  Bit 0: MAC0.
 *  Bit 1: MAC1.
 *  Bit 2: NIOS.
 *  Bit 3 : VirtIO.
 */
#define MKS_MOD_MOD_ERR_HOLD2_IGR_ARB_2_PAR_MEOP_ACCESSTYPE     GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD2_IGR_ARB_2_PAR_MEOP_SHIFT          16
#define MKS_MOD_MOD_ERR_HOLD2_IGR_ARB_2_PAR_MEOP_WIDTH          4
#define MKS_MOD_MOD_ERR_HOLD2_IGR_ARB_2_PAR_MEOP_MASK           (0xFU << MKS_MOD_MOD_ERR_HOLD2_IGR_ARB_2_PAR_MEOP_SHIFT)

/*
 * Missing Sop Error from ingress arbiter to parser from recirc interfacce.
 * Write a one to clear the status bit. 
 *  Bit 0: MAC0.
 *  Bit 1: MAC1.
 *  Bit 2: NIOS.
 *  Bit 3 : VirtIO.
 */
#define MKS_MOD_MOD_ERR_HOLD2_REC_IARB_2_PAR_MSOP_ACCESSTYPE    GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD2_REC_IARB_2_PAR_MSOP_SHIFT         20
#define MKS_MOD_MOD_ERR_HOLD2_REC_IARB_2_PAR_MSOP_WIDTH         4
#define MKS_MOD_MOD_ERR_HOLD2_REC_IARB_2_PAR_MSOP_MASK          (0xFU << MKS_MOD_MOD_ERR_HOLD2_REC_IARB_2_PAR_MSOP_SHIFT)

/*
 * Missing Eop Error from ingress arbiter to parser interface. Write a one to
 * clear the status bit. 
 *  Bit 0: MAC0.
 *  Bit 1: MAC1.
 *  Bit 2: NIOS.
 *  Bit 3 : VirtIO.
 */
#define MKS_MOD_MOD_ERR_HOLD2_REC_IARB_2_PAR_MEOP_ACCESSTYPE    GENHDR_ACCESSTYPE_W1C
#define MKS_MOD_MOD_ERR_HOLD2_REC_IARB_2_PAR_MEOP_SHIFT         24
#define MKS_MOD_MOD_ERR_HOLD2_REC_IARB_2_PAR_MEOP_WIDTH         4
#define MKS_MOD_MOD_ERR_HOLD2_REC_IARB_2_PAR_MEOP_MASK          (0xFU << MKS_MOD_MOD_ERR_HOLD2_REC_IARB_2_PAR_MEOP_SHIFT)

#ifdef GENHDR_STRUCT
/* Modifier error status hold #2 */
typedef union {
  struct {
    uint32_t rec_igr_miss_eop : 4;      /* Missing Eop Error at the ingress arbiter interface from recirc interface.
                                           Write a one to clear the status bit.
                                           Bit 0: MAC0.
                                           Bit 1: MAC1.
                                           Bit 2: NIOS.
                                           Bit 3 : VirtIO. */
    uint32_t rec_igr_miss_sop : 4;      /* Missing Sop Error at the ingress arbiter interface from recirc interface.
                                           Write a one to clear the status bit.
                                           Bit 0: MAC0.
                                           Bit 1: MAC1.
                                           Bit 2: NIOS.
                                           Bit 3 : VirtIO. */
    uint32_t rec_igr_ov_err : 4;        /* Buffer over flow at the ingress arbiter interface from recirc interface.
                                           Write a one to clear the status bit.
                                           Bit 0: MAC0.
                                           Bit 1: MAC1.
                                           Bit 2: ARM.
                                           Bit 3 : VirtIO. */
    uint32_t igr_arb_2_par_msop : 4;    /* Missing Sop Error from ingress arbiter to parser interface. Write a one to
                                           clear the status bit.
                                           Bit 0: MAC0.
                                           Bit 1: MAC1.
                                           Bit 2: NIOS.
                                           Bit 3 : VirtIO. */
    uint32_t igr_arb_2_par_meop : 4;    /* Missing Eop Error from ingress arbiter to parser interface. Write a one to
                                           clear the status bit.
                                           Bit 0: MAC0.
                                           Bit 1: MAC1.
                                           Bit 2: NIOS.
                                           Bit 3 : VirtIO. */
    uint32_t rec_iarb_2_par_msop : 4;   /* Missing Sop Error from ingress arbiter to parser from recirc interfacce.
                                           Write a one to clear the status bit.
                                           Bit 0: MAC0.
                                           Bit 1: MAC1.
                                           Bit 2: NIOS.
                                           Bit 3 : VirtIO. */
    uint32_t rec_iarb_2_par_meop : 4;   /* Missing Eop Error from ingress arbiter to parser interface. Write a one to
                                           clear the status bit.
                                           Bit 0: MAC0.
                                           Bit 1: MAC1.
                                           Bit 2: NIOS.
                                           Bit 3 : VirtIO. */
    uint32_t : 4;
  } field;
  uint32_t val;
} MKS_MOD_MOD_ERR_HOLD2_t;
#endif /* GENHDR_STRUCT */

#endif /* _MKS_MOD_REGISTERS_H_ */

