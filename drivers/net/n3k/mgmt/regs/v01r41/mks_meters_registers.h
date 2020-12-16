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
 * Date: 2020-12-02 10:37
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

#ifndef _MKS_METERS_REGISTERS_H_
#define _MKS_METERS_REGISTERS_H_

/* Meters */

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

#define MKS_METERS_ACCESSTYPE           GENHDR_ACCESSTYPE_RW /*  Default access type. Access types defined above. */
#define MKS_METERS_REGWIDTH             32 /* Default width of register in bits */
#define MKS_METERS_ACCESSWIDTH          32 /* Default width of access word in bit */


/* Version Number of This Module */
#define MKS_METERS_MET_VERSION                          0x00000000U
/* Version format: DDMMYY_XX. */
#define MKS_METERS_MET_VERSION_VERSION_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_METERS_MET_VERSION_VERSION_SHIFT            0
#define MKS_METERS_MET_VERSION_VERSION_WIDTH            32
#define MKS_METERS_MET_VERSION_VERSION_MASK             (0xFFFFFFFFU << MKS_METERS_MET_VERSION_VERSION_SHIFT)

#ifdef GENHDR_STRUCT
/* Version Number of This Module */
typedef union {
  struct {
    uint32_t version : 32;      /* Version format: DDMMYY_XX. */
  } field;
  uint32_t val;
} MKS_METERS_MET_VERSION_t;
#endif /* GENHDR_STRUCT */

/* A General Purpose R/W Register Intended for Debug Use */
#define MKS_METERS_MET_SCRATCH                          0x00000004U
/* Test data. */
#define MKS_METERS_MET_SCRATCH_SCRATCH_SHIFT            0
#define MKS_METERS_MET_SCRATCH_SCRATCH_WIDTH            32
#define MKS_METERS_MET_SCRATCH_SCRATCH_MASK             (0xFFFFFFFFU << MKS_METERS_MET_SCRATCH_SCRATCH_SHIFT)

#ifdef GENHDR_STRUCT
/* A General Purpose R/W Register Intended for Debug Use */
typedef union {
  struct {
    uint32_t scratch : 32;      /* Test data. */
  } field;
  uint32_t val;
} MKS_METERS_MET_SCRATCH_t;
#endif /* GENHDR_STRUCT */

/* Provides Initialisation and Alarm Status Bits */
#define MKS_METERS_MET_STAT                             0x00000008U
/*
 * 0x1 = the block has initialised and is ready for configuration and requests.
 *  0x0 = the block has yet to complete its initialisation; do not perform any
 * configuration or send in any requests (these will be discarded).
 *  Reset to 0.
 */
#define MKS_METERS_MET_STAT_INIT_DONE_ACCESSTYPE        GENHDR_ACCESSTYPE_RO
#define MKS_METERS_MET_STAT_INIT_DONE_SHIFT             0
#define MKS_METERS_MET_STAT_INIT_DONE_WIDTH             1
#define MKS_METERS_MET_STAT_INIT_DONE_MASK              (0x1U << MKS_METERS_MET_STAT_INIT_DONE_SHIFT)

/*
 * 0x1 = an unexpected but potentially recoverable condition has been detected
 * in the block. Access the Warning_0 register to determine the cause and clear
 * the alarm. Reset to 0.
 */
#define MKS_METERS_MET_STAT_WARNING_ACCESSTYPE          GENHDR_ACCESSTYPE_RO
#define MKS_METERS_MET_STAT_WARNING_SHIFT               30
#define MKS_METERS_MET_STAT_WARNING_WIDTH               1
#define MKS_METERS_MET_STAT_WARNING_MASK                (0x1U << MKS_METERS_MET_STAT_WARNING_SHIFT)

/*
 * 0x1 = an unexpected and fatal condition has been detected in the block.
 * Access the Fatal_0 register to determine the cause. Reset to 0.
 */
#define MKS_METERS_MET_STAT_FATAL_ERROR_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define MKS_METERS_MET_STAT_FATAL_ERROR_SHIFT           31
#define MKS_METERS_MET_STAT_FATAL_ERROR_WIDTH           1
#define MKS_METERS_MET_STAT_FATAL_ERROR_MASK            (0x1U << MKS_METERS_MET_STAT_FATAL_ERROR_SHIFT)

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
} MKS_METERS_MET_STAT_t;
#endif /* GENHDR_STRUCT */

