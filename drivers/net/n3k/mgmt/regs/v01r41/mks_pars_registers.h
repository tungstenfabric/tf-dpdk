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

#ifndef _MKS_PARS_REGISTERS_H_
#define _MKS_PARS_REGISTERS_H_

/* OVS Parser */

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

#define MKS_PARS_ACCESSTYPE             GENHDR_ACCESSTYPE_RW /*  Default access type. Access types defined above. */
#define MKS_PARS_REGWIDTH               32 /* Default width of register in bits */
#define MKS_PARS_ACCESSWIDTH            32 /* Default width of access word in bit */


/* Parser status counter control */
#define MKS_PARS_PARS_STATS_CFG                                 0x00000000U
/*
 * Status read enable:
 *  SW sets this bit to initiate read port status.
 *  HW clears once all counters are loaded to the holding registers.
 */
#define MKS_PARS_PARS_STATS_CFG_STATUS_RD_ENABLE_SHIFT          0
#define MKS_PARS_PARS_STATS_CFG_STATUS_RD_ENABLE_WIDTH          1
#define MKS_PARS_PARS_STATS_CFG_STATUS_RD_ENABLE_MASK           (0x1U << MKS_PARS_PARS_STATS_CFG_STATUS_RD_ENABLE_SHIFT)

/*
 * Status write enable:
 *  SW sets this bit to initiate read port status.
 *  HW clears once all counters are loaded to the holding registers.
 */
#define MKS_PARS_PARS_STATS_CFG_STATUS_WR_ENABLE_SHIFT          1
#define MKS_PARS_PARS_STATS_CFG_STATUS_WR_ENABLE_WIDTH          1
#define MKS_PARS_PARS_STATS_CFG_STATUS_WR_ENABLE_MASK           (0x1U << MKS_PARS_PARS_STATS_CFG_STATUS_WR_ENABLE_SHIFT)

/* Block ID: */
#define MKS_PARS_PARS_STATS_CFG_STATUS_BLK_ID_SHIFT             2
#define MKS_PARS_PARS_STATS_CFG_STATUS_BLK_ID_WIDTH             2
#define MKS_PARS_PARS_STATS_CFG_STATUS_BLK_ID_MASK              (0x3U << MKS_PARS_PARS_STATS_CFG_STATUS_BLK_ID_SHIFT)

/* Status counter Port ID: */
#define MKS_PARS_PARS_STATS_CFG_STATUS_PORT_ID_SHIFT            4
#define MKS_PARS_PARS_STATS_CFG_STATUS_PORT_ID_WIDTH            2
#define MKS_PARS_PARS_STATS_CFG_STATUS_PORT_ID_MASK             (0x3U << MKS_PARS_PARS_STATS_CFG_STATUS_PORT_ID_SHIFT)

/* Status counter SubPort ID. */
#define MKS_PARS_PARS_STATS_CFG_STATUS_SUB_PORT_ID_SHIFT        6
#define MKS_PARS_PARS_STATS_CFG_STATUS_SUB_PORT_ID_WIDTH        7
#define MKS_PARS_PARS_STATS_CFG_STATUS_SUB_PORT_ID_MASK         (0x7FU << MKS_PARS_PARS_STATS_CFG_STATUS_SUB_PORT_ID_SHIFT)

/* Set this bit to read the stats for mirrored packets. */
#define MKS_PARS_PARS_STATS_CFG_STATUS_MIRR_STAT_SHIFT          13
#define MKS_PARS_PARS_STATS_CFG_STATUS_MIRR_STAT_WIDTH          1
#define MKS_PARS_PARS_STATS_CFG_STATUS_MIRR_STAT_MASK           (0x1U << MKS_PARS_PARS_STATS_CFG_STATUS_MIRR_STAT_SHIFT)

#ifdef GENHDR_STRUCT
/* Parser status counter control */
typedef union {
  struct {
    uint32_t status_rd_enable : 1;      /* Status read enable:
                                           SW sets this bit to initiate read port status.
                                           HW clears once all counters are loaded to the holding registers. */
    uint32_t status_wr_enable : 1;      /* Status write enable:
                                           SW sets this bit to initiate read port status.
                                           HW clears once all counters are loaded to the holding registers. */
    uint32_t status_blk_id : 2;         /* Block ID: */
    uint32_t status_port_id : 2;        /* Status counter Port ID: */
    uint32_t status_sub_port_id : 7;    /* Status counter SubPort ID. */
    uint32_t status_mirr_stat : 1;      /* Set this bit to read the stats for mirrored packets. */
    uint32_t : 18;
  } field;
  uint32_t val;
} MKS_PARS_PARS_STATS_CFG_t;
#endif /* GENHDR_STRUCT */

#define MKS_PARS_PARS_STATS_CFG_STATUS_BLK_ID_RD_UC             0x0U /* Read Unicast. */
#define MKS_PARS_PARS_STATS_CFG_STATUS_BLK_ID_RD_MC_BC          0x1U /* Read Multi/Broadcast. */
#define MKS_PARS_PARS_STATS_CFG_STATUS_BLK_ID_RD_BCNT           0x2U /* Read byte count. */

#define MKS_PARS_PARS_STATS_CFG_STATUS_PORT_ID_MAC_0            0x0U
#define MKS_PARS_PARS_STATS_CFG_STATUS_PORT_ID_MAC_1            0x1U
#define MKS_PARS_PARS_STATS_CFG_STATUS_PORT_ID_MGMT             0x2U
#define MKS_PARS_PARS_STATS_CFG_STATUS_PORT_ID_VIRTIO           0x3U

/*
 * Register to hold value of a statistics register for which a read has been
 * issued
 */
#define MKS_PARS_PARS_STAT_REG0                         0x00000004U
/* Bits [31:0] of the statistics register in question. */
#define MKS_PARS_PARS_STAT_REG0_BITS_31_0_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define MKS_PARS_PARS_STAT_REG0_BITS_31_0_SHIFT         0
#define MKS_PARS_PARS_STAT_REG0_BITS_31_0_WIDTH         32
#define MKS_PARS_PARS_STAT_REG0_BITS_31_0_MASK          (0xFFFFFFFFU << MKS_PARS_PARS_STAT_REG0_BITS_31_0_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Register to hold value of a statistics register for which a read has been
 * issued
 */
typedef union {
  struct {
    uint32_t bits_31_0 : 32;    /* Bits [31:0] of the statistics register in question. */
  } field;
  uint32_t val;
} MKS_PARS_PARS_STAT_REG0_t;
#endif /* GENHDR_STRUCT */

/*
 * Register to hold value of a statistics register for which a read has been
 * issued
 */
#define MKS_PARS_PARS_STAT_REG1                         0x00000008U
/* Bits [31:0] of the statistics register in question. */
#define MKS_PARS_PARS_STAT_REG1_BITS_39_32_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define MKS_PARS_PARS_STAT_REG1_BITS_39_32_SHIFT        0
#define MKS_PARS_PARS_STAT_REG1_BITS_39_32_WIDTH        8
#define MKS_PARS_PARS_STAT_REG1_BITS_39_32_MASK         (0xFFU << MKS_PARS_PARS_STAT_REG1_BITS_39_32_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Register to hold value of a statistics register for which a read has been
 * issued
 */
typedef union {
  struct {
    uint32_t bits_39_32 : 8;    /* Bits [31:0] of the statistics register in question. */
    uint32_t : 24;
  } field;
  uint32_t val;
} MKS_PARS_PARS_STAT_REG1_t;
#endif /* GENHDR_STRUCT */

