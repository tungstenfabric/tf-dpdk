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

#ifndef _VIRTIO_LIVE_MIGRATION_REGISTERS_H_
#define _VIRTIO_LIVE_MIGRATION_REGISTERS_H_

/* Virtio Live Migration */

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

#define VIRTIO_LIVE_MIGRATION_ACCESSTYPE GENHDR_ACCESSTYPE_RW /*  Default access type. Access types defined above. */
#define VIRTIO_LIVE_MIGRATION_REGWIDTH  32 /* Default width of register in bits */
#define VIRTIO_LIVE_MIGRATION_ACCESSWIDTH 32 /* Default width of access word in bit */


/* Live Migration Control register */
#define VIRTIO_LIVE_MIGRATION_LM_CONTROL                        0x00000000U
/*
 *  (Field name needs update)
 *  0 = Logging disabled
 *  1 = Logging enabled.
 */
#define VIRTIO_LIVE_MIGRATION_LM_CONTROL_LOG_EN_SHIFT           0
#define VIRTIO_LIVE_MIGRATION_LM_CONTROL_LOG_EN_WIDTH           1
#define VIRTIO_LIVE_MIGRATION_LM_CONTROL_LOG_EN_MASK            (0x1U << VIRTIO_LIVE_MIGRATION_LM_CONTROL_LOG_EN_SHIFT)

/*
 *  (Field name needs update)
 *  0 = Log to VF
 *  1 = Log to PF.
 */
#define VIRTIO_LIVE_MIGRATION_LM_CONTROL_VF_PF_SEL_SHIFT        1
#define VIRTIO_LIVE_MIGRATION_LM_CONTROL_VF_PF_SEL_WIDTH        1
#define VIRTIO_LIVE_MIGRATION_LM_CONTROL_VF_PF_SEL_MASK         (0x1U << VIRTIO_LIVE_MIGRATION_LM_CONTROL_VF_PF_SEL_SHIFT)

/*
 *  (Field name needs update)
 *  0 = command_ack
 *  1 = command_pending.
 */
#define VIRTIO_LIVE_MIGRATION_LM_CONTROL_CMD_STATUS_SHIFT       7
#define VIRTIO_LIVE_MIGRATION_LM_CONTROL_CMD_STATUS_WIDTH       1
#define VIRTIO_LIVE_MIGRATION_LM_CONTROL_CMD_STATUS_MASK        (0x1U << VIRTIO_LIVE_MIGRATION_LM_CONTROL_CMD_STATUS_SHIFT)

/*
 * 0x00 = NULL
 *  0x02 = Get ring indexes[pr] 0x03 = Set ring indexes
 *  0x05 = Send RARP. 
 */
#define VIRTIO_LIVE_MIGRATION_LM_CONTROL_COMMAND_SHIFT          8
#define VIRTIO_LIVE_MIGRATION_LM_CONTROL_COMMAND_WIDTH          8
#define VIRTIO_LIVE_MIGRATION_LM_CONTROL_COMMAND_MASK           (0xFFU << VIRTIO_LIVE_MIGRATION_LM_CONTROL_COMMAND_SHIFT)

#ifdef GENHDR_STRUCT
/* Live Migration Control register */
typedef union {
  struct {
    uint32_t log_en : 1;        /* (Field name needs update)
                                   0 = Logging disabled
                                   1 = Logging enabled. */
    uint32_t vf_pf_sel : 1;     /* (Field name needs update)
                                   0 = Log to VF
                                   1 = Log to PF. */
    uint32_t : 5;
    uint32_t cmd_status : 1;    /* (Field name needs update)
                                   0 = command_ack
                                   1 = command_pending. */
    uint32_t command : 8;       /* 0x00 = NULL
                                   0x02 = Get ring indexes[pr] 0x03 = Set ring indexes
                                   0x05 = Send RARP. */
    uint32_t : 16;
  } field;
  uint32_t val;
} VIRTIO_LIVE_MIGRATION_LM_CONTROL_t;
#endif /* GENHDR_STRUCT */

