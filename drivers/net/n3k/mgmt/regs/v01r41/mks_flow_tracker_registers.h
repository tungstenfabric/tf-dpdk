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

#ifndef _MKS_FLOW_TRACKER_REGISTERS_H_
#define _MKS_FLOW_TRACKER_REGISTERS_H_

/* Flow Tracker */

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

#define MKS_FLOW_TRACKER_ACCESSTYPE     GENHDR_ACCESSTYPE_RW /*  Default access type. Access types defined above. */
#define MKS_FLOW_TRACKER_REGWIDTH       32 /* Default width of register in bits */
#define MKS_FLOW_TRACKER_ACCESSWIDTH    32 /* Default width of access word in bit */


/* Version Number of This Module */
#define MKS_FLOW_TRACKER_FT_VERSION                     0x00000000U
/* Version format: DDMMYY_XX. */
#define MKS_FLOW_TRACKER_FT_VERSION_VERSION_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define MKS_FLOW_TRACKER_FT_VERSION_VERSION_SHIFT       0
#define MKS_FLOW_TRACKER_FT_VERSION_VERSION_WIDTH       32
#define MKS_FLOW_TRACKER_FT_VERSION_VERSION_MASK        (0xFFFFFFFFU << MKS_FLOW_TRACKER_FT_VERSION_VERSION_SHIFT)

