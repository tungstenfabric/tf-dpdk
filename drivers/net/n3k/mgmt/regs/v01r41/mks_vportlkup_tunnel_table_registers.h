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

#ifndef _MKS_VPORTLKUP_TUNNEL_TABLE_REGISTERS_H_
#define _MKS_VPORTLKUP_TUNNEL_TABLE_REGISTERS_H_

/*
 * Control Status Registers for Virtual port lookup(MHL IP),Tunnel table and
 * SMAC Address Table
 */

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

#define MKS_VPORTLKUP_TUNNEL_TABLE_ACCESSTYPE GENHDR_ACCESSTYPE_RW /*  Default access type. Access types defined above. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_REGWIDTH 32 /* Default width of register in bits */
#define MKS_VPORTLKUP_TUNNEL_TABLE_ACCESSWIDTH 32 /* Default width of access word in bit */


/* This gives the version of the Exact Match Lookup component RTL code */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_VERSION                    0x00000000U
/*
 * Version is in the following format:
 *  DDMMYY_C_X,where:
 *  DD is day
 *  MM is month
 *  YY is year
 *  X is sequential number
 *  C indicates core module type(0x0 = MHL).
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_VERSION_VERSION_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_VERSION_VERSION_SHIFT      0
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_VERSION_VERSION_WIDTH      32
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_VERSION_VERSION_MASK       (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_VERSION_VERSION_SHIFT)

#ifdef GENHDR_STRUCT
/* This gives the version of the Exact Match Lookup component RTL code */
typedef union {
  struct {
    uint32_t version : 32;      /* Version is in the following format:
                                   DDMMYY_C_X,where:
                                   DD is day
                                   MM is month
                                   YY is year
                                   X is sequential number
                                   C indicates core module type(0x0 = MHL). */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_VERSION_t;
#endif /* GENHDR_STRUCT */

/* This is a general purpose register intended for debug use */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_SCRATCH                    0x00000004U
/* Test data. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_SCRATCH_SCRATCH_SHIFT      0
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_SCRATCH_SCRATCH_WIDTH      32
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_SCRATCH_SCRATCH_MASK       (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_SCRATCH_SCRATCH_SHIFT)

#ifdef GENHDR_STRUCT
/* This is a general purpose register intended for debug use */
typedef union {
  struct {
    uint32_t scratch : 32;      /* Test data. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_SCRATCH_t;
#endif /* GENHDR_STRUCT */

/*
 * This register reflects the current configuration of the component - number of
 * entries
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_NUM_EM                     0x00000008U
/* Number of entries which can be inserted to the database. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_NUM_EM_NUM_ENTRIES_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_NUM_EM_NUM_ENTRIES_SHIFT   0
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_NUM_EM_NUM_ENTRIES_WIDTH   28
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_NUM_EM_NUM_ENTRIES_MASK    (0xFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_NUM_EM_NUM_ENTRIES_SHIFT)

/* Reserved bits. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_NUM_EM_RESERVED_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_NUM_EM_RESERVED_SHIFT      28
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_NUM_EM_RESERVED_WIDTH      4
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_NUM_EM_RESERVED_MASK       (0xFU << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_NUM_EM_RESERVED_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * This register reflects the current configuration of the component - number of
 * entries
 */
typedef union {
  struct {
    uint32_t num_entries : 28;  /* Number of entries which can be inserted to the database. */
    uint32_t reserved : 4;      /* Reserved bits. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_NUM_EM_t;
#endif /* GENHDR_STRUCT */

/* This register reflects the current configuration of the component - key width */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_KEY_WIDTH                  0x0000000CU
/* Configured key width in bits. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_KEY_WIDTH_KEY_WIDTH_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_KEY_WIDTH_KEY_WIDTH_SHIFT  0
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_KEY_WIDTH_KEY_WIDTH_WIDTH  32
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_KEY_WIDTH_KEY_WIDTH_MASK   (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_KEY_WIDTH_KEY_WIDTH_SHIFT)

#ifdef GENHDR_STRUCT
/* This register reflects the current configuration of the component - key width */
typedef union {
  struct {
    uint32_t key_width : 32;    /* Configured key width in bits. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_KEY_WIDTH_t;
#endif /* GENHDR_STRUCT */

/*
 * This register reflects the current configuration of the component - result
 * width
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_RES_WIDTH                  0x00000010U
/* Configured result width in bits. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_RES_WIDTH_RESULT_WIDTH_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_RES_WIDTH_RESULT_WIDTH_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_RES_WIDTH_RESULT_WIDTH_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_RES_WIDTH_RESULT_WIDTH_MASK (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_RES_WIDTH_RESULT_WIDTH_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * This register reflects the current configuration of the component - result
 * width
 */
typedef union {
  struct {
    uint32_t result_width : 32; /* Configured result width in bits. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_RES_WIDTH_t;
#endif /* GENHDR_STRUCT */

/* Alarms reported in the hardware */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_ALARMS                     0x00000014U
/*
 * Alarm indicated by the lookup core.
 *  Check VPLKP_EMLC_FAT_ERR_0 and VPLKP_EMLC_WARNING_0 registers for further
 * details.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_ALARMS_CORE_ALARM_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_ALARMS_CORE_ALARM_SHIFT    0
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_ALARMS_CORE_ALARM_WIDTH    1
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_ALARMS_CORE_ALARM_MASK     (0x1U << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_ALARMS_CORE_ALARM_SHIFT)

/* Reserved bits. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_ALARMS_RESERVED_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_ALARMS_RESERVED_SHIFT      1
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_ALARMS_RESERVED_WIDTH      31
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_ALARMS_RESERVED_MASK       (0x7FFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_ALARMS_RESERVED_SHIFT)

#ifdef GENHDR_STRUCT
/* Alarms reported in the hardware */
typedef union {
  struct {
    uint32_t core_alarm : 1;    /* Alarm indicated by the lookup core.
                                   Check VPLKP_EMLC_FAT_ERR_0 and VPLKP_EMLC_WARNING_0 registers for further
                                   details. */
    uint32_t reserved : 31;     /* Reserved bits. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EML_ALARMS_t;
#endif /* GENHDR_STRUCT */

/* This gives the version of the Exact Match Lookup core component RTL code */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_VERSION                   0x00000018U
/*
 * Version format: DDMMYY_XX,where:
 *  DD is day
 *  MM is month
 *  YY is year
 *  XX is sequential number.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_VERSION_VERSION_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_VERSION_VERSION_SHIFT     0
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_VERSION_VERSION_WIDTH     32
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_VERSION_VERSION_MASK      (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_VERSION_VERSION_SHIFT)

#ifdef GENHDR_STRUCT
/* This gives the version of the Exact Match Lookup core component RTL code */
typedef union {
  struct {
    uint32_t version : 32;      /* Version format: DDMMYY_XX,where:
                                   DD is day
                                   MM is month
                                   YY is year
                                   XX is sequential number. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_VERSION_t;
#endif /* GENHDR_STRUCT */

/* This is general purpose register intended for debug use */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_SCRATCH                   0x0000001CU
/* Test data. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_SCRATCH_SCRATCH_SHIFT     0
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_SCRATCH_SCRATCH_WIDTH     32
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_SCRATCH_SCRATCH_MASK      (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_SCRATCH_SCRATCH_SHIFT)

#ifdef GENHDR_STRUCT
/* This is general purpose register intended for debug use */
typedef union {
  struct {
    uint32_t scratch : 32;      /* Test data. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_SCRATCH_t;
#endif /* GENHDR_STRUCT */

/* Used for General control of the IP */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_GEN_CTRL                  0x00000020U
/* Reserved bits. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_GEN_CTRL_RESERVED_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_GEN_CTRL_RESERVED_SHIFT   0
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_GEN_CTRL_RESERVED_WIDTH   31
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_GEN_CTRL_RESERVED_MASK    (0x7FFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_GEN_CTRL_RESERVED_SHIFT)

/*
 * 0x0 = the IP is still initialising and is not ready for search requests or
 * CPU configuration.
 *  0x1 = the IP has initialised and is ready for CPU configuration.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_GEN_CTRL_INIT_DONE_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_GEN_CTRL_INIT_DONE_SHIFT  31
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_GEN_CTRL_INIT_DONE_WIDTH  1
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_GEN_CTRL_INIT_DONE_MASK   (0x1U << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_GEN_CTRL_INIT_DONE_SHIFT)

#ifdef GENHDR_STRUCT
/* Used for General control of the IP */
typedef union {
  struct {
    uint32_t reserved : 31;     /* Reserved bits. */
    uint32_t init_done : 1;     /* 0x0 = the IP is still initialising and is not ready for search requests or
                                   CPU configuration.
                                   0x1 = the IP has initialised and is ready for CPU configuration. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_GEN_CTRL_t;
#endif /* GENHDR_STRUCT */

/* Used to request management operations */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL                 0x00000024U
/*
 * Specifies the operation type:
 *  0x0 = flush(delete all entries)
 *  0x1 = insert an entry,using the key specified by the VPLKP_EMLC_KEY_0
 * register
 *  and the result specified by the VPLKP_EMLC_RES_0 registers.
 *  0x2 = delete an entry,using the key specified by the VPLKP_EMLC_KEY_0
 * register.
 *  0x3 = performs a management search,using the key specified in the
 * VPLKP_EMLC_KEY_0 register.
 *  On completion,the success field indicates whether a match was found.
 *  The result associated with the matching key can be read from
 * VPLKP_EMLC_RES_0 registers.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_OP_TYPE_SHIFT   0
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_OP_TYPE_WIDTH   3
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_OP_TYPE_MASK    (0x7U << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_OP_TYPE_SHIFT)

/* Reserved bits. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_RESERVED_0_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_RESERVED_0_SHIFT 3
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_RESERVED_0_WIDTH 5
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_RESERVED_0_MASK (0x1FU << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_RESERVED_0_SHIFT)

/*
 * On completion of a flush,insert or delete operation,this indicates that the
 * opeartion was successful.
 *  (Note that only 0x1 is expected).
 *  On completion of a management search operation,this indicates whether a
 * match was found.
 *  (0x1 = a match was found, 0x0 = no match found).
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_SUCCESS_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_SUCCESS_SHIFT   8
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_SUCCESS_WIDTH   1
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_SUCCESS_MASK    (0x1U << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_SUCCESS_SHIFT)

/* Reserved bits. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_RESERVED_1_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_RESERVED_1_SHIFT 9
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_RESERVED_1_WIDTH 22
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_RESERVED_1_MASK (0x3FFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_RESERVED_1_SHIFT)

/*
 * This bit is automatically set when the register is written to initiate an
 * operation
 *  and is automatically cleared when the operation has completed.
 *  0x1 = an operation is ongoing and this register should not be written to.
 *  0x0 = any previous operation has completed,and another operation may be
 * requested using a write to this register.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_BUSY_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_BUSY_SHIFT      31
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_BUSY_WIDTH      1
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_BUSY_MASK       (0x1U << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_BUSY_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to request management operations */
typedef union {
  struct {
    uint32_t op_type : 3;       /* Specifies the operation type:
                                   0x0 = flush(delete all entries)
                                   0x1 = insert an entry,using the key specified by the VPLKP_EMLC_KEY_0
                                   register
                                   and the result specified by the VPLKP_EMLC_RES_0 registers.
                                   0x2 = delete an entry,using the key specified by the VPLKP_EMLC_KEY_0
                                   register.
                                   0x3 = performs a management search,using the key specified in the
                                   VPLKP_EMLC_KEY_0 register.
                                   On completion,the success field indicates whether a match was found.
                                   The result associated with the matching key can be read from
                                   VPLKP_EMLC_RES_0 registers. */
    uint32_t reserved_0 : 5;    /* Reserved bits. */
    uint32_t success : 1;       /* On completion of a flush,insert or delete operation,this indicates that the
                                   opeartion was successful.
                                   (Note that only 0x1 is expected).
                                   On completion of a management search operation,this indicates whether a
                                   match was found.
                                   (0x1 = a match was found, 0x0 = no match found). */
    uint32_t reserved_1 : 22;   /* Reserved bits. */
    uint32_t busy : 1;          /* This bit is automatically set when the register is written to initiate an
                                   operation
                                   and is automatically cleared when the operation has completed.
                                   0x1 = an operation is ongoing and this register should not be written to.
                                   0x0 = any previous operation has completed,and another operation may be
                                   requested using a write to this register. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_t;
#endif /* GENHDR_STRUCT */

/* Indicates the cause of a warning alarm */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_WARNING_0                 0x00000028U
/* Reserved bits. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_WARNING_0_RESERVED_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_WARNING_0_RESERVED_SHIFT  0
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_WARNING_0_RESERVED_WIDTH  31
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_WARNING_0_RESERVED_MASK   (0x7FFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_WARNING_0_RESERVED_SHIFT)

/*
 * CPU access made to an unsupported register space.
 *  Write 1 to clear.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_WARNING_0_UNSUP_CPUSPACE_SHIFT 31
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_WARNING_0_UNSUP_CPUSPACE_WIDTH 1
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_WARNING_0_UNSUP_CPUSPACE_MASK (0x1U << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_WARNING_0_UNSUP_CPUSPACE_SHIFT)

#ifdef GENHDR_STRUCT
/* Indicates the cause of a warning alarm */
typedef union {
  struct {
    uint32_t reserved : 31;             /* Reserved bits. */
    uint32_t unsup_cpuspace : 1;        /* CPU access made to an unsupported register space.
                                           Write 1 to clear. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_WARNING_0_t;
#endif /* GENHDR_STRUCT */

/* Provides various error flags for the logic */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_FAT_ERR_0                 0x0000002CU
/*
 * Should be read when the Alarm flag is asserted in VPLKP_EML_ALARMS register.
 *  These fields are for hardware debug purposes.
 *  Their state should be reported in hardware bug reports.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_FAT_ERR_0_FATAL_ERROR_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_FAT_ERR_0_FATAL_ERROR_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_FAT_ERR_0_FATAL_ERROR_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_FAT_ERR_0_FATAL_ERROR_MASK (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_FAT_ERR_0_FATAL_ERROR_SHIFT)

#ifdef GENHDR_STRUCT
/* Provides various error flags for the logic */
typedef union {
  struct {
    uint32_t fatal_error : 32;  /* Should be read when the Alarm flag is asserted in VPLKP_EML_ALARMS register.
                                   These fields are for hardware debug purposes.
                                   Their state should be reported in hardware bug reports. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_FAT_ERR_0_t;
#endif /* GENHDR_STRUCT */

/* Provides various monitoring flags for the logic used for hardware debug */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MON_0                     0x00000030U
/*
 * These fields are for hardware debug purposes.
 *  Their state should be reported in hardware bug reports.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MON_0_MONITOR_SHIFT       0
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MON_0_MONITOR_WIDTH       32
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MON_0_MONITOR_MASK        (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MON_0_MONITOR_SHIFT)

#ifdef GENHDR_STRUCT
/* Provides various monitoring flags for the logic used for hardware debug */
typedef union {
  struct {
    uint32_t monitor : 32;      /* These fields are for hardware debug purposes.
                                   Their state should be reported in hardware bug reports. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MON_0_t;
#endif /* GENHDR_STRUCT */

/*
 * The 1st Key register. For an insert or management search operation,this is
 * written before the operation is requested to specify the least-significant 32
 * key bits to be used
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_0                     0x00000034U
/* Includes pre-key and label( {8'hFF, 4'b0, 20 bits MPLS label} ). */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_0_KEY_SHIFT           0
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_0_KEY_WIDTH           32
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_0_KEY_MASK            (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_0_KEY_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * The 1st Key register. For an insert or management search operation,this is
 * written before the operation is requested to specify the least-significant 32
 * key bits to be used
 */
typedef union {
  struct {
    uint32_t key : 32;  /* Includes pre-key and label( {8'hFF, 4'b0, 20 bits MPLS label} ). */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_0_t;
#endif /* GENHDR_STRUCT */

/* Reserved register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_1                     0x00000038U
/* Reserved register. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_1_RSVD_SHIFT          0
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_1_RSVD_WIDTH          32
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_1_RSVD_MASK           (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_1_RSVD_SHIFT)

#ifdef GENHDR_STRUCT
/* Reserved register */
typedef union {
  struct {
    uint32_t rsvd : 32; /* Reserved register. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_1_t;
#endif /* GENHDR_STRUCT */

/* Reserved register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_2                     0x0000003CU
/* Reserved register. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_2_RSVD_SHIFT          0
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_2_RSVD_WIDTH          32
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_2_RSVD_MASK           (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_2_RSVD_SHIFT)

#ifdef GENHDR_STRUCT
/* Reserved register */
typedef union {
  struct {
    uint32_t rsvd : 32; /* Reserved register. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_2_t;
#endif /* GENHDR_STRUCT */

/* Reserved register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_3                     0x00000040U
/* Reserved register. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_3_RSVD_SHIFT          0
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_3_RSVD_WIDTH          32
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_3_RSVD_MASK           (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_3_RSVD_SHIFT)

#ifdef GENHDR_STRUCT
/* Reserved register */
typedef union {
  struct {
    uint32_t rsvd : 32; /* Reserved register. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_3_t;
#endif /* GENHDR_STRUCT */

/*
 * The 1st Result register. For an insert operation,this is written before the
 * operation is requested to specify the least-significant 32 result bits to be
 * used. For management search operation,this can be read to get the result
 * associated with a matching key
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0                     0x00000044U
/*
 * Key mask register index.Total 8 reduced key mask combinations are available.
 *  For MPLSoGRE and MPLSoUDP,this field is zero(i.e.)No masking.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_KEYMASK_SHIFT       0
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_KEYMASK_WIDTH       3
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_KEYMASK_MASK        (0x7U << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_KEYMASK_SHIFT)

/* Virtual port type. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_VPORT_SHIFT         4
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_VPORT_WIDTH         2
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_VPORT_MASK          (0x3U << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_VPORT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * The 1st Result register. For an insert operation,this is written before the
 * operation is requested to specify the least-significant 32 result bits to be
 * used. For management search operation,this can be read to get the result
 * associated with a matching key
 */
typedef union {
  struct {
    uint32_t keymask : 3;       /* Key mask register index.Total 8 reduced key mask combinations are available.
                                   For MPLSoGRE and MPLSoUDP,this field is zero(i.e.)No masking. */
    uint32_t : 1;
    uint32_t vport : 2;         /* Virtual port type. */
    uint32_t : 26;
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_t;
#endif /* GENHDR_STRUCT */

#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_KEY_MASK_ENCODING_FULLKEY 0x0U /* micro flow full key exact match mode. no field will be masked out in the key . */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_KEY_MASK_ENCODING_MASK_1 0x1U /* key mask combination 1. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_KEY_MASK_ENCODING_MASK_2 0x2U /* key mask combination 2. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_KEY_MASK_ENCODING_MASK_3 0x3U /* key mask combination 3. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_KEY_MASK_ENCODING_MASK_4 0x4U /* key mask combination 4. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_KEY_MASK_ENCODING_MASK_5 0x5U /* key mask combination 5. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_KEY_MASK_ENCODING_MASK_6 0x6U /* key mask combination 6. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_KEY_MASK_ENCODING_MASK_7 0x7U /* key mask combination 7. */

#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_VIRTUAL_PORT_ENCODING_SLOWPATH 0x0U /* Slow path (default). */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_VIRTUAL_PORT_ENCODING_L_2PACKET 0x1U /* layer 2 packet. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_VIRTUAL_PORT_ENCODING_L_3PACKET 0x2U /* layer 3 packet. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_VIRTUAL_PORT_ENCODING_RESERVED 0x3U /* layer 3 packet as per current implementation. */

/* Reserved register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_1                     0x00000048U
/* Reserved register. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_1_RSVD_SHIFT          0
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_1_RSVD_WIDTH          8
#define MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_1_RSVD_MASK           (0xFFU << MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_1_RSVD_SHIFT)

#ifdef GENHDR_STRUCT
/* Reserved register */
typedef union {
  struct {
    uint32_t rsvd : 8;  /* Reserved register. */
    uint32_t : 24;
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_1_t;
#endif /* GENHDR_STRUCT */

/* Tunnel table command address register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_CMD_ADDR                  0x0000004CU
/*
 * Index to tunnel table - i.e,word address.
 *  Tunnel table has 4K entries/words,word width is 462 bits.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_CMD_ADDR_TUNNEL_TBL_ADDR_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_CMD_ADDR_TUNNEL_TBL_ADDR_WIDTH 12
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_CMD_ADDR_TUNNEL_TBL_ADDR_MASK (0xFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_CMD_ADDR_TUNNEL_TBL_ADDR_SHIFT)

/*
 * Specified operation type: 0 -> No operation.
 *  1 -> Write operation(i.e. write contents from data registers to the
 * specified tunnel table address).
 *  2 -> Read operation(i.e. read contents from specified tunnel table address
 * to the data registers).
 *  3 -> No operation.
 *  NOTE:The FPGA logic detects the rising edge based on the write (2'b01) or
 * read(2'b10) in the op_type[1:0] and clears this register once the write/read
 * operation is completed.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_CMD_ADDR_OP_TYPE_SHIFT    16
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_CMD_ADDR_OP_TYPE_WIDTH    2
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_CMD_ADDR_OP_TYPE_MASK     (0x3U << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_CMD_ADDR_OP_TYPE_SHIFT)

#ifdef GENHDR_STRUCT
/* Tunnel table command address register */
typedef union {
  struct {
    uint32_t tunnel_tbl_addr : 12;      /* Index to tunnel table - i.e,word address.
                                           Tunnel table has 4K entries/words,word width is 462 bits. */
    uint32_t : 4;
    uint32_t op_type : 2;               /* Specified operation type: 0 -> No operation.
                                           1 -> Write operation(i.e. write contents from data registers to the
                                           specified tunnel table address).
                                           2 -> Read operation(i.e. read contents from specified tunnel table address
                                           to the data registers).
                                           3 -> No operation.
                                           NOTE:The FPGA logic detects the rising edge based on the write (2'b01) or
                                           read(2'b10) in the op_type[1:0] and clears this register once the write/read
                                           operation is completed. */
    uint32_t : 14;
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_CMD_ADDR_t;
#endif /* GENHDR_STRUCT */

/* Tunnel table status register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_STS                       0x00000050U
/*
 * If '1',already a read/write operation(as specified in op_type) is under
 * progress.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_STS_BUSY_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_STS_BUSY_SHIFT            0
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_STS_BUSY_WIDTH            1
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_STS_BUSY_MASK             (0x1U << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_STS_BUSY_SHIFT)

#ifdef GENHDR_STRUCT
/* Tunnel table status register */
typedef union {
  struct {
    uint32_t busy : 1;  /* If '1',already a read/write operation(as specified in op_type) is under
                           progress. */
    uint32_t : 31;
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_STS_t;
#endif /* GENHDR_STRUCT */

/* The 1st tunnel table data register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_0                    0x00000054U
/*
 * This bit defines IPv4/IPv6 tunnel type.
 *  Default:'0' - IPv4.Currently,only IPv4 is supported.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_0_IP_TYPE_SHIFT      0
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_0_IP_TYPE_WIDTH      1
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_0_IP_TYPE_MASK       (0x1U << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_0_IP_TYPE_SHIFT)

/*
 * IP Encap SA [30:0] bits.
 *  To be used for IPv4 or IPv6.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_0_ENCAP_IP_SA_0_SHIFT 1
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_0_ENCAP_IP_SA_0_WIDTH 31
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_0_ENCAP_IP_SA_0_MASK (0x7FFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_0_ENCAP_IP_SA_0_SHIFT)

#ifdef GENHDR_STRUCT
/* The 1st tunnel table data register */
typedef union {
  struct {
    uint32_t ip_type : 1;               /* This bit defines IPv4/IPv6 tunnel type.
                                           Default:'0' - IPv4.Currently,only IPv4 is supported. */
    uint32_t encap_ip_sa_0 : 31;        /* IP Encap SA [30:0] bits.
                                           To be used for IPv4 or IPv6. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_0_t;
#endif /* GENHDR_STRUCT */

/* The 2nd tunnel table data register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_1                    0x00000058U
/*
 * IP Encap SA [62:31] bits.
 *  To be used for IPv4 or IPv6.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_1_ENCAP_IP_SA_1_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_1_ENCAP_IP_SA_1_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_1_ENCAP_IP_SA_1_MASK (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_1_ENCAP_IP_SA_1_SHIFT)

#ifdef GENHDR_STRUCT
/* The 2nd tunnel table data register */
typedef union {
  struct {
    uint32_t encap_ip_sa_1 : 32;        /* IP Encap SA [62:31] bits.
                                           To be used for IPv4 or IPv6. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_1_t;
#endif /* GENHDR_STRUCT */

/* The 3rd tunnel table data register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_2                    0x0000005CU
/*
 * IP Encap SA [94:63] bits.
 *  To be used for IPv4 or IPv6.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_2_ENCAP_IP_SA_2_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_2_ENCAP_IP_SA_2_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_2_ENCAP_IP_SA_2_MASK (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_2_ENCAP_IP_SA_2_SHIFT)

#ifdef GENHDR_STRUCT
/* The 3rd tunnel table data register */
typedef union {
  struct {
    uint32_t encap_ip_sa_2 : 32;        /* IP Encap SA [94:63] bits.
                                           To be used for IPv4 or IPv6. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_2_t;
#endif /* GENHDR_STRUCT */

/* The 4th tunnel table data register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_3                    0x00000060U
/*
 * IP Encap SA [126:95] bits.
 *  To be used for IPv4 or IPv6.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_3_ENCAP_IP_SA_3_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_3_ENCAP_IP_SA_3_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_3_ENCAP_IP_SA_3_MASK (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_3_ENCAP_IP_SA_3_SHIFT)

#ifdef GENHDR_STRUCT
/* The 4th tunnel table data register */
typedef union {
  struct {
    uint32_t encap_ip_sa_3 : 32;        /* IP Encap SA [126:95] bits.
                                           To be used for IPv4 or IPv6. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_3_t;
#endif /* GENHDR_STRUCT */

/* The 5th tunnel table data register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_4                    0x00000064U
/*
 * IP Encap SA [127] bit.
 *  To be used for IPv4 or IPv6.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_4_ENCAP_IP_SA_4_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_4_ENCAP_IP_SA_4_WIDTH 1
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_4_ENCAP_IP_SA_4_MASK (0x1U << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_4_ENCAP_IP_SA_4_SHIFT)

/*
 * IP Encap DA [30:0] bits.
 *  To be used for IPv4 or IPv6.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_4_ENCAP_IP_DA_0_SHIFT 1
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_4_ENCAP_IP_DA_0_WIDTH 31
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_4_ENCAP_IP_DA_0_MASK (0x7FFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_4_ENCAP_IP_DA_0_SHIFT)

#ifdef GENHDR_STRUCT
/* The 5th tunnel table data register */
typedef union {
  struct {
    uint32_t encap_ip_sa_4 : 1;         /* IP Encap SA [127] bit.
                                           To be used for IPv4 or IPv6. */
    uint32_t encap_ip_da_0 : 31;        /* IP Encap DA [30:0] bits.
                                           To be used for IPv4 or IPv6. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_4_t;
#endif /* GENHDR_STRUCT */

/* The 6th tunnel table data register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_5                    0x00000068U
/*
 * IP Encap DA [62:31] bits.
 *  To be used for IPv4 or IPv6.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_5_ENCAP_IP_DA_1_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_5_ENCAP_IP_DA_1_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_5_ENCAP_IP_DA_1_MASK (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_5_ENCAP_IP_DA_1_SHIFT)

#ifdef GENHDR_STRUCT
/* The 6th tunnel table data register */
typedef union {
  struct {
    uint32_t encap_ip_da_1 : 32;        /* IP Encap DA [62:31] bits.
                                           To be used for IPv4 or IPv6. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_5_t;
#endif /* GENHDR_STRUCT */

/* The 7th tunnel table data register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_6                    0x0000006CU
/*
 * IP Encap DA [94:63] bits.
 *  To be used for IPv4 or IPv6.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_6_ENCAP_IP_DA_2_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_6_ENCAP_IP_DA_2_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_6_ENCAP_IP_DA_2_MASK (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_6_ENCAP_IP_DA_2_SHIFT)

#ifdef GENHDR_STRUCT
/* The 7th tunnel table data register */
typedef union {
  struct {
    uint32_t encap_ip_da_2 : 32;        /* IP Encap DA [94:63] bits.
                                           To be used for IPv4 or IPv6. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_6_t;
#endif /* GENHDR_STRUCT */

/* The 8th tunnel table data register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_7                    0x00000070U
/*
 * IP Encap DA [126:95] bits.
 *  To be used for IPv4 or IPv6.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_7_ENCAP_IP_DA_3_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_7_ENCAP_IP_DA_3_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_7_ENCAP_IP_DA_3_MASK (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_7_ENCAP_IP_DA_3_SHIFT)

#ifdef GENHDR_STRUCT
/* The 8th tunnel table data register */
typedef union {
  struct {
    uint32_t encap_ip_da_3 : 32;        /* IP Encap DA [126:95] bits.
                                           To be used for IPv4 or IPv6. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_7_t;
#endif /* GENHDR_STRUCT */

/* The 9th tunnel table data register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_8                    0x00000074U
/*
 * IP Encap DA [127] bit.
 *  To be used for IPv4 or IPv6.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_8_ENCAP_IP_DA_4_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_8_ENCAP_IP_DA_4_WIDTH 1
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_8_ENCAP_IP_DA_4_MASK (0x1U << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_8_ENCAP_IP_DA_4_SHIFT)

/* Destination MAC Address [30:0] bits. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_8_DEST_MAC_0_SHIFT   1
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_8_DEST_MAC_0_WIDTH   31
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_8_DEST_MAC_0_MASK    (0x7FFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_8_DEST_MAC_0_SHIFT)

#ifdef GENHDR_STRUCT
/* The 9th tunnel table data register */
typedef union {
  struct {
    uint32_t encap_ip_da_4 : 1;         /* IP Encap DA [127] bit.
                                           To be used for IPv4 or IPv6. */
    uint32_t dest_mac_0 : 31;           /* Destination MAC Address [30:0] bits. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_8_t;
#endif /* GENHDR_STRUCT */

/* The 10th tunnel table data register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_9                    0x00000078U
/* Destination MAC address[47:31] bits. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_9_DEST_MAC_1_SHIFT   0
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_9_DEST_MAC_1_WIDTH   17
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_9_DEST_MAC_1_MASK    (0x1FFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_9_DEST_MAC_1_SHIFT)

/*
 * The FPGA MAC Identifier.
 *  Also it is an index to SMAC table that are set per interface.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_9_PHYSICAL_PORT_ID_SHIFT 17
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_9_PHYSICAL_PORT_ID_WIDTH 4
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_9_PHYSICAL_PORT_ID_MASK (0xFU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_9_PHYSICAL_PORT_ID_SHIFT)

/*
 * Encap vxlan vni[10:0]bits.
 *  Encap GRE key[10:0]bits.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_9_ENCAP_VXLAN_VNI_0_SHIFT 21
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_9_ENCAP_VXLAN_VNI_0_WIDTH 11
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_9_ENCAP_VXLAN_VNI_0_MASK (0x7FFU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_9_ENCAP_VXLAN_VNI_0_SHIFT)

#ifdef GENHDR_STRUCT
/* The 10th tunnel table data register */
typedef union {
  struct {
    uint32_t dest_mac_1 : 17;           /* Destination MAC address[47:31] bits. */
    uint32_t physical_port_id : 4;      /* The FPGA MAC Identifier.
                                           Also it is an index to SMAC table that are set per interface. */
    uint32_t encap_vxlan_vni_0 : 11;    /* Encap vxlan vni[10:0]bits.
                                           Encap GRE key[10:0]bits. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_9_t;
#endif /* GENHDR_STRUCT */

/* The 11th tunnel table data register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_10                   0x0000007CU
/*
 * Encap vxlan vni[23:11]bits.
 *  Encap GRE key[23:11]bits.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_10_ENCAP_VXLAN_VNI_1_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_10_ENCAP_VXLAN_VNI_1_WIDTH 13
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_10_ENCAP_VXLAN_VNI_1_MASK (0x1FFFU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_10_ENCAP_VXLAN_VNI_1_SHIFT)

/* Reserved. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_10_RSVD_SHIFT        13
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_10_RSVD_WIDTH        8
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_10_RSVD_MASK         (0xFFU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_10_RSVD_SHIFT)

/* IPv4 TTL or IPv6 hop limit. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_10_ENCAP_TTL_SHIFT   21
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_10_ENCAP_TTL_WIDTH   8
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_10_ENCAP_TTL_MASK    (0xFFU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_10_ENCAP_TTL_SHIFT)

/* Encap IP tos[2:0]bits: DSCP,ECN value. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_10_TOS_0_SHIFT       29
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_10_TOS_0_WIDTH       3
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_10_TOS_0_MASK        (0x7U << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_10_TOS_0_SHIFT)

#ifdef GENHDR_STRUCT
/* The 11th tunnel table data register */
typedef union {
  struct {
    uint32_t encap_vxlan_vni_1 : 13;    /* Encap vxlan vni[23:11]bits.
                                           Encap GRE key[23:11]bits. */
    uint32_t rsvd : 8;                  /* Reserved. */
    uint32_t encap_ttl : 8;             /* IPv4 TTL or IPv6 hop limit. */
    uint32_t tos_0 : 3;                 /* Encap IP tos[2:0]bits: DSCP,ECN value. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_10_t;
#endif /* GENHDR_STRUCT */

/* The 12th tunnel table data register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_11                   0x00000080U
/*  Encap IP tos[7:3]bits: DSCP,ECN value. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_11_TOS_1_SHIFT       0
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_11_TOS_1_WIDTH       5
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_11_TOS_1_MASK        (0x1FU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_11_TOS_1_SHIFT)

/*  Encap IP protocol(next protocol in IP header). */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_11_ENCAP_IP_PROT_SHIFT 5
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_11_ENCAP_IP_PROT_WIDTH 8
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_11_ENCAP_IP_PROT_MASK (0xFFU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_11_ENCAP_IP_PROT_SHIFT)

/*  Encap vlantag 0 vid(single or first outer vlan for vxlan tunnel). */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_11_ENCAP_TCI_0_SHIFT 13
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_11_ENCAP_TCI_0_WIDTH 16
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_11_ENCAP_TCI_0_MASK  (0xFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_11_ENCAP_TCI_0_SHIFT)

/*  Encap vlantag 1 vid [2:0] bits(second outer VLAN for Vxlan in Q-in-Q). */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_11_ENCAP_TCI_1_SHIFT 29
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_11_ENCAP_TCI_1_WIDTH 3
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_11_ENCAP_TCI_1_MASK  (0x7U << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_11_ENCAP_TCI_1_SHIFT)

#ifdef GENHDR_STRUCT
/* The 12th tunnel table data register */
typedef union {
  struct {
    uint32_t tos_1 : 5;                 /* Encap IP tos[7:3]bits: DSCP,ECN value. */
    uint32_t encap_ip_prot : 8;         /* Encap IP protocol(next protocol in IP header). */
    uint32_t encap_tci_0 : 16;          /* Encap vlantag 0 vid(single or first outer vlan for vxlan tunnel). */
    uint32_t encap_tci_1 : 3;           /* Encap vlantag 1 vid [2:0] bits(second outer VLAN for Vxlan in Q-in-Q). */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_11_t;
#endif /* GENHDR_STRUCT */

/* The 13th tunnel table data register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_12                   0x00000084U
/* Encap vlantag 1 vid[15:3] bits(second outer VLAN for Vxlan in Q-in-Q). */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_12_ENCAP_TCI_1_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_12_ENCAP_TCI_1_WIDTH 13
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_12_ENCAP_TCI_1_MASK  (0x1FFFU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_12_ENCAP_TCI_1_SHIFT)

/* Encap vxlan tci 1 enable(double VLAN enablement for Vxlan). */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_12_ENCAP_VXLAN_TCI_1_EN_SHIFT 13
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_12_ENCAP_VXLAN_TCI_1_EN_WIDTH 1
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_12_ENCAP_VXLAN_TCI_1_EN_MASK (0x1U << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_12_ENCAP_VXLAN_TCI_1_EN_SHIFT)

/* Encap ethertype(EtherType for packet without VLAN). */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_12_ENCAP_ETH_TYPE_SHIFT 14
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_12_ENCAP_ETH_TYPE_WIDTH 16
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_12_ENCAP_ETH_TYPE_MASK (0xFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_12_ENCAP_ETH_TYPE_SHIFT)

/* Encap vxlan tci 0 enable(Single VLAN enablement for Vxlan). */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_12_ENCAP_VXLAN_TCI_0_EN_SHIFT 30
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_12_ENCAP_VXLAN_TCI_0_EN_WIDTH 1
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_12_ENCAP_VXLAN_TCI_0_EN_MASK (0x1U << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_12_ENCAP_VXLAN_TCI_0_EN_SHIFT)

/* Encap GRE Sequence[16] bit. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_12_ENCAP_GRE_SEQ_NUM_1_SHIFT 31
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_12_ENCAP_GRE_SEQ_NUM_1_WIDTH 1
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_12_ENCAP_GRE_SEQ_NUM_1_MASK (0x1U << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_12_ENCAP_GRE_SEQ_NUM_1_SHIFT)

#ifdef GENHDR_STRUCT
/* The 13th tunnel table data register */
typedef union {
  struct {
    uint32_t encap_tci_1 : 13;          /* Encap vlantag 1 vid[15:3] bits(second outer VLAN for Vxlan in Q-in-Q). */
    uint32_t encap_vxlan_tci_1_en : 1;  /* Encap vxlan tci 1 enable(double VLAN enablement for Vxlan). */
    uint32_t encap_eth_type : 16;       /* Encap ethertype(EtherType for packet without VLAN). */
    uint32_t encap_vxlan_tci_0_en : 1;  /* Encap vxlan tci 0 enable(Single VLAN enablement for Vxlan). */
    uint32_t encap_gre_seq_num_1 : 1;   /* Encap GRE Sequence[16] bit. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_12_t;
#endif /* GENHDR_STRUCT */

/* The 14th tunnel table data register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_13                   0x00000088U
/* Encap GRE Sequence[31:17] bits. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_13_ENCAP_GRE_SEQ_NUM_2_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_13_ENCAP_GRE_SEQ_NUM_2_WIDTH 15
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_13_ENCAP_GRE_SEQ_NUM_2_MASK (0x7FFFU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_13_ENCAP_GRE_SEQ_NUM_2_SHIFT)

/*
 * The tunnel UDP destination port[15:0] bits.
 *  Encap GRE Sequence[15:0] bits(encapGreSeqNum0).
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_13_ENCAP_L4_DST_PORT_SHIFT 15
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_13_ENCAP_L4_DST_PORT_WIDTH 16
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_13_ENCAP_L4_DST_PORT_MASK (0xFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_13_ENCAP_L4_DST_PORT_SHIFT)

/*
 * Encap vxlan flags[0] bit.
 *  Encap GRE key[24] bit.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_13_ENCAP_VXLAN_FLAGS_0_SHIFT 31
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_13_ENCAP_VXLAN_FLAGS_0_WIDTH 1
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_13_ENCAP_VXLAN_FLAGS_0_MASK (0x1U << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_13_ENCAP_VXLAN_FLAGS_0_SHIFT)

#ifdef GENHDR_STRUCT
/* The 14th tunnel table data register */
typedef union {
  struct {
    uint32_t encap_gre_seq_num_2 : 15;  /* Encap GRE Sequence[31:17] bits. */
    uint32_t encap_l4_dst_port : 16;    /* The tunnel UDP destination port[15:0] bits.
                                           Encap GRE Sequence[15:0] bits(encapGreSeqNum0). */
    uint32_t encap_vxlan_flags_0 : 1;   /* Encap vxlan flags[0] bit.
                                           Encap GRE key[24] bit. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_13_t;
#endif /* GENHDR_STRUCT */

/* The 15th tunnel table data register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14                   0x0000008CU
/*
 * Encap vxlan flags[7:1] bits.
 *  Encap GRE key[31:25] bits.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_ENCAP_VXLAN_FLAGS_1_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_ENCAP_VXLAN_FLAGS_1_WIDTH 7
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_ENCAP_VXLAN_FLAGS_1_MASK (0x7FU << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_ENCAP_VXLAN_FLAGS_1_SHIFT)

/*
 * Encap vxlan enable.
 *  Encap GRE key field enable.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_ENCAP_VXLAN_EN_SHIFT 7
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_ENCAP_VXLAN_EN_WIDTH 1
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_ENCAP_VXLAN_EN_MASK (0x1U << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_ENCAP_VXLAN_EN_SHIFT)

/* MPLSoUDP: Encap ETH header to the packet. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_MPLS_OUDPEN_ETH_HDR_SHIFT 8
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_MPLS_OUDPEN_ETH_HDR_WIDTH 1
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_MPLS_OUDPEN_ETH_HDR_MASK (0x1U << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_MPLS_OUDPEN_ETH_HDR_SHIFT)

/* MPLSoUDP: Encap IP and UDP header to the packet. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_MPLS_OUDPEN_IP_HDR_SHIFT 9
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_MPLS_OUDPEN_IP_HDR_WIDTH 1
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_MPLS_OUDPEN_IP_HDR_MASK (0x1U << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_MPLS_OUDPEN_IP_HDR_SHIFT)

/* IPoGRE: Encap IP and GRE header to the packet. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_IP_OGRE_EN_IP_GRE_HDR_SHIFT 10
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_IP_OGRE_EN_IP_GRE_HDR_WIDTH 1
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_IP_OGRE_EN_IP_GRE_HDR_MASK (0x1U << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_IP_OGRE_EN_IP_GRE_HDR_SHIFT)

/* IPoGRE: Encap ETH header to the packet. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_IP_OGRE_EN_ETH_HDR_SHIFT 11
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_IP_OGRE_EN_ETH_HDR_WIDTH 1
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_IP_OGRE_EN_ETH_HDR_MASK (0x1U << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_IP_OGRE_EN_ETH_HDR_SHIFT)

/* MPLSoGRE: Encap ETH header to the packet. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_MPLS_OGRE_EN_ETH_HDR_SHIFT 12
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_MPLS_OGRE_EN_ETH_HDR_WIDTH 1
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_MPLS_OGRE_EN_ETH_HDR_MASK (0x1U << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_MPLS_OGRE_EN_ETH_HDR_SHIFT)

/* MPLSoGRE: Encap IP and GRE header to the packet. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_MPLS_OGRE_EN_IP_HDR_SHIFT 13
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_MPLS_OGRE_EN_IP_HDR_WIDTH 1
#define MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_MPLS_OGRE_EN_IP_HDR_MASK (0x1U << MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_MPLS_OGRE_EN_IP_HDR_SHIFT)

#ifdef GENHDR_STRUCT
/* The 15th tunnel table data register */
typedef union {
  struct {
    uint32_t encap_vxlan_flags_1 : 7;           /* Encap vxlan flags[7:1] bits.
                                                   Encap GRE key[31:25] bits. */
    uint32_t encap_vxlan_en : 1;                /* Encap vxlan enable.
                                                   Encap GRE key field enable. */
    uint32_t mpls_oudpen_eth_hdr : 1;           /* MPLSoUDP: Encap ETH header to the packet. */
    uint32_t mpls_oudpen_ip_hdr : 1;            /* MPLSoUDP: Encap IP and UDP header to the packet. */
    uint32_t ip_ogre_en_ip_gre_hdr : 1;         /* IPoGRE: Encap IP and GRE header to the packet. */
    uint32_t ip_ogre_en_eth_hdr : 1;            /* IPoGRE: Encap ETH header to the packet. */
    uint32_t mpls_ogre_en_eth_hdr : 1;          /* MPLSoGRE: Encap ETH header to the packet. */
    uint32_t mpls_ogre_en_ip_hdr : 1;           /* MPLSoGRE: Encap IP and GRE header to the packet. */
    uint32_t : 18;
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_TUNNEL_TBL_DATA_14_t;
#endif /* GENHDR_STRUCT */

/* Reserved register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_RSVD0                0x00000090U
/*
 * Reserved register.
 *  Read only.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_RSVD0_RSVD_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_RSVD0_RSVD_SHIFT     0
#define MKS_VPORTLKUP_TUNNEL_TABLE_RSVD0_RSVD_WIDTH     32
#define MKS_VPORTLKUP_TUNNEL_TABLE_RSVD0_RSVD_MASK      (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_RSVD0_RSVD_SHIFT)

#ifdef GENHDR_STRUCT
/* Reserved register */
typedef union {
  struct {
    uint32_t rsvd : 32; /* Reserved register.
                           Read only. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_RSVD0_t;
#endif /* GENHDR_STRUCT */

/* Reserved register */
#define MKS_VPORTLKUP_TUNNEL_TABLE_RSVD1                0x00000094U
/*
 * Reserved register.
 *  Read only.
 */
#define MKS_VPORTLKUP_TUNNEL_TABLE_RSVD1_RSVD_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_RSVD1_RSVD_SHIFT     0
#define MKS_VPORTLKUP_TUNNEL_TABLE_RSVD1_RSVD_WIDTH     32
#define MKS_VPORTLKUP_TUNNEL_TABLE_RSVD1_RSVD_MASK      (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_RSVD1_RSVD_SHIFT)

#ifdef GENHDR_STRUCT
/* Reserved register */
typedef union {
  struct {
    uint32_t rsvd : 32; /* Reserved register.
                           Read only. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_RSVD1_t;
#endif /* GENHDR_STRUCT */

/* Version number of the OVS FPGA RTL code (based on Reduced key) */
#define MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_VERSION_REDU                    0x00000098U
/* Revision Number. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_VERSION_REDU_REV_NUM_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_VERSION_REDU_REV_NUM_SHIFT      0
#define MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_VERSION_REDU_REV_NUM_WIDTH      8
#define MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_VERSION_REDU_REV_NUM_MASK       (0xFFU << MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_VERSION_REDU_REV_NUM_SHIFT)

/* Version Number. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_VERSION_REDU_VER_NUM_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_VERSION_REDU_VER_NUM_SHIFT      8
#define MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_VERSION_REDU_VER_NUM_WIDTH      8
#define MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_VERSION_REDU_VER_NUM_MASK       (0xFFU << MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_VERSION_REDU_VER_NUM_SHIFT)

/* Reserved bits. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_VERSION_REDU_RESERVED_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_VERSION_REDU_RESERVED_SHIFT     16
#define MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_VERSION_REDU_RESERVED_WIDTH     16
#define MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_VERSION_REDU_RESERVED_MASK      (0xFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_VERSION_REDU_RESERVED_SHIFT)

#ifdef GENHDR_STRUCT
/* Version number of the OVS FPGA RTL code (based on Reduced key) */
typedef union {
  struct {
    uint32_t rev_num : 8;       /* Revision Number. */
    uint32_t ver_num : 8;       /* Version Number. */
    uint32_t reserved : 16;     /* Reserved bits. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_VERSION_REDU_t;
#endif /* GENHDR_STRUCT */

/* Compile date of the bitstream for OVS FPGA RTL code (based on Reduced key) */
#define MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_CMPL_DATE_REDU                  0x0000009CU
/* Compile date, format: yyyy_mm_dd. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_CMPL_DATE_REDU_CMPL_DATE_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_CMPL_DATE_REDU_CMPL_DATE_SHIFT  0
#define MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_CMPL_DATE_REDU_CMPL_DATE_WIDTH  32
#define MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_CMPL_DATE_REDU_CMPL_DATE_MASK   (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_CMPL_DATE_REDU_CMPL_DATE_SHIFT)

#ifdef GENHDR_STRUCT
/* Compile date of the bitstream for OVS FPGA RTL code (based on Reduced key) */
typedef union {
  struct {
    uint32_t cmpl_date : 32;    /* Compile date, format: yyyy_mm_dd. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_FPGA_CMPL_DATE_REDU_t;
#endif /* GENHDR_STRUCT */

/* Lower 32bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_0                    0x000000A0U
/* Source MAC Address bits [31:0]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_0_SMAC_ADDR_LO_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_0_SMAC_ADDR_LO_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_0_SMAC_ADDR_LO_MASK  (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_0_SMAC_ADDR_LO_SHIFT)

#ifdef GENHDR_STRUCT
/* Lower 32bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_lo : 32; /* Source MAC Address bits [31:0]. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_0_t;
#endif /* GENHDR_STRUCT */

/* Higher 16bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_0                    0x000000A4U
/* Source MAC Address bits[47:32]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_0_SMAC_ADDR_HI_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_0_SMAC_ADDR_HI_WIDTH 16
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_0_SMAC_ADDR_HI_MASK  (0xFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_0_SMAC_ADDR_HI_SHIFT)

#ifdef GENHDR_STRUCT
/* Higher 16bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_hi : 16; /* Source MAC Address bits[47:32]. */
    uint32_t : 16;
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_0_t;
#endif /* GENHDR_STRUCT */

/* Lower 32bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_1                    0x000000A8U
/* Source MAC Address bits [31:0]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_1_SMAC_ADDR_LO_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_1_SMAC_ADDR_LO_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_1_SMAC_ADDR_LO_MASK  (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_1_SMAC_ADDR_LO_SHIFT)

#ifdef GENHDR_STRUCT
/* Lower 32bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_lo : 32; /* Source MAC Address bits [31:0]. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_1_t;
#endif /* GENHDR_STRUCT */

/* Higher 16bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_1                    0x000000ACU
/* Source MAC Address bits[47:32]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_1_SMAC_ADDR_HI_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_1_SMAC_ADDR_HI_WIDTH 16
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_1_SMAC_ADDR_HI_MASK  (0xFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_1_SMAC_ADDR_HI_SHIFT)

#ifdef GENHDR_STRUCT
/* Higher 16bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_hi : 16; /* Source MAC Address bits[47:32]. */
    uint32_t : 16;
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_1_t;
#endif /* GENHDR_STRUCT */

/* Lower 32bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_2                    0x000000B0U
/* Source MAC Address bits [31:0]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_2_SMAC_ADDR_LO_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_2_SMAC_ADDR_LO_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_2_SMAC_ADDR_LO_MASK  (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_2_SMAC_ADDR_LO_SHIFT)

#ifdef GENHDR_STRUCT
/* Lower 32bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_lo : 32; /* Source MAC Address bits [31:0]. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_2_t;
#endif /* GENHDR_STRUCT */

/* Higher 16bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_2                    0x000000B4U
/* Source MAC Address bits[47:32]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_2_SMAC_ADDR_HI_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_2_SMAC_ADDR_HI_WIDTH 16
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_2_SMAC_ADDR_HI_MASK  (0xFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_2_SMAC_ADDR_HI_SHIFT)

#ifdef GENHDR_STRUCT
/* Higher 16bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_hi : 16; /* Source MAC Address bits[47:32]. */
    uint32_t : 16;
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_2_t;
#endif /* GENHDR_STRUCT */

/* Lower 32bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_3                    0x000000B8U
/* Source MAC Address bits [31:0]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_3_SMAC_ADDR_LO_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_3_SMAC_ADDR_LO_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_3_SMAC_ADDR_LO_MASK  (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_3_SMAC_ADDR_LO_SHIFT)

#ifdef GENHDR_STRUCT
/* Lower 32bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_lo : 32; /* Source MAC Address bits [31:0]. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_3_t;
#endif /* GENHDR_STRUCT */

/* Higher 16bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_3                    0x000000BCU
/* Source MAC Address bits[47:32]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_3_SMAC_ADDR_HI_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_3_SMAC_ADDR_HI_WIDTH 16
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_3_SMAC_ADDR_HI_MASK  (0xFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_3_SMAC_ADDR_HI_SHIFT)

#ifdef GENHDR_STRUCT
/* Higher 16bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_hi : 16; /* Source MAC Address bits[47:32]. */
    uint32_t : 16;
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_3_t;
#endif /* GENHDR_STRUCT */

/* Lower 32bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_4                    0x000000C0U
/* Source MAC Address bits [31:0]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_4_SMAC_ADDR_LO_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_4_SMAC_ADDR_LO_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_4_SMAC_ADDR_LO_MASK  (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_4_SMAC_ADDR_LO_SHIFT)

#ifdef GENHDR_STRUCT
/* Lower 32bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_lo : 32; /* Source MAC Address bits [31:0]. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_4_t;
#endif /* GENHDR_STRUCT */

/* Higher 16bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_4                    0x000000C4U
/* Source MAC Address bits[47:32]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_4_SMAC_ADDR_HI_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_4_SMAC_ADDR_HI_WIDTH 16
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_4_SMAC_ADDR_HI_MASK  (0xFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_4_SMAC_ADDR_HI_SHIFT)

#ifdef GENHDR_STRUCT
/* Higher 16bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_hi : 16; /* Source MAC Address bits[47:32]. */
    uint32_t : 16;
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_4_t;
#endif /* GENHDR_STRUCT */

/* Lower 32bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_5                    0x000000C8U
/* Source MAC Address bits [31:0]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_5_SMAC_ADDR_LO_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_5_SMAC_ADDR_LO_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_5_SMAC_ADDR_LO_MASK  (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_5_SMAC_ADDR_LO_SHIFT)

#ifdef GENHDR_STRUCT
/* Lower 32bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_lo : 32; /* Source MAC Address bits [31:0]. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_5_t;
#endif /* GENHDR_STRUCT */

/* Higher 16bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_5                    0x000000CCU
/* Source MAC Address bits[47:32]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_5_SMAC_ADDR_HI_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_5_SMAC_ADDR_HI_WIDTH 16
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_5_SMAC_ADDR_HI_MASK  (0xFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_5_SMAC_ADDR_HI_SHIFT)

#ifdef GENHDR_STRUCT
/* Higher 16bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_hi : 16; /* Source MAC Address bits[47:32]. */
    uint32_t : 16;
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_5_t;
#endif /* GENHDR_STRUCT */

/* Lower 32bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_6                    0x000000D0U
/* Source MAC Address bits [31:0]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_6_SMAC_ADDR_LO_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_6_SMAC_ADDR_LO_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_6_SMAC_ADDR_LO_MASK  (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_6_SMAC_ADDR_LO_SHIFT)

#ifdef GENHDR_STRUCT
/* Lower 32bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_lo : 32; /* Source MAC Address bits [31:0]. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_6_t;
#endif /* GENHDR_STRUCT */

/* Higher 16bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_6                    0x000000D4U
/* Source MAC Address bits[47:32]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_6_SMAC_ADDR_HI_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_6_SMAC_ADDR_HI_WIDTH 16
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_6_SMAC_ADDR_HI_MASK  (0xFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_6_SMAC_ADDR_HI_SHIFT)

#ifdef GENHDR_STRUCT
/* Higher 16bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_hi : 16; /* Source MAC Address bits[47:32]. */
    uint32_t : 16;
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_6_t;
#endif /* GENHDR_STRUCT */

/* Lower 32bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_7                    0x000000D8U
/* Source MAC Address bits [31:0]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_7_SMAC_ADDR_LO_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_7_SMAC_ADDR_LO_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_7_SMAC_ADDR_LO_MASK  (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_7_SMAC_ADDR_LO_SHIFT)

#ifdef GENHDR_STRUCT
/* Lower 32bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_lo : 32; /* Source MAC Address bits [31:0]. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_7_t;
#endif /* GENHDR_STRUCT */

/* Higher 16bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_7                    0x000000DCU
/* Source MAC Address bits[47:32]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_7_SMAC_ADDR_HI_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_7_SMAC_ADDR_HI_WIDTH 16
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_7_SMAC_ADDR_HI_MASK  (0xFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_7_SMAC_ADDR_HI_SHIFT)

#ifdef GENHDR_STRUCT
/* Higher 16bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_hi : 16; /* Source MAC Address bits[47:32]. */
    uint32_t : 16;
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_7_t;
#endif /* GENHDR_STRUCT */

/* Lower 32bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_8                    0x000000E0U
/* Source MAC Address bits [31:0]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_8_SMAC_ADDR_LO_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_8_SMAC_ADDR_LO_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_8_SMAC_ADDR_LO_MASK  (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_8_SMAC_ADDR_LO_SHIFT)

#ifdef GENHDR_STRUCT
/* Lower 32bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_lo : 32; /* Source MAC Address bits [31:0]. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_8_t;
#endif /* GENHDR_STRUCT */

/* Higher 16bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_8                    0x000000E4U
/* Source MAC Address bits[47:32]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_8_SMAC_ADDR_HI_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_8_SMAC_ADDR_HI_WIDTH 16
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_8_SMAC_ADDR_HI_MASK  (0xFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_8_SMAC_ADDR_HI_SHIFT)

#ifdef GENHDR_STRUCT
/* Higher 16bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_hi : 16; /* Source MAC Address bits[47:32]. */
    uint32_t : 16;
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_8_t;
#endif /* GENHDR_STRUCT */

/* Lower 32bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_9                    0x000000E8U
/* Source MAC Address bits [31:0]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_9_SMAC_ADDR_LO_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_9_SMAC_ADDR_LO_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_9_SMAC_ADDR_LO_MASK  (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_9_SMAC_ADDR_LO_SHIFT)

#ifdef GENHDR_STRUCT
/* Lower 32bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_lo : 32; /* Source MAC Address bits [31:0]. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_9_t;
#endif /* GENHDR_STRUCT */

/* Higher 16bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_9                    0x000000ECU
/* Source MAC Address bits[47:32]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_9_SMAC_ADDR_HI_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_9_SMAC_ADDR_HI_WIDTH 16
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_9_SMAC_ADDR_HI_MASK  (0xFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_9_SMAC_ADDR_HI_SHIFT)

#ifdef GENHDR_STRUCT
/* Higher 16bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_hi : 16; /* Source MAC Address bits[47:32]. */
    uint32_t : 16;
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_9_t;
#endif /* GENHDR_STRUCT */

/* Lower 32bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_10                   0x000000F0U
/* Source MAC Address bits [31:0]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_10_SMAC_ADDR_LO_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_10_SMAC_ADDR_LO_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_10_SMAC_ADDR_LO_MASK (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_10_SMAC_ADDR_LO_SHIFT)

#ifdef GENHDR_STRUCT
/* Lower 32bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_lo : 32; /* Source MAC Address bits [31:0]. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_10_t;
#endif /* GENHDR_STRUCT */

/* Higher 16bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_10                   0x000000F4U
/* Source MAC Address bits[47:32]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_10_SMAC_ADDR_HI_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_10_SMAC_ADDR_HI_WIDTH 16
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_10_SMAC_ADDR_HI_MASK (0xFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_10_SMAC_ADDR_HI_SHIFT)

#ifdef GENHDR_STRUCT
/* Higher 16bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_hi : 16; /* Source MAC Address bits[47:32]. */
    uint32_t : 16;
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_10_t;
#endif /* GENHDR_STRUCT */

/* Lower 32bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_11                   0x000000F8U
/* Source MAC Address bits [31:0]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_11_SMAC_ADDR_LO_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_11_SMAC_ADDR_LO_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_11_SMAC_ADDR_LO_MASK (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_11_SMAC_ADDR_LO_SHIFT)

#ifdef GENHDR_STRUCT
/* Lower 32bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_lo : 32; /* Source MAC Address bits [31:0]. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_11_t;
#endif /* GENHDR_STRUCT */

/* Higher 16bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_11                   0x000000FCU
/* Source MAC Address bits[47:32]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_11_SMAC_ADDR_HI_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_11_SMAC_ADDR_HI_WIDTH 16
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_11_SMAC_ADDR_HI_MASK (0xFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_11_SMAC_ADDR_HI_SHIFT)

#ifdef GENHDR_STRUCT
/* Higher 16bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_hi : 16; /* Source MAC Address bits[47:32]. */
    uint32_t : 16;
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_11_t;
#endif /* GENHDR_STRUCT */

/* Lower 32bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_12                   0x00000100U
/* Source MAC Address bits [31:0]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_12_SMAC_ADDR_LO_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_12_SMAC_ADDR_LO_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_12_SMAC_ADDR_LO_MASK (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_12_SMAC_ADDR_LO_SHIFT)

#ifdef GENHDR_STRUCT
/* Lower 32bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_lo : 32; /* Source MAC Address bits [31:0]. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_12_t;
#endif /* GENHDR_STRUCT */

/* Higher 16bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_12                   0x00000104U
/* Source MAC Address bits[47:32]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_12_SMAC_ADDR_HI_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_12_SMAC_ADDR_HI_WIDTH 16
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_12_SMAC_ADDR_HI_MASK (0xFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_12_SMAC_ADDR_HI_SHIFT)

#ifdef GENHDR_STRUCT
/* Higher 16bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_hi : 16; /* Source MAC Address bits[47:32]. */
    uint32_t : 16;
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_12_t;
#endif /* GENHDR_STRUCT */

/* Lower 32bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_13                   0x00000108U
/* Source MAC Address bits [31:0]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_13_SMAC_ADDR_LO_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_13_SMAC_ADDR_LO_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_13_SMAC_ADDR_LO_MASK (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_13_SMAC_ADDR_LO_SHIFT)

#ifdef GENHDR_STRUCT
/* Lower 32bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_lo : 32; /* Source MAC Address bits [31:0]. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_13_t;
#endif /* GENHDR_STRUCT */

/* Higher 16bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_13                   0x0000010CU
/* Source MAC Address bits[47:32]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_13_SMAC_ADDR_HI_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_13_SMAC_ADDR_HI_WIDTH 16
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_13_SMAC_ADDR_HI_MASK (0xFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_13_SMAC_ADDR_HI_SHIFT)

#ifdef GENHDR_STRUCT
/* Higher 16bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_hi : 16; /* Source MAC Address bits[47:32]. */
    uint32_t : 16;
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_13_t;
#endif /* GENHDR_STRUCT */

/* Lower 32bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_14                   0x00000110U
/* Source MAC Address bits [31:0]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_14_SMAC_ADDR_LO_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_14_SMAC_ADDR_LO_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_14_SMAC_ADDR_LO_MASK (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_14_SMAC_ADDR_LO_SHIFT)

#ifdef GENHDR_STRUCT
/* Lower 32bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_lo : 32; /* Source MAC Address bits [31:0]. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_14_t;
#endif /* GENHDR_STRUCT */

/* Higher 16bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_14                   0x00000114U
/* Source MAC Address bits[47:32]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_14_SMAC_ADDR_HI_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_14_SMAC_ADDR_HI_WIDTH 16
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_14_SMAC_ADDR_HI_MASK (0xFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_14_SMAC_ADDR_HI_SHIFT)

#ifdef GENHDR_STRUCT
/* Higher 16bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_hi : 16; /* Source MAC Address bits[47:32]. */
    uint32_t : 16;
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_14_t;
#endif /* GENHDR_STRUCT */

/* Lower 32bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_15                   0x00000118U
/* Source MAC Address bits [31:0]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_15_SMAC_ADDR_LO_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_15_SMAC_ADDR_LO_WIDTH 32
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_15_SMAC_ADDR_LO_MASK (0xFFFFFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_15_SMAC_ADDR_LO_SHIFT)

#ifdef GENHDR_STRUCT
/* Lower 32bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_lo : 32; /* Source MAC Address bits [31:0]. */
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_LO_15_t;
#endif /* GENHDR_STRUCT */

/* Higher 16bit of source MAC address */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_15                   0x0000011CU
/* Source MAC Address bits[47:32]. */
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_15_SMAC_ADDR_HI_SHIFT 0
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_15_SMAC_ADDR_HI_WIDTH 16
#define MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_15_SMAC_ADDR_HI_MASK (0xFFFFU << MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_15_SMAC_ADDR_HI_SHIFT)

#ifdef GENHDR_STRUCT
/* Higher 16bit of source MAC address */
typedef union {
  struct {
    uint32_t smac_addr_hi : 16; /* Source MAC Address bits[47:32]. */
    uint32_t : 16;
  } field;
  uint32_t val;
} MKS_VPORTLKUP_TUNNEL_TABLE_SMAC_TBL_REG_HI_15_t;
#endif /* GENHDR_STRUCT */

#endif /* _MKS_VPORTLKUP_TUNNEL_TABLE_REGISTERS_H_ */