/* Live Migration Status register */
#define VIRTIO_LIVE_MIGRATION_LM_STAT                           0x00000008U
/* No status currently implemented. */
#define VIRTIO_LIVE_MIGRATION_LM_STAT_RESERVED_SHIFT            0
#define VIRTIO_LIVE_MIGRATION_LM_STAT_RESERVED_WIDTH            32
#define VIRTIO_LIVE_MIGRATION_LM_STAT_RESERVED_MASK             (0xFFFFFFFFU << VIRTIO_LIVE_MIGRATION_LM_STAT_RESERVED_SHIFT)

#ifdef GENHDR_STRUCT
/* Live Migration Status register */
typedef union {
  struct {
    uint32_t reserved : 32;     /* No status currently implemented. */
  } field;
  uint32_t val;
} VIRTIO_LIVE_MIGRATION_LM_STAT_t;
#endif /* GENHDR_STRUCT */

/* Live Migration Log Memory Start Address (Lower bytes) */
#define VIRTIO_LIVE_MIGRATION_LM_BASE_ADDR_L                    0x00000010U
/*
 * Lower 32 bits of the 64 bit Start Address for Live Migration Logging (Dirty
 * Page) in memory.
 */
#define VIRTIO_LIVE_MIGRATION_LM_BASE_ADDR_L_LOG_ADDR_SHIFT     0
#define VIRTIO_LIVE_MIGRATION_LM_BASE_ADDR_L_LOG_ADDR_WIDTH     32
#define VIRTIO_LIVE_MIGRATION_LM_BASE_ADDR_L_LOG_ADDR_MASK      (0xFFFFFFFFU << VIRTIO_LIVE_MIGRATION_LM_BASE_ADDR_L_LOG_ADDR_SHIFT)

#ifdef GENHDR_STRUCT
/* Live Migration Log Memory Start Address (Lower bytes) */
typedef union {
  struct {
    uint32_t log_addr : 32;     /* Lower 32 bits of the 64 bit Start Address for Live Migration Logging (Dirty
                                   Page) in memory. */
  } field;
  uint32_t val;
} VIRTIO_LIVE_MIGRATION_LM_BASE_ADDR_L_t;
#endif /* GENHDR_STRUCT */

/* Live Migration Log Memory Start Address (Upper bytes) */
#define VIRTIO_LIVE_MIGRATION_LM_BASE_ADDR_H                    0x00000014U
/*
 * Upper 32 bits of the 64 bit Start Address for Live Migration Logging (Dirty
 * Page) in memory.
 */
#define VIRTIO_LIVE_MIGRATION_LM_BASE_ADDR_H_LOG_ADDR_SHIFT     0
#define VIRTIO_LIVE_MIGRATION_LM_BASE_ADDR_H_LOG_ADDR_WIDTH     32
#define VIRTIO_LIVE_MIGRATION_LM_BASE_ADDR_H_LOG_ADDR_MASK      (0xFFFFFFFFU << VIRTIO_LIVE_MIGRATION_LM_BASE_ADDR_H_LOG_ADDR_SHIFT)

#ifdef GENHDR_STRUCT
/* Live Migration Log Memory Start Address (Upper bytes) */
typedef union {
  struct {
    uint32_t log_addr : 32;     /* Upper 32 bits of the 64 bit Start Address for Live Migration Logging (Dirty
                                   Page) in memory. */
  } field;
  uint32_t val;
} VIRTIO_LIVE_MIGRATION_LM_BASE_ADDR_H_t;
#endif /* GENHDR_STRUCT */

/* Live Migration Log Memory End Address (Lower bytes) */
#define VIRTIO_LIVE_MIGRATION_LM_END_ADDR_L                     0x00000018U
/*
 * Lower 32 bits of the 64 bit End Address for Live Migration Logging (Dirty
 * Page) in memory. This value is the base address + log memory size.
 */
#define VIRTIO_LIVE_MIGRATION_LM_END_ADDR_L_END_ADDR_SHIFT      0
#define VIRTIO_LIVE_MIGRATION_LM_END_ADDR_L_END_ADDR_WIDTH      32
#define VIRTIO_LIVE_MIGRATION_LM_END_ADDR_L_END_ADDR_MASK       (0xFFFFFFFFU << VIRTIO_LIVE_MIGRATION_LM_END_ADDR_L_END_ADDR_SHIFT)