#ifdef GENHDR_STRUCT
/* Version Number of This Module */
typedef union {
  struct {
    uint32_t version : 32;      /* Version format: DDMMYY_XX. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_VERSION_t;
#endif /* GENHDR_STRUCT */

/* A General Purpose R/W Register Intended for Debug Use */
#define MKS_FLOW_TRACKER_FT_SCRATCH                     0x00000004U
/* Test data. */
#define MKS_FLOW_TRACKER_FT_SCRATCH_SCRATCH_SHIFT       0
#define MKS_FLOW_TRACKER_FT_SCRATCH_SCRATCH_WIDTH       32
#define MKS_FLOW_TRACKER_FT_SCRATCH_SCRATCH_MASK        (0xFFFFFFFFU << MKS_FLOW_TRACKER_FT_SCRATCH_SCRATCH_SHIFT)

#ifdef GENHDR_STRUCT
/* A General Purpose R/W Register Intended for Debug Use */
typedef union {
  struct {
    uint32_t scratch : 32;      /* Test data. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_SCRATCH_t;
#endif /* GENHDR_STRUCT */

/* Provides Initialisation and Alarm Status Bits */
#define MKS_FLOW_TRACKER_FT_STAT                        0x00000008U
/*
 * 0x1 = the block has initialised and is ready for configuration and requests.
 *  0x0 = the block has yet to complete its initialisation; do not perform any
 * configuration or send in any requests (these will be discarded).
 *  Reset to 0.
 */
#define MKS_FLOW_TRACKER_FT_STAT_INIT_DONE_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define MKS_FLOW_TRACKER_FT_STAT_INIT_DONE_SHIFT        0
#define MKS_FLOW_TRACKER_FT_STAT_INIT_DONE_WIDTH        1
#define MKS_FLOW_TRACKER_FT_STAT_INIT_DONE_MASK         (0x1U << MKS_FLOW_TRACKER_FT_STAT_INIT_DONE_SHIFT)

/*
 * 0x1 = an unexpected but potentially recoverable condition has been detected
 * in the block. Access the Warning_0 register to determine the cause and clear
 * the alarm. Reset to 0.
 */
#define MKS_FLOW_TRACKER_FT_STAT_WARNING_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_FLOW_TRACKER_FT_STAT_WARNING_SHIFT          30
#define MKS_FLOW_TRACKER_FT_STAT_WARNING_WIDTH          1
#define MKS_FLOW_TRACKER_FT_STAT_WARNING_MASK           (0x1U << MKS_FLOW_TRACKER_FT_STAT_WARNING_SHIFT)

/*
 * 0x1 = an unexpected and fatal condition has been detected in the block.
 * Access the Fatal_0 register to determine the cause. Reset to 0.
 */
#define MKS_FLOW_TRACKER_FT_STAT_FATAL_ERROR_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_FLOW_TRACKER_FT_STAT_FATAL_ERROR_SHIFT      31
#define MKS_FLOW_TRACKER_FT_STAT_FATAL_ERROR_WIDTH      1
#define MKS_FLOW_TRACKER_FT_STAT_FATAL_ERROR_MASK       (0x1U << MKS_FLOW_TRACKER_FT_STAT_FATAL_ERROR_SHIFT)

#ifdef GENHDR_STRUCT
/* Provides Initialisation and Alarm Status Bits */
typedef union {
  struct {
    uint32_t init_done : 1;     /* 0x1 = the block has initialised and is ready for configuration and requests.
                                   0x0 = the block has yet to complete its initialisation; do not perform any
                                   configuration or send in any requests (these will be discarded).
                                   Reset to 0. */
    uint32_t : 29;
    uint32_t warning : 1;       /* 0x1 = an unexpected but potentially recoverable condition has been detected
                                   in the block. Access the Warning_0 register to determine the cause and clear
                                   the alarm. Reset to 0. */
    uint32_t fatal_error : 1;   /* 0x1 = an unexpected and fatal condition has been detected in the block.
                                   Access the Fatal_0 register to determine the cause. Reset to 0. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_STAT_t;
#endif /* GENHDR_STRUCT */

/* Indicates the Cause of a Warning Alarm */
#define MKS_FLOW_TRACKER_FT_WARNING_0                                   0x00000020U
/*
 * 1 = the CPU has been prevented from reading or writing to an out-of-range
 * flow index (flow index >= 2^LOG2_FLOWS, or flow index >= INTERNAL_FLOWS if
 * this is nonzero).
 *  Write a 1 to clear this bit.
 *  Reset to 0. 
 */
#define MKS_FLOW_TRACKER_FT_WARNING_0_WARN_CPU_CMD_INDEX_ACCESSTYPE     GENHDR_ACCESSTYPE_W1C
#define MKS_FLOW_TRACKER_FT_WARNING_0_WARN_CPU_CMD_INDEX_SHIFT          0
#define MKS_FLOW_TRACKER_FT_WARNING_0_WARN_CPU_CMD_INDEX_WIDTH          1
#define MKS_FLOW_TRACKER_FT_WARNING_0_WARN_CPU_CMD_INDEX_MASK           (0x1U << MKS_FLOW_TRACKER_FT_WARNING_0_WARN_CPU_CMD_INDEX_SHIFT)

/*
 * 1 = requests have been discarded because the processing cannot keep pace.
 *  Write a 1 to clear this bit.
 *  Reset to 0. 
 */
#define MKS_FLOW_TRACKER_FT_WARNING_0_WARN_REQ_DISCARD_ACCESSTYPE       GENHDR_ACCESSTYPE_W1C
#define MKS_FLOW_TRACKER_FT_WARNING_0_WARN_REQ_DISCARD_SHIFT            4
#define MKS_FLOW_TRACKER_FT_WARNING_0_WARN_REQ_DISCARD_WIDTH            1
#define MKS_FLOW_TRACKER_FT_WARNING_0_WARN_REQ_DISCARD_MASK             (0x1U << MKS_FLOW_TRACKER_FT_WARNING_0_WARN_REQ_DISCARD_SHIFT)

/*
 * 1 = a request has been discarded for an out-of-range flow index (flow index
 * >= INTERNAL_FLOWS if this is nonzero).
 *  Write a 1 to clear this bit.
 *  Reset to 0. 
 */
#define MKS_FLOW_TRACKER_FT_WARNING_0_WARN_REQ_INDEX_ACCESSTYPE         GENHDR_ACCESSTYPE_W1C
#define MKS_FLOW_TRACKER_FT_WARNING_0_WARN_REQ_INDEX_SHIFT              5
#define MKS_FLOW_TRACKER_FT_WARNING_0_WARN_REQ_INDEX_WIDTH              1
#define MKS_FLOW_TRACKER_FT_WARNING_0_WARN_REQ_INDEX_MASK               (0x1U << MKS_FLOW_TRACKER_FT_WARNING_0_WARN_REQ_INDEX_SHIFT)

#ifdef GENHDR_STRUCT
/* Indicates the Cause of a Warning Alarm */
typedef union {
  struct {
    uint32_t warn_cpu_cmd_index : 1;    /* 1 = the CPU has been prevented from reading or writing to an out-of-range
                                           flow index (flow index >= 2^LOG2_FLOWS, or flow index >= INTERNAL_FLOWS if
                                           this is nonzero).
                                           Write a 1 to clear this bit.
                                           Reset to 0. */
    uint32_t : 3;
    uint32_t warn_req_discard : 1;      /* 1 = requests have been discarded because the processing cannot keep pace.
                                           Write a 1 to clear this bit.
                                           Reset to 0. */
    uint32_t warn_req_index : 1;        /* 1 = a request has been discarded for an out-of-range flow index (flow index
                                           >= INTERNAL_FLOWS if this is nonzero).
                                           Write a 1 to clear this bit.
                                           Reset to 0. */
    uint32_t : 26;
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_WARNING_0_t;
#endif /* GENHDR_STRUCT */

/* Provides Various Monitoring Flags for the Logic */
#define MKS_FLOW_TRACKER_FT_MON_0                       0x00000028U
/* Provides Various Monitoring Flags for the Logic. */
#define MKS_FLOW_TRACKER_FT_MON_0_MONITOR_SHIFT         0
#define MKS_FLOW_TRACKER_FT_MON_0_MONITOR_WIDTH         32
#define MKS_FLOW_TRACKER_FT_MON_0_MONITOR_MASK          (0xFFFFFFFFU << MKS_FLOW_TRACKER_FT_MON_0_MONITOR_SHIFT)

#ifdef GENHDR_STRUCT
/* Provides Various Monitoring Flags for the Logic */
typedef union {
  struct {
    uint32_t monitor : 32;      /* Provides Various Monitoring Flags for the Logic. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_MON_0_t;
#endif /* GENHDR_STRUCT */

/* Provides Various Fatal Error Flags for the Logic */
#define MKS_FLOW_TRACKER_FT_FATAL_0                             0x00000030U
/* Provides various fatal error flags for the logic. Reset to 0. */
#define MKS_FLOW_TRACKER_FT_FATAL_0_FATAL_ERROR_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define MKS_FLOW_TRACKER_FT_FATAL_0_FATAL_ERROR_SHIFT           0
#define MKS_FLOW_TRACKER_FT_FATAL_0_FATAL_ERROR_WIDTH           32
#define MKS_FLOW_TRACKER_FT_FATAL_0_FATAL_ERROR_MASK            (0xFFFFFFFFU << MKS_FLOW_TRACKER_FT_FATAL_0_FATAL_ERROR_SHIFT)

#ifdef GENHDR_STRUCT
/* Provides Various Fatal Error Flags for the Logic */
typedef union {
  struct {
    uint32_t fatal_error : 32;  /* Provides various fatal error flags for the logic. Reset to 0. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_FATAL_0_t;
#endif /* GENHDR_STRUCT */

/*
 * These two registers allow the current timestamp value to be read (the
 * timestamp is a free-running 24 bit counter with a granularity of 100ms). The
 * Timestamp_0 register is first read first to latch the value and to provide
 * the LSBs (only the lower 24 bits are valid); the Timestamp_1 register can
 * then be read to provide any corresponding MSBs (this is not currently used)
 */
#define MKS_FLOW_TRACKER_FT_TIMESTAMP_N(ft_timestamp_n_ix)      (0x00000080U + ((ft_timestamp_n_ix) * 4))
#define MKS_FLOW_TRACKER_FT_TIMESTAMP_N_MAX_INDEX               0x1U
/* Upper or lower 32 bits of the current timestamp. */
#define MKS_FLOW_TRACKER_FT_TIMESTAMP_N_TIMESTAMP_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define MKS_FLOW_TRACKER_FT_TIMESTAMP_N_TIMESTAMP_SHIFT         0
#define MKS_FLOW_TRACKER_FT_TIMESTAMP_N_TIMESTAMP_WIDTH         32
#define MKS_FLOW_TRACKER_FT_TIMESTAMP_N_TIMESTAMP_MASK          (0xFFFFFFFFU << MKS_FLOW_TRACKER_FT_TIMESTAMP_N_TIMESTAMP_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * These two registers allow the current timestamp value to be read (the
 * timestamp is a free-running 24 bit counter with a granularity of 100ms). The
 * Timestamp_0 register is first read first to latch the value and to provide
 * the LSBs (only the lower 24 bits are valid); the Timestamp_1 register can
 * then be read to provide any corresponding MSBs (this is not currently used)
 */
typedef union {
  struct {
    uint32_t timestamp : 32;    /* Upper or lower 32 bits of the current timestamp. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_TIMESTAMP_N_t;
#endif /* GENHDR_STRUCT */

/*
 * A write to this register initiates a write or read access to state table
 * location X, to update or view the new_state that is assigned during request
 * processing when {ptype, current_state[4:0], SYN, ACK, FIN, RST} = X
 */
#define MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG                             0x000000A0U
/*
 * The table entry to be modified.
 *  index[9] = ptype
 *  index[8:4] = current_state[4:0]
 *  index[3] = SYN
 *  index[2] = ACK
 *  index[1] = FIN
 *  index[0] = RST. 
 */
#define MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_INDEX_SHIFT                 0
#define MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_INDEX_WIDTH                 10
#define MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_INDEX_MASK                  (0x3FFU << MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_INDEX_SHIFT)

/* The value to be written to, or read from, the table. */
#define MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_NEW_STATE_SHIFT             16
#define MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_NEW_STATE_WIDTH             5
#define MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_NEW_STATE_MASK              (0x1FU << MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_NEW_STATE_SHIFT)

/*
 * 0x0 = The timestamp is updated.
 *  0x1 = The timestamp is not updated. 
 */
#define MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_DIS_TSTAMP_UPDATE_SHIFT     24
#define MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_DIS_TSTAMP_UPDATE_WIDTH     1
#define MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_DIS_TSTAMP_UPDATE_MASK      (0x1U << MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_DIS_TSTAMP_UPDATE_SHIFT)

/*
 * 0x0 = Read (perform a read).
 *  0x1 = Write (perform a write). 
 */
#define MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_OPERATION_SHIFT             28
#define MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_OPERATION_WIDTH             1
#define MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_OPERATION_MASK              (0x1U << MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_OPERATION_SHIFT)

/*
 * This bit is automatically set when the register is written, and is
 * automatically cleared when the logic has completed the request.
 *  0x1 = a request is ongoing and this register must not be written to.
 *  0x0 = any previous request has completed (and the new_state field gives the
 * data for a read request) and another request may be made using a write to
 * this register.
 *  Reset to 0. 
 */
#define MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_BUSY_ACCESSTYPE             GENHDR_ACCESSTYPE_RO
#define MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_BUSY_SHIFT                  31
#define MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_BUSY_WIDTH                  1
#define MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_BUSY_MASK                   (0x1U << MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_BUSY_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * A write to this register initiates a write or read access to state table
 * location X, to update or view the new_state that is assigned during request
 * processing when {ptype, current_state[4:0], SYN, ACK, FIN, RST} = X
 */
typedef union {
  struct {
    uint32_t index : 10;                /* The table entry to be modified.
                                           index[9] = ptype
                                           index[8:4] = current_state[4:0]
                                           index[3] = SYN
                                           index[2] = ACK
                                           index[1] = FIN
                                           index[0] = RST. */
    uint32_t : 6;
    uint32_t new_state : 5;             /* The value to be written to, or read from, the table. */
    uint32_t : 3;
    uint32_t dis_tstamp_update : 1;     /* 0x0 = The timestamp is updated.
                                           0x1 = The timestamp is not updated. */
    uint32_t : 3;
    uint32_t operation : 1;             /* 0x0 = Read (perform a read).
                                           0x1 = Write (perform a write). */
    uint32_t : 2;
    uint32_t busy : 1;                  /* This bit is automatically set when the register is written, and is
                                           automatically cleared when the logic has completed the request.
                                           0x1 = a request is ongoing and this register must not be written to.
                                           0x0 = any previous request has completed (and the new_state field gives the
                                           data for a read request) and another request may be made using a write to
                                           this register.
                                           Reset to 0. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_t;
#endif /* GENHDR_STRUCT */

/*
 * A write to this register initiates a structure write or structure read
 * operation. The Data_N registers specify the data for a write, and hold the
 * data after a read
 */
#define MKS_FLOW_TRACKER_FT_COMMAND                     0x000000C0U
/* Specifies the structure to be targeted (some MSBs may be unused). */
#define MKS_FLOW_TRACKER_FT_COMMAND_INDEX_SHIFT         0
#define MKS_FLOW_TRACKER_FT_COMMAND_INDEX_WIDTH         28
#define MKS_FLOW_TRACKER_FT_COMMAND_INDEX_MASK          (0xFFFFFFFU << MKS_FLOW_TRACKER_FT_COMMAND_INDEX_SHIFT)

/*
 * 0x0 = Read (just performs a read).
 *  0x1 = Read_Clear (performs a read and then the logic immediately performs a
 * write to clear the dirty bit).
 *  0x2 = Write (write all 64B or 32B).
 *  0x3 = Partial_Write (write all 64B or 32B but leave the timestamp,
 * packet_count, byte_count, state and dirty fields unmodified).
 *  0x4 = Flush (zero all word32s for all structures).
 *  0x5 -0x7 = Reserved. 
 */
#define MKS_FLOW_TRACKER_FT_COMMAND_OPERATION_SHIFT     28
#define MKS_FLOW_TRACKER_FT_COMMAND_OPERATION_WIDTH     3
#define MKS_FLOW_TRACKER_FT_COMMAND_OPERATION_MASK      (0x7U << MKS_FLOW_TRACKER_FT_COMMAND_OPERATION_SHIFT)

/*
 * This bit is automatically set when the register is written, and is
 * automatically cleared when the logic has completed the request.
 *  0x1 = a request is ongoing and this register must not be written to.
 *  0x0 = any previous request has completed (and the data for a read request
 * can be read from the Data_N registers) and another request may be made using
 * a write to this register.
 *  Reset to 0. 
 */
#define MKS_FLOW_TRACKER_FT_COMMAND_BUSY_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_FLOW_TRACKER_FT_COMMAND_BUSY_SHIFT          31
#define MKS_FLOW_TRACKER_FT_COMMAND_BUSY_WIDTH          1
#define MKS_FLOW_TRACKER_FT_COMMAND_BUSY_MASK           (0x1U << MKS_FLOW_TRACKER_FT_COMMAND_BUSY_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * A write to this register initiates a structure write or structure read
 * operation. The Data_N registers specify the data for a write, and hold the
 * data after a read
 */
typedef union {
  struct {
    uint32_t index : 28;        /* Specifies the structure to be targeted (some MSBs may be unused). */
    uint32_t operation : 3;     /* 0x0 = Read (just performs a read).
                                   0x1 = Read_Clear (performs a read and then the logic immediately performs a
                                   write to clear the dirty bit).
                                   0x2 = Write (write all 64B or 32B).
                                   0x3 = Partial_Write (write all 64B or 32B but leave the timestamp,
                                   packet_count, byte_count, state and dirty fields unmodified).
                                   0x4 = Flush (zero all word32s for all structures).
                                   0x5 -0x7 = Reserved. */
    uint32_t busy : 1;          /* This bit is automatically set when the register is written, and is
                                   automatically cleared when the logic has completed the request.
                                   0x1 = a request is ongoing and this register must not be written to.
                                   0x0 = any previous request has completed (and the data for a read request
                                   can be read from the Data_N registers) and another request may be made using
                                   a write to this register.
                                   Reset to 0. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_COMMAND_t;
#endif /* GENHDR_STRUCT */

/*
 * Register used to specify or read the data for word32 0 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
#define MKS_FLOW_TRACKER_FT_DATA_0                              0x00000400U
/* 'cookie'. */
#define MKS_FLOW_TRACKER_FT_DATA_0_COOKIE_SHIFT                 0
#define MKS_FLOW_TRACKER_FT_DATA_0_COOKIE_WIDTH                 24
#define MKS_FLOW_TRACKER_FT_DATA_0_COOKIE_MASK                  (0xFFFFFFU << MKS_FLOW_TRACKER_FT_DATA_0_COOKIE_SHIFT)

/* Bits 7:0 of 'mirror_index_b'. */
#define MKS_FLOW_TRACKER_FT_DATA_0_MIRROR_IDX_B7_0_SHIFT        24
#define MKS_FLOW_TRACKER_FT_DATA_0_MIRROR_IDX_B7_0_WIDTH        8
#define MKS_FLOW_TRACKER_FT_DATA_0_MIRROR_IDX_B7_0_MASK         (0xFFU << MKS_FLOW_TRACKER_FT_DATA_0_MIRROR_IDX_B7_0_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Register used to specify or read the data for word32 0 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
typedef union {
  struct {
    uint32_t cookie : 24;               /* 'cookie'. */
    uint32_t mirror_idx_b7_0 : 8;       /* Bits 7:0 of 'mirror_index_b'. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_DATA_0_t;
#endif /* GENHDR_STRUCT */

/*
 * Register used to specify or read the data for word32 1 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
#define MKS_FLOW_TRACKER_FT_DATA_1                              0x00000404U
/* 'mirror_index_a'. */
#define MKS_FLOW_TRACKER_FT_DATA_1_MIRROR_IDX_A_SHIFT           0
#define MKS_FLOW_TRACKER_FT_DATA_1_MIRROR_IDX_A_WIDTH           24
#define MKS_FLOW_TRACKER_FT_DATA_1_MIRROR_IDX_A_MASK            (0xFFFFFFU << MKS_FLOW_TRACKER_FT_DATA_1_MIRROR_IDX_A_SHIFT)

/* Bits 15:8 of 'mirror_index_b'. */
#define MKS_FLOW_TRACKER_FT_DATA_1_MIRROR_IDX_B15_8_SHIFT       24
#define MKS_FLOW_TRACKER_FT_DATA_1_MIRROR_IDX_B15_8_WIDTH       8
#define MKS_FLOW_TRACKER_FT_DATA_1_MIRROR_IDX_B15_8_MASK        (0xFFU << MKS_FLOW_TRACKER_FT_DATA_1_MIRROR_IDX_B15_8_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Register used to specify or read the data for word32 1 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
typedef union {
  struct {
    uint32_t mirror_idx_a : 24;         /* 'mirror_index_a'. */
    uint32_t mirror_idx_b15_8 : 8;      /* Bits 15:8 of 'mirror_index_b'. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_DATA_1_t;
#endif /* GENHDR_STRUCT */

/*
 * Register used to specify or read the data for word32 2 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
#define MKS_FLOW_TRACKER_FT_DATA_2                              0x00000408U
/* 32 least significant bits of the byte count. */
#define MKS_FLOW_TRACKER_FT_DATA_2_BYTE_COUNT_LSB_SHIFT         0
#define MKS_FLOW_TRACKER_FT_DATA_2_BYTE_COUNT_LSB_WIDTH         32
#define MKS_FLOW_TRACKER_FT_DATA_2_BYTE_COUNT_LSB_MASK          (0xFFFFFFFFU << MKS_FLOW_TRACKER_FT_DATA_2_BYTE_COUNT_LSB_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Register used to specify or read the data for word32 2 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
typedef union {
  struct {
    uint32_t byte_count_lsb : 32;       /* 32 least significant bits of the byte count. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_DATA_2_t;
#endif /* GENHDR_STRUCT */

/*
 * Register used to specify or read the data for word32 3 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
#define MKS_FLOW_TRACKER_FT_DATA_3                              0x0000040CU
/* 32 most significant bits of the byte count. */
#define MKS_FLOW_TRACKER_FT_DATA_3_BYTE_COUNT_MSB_SHIFT         0
#define MKS_FLOW_TRACKER_FT_DATA_3_BYTE_COUNT_MSB_WIDTH         32
#define MKS_FLOW_TRACKER_FT_DATA_3_BYTE_COUNT_MSB_MASK          (0xFFFFFFFFU << MKS_FLOW_TRACKER_FT_DATA_3_BYTE_COUNT_MSB_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Register used to specify or read the data for word32 3 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
typedef union {
  struct {
    uint32_t byte_count_msb : 32;       /* 32 most significant bits of the byte count. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_DATA_3_t;
#endif /* GENHDR_STRUCT */

/*
 * Register used to specify or read the data for word32 4 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
#define MKS_FLOW_TRACKER_FT_DATA_4                              0x00000410U
/* 32 least significant bits of the packet count. */
#define MKS_FLOW_TRACKER_FT_DATA_4_PKT_COUNT_LSB_SHIFT          0
#define MKS_FLOW_TRACKER_FT_DATA_4_PKT_COUNT_LSB_WIDTH          32
#define MKS_FLOW_TRACKER_FT_DATA_4_PKT_COUNT_LSB_MASK           (0xFFFFFFFFU << MKS_FLOW_TRACKER_FT_DATA_4_PKT_COUNT_LSB_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Register used to specify or read the data for word32 4 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
typedef union {
  struct {
    uint32_t pkt_count_lsb : 32;        /* 32 least significant bits of the packet count. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_DATA_4_t;
#endif /* GENHDR_STRUCT */

/*
 * Register used to specify or read the data for word32 5 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
#define MKS_FLOW_TRACKER_FT_DATA_5                              0x00000414U
/* 24 most significant bits of the packet count. */
#define MKS_FLOW_TRACKER_FT_DATA_5_PKT_COUNT_MSB_SHIFT          0
#define MKS_FLOW_TRACKER_FT_DATA_5_PKT_COUNT_MSB_WIDTH          24
#define MKS_FLOW_TRACKER_FT_DATA_5_PKT_COUNT_MSB_MASK           (0xFFFFFFU << MKS_FLOW_TRACKER_FT_DATA_5_PKT_COUNT_MSB_SHIFT)

/* Indicates the type of mirror flow. */
#define MKS_FLOW_TRACKER_FT_DATA_5_MIRROR_TYPE_SHIFT            24
#define MKS_FLOW_TRACKER_FT_DATA_5_MIRROR_TYPE_WIDTH            1
#define MKS_FLOW_TRACKER_FT_DATA_5_MIRROR_TYPE_MASK             (0x1U << MKS_FLOW_TRACKER_FT_DATA_5_MIRROR_TYPE_SHIFT)

/* Indicates the tracking mode. */
#define MKS_FLOW_TRACKER_FT_DATA_5_TRACK_MODE_SHIFT             25
#define MKS_FLOW_TRACKER_FT_DATA_5_TRACK_MODE_WIDTH             1
#define MKS_FLOW_TRACKER_FT_DATA_5_TRACK_MODE_MASK              (0x1U << MKS_FLOW_TRACKER_FT_DATA_5_TRACK_MODE_SHIFT)

/*
 * Indicates same flow before epoch was mirror or not.
 *  NOTE:Currently this bit is reserved in RTL.
 */
#define MKS_FLOW_TRACKER_FT_DATA_5_MIRROR_EPOCH_BIT_SHIFT       26
#define MKS_FLOW_TRACKER_FT_DATA_5_MIRROR_EPOCH_BIT_WIDTH       1
#define MKS_FLOW_TRACKER_FT_DATA_5_MIRROR_EPOCH_BIT_MASK        (0x1U << MKS_FLOW_TRACKER_FT_DATA_5_MIRROR_EPOCH_BIT_SHIFT)

/* Parity bits generated and checked by the logic. */
#define MKS_FLOW_TRACKER_FT_DATA_5_PARITY_ACCESSTYPE            GENHDR_ACCESSTYPE_RO
#define MKS_FLOW_TRACKER_FT_DATA_5_PARITY_SHIFT                 27
#define MKS_FLOW_TRACKER_FT_DATA_5_PARITY_WIDTH                 4
#define MKS_FLOW_TRACKER_FT_DATA_5_PARITY_MASK                  (0xFU << MKS_FLOW_TRACKER_FT_DATA_5_PARITY_SHIFT)

/*
 * Bit which if set to 0x1 indicates that the FT Structure has been updated by
 * HW since the last SW 'Read_Clear' operation.
 */
#define MKS_FLOW_TRACKER_FT_DATA_5_DIRTY_SHIFT                  31
#define MKS_FLOW_TRACKER_FT_DATA_5_DIRTY_WIDTH                  1
#define MKS_FLOW_TRACKER_FT_DATA_5_DIRTY_MASK                   (0x1U << MKS_FLOW_TRACKER_FT_DATA_5_DIRTY_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Register used to specify or read the data for word32 5 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
typedef union {
  struct {
    uint32_t pkt_count_msb : 24;        /* 24 most significant bits of the packet count. */
    uint32_t mirror_type : 1;           /* Indicates the type of mirror flow. */
    uint32_t track_mode : 1;            /* Indicates the tracking mode. */
    uint32_t mirror_epoch_bit : 1;      /* Indicates same flow before epoch was mirror or not.
                                           NOTE:Currently this bit is reserved in RTL. */
    uint32_t parity : 4;                /* Parity bits generated and checked by the logic. */
    uint32_t dirty : 1;                 /* Bit which if set to 0x1 indicates that the FT Structure has been updated by
                                           HW since the last SW 'Read_Clear' operation. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_DATA_5_t;
#endif /* GENHDR_STRUCT */

/*
 * Register used to specify or read the data for word32 6 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
#define MKS_FLOW_TRACKER_FT_DATA_6                              0x00000418U
/* inv_index_timeout'. */
#define MKS_FLOW_TRACKER_FT_DATA_6_INV_INDEX_TIMEOUT_SHIFT      0
#define MKS_FLOW_TRACKER_FT_DATA_6_INV_INDEX_TIMEOUT_WIDTH      24
#define MKS_FLOW_TRACKER_FT_DATA_6_INV_INDEX_TIMEOUT_MASK       (0xFFFFFFU << MKS_FLOW_TRACKER_FT_DATA_6_INV_INDEX_TIMEOUT_SHIFT)

/* Bits 23:16 of 'mirror_index_b'. */
#define MKS_FLOW_TRACKER_FT_DATA_6_MIRROR_IDX_B23_16_SHIFT      24
#define MKS_FLOW_TRACKER_FT_DATA_6_MIRROR_IDX_B23_16_WIDTH      8
#define MKS_FLOW_TRACKER_FT_DATA_6_MIRROR_IDX_B23_16_MASK       (0xFFU << MKS_FLOW_TRACKER_FT_DATA_6_MIRROR_IDX_B23_16_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Register used to specify or read the data for word32 6 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
typedef union {
  struct {
    uint32_t inv_index_timeout : 24;    /* inv_index_timeout'. */
    uint32_t mirror_idx_b23_16 : 8;     /* Bits 23:16 of 'mirror_index_b'. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_DATA_6_t;
#endif /* GENHDR_STRUCT */

/*
 * Register used to specify or read the data for word32 7 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
#define MKS_FLOW_TRACKER_FT_DATA_7                      0x0000041CU
/* Timestamp which increments every 100ms. */
#define MKS_FLOW_TRACKER_FT_DATA_7_TIMESTAMP_SHIFT      0
#define MKS_FLOW_TRACKER_FT_DATA_7_TIMESTAMP_WIDTH      24
#define MKS_FLOW_TRACKER_FT_DATA_7_TIMESTAMP_MASK       (0xFFFFFFU << MKS_FLOW_TRACKER_FT_DATA_7_TIMESTAMP_SHIFT)

/* Current State. */
#define MKS_FLOW_TRACKER_FT_DATA_7_STATE_SHIFT          24
#define MKS_FLOW_TRACKER_FT_DATA_7_STATE_WIDTH          5
#define MKS_FLOW_TRACKER_FT_DATA_7_STATE_MASK           (0x1FU << MKS_FLOW_TRACKER_FT_DATA_7_STATE_SHIFT)

/* Connection type. */
#define MKS_FLOW_TRACKER_FT_DATA_7_CONN_TYPE_SHIFT      29
#define MKS_FLOW_TRACKER_FT_DATA_7_CONN_TYPE_WIDTH      2
#define MKS_FLOW_TRACKER_FT_DATA_7_CONN_TYPE_MASK       (0x3U << MKS_FLOW_TRACKER_FT_DATA_7_CONN_TYPE_SHIFT)

/* Bit which if set to 0x1 enables the FT structure for use. */
#define MKS_FLOW_TRACKER_FT_DATA_7_VALID_SHIFT          31
#define MKS_FLOW_TRACKER_FT_DATA_7_VALID_WIDTH          1
#define MKS_FLOW_TRACKER_FT_DATA_7_VALID_MASK           (0x1U << MKS_FLOW_TRACKER_FT_DATA_7_VALID_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Register used to specify or read the data for word32 7 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
typedef union {
  struct {
    uint32_t timestamp : 24;    /* Timestamp which increments every 100ms. */
    uint32_t state : 5;         /* Current State. */
    uint32_t conn_type : 2;     /* Connection type. */
    uint32_t valid : 1;         /* Bit which if set to 0x1 enables the FT structure for use. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_DATA_7_t;
#endif /* GENHDR_STRUCT */

/*
 * Register used to specify or read the data for word32 8 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
#define MKS_FLOW_TRACKER_FT_DATA_8                              0x00000420U
/* Data for FT structure word32 8. */
#define MKS_FLOW_TRACKER_FT_DATA_8_WORD_32_DATA_SHIFT           0
#define MKS_FLOW_TRACKER_FT_DATA_8_WORD_32_DATA_WIDTH           32
#define MKS_FLOW_TRACKER_FT_DATA_8_WORD_32_DATA_MASK            (0xFFFFFFFFU << MKS_FLOW_TRACKER_FT_DATA_8_WORD_32_DATA_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Register used to specify or read the data for word32 8 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
typedef union {
  struct {
    uint32_t word_32_data : 32; /* Data for FT structure word32 8. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_DATA_8_t;
#endif /* GENHDR_STRUCT */

/*
 * Register used to specify or read the data for word32 9 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
#define MKS_FLOW_TRACKER_FT_DATA_9                              0x00000424U
/* Data for FT structure word32 9. */
#define MKS_FLOW_TRACKER_FT_DATA_9_WORD_32_DATA_SHIFT           0
#define MKS_FLOW_TRACKER_FT_DATA_9_WORD_32_DATA_WIDTH           32
#define MKS_FLOW_TRACKER_FT_DATA_9_WORD_32_DATA_MASK            (0xFFFFFFFFU << MKS_FLOW_TRACKER_FT_DATA_9_WORD_32_DATA_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Register used to specify or read the data for word32 9 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
typedef union {
  struct {
    uint32_t word_32_data : 32; /* Data for FT structure word32 9. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_DATA_9_t;
#endif /* GENHDR_STRUCT */

/*
 * Register used to specify or read the data for word32 10 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
#define MKS_FLOW_TRACKER_FT_DATA_10                             0x00000428U
/* Data for FT structure word32 10. */
#define MKS_FLOW_TRACKER_FT_DATA_10_WORD_32_DATA_SHIFT          0
#define MKS_FLOW_TRACKER_FT_DATA_10_WORD_32_DATA_WIDTH          32
#define MKS_FLOW_TRACKER_FT_DATA_10_WORD_32_DATA_MASK           (0xFFFFFFFFU << MKS_FLOW_TRACKER_FT_DATA_10_WORD_32_DATA_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Register used to specify or read the data for word32 10 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
typedef union {
  struct {
    uint32_t word_32_data : 32; /* Data for FT structure word32 10. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_DATA_10_t;
#endif /* GENHDR_STRUCT */

/*
 * Register used to specify or read the data for word32 11 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
#define MKS_FLOW_TRACKER_FT_DATA_11                             0x0000042CU
/* Data for FT structure word32 11. */
#define MKS_FLOW_TRACKER_FT_DATA_11_WORD_32_DATA_SHIFT          0
#define MKS_FLOW_TRACKER_FT_DATA_11_WORD_32_DATA_WIDTH          32
#define MKS_FLOW_TRACKER_FT_DATA_11_WORD_32_DATA_MASK           (0xFFFFFFFFU << MKS_FLOW_TRACKER_FT_DATA_11_WORD_32_DATA_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Register used to specify or read the data for word32 11 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
typedef union {
  struct {
    uint32_t word_32_data : 32; /* Data for FT structure word32 11. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_DATA_11_t;
#endif /* GENHDR_STRUCT */

/*
 * Register used to specify or read the data for word32 12 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
#define MKS_FLOW_TRACKER_FT_DATA_12                             0x00000430U
/* Data for FT structure word32 12. */
#define MKS_FLOW_TRACKER_FT_DATA_12_WORD_32_DATA_SHIFT          0
#define MKS_FLOW_TRACKER_FT_DATA_12_WORD_32_DATA_WIDTH          32
#define MKS_FLOW_TRACKER_FT_DATA_12_WORD_32_DATA_MASK           (0xFFFFFFFFU << MKS_FLOW_TRACKER_FT_DATA_12_WORD_32_DATA_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Register used to specify or read the data for word32 12 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
typedef union {
  struct {
    uint32_t word_32_data : 32; /* Data for FT structure word32 12. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_DATA_12_t;
#endif /* GENHDR_STRUCT */

/*
 * Register used to specify or read the data for word32 13 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
#define MKS_FLOW_TRACKER_FT_DATA_13                             0x00000434U
/* Data for FT structure word32 13. */
#define MKS_FLOW_TRACKER_FT_DATA_13_WORD_32_DATA_SHIFT          0
#define MKS_FLOW_TRACKER_FT_DATA_13_WORD_32_DATA_WIDTH          32
#define MKS_FLOW_TRACKER_FT_DATA_13_WORD_32_DATA_MASK           (0xFFFFFFFFU << MKS_FLOW_TRACKER_FT_DATA_13_WORD_32_DATA_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Register used to specify or read the data for word32 13 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
typedef union {
  struct {
    uint32_t word_32_data : 32; /* Data for FT structure word32 13. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_DATA_13_t;
#endif /* GENHDR_STRUCT */

/*
 * Register used to specify or read the data for word32 14 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
#define MKS_FLOW_TRACKER_FT_DATA_14                             0x00000438U
/* Data for FT structure word32 14. */
#define MKS_FLOW_TRACKER_FT_DATA_14_WORD_32_DATA_SHIFT          0
#define MKS_FLOW_TRACKER_FT_DATA_14_WORD_32_DATA_WIDTH          32
#define MKS_FLOW_TRACKER_FT_DATA_14_WORD_32_DATA_MASK           (0xFFFFFFFFU << MKS_FLOW_TRACKER_FT_DATA_14_WORD_32_DATA_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Register used to specify or read the data for word32 14 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
typedef union {
  struct {
    uint32_t word_32_data : 32; /* Data for FT structure word32 14. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_DATA_14_t;
#endif /* GENHDR_STRUCT */

/*
 * Register used to specify or read the data for word32 15 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
#define MKS_FLOW_TRACKER_FT_DATA_15                             0x0000043CU
/* Data for FT structure word32 15. */
#define MKS_FLOW_TRACKER_FT_DATA_15_WORD_32_DATA_SHIFT          0
#define MKS_FLOW_TRACKER_FT_DATA_15_WORD_32_DATA_WIDTH          32
#define MKS_FLOW_TRACKER_FT_DATA_15_WORD_32_DATA_MASK           (0xFFFFFFFFU << MKS_FLOW_TRACKER_FT_DATA_15_WORD_32_DATA_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Register used to specify or read the data for word32 15 of the Flow Tracker
 * structure, in conjunction with the Command register
 */
typedef union {
  struct {
    uint32_t word_32_data : 32; /* Data for FT structure word32 15. */
  } field;
  uint32_t val;
} MKS_FLOW_TRACKER_FT_DATA_15_t;
#endif /* GENHDR_STRUCT */

#endif /* _MKS_FLOW_TRACKER_REGISTERS_H_ */