/* PLD packet drop counter limit */
#define MKS_PARS_PARS_PLD_DROP_LIMIT                    0x0000000CU
/*
 * This is reference count value. After PARS_PLD_DROP_CNT reaches this value it
 * should stop counting and retain its value until the next read.
 */
#define MKS_PARS_PARS_PLD_DROP_LIMIT_LIMIT_ACCESSTYPE   GENHDR_ACCESSTYPE_WO
#define MKS_PARS_PARS_PLD_DROP_LIMIT_LIMIT_SHIFT        0
#define MKS_PARS_PARS_PLD_DROP_LIMIT_LIMIT_WIDTH        32
#define MKS_PARS_PARS_PLD_DROP_LIMIT_LIMIT_MASK         (0xFFFFFFFFU << MKS_PARS_PARS_PLD_DROP_LIMIT_LIMIT_SHIFT)

#ifdef GENHDR_STRUCT
/* PLD packet drop counter limit */
typedef union {
  struct {
    uint32_t limit : 32;        /* This is reference count value. After PARS_PLD_DROP_CNT reaches this value it
                                   should stop counting and retain its value until the next read. */
  } field;
  uint32_t val;
} MKS_PARS_PARS_PLD_DROP_LIMIT_t;
#endif /* GENHDR_STRUCT */

/* Rsvd register */
#define MKS_PARS_RSVD_0                 0x00000010U
/* Reserved register.Read only. */
#define MKS_PARS_RSVD_0_RSVD_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_PARS_RSVD_0_RSVD_SHIFT      0
#define MKS_PARS_RSVD_0_RSVD_WIDTH      32
#define MKS_PARS_RSVD_0_RSVD_MASK       (0xFFFFFFFFU << MKS_PARS_RSVD_0_RSVD_SHIFT)

#ifdef GENHDR_STRUCT
/* Rsvd register */
typedef union {
  struct {
    uint32_t rsvd : 32; /* Reserved register.Read only. */
  } field;
  uint32_t val;
} MKS_PARS_RSVD_0_t;
#endif /* GENHDR_STRUCT */

/* Rsvd register */
#define MKS_PARS_RSVD_1                 0x00000014U
/* Reserved register.Read only. */
#define MKS_PARS_RSVD_1_RSVD_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_PARS_RSVD_1_RSVD_SHIFT      0
#define MKS_PARS_RSVD_1_RSVD_WIDTH      32
#define MKS_PARS_RSVD_1_RSVD_MASK       (0xFFFFFFFFU << MKS_PARS_RSVD_1_RSVD_SHIFT)

#ifdef GENHDR_STRUCT
/* Rsvd register */
typedef union {
  struct {
    uint32_t rsvd : 32; /* Reserved register.Read only. */
  } field;
  uint32_t val;
} MKS_PARS_RSVD_1_t;
#endif /* GENHDR_STRUCT */

/* Rsvd register */
#define MKS_PARS_RSVD_2                 0x00000018U
/* Reserved register.Read only. */
#define MKS_PARS_RSVD_2_RSVD_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_PARS_RSVD_2_RSVD_SHIFT      0
#define MKS_PARS_RSVD_2_RSVD_WIDTH      32
#define MKS_PARS_RSVD_2_RSVD_MASK       (0xFFFFFFFFU << MKS_PARS_RSVD_2_RSVD_SHIFT)

#ifdef GENHDR_STRUCT
/* Rsvd register */
typedef union {
  struct {
    uint32_t rsvd : 32; /* Reserved register.Read only. */
  } field;
  uint32_t val;
} MKS_PARS_RSVD_2_t;
#endif /* GENHDR_STRUCT */

/* Rx error status in parser */
#define MKS_PARS_PARS_RX_ERR_HOLD                       0x0000001CU
/*
 * Rx error on MAC0 port.
 *  When set indicates errored packet received from MAC0 port.
 */
#define MKS_PARS_PARS_RX_ERR_HOLD_MAC_0_ERR_ACCESSTYPE  GENHDR_ACCESSTYPE_W1C
#define MKS_PARS_PARS_RX_ERR_HOLD_MAC_0_ERR_SHIFT       0
#define MKS_PARS_PARS_RX_ERR_HOLD_MAC_0_ERR_WIDTH       1
#define MKS_PARS_PARS_RX_ERR_HOLD_MAC_0_ERR_MASK        (0x1U << MKS_PARS_PARS_RX_ERR_HOLD_MAC_0_ERR_SHIFT)

/*
 * Rx error on MAC1 port.
 *  When set indicates errored packet received from MAC1 port.
 */
#define MKS_PARS_PARS_RX_ERR_HOLD_MAC_1_ERR_ACCESSTYPE  GENHDR_ACCESSTYPE_W1C
#define MKS_PARS_PARS_RX_ERR_HOLD_MAC_1_ERR_SHIFT       1
#define MKS_PARS_PARS_RX_ERR_HOLD_MAC_1_ERR_WIDTH       1
#define MKS_PARS_PARS_RX_ERR_HOLD_MAC_1_ERR_MASK        (0x1U << MKS_PARS_PARS_RX_ERR_HOLD_MAC_1_ERR_SHIFT)

/*
 * Rx error on MGMT port.
 *  When set indicates errored packet received from MGMT port.
 */
#define MKS_PARS_PARS_RX_ERR_HOLD_MGMT_ERR_ACCESSTYPE   GENHDR_ACCESSTYPE_W1C
#define MKS_PARS_PARS_RX_ERR_HOLD_MGMT_ERR_SHIFT        2
#define MKS_PARS_PARS_RX_ERR_HOLD_MGMT_ERR_WIDTH        1
#define MKS_PARS_PARS_RX_ERR_HOLD_MGMT_ERR_MASK         (0x1U << MKS_PARS_PARS_RX_ERR_HOLD_MGMT_ERR_SHIFT)

/*
 * Rx error on VIRTIO port.
 *  When set indicates errored packet received from VIRTIO port.
 */
#define MKS_PARS_PARS_RX_ERR_HOLD_VIRTIO_ERR_ACCESSTYPE GENHDR_ACCESSTYPE_W1C
#define MKS_PARS_PARS_RX_ERR_HOLD_VIRTIO_ERR_SHIFT      3
#define MKS_PARS_PARS_RX_ERR_HOLD_VIRTIO_ERR_WIDTH      1
#define MKS_PARS_PARS_RX_ERR_HOLD_VIRTIO_ERR_MASK       (0x1U << MKS_PARS_PARS_RX_ERR_HOLD_VIRTIO_ERR_SHIFT)

/*
 * Rx error on VIRTIO subport.
 *  VIRTIO subport which received errored packet.
 */
#define MKS_PARS_PARS_RX_ERR_HOLD_SUB_PRT_ERR_ACCESSTYPE GENHDR_ACCESSTYPE_W1C
#define MKS_PARS_PARS_RX_ERR_HOLD_SUB_PRT_ERR_SHIFT     4
#define MKS_PARS_PARS_RX_ERR_HOLD_SUB_PRT_ERR_WIDTH     6
#define MKS_PARS_PARS_RX_ERR_HOLD_SUB_PRT_ERR_MASK      (0x3FU << MKS_PARS_PARS_RX_ERR_HOLD_SUB_PRT_ERR_SHIFT)