#ifdef GENHDR_STRUCT
/* Live Migration Log Memory End Address (Lower bytes) */
typedef union {
  struct {
    uint32_t end_addr : 32;     /* Lower 32 bits of the 64 bit End Address for Live Migration Logging (Dirty
                                   Page) in memory. This value is the base address + log memory size. */
  } field;
  uint32_t val;
} VIRTIO_LIVE_MIGRATION_LM_END_ADDR_L_t;
#endif /* GENHDR_STRUCT */

/* Live Migration Log Memory End Address (Upper bytes) */
#define VIRTIO_LIVE_MIGRATION_LM_END_ADDR_H                     0x0000001CU
/*
 * Upper 32 bits of the 64 bit End Address for Live Migration Logging (Dirty
 * Page) in memory. This value is the base address + log memory size.
 */
#define VIRTIO_LIVE_MIGRATION_LM_END_ADDR_H_END_ADDR_SHIFT      0
#define VIRTIO_LIVE_MIGRATION_LM_END_ADDR_H_END_ADDR_WIDTH      32
#define VIRTIO_LIVE_MIGRATION_LM_END_ADDR_H_END_ADDR_MASK       (0xFFFFFFFFU << VIRTIO_LIVE_MIGRATION_LM_END_ADDR_H_END_ADDR_SHIFT)

#ifdef GENHDR_STRUCT
/* Live Migration Log Memory End Address (Upper bytes) */
typedef union {
  struct {
    uint32_t end_addr : 32;     /* Upper 32 bits of the 64 bit End Address for Live Migration Logging (Dirty
                                   Page) in memory. This value is the base address + log memory size. */
  } field;
  uint32_t val;
} VIRTIO_LIVE_MIGRATION_LM_END_ADDR_H_t;
#endif /* GENHDR_STRUCT */

/* Live Migration HW Rx Index Registers */
#define VIRTIO_LIVE_MIGRATION_LM_RX_IDX                         0x00000020U
/*
 * Hardware Rx AvailIndex value. 
 * Required for Live Migration to allow migrated VM to start in correct hardware
 * state. 
 * Can also be accessed by half-word access to 0x20.
 */
#define VIRTIO_LIVE_MIGRATION_LM_RX_IDX_RX_AVAIL_IDX_SHIFT      0
#define VIRTIO_LIVE_MIGRATION_LM_RX_IDX_RX_AVAIL_IDX_WIDTH      16
#define VIRTIO_LIVE_MIGRATION_LM_RX_IDX_RX_AVAIL_IDX_MASK       (0xFFFFU << VIRTIO_LIVE_MIGRATION_LM_RX_IDX_RX_AVAIL_IDX_SHIFT)

/*
 * Hardware Rx UsedIndex value. 
 * Required for Live Migration to allow migrated VM to start in correct hardware
 * state. 
 * Can also be accessed by half-word access to 0x22.
 */
#define VIRTIO_LIVE_MIGRATION_LM_RX_IDX_RX_USED_IDX_SHIFT       16
#define VIRTIO_LIVE_MIGRATION_LM_RX_IDX_RX_USED_IDX_WIDTH       16
#define VIRTIO_LIVE_MIGRATION_LM_RX_IDX_RX_USED_IDX_MASK        (0xFFFFU << VIRTIO_LIVE_MIGRATION_LM_RX_IDX_RX_USED_IDX_SHIFT)

#ifdef GENHDR_STRUCT
/* Live Migration HW Rx Index Registers */
typedef union {
  struct {
    uint32_t rx_avail_idx : 16; /* Hardware Rx AvailIndex value.
                                   Required for Live Migration to allow migrated VM to start in correct hardware
                                   state.
                                   Can also be accessed by half-word access to 0x20. */
    uint32_t rx_used_idx : 16;  /* Hardware Rx UsedIndex value.
                                   Required for Live Migration to allow migrated VM to start in correct hardware
                                   state.
                                   Can also be accessed by half-word access to 0x22. */
  } field;
  uint32_t val;
} VIRTIO_LIVE_MIGRATION_LM_RX_IDX_t;
#endif /* GENHDR_STRUCT */

/* Live Migration HW Tx Index Registers */
#define VIRTIO_LIVE_MIGRATION_LM_TX_IDX                         0x00000024U
/*
 * Hardware Tx AvailIndex value. 
 * Required for Live Migration to allow migrated VM to start in correct hardware
 * state. 
 * Can also be accessed by half-word access to 0x24.
 */