/* Indicates the Cause of a Warning Alarm */
#define MKS_METERS_MET_WARNING_0                                0x00000020U
/*
 * 1 = the CPU has been prevented from writing or accessing the configuration
 * for an out-of-range meter index.
 *  Write a 1 to clear this bit.
 *  Reset to 0.
 */
#define MKS_METERS_MET_WARNING_0_WARN_CPU_CFG_IDX_ACCESSTYPE    GENHDR_ACCESSTYPE_W1C
#define MKS_METERS_MET_WARNING_0_WARN_CPU_CFG_IDX_SHIFT         0
#define MKS_METERS_MET_WARNING_0_WARN_CPU_CFG_IDX_WIDTH         1
#define MKS_METERS_MET_WARNING_0_WARN_CPU_CFG_IDX_MASK          (0x1U << MKS_METERS_MET_WARNING_0_WARN_CPU_CFG_IDX_SHIFT)

/*
 * 1 = the CPU has been prevented from accessing an out-of-range statistics
 * counter.
 *  Write a 1 to clear this bit.
 *  Reset to 0.
 */
#define MKS_METERS_MET_WARNING_0_WARN_CPU_STATS_IDX_ACCESSTYPE  GENHDR_ACCESSTYPE_W1C
#define MKS_METERS_MET_WARNING_0_WARN_CPU_STATS_IDX_SHIFT       1
#define MKS_METERS_MET_WARNING_0_WARN_CPU_STATS_IDX_WIDTH       1
#define MKS_METERS_MET_WARNING_0_WARN_CPU_STATS_IDX_MASK        (0x1U << MKS_METERS_MET_WARNING_0_WARN_CPU_STATS_IDX_SHIFT)

/*
 * 1 = requests have been discarded because the processing cannot keep pace.
 *  Write a 1 to clear this bit.
 *  Reset to 0.
 */
#define MKS_METERS_MET_WARNING_0_WARN_REQ_DISCARD_ACCESSTYPE    GENHDR_ACCESSTYPE_W1C
#define MKS_METERS_MET_WARNING_0_WARN_REQ_DISCARD_SHIFT         4
#define MKS_METERS_MET_WARNING_0_WARN_REQ_DISCARD_WIDTH         1
#define MKS_METERS_MET_WARNING_0_WARN_REQ_DISCARD_MASK          (0x1U << MKS_METERS_MET_WARNING_0_WARN_REQ_DISCARD_SHIFT)

#ifdef GENHDR_STRUCT
/* Indicates the Cause of a Warning Alarm */
typedef union {
  struct {
    uint32_t warn_cpu_cfg_idx : 1;      /* 1 = the CPU has been prevented from writing or accessing the configuration
                                           for an out-of-range meter index.
                                           Write a 1 to clear this bit.
                                           Reset to 0. */
    uint32_t warn_cpu_stats_idx : 1;    /* 1 = the CPU has been prevented from accessing an out-of-range statistics
                                           counter.
                                           Write a 1 to clear this bit.
                                           Reset to 0. */
    uint32_t : 2;
    uint32_t warn_req_discard : 1;      /* 1 = requests have been discarded because the processing cannot keep pace.
                                           Write a 1 to clear this bit.
                                           Reset to 0. */
    uint32_t : 27;
  } field;
  uint32_t val;
} MKS_METERS_MET_WARNING_0_t;
#endif /* GENHDR_STRUCT */

/* Provides Various Monitoring Flags for the Logic */
#define MKS_METERS_MET_MON_0                    0x00000028U
/* Provides Various Monitoring Flags for the Logic. */
#define MKS_METERS_MET_MON_0_MONITOR_SHIFT      0
#define MKS_METERS_MET_MON_0_MONITOR_WIDTH      32
#define MKS_METERS_MET_MON_0_MONITOR_MASK       (0xFFFFFFFFU << MKS_METERS_MET_MON_0_MONITOR_SHIFT)