#ifdef GENHDR_STRUCT
/* Rx error status in parser */
typedef union {
  struct {
    uint32_t mac_0_err : 1;     /* Rx error on MAC0 port.
                                   When set indicates errored packet received from MAC0 port. */
    uint32_t mac_1_err : 1;     /* Rx error on MAC1 port.
                                   When set indicates errored packet received from MAC1 port. */
    uint32_t mgmt_err : 1;      /* Rx error on MGMT port.
                                   When set indicates errored packet received from MGMT port. */
    uint32_t virtio_err : 1;    /* Rx error on VIRTIO port.
                                   When set indicates errored packet received from VIRTIO port. */
    uint32_t sub_prt_err : 6;   /* Rx error on VIRTIO subport.
                                   VIRTIO subport which received errored packet. */
    uint32_t : 22;
  } field;
  uint32_t val;
} MKS_PARS_PARS_RX_ERR_HOLD_t;
#endif /* GENHDR_STRUCT */

/* Parser RX error counter */
#define MKS_PARS_PARS_RX_ERR_CNT                        0x00000020U
/*
 * Stats counter for number of packets with error coming into Parser 
 *  Need to program the portId for which rx error values needs to be read 
 *  in PARS_STATS_CFG.statusPortId field.
 */
#define MKS_PARS_PARS_RX_ERR_CNT_PACKETS_SHIFT          0
#define MKS_PARS_PARS_RX_ERR_CNT_PACKETS_WIDTH          8
#define MKS_PARS_PARS_RX_ERR_CNT_PACKETS_MASK           (0xFFU << MKS_PARS_PARS_RX_ERR_CNT_PACKETS_SHIFT)

#ifdef GENHDR_STRUCT
/* Parser RX error counter */
typedef union {
  struct {
    uint32_t packets : 8;       /* Stats counter for number of packets with error coming into Parser
                                   Need to program the portId for which rx error values needs to be read
                                   in PARS_STATS_CFG.statusPortId field. */
    uint32_t : 24;
  } field;
  uint32_t val;
} MKS_PARS_PARS_RX_ERR_CNT_t;
#endif /* GENHDR_STRUCT */

/* Rsvd register */
#define MKS_PARS_RSVD_3                 0x00000024U
/* Reserved register.Read only. */
#define MKS_PARS_RSVD_3_RSVD_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_PARS_RSVD_3_RSVD_SHIFT      0
#define MKS_PARS_RSVD_3_RSVD_WIDTH      32
#define MKS_PARS_RSVD_3_RSVD_MASK       (0xFFFFFFFFU << MKS_PARS_RSVD_3_RSVD_SHIFT)

#ifdef GENHDR_STRUCT
/* Rsvd register */
typedef union {
  struct {
    uint32_t rsvd : 32; /* Reserved register.Read only. */
  } field;
  uint32_t val;
} MKS_PARS_RSVD_3_t;
#endif /* GENHDR_STRUCT */

/* Rsvd register */
#define MKS_PARS_RSVD_4                 0x00000028U
/* Reserved register.Read only. */
#define MKS_PARS_RSVD_4_RSVD_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_PARS_RSVD_4_RSVD_SHIFT      0
#define MKS_PARS_RSVD_4_RSVD_WIDTH      32
#define MKS_PARS_RSVD_4_RSVD_MASK       (0xFFFFFFFFU << MKS_PARS_RSVD_4_RSVD_SHIFT)

#ifdef GENHDR_STRUCT
/* Rsvd register */
typedef union {
  struct {
    uint32_t rsvd : 32; /* Reserved register.Read only. */
  } field;
  uint32_t val;
} MKS_PARS_RSVD_4_t;
#endif /* GENHDR_STRUCT */

/* Rsvd register */
#define MKS_PARS_RSVD_5                 0x0000002CU
/* Reserved register.Read only. */
#define MKS_PARS_RSVD_5_RSVD_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_PARS_RSVD_5_RSVD_SHIFT      0
#define MKS_PARS_RSVD_5_RSVD_WIDTH      32
#define MKS_PARS_RSVD_5_RSVD_MASK       (0xFFFFFFFFU << MKS_PARS_RSVD_5_RSVD_SHIFT)

#ifdef GENHDR_STRUCT
/* Rsvd register */
typedef union {
  struct {
    uint32_t rsvd : 32; /* Reserved register.Read only. */
  } field;
  uint32_t val;
} MKS_PARS_RSVD_5_t;
#endif /* GENHDR_STRUCT */

/* Rsvd register */
#define MKS_PARS_RSVD_6                 0x00000030U
/* Reserved register.Read only. */
#define MKS_PARS_RSVD_6_RSVD_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_PARS_RSVD_6_RSVD_SHIFT      0
#define MKS_PARS_RSVD_6_RSVD_WIDTH      32
#define MKS_PARS_RSVD_6_RSVD_MASK       (0xFFFFFFFFU << MKS_PARS_RSVD_6_RSVD_SHIFT)

#ifdef GENHDR_STRUCT
/* Rsvd register */
typedef union {
  struct {
    uint32_t rsvd : 32; /* Reserved register.Read only. */
  } field;
  uint32_t val;
} MKS_PARS_RSVD_6_t;
#endif /* GENHDR_STRUCT */

/* Rsvd register */
#define MKS_PARS_RSVD_7                 0x00000034U
/* Reserved register.Read only. */
#define MKS_PARS_RSVD_7_RSVD_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_PARS_RSVD_7_RSVD_SHIFT      0
#define MKS_PARS_RSVD_7_RSVD_WIDTH      32
#define MKS_PARS_RSVD_7_RSVD_MASK       (0xFFFFFFFFU << MKS_PARS_RSVD_7_RSVD_SHIFT)

#ifdef GENHDR_STRUCT
/* Rsvd register */
typedef union {
  struct {
    uint32_t rsvd : 32; /* Reserved register.Read only. */
  } field;
  uint32_t val;
} MKS_PARS_RSVD_7_t;
#endif /* GENHDR_STRUCT */

/* Rsvd register */
#define MKS_PARS_RSVD_8                 0x00000038U
/* Reserved register.Read only. */
#define MKS_PARS_RSVD_8_RSVD_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_PARS_RSVD_8_RSVD_SHIFT      0
#define MKS_PARS_RSVD_8_RSVD_WIDTH      32
#define MKS_PARS_RSVD_8_RSVD_MASK       (0xFFFFFFFFU << MKS_PARS_RSVD_8_RSVD_SHIFT)

#ifdef GENHDR_STRUCT
/* Rsvd register */
typedef union {
  struct {
    uint32_t rsvd : 32; /* Reserved register.Read only. */
  } field;
  uint32_t val;
} MKS_PARS_RSVD_8_t;
#endif /* GENHDR_STRUCT */

/* Rsvd register */
#define MKS_PARS_RSVD_9                 0x0000003CU
/* Reserved register.Read only. */
#define MKS_PARS_RSVD_9_RSVD_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_PARS_RSVD_9_RSVD_SHIFT      0
#define MKS_PARS_RSVD_9_RSVD_WIDTH      32
#define MKS_PARS_RSVD_9_RSVD_MASK       (0xFFFFFFFFU << MKS_PARS_RSVD_9_RSVD_SHIFT)

#ifdef GENHDR_STRUCT
/* Rsvd register */
typedef union {
  struct {
    uint32_t rsvd : 32; /* Reserved register.Read only. */
  } field;
  uint32_t val;
} MKS_PARS_RSVD_9_t;
#endif /* GENHDR_STRUCT */