#define VIRTIO_LIVE_MIGRATION_LM_TX_IDX_TX_AVAIL_IDX_SHIFT      0
#define VIRTIO_LIVE_MIGRATION_LM_TX_IDX_TX_AVAIL_IDX_WIDTH      16
#define VIRTIO_LIVE_MIGRATION_LM_TX_IDX_TX_AVAIL_IDX_MASK       (0xFFFFU << VIRTIO_LIVE_MIGRATION_LM_TX_IDX_TX_AVAIL_IDX_SHIFT)

/*
 * Hardware Tx UsedIndex value. 
 * Required for Live Migration to allow migrated VM to start in correct hardware
 * state. 
 * Can also be accessed by half-word access to 0x26.
 */
#define VIRTIO_LIVE_MIGRATION_LM_TX_IDX_TX_USED_IDX_SHIFT       16
#define VIRTIO_LIVE_MIGRATION_LM_TX_IDX_TX_USED_IDX_WIDTH       16
#define VIRTIO_LIVE_MIGRATION_LM_TX_IDX_TX_USED_IDX_MASK        (0xFFFFU << VIRTIO_LIVE_MIGRATION_LM_TX_IDX_TX_USED_IDX_SHIFT)

#ifdef GENHDR_STRUCT
/* Live Migration HW Tx Index Registers */
typedef union {
  struct {
    uint32_t tx_avail_idx : 16; /* Hardware Tx AvailIndex value.
                                   Required for Live Migration to allow migrated VM to start in correct hardware
                                   state.
                                   Can also be accessed by half-word access to 0x24. */
    uint32_t tx_used_idx : 16;  /* Hardware Tx UsedIndex value.
                                   Required for Live Migration to allow migrated VM to start in correct hardware
                                   state.
                                   Can also be accessed by half-word access to 0x26. */
  } field;
  uint32_t val;
} VIRTIO_LIVE_MIGRATION_LM_TX_IDX_t;
#endif /* GENHDR_STRUCT */

/* Live Migration Virtqueue Pairs Register */
#define VIRTIO_LIVE_MIGRATION_LM_VQP                    0x00000028U
/*
 * Hardware Virtqueue Pairs value. 
 * Required for Live Migration to allow migrated VM to start in correct hardware
 * state without needing to send ctrl queue messages. 
 * Can also be accessed by half-word access to 0x28. 
 *  Write Only.
 */
#define VIRTIO_LIVE_MIGRATION_LM_VQP_VQ_PAIRS_ACCESSTYPE GENHDR_ACCESSTYPE_WO
#define VIRTIO_LIVE_MIGRATION_LM_VQP_VQ_PAIRS_SHIFT     0
#define VIRTIO_LIVE_MIGRATION_LM_VQP_VQ_PAIRS_WIDTH     16
#define VIRTIO_LIVE_MIGRATION_LM_VQP_VQ_PAIRS_MASK      (0xFFFFU << VIRTIO_LIVE_MIGRATION_LM_VQP_VQ_PAIRS_SHIFT)

/* Reserved. */
#define VIRTIO_LIVE_MIGRATION_LM_VQP_RESERVED_ACCESSTYPE GENHDR_ACCESSTYPE_WO
#define VIRTIO_LIVE_MIGRATION_LM_VQP_RESERVED_SHIFT     16
#define VIRTIO_LIVE_MIGRATION_LM_VQP_RESERVED_WIDTH     16
#define VIRTIO_LIVE_MIGRATION_LM_VQP_RESERVED_MASK      (0xFFFFU << VIRTIO_LIVE_MIGRATION_LM_VQP_RESERVED_SHIFT)

#ifdef GENHDR_STRUCT
/* Live Migration Virtqueue Pairs Register */
typedef union {
  struct {
    uint32_t vq_pairs : 16;     /* Hardware Virtqueue Pairs value.
                                   Required for Live Migration to allow migrated VM to start in correct hardware
                                   state without needing to send ctrl queue messages.
                                   Can also be accessed by half-word access to 0x28.
                                   Write Only. */
    uint32_t reserved : 16;     /* Reserved. */
  } field;
  uint32_t val;
} VIRTIO_LIVE_MIGRATION_LM_VQP_t;
#endif /* GENHDR_STRUCT */

#endif /* _VIRTIO_LIVE_MIGRATION_REGISTERS_H_ */