#ifdef GENHDR_STRUCT
/* Provides Various Monitoring Flags for the Logic */
typedef union {
  struct {
    uint32_t monitor : 32;      /* Provides Various Monitoring Flags for the Logic. */
  } field;
  uint32_t val;
} MKS_METERS_MET_MON_0_t;
#endif /* GENHDR_STRUCT */

/* Provides Various Fatal Error Flags for the Logic */
#define MKS_METERS_MET_FATAL_0                          0x00000030U
/* Provides various fatal error flags for the logic. Reset to 0. */
#define MKS_METERS_MET_FATAL_0_FATAL_ERROR_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define MKS_METERS_MET_FATAL_0_FATAL_ERROR_SHIFT        0
#define MKS_METERS_MET_FATAL_0_FATAL_ERROR_WIDTH        32
#define MKS_METERS_MET_FATAL_0_FATAL_ERROR_MASK         (0xFFFFFFFFU << MKS_METERS_MET_FATAL_0_FATAL_ERROR_SHIFT)

#ifdef GENHDR_STRUCT
/* Provides Various Fatal Error Flags for the Logic */
typedef union {
  struct {
    uint32_t fatal_error : 32;  /* Provides various fatal error flags for the logic. Reset to 0. */
  } field;
  uint32_t val;
} MKS_METERS_MET_FATAL_0_t;
#endif /* GENHDR_STRUCT */

/*
 * This register is used in conjunction with the Config_Data_1 and Config_Data_2
 * registers to set or read back the configuration for a meter
 */
#define MKS_METERS_MET_CONFIG_CTRL                              0x00000080U
/* Specifies the meter index to be accessed (some MSBs may be unused). */
#define MKS_METERS_MET_CONFIG_CTRL_METER_INDEX_SHIFT            0
#define MKS_METERS_MET_CONFIG_CTRL_METER_INDEX_WIDTH            24
#define MKS_METERS_MET_CONFIG_CTRL_METER_INDEX_MASK             (0xFFFFFFU << MKS_METERS_MET_CONFIG_CTRL_METER_INDEX_SHIFT)

/*
 * Specifies the bank to be accessed.
 *  0x0 = bank A.
 *  0x1 = bank B.
 */
#define MKS_METERS_MET_CONFIG_CTRL_METER_BANK_SHIFT             24
#define MKS_METERS_MET_CONFIG_CTRL_METER_BANK_WIDTH             1
#define MKS_METERS_MET_CONFIG_CTRL_METER_BANK_MASK              (0x1U << MKS_METERS_MET_CONFIG_CTRL_METER_BANK_SHIFT)

/*
 * Specifies the operation type.
 *  0x0 = Read.
 *  0x1 = Write.
 */
#define MKS_METERS_MET_CONFIG_CTRL_OPERATION_SHIFT              28
#define MKS_METERS_MET_CONFIG_CTRL_OPERATION_WIDTH              1
#define MKS_METERS_MET_CONFIG_CTRL_OPERATION_MASK               (0x1U << MKS_METERS_MET_CONFIG_CTRL_OPERATION_SHIFT)

/*
 * This bit is automatically set when the register is written, and is
 * automatically cleared when the logic has completed the request.
 *  0x1 = a request is ongoing and this register must not be written to.
 *  0x0 = any previous request has completed (and the data for a read request
 * can be read from the Data_N registers) and another request may be made using
 * a write to this register.
 *  Reset to 0.
 */
#define MKS_METERS_MET_CONFIG_CTRL_BUSY_ACCESSTYPE              GENHDR_ACCESSTYPE_RO
#define MKS_METERS_MET_CONFIG_CTRL_BUSY_SHIFT                   31
#define MKS_METERS_MET_CONFIG_CTRL_BUSY_WIDTH                   1
#define MKS_METERS_MET_CONFIG_CTRL_BUSY_MASK                    (0x1U << MKS_METERS_MET_CONFIG_CTRL_BUSY_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * This register is used in conjunction with the Config_Data_1 and Config_Data_2
 * registers to set or read back the configuration for a meter
 */