/* Rsvd register */
#define MKS_PARS_RSVD_10                0x00000040U
/* Reserved register.Read only. */
#define MKS_PARS_RSVD_10_RSVD_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_PARS_RSVD_10_RSVD_SHIFT     0
#define MKS_PARS_RSVD_10_RSVD_WIDTH     32
#define MKS_PARS_RSVD_10_RSVD_MASK      (0xFFFFFFFFU << MKS_PARS_RSVD_10_RSVD_SHIFT)

#ifdef GENHDR_STRUCT
/* Rsvd register */
typedef union {
  struct {
    uint32_t rsvd : 32; /* Reserved register.Read only. */
  } field;
  uint32_t val;
} MKS_PARS_RSVD_10_t;
#endif /* GENHDR_STRUCT */

/* MAC0 Packet counter */
#define MKS_PARS_PARS_PORT0_CNT                         0x00000044U
/* Counts number of packets for MAC0 (free running counter). */
#define MKS_PARS_PARS_PORT0_CNT_PACKETS_SHIFT           0
#define MKS_PARS_PARS_PORT0_CNT_PACKETS_WIDTH           32
#define MKS_PARS_PARS_PORT0_CNT_PACKETS_MASK            (0xFFFFFFFFU << MKS_PARS_PARS_PORT0_CNT_PACKETS_SHIFT)

#ifdef GENHDR_STRUCT
/* MAC0 Packet counter */
typedef union {
  struct {
    uint32_t packets : 32;      /* Counts number of packets for MAC0 (free running counter). */
  } field;
  uint32_t val;
} MKS_PARS_PARS_PORT0_CNT_t;
#endif /* GENHDR_STRUCT */

/* MAC1 Packet counter */
#define MKS_PARS_PARS_PORT1_CNT                         0x00000048U
/* Counts number of packets for MAC1 (free running counter). */
#define MKS_PARS_PARS_PORT1_CNT_PACKETS_SHIFT           0
#define MKS_PARS_PARS_PORT1_CNT_PACKETS_WIDTH           32
#define MKS_PARS_PARS_PORT1_CNT_PACKETS_MASK            (0xFFFFFFFFU << MKS_PARS_PARS_PORT1_CNT_PACKETS_SHIFT)

#ifdef GENHDR_STRUCT
/* MAC1 Packet counter */
typedef union {
  struct {
    uint32_t packets : 32;      /* Counts number of packets for MAC1 (free running counter). */
  } field;
  uint32_t val;
} MKS_PARS_PARS_PORT1_CNT_t;
#endif /* GENHDR_STRUCT */

/* MGMT Packet counter */
#define MKS_PARS_PARS_PORT2_CNT                         0x0000004CU
/* Counts number of packets for MGMT (free running counter). */
#define MKS_PARS_PARS_PORT2_CNT_PACKETS_SHIFT           0
#define MKS_PARS_PARS_PORT2_CNT_PACKETS_WIDTH           32
#define MKS_PARS_PARS_PORT2_CNT_PACKETS_MASK            (0xFFFFFFFFU << MKS_PARS_PARS_PORT2_CNT_PACKETS_SHIFT)

#ifdef GENHDR_STRUCT
/* MGMT Packet counter */
typedef union {
  struct {
    uint32_t packets : 32;      /* Counts number of packets for MGMT (free running counter). */
  } field;
  uint32_t val;
} MKS_PARS_PARS_PORT2_CNT_t;
#endif /* GENHDR_STRUCT */

/* VIRTIO packet counter */
#define MKS_PARS_PARS_PORT3_CNT                         0x00000050U
/* Counts number of packets for VIRTIO (free running counter). */
#define MKS_PARS_PARS_PORT3_CNT_PACKETS_SHIFT           0
#define MKS_PARS_PARS_PORT3_CNT_PACKETS_WIDTH           32
#define MKS_PARS_PARS_PORT3_CNT_PACKETS_MASK            (0xFFFFFFFFU << MKS_PARS_PARS_PORT3_CNT_PACKETS_SHIFT)

#ifdef GENHDR_STRUCT
/* VIRTIO packet counter */
typedef union {
  struct {
    uint32_t packets : 32;      /* Counts number of packets for VIRTIO (free running counter). */
  } field;
  uint32_t val;
} MKS_PARS_PARS_PORT3_CNT_t;
#endif /* GENHDR_STRUCT */

/* Match Vid Config Register */
#define MKS_PARS_PARS_0_MTCHVID_CFG                     0x00000054U
/*
 * Register to match incoming Vid.
 *  This register has vid value that needs to change to LAG vid value.
 */
#define MKS_PARS_PARS_0_MTCHVID_CFG_MATCHVID_SHIFT      0
#define MKS_PARS_PARS_0_MTCHVID_CFG_MATCHVID_WIDTH      8
#define MKS_PARS_PARS_0_MTCHVID_CFG_MATCHVID_MASK       (0xFFU << MKS_PARS_PARS_0_MTCHVID_CFG_MATCHVID_SHIFT)

#ifdef GENHDR_STRUCT
/* Match Vid Config Register */
typedef union {
  struct {
    uint32_t matchvid : 8;      /* Register to match incoming Vid.
                                   This register has vid value that needs to change to LAG vid value. */
    uint32_t : 24;
  } field;
  uint32_t val;
} MKS_PARS_PARS_0_MTCHVID_CFG_t;
#endif /* GENHDR_STRUCT */

/* Match Vid Config Register */
#define MKS_PARS_PARS_1_MTCHVID_CFG                     0x00000058U
/*
 * Register to match incoming Vid.
 *  This register has vid value that needs to change to LAG vid value.
 */
#define MKS_PARS_PARS_1_MTCHVID_CFG_MATCHVID_SHIFT      0
#define MKS_PARS_PARS_1_MTCHVID_CFG_MATCHVID_WIDTH      8
#define MKS_PARS_PARS_1_MTCHVID_CFG_MATCHVID_MASK       (0xFFU << MKS_PARS_PARS_1_MTCHVID_CFG_MATCHVID_SHIFT)

#ifdef GENHDR_STRUCT
/* Match Vid Config Register */
typedef union {
  struct {
    uint32_t matchvid : 8;      /* Register to match incoming Vid.
                                   This register has vid value that needs to change to LAG vid value. */
    uint32_t : 24;
  } field;
  uint32_t val;
} MKS_PARS_PARS_1_MTCHVID_CFG_t;
#endif /* GENHDR_STRUCT */

/* LAG Vid Config Register */
#define MKS_PARS_PARS_LAGVID_CFG                        0x0000005CU
/*
 * Lag Vid.
 *  This register has Lag vid value.
 */
#define MKS_PARS_PARS_LAGVID_CFG_LAGVID_SHIFT           0
#define MKS_PARS_PARS_LAGVID_CFG_LAGVID_WIDTH           8
#define MKS_PARS_PARS_LAGVID_CFG_LAGVID_MASK            (0xFFU << MKS_PARS_PARS_LAGVID_CFG_LAGVID_SHIFT)

#ifdef GENHDR_STRUCT
/* LAG Vid Config Register */
typedef union {
  struct {
    uint32_t lagvid : 8;        /* Lag Vid.
                                   This register has Lag vid value. */
    uint32_t : 24;
  } field;
  uint32_t val;
} MKS_PARS_PARS_LAGVID_CFG_t;
#endif /* GENHDR_STRUCT */

/* This register has all the enables */
#define MKS_PARS_PARS_ENABLE_CFG                                0x00000060U
/* This bit sets high_pri bit(in PLD_S) for ARP headers. */
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_ARP_SHIFT               0
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_ARP_WIDTH               1
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_ARP_MASK                (0x1U << MKS_PARS_PARS_ENABLE_CFG_ENABLE_ARP_SHIFT)

/* This bit sets high_pri bit(in PLD_S) for icmp headers. */
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_ICMP_SHIFT              1
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_ICMP_WIDTH              1
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_ICMP_MASK               (0x1U << MKS_PARS_PARS_ENABLE_CFG_ENABLE_ICMP_SHIFT)

/* This bit is used to enable LAG on MAC0 and MAC1 ports. */
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_LAG_SHIFT               2
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_LAG_WIDTH               1
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_LAG_MASK                (0x1U << MKS_PARS_PARS_ENABLE_CFG_ENABLE_LAG_SHIFT)

/*
 * When this bit is set the HW will extract {subport,portId} in vid field of
 * key.
 */
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_PTSUBPT_SHIFT           3
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_PTSUBPT_WIDTH           1
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_PTSUBPT_MASK            (0x1U << MKS_PARS_PARS_ENABLE_CFG_ENABLE_PTSUBPT_SHIFT)

/*
 * Enable for Ip over Gre mode.
 *  Write 1 to this bit in order to enable ip over gre mode.
 *  NOTE: As per current implementation, if this bit is '1', it means GRE or
 * VxLAN enabled and if it is '0', then the tunnel mode enabled is as per flow
 * table entry.
 */
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_IPOVRGRE_SHIFT          4
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_IPOVRGRE_WIDTH          1
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_IPOVRGRE_MASK           (0x1U << MKS_PARS_PARS_ENABLE_CFG_ENABLE_IPOVRGRE_SHIFT)

/*
 * Enable TCP Flags field in the key.
 *  Write 1 to this bit in order to enable inner as well as outer tcp flags in
 * the key.
 */
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_TCPFLG_SHIFT            5
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_TCPFLG_WIDTH            1
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_TCPFLG_MASK             (0x1U << MKS_PARS_PARS_ENABLE_CFG_ENABLE_TCPFLG_SHIFT)

/*
 * Enable new header format.
 *  Write 1 to this bit in order to enable new header format for ethernet header
 * with Intel vlan tag.
 */
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_NEWHDR_SHIFT            6
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_NEWHDR_WIDTH            1
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_NEWHDR_MASK             (0x1U << MKS_PARS_PARS_ENABLE_CFG_ENABLE_NEWHDR_SHIFT)

/*
 * Enable mirroring .
 *  Write 1 to this bit in order to enable mirroring.
 */
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_MIRROR_SHIFT            7
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_MIRROR_WIDTH            1
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_MIRROR_MASK             (0x1U << MKS_PARS_PARS_ENABLE_CFG_ENABLE_MIRROR_SHIFT)

/*
 * Enable QID .
 *  Write 1 to this bit in order to enable qid extraction in key.
 */
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_QID_SHIFT               8
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_QID_WIDTH               1
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_QID_MASK                (0x1U << MKS_PARS_PARS_ENABLE_CFG_ENABLE_QID_SHIFT)

/*
 * Enable Masking L4 fields for Match .
 *  When set, L4 SP, DP are not used for Match .
 *  Write 1 to this bit in order to enable Masking L4 fields for Match .
 */
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_MASK_L4_SHIFT           9
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_MASK_L4_WIDTH           1
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_MASK_L4_MASK            (0x1U << MKS_PARS_PARS_ENABLE_CFG_ENABLE_MASK_L4_SHIFT)

/*
 * Enable to include TTL field(s) in the lookup key for Match .
 *  When set, network TTL is not used for Match .
 *  Write 1 in order to enable TTL fields for Match .
 */
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_LKUP_TTL_SHIFT          10
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_LKUP_TTL_WIDTH          1
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_LKUP_TTL_MASK           (0x1U << MKS_PARS_PARS_ENABLE_CFG_ENABLE_LKUP_TTL_SHIFT)

/* Write '1' to enable MPLS over UDP feature. */
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_MPLSO_UDP_SHIFT         11
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_MPLSO_UDP_WIDTH         1
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_MPLSO_UDP_MASK          (0x1U << MKS_PARS_PARS_ENABLE_CFG_ENABLE_MPLSO_UDP_SHIFT)

/* Write '1' to enable MPLS over GRE feature. */
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_MPLSO_GRE_SHIFT         12
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_MPLSO_GRE_WIDTH         1
#define MKS_PARS_PARS_ENABLE_CFG_ENABLE_MPLSO_GRE_MASK          (0x1U << MKS_PARS_PARS_ENABLE_CFG_ENABLE_MPLSO_GRE_SHIFT)

/* Reserved. */
#define MKS_PARS_PARS_ENABLE_CFG_RSVD_SHIFT                     13
#define MKS_PARS_PARS_ENABLE_CFG_RSVD_WIDTH                     2
#define MKS_PARS_PARS_ENABLE_CFG_RSVD_MASK                      (0x3U << MKS_PARS_PARS_ENABLE_CFG_RSVD_SHIFT)

/*
 * If this enable bit is 1 and also if either FIN or RST bit is 1 in the control
 * flags for TCP packet,then external PSG EM Lookup search will be bypassed and
 * the corresponding default lookup rule will be applied. Subsequently,the
 * packet will be forwarded to slow path.
 *  This bit has to be enabled for vRouter only and not for OvS.
 *  NOTE : In case of Tunnel packet, FIN/RST flag in the inner layer TCP packet
 * will be checked in PARSER.
 */
#define MKS_PARS_PARS_ENABLE_CFG_EN_BYP_EMLKUP_TCP_SHIFT        15
#define MKS_PARS_PARS_ENABLE_CFG_EN_BYP_EMLKUP_TCP_WIDTH        1
#define MKS_PARS_PARS_ENABLE_CFG_EN_BYP_EMLKUP_TCP_MASK         (0x1U << MKS_PARS_PARS_ENABLE_CFG_EN_BYP_EMLKUP_TCP_SHIFT)

#ifdef GENHDR_STRUCT
/* This register has all the enables */
typedef union {
  struct {
    uint32_t enable_arp : 1;            /* This bit sets high_pri bit(in PLD_S) for ARP headers. */
    uint32_t enable_icmp : 1;           /* This bit sets high_pri bit(in PLD_S) for icmp headers. */
    uint32_t enable_lag : 1;            /* This bit is used to enable LAG on MAC0 and MAC1 ports. */
    uint32_t enable_ptsubpt : 1;        /* When this bit is set the HW will extract {subport,portId} in vid field of
                                           key. */
    uint32_t enable_ipovrgre : 1;       /* Enable for Ip over Gre mode.
                                           Write 1 to this bit in order to enable ip over gre mode.
                                           NOTE: As per current implementation, if this bit is '1', it means GRE or
                                           VxLAN enabled and if it is '0', then the tunnel mode enabled is as per flow
                                           table entry. */
    uint32_t enable_tcpflg : 1;         /* Enable TCP Flags field in the key.
                                           Write 1 to this bit in order to enable inner as well as outer tcp flags in
                                           the key. */
    uint32_t enable_newhdr : 1;         /* Enable new header format.
                                           Write 1 to this bit in order to enable new header format for ethernet header
                                           with Intel vlan tag. */
    uint32_t enable_mirror : 1;         /* Enable mirroring .
                                           Write 1 to this bit in order to enable mirroring. */
    uint32_t enable_qid : 1;            /* Enable QID .
                                           Write 1 to this bit in order to enable qid extraction in key. */
    uint32_t enable_mask_l4 : 1;        /* Enable Masking L4 fields for Match .
                                           When set, L4 SP, DP are not used for Match .
                                           Write 1 to this bit in order to enable Masking L4 fields for Match . */
    uint32_t enable_lkup_ttl : 1;       /* Enable to include TTL field(s) in the lookup key for Match .
                                           When set, network TTL is not used for Match .
                                           Write 1 in order to enable TTL fields for Match . */
    uint32_t enable_mplso_udp : 1;      /* Write '1' to enable MPLS over UDP feature. */
    uint32_t enable_mplso_gre : 1;      /* Write '1' to enable MPLS over GRE feature. */
    uint32_t rsvd : 2;                  /* Reserved. */
    uint32_t en_byp_emlkup_tcp : 1;     /* If this enable bit is 1 and also if either FIN or RST bit is 1 in the control
                                           flags for TCP packet,then external PSG EM Lookup search will be bypassed and
                                           the corresponding default lookup rule will be applied. Subsequently,the
                                           packet will be forwarded to slow path.
                                           This bit has to be enabled for vRouter only and not for OvS.
                                           NOTE : In case of Tunnel packet, FIN/RST flag in the inner layer TCP packet
                                           will be checked in PARSER. */
    uint32_t : 16;
  } field;
  uint32_t val;
} MKS_PARS_PARS_ENABLE_CFG_t;
#endif /* GENHDR_STRUCT */