typedef union {
  struct {
    uint32_t meter_index : 24;  /* Specifies the meter index to be accessed (some MSBs may be unused). */
    uint32_t meter_bank : 1;    /* Specifies the bank to be accessed.
                                   0x0 = bank A.
                                   0x1 = bank B. */
    uint32_t : 3;
    uint32_t operation : 1;     /* Specifies the operation type.
                                   0x0 = Read.
                                   0x1 = Write. */
    uint32_t : 2;
    uint32_t busy : 1;          /* This bit is automatically set when the register is written, and is
                                   automatically cleared when the logic has completed the request.
                                   0x1 = a request is ongoing and this register must not be written to.
                                   0x0 = any previous request has completed (and the data for a read request
                                   can be read from the Data_N registers) and another request may be made using
                                   a write to this register.
                                   Reset to 0. */
  } field;
  uint32_t val;
} MKS_METERS_MET_CONFIG_CTRL_t;
#endif /* GENHDR_STRUCT */

/*
 * Register used to specify or read the configuration for a meter, in
 * conjunction with the Config_Ctrl register
 */
#define MKS_METERS_MET_CONFIG_DATA_1                            0x00000084U
/*
 * Specifies the rate using a granularity of 100,000,000bps when in bits per
 * second mode, or a granularity of 100,000pps when in packets per second mode
 * (some MSBs may be unused).
 */
#define MKS_METERS_MET_CONFIG_DATA_1_METER_RATE_SHIFT           0
#define MKS_METERS_MET_CONFIG_DATA_1_METER_RATE_WIDTH           28
#define MKS_METERS_MET_CONFIG_DATA_1_METER_RATE_MASK            (0xFFFFFFFU << MKS_METERS_MET_CONFIG_DATA_1_METER_RATE_SHIFT)

/*
 * Specifies whether the meter is enabled.
 *  0x0 = the meter is disabled (and so cannot contribute to a 'drop' decision).
 *  0x1 = the meter is enabled.
 */
#define MKS_METERS_MET_CONFIG_DATA_1_METER_ENABLE_SHIFT         28
#define MKS_METERS_MET_CONFIG_DATA_1_METER_ENABLE_WIDTH         1
#define MKS_METERS_MET_CONFIG_DATA_1_METER_ENABLE_MASK          (0x1U << MKS_METERS_MET_CONFIG_DATA_1_METER_ENABLE_SHIFT)

/*
 * Specifies the operation mode.
 *  0x0 = the meter operates in 'bits per second' mode.
 *  0x1 = the meter operates in 'packets per second' mode.
 */
#define MKS_METERS_MET_CONFIG_DATA_1_METER_MODE_SHIFT           29
#define MKS_METERS_MET_CONFIG_DATA_1_METER_MODE_WIDTH           1
#define MKS_METERS_MET_CONFIG_DATA_1_METER_MODE_MASK            (0x1U << MKS_METERS_MET_CONFIG_DATA_1_METER_MODE_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Register used to specify or read the configuration for a meter, in
 * conjunction with the Config_Ctrl register
 */
typedef union {
  struct {
    uint32_t meter_rate : 28;   /* Specifies the rate using a granularity of 100,000,000bps when in bits per
                                   second mode, or a granularity of 100,000pps when in packets per second mode
                                   (some MSBs may be unused). */
    uint32_t meter_enable : 1;  /* Specifies whether the meter is enabled.
                                   0x0 = the meter is disabled (and so cannot contribute to a 'drop' decision).
                                   0x1 = the meter is enabled. */
    uint32_t meter_mode : 1;    /* Specifies the operation mode.
                                   0x0 = the meter operates in 'bits per second' mode.
                                   0x1 = the meter operates in 'packets per second' mode. */
    uint32_t : 2;
  } field;
  uint32_t val;
} MKS_METERS_MET_CONFIG_DATA_1_t;
#endif /* GENHDR_STRUCT */

/*
 * Register used to specify or read the configuration for a meter, in
 * conjunction with the Config_Ctrl register
 */
#define MKS_METERS_MET_CONFIG_DATA_2                            0x00000088U
/*
 * Specifies the bucket depth using a granularity of 1000 bytes when in bits per
 * second mode), or a granularity of 8 packets when in packets per second mode
 * (some MSBs may be unused).
 */