/*
 * Illegal packet error status in parser. Illegal packet is any packet where 
 *  total length of all the headers is less than total packet length
 */
#define MKS_PARS_PARS_ILLGPKT_ERR_HLD                           0x00000064U
/*
 * Illegal packet error on MAC0 port.
 *  When set indicates illegal packet received from MAC0 port.
 */
#define MKS_PARS_PARS_ILLGPKT_ERR_HLD_MAC_0_ERR_ACCESSTYPE      GENHDR_ACCESSTYPE_W1C
#define MKS_PARS_PARS_ILLGPKT_ERR_HLD_MAC_0_ERR_SHIFT           0
#define MKS_PARS_PARS_ILLGPKT_ERR_HLD_MAC_0_ERR_WIDTH           1
#define MKS_PARS_PARS_ILLGPKT_ERR_HLD_MAC_0_ERR_MASK            (0x1U << MKS_PARS_PARS_ILLGPKT_ERR_HLD_MAC_0_ERR_SHIFT)

/*
 * Illegal packet error on MAC1 port.
 *  When set indicates illegal packet received from MAC1 port.
 */
#define MKS_PARS_PARS_ILLGPKT_ERR_HLD_MAC_1_ERR_ACCESSTYPE      GENHDR_ACCESSTYPE_W1C
#define MKS_PARS_PARS_ILLGPKT_ERR_HLD_MAC_1_ERR_SHIFT           1
#define MKS_PARS_PARS_ILLGPKT_ERR_HLD_MAC_1_ERR_WIDTH           1
#define MKS_PARS_PARS_ILLGPKT_ERR_HLD_MAC_1_ERR_MASK            (0x1U << MKS_PARS_PARS_ILLGPKT_ERR_HLD_MAC_1_ERR_SHIFT)

/*
 * Illegal packet error on MGMT port.
 *  When set indicates illegal packet received from MGMT port.
 */
#define MKS_PARS_PARS_ILLGPKT_ERR_HLD_MGMT_ERR_ACCESSTYPE       GENHDR_ACCESSTYPE_W1C
#define MKS_PARS_PARS_ILLGPKT_ERR_HLD_MGMT_ERR_SHIFT            2
#define MKS_PARS_PARS_ILLGPKT_ERR_HLD_MGMT_ERR_WIDTH            1
#define MKS_PARS_PARS_ILLGPKT_ERR_HLD_MGMT_ERR_MASK             (0x1U << MKS_PARS_PARS_ILLGPKT_ERR_HLD_MGMT_ERR_SHIFT)

/*
 * Illegal packet error on VIRTIO port.
 *  When set indicates illegal packet received from VIRTIO port.
 */
#define MKS_PARS_PARS_ILLGPKT_ERR_HLD_VIRTIO_ERR_ACCESSTYPE     GENHDR_ACCESSTYPE_W1C
#define MKS_PARS_PARS_ILLGPKT_ERR_HLD_VIRTIO_ERR_SHIFT          3
#define MKS_PARS_PARS_ILLGPKT_ERR_HLD_VIRTIO_ERR_WIDTH          1
#define MKS_PARS_PARS_ILLGPKT_ERR_HLD_VIRTIO_ERR_MASK           (0x1U << MKS_PARS_PARS_ILLGPKT_ERR_HLD_VIRTIO_ERR_SHIFT)

/*
 * Illegal packet error on VIRTIO subport.
 *  VIRTIO subport which received illegal packet.
 */
#define MKS_PARS_PARS_ILLGPKT_ERR_HLD_SUB_PRT_ERR_ACCESSTYPE    GENHDR_ACCESSTYPE_W1C
#define MKS_PARS_PARS_ILLGPKT_ERR_HLD_SUB_PRT_ERR_SHIFT         4
#define MKS_PARS_PARS_ILLGPKT_ERR_HLD_SUB_PRT_ERR_WIDTH         6
#define MKS_PARS_PARS_ILLGPKT_ERR_HLD_SUB_PRT_ERR_MASK          (0x3FU << MKS_PARS_PARS_ILLGPKT_ERR_HLD_SUB_PRT_ERR_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Illegal packet error status in parser. Illegal packet is any packet where 
 *  total length of all the headers is less than total packet length
 */
typedef union {
  struct {
    uint32_t mac_0_err : 1;     /* Illegal packet error on MAC0 port.
                                   When set indicates illegal packet received from MAC0 port. */
    uint32_t mac_1_err : 1;     /* Illegal packet error on MAC1 port.
                                   When set indicates illegal packet received from MAC1 port. */
    uint32_t mgmt_err : 1;      /* Illegal packet error on MGMT port.
                                   When set indicates illegal packet received from MGMT port. */
    uint32_t virtio_err : 1;    /* Illegal packet error on VIRTIO port.
                                   When set indicates illegal packet received from VIRTIO port. */
    uint32_t sub_prt_err : 6;   /* Illegal packet error on VIRTIO subport.
                                   VIRTIO subport which received illegal packet. */
    uint32_t : 22;
  } field;
  uint32_t val;
} MKS_PARS_PARS_ILLGPKT_ERR_HLD_t;
#endif /* GENHDR_STRUCT */

/*
 * Parser ILLGLPKT error counter. Illegal packet is any packet where 
 *  total length of all the headers is less than total packet length
 */
#define MKS_PARS_PARS_ILLGPKT_ERR_CNT                   0x00000068U
/*
 * Stats counter for number of illegal packets coming into Parser. 
 *  This is cumulative count for all the ports. No per packet count.
 */
#define MKS_PARS_PARS_ILLGPKT_ERR_CNT_PACKETS_SHIFT     0
#define MKS_PARS_PARS_ILLGPKT_ERR_CNT_PACKETS_WIDTH     8
#define MKS_PARS_PARS_ILLGPKT_ERR_CNT_PACKETS_MASK      (0xFFU << MKS_PARS_PARS_ILLGPKT_ERR_CNT_PACKETS_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Parser ILLGLPKT error counter. Illegal packet is any packet where 
 *  total length of all the headers is less than total packet length
 */
typedef union {
  struct {
    uint32_t packets : 8;       /* Stats counter for number of illegal packets coming into Parser.
                                   This is cumulative count for all the ports. No per packet count. */
    uint32_t : 24;
  } field;
  uint32_t val;
} MKS_PARS_PARS_ILLGPKT_ERR_CNT_t;
#endif /* GENHDR_STRUCT */

/* Rcirculation packets with portId as 0  */
#define MKS_PARS_PARS_RECIRPORT0_CNT                    0x0000006CU
/*
 * Counts number of packets for Recirculation packets with portId as 0 (free
 * running counter).
 */
#define MKS_PARS_PARS_RECIRPORT0_CNT_PACKETS_SHIFT      0
#define MKS_PARS_PARS_RECIRPORT0_CNT_PACKETS_WIDTH      32
#define MKS_PARS_PARS_RECIRPORT0_CNT_PACKETS_MASK       (0xFFFFFFFFU << MKS_PARS_PARS_RECIRPORT0_CNT_PACKETS_SHIFT)

#ifdef GENHDR_STRUCT
/* Rcirculation packets with portId as 0  */
typedef union {
  struct {
    uint32_t packets : 32;      /* Counts number of packets for Recirculation packets with portId as 0 (free
                                   running counter). */
  } field;
  uint32_t val;
} MKS_PARS_PARS_RECIRPORT0_CNT_t;
#endif /* GENHDR_STRUCT */

/* Rcirculation packets with portId as 1  */
#define MKS_PARS_PARS_RECIRPORT1_CNT                    0x00000070U
/*
 * Counts number of packets for Recirculation packets with portId as 1 (free
 * running counter).
 */
#define MKS_PARS_PARS_RECIRPORT1_CNT_PACKETS_SHIFT      0
#define MKS_PARS_PARS_RECIRPORT1_CNT_PACKETS_WIDTH      32
#define MKS_PARS_PARS_RECIRPORT1_CNT_PACKETS_MASK       (0xFFFFFFFFU << MKS_PARS_PARS_RECIRPORT1_CNT_PACKETS_SHIFT)

#ifdef GENHDR_STRUCT
/* Rcirculation packets with portId as 1  */
typedef union {
  struct {
    uint32_t packets : 32;      /* Counts number of packets for Recirculation packets with portId as 1 (free
                                   running counter). */
  } field;
  uint32_t val;
} MKS_PARS_PARS_RECIRPORT1_CNT_t;
#endif /* GENHDR_STRUCT */

/* Rcirculation packets with portId as 2  */
#define MKS_PARS_PARS_RECIRPORT2_CNT                    0x00000074U
/*
 * Counts number of packets for Recirculation packets with portId as 2 (free
 * running counter).
 */
#define MKS_PARS_PARS_RECIRPORT2_CNT_PACKETS_SHIFT      0
#define MKS_PARS_PARS_RECIRPORT2_CNT_PACKETS_WIDTH      32
#define MKS_PARS_PARS_RECIRPORT2_CNT_PACKETS_MASK       (0xFFFFFFFFU << MKS_PARS_PARS_RECIRPORT2_CNT_PACKETS_SHIFT)

#ifdef GENHDR_STRUCT
/* Rcirculation packets with portId as 2  */
typedef union {
  struct {
    uint32_t packets : 32;      /* Counts number of packets for Recirculation packets with portId as 2 (free
                                   running counter). */
  } field;
  uint32_t val;
} MKS_PARS_PARS_RECIRPORT2_CNT_t;
#endif /* GENHDR_STRUCT */

/* Rcirculation packets with portId as 3  */
#define MKS_PARS_PARS_RECIRPORT3_CNT                    0x00000078U
/*
 * Counts number of packets for Recirculation packets with portId as 3 (free
 * running counter).
 */
#define MKS_PARS_PARS_RECIRPORT3_CNT_PACKETS_SHIFT      0
#define MKS_PARS_PARS_RECIRPORT3_CNT_PACKETS_WIDTH      32
#define MKS_PARS_PARS_RECIRPORT3_CNT_PACKETS_MASK       (0xFFFFFFFFU << MKS_PARS_PARS_RECIRPORT3_CNT_PACKETS_SHIFT)

#ifdef GENHDR_STRUCT
/* Rcirculation packets with portId as 3  */
typedef union {
  struct {
    uint32_t packets : 32;      /* Counts number of packets for Recirculation packets with portId as 3 (free
                                   running counter). */
  } field;
  uint32_t val;
} MKS_PARS_PARS_RECIRPORT3_CNT_t;
#endif /* GENHDR_STRUCT */

/*
 * This register has all the mask bits per tuple field 
 *  On setting mask bit the respective field in the key will be zeroed out
 */
#define MKS_PARS_PARS_MSKKEY_CFG                                0x0000007CU
/* Set this bit to zero out the outer dst_mac(dst_mac_0) field in the key. */
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_DSTMAC0_SHIFT              0
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_DSTMAC0_WIDTH              1
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_DSTMAC0_MASK               (0x1U << MKS_PARS_PARS_MSKKEY_CFG_MSK_DSTMAC0_SHIFT)

/* Set this bit to zero out the outer src_mac(src_mac_0) field in the key. */
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_SRCMAC0_SHIFT              1
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_SRCMAC0_WIDTH              1
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_SRCMAC0_MASK               (0x1U << MKS_PARS_PARS_MSKKEY_CFG_MSK_SRCMAC0_SHIFT)

/*
 * Set this bit to zero out the outer l4_dst_port(l4_dst_port_0) field in the
 * key.
 */
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_L4_DST0_SHIFT              2
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_L4_DST0_WIDTH              1
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_L4_DST0_MASK               (0x1U << MKS_PARS_PARS_MSKKEY_CFG_MSK_L4_DST0_SHIFT)

/*
 * Set this bit to zero out the outer l4_src_port(l4_src_port_0) field in the
 * key for tunnel packets.
 */
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_L4_SRC0_SHIFT              3
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_L4_SRC0_WIDTH              1
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_L4_SRC0_MASK               (0x1U << MKS_PARS_PARS_MSKKEY_CFG_MSK_L4_SRC0_SHIFT)

/* Set this bit to zero out the outer dst_ip(dst_ip_0) field in the key. */
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_DSTIP0_SHIFT               4
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_DSTIP0_WIDTH               1
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_DSTIP0_MASK                (0x1U << MKS_PARS_PARS_MSKKEY_CFG_MSK_DSTIP0_SHIFT)

/* Set this bit to zero out the outer src_ip(src_ip_0) field in the key. */
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_SRCIP0_SHIFT               5
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_SRCIP0_WIDTH               1
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_SRCIP0_MASK                (0x1U << MKS_PARS_PARS_MSKKEY_CFG_MSK_SRCIP0_SHIFT)

/* Set this bit to zero out the outer tci_vlana(tci_vlana_0) field in the key. */
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_TCIVLANA0_SHIFT            6
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_TCIVLANA0_WIDTH            1
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_TCIVLANA0_MASK             (0x1U << MKS_PARS_PARS_MSKKEY_CFG_MSK_TCIVLANA0_SHIFT)

/* Set this bit to zero out the outer tci_vlanb(tci_vlanb_0) field in the key. */
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_TCIVLANB0_SHIFT            7
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_TCIVLANB0_WIDTH            1
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_TCIVLANB0_MASK             (0x1U << MKS_PARS_PARS_MSKKEY_CFG_MSK_TCIVLANB0_SHIFT)

/* Set this bit to zero out the outer ethtype(ethtype_0) field in the key. */
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_ETYPE0_SHIFT               8
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_ETYPE0_WIDTH               1
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_ETYPE0_MASK                (0x1U << MKS_PARS_PARS_MSKKEY_CFG_MSK_ETYPE0_SHIFT)