#define MKS_METERS_MET_CONFIG_DATA_2_METER_BKT_DEPTH_SHIFT      0
#define MKS_METERS_MET_CONFIG_DATA_2_METER_BKT_DEPTH_WIDTH      28
#define MKS_METERS_MET_CONFIG_DATA_2_METER_BKT_DEPTH_MASK       (0xFFFFFFFU << MKS_METERS_MET_CONFIG_DATA_2_METER_BKT_DEPTH_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Register used to specify or read the configuration for a meter, in
 * conjunction with the Config_Ctrl register
 */
typedef union {
  struct {
    uint32_t meter_bkt_depth : 28;      /* Specifies the bucket depth using a granularity of 1000 bytes when in bits per
                                           second mode), or a granularity of 8 packets when in packets per second mode
                                           (some MSBs may be unused). */
    uint32_t : 4;
  } field;
  uint32_t val;
} MKS_METERS_MET_CONFIG_DATA_2_t;
#endif /* GENHDR_STRUCT */

/*
 * This register is used in conjunction with the Stats_Data_1 and Stats_Data_2
 * registers to read a 64bit statistics counter and to optionally clear it
 */
#define MKS_METERS_MET_STATS_CTRL                               0x00000100U
/*
 * Specifies the counter to be accessed (some MSBs may be unused).
 *  For the bank A and bank B counters, this specifies a meter index.
 *  For the global counters, 0x0 accesses counters for the number of requests
 * already dropped upstream; 0x1 accesses counters for the number of requests
 * actively marked as keep by the meters block; 0x2 accesses counters for the
 * number of requests marked as drop by the meters block.
 */
#define MKS_METERS_MET_STATS_CTRL_COUNTER_OFFSET_SHIFT          0
#define MKS_METERS_MET_STATS_CTRL_COUNTER_OFFSET_WIDTH          24
#define MKS_METERS_MET_STATS_CTRL_COUNTER_OFFSET_MASK           (0xFFFFFFU << MKS_METERS_MET_STATS_CTRL_COUNTER_OFFSET_SHIFT)

/*
 * Specifies the type of counter to be accessed.
 *  0x0 = bank A keep packets.
 *  0x1 = bank A keep bytes.
 *  0x2 = bank A drop packets.
 *  0x3 = bank A drop bytes.
 *  0x4 = bank B keep packets.
 *  0x5 = bank B keep bytes.
 *  0x6 = bank B drop packets.
 *  0x7 = bank B drop bytes.
 *  0x8 - 0xb = unused.
 *  0xc = global packet counters.
 *  0xc = global byte counters.
 */
#define MKS_METERS_MET_STATS_CTRL_COUNTER_TYPE_SHIFT            24
#define MKS_METERS_MET_STATS_CTRL_COUNTER_TYPE_WIDTH            4
#define MKS_METERS_MET_STATS_CTRL_COUNTER_TYPE_MASK             (0xFU << MKS_METERS_MET_STATS_CTRL_COUNTER_TYPE_SHIFT)

/*
 * Specifies whether the counter should be cleared after the read.
 *  0x0 = Don't clear the counter after the read.
 *  0x1 = Clear the counter after the read.
 */
#define MKS_METERS_MET_STATS_CTRL_CLEAR_SHIFT                   28
#define MKS_METERS_MET_STATS_CTRL_CLEAR_WIDTH                   1
#define MKS_METERS_MET_STATS_CTRL_CLEAR_MASK                    (0x1U << MKS_METERS_MET_STATS_CTRL_CLEAR_SHIFT)

/*
 * This bit is automatically set when the register is written, and is
 * automatically cleared when the logic has completed the request.
 *  0x1 = a request is ongoing and this register must not be written to.
 *  0x0 = any previous request has completed (and the data for a read request
 * can be read from the Data_N registers) and another request may be made using
 * a write to this register.
 *  Reset to 0.
 */
#define MKS_METERS_MET_STATS_CTRL_BUSY_ACCESSTYPE               GENHDR_ACCESSTYPE_RO
#define MKS_METERS_MET_STATS_CTRL_BUSY_SHIFT                    31
#define MKS_METERS_MET_STATS_CTRL_BUSY_WIDTH                    1
#define MKS_METERS_MET_STATS_CTRL_BUSY_MASK                     (0x1U << MKS_METERS_MET_STATS_CTRL_BUSY_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * This register is used in conjunction with the Stats_Data_1 and Stats_Data_2
 * registers to read a 64bit statistics counter and to optionally clear it
 */