/*
 * Set this bit to zero out the outer ip_protocol(ip_0_protocol) field in the
 * key.
 */
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_IPPROT0_SHIFT              9
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_IPPROT0_WIDTH              1
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_IPPROT0_MASK               (0x1U << MKS_PARS_PARS_MSKKEY_CFG_MSK_IPPROT0_SHIFT)

/*
 * Set this bit to zero out the outer ip_protocol(ip_1_protocol) field in the
 * key.
 */
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_IPPROT1_SHIFT              10
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_IPPROT1_WIDTH              1
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_IPPROT1_MASK               (0x1U << MKS_PARS_PARS_MSKKEY_CFG_MSK_IPPROT1_SHIFT)

/* Set this bit to zero out the outer dst_mac(dst_mac_1) field in the key. */
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_DSTMAC1_SHIFT              11
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_DSTMAC1_WIDTH              1
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_DSTMAC1_MASK               (0x1U << MKS_PARS_PARS_MSKKEY_CFG_MSK_DSTMAC1_SHIFT)

/* Set this bit to zero out the outer src_mac(src_mac_1) field in the key. */
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_SRCMAC1_SHIFT              12
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_SRCMAC1_WIDTH              1
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_SRCMAC1_MASK               (0x1U << MKS_PARS_PARS_MSKKEY_CFG_MSK_SRCMAC1_SHIFT)

/*
 * Set this bit to zero out the outer l4_dst_port(l4_dst_port_1) field in the
 * key.
 */
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_L4_DST1_SHIFT              13
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_L4_DST1_WIDTH              1
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_L4_DST1_MASK               (0x1U << MKS_PARS_PARS_MSKKEY_CFG_MSK_L4_DST1_SHIFT)

/*
 * Set this bit to zero out the outer l4_src_port(l4_src_port_1) field in the
 * key.
 */
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_L4_SRC1_SHIFT              14
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_L4_SRC1_WIDTH              1
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_L4_SRC1_MASK               (0x1U << MKS_PARS_PARS_MSKKEY_CFG_MSK_L4_SRC1_SHIFT)

/* Set this bit to zero out the outer dst_ip(dst_ip_1) field in the key. */
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_DSTIP1_SHIFT               15
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_DSTIP1_WIDTH               1
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_DSTIP1_MASK                (0x1U << MKS_PARS_PARS_MSKKEY_CFG_MSK_DSTIP1_SHIFT)

/* Set this bit to zero out the outer src_ip(src_ip_1) field in the key. */
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_SRCIP1_SHIFT               16
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_SRCIP1_WIDTH               1
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_SRCIP1_MASK                (0x1U << MKS_PARS_PARS_MSKKEY_CFG_MSK_SRCIP1_SHIFT)

/* Set this bit to zero out the outer tci_vlana(tci_vlana_1) field in the key. */
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_TCIVLANA1_SHIFT            17
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_TCIVLANA1_WIDTH            1
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_TCIVLANA1_MASK             (0x1U << MKS_PARS_PARS_MSKKEY_CFG_MSK_TCIVLANA1_SHIFT)

/* Set this bit to zero out the outer tci_vlanb(tci_vlanb_1) field in the key. */
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_TCIVLANB1_SHIFT            18
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_TCIVLANB1_WIDTH            1
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_TCIVLANB1_MASK             (0x1U << MKS_PARS_PARS_MSKKEY_CFG_MSK_TCIVLANB1_SHIFT)

/* Set this bit to zero out the outer ethtype(ethtype_1) field in the key. */
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_ETYPE1_SHIFT               19
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_ETYPE1_WIDTH               1
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_ETYPE1_MASK                (0x1U << MKS_PARS_PARS_MSKKEY_CFG_MSK_ETYPE1_SHIFT)

/* Set this bit to zero out the outer tunnel(vxlnid_or_teid) field in the key. */
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_VXLNTEID_SHIFT             20
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_VXLNTEID_WIDTH             1
#define MKS_PARS_PARS_MSKKEY_CFG_MSK_VXLNTEID_MASK              (0x1U << MKS_PARS_PARS_MSKKEY_CFG_MSK_VXLNTEID_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * This register has all the mask bits per tuple field 
 *  On setting mask bit the respective field in the key will be zeroed out
 */
typedef union {
  struct {
    uint32_t msk_dstmac0 : 1;           /* Set this bit to zero out the outer dst_mac(dst_mac_0) field in the key. */
    uint32_t msk_srcmac0 : 1;           /* Set this bit to zero out the outer src_mac(src_mac_0) field in the key. */
    uint32_t msk_l4_dst0 : 1;           /* Set this bit to zero out the outer l4_dst_port(l4_dst_port_0) field in the
                                           key. */
    uint32_t msk_l4_src0 : 1;           /* Set this bit to zero out the outer l4_src_port(l4_src_port_0) field in the
                                           key for tunnel packets. */
    uint32_t msk_dstip0 : 1;            /* Set this bit to zero out the outer dst_ip(dst_ip_0) field in the key. */
    uint32_t msk_srcip0 : 1;            /* Set this bit to zero out the outer src_ip(src_ip_0) field in the key. */
    uint32_t msk_tcivlana0 : 1;         /* Set this bit to zero out the outer tci_vlana(tci_vlana_0) field in the key. */
    uint32_t msk_tcivlanb0 : 1;         /* Set this bit to zero out the outer tci_vlanb(tci_vlanb_0) field in the key. */
    uint32_t msk_etype0 : 1;            /* Set this bit to zero out the outer ethtype(ethtype_0) field in the key. */
    uint32_t msk_ipprot0 : 1;           /* Set this bit to zero out the outer ip_protocol(ip_0_protocol) field in the
                                           key. */
    uint32_t msk_ipprot1 : 1;           /* Set this bit to zero out the outer ip_protocol(ip_1_protocol) field in the
                                           key. */
    uint32_t msk_dstmac1 : 1;           /* Set this bit to zero out the outer dst_mac(dst_mac_1) field in the key. */
    uint32_t msk_srcmac1 : 1;           /* Set this bit to zero out the outer src_mac(src_mac_1) field in the key. */
    uint32_t msk_l4_dst1 : 1;           /* Set this bit to zero out the outer l4_dst_port(l4_dst_port_1) field in the
                                           key. */
    uint32_t msk_l4_src1 : 1;           /* Set this bit to zero out the outer l4_src_port(l4_src_port_1) field in the
                                           key. */
    uint32_t msk_dstip1 : 1;            /* Set this bit to zero out the outer dst_ip(dst_ip_1) field in the key. */
    uint32_t msk_srcip1 : 1;            /* Set this bit to zero out the outer src_ip(src_ip_1) field in the key. */
    uint32_t msk_tcivlana1 : 1;         /* Set this bit to zero out the outer tci_vlana(tci_vlana_1) field in the key. */
    uint32_t msk_tcivlanb1 : 1;         /* Set this bit to zero out the outer tci_vlanb(tci_vlanb_1) field in the key. */
    uint32_t msk_etype1 : 1;            /* Set this bit to zero out the outer ethtype(ethtype_1) field in the key. */
    uint32_t msk_vxlnteid : 1;          /* Set this bit to zero out the outer tunnel(vxlnid_or_teid) field in the key. */
    uint32_t : 11;
  } field;
  uint32_t val;
} MKS_PARS_PARS_MSKKEY_CFG_t;
#endif /* GENHDR_STRUCT */

#endif /* _MKS_PARS_REGISTERS_H_ */