typedef union {
  struct {
    uint32_t counter_offset : 24;       /* Specifies the counter to be accessed (some MSBs may be unused).
                                           For the bank A and bank B counters, this specifies a meter index.
                                           For the global counters, 0x0 accesses counters for the number of requests
                                           already dropped upstream; 0x1 accesses counters for the number of requests
                                           actively marked as keep by the meters block; 0x2 accesses counters for the
                                           number of requests marked as drop by the meters block. */
    uint32_t counter_type : 4;          /* Specifies the type of counter to be accessed.
                                           0x0 = bank A keep packets.
                                           0x1 = bank A keep bytes.
                                           0x2 = bank A drop packets.
                                           0x3 = bank A drop bytes.
                                           0x4 = bank B keep packets.
                                           0x5 = bank B keep bytes.
                                           0x6 = bank B drop packets.
                                           0x7 = bank B drop bytes.
                                           0x8 - 0xb = unused.
                                           0xc = global packet counters.
                                           0xc = global byte counters. */
    uint32_t clear : 1;                 /* Specifies whether the counter should be cleared after the read.
                                           0x0 = Don't clear the counter after the read.
                                           0x1 = Clear the counter after the read. */
    uint32_t : 2;
    uint32_t busy : 1;                  /* This bit is automatically set when the register is written, and is
                                           automatically cleared when the logic has completed the request.
                                           0x1 = a request is ongoing and this register must not be written to.
                                           0x0 = any previous request has completed (and the data for a read request
                                           can be read from the Data_N registers) and another request may be made using
                                           a write to this register.
                                           Reset to 0. */
  } field;
  uint32_t val;
} MKS_METERS_MET_STATS_CTRL_t;
#endif /* GENHDR_STRUCT */

/* Provides the 32 LSBs of the accessed count value */
#define MKS_METERS_MET_STATS_DATA_1                             0x00000104U
/* Provides the 32 LSBs of the accessed count value. */
#define MKS_METERS_MET_STATS_DATA_1_COUNT_LSBS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_METERS_MET_STATS_DATA_1_COUNT_LSBS_SHIFT            0
#define MKS_METERS_MET_STATS_DATA_1_COUNT_LSBS_WIDTH            32
#define MKS_METERS_MET_STATS_DATA_1_COUNT_LSBS_MASK             (0xFFFFFFFFU << MKS_METERS_MET_STATS_DATA_1_COUNT_LSBS_SHIFT)

#ifdef GENHDR_STRUCT
/* Provides the 32 LSBs of the accessed count value */
typedef union {
  struct {
    uint32_t count_lsbs : 32;   /* Provides the 32 LSBs of the accessed count value. */
  } field;
  uint32_t val;
} MKS_METERS_MET_STATS_DATA_1_t;
#endif /* GENHDR_STRUCT */

/* Provides the 32 MSBs of the accessed count value */
#define MKS_METERS_MET_STATS_DATA_2                             0x00000108U
/* Provides the 32 MSBs of the accessed count value. */
#define MKS_METERS_MET_STATS_DATA_2_COUNT_MSBS_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_METERS_MET_STATS_DATA_2_COUNT_MSBS_SHIFT            0
#define MKS_METERS_MET_STATS_DATA_2_COUNT_MSBS_WIDTH            32
#define MKS_METERS_MET_STATS_DATA_2_COUNT_MSBS_MASK             (0xFFFFFFFFU << MKS_METERS_MET_STATS_DATA_2_COUNT_MSBS_SHIFT)

#ifdef GENHDR_STRUCT
/* Provides the 32 MSBs of the accessed count value */
typedef union {
  struct {
    uint32_t count_msbs : 32;   /* Provides the 32 MSBs of the accessed count value. */
  } field;
  uint32_t val;
} MKS_METERS_MET_STATS_DATA_2_t;
#endif /* GENHDR_STRUCT */

#endif /* _MKS_METERS_REGISTERS_H_ */

