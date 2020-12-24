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

#ifndef _LKUP_REGISTERS_H_
#define _LKUP_REGISTERS_H_

/* EM Lookup Core */

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

#define LKUP_ACCESSTYPE                 GENHDR_ACCESSTYPE_RW /*  Default access type. Access types defined above. */
#define LKUP_REGWIDTH                   32 /* Default width of register in bits */
#define LKUP_ACCESSWIDTH                32 /* Default width of access word in bit */


/* Version number of the MB Lookup component */
#define LKUP_EMLC_VERSION                       0x00000000U
/*
 * Version format: DDMMYY_XX.
 *  Resets to 0x18101800.
 */
#define LKUP_EMLC_VERSION_VERSION_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define LKUP_EMLC_VERSION_VERSION_SHIFT         0
#define LKUP_EMLC_VERSION_VERSION_WIDTH         32
#define LKUP_EMLC_VERSION_VERSION_MASK          (0xFFFFFFFFU << LKUP_EMLC_VERSION_VERSION_SHIFT)

#ifdef GENHDR_STRUCT
/* Version number of the MB Lookup component */
typedef union {
  struct {
    uint32_t version : 32;      /* Version format: DDMMYY_XX.
                                   Resets to 0x18101800. */
  } field;
  uint32_t val;
} LKUP_EMLC_VERSION_t;
#endif /* GENHDR_STRUCT */

/* Test register to check read/write access */
#define LKUP_EMLC_SCRATCH                       0x00000004U
/*
 * Write and read back test data to verify register access.
 *  Resets to 0.
 */
#define LKUP_EMLC_SCRATCH_SCRATCH_SHIFT         0
#define LKUP_EMLC_SCRATCH_SCRATCH_WIDTH         32
#define LKUP_EMLC_SCRATCH_SCRATCH_MASK          (0xFFFFFFFFU << LKUP_EMLC_SCRATCH_SCRATCH_SHIFT)

#ifdef GENHDR_STRUCT
/* Test register to check read/write access */
typedef union {
  struct {
    uint32_t scratch : 32;      /* Write and read back test data to verify register access.
                                   Resets to 0. */
  } field;
  uint32_t val;
} LKUP_EMLC_SCRATCH_t;
#endif /* GENHDR_STRUCT */

/* Reserved */
#define LKUP_EMLC_GEN_CTRL                      0x00000018U
/* Reserved for future use. */
#define LKUP_EMLC_GEN_CTRL_RESERVED_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define LKUP_EMLC_GEN_CTRL_RESERVED_SHIFT       0
#define LKUP_EMLC_GEN_CTRL_RESERVED_WIDTH       32
#define LKUP_EMLC_GEN_CTRL_RESERVED_MASK        (0xFFFFFFFFU << LKUP_EMLC_GEN_CTRL_RESERVED_SHIFT)

#ifdef GENHDR_STRUCT
/* Reserved */
typedef union {
  struct {
    uint32_t reserved : 32;     /* Reserved for future use. */
  } field;
  uint32_t val;
} LKUP_EMLC_GEN_CTRL_t;
#endif /* GENHDR_STRUCT */

/* Management control register */
#define LKUP_EMLC_MGMT_CTRL                     0x00000020U
/*
 * management request type. The following commands are defined:
 *  0 - EMLC_FLUSH - Flush the EMLC lookup table.
 *  1 - EMLC_INSERT - Insert a key to the EMLC table.
 *  2 - EMLC_DELETE - Delete a key from the EMLC table.
 *  3 - EMLC_SEARCH - Find a key in the EMLC table.
 *  ::MBL ONLY:: 
 *  4 - EMLC_MODIFY_KEY - Modify result field of an entry, using a key to find
 * the entry.
 *  5 - EMLC_MODIFY_HANDLE - Modify result field of an entry, using a handle to
 * find the entry.
 *  6 - EMLC_DELETE_HANDLE - Delete an entry from the table, using a handle to
 * find the entry.
 *  7 - EMLC_SEARCH_HANDLE - Search for an entry in the table, using a handle to
 * find the entry.
 *  8 - MBL_INSERT_MODIFY - Insert new entry or modify existing entry. Check
 * infoCode when completed.
 *  Resets to 0. 
 */
#define LKUP_EMLC_MGMT_CTRL_REQ_TYPE_SHIFT      0
#define LKUP_EMLC_MGMT_CTRL_REQ_TYPE_WIDTH      4
#define LKUP_EMLC_MGMT_CTRL_REQ_TYPE_MASK       (0xFU << LKUP_EMLC_MGMT_CTRL_REQ_TYPE_SHIFT)

/*
 * Indicates whether a command has completed successfully (1).
 *  When .reqType is EMLC_SEARCH this bit indicates a match on the key that was
 * looked up.
 */
#define LKUP_EMLC_MGMT_CTRL_SUCCESS_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define LKUP_EMLC_MGMT_CTRL_SUCCESS_SHIFT       8
#define LKUP_EMLC_MGMT_CTRL_SUCCESS_WIDTH       1
#define LKUP_EMLC_MGMT_CTRL_SUCCESS_MASK        (0x1U << LKUP_EMLC_MGMT_CTRL_SUCCESS_SHIFT)

/*
 * Logical table for which the command is destined. Don't care for EMLC_FLUSH
 * command.
 */
#define LKUP_EMLC_MGMT_CTRL_TAB_SHIFT           16
#define LKUP_EMLC_MGMT_CTRL_TAB_WIDTH           4
#define LKUP_EMLC_MGMT_CTRL_TAB_MASK            (0xFU << LKUP_EMLC_MGMT_CTRL_TAB_SHIFT)

/*
 * ::MBL ONLY:: Info code returned after command has been completed. The
 * following info codes are defined:
 *  0 - MGMT_INFO_NONE - No additional information.
 *  1 - MGMT_INFO_INSERT_KEY_EXISTS - EMLC_INSERT_MODIFY command only: key
 * exists, result has been modified.
 *  Resets to 0. 
 */
#define LKUP_EMLC_MGMT_CTRL_INFO_CODE_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define LKUP_EMLC_MGMT_CTRL_INFO_CODE_SHIFT     24
#define LKUP_EMLC_MGMT_CTRL_INFO_CODE_WIDTH     2
#define LKUP_EMLC_MGMT_CTRL_INFO_CODE_MASK      (0x3U << LKUP_EMLC_MGMT_CTRL_INFO_CODE_SHIFT)

/*
 * ::MBL ONLY:: Error code returned after command has been completed. The
 * following error codes are defined:
 *  0 - MGMT_ERR_NONE - No error, operation successful.
 *  1 - MGMT_ERR_INSERT_ROW_FULL - insert failed due to no free bins in a row.
 *  2 - MGMT_ERR_INSERT_REHASH_FAIL - insert failed due to a signature clash,
 * unresolved by re-hashing.
 *  3 - MGMT_ERR_INSERT_KEY_EXISTS - insert failed - key already present in the
 * selected logical table.
 *  4 - MGMT_ERR_PTR_MGMT - pointer management error (fatal).
 *  5 - MGMT_ERR_INSERT_TABLE_FULL - insert failed - selected logical table
 * full.
 *  6 - MGMT_ERR_KEY_NOT_EXIST - delete/modify failed - key does not exist.
 *  7 - MGMT_ERR_INVALID_OPCODE - invalid opcode used in req_type.
 *  8 - MGMT_ERR_HASH_CHECKSUM - checksum error in Hash Table entry.
 *  9 - MGMT_ERR_KEY_CHECKSUM - checksum error in Key Table entry.
 *  Resets to 0. 
 */
#define LKUP_EMLC_MGMT_CTRL_ERR_CODE_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define LKUP_EMLC_MGMT_CTRL_ERR_CODE_SHIFT      27
#define LKUP_EMLC_MGMT_CTRL_ERR_CODE_WIDTH      4
#define LKUP_EMLC_MGMT_CTRL_ERR_CODE_MASK       (0xFU << LKUP_EMLC_MGMT_CTRL_ERR_CODE_SHIFT)

/*
 * Indicates whether the EMLC management is busy executing a command (1) or
 * ready to accept the next command (0).
 */
#define LKUP_EMLC_MGMT_CTRL_BUSY_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define LKUP_EMLC_MGMT_CTRL_BUSY_SHIFT          31
#define LKUP_EMLC_MGMT_CTRL_BUSY_WIDTH          1
#define LKUP_EMLC_MGMT_CTRL_BUSY_MASK           (0x1U << LKUP_EMLC_MGMT_CTRL_BUSY_SHIFT)

#ifdef GENHDR_STRUCT
/* Management control register */
typedef union {
  struct {
    uint32_t req_type : 4;      /* management request type. The following commands are defined:
                                   0 - EMLC_FLUSH - Flush the EMLC lookup table.
                                   1 - EMLC_INSERT - Insert a key to the EMLC table.
                                   2 - EMLC_DELETE - Delete a key from the EMLC table.
                                   3 - EMLC_SEARCH - Find a key in the EMLC table.
                                   ::MBL ONLY::
                                   4 - EMLC_MODIFY_KEY - Modify result field of an entry, using a key to find
                                   the entry.
                                   5 - EMLC_MODIFY_HANDLE - Modify result field of an entry, using a handle to
                                   find the entry.
                                   6 - EMLC_DELETE_HANDLE - Delete an entry from the table, using a handle to
                                   find the entry.
                                   7 - EMLC_SEARCH_HANDLE - Search for an entry in the table, using a handle to
                                   find the entry.
                                   8 - MBL_INSERT_MODIFY - Insert new entry or modify existing entry. Check
                                   infoCode when completed.
                                   Resets to 0. */
    uint32_t : 4;
    uint32_t success : 1;       /* Indicates whether a command has completed successfully (1).
                                   When .reqType is EMLC_SEARCH this bit indicates a match on the key that was
                                   looked up. */
    uint32_t : 7;
    uint32_t tab : 4;           /* Logical table for which the command is destined. Don't care for EMLC_FLUSH
                                   command. */
    uint32_t : 4;
    uint32_t info_code : 2;     /* ::MBL ONLY:: Info code returned after command has been completed. The
                                   following info codes are defined:
                                   0 - MGMT_INFO_NONE - No additional information.
                                   1 - MGMT_INFO_INSERT_KEY_EXISTS - EMLC_INSERT_MODIFY command only: key
                                   exists, result has been modified.
                                   Resets to 0. */
    uint32_t : 1;
    uint32_t err_code : 4;      /* ::MBL ONLY:: Error code returned after command has been completed. The
                                   following error codes are defined:
                                   0 - MGMT_ERR_NONE - No error, operation successful.
                                   1 - MGMT_ERR_INSERT_ROW_FULL - insert failed due to no free bins in a row.
                                   2 - MGMT_ERR_INSERT_REHASH_FAIL - insert failed due to a signature clash,
                                   unresolved by re-hashing.
                                   3 - MGMT_ERR_INSERT_KEY_EXISTS - insert failed - key already present in the
                                   selected logical table.
                                   4 - MGMT_ERR_PTR_MGMT - pointer management error (fatal).
                                   5 - MGMT_ERR_INSERT_TABLE_FULL - insert failed - selected logical table
                                   full.
                                   6 - MGMT_ERR_KEY_NOT_EXIST - delete/modify failed - key does not exist.
                                   7 - MGMT_ERR_INVALID_OPCODE - invalid opcode used in req_type.
                                   8 - MGMT_ERR_HASH_CHECKSUM - checksum error in Hash Table entry.
                                   9 - MGMT_ERR_KEY_CHECKSUM - checksum error in Key Table entry.
                                   Resets to 0. */
    uint32_t busy : 1;          /* Indicates whether the EMLC management is busy executing a command (1) or
                                   ready to accept the next command (0). */
  } field;
  uint32_t val;
} LKUP_EMLC_MGMT_CTRL_t;
#endif /* GENHDR_STRUCT */

/* ::MBL ONLY:: Key handle register */
#define LKUP_EMLC_KEY_HANDLE                    0x00000024U
/*
 * Register to which a value is written to by the user before executing
 * EMLC_SEARCH_HANDLE, EMLC_DELETE_HANDLE, EMLC_MODIFY_HANDLE commands.
 *  For the following commands: EMLC_INSERT, EMLC_SEARCH_KEY, EMLC_DELETE_KEY
 * and EMLC_MODIFY_KEY this register contains the handle corresponding to the
 * key used, and is written by the hardware management on successful completion
 * of the command execution.
 *  Resets to 0. 
 */
#define LKUP_EMLC_KEY_HANDLE_HANDLE_SHIFT       0
#define LKUP_EMLC_KEY_HANDLE_HANDLE_WIDTH       32
#define LKUP_EMLC_KEY_HANDLE_HANDLE_MASK        (0xFFFFFFFFU << LKUP_EMLC_KEY_HANDLE_HANDLE_SHIFT)

#ifdef GENHDR_STRUCT
/* ::MBL ONLY:: Key handle register */
typedef union {
  struct {
    uint32_t handle : 32;       /* Register to which a value is written to by the user before executing
                                   EMLC_SEARCH_HANDLE, EMLC_DELETE_HANDLE, EMLC_MODIFY_HANDLE commands.
                                   For the following commands: EMLC_INSERT, EMLC_SEARCH_KEY, EMLC_DELETE_KEY
                                   and EMLC_MODIFY_KEY this register contains the handle corresponding to the
                                   key used, and is written by the hardware management on successful completion
                                   of the command execution.
                                   Resets to 0. */
  } field;
  uint32_t val;
} LKUP_EMLC_KEY_HANDLE_t;
#endif /* GENHDR_STRUCT */

/* ::MBL ONLY:: Register controlling next handle requests */
#define LKUP_EMLC_NXT_HANDLE_REQ                0x00000028U
/*
 * Logical table for which the next free handle is requested.
 *  Requesting next pointer for the same logical table without requesting INSERT
 * KEY operation will result in the same pointer being returned.
 *  Resets to 0. 
 */
#define LKUP_EMLC_NXT_HANDLE_REQ_TAB_SHIFT      0
#define LKUP_EMLC_NXT_HANDLE_REQ_TAB_WIDTH      4
#define LKUP_EMLC_NXT_HANDLE_REQ_TAB_MASK       (0xFU << LKUP_EMLC_NXT_HANDLE_REQ_TAB_SHIFT)

/*
 * After writing to tab field, this bit indicates 1 (busy). When the bit shows
 * 0, this means that Valid bit and Next Handle can be read.
 *  Resets to 0.
 */
#define LKUP_EMLC_NXT_HANDLE_REQ_BUSY_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define LKUP_EMLC_NXT_HANDLE_REQ_BUSY_SHIFT     31
#define LKUP_EMLC_NXT_HANDLE_REQ_BUSY_WIDTH     1
#define LKUP_EMLC_NXT_HANDLE_REQ_BUSY_MASK      (0x1U << LKUP_EMLC_NXT_HANDLE_REQ_BUSY_SHIFT)

#ifdef GENHDR_STRUCT
/* ::MBL ONLY:: Register controlling next handle requests */
typedef union {
  struct {
    uint32_t tab : 4;   /* Logical table for which the next free handle is requested.
                           Requesting next pointer for the same logical table without requesting INSERT
                           KEY operation will result in the same pointer being returned.
                           Resets to 0. */
    uint32_t : 27;
    uint32_t busy : 1;  /* After writing to tab field, this bit indicates 1 (busy). When the bit shows
                           0, this means that Valid bit and Next Handle can be read.
                           Resets to 0. */
  } field;
  uint32_t val;
} LKUP_EMLC_NXT_HANDLE_REQ_t;
#endif /* GENHDR_STRUCT */

/* ::MBL ONLY:: Next handle register */
#define LKUP_EMLC_NXT_HANDLE                    0x0000002CU
/*
 * Register from which a value of the next free handle can be read.
 *  Subsequent reads return the same value until MBL_INSERT command is
 * successfully executed.
 *  Resets to the handle value which will be used for the first MBL_INSERT
 * operation for logical table 0. 
 */
#define LKUP_EMLC_NXT_HANDLE_HANDLE_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define LKUP_EMLC_NXT_HANDLE_HANDLE_SHIFT       0
#define LKUP_EMLC_NXT_HANDLE_HANDLE_WIDTH       31
#define LKUP_EMLC_NXT_HANDLE_HANDLE_MASK        (0x7FFFFFFFU << LKUP_EMLC_NXT_HANDLE_HANDLE_SHIFT)

/*
 * Indicates a valid next handle.
 *  Resets to 0.
 */
#define LKUP_EMLC_NXT_HANDLE_VALID_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define LKUP_EMLC_NXT_HANDLE_VALID_SHIFT        31
#define LKUP_EMLC_NXT_HANDLE_VALID_WIDTH        1
#define LKUP_EMLC_NXT_HANDLE_VALID_MASK         (0x1U << LKUP_EMLC_NXT_HANDLE_VALID_SHIFT)

#ifdef GENHDR_STRUCT
/* ::MBL ONLY:: Next handle register */
typedef union {
  struct {
    uint32_t handle : 31;       /* Register from which a value of the next free handle can be read.
                                   Subsequent reads return the same value until MBL_INSERT command is
                                   successfully executed.
                                   Resets to the handle value which will be used for the first MBL_INSERT
                                   operation for logical table 0. */
    uint32_t valid : 1;         /* Indicates a valid next handle.
                                   Resets to 0. */
  } field;
  uint32_t val;
} LKUP_EMLC_NXT_HANDLE_t;
#endif /* GENHDR_STRUCT */

/* Indicates the cause of a warning alarm */
#define LKUP_EMLC_WARNING_0                     0x00000040U
/*
 * Warning flags:
 *  wflag[0] - hash table checksum error.
 *  wflag[1] - key table checksum error.
 *  Resets to 0. 
 */
#define LKUP_EMLC_WARNING_0_WFLAG_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define LKUP_EMLC_WARNING_0_WFLAG_SHIFT         0
#define LKUP_EMLC_WARNING_0_WFLAG_WIDTH         2
#define LKUP_EMLC_WARNING_0_WFLAG_MASK          (0x3U << LKUP_EMLC_WARNING_0_WFLAG_SHIFT)

#ifdef GENHDR_STRUCT
/* Indicates the cause of a warning alarm */
typedef union {
  struct {
    uint32_t wflag : 2;         /* Warning flags:
                                   wflag[0] - hash table checksum error.
                                   wflag[1] - key table checksum error.
                                   Resets to 0. */
    uint32_t : 30;
  } field;
  uint32_t val;
} LKUP_EMLC_WARNING_0_t;
#endif /* GENHDR_STRUCT */

/* Hardware fatal error flags */
#define LKUP_EMLC_FATAL_0                       0x00000050U
/*
 * Fatal error flags:
 *  eflag[ 0] - err_key_mdata_fifo_full
 *  eflag[ 1] - err_key_mdata_fifo_empty
 *  eflag[ 2] - err_pre_hash_table_read_fifo_full
 *  eflag[ 3] - err_hash_table_pending_fifo_full
 *  eflag[ 4] - err_hash_table_pending_fifo_empty
 *  eflag[ 5] - err_hash_table_status_fifo_full
 *  eflag[ 6] - err_hash_table_status_fifo_empty
 *  eflag[ 7] - err_hash_table_rdata_fifo_full
 *  eflag[ 8] - err_hash_table_rdata_fifo_empty
 *  eflag[ 9] - err_pre_key_table_read_fifo_full
 *  eflag[10] - err_key_table_pending_fifo_full
 *  eflag[11] - err_key_table_pending_fifo_empty
 *  eflag[12] - err_key_table_rdata_fifo_full
 *  eflag[13] - err_key_table_rdata_fifo_empty
 *  eflag[14] - err_key_comp_pending_fifo_full
 *  eflag[15] - err_key_comp_pending_fifo_empty
 *  eflag[16] - err_lookup_resp_tab
 *  eflag[17] - mgmt_alarm
 *  eflag[18] - watchdog_alarm
 *  Resets to 0. 
 */
#define LKUP_EMLC_FATAL_0_EFLAG_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define LKUP_EMLC_FATAL_0_EFLAG_SHIFT           0
#define LKUP_EMLC_FATAL_0_EFLAG_WIDTH           13
#define LKUP_EMLC_FATAL_0_EFLAG_MASK            (0x1FFFU << LKUP_EMLC_FATAL_0_EFLAG_SHIFT)

#ifdef GENHDR_STRUCT
/* Hardware fatal error flags */
typedef union {
  struct {
    uint32_t eflag : 13;        /* Fatal error flags:
                                   eflag[ 0] - err_key_mdata_fifo_full
                                   eflag[ 1] - err_key_mdata_fifo_empty
                                   eflag[ 2] - err_pre_hash_table_read_fifo_full
                                   eflag[ 3] - err_hash_table_pending_fifo_full
                                   eflag[ 4] - err_hash_table_pending_fifo_empty
                                   eflag[ 5] - err_hash_table_status_fifo_full
                                   eflag[ 6] - err_hash_table_status_fifo_empty
                                   eflag[ 7] - err_hash_table_rdata_fifo_full
                                   eflag[ 8] - err_hash_table_rdata_fifo_empty
                                   eflag[ 9] - err_pre_key_table_read_fifo_full
                                   eflag[10] - err_key_table_pending_fifo_full
                                   eflag[11] - err_key_table_pending_fifo_empty
                                   eflag[12] - err_key_table_rdata_fifo_full
                                   eflag[13] - err_key_table_rdata_fifo_empty
                                   eflag[14] - err_key_comp_pending_fifo_full
                                   eflag[15] - err_key_comp_pending_fifo_empty
                                   eflag[16] - err_lookup_resp_tab
                                   eflag[17] - mgmt_alarm
                                   eflag[18] - watchdog_alarm
                                   Resets to 0. */
    uint32_t : 19;
  } field;
  uint32_t val;
} LKUP_EMLC_FATAL_0_t;
#endif /* GENHDR_STRUCT */

/* Provides various monitoring flags for the logic */
#define LKUP_EMLC_MON_0                 0x00000060U
/*
 * Monitoring flags:
 *  mflag[0] - external hash table request has been skipped at least once, due
 * to congestion
 *  mflag[1] - external key table request has been skipped at least once, due to
 * congestion
 *  ::MBL ONLY:: 
 *  mflag[2] - external key table request has been skipped at least once, due to
 * congestion
 *  Resets to 0. 
 */
#define LKUP_EMLC_MON_0_MFLAG_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define LKUP_EMLC_MON_0_MFLAG_SHIFT     0
#define LKUP_EMLC_MON_0_MFLAG_WIDTH     2
#define LKUP_EMLC_MON_0_MFLAG_MASK      (0x3U << LKUP_EMLC_MON_0_MFLAG_SHIFT)

#ifdef GENHDR_STRUCT
/* Provides various monitoring flags for the logic */
typedef union {
  struct {
    uint32_t mflag : 2;         /* Monitoring flags:
                                   mflag[0] - external hash table request has been skipped at least once, due
                                   to congestion
                                   mflag[1] - external key table request has been skipped at least once, due to
                                   congestion
                                   ::MBL ONLY::
                                   mflag[2] - external key table request has been skipped at least once, due to
                                   congestion
                                   Resets to 0. */
    uint32_t : 30;
  } field;
  uint32_t val;
} LKUP_EMLC_MON_0_t;
#endif /* GENHDR_STRUCT */

/* Total number of entries in EMLC table */
#define LKUP_EMLC_TOTAL_ENTRIES                                 0x00000080U
/* Shows the total number of entries inserted to the table. Resets to 0. */
#define LKUP_EMLC_TOTAL_ENTRIES_MBL_TOT_ENTRIES_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define LKUP_EMLC_TOTAL_ENTRIES_MBL_TOT_ENTRIES_SHIFT           0
#define LKUP_EMLC_TOTAL_ENTRIES_MBL_TOT_ENTRIES_WIDTH           32
#define LKUP_EMLC_TOTAL_ENTRIES_MBL_TOT_ENTRIES_MASK            (0xFFFFFFFFU << LKUP_EMLC_TOTAL_ENTRIES_MBL_TOT_ENTRIES_SHIFT)

#ifdef GENHDR_STRUCT
/* Total number of entries in EMLC table */
typedef union {
  struct {
    uint32_t mbl_tot_entries : 32;      /* Shows the total number of entries inserted to the table. Resets to 0. */
  } field;
  uint32_t val;
} LKUP_EMLC_TOTAL_ENTRIES_t;
#endif /* GENHDR_STRUCT */

/* Total number of entries in EMLC table */
#define LKUP_EMLC_TOTAL_REHASHES                                0x00000084U
/*
 * Shows the total number of re-hashes performed during insertions. Reset after
 * EMLC_FLUSH. Resets to 0.
 */
#define LKUP_EMLC_TOTAL_REHASHES_MBL_TOT_REHASHES_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define LKUP_EMLC_TOTAL_REHASHES_MBL_TOT_REHASHES_SHIFT         0
#define LKUP_EMLC_TOTAL_REHASHES_MBL_TOT_REHASHES_WIDTH         32
#define LKUP_EMLC_TOTAL_REHASHES_MBL_TOT_REHASHES_MASK          (0xFFFFFFFFU << LKUP_EMLC_TOTAL_REHASHES_MBL_TOT_REHASHES_SHIFT)

#ifdef GENHDR_STRUCT
/* Total number of entries in EMLC table */
typedef union {
  struct {
    uint32_t mbl_tot_rehashes : 32;     /* Shows the total number of re-hashes performed during insertions. Reset after
                                           EMLC_FLUSH. Resets to 0. */
  } field;
  uint32_t val;
} LKUP_EMLC_TOTAL_REHASHES_t;
#endif /* GENHDR_STRUCT */

/* ::MBL ONLY:: Total number of entries in EMLC table */
#define LKUP_EMLC_MAX_USED_BINS                         0x00000088U
/*
 * Maximum number of bins used in any row of the hash table. Reset after
 * EMLC_FLUSH. Resets to 0.
 */
#define LKUP_EMLC_MAX_USED_BINS_MAX_USED_BINS_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define LKUP_EMLC_MAX_USED_BINS_MAX_USED_BINS_SHIFT     0
#define LKUP_EMLC_MAX_USED_BINS_MAX_USED_BINS_WIDTH     32
#define LKUP_EMLC_MAX_USED_BINS_MAX_USED_BINS_MASK      (0xFFFFFFFFU << LKUP_EMLC_MAX_USED_BINS_MAX_USED_BINS_SHIFT)

#ifdef GENHDR_STRUCT
/* ::MBL ONLY:: Total number of entries in EMLC table */
typedef union {
  struct {
    uint32_t max_used_bins : 32;        /* Maximum number of bins used in any row of the hash table. Reset after
                                           EMLC_FLUSH. Resets to 0. */
  } field;
  uint32_t val;
} LKUP_EMLC_MAX_USED_BINS_t;
#endif /* GENHDR_STRUCT */

/* Register controlling statistics access */
#define LKUP_EMLC_STATS_CTRL                    0x00000090U
/*
 * Selects a counter from a group selected by grpSel. Resets to 0.
 *  If grpSel is 0, then the cntSel selects the logical table. Otherwise, cntSel
 * selects the management command,
 *  as defined in EMLC MGMT_CTRL register.
 *  Resets to 0. 
 */
#define LKUP_EMLC_STATS_CTRL_CNT_SEL_SHIFT      0
#define LKUP_EMLC_STATS_CTRL_CNT_SEL_WIDTH      8
#define LKUP_EMLC_STATS_CTRL_CNT_SEL_MASK       (0xFFU << LKUP_EMLC_STATS_CTRL_CNT_SEL_SHIFT)

/*
 * Selects a group of counters:
 *  0 - logical table entries
 *  1 - command minimum execution time (hardware debugging and optimisation
 * only)
 *  2 - command maximum execution time (hardware debugging and optimisation
 * only)
 *  Resets to 0.
 */
#define LKUP_EMLC_STATS_CTRL_GRP_SEL_SHIFT      8
#define LKUP_EMLC_STATS_CTRL_GRP_SEL_WIDTH      4
#define LKUP_EMLC_STATS_CTRL_GRP_SEL_MASK       (0xFU << LKUP_EMLC_STATS_CTRL_GRP_SEL_SHIFT)

/*
 * After writing to stage field, this bit indicates 1 (busy). When the bit shows
 * 0, this means the statistics counter can be read from EMLC_STAGE_ENTRIES
 * register.
 *  Resets to 0.
 */
#define LKUP_EMLC_STATS_CTRL_BUSY_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define LKUP_EMLC_STATS_CTRL_BUSY_SHIFT         31
#define LKUP_EMLC_STATS_CTRL_BUSY_WIDTH         1
#define LKUP_EMLC_STATS_CTRL_BUSY_MASK          (0x1U << LKUP_EMLC_STATS_CTRL_BUSY_SHIFT)

#ifdef GENHDR_STRUCT
/* Register controlling statistics access */
typedef union {
  struct {
    uint32_t cnt_sel : 8;       /* Selects a counter from a group selected by grpSel. Resets to 0.
                                   If grpSel is 0, then the cntSel selects the logical table. Otherwise, cntSel
                                   selects the management command,
                                   as defined in EMLC MGMT_CTRL register.
                                   Resets to 0. */
    uint32_t grp_sel : 4;       /* Selects a group of counters:
                                   0 - logical table entries
                                   1 - command minimum execution time (hardware debugging and optimisation
                                   only)
                                   2 - command maximum execution time (hardware debugging and optimisation
                                   only)
                                   Resets to 0. */
    uint32_t : 19;
    uint32_t busy : 1;          /* After writing to stage field, this bit indicates 1 (busy). When the bit shows
                                   0, this means the statistics counter can be read from EMLC_STAGE_ENTRIES
                                   register.
                                   Resets to 0. */
  } field;
  uint32_t val;
} LKUP_EMLC_STATS_CTRL_t;
#endif /* GENHDR_STRUCT */

/* Result of stats query */
#define LKUP_EMLC_STATS_RESULT                                  0x00000094U
/*
 * Result of stats query, in response to command sent using MHL_STATS_CTRL
 * register.
 *  Resets to 0.
 */
#define LKUP_EMLC_STATS_RESULT_MBL_STATS_RESULT_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define LKUP_EMLC_STATS_RESULT_MBL_STATS_RESULT_SHIFT           0
#define LKUP_EMLC_STATS_RESULT_MBL_STATS_RESULT_WIDTH           32
#define LKUP_EMLC_STATS_RESULT_MBL_STATS_RESULT_MASK            (0xFFFFFFFFU << LKUP_EMLC_STATS_RESULT_MBL_STATS_RESULT_SHIFT)

#ifdef GENHDR_STRUCT
/* Result of stats query */
typedef union {
  struct {
    uint32_t mbl_stats_result : 32;     /* Result of stats query, in response to command sent using MHL_STATS_CTRL
                                           register.
                                           Resets to 0. */
  } field;
  uint32_t val;
} LKUP_EMLC_STATS_RESULT_t;
#endif /* GENHDR_STRUCT */

/* ::MBL ONLY:: Maximum latency of search */
#define LKUP_EMLC_MAX_LKUP_LTNCY                                0x00000098U
/*
 * The maximum latency from the lookup request to the lookup response being
 * stored in the final FIFO.
 *  Reset after EMLC_FLUSH. Resets to 0.
 *  Intended for debug and optimisation only. 
 */
#define LKUP_EMLC_MAX_LKUP_LTNCY_MBL_MAX_LKUP_LAT_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define LKUP_EMLC_MAX_LKUP_LTNCY_MBL_MAX_LKUP_LAT_SHIFT         0
#define LKUP_EMLC_MAX_LKUP_LTNCY_MBL_MAX_LKUP_LAT_WIDTH         32
#define LKUP_EMLC_MAX_LKUP_LTNCY_MBL_MAX_LKUP_LAT_MASK          (0xFFFFFFFFU << LKUP_EMLC_MAX_LKUP_LTNCY_MBL_MAX_LKUP_LAT_SHIFT)

#ifdef GENHDR_STRUCT
/* ::MBL ONLY:: Maximum latency of search */
typedef union {
  struct {
    uint32_t mbl_max_lkup_lat : 32;     /* The maximum latency from the lookup request to the lookup response being
                                           stored in the final FIFO.
                                           Reset after EMLC_FLUSH. Resets to 0.
                                           Intended for debug and optimisation only. */
  } field;
  uint32_t val;
} LKUP_EMLC_MAX_LKUP_LTNCY_t;
#endif /* GENHDR_STRUCT */

/* ::MBL ONLY:: Maximum index of re-hashing polynomial */
#define LKUP_EMLC_MAX_REHASH_IDX                                0x0000009CU
/*
 * Maximum index of re-hashing polynomial used in the hash table. This is used
 * to choose the number polynomials used for re-hashing.
 *  Reset after EMLC_FLUSH. Resets to 0.
 *  Intended for debug and optimisation only. 
 */
#define LKUP_EMLC_MAX_REHASH_IDX_MBL_MAX_RH_INDEX_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define LKUP_EMLC_MAX_REHASH_IDX_MBL_MAX_RH_INDEX_SHIFT         0
#define LKUP_EMLC_MAX_REHASH_IDX_MBL_MAX_RH_INDEX_WIDTH         32
#define LKUP_EMLC_MAX_REHASH_IDX_MBL_MAX_RH_INDEX_MASK          (0xFFFFFFFFU << LKUP_EMLC_MAX_REHASH_IDX_MBL_MAX_RH_INDEX_SHIFT)

#ifdef GENHDR_STRUCT
/* ::MBL ONLY:: Maximum index of re-hashing polynomial */
typedef union {
  struct {
    uint32_t mbl_max_rh_index : 32;     /* Maximum index of re-hashing polynomial used in the hash table. This is used
                                           to choose the number polynomials used for re-hashing.
                                           Reset after EMLC_FLUSH. Resets to 0.
                                           Intended for debug and optimisation only. */
  } field;
  uint32_t val;
} LKUP_EMLC_MAX_REHASH_IDX_t;
#endif /* GENHDR_STRUCT */

/* The 1st Key register */
#define LKUP_EMLC_KEY_0                         0x00001000U
/*
 * If it is tunnel packet,it contains outer Dst Ip bits[31:0] else it contains
 * 32'h0.
 */
#define LKUP_EMLC_KEY_0_TUNNEL_DST_IP_SHIFT     0
#define LKUP_EMLC_KEY_0_TUNNEL_DST_IP_WIDTH     32
#define LKUP_EMLC_KEY_0_TUNNEL_DST_IP_MASK      (0xFFFFFFFFU << LKUP_EMLC_KEY_0_TUNNEL_DST_IP_SHIFT)

#ifdef GENHDR_STRUCT
/* The 1st Key register */
typedef union {
  struct {
    uint32_t tunnel_dst_ip : 32;        /* If it is tunnel packet,it contains outer Dst Ip bits[31:0] else it contains
                                           32'h0. */
  } field;
  uint32_t val;
} LKUP_EMLC_KEY_0_t;
#endif /* GENHDR_STRUCT */

/* The 2nd Key register */
#define LKUP_EMLC_KEY_1                         0x00001004U
/*
 * If it is tunnel packet,it contains outer Src Ip bits[31:0] else it contains
 * 32'h0.
 */
#define LKUP_EMLC_KEY_1_TUNNEL_SRC_IP_SHIFT     0
#define LKUP_EMLC_KEY_1_TUNNEL_SRC_IP_WIDTH     32
#define LKUP_EMLC_KEY_1_TUNNEL_SRC_IP_MASK      (0xFFFFFFFFU << LKUP_EMLC_KEY_1_TUNNEL_SRC_IP_SHIFT)

#ifdef GENHDR_STRUCT
/* The 2nd Key register */
typedef union {
  struct {
    uint32_t tunnel_src_ip : 32;        /* If it is tunnel packet,it contains outer Src Ip bits[31:0] else it contains
                                           32'h0. */
  } field;
  uint32_t val;
} LKUP_EMLC_KEY_1_t;
#endif /* GENHDR_STRUCT */

/* The 3rd Key register */
#define LKUP_EMLC_KEY_2                                 0x00001008U
/*
 * If it is tunnel packet,it contains outer ip protocol else it contains the
 * mask field 8'hff.
 */
#define LKUP_EMLC_KEY_2_TUNNEL_IP_PROTOCOL_SHIFT        0
#define LKUP_EMLC_KEY_2_TUNNEL_IP_PROTOCOL_WIDTH        8
#define LKUP_EMLC_KEY_2_TUNNEL_IP_PROTOCOL_MASK         (0xFFU << LKUP_EMLC_KEY_2_TUNNEL_IP_PROTOCOL_SHIFT)

/*
 * If it is tunnel packet,it contains Inner dot2q.
 *  If it is non-tunnel packet,it contains outer dot2q.
 */
#define LKUP_EMLC_KEY_2_TCI_VLAN_B_SHIFT                8
#define LKUP_EMLC_KEY_2_TCI_VLAN_B_WIDTH                16
#define LKUP_EMLC_KEY_2_TCI_VLAN_B_MASK                 (0xFFFFU << LKUP_EMLC_KEY_2_TCI_VLAN_B_SHIFT)

/*
 * If it is a tunnel packet,it contains Inner dot1q[7:0].
 *  If it is non-tunnel packet,it contains outer dot1q[7:0].
 */
#define LKUP_EMLC_KEY_2_TCI_VLAN_A0_SHIFT               24
#define LKUP_EMLC_KEY_2_TCI_VLAN_A0_WIDTH               8
#define LKUP_EMLC_KEY_2_TCI_VLAN_A0_MASK                (0xFFU << LKUP_EMLC_KEY_2_TCI_VLAN_A0_SHIFT)

#ifdef GENHDR_STRUCT
/* The 3rd Key register */
typedef union {
  struct {
    uint32_t tunnel_ip_protocol : 8;    /* If it is tunnel packet,it contains outer ip protocol else it contains the
                                           mask field 8'hff. */
    uint32_t tci_vlan_b : 16;           /* If it is tunnel packet,it contains Inner dot2q.
                                           If it is non-tunnel packet,it contains outer dot2q. */
    uint32_t tci_vlan_a0 : 8;           /* If it is a tunnel packet,it contains Inner dot1q[7:0].
                                           If it is non-tunnel packet,it contains outer dot1q[7:0]. */
  } field;
  uint32_t val;
} LKUP_EMLC_KEY_2_t;
#endif /* GENHDR_STRUCT */

/* The 4th Key register */
#define LKUP_EMLC_KEY_3                                 0x0000100CU
/*
 * If it is a tunnel packet,it contains Inner dot1q[15:8].
 *  If it is non-tunnel packet,it contains outer dot1q[15:8].
 */
#define LKUP_EMLC_KEY_3_TCI_VLAN_A1_SHIFT               0
#define LKUP_EMLC_KEY_3_TCI_VLAN_A1_WIDTH               8
#define LKUP_EMLC_KEY_3_TCI_VLAN_A1_MASK                (0xFFU << LKUP_EMLC_KEY_3_TCI_VLAN_A1_SHIFT)

/*
 * If it is tunnel packet, Vxlan : vxlan_id[23:0] 
 *  MPLS : {S,TC[2:0],MPLS label[19:0]} 
 *  GRE : gre_key[23:0] 
 *  If it is non-tunnel packet,it is 0.
 */
#define LKUP_EMLC_KEY_3_VXLNID_TEID_KEY_0_SHIFT         8
#define LKUP_EMLC_KEY_3_VXLNID_TEID_KEY_0_WIDTH         24
#define LKUP_EMLC_KEY_3_VXLNID_TEID_KEY_0_MASK          (0xFFFFFFU << LKUP_EMLC_KEY_3_VXLNID_TEID_KEY_0_SHIFT)

#ifdef GENHDR_STRUCT
/* The 4th Key register */
typedef union {
  struct {
    uint32_t tci_vlan_a1 : 8;           /* If it is a tunnel packet,it contains Inner dot1q[15:8].
                                           If it is non-tunnel packet,it contains outer dot1q[15:8]. */
    uint32_t vxlnid_teid_key_0 : 24;    /* If it is tunnel packet, Vxlan : vxlan_id[23:0]
                                           MPLS : {S,TC[2:0],MPLS label[19:0]}
                                           GRE : gre_key[23:0]
                                           If it is non-tunnel packet,it is 0. */
  } field;
  uint32_t val;
} LKUP_EMLC_KEY_3_t;
#endif /* GENHDR_STRUCT */

/* The 5th Key register */
#define LKUP_EMLC_KEY_4                                 0x00001010U
/*
 * If it is tunnel packet, Vxlan : 8'b0
 *  MPLS : 8'hff
 *  GRE : gre_key[31:24] 
 *  If it is non-tunnel packet,it is 0.
 */
#define LKUP_EMLC_KEY_4_VXLNID_TEID_KEY_1_SHIFT         0
#define LKUP_EMLC_KEY_4_VXLNID_TEID_KEY_1_WIDTH         8
#define LKUP_EMLC_KEY_4_VXLNID_TEID_KEY_1_MASK          (0xFFU << LKUP_EMLC_KEY_4_VXLNID_TEID_KEY_1_SHIFT)

/*
 * If it is tunnel packet,it contains inner Dst Ip bits[23:0].
 *  when enable_arp=1,it is ARP mode.It contains arp_1_hdr.tip bits[23:0].
 *  If it is non-tunnel packet,it contains outer Dst Ip bits[23:0].
 *  When enable_arp=1,it is ARP mode.It contains arp_0_hdr.tip bits[23:0].
 */
#define LKUP_EMLC_KEY_4_DST_IP_0_SHIFT                  8
#define LKUP_EMLC_KEY_4_DST_IP_0_WIDTH                  24
#define LKUP_EMLC_KEY_4_DST_IP_0_MASK                   (0xFFFFFFU << LKUP_EMLC_KEY_4_DST_IP_0_SHIFT)

#ifdef GENHDR_STRUCT
/* The 5th Key register */
typedef union {
  struct {
    uint32_t vxlnid_teid_key_1 : 8;     /* If it is tunnel packet, Vxlan : 8'b0
                                           MPLS : 8'hff
                                           GRE : gre_key[31:24]
                                           If it is non-tunnel packet,it is 0. */
    uint32_t dst_ip_0 : 24;             /* If it is tunnel packet,it contains inner Dst Ip bits[23:0].
                                           when enable_arp=1,it is ARP mode.It contains arp_1_hdr.tip bits[23:0].
                                           If it is non-tunnel packet,it contains outer Dst Ip bits[23:0].
                                           When enable_arp=1,it is ARP mode.It contains arp_0_hdr.tip bits[23:0]. */
  } field;
  uint32_t val;
} LKUP_EMLC_KEY_4_t;
#endif /* GENHDR_STRUCT */

/* The 6th Key register */
#define LKUP_EMLC_KEY_5                         0x00001014U
/*
 * If it is tunnel packet,it contains inner Dst Ip bits[55:24].
 *  when enable_arp=1,it is ARP mode.It contains arp_1_hdr.tip bits[31:24] and
 * arp_1_hdr.tha bits[23:0]
 *  {arp_1_hdr.tha[23:0],arp_1_hdr.tip[31:24]}.
 *  If it is non-tunnel packet,it contains outer Dst Ip bits[55:24]. when
 * enable_arp=1,it is ARP mode.It contains arp_0_hdr.tip bits[31:24] and
 * arp_0_hdr.tha bits[23:0]
 *  {arp_0_hdr.tha[23:0],arp_0_hdr.tip[31:24]}.
 */
#define LKUP_EMLC_KEY_5_DST_IP_1_SHIFT          0
#define LKUP_EMLC_KEY_5_DST_IP_1_WIDTH          32
#define LKUP_EMLC_KEY_5_DST_IP_1_MASK           (0xFFFFFFFFU << LKUP_EMLC_KEY_5_DST_IP_1_SHIFT)

#ifdef GENHDR_STRUCT
/* The 6th Key register */
typedef union {
  struct {
    uint32_t dst_ip_1 : 32;     /* If it is tunnel packet,it contains inner Dst Ip bits[55:24].
                                   when enable_arp=1,it is ARP mode.It contains arp_1_hdr.tip bits[31:24] and
                                   arp_1_hdr.tha bits[23:0]
                                   {arp_1_hdr.tha[23:0],arp_1_hdr.tip[31:24]}.
                                   If it is non-tunnel packet,it contains outer Dst Ip bits[55:24]. when
                                   enable_arp=1,it is ARP mode.It contains arp_0_hdr.tip bits[31:24] and
                                   arp_0_hdr.tha bits[23:0]
                                   {arp_0_hdr.tha[23:0],arp_0_hdr.tip[31:24]}. */
  } field;
  uint32_t val;
} LKUP_EMLC_KEY_5_t;
#endif /* GENHDR_STRUCT */

/* The 7th Key register */
#define LKUP_EMLC_KEY_6                         0x00001018U
/*
 * If it is tunnel packet,it contains inner Dst Ip bits[87:56].
 *  when enable_arp=1,it is ARP mode.It contains arp_1_hdr.tha bits[47:24]
 *  {8'h0,arp_1_hdr.tha[47:24]}.
 *  If it is non-tunnel packet,it contains outer Dst Ip bits[87:56]. when
 * enable_arp=1,it is ARP mode.It contains arp_0_hdr.tha bits[47:24]
 *  {8'h0,arp_0_hdr.tha[47:24]}.
 */
#define LKUP_EMLC_KEY_6_DST_IP_2_SHIFT          0
#define LKUP_EMLC_KEY_6_DST_IP_2_WIDTH          32
#define LKUP_EMLC_KEY_6_DST_IP_2_MASK           (0xFFFFFFFFU << LKUP_EMLC_KEY_6_DST_IP_2_SHIFT)

#ifdef GENHDR_STRUCT
/* The 7th Key register */
typedef union {
  struct {
    uint32_t dst_ip_2 : 32;     /* If it is tunnel packet,it contains inner Dst Ip bits[87:56].
                                   when enable_arp=1,it is ARP mode.It contains arp_1_hdr.tha bits[47:24]
                                   {8'h0,arp_1_hdr.tha[47:24]}.
                                   If it is non-tunnel packet,it contains outer Dst Ip bits[87:56]. when
                                   enable_arp=1,it is ARP mode.It contains arp_0_hdr.tha bits[47:24]
                                   {8'h0,arp_0_hdr.tha[47:24]}. */
  } field;
  uint32_t val;
} LKUP_EMLC_KEY_6_t;
#endif /* GENHDR_STRUCT */

/* The 8th Key register */
#define LKUP_EMLC_KEY_7                         0x0000101CU
/*
 * If it is tunnel packet,it contains inner Dst Ip bits[119:88].
 *  when enable_arp=1,it is ARP mode.It contains 32'h0.
 *  If it is non-tunnel packet,it contains outer Dst Ip bits[119:88].
 *  when enable_arp=1,it is ARP mode.It contains 32'h0.
 */
#define LKUP_EMLC_KEY_7_DST_IP_3_SHIFT          0
#define LKUP_EMLC_KEY_7_DST_IP_3_WIDTH          32
#define LKUP_EMLC_KEY_7_DST_IP_3_MASK           (0xFFFFFFFFU << LKUP_EMLC_KEY_7_DST_IP_3_SHIFT)

#ifdef GENHDR_STRUCT
/* The 8th Key register */
typedef union {
  struct {
    uint32_t dst_ip_3 : 32;     /* If it is tunnel packet,it contains inner Dst Ip bits[119:88].
                                   when enable_arp=1,it is ARP mode.It contains 32'h0.
                                   If it is non-tunnel packet,it contains outer Dst Ip bits[119:88].
                                   when enable_arp=1,it is ARP mode.It contains 32'h0. */
  } field;
  uint32_t val;
} LKUP_EMLC_KEY_7_t;
#endif /* GENHDR_STRUCT */

/* The 9th Key register */
#define LKUP_EMLC_KEY_8                         0x00001020U
/*
 * If it is tunnel packet,it contains inner Dst Ip bits[127:120].
 *  when enable_arp=1,it is ARP mode.It contains 8'h0.
 *  If it is non-tunnel packet,it contains outer Dst Ip bits[127:120].
 *  when enable_arp=1,it is ARP mode.It contains 8'h0.
 */
#define LKUP_EMLC_KEY_8_DST_IP_4_SHIFT          0
#define LKUP_EMLC_KEY_8_DST_IP_4_WIDTH          8
#define LKUP_EMLC_KEY_8_DST_IP_4_MASK           (0xFFU << LKUP_EMLC_KEY_8_DST_IP_4_SHIFT)

/*
 * If it is tunnel packet,it contains inner Src Ip bits[23:0].
 *  When enable_arp=1,it is ARP mode.It contains arp_1_hdr.sip bits[23:0].
 *  If it is non-tunnel packet,it contains outer Src Ip bits[23:0].
 *  When enable_arp=1,it is ARP mode.It contains arp_0_hdr.sip bits[23:0].
 */
#define LKUP_EMLC_KEY_8_SRC_IP_0_SHIFT          8
#define LKUP_EMLC_KEY_8_SRC_IP_0_WIDTH          24
#define LKUP_EMLC_KEY_8_SRC_IP_0_MASK           (0xFFFFFFU << LKUP_EMLC_KEY_8_SRC_IP_0_SHIFT)

#ifdef GENHDR_STRUCT
/* The 9th Key register */
typedef union {
  struct {
    uint32_t dst_ip_4 : 8;      /* If it is tunnel packet,it contains inner Dst Ip bits[127:120].
                                   when enable_arp=1,it is ARP mode.It contains 8'h0.
                                   If it is non-tunnel packet,it contains outer Dst Ip bits[127:120].
                                   when enable_arp=1,it is ARP mode.It contains 8'h0. */
    uint32_t src_ip_0 : 24;     /* If it is tunnel packet,it contains inner Src Ip bits[23:0].
                                   When enable_arp=1,it is ARP mode.It contains arp_1_hdr.sip bits[23:0].
                                   If it is non-tunnel packet,it contains outer Src Ip bits[23:0].
                                   When enable_arp=1,it is ARP mode.It contains arp_0_hdr.sip bits[23:0]. */
  } field;
  uint32_t val;
} LKUP_EMLC_KEY_8_t;
#endif /* GENHDR_STRUCT */

/* The 10th Key register */
#define LKUP_EMLC_KEY_9                         0x00001024U
/*
 * If it is tunnel packet,it contains inner Src Ip bits[55:24].
 *  When enable_arp=1,it is ARP mode.It contains arp_1_hdr.sip bits[31:24] and
 * arp_1_hdr.sha bits[23:0]
 *  {arp_1_hdr.sha[23:0],arp_1_hdr.sip[31:24]}.
 *  If it is non-tunnel packet,it contains outer Src Ip bits[55:24].
 *  When enable_arp=1,it is ARP mode.It contains arp_0_hdr.sip bits[31:24] and
 * arp_0_hdr.sha bits[23:0]
 *  {arp_0_hdr.sha[23:0],arp_0_hdr.sip[31:24]}.
 */
#define LKUP_EMLC_KEY_9_SRC_IP_1_SHIFT          0
#define LKUP_EMLC_KEY_9_SRC_IP_1_WIDTH          32
#define LKUP_EMLC_KEY_9_SRC_IP_1_MASK           (0xFFFFFFFFU << LKUP_EMLC_KEY_9_SRC_IP_1_SHIFT)

#ifdef GENHDR_STRUCT
/* The 10th Key register */
typedef union {
  struct {
    uint32_t src_ip_1 : 32;     /* If it is tunnel packet,it contains inner Src Ip bits[55:24].
                                   When enable_arp=1,it is ARP mode.It contains arp_1_hdr.sip bits[31:24] and
                                   arp_1_hdr.sha bits[23:0]
                                   {arp_1_hdr.sha[23:0],arp_1_hdr.sip[31:24]}.
                                   If it is non-tunnel packet,it contains outer Src Ip bits[55:24].
                                   When enable_arp=1,it is ARP mode.It contains arp_0_hdr.sip bits[31:24] and
                                   arp_0_hdr.sha bits[23:0]
                                   {arp_0_hdr.sha[23:0],arp_0_hdr.sip[31:24]}. */
  } field;
  uint32_t val;
} LKUP_EMLC_KEY_9_t;
#endif /* GENHDR_STRUCT */

/* The 11th Key register */
#define LKUP_EMLC_KEY_10                        0x00001028U
/*
 * If it is tunnel packet,it contains inner Src Ip bits[87:56].
 *  When enable_arp=1,it is ARP mode.It contains arp_1_hdr.opcode bits[7:0] and
 * arp_1_hdr.sha[47:24].
 *  {arp_1_hdr.opcode[7:0],arp_1_hdr.sha[47:24]}.
 *  If it is non-tunnel packet,it contains outer Src Ip bits[87:56].
 *  When enable_arp=1,it is ARP mode.It contains arp_0_hdr.opcode bits[7:0] and
 * arp_1_hdr.sha[47:24].
 *  {arp_0_hdr.opcode[7:0],arp_0_hdr.sha[47:24]}.
 */
#define LKUP_EMLC_KEY_10_SRC_IP_2_SHIFT         0
#define LKUP_EMLC_KEY_10_SRC_IP_2_WIDTH         32
#define LKUP_EMLC_KEY_10_SRC_IP_2_MASK          (0xFFFFFFFFU << LKUP_EMLC_KEY_10_SRC_IP_2_SHIFT)

#ifdef GENHDR_STRUCT
/* The 11th Key register */
typedef union {
  struct {
    uint32_t src_ip_2 : 32;     /* If it is tunnel packet,it contains inner Src Ip bits[87:56].
                                   When enable_arp=1,it is ARP mode.It contains arp_1_hdr.opcode bits[7:0] and
                                   arp_1_hdr.sha[47:24].
                                   {arp_1_hdr.opcode[7:0],arp_1_hdr.sha[47:24]}.
                                   If it is non-tunnel packet,it contains outer Src Ip bits[87:56].
                                   When enable_arp=1,it is ARP mode.It contains arp_0_hdr.opcode bits[7:0] and
                                   arp_1_hdr.sha[47:24].
                                   {arp_0_hdr.opcode[7:0],arp_0_hdr.sha[47:24]}. */
  } field;
  uint32_t val;
} LKUP_EMLC_KEY_10_t;
#endif /* GENHDR_STRUCT */

/* The 12th Key register */
#define LKUP_EMLC_KEY_11                        0x0000102CU
/*
 * If it is tunnel packet,it contains inner Src Ip bits[119:88].
 *  When enable_arp=1,it is ARP mode.It contains arp_1_hdr.opcode bits[7:0].
 *  {24'h0,arp_1_hdr.opcode[15:8]}.
 *  If it is non-tunnel packet,it contains outer Src Ip bits[119:88].
 *  When enable_arp=1,it is ARP mode.It contains arp_0_hdr.opcode bits[7:0].
 *  {24'h0,arp_0_hdr.opcode[15:8]}.
 */
#define LKUP_EMLC_KEY_11_SRC_IP_3_SHIFT         0
#define LKUP_EMLC_KEY_11_SRC_IP_3_WIDTH         32
#define LKUP_EMLC_KEY_11_SRC_IP_3_MASK          (0xFFFFFFFFU << LKUP_EMLC_KEY_11_SRC_IP_3_SHIFT)

#ifdef GENHDR_STRUCT
/* The 12th Key register */
typedef union {
  struct {
    uint32_t src_ip_3 : 32;     /* If it is tunnel packet,it contains inner Src Ip bits[119:88].
                                   When enable_arp=1,it is ARP mode.It contains arp_1_hdr.opcode bits[7:0].
                                   {24'h0,arp_1_hdr.opcode[15:8]}.
                                   If it is non-tunnel packet,it contains outer Src Ip bits[119:88].
                                   When enable_arp=1,it is ARP mode.It contains arp_0_hdr.opcode bits[7:0].
                                   {24'h0,arp_0_hdr.opcode[15:8]}. */
  } field;
  uint32_t val;
} LKUP_EMLC_KEY_11_t;
#endif /* GENHDR_STRUCT */

/* The 13th Key register */
#define LKUP_EMLC_KEY_12                        0x00001030U
/*
 * If it is tunnel packet,it contains inner Src Ip bits[127:120].
 *  When enable_arp=1,it is ARP mode.It contains 8'h0
 *  If it is non-tunnel packet,it contains outer Src Ip bits[127:120].
 *  When enable_arp=1,it is ARP mode.It contains 8'h0.
 */
#define LKUP_EMLC_KEY_12_SRC_IP_4_SHIFT         0
#define LKUP_EMLC_KEY_12_SRC_IP_4_WIDTH         8
#define LKUP_EMLC_KEY_12_SRC_IP_4_MASK          (0xFFU << LKUP_EMLC_KEY_12_SRC_IP_4_SHIFT)

/*
 * If it is tunnel packet,it contains inner Dst Port bits[15:0].
 *  If it is non-tunnel packet,it contains outer Dst Port bits [15:0].
 */
#define LKUP_EMLC_KEY_12_DST_PORT_SHIFT         8
#define LKUP_EMLC_KEY_12_DST_PORT_WIDTH         16
#define LKUP_EMLC_KEY_12_DST_PORT_MASK          (0xFFFFU << LKUP_EMLC_KEY_12_DST_PORT_SHIFT)

/*
 * If it is tunnel packet,it contains inner Src Port bits[7:0].
 *  If it is non-tunnel packet,it contains outer Src Port bits[7:0].
 */
#define LKUP_EMLC_KEY_12_SRC_PORT_0_SHIFT       24
#define LKUP_EMLC_KEY_12_SRC_PORT_0_WIDTH       8
#define LKUP_EMLC_KEY_12_SRC_PORT_0_MASK        (0xFFU << LKUP_EMLC_KEY_12_SRC_PORT_0_SHIFT)

#ifdef GENHDR_STRUCT
/* The 13th Key register */
typedef union {
  struct {
    uint32_t src_ip_4 : 8;      /* If it is tunnel packet,it contains inner Src Ip bits[127:120].
                                   When enable_arp=1,it is ARP mode.It contains 8'h0
                                   If it is non-tunnel packet,it contains outer Src Ip bits[127:120].
                                   When enable_arp=1,it is ARP mode.It contains 8'h0. */
    uint32_t dst_port : 16;     /* If it is tunnel packet,it contains inner Dst Port bits[15:0].
                                   If it is non-tunnel packet,it contains outer Dst Port bits [15:0]. */
    uint32_t src_port_0 : 8;    /* If it is tunnel packet,it contains inner Src Port bits[7:0].
                                   If it is non-tunnel packet,it contains outer Src Port bits[7:0]. */
  } field;
  uint32_t val;
} LKUP_EMLC_KEY_12_t;
#endif /* GENHDR_STRUCT */

/* The 14th Key register */
#define LKUP_EMLC_KEY_13                        0x00001034U
/*
 * If it is tunnel packet,it contains inner Src Port bits[15:8].
 *  If it is non-tunnel packet,it contains outer Src Port bits[15:8].
 */
#define LKUP_EMLC_KEY_13_SRC_PORT_1_SHIFT       0
#define LKUP_EMLC_KEY_13_SRC_PORT_1_WIDTH       8
#define LKUP_EMLC_KEY_13_SRC_PORT_1_MASK        (0xFFU << LKUP_EMLC_KEY_13_SRC_PORT_1_SHIFT)

/*
 * If it is tunnel packet,it contains inner ip protocol.
 *  If it is non-tunnel packet,it contains outer ip protocol.
 */
#define LKUP_EMLC_KEY_13_IP_PROTOCOL_SHIFT      8
#define LKUP_EMLC_KEY_13_IP_PROTOCOL_WIDTH      8
#define LKUP_EMLC_KEY_13_IP_PROTOCOL_MASK       (0xFFU << LKUP_EMLC_KEY_13_IP_PROTOCOL_SHIFT)

/*
 * Flags are parseFlag[15:12]: is_tunnel[3:0] - set if packet is tunnel,it has
 * vxlan/gtpu/gre/mpls headers.
 *  0:No tunnel, 1:vxlan, 2:IpoverGre, 3:geneve,
 *  4:gtpu, 5:EthoverGre, 6:MplsoverUdp, 7:MplsoverGre.
 *  parseFlag[11:10]: is_fragment[1:0] - Currently not used. 0:No frag, 1:HF,
 * 3:NHF.
 *  parseFlag[9:6] : etype[3:0] - If it is Tunnel packet,it has Inner layer
 * Ethertype.
 *  If it is Non-Tunnel packet,it has Outer layer Ethertype.
 *  1:IPV4, 2:ARP, 3:IPV6, 0:If any other ethertype.
 *  parseFlag[5] : A_B toggle - 1:A, 0:B.
 *  parseFlag[4:2] : rsvd - Reserved bits.
 *  parseFlag[1] : is_recirc - set if recirc is enable.
 *  parseFlag[0] : gre_version - 0:version0, 1:version1.
 * .
 */
#define LKUP_EMLC_KEY_13_PARSE_FLAG_SHIFT       16
#define LKUP_EMLC_KEY_13_PARSE_FLAG_WIDTH       16
#define LKUP_EMLC_KEY_13_PARSE_FLAG_MASK        (0xFFFFU << LKUP_EMLC_KEY_13_PARSE_FLAG_SHIFT)

#ifdef GENHDR_STRUCT
/* The 14th Key register */
typedef union {
  struct {
    uint32_t src_port_1 : 8;    /* If it is tunnel packet,it contains inner Src Port bits[15:8].
                                   If it is non-tunnel packet,it contains outer Src Port bits[15:8]. */
    uint32_t ip_protocol : 8;   /* If it is tunnel packet,it contains inner ip protocol.
                                   If it is non-tunnel packet,it contains outer ip protocol. */
    uint32_t parse_flag : 16;   /* Flags are parseFlag[15:12]: is_tunnel[3:0] - set if packet is tunnel,it has
                                   vxlan/gtpu/gre/mpls headers.
                                   0:No tunnel, 1:vxlan, 2:IpoverGre, 3:geneve,
                                   4:gtpu, 5:EthoverGre, 6:MplsoverUdp, 7:MplsoverGre.
                                   parseFlag[11:10]: is_fragment[1:0] - Currently not used. 0:No frag, 1:HF,
                                   3:NHF.
                                   parseFlag[9:6] : etype[3:0] - If it is Tunnel packet,it has Inner layer
                                   Ethertype.
                                   If it is Non-Tunnel packet,it has Outer layer Ethertype.
                                   1:IPV4, 2:ARP, 3:IPV6, 0:If any other ethertype.
                                   parseFlag[5] : A_B toggle - 1:A, 0:B.
                                   parseFlag[4:2] : rsvd - Reserved bits.
                                   parseFlag[1] : is_recirc - set if recirc is enable.
                                   parseFlag[0] : gre_version - 0:version0, 1:version1.
                                   . */
  } field;
  uint32_t val;
} LKUP_EMLC_KEY_13_t;
#endif /* GENHDR_STRUCT */

/* The 15th Key register */
#define LKUP_EMLC_KEY_14                        0x00001038U
/*
 * If it is tunnel packet,it contains Inner Dst Mac bits[31:0].
 *  If it is non-tunnel packet,it contains Outer Dst Mac bits[31:0].
 */
#define LKUP_EMLC_KEY_14_DST_MAC_0_SHIFT        0
#define LKUP_EMLC_KEY_14_DST_MAC_0_WIDTH        32
#define LKUP_EMLC_KEY_14_DST_MAC_0_MASK         (0xFFFFFFFFU << LKUP_EMLC_KEY_14_DST_MAC_0_SHIFT)

#ifdef GENHDR_STRUCT
/* The 15th Key register */
typedef union {
  struct {
    uint32_t dst_mac_0 : 32;    /* If it is tunnel packet,it contains Inner Dst Mac bits[31:0].
                                   If it is non-tunnel packet,it contains Outer Dst Mac bits[31:0]. */
  } field;
  uint32_t val;
} LKUP_EMLC_KEY_14_t;
#endif /* GENHDR_STRUCT */

/* The 16th Key register */
#define LKUP_EMLC_KEY_15                        0x0000103CU
/*
 * If it is tunnel packet,it contains Inner Dst Mac bits[47:32].
 *  If it is non-tunnel packet,it contains Outer Dst Mac bits[47:32].
 */
#define LKUP_EMLC_KEY_15_DST_MAC_1_SHIFT        0
#define LKUP_EMLC_KEY_15_DST_MAC_1_WIDTH        16
#define LKUP_EMLC_KEY_15_DST_MAC_1_MASK         (0xFFFFU << LKUP_EMLC_KEY_15_DST_MAC_1_SHIFT)

/*
 * If it is tunnel packet,it contains Inner Src Mac bits[15:0].
 *  If it is non-tunnel packet,it contains Outer Src Mac bits[15:0].
 *  When recirculation is enabled,it contains recirculation id[15:0].
 */
#define LKUP_EMLC_KEY_15_SRC_MAC_0_SHIFT        16
#define LKUP_EMLC_KEY_15_SRC_MAC_0_WIDTH        16
#define LKUP_EMLC_KEY_15_SRC_MAC_0_MASK         (0xFFFFU << LKUP_EMLC_KEY_15_SRC_MAC_0_SHIFT)

#ifdef GENHDR_STRUCT
/* The 16th Key register */
typedef union {
  struct {
    uint32_t dst_mac_1 : 16;    /* If it is tunnel packet,it contains Inner Dst Mac bits[47:32].
                                   If it is non-tunnel packet,it contains Outer Dst Mac bits[47:32]. */
    uint32_t src_mac_0 : 16;    /* If it is tunnel packet,it contains Inner Src Mac bits[15:0].
                                   If it is non-tunnel packet,it contains Outer Src Mac bits[15:0].
                                   When recirculation is enabled,it contains recirculation id[15:0]. */
  } field;
  uint32_t val;
} LKUP_EMLC_KEY_15_t;
#endif /* GENHDR_STRUCT */

/* The 17th Key register */
#define LKUP_EMLC_KEY_16                        0x00001040U
/*
 * If it is tunnel packet,it contains Inner Src Mac bits[47:16].
 *  If it is non-tunnel packet,it contains Outer Src Mac bits[47:16].
 *  When recirculation is enabled,it contains {24'b0,recirculation id[23:16]}.
 *  NOTE: recircId=24 bits. SW just requires to match on recircId field and zero
 * out other key fields. So will reuse src_mac lower 3Bytes for putting
 * recircId.
 */
#define LKUP_EMLC_KEY_16_SRC_MAC_1_SHIFT        0
#define LKUP_EMLC_KEY_16_SRC_MAC_1_WIDTH        32
#define LKUP_EMLC_KEY_16_SRC_MAC_1_MASK         (0xFFFFFFFFU << LKUP_EMLC_KEY_16_SRC_MAC_1_SHIFT)

#ifdef GENHDR_STRUCT
/* The 17th Key register */
typedef union {
  struct {
    uint32_t src_mac_1 : 32;    /* If it is tunnel packet,it contains Inner Src Mac bits[47:16].
                                   If it is non-tunnel packet,it contains Outer Src Mac bits[47:16].
                                   When recirculation is enabled,it contains {24'b0,recirculation id[23:16]}.
                                   NOTE: recircId=24 bits. SW just requires to match on recircId field and zero
                                   out other key fields. So will reuse src_mac lower 3Bytes for putting
                                   recircId. */
  } field;
  uint32_t val;
} LKUP_EMLC_KEY_16_t;
#endif /* GENHDR_STRUCT */

/* The 18th Key register */
#define LKUP_EMLC_KEY_17                0x00001044U
/* Vid. */
#define LKUP_EMLC_KEY_17_VID_SHIFT      0
#define LKUP_EMLC_KEY_17_VID_WIDTH      8
#define LKUP_EMLC_KEY_17_VID_MASK       (0xFFU << LKUP_EMLC_KEY_17_VID_SHIFT)

/* Qid. */
#define LKUP_EMLC_KEY_17_QID_SHIFT      8
#define LKUP_EMLC_KEY_17_QID_WIDTH      16
#define LKUP_EMLC_KEY_17_QID_MASK       (0xFFFFU << LKUP_EMLC_KEY_17_QID_SHIFT)

#ifdef GENHDR_STRUCT
/* The 18th Key register */
typedef union {
  struct {
    uint32_t vid : 8;   /* Vid. */
    uint32_t qid : 16;  /* Qid. */
    uint32_t : 8;
  } field;
  uint32_t val;
} LKUP_EMLC_KEY_17_t;
#endif /* GENHDR_STRUCT */

/* The 1st Result register */
#define LKUP_EMLC_RES_0                         0x00002000U
/* Mac DA bits [31:0]. */
#define LKUP_EMLC_RES_0_MAC_DA_0_SHIFT          0
#define LKUP_EMLC_RES_0_MAC_DA_0_WIDTH          32
#define LKUP_EMLC_RES_0_MAC_DA_0_MASK           (0xFFFFFFFFU << LKUP_EMLC_RES_0_MAC_DA_0_SHIFT)

#ifdef GENHDR_STRUCT
/* The 1st Result register */
typedef union {
  struct {
    uint32_t mac_da_0 : 32;     /* Mac DA bits [31:0]. */
  } field;
  uint32_t val;
} LKUP_EMLC_RES_0_t;
#endif /* GENHDR_STRUCT */

/* The 2nd Result register */
#define LKUP_EMLC_RES_1                         0x00002004U
/* Mac DA bits [47:32]. */
#define LKUP_EMLC_RES_1_MAC_DA_1_SHIFT          0
#define LKUP_EMLC_RES_1_MAC_DA_1_WIDTH          16
#define LKUP_EMLC_RES_1_MAC_DA_1_MASK           (0xFFFFU << LKUP_EMLC_RES_1_MAC_DA_1_SHIFT)

/* Mac SA bits [15:0]. */
#define LKUP_EMLC_RES_1_MAC_SA_0_SHIFT          16
#define LKUP_EMLC_RES_1_MAC_SA_0_WIDTH          16
#define LKUP_EMLC_RES_1_MAC_SA_0_MASK           (0xFFFFU << LKUP_EMLC_RES_1_MAC_SA_0_SHIFT)

#ifdef GENHDR_STRUCT
/* The 2nd Result register */
typedef union {
  struct {
    uint32_t mac_da_1 : 16;     /* Mac DA bits [47:32]. */
    uint32_t mac_sa_0 : 16;     /* Mac SA bits [15:0]. */
  } field;
  uint32_t val;
} LKUP_EMLC_RES_1_t;
#endif /* GENHDR_STRUCT */

/* The 3rd Result register */
#define LKUP_EMLC_RES_2                         0x00002008U
/* Mac SA bits [47:16]. */
#define LKUP_EMLC_RES_2_MAC_SA_1_SHIFT          0
#define LKUP_EMLC_RES_2_MAC_SA_1_WIDTH          32
#define LKUP_EMLC_RES_2_MAC_SA_1_MASK           (0xFFFFFFFFU << LKUP_EMLC_RES_2_MAC_SA_1_SHIFT)

#ifdef GENHDR_STRUCT
/* The 3rd Result register */
typedef union {
  struct {
    uint32_t mac_sa_1 : 32;     /* Mac SA bits [47:16]. */
  } field;
  uint32_t val;
} LKUP_EMLC_RES_2_t;
#endif /* GENHDR_STRUCT */

/* The 4th Result register */
#define LKUP_EMLC_RES_3                         0x0000200CU
/*
 * IP DA bits [31:0]: To be used for Ipv4 or Ipv6. 
 *  ARP DIP bits [31:0]: To be used for ARP in non-encap packet.
 */
#define LKUP_EMLC_RES_3_IP_DA_0_SHIFT           0
#define LKUP_EMLC_RES_3_IP_DA_0_WIDTH           32
#define LKUP_EMLC_RES_3_IP_DA_0_MASK            (0xFFFFFFFFU << LKUP_EMLC_RES_3_IP_DA_0_SHIFT)

#ifdef GENHDR_STRUCT
/* The 4th Result register */
typedef union {
  struct {
    uint32_t ip_da_0 : 32;      /* IP DA bits [31:0]: To be used for Ipv4 or Ipv6.
                                   ARP DIP bits [31:0]: To be used for ARP in non-encap packet. */
  } field;
  uint32_t val;
} LKUP_EMLC_RES_3_t;
#endif /* GENHDR_STRUCT */

/* The 5th Result register */
#define LKUP_EMLC_RES_4                         0x00002010U
/*
 * IP DA bits [63:32]: To be used for Ipv4 or Ipv6. 
 *  ARP DHA bits [31:0]: To be used for ARP in non-encap packet.
 */
#define LKUP_EMLC_RES_4_IP_DA_1_SHIFT           0
#define LKUP_EMLC_RES_4_IP_DA_1_WIDTH           32
#define LKUP_EMLC_RES_4_IP_DA_1_MASK            (0xFFFFFFFFU << LKUP_EMLC_RES_4_IP_DA_1_SHIFT)

#ifdef GENHDR_STRUCT
/* The 5th Result register */
typedef union {
  struct {
    uint32_t ip_da_1 : 32;      /* IP DA bits [63:32]: To be used for Ipv4 or Ipv6.
                                   ARP DHA bits [31:0]: To be used for ARP in non-encap packet. */
  } field;
  uint32_t val;
} LKUP_EMLC_RES_4_t;
#endif /* GENHDR_STRUCT */

/* The 6th Result register */
#define LKUP_EMLC_RES_5                         0x00002014U
/*
 * IP DA bits [95:64]: To be used for Ipv4 or Ipv6. 
 *  ARP DHA bits [47:32]: To be used for ARP in non-encap packet.
 */
#define LKUP_EMLC_RES_5_IP_DA_2_SHIFT           0
#define LKUP_EMLC_RES_5_IP_DA_2_WIDTH           32
#define LKUP_EMLC_RES_5_IP_DA_2_MASK            (0xFFFFFFFFU << LKUP_EMLC_RES_5_IP_DA_2_SHIFT)

#ifdef GENHDR_STRUCT
/* The 6th Result register */
typedef union {
  struct {
    uint32_t ip_da_2 : 32;      /* IP DA bits [95:64]: To be used for Ipv4 or Ipv6.
                                   ARP DHA bits [47:32]: To be used for ARP in non-encap packet. */
  } field;
  uint32_t val;
} LKUP_EMLC_RES_5_t;
#endif /* GENHDR_STRUCT */

/* The 7th Result register */
#define LKUP_EMLC_RES_6                         0x00002018U
/* IP DA bits [127:96]: To be used for Ipv4 or Ipv6. */
#define LKUP_EMLC_RES_6_IP_DA_3_SHIFT           0
#define LKUP_EMLC_RES_6_IP_DA_3_WIDTH           32
#define LKUP_EMLC_RES_6_IP_DA_3_MASK            (0xFFFFFFFFU << LKUP_EMLC_RES_6_IP_DA_3_SHIFT)

#ifdef GENHDR_STRUCT
/* The 7th Result register */
typedef union {
  struct {
    uint32_t ip_da_3 : 32;      /* IP DA bits [127:96]: To be used for Ipv4 or Ipv6. */
  } field;
  uint32_t val;
} LKUP_EMLC_RES_6_t;
#endif /* GENHDR_STRUCT */

/* The 8th Result register */
#define LKUP_EMLC_RES_7                         0x0000201CU
/*
 * IP SA bits [31:0]: To be used for Ipv4 or Ipv6. 
 *  ARP SIP bits [31:0]: To be used for ARP in non-encap packet.
 */
#define LKUP_EMLC_RES_7_IP_SA_0_SHIFT           0
#define LKUP_EMLC_RES_7_IP_SA_0_WIDTH           32
#define LKUP_EMLC_RES_7_IP_SA_0_MASK            (0xFFFFFFFFU << LKUP_EMLC_RES_7_IP_SA_0_SHIFT)

#ifdef GENHDR_STRUCT
/* The 8th Result register */
typedef union {
  struct {
    uint32_t ip_sa_0 : 32;      /* IP SA bits [31:0]: To be used for Ipv4 or Ipv6.
                                   ARP SIP bits [31:0]: To be used for ARP in non-encap packet. */
  } field;
  uint32_t val;
} LKUP_EMLC_RES_7_t;
#endif /* GENHDR_STRUCT */

/* The 9th Result register */
#define LKUP_EMLC_RES_8                         0x00002020U
/*
 * IP SA bits [63:32]: To be used for Ipv4 or Ipv6. 
 *  ARP SHA bits [31:0]: To be used for ARP in non-encap packet.
 */
#define LKUP_EMLC_RES_8_IP_SA_1_SHIFT           0
#define LKUP_EMLC_RES_8_IP_SA_1_WIDTH           32
#define LKUP_EMLC_RES_8_IP_SA_1_MASK            (0xFFFFFFFFU << LKUP_EMLC_RES_8_IP_SA_1_SHIFT)

#ifdef GENHDR_STRUCT
/* The 9th Result register */
typedef union {
  struct {
    uint32_t ip_sa_1 : 32;      /* IP SA bits [63:32]: To be used for Ipv4 or Ipv6.
                                   ARP SHA bits [31:0]: To be used for ARP in non-encap packet. */
  } field;
  uint32_t val;
} LKUP_EMLC_RES_8_t;
#endif /* GENHDR_STRUCT */

/* The 10th Result register */
#define LKUP_EMLC_RES_9                         0x00002024U
/*
 * IP SA bits [95:64]: To be used for Ipv4 or Ipv6. 
 *  ARP SHA bits [47:32]: To be used for ARP in non-encap packet.
 */
#define LKUP_EMLC_RES_9_IP_SA_2_SHIFT           0
#define LKUP_EMLC_RES_9_IP_SA_2_WIDTH           32
#define LKUP_EMLC_RES_9_IP_SA_2_MASK            (0xFFFFFFFFU << LKUP_EMLC_RES_9_IP_SA_2_SHIFT)

#ifdef GENHDR_STRUCT
/* The 10th Result register */
typedef union {
  struct {
    uint32_t ip_sa_2 : 32;      /* IP SA bits [95:64]: To be used for Ipv4 or Ipv6.
                                   ARP SHA bits [47:32]: To be used for ARP in non-encap packet. */
  } field;
  uint32_t val;
} LKUP_EMLC_RES_9_t;
#endif /* GENHDR_STRUCT */

/* The 11th Result register */
#define LKUP_EMLC_RES_10                        0x00002028U
/* IP SA bits [127:96]: To be used for Ipv4 or Ipv6. */
#define LKUP_EMLC_RES_10_IP_SA_3_SHIFT          0
#define LKUP_EMLC_RES_10_IP_SA_3_WIDTH          32
#define LKUP_EMLC_RES_10_IP_SA_3_MASK           (0xFFFFFFFFU << LKUP_EMLC_RES_10_IP_SA_3_SHIFT)

#ifdef GENHDR_STRUCT
/* The 11th Result register */
typedef union {
  struct {
    uint32_t ip_sa_3 : 32;      /* IP SA bits [127:96]: To be used for Ipv4 or Ipv6. */
  } field;
  uint32_t val;
} LKUP_EMLC_RES_10_t;
#endif /* GENHDR_STRUCT */

/* The 12th Result register */
#define LKUP_EMLC_RES_11                        0x0000202CU
/*
 * emlcRes11[31:0] : rsvd - Reserved in case of non-encap packet.
 *  emlcRes11[0] : use_tunnel_table - Tunnel table enable in case of encap
 * tunnel.
 *  emlcRes11[12:1] : tunnel_id[11:0] - Index to tunnel table in case of encap
 * tunnel.
 *  emlcRes11[15:13] : rsvd - Reserved bits in case of encap tunnel.
 *  emlcRes11[31:16] : mpls_label[15:0] - MPLS label in case of encap tunnel.
 */
#define LKUP_EMLC_RES_11_EMLC_RES_11_SHIFT      0
#define LKUP_EMLC_RES_11_EMLC_RES_11_WIDTH      32
#define LKUP_EMLC_RES_11_EMLC_RES_11_MASK       (0xFFFFFFFFU << LKUP_EMLC_RES_11_EMLC_RES_11_SHIFT)

#ifdef GENHDR_STRUCT
/* The 12th Result register */
typedef union {
  struct {
    uint32_t emlc_res_11 : 32;  /* emlcRes11[31:0] : rsvd - Reserved in case of non-encap packet.
                                   emlcRes11[0] : use_tunnel_table - Tunnel table enable in case of encap
                                   tunnel.
                                   emlcRes11[12:1] : tunnel_id[11:0] - Index to tunnel table in case of encap
                                   tunnel.
                                   emlcRes11[15:13] : rsvd - Reserved bits in case of encap tunnel.
                                   emlcRes11[31:16] : mpls_label[15:0] - MPLS label in case of encap tunnel. */
  } field;
  uint32_t val;
} LKUP_EMLC_RES_11_t;
#endif /* GENHDR_STRUCT */

/* The 13th Result register */
#define LKUP_EMLC_RES_12                        0x00002030U
/*
 * emlcRes12[6:0] : mpls_label[22:16] - MPLS label in case of encap tunnel.
 *  Reserved bits in case of non-encap packet.
 *  NOTE: mpls_label[19:0] is 20-bit label value field and mpls_label[22:20] is
 * 3-bit Traffic Class field.
 *  emlcRes12[15:7] : reserved - Reserved bits.
 *  emlcRes12[31:16] : l4DstPort - L4 Dst port[15:0] bits.
 */
#define LKUP_EMLC_RES_12_EMLC_RES_12_SHIFT      0
#define LKUP_EMLC_RES_12_EMLC_RES_12_WIDTH      32
#define LKUP_EMLC_RES_12_EMLC_RES_12_MASK       (0xFFFFFFFFU << LKUP_EMLC_RES_12_EMLC_RES_12_SHIFT)

#ifdef GENHDR_STRUCT
/* The 13th Result register */
typedef union {
  struct {
    uint32_t emlc_res_12 : 32;  /* emlcRes12[6:0] : mpls_label[22:16] - MPLS label in case of encap tunnel.
                                   Reserved bits in case of non-encap packet.
                                   NOTE: mpls_label[19:0] is 20-bit label value field and mpls_label[22:20] is
                                   3-bit Traffic Class field.
                                   emlcRes12[15:7] : reserved - Reserved bits.
                                   emlcRes12[31:16] : l4DstPort - L4 Dst port[15:0] bits. */
  } field;
  uint32_t val;
} LKUP_EMLC_RES_12_t;
#endif /* GENHDR_STRUCT */

/* The 14th Result register */
#define LKUP_EMLC_RES_13                        0x00002034U
/* L4 Src port[15:0] bits. */
#define LKUP_EMLC_RES_13_L_4_SRC_PORT_SHIFT     0
#define LKUP_EMLC_RES_13_L_4_SRC_PORT_WIDTH     16
#define LKUP_EMLC_RES_13_L_4_SRC_PORT_MASK      (0xFFFFU << LKUP_EMLC_RES_13_L_4_SRC_PORT_SHIFT)

/*
 * IP protocol to be used for IP packet.
 *  ARP Opcode to be used for ARP packet.
 */
#define LKUP_EMLC_RES_13_ARP_OPCODE_SHIFT       16
#define LKUP_EMLC_RES_13_ARP_OPCODE_WIDTH       8
#define LKUP_EMLC_RES_13_ARP_OPCODE_MASK        (0xFFU << LKUP_EMLC_RES_13_ARP_OPCODE_SHIFT)

/* IP tos. */
#define LKUP_EMLC_RES_13_IP_TOS_SHIFT           24
#define LKUP_EMLC_RES_13_IP_TOS_WIDTH           8
#define LKUP_EMLC_RES_13_IP_TOS_MASK            (0xFFU << LKUP_EMLC_RES_13_IP_TOS_SHIFT)

#ifdef GENHDR_STRUCT
/* The 14th Result register */
typedef union {
  struct {
    uint32_t l_4_src_port : 16; /* L4 Src port[15:0] bits. */
    uint32_t arp_opcode : 8;    /* IP protocol to be used for IP packet.
                                   ARP Opcode to be used for ARP packet. */
    uint32_t ip_tos : 8;        /* IP tos. */
  } field;
  uint32_t val;
} LKUP_EMLC_RES_13_t;
#endif /* GENHDR_STRUCT */

/* The 15th Result register */
#define LKUP_EMLC_RES_14                                0x00002038U
/* Mod vid vlan. */
#define LKUP_EMLC_RES_14_MOD_VID_VLAN_SHIFT             0
#define LKUP_EMLC_RES_14_MOD_VID_VLAN_WIDTH             12
#define LKUP_EMLC_RES_14_MOD_VID_VLAN_MASK              (0xFFFU << LKUP_EMLC_RES_14_MOD_VID_VLAN_SHIFT)

/* Mod cfi cos vlan. */
#define LKUP_EMLC_RES_14_MOD_CFI_COS_VLAN_SHIFT         12
#define LKUP_EMLC_RES_14_MOD_CFI_COS_VLAN_WIDTH         4
#define LKUP_EMLC_RES_14_MOD_CFI_COS_VLAN_MASK          (0xFU << LKUP_EMLC_RES_14_MOD_CFI_COS_VLAN_SHIFT)

/* StatsIdx or cookie bits[15:0]. */
#define LKUP_EMLC_RES_14_STATS_IDX_0_SHIFT              16
#define LKUP_EMLC_RES_14_STATS_IDX_0_WIDTH              16
#define LKUP_EMLC_RES_14_STATS_IDX_0_MASK               (0xFFFFU << LKUP_EMLC_RES_14_STATS_IDX_0_SHIFT)

#ifdef GENHDR_STRUCT
/* The 15th Result register */
typedef union {
  struct {
    uint32_t mod_vid_vlan : 12;         /* Mod vid vlan. */
    uint32_t mod_cfi_cos_vlan : 4;      /* Mod cfi cos vlan. */
    uint32_t stats_idx_0 : 16;          /* StatsIdx or cookie bits[15:0]. */
  } field;
  uint32_t val;
} LKUP_EMLC_RES_14_t;
#endif /* GENHDR_STRUCT */

/* The 16th Result register */
#define LKUP_EMLC_RES_15                        0x0000203CU
/* StatsIdx or cookie bits[20:16]. */
#define LKUP_EMLC_RES_15_STATS_IDX_1_SHIFT      0
#define LKUP_EMLC_RES_15_STATS_IDX_1_WIDTH      5
#define LKUP_EMLC_RES_15_STATS_IDX_1_MASK       (0x1FU << LKUP_EMLC_RES_15_STATS_IDX_1_SHIFT)

/* Reserved bits. */
#define LKUP_EMLC_RES_15_RSVD_SHIFT             5
#define LKUP_EMLC_RES_15_RSVD_WIDTH             3
#define LKUP_EMLC_RES_15_RSVD_MASK              (0x7U << LKUP_EMLC_RES_15_RSVD_SHIFT)

/* To be used in epoch. */
#define LKUP_EMLC_RES_15_VERSION_SHIFT          8
#define LKUP_EMLC_RES_15_VERSION_WIDTH          8
#define LKUP_EMLC_RES_15_VERSION_MASK           (0xFFU << LKUP_EMLC_RES_15_VERSION_SHIFT)

/* Recirculation Id bits[15:0]. */
#define LKUP_EMLC_RES_15_RECIRC_ID_0_SHIFT      16
#define LKUP_EMLC_RES_15_RECIRC_ID_0_WIDTH      16
#define LKUP_EMLC_RES_15_RECIRC_ID_0_MASK       (0xFFFFU << LKUP_EMLC_RES_15_RECIRC_ID_0_SHIFT)

#ifdef GENHDR_STRUCT
/* The 16th Result register */
typedef union {
  struct {
    uint32_t stats_idx_1 : 5;   /* StatsIdx or cookie bits[20:16]. */
    uint32_t rsvd : 3;          /* Reserved bits. */
    uint32_t version : 8;       /* To be used in epoch. */
    uint32_t recirc_id_0 : 16;  /* Recirculation Id bits[15:0]. */
  } field;
  uint32_t val;
} LKUP_EMLC_RES_15_t;
#endif /* GENHDR_STRUCT */

/* The 17th Result register */
#define LKUP_EMLC_RES_16                        0x00002040U
/* Recirculation Id bits[23:16]. */
#define LKUP_EMLC_RES_16_RECIRC_ID_1_SHIFT      0
#define LKUP_EMLC_RES_16_RECIRC_ID_1_WIDTH      8
#define LKUP_EMLC_RES_16_RECIRC_ID_1_MASK       (0xFFU << LKUP_EMLC_RES_16_RECIRC_ID_1_SHIFT)

/* Qid. */
#define LKUP_EMLC_RES_16_QID_SHIFT              8
#define LKUP_EMLC_RES_16_QID_WIDTH              16
#define LKUP_EMLC_RES_16_QID_MASK               (0xFFFFU << LKUP_EMLC_RES_16_QID_SHIFT)

/* Meter ID A. */
#define LKUP_EMLC_RES_16_METER_ID_A_SHIFT       24
#define LKUP_EMLC_RES_16_METER_ID_A_WIDTH       7
#define LKUP_EMLC_RES_16_METER_ID_A_MASK        (0x7FU << LKUP_EMLC_RES_16_METER_ID_A_SHIFT)

/* Reserved bits. */
#define LKUP_EMLC_RES_16_RSVD_SHIFT             31
#define LKUP_EMLC_RES_16_RSVD_WIDTH             1
#define LKUP_EMLC_RES_16_RSVD_MASK              (0x1U << LKUP_EMLC_RES_16_RSVD_SHIFT)

#ifdef GENHDR_STRUCT
/* The 17th Result register */
typedef union {
  struct {
    uint32_t recirc_id_1 : 8;   /* Recirculation Id bits[23:16]. */
    uint32_t qid : 16;          /* Qid. */
    uint32_t meter_id_a : 7;    /* Meter ID A. */
    uint32_t rsvd : 1;          /* Reserved bits. */
  } field;
  uint32_t val;
} LKUP_EMLC_RES_16_t;
#endif /* GENHDR_STRUCT */

/* The 18th Result register */
#define LKUP_EMLC_RES_17                                0x00002044U
/* Reserved bits. */
#define LKUP_EMLC_RES_17_RSVD_1_SHIFT                   0
#define LKUP_EMLC_RES_17_RSVD_1_WIDTH                   1
#define LKUP_EMLC_RES_17_RSVD_1_MASK                    (0x1U << LKUP_EMLC_RES_17_RSVD_1_SHIFT)

/* Meter ID on A. */
#define LKUP_EMLC_RES_17_METER_ON_A_SHIFT               1
#define LKUP_EMLC_RES_17_METER_ON_A_WIDTH               1
#define LKUP_EMLC_RES_17_METER_ON_A_MASK                (0x1U << LKUP_EMLC_RES_17_METER_ON_A_SHIFT)

/* Reserved bits. */
#define LKUP_EMLC_RES_17_RSVD_2_SHIFT                   2
#define LKUP_EMLC_RES_17_RSVD_2_WIDTH                   7
#define LKUP_EMLC_RES_17_RSVD_2_MASK                    (0x7FU << LKUP_EMLC_RES_17_RSVD_2_SHIFT)

/* Reserved bits. */
#define LKUP_EMLC_RES_17_RSVD_3_SHIFT                   9
#define LKUP_EMLC_RES_17_RSVD_3_WIDTH                   2
#define LKUP_EMLC_RES_17_RSVD_3_MASK                    (0x3U << LKUP_EMLC_RES_17_RSVD_3_SHIFT)

/* Reserved bits. */
#define LKUP_EMLC_RES_17_RSVD_4_SHIFT                   11
#define LKUP_EMLC_RES_17_RSVD_4_WIDTH                   1
#define LKUP_EMLC_RES_17_RSVD_4_MASK                    (0x1U << LKUP_EMLC_RES_17_RSVD_4_SHIFT)

/* Port. */
#define LKUP_EMLC_RES_17_PORT_SHIFT                     12
#define LKUP_EMLC_RES_17_PORT_WIDTH                     2
#define LKUP_EMLC_RES_17_PORT_MASK                      (0x3U << LKUP_EMLC_RES_17_PORT_SHIFT)

/* Subport. */
#define LKUP_EMLC_RES_17_SUBPORT_SHIFT                  14
#define LKUP_EMLC_RES_17_SUBPORT_WIDTH                  7
#define LKUP_EMLC_RES_17_SUBPORT_MASK                   (0x7FU << LKUP_EMLC_RES_17_SUBPORT_SHIFT)

/* Insert vlan. */
#define LKUP_EMLC_RES_17_INSERT_VLAN_SHIFT              21
#define LKUP_EMLC_RES_17_INSERT_VLAN_WIDTH              1
#define LKUP_EMLC_RES_17_INSERT_VLAN_MASK               (0x1U << LKUP_EMLC_RES_17_INSERT_VLAN_SHIFT)

/* Strip vlan. */
#define LKUP_EMLC_RES_17_STRIP_VLAN_SHIFT               22
#define LKUP_EMLC_RES_17_STRIP_VLAN_WIDTH               1
#define LKUP_EMLC_RES_17_STRIP_VLAN_MASK                (0x1U << LKUP_EMLC_RES_17_STRIP_VLAN_SHIFT)

/* Modify vlan. */
#define LKUP_EMLC_RES_17_MOD_VLAN_SHIFT                 23
#define LKUP_EMLC_RES_17_MOD_VLAN_WIDTH                 1
#define LKUP_EMLC_RES_17_MOD_VLAN_MASK                  (0x1U << LKUP_EMLC_RES_17_MOD_VLAN_SHIFT)

/* Mac NAT DA enable. */
#define LKUP_EMLC_RES_17_MAC_NAT_DA_EN_SHIFT            24
#define LKUP_EMLC_RES_17_MAC_NAT_DA_EN_WIDTH            1
#define LKUP_EMLC_RES_17_MAC_NAT_DA_EN_MASK             (0x1U << LKUP_EMLC_RES_17_MAC_NAT_DA_EN_SHIFT)

/* Mac NAT SA enable. */
#define LKUP_EMLC_RES_17_MAC_NAT_SA_EN_SHIFT            25
#define LKUP_EMLC_RES_17_MAC_NAT_SA_EN_WIDTH            1
#define LKUP_EMLC_RES_17_MAC_NAT_SA_EN_MASK             (0x1U << LKUP_EMLC_RES_17_MAC_NAT_SA_EN_SHIFT)

/*
 * IP NAT DA enable.
 *  ARP DIP modification enable.
 */
#define LKUP_EMLC_RES_17_IP_NAT_DA_EN_SHIFT             26
#define LKUP_EMLC_RES_17_IP_NAT_DA_EN_WIDTH             1
#define LKUP_EMLC_RES_17_IP_NAT_DA_EN_MASK              (0x1U << LKUP_EMLC_RES_17_IP_NAT_DA_EN_SHIFT)

/*
 * IP NAT SA enable.
 *  ARP SIP modification enable.
 */
#define LKUP_EMLC_RES_17_IP_NAT_SA_EN_SHIFT             27
#define LKUP_EMLC_RES_17_IP_NAT_SA_EN_WIDTH             1
#define LKUP_EMLC_RES_17_IP_NAT_SA_EN_MASK              (0x1U << LKUP_EMLC_RES_17_IP_NAT_SA_EN_SHIFT)

/*
 * L4 NAT DstPort enable.
 *  ARP DHA modification enable.
 */
#define LKUP_EMLC_RES_17_L_4_NAT_DSTPORT_EN_SHIFT       28
#define LKUP_EMLC_RES_17_L_4_NAT_DSTPORT_EN_WIDTH       1
#define LKUP_EMLC_RES_17_L_4_NAT_DSTPORT_EN_MASK        (0x1U << LKUP_EMLC_RES_17_L_4_NAT_DSTPORT_EN_SHIFT)

/*
 * L4 NAT SrcPort enable.
 *  ARP SHA modification enable.
 */
#define LKUP_EMLC_RES_17_L_4_NAT_SRCPORT_EN_SHIFT       29
#define LKUP_EMLC_RES_17_L_4_NAT_SRCPORT_EN_WIDTH       1
#define LKUP_EMLC_RES_17_L_4_NAT_SRCPORT_EN_MASK        (0x1U << LKUP_EMLC_RES_17_L_4_NAT_SRCPORT_EN_SHIFT)

/* Decrement TTL enable. */
#define LKUP_EMLC_RES_17_DEC_TTL_EN_SHIFT               30
#define LKUP_EMLC_RES_17_DEC_TTL_EN_WIDTH               1
#define LKUP_EMLC_RES_17_DEC_TTL_EN_MASK                (0x1U << LKUP_EMLC_RES_17_DEC_TTL_EN_SHIFT)

/* Strip and forward enable. */
#define LKUP_EMLC_RES_17_STRIP_NFWD_SHIFT               31
#define LKUP_EMLC_RES_17_STRIP_NFWD_WIDTH               1
#define LKUP_EMLC_RES_17_STRIP_NFWD_MASK                (0x1U << LKUP_EMLC_RES_17_STRIP_NFWD_SHIFT)

#ifdef GENHDR_STRUCT
/* The 18th Result register */
typedef union {
  struct {
    uint32_t rsvd_1 : 1;                /* Reserved bits. */
    uint32_t meter_on_a : 1;            /* Meter ID on A. */
    uint32_t rsvd_2 : 7;                /* Reserved bits. */
    uint32_t rsvd_3 : 2;                /* Reserved bits. */
    uint32_t rsvd_4 : 1;                /* Reserved bits. */
    uint32_t port : 2;                  /* Port. */
    uint32_t subport : 7;               /* Subport. */
    uint32_t insert_vlan : 1;           /* Insert vlan. */
    uint32_t strip_vlan : 1;            /* Strip vlan. */
    uint32_t mod_vlan : 1;              /* Modify vlan. */
    uint32_t mac_nat_da_en : 1;         /* Mac NAT DA enable. */
    uint32_t mac_nat_sa_en : 1;         /* Mac NAT SA enable. */
    uint32_t ip_nat_da_en : 1;          /* IP NAT DA enable.
                                           ARP DIP modification enable. */
    uint32_t ip_nat_sa_en : 1;          /* IP NAT SA enable.
                                           ARP SIP modification enable. */
    uint32_t l_4_nat_dstport_en : 1;    /* L4 NAT DstPort enable.
                                           ARP DHA modification enable. */
    uint32_t l_4_nat_srcport_en : 1;    /* L4 NAT SrcPort enable.
                                           ARP SHA modification enable. */
    uint32_t dec_ttl_en : 1;            /* Decrement TTL enable. */
    uint32_t strip_nfwd : 1;            /* Strip and forward enable. */
  } field;
  uint32_t val;
} LKUP_EMLC_RES_17_t;
#endif /* GENHDR_STRUCT */

/* The 19th Result register */
#define LKUP_EMLC_RES_18                                        0x00002048U
/* Insert and set enable. */
#define LKUP_EMLC_RES_18_INSERT_NSET_SHIFT                      0
#define LKUP_EMLC_RES_18_INSERT_NSET_WIDTH                      1
#define LKUP_EMLC_RES_18_INSERT_NSET_MASK                       (0x1U << LKUP_EMLC_RES_18_INSERT_NSET_SHIFT)

/*
 * Modify IP protocol field enable.
 *  Modify ARP opcode field enable.
 */
#define LKUP_EMLC_RES_18_ARP_OPCODE_MOD_EN_SHIFT                1
#define LKUP_EMLC_RES_18_ARP_OPCODE_MOD_EN_WIDTH                1
#define LKUP_EMLC_RES_18_ARP_OPCODE_MOD_EN_MASK                 (0x1U << LKUP_EMLC_RES_18_ARP_OPCODE_MOD_EN_SHIFT)

/* Modify IP TOS field enable. */
#define LKUP_EMLC_RES_18_IP_TOS_EN_SHIFT                        2
#define LKUP_EMLC_RES_18_IP_TOS_EN_WIDTH                        1
#define LKUP_EMLC_RES_18_IP_TOS_EN_MASK                         (0x1U << LKUP_EMLC_RES_18_IP_TOS_EN_SHIFT)

/* MPLSoGRE: Decap ETH header from the packet. */
#define LKUP_EMLC_RES_18_MPLS_OGRE_DE_ETH_HDR_SHIFT             3
#define LKUP_EMLC_RES_18_MPLS_OGRE_DE_ETH_HDR_WIDTH             1
#define LKUP_EMLC_RES_18_MPLS_OGRE_DE_ETH_HDR_MASK              (0x1U << LKUP_EMLC_RES_18_MPLS_OGRE_DE_ETH_HDR_SHIFT)

/* MPLSoGRE: Decap IP header from the packet. */
#define LKUP_EMLC_RES_18_MPLS_OGRE_DE_IP_HDR_SHIFT              4
#define LKUP_EMLC_RES_18_MPLS_OGRE_DE_IP_HDR_WIDTH              1
#define LKUP_EMLC_RES_18_MPLS_OGRE_DE_IP_HDR_MASK               (0x1U << LKUP_EMLC_RES_18_MPLS_OGRE_DE_IP_HDR_SHIFT)

/* Modify TTL field in IPV4 header with field ip_tos. */
#define LKUP_EMLC_RES_18_MOD_TTL_EN_SHIFT                       5
#define LKUP_EMLC_RES_18_MOD_TTL_EN_WIDTH                       1
#define LKUP_EMLC_RES_18_MOD_TTL_EN_MASK                        (0x1U << LKUP_EMLC_RES_18_MOD_TTL_EN_SHIFT)

/*
 * Decap VXLAN enable.
 *  Encap GRE seq field enable.
 */
#define LKUP_EMLC_RES_18_DECAP_VXLAN_EN_SHIFT                   6
#define LKUP_EMLC_RES_18_DECAP_VXLAN_EN_WIDTH                   1
#define LKUP_EMLC_RES_18_DECAP_VXLAN_EN_MASK                    (0x1U << LKUP_EMLC_RES_18_DECAP_VXLAN_EN_SHIFT)

/* Drop packet indication. */
#define LKUP_EMLC_RES_18_DROP_SHIFT                             7
#define LKUP_EMLC_RES_18_DROP_WIDTH                             1
#define LKUP_EMLC_RES_18_DROP_MASK                              (0x1U << LKUP_EMLC_RES_18_DROP_SHIFT)

/* High priority queue. */
#define LKUP_EMLC_RES_18_TXQ_CTRL_PKT_PORT_SHIFT                8
#define LKUP_EMLC_RES_18_TXQ_CTRL_PKT_PORT_WIDTH                1
#define LKUP_EMLC_RES_18_TXQ_CTRL_PKT_PORT_MASK                 (0x1U << LKUP_EMLC_RES_18_TXQ_CTRL_PKT_PORT_SHIFT)

/* MPLSoUDP: Decap ETH header from the packet. */
#define LKUP_EMLC_RES_18_MPLS_OUDPDE_ETH_HDR_SHIFT              9
#define LKUP_EMLC_RES_18_MPLS_OUDPDE_ETH_HDR_WIDTH              1
#define LKUP_EMLC_RES_18_MPLS_OUDPDE_ETH_HDR_MASK               (0x1U << LKUP_EMLC_RES_18_MPLS_OUDPDE_ETH_HDR_SHIFT)

/* MPLSoUDP: Decap IP header from the packet. */
#define LKUP_EMLC_RES_18_MPLS_OUDPDE_IP_HDR_SHIFT               10
#define LKUP_EMLC_RES_18_MPLS_OUDPDE_IP_HDR_WIDTH               1
#define LKUP_EMLC_RES_18_MPLS_OUDPDE_IP_HDR_MASK                (0x1U << LKUP_EMLC_RES_18_MPLS_OUDPDE_IP_HDR_SHIFT)

/* IPoGRE: Decap IP and GRE header from the packet. */
#define LKUP_EMLC_RES_18_IP_OGRE_DE_IP_GRE_HDR_SHIFT            11
#define LKUP_EMLC_RES_18_IP_OGRE_DE_IP_GRE_HDR_WIDTH            1
#define LKUP_EMLC_RES_18_IP_OGRE_DE_IP_GRE_HDR_MASK             (0x1U << LKUP_EMLC_RES_18_IP_OGRE_DE_IP_GRE_HDR_SHIFT)

/* IPoGRE: Decap ETH header from the packet. */
#define LKUP_EMLC_RES_18_IP_OGRE_DE_ETH_HDR_SHIFT               12
#define LKUP_EMLC_RES_18_IP_OGRE_DE_ETH_HDR_WIDTH               1
#define LKUP_EMLC_RES_18_IP_OGRE_DE_ETH_HDR_MASK                (0x1U << LKUP_EMLC_RES_18_IP_OGRE_DE_ETH_HDR_SHIFT)

/* Encap GRE flag C. */
#define LKUP_EMLC_RES_18_ENCAP_GRE_FLAG_C_SHIFT                 13
#define LKUP_EMLC_RES_18_ENCAP_GRE_FLAG_C_WIDTH                 1
#define LKUP_EMLC_RES_18_ENCAP_GRE_FLAG_C_MASK                  (0x1U << LKUP_EMLC_RES_18_ENCAP_GRE_FLAG_C_SHIFT)

/* Recirculation enable. */
#define LKUP_EMLC_RES_18_RECIRC_EN_SHIFT                        14
#define LKUP_EMLC_RES_18_RECIRC_EN_WIDTH                        1
#define LKUP_EMLC_RES_18_RECIRC_EN_MASK                         (0x1U << LKUP_EMLC_RES_18_RECIRC_EN_SHIFT)

/* Recirculation mode. */
#define LKUP_EMLC_RES_18_RECIRC_MODE_SHIFT                      15
#define LKUP_EMLC_RES_18_RECIRC_MODE_WIDTH                      2
#define LKUP_EMLC_RES_18_RECIRC_MODE_MASK                       (0x3U << LKUP_EMLC_RES_18_RECIRC_MODE_SHIFT)

/* Connection track ID mux. */
#define LKUP_EMLC_RES_18_CONN_TRACK_ID_MUX_SHIFT                17
#define LKUP_EMLC_RES_18_CONN_TRACK_ID_MUX_WIDTH                1
#define LKUP_EMLC_RES_18_CONN_TRACK_ID_MUX_MASK                 (0x1U << LKUP_EMLC_RES_18_CONN_TRACK_ID_MUX_SHIFT)

/* Connection track ID miss. */
#define LKUP_EMLC_RES_18_CONN_TRACK_ID_MISS_SHIFT               18
#define LKUP_EMLC_RES_18_CONN_TRACK_ID_MISS_WIDTH               1
#define LKUP_EMLC_RES_18_CONN_TRACK_ID_MISS_MASK                (0x1U << LKUP_EMLC_RES_18_CONN_TRACK_ID_MISS_SHIFT)

/* Connection track packet type. */
#define LKUP_EMLC_RES_18_CONN_TRACK_PTYPE_SHIFT                 19
#define LKUP_EMLC_RES_18_CONN_TRACK_PTYPE_WIDTH                 1
#define LKUP_EMLC_RES_18_CONN_TRACK_PTYPE_MASK                  (0x1U << LKUP_EMLC_RES_18_CONN_TRACK_PTYPE_SHIFT)

/* Mirror status. */
#define LKUP_EMLC_RES_18_MIRROR_STATE_SHIFT                     20
#define LKUP_EMLC_RES_18_MIRROR_STATE_WIDTH                     1
#define LKUP_EMLC_RES_18_MIRROR_STATE_MASK                      (0x1U << LKUP_EMLC_RES_18_MIRROR_STATE_SHIFT)

/* Security status. */
#define LKUP_EMLC_RES_18_SEC_STATE_SHIFT                        21
#define LKUP_EMLC_RES_18_SEC_STATE_WIDTH                        2
#define LKUP_EMLC_RES_18_SEC_STATE_MASK                         (0x3U << LKUP_EMLC_RES_18_SEC_STATE_SHIFT)

/*
 * Encap tunnel UDP Source port[8:0] in case of encap-tunnel.
 *  Reserved in non-encap packet.
 */
#define LKUP_EMLC_RES_18_ENCAP_L4_SRC_PORT_0_SHIFT              23
#define LKUP_EMLC_RES_18_ENCAP_L4_SRC_PORT_0_WIDTH              9
#define LKUP_EMLC_RES_18_ENCAP_L4_SRC_PORT_0_MASK               (0x1FFU << LKUP_EMLC_RES_18_ENCAP_L4_SRC_PORT_0_SHIFT)

#ifdef GENHDR_STRUCT
/* The 19th Result register */
typedef union {
  struct {
    uint32_t insert_nset : 1;                   /* Insert and set enable. */
    uint32_t arp_opcode_mod_en : 1;             /* Modify IP protocol field enable.
                                                   Modify ARP opcode field enable. */
    uint32_t ip_tos_en : 1;                     /* Modify IP TOS field enable. */
    uint32_t mpls_ogre_de_eth_hdr : 1;          /* MPLSoGRE: Decap ETH header from the packet. */
    uint32_t mpls_ogre_de_ip_hdr : 1;           /* MPLSoGRE: Decap IP header from the packet. */
    uint32_t mod_ttl_en : 1;                    /* Modify TTL field in IPV4 header with field ip_tos. */
    uint32_t decap_vxlan_en : 1;                /* Decap VXLAN enable.
                                                   Encap GRE seq field enable. */
    uint32_t drop : 1;                          /* Drop packet indication. */
    uint32_t txq_ctrl_pkt_port : 1;             /* High priority queue. */
    uint32_t mpls_oudpde_eth_hdr : 1;           /* MPLSoUDP: Decap ETH header from the packet. */
    uint32_t mpls_oudpde_ip_hdr : 1;            /* MPLSoUDP: Decap IP header from the packet. */
    uint32_t ip_ogre_de_ip_gre_hdr : 1;         /* IPoGRE: Decap IP and GRE header from the packet. */
    uint32_t ip_ogre_de_eth_hdr : 1;            /* IPoGRE: Decap ETH header from the packet. */
    uint32_t encap_gre_flag_c : 1;              /* Encap GRE flag C. */
    uint32_t recirc_en : 1;                     /* Recirculation enable. */
    uint32_t recirc_mode : 2;                   /* Recirculation mode. */
    uint32_t conn_track_id_mux : 1;             /* Connection track ID mux. */
    uint32_t conn_track_id_miss : 1;            /* Connection track ID miss. */
    uint32_t conn_track_ptype : 1;              /* Connection track packet type. */
    uint32_t mirror_state : 1;                  /* Mirror status. */
    uint32_t sec_state : 2;                     /* Security status. */
    uint32_t encap_l4_src_port_0 : 9;           /* Encap tunnel UDP Source port[8:0] in case of encap-tunnel.
                                                   Reserved in non-encap packet. */
  } field;
  uint32_t val;
} LKUP_EMLC_RES_18_t;
#endif /* GENHDR_STRUCT */

/* The 20th Result register */
#define LKUP_EMLC_RES_19                                0x0000204CU
/*
 * Encap tunnel UDP Source port[15:9] in case of encap-tunnel.
 *  Reserved in non-encap packet.
 */
#define LKUP_EMLC_RES_19_ENCAP_L4_SRC_PORT_1_SHIFT      0
#define LKUP_EMLC_RES_19_ENCAP_L4_SRC_PORT_1_WIDTH      7
#define LKUP_EMLC_RES_19_ENCAP_L4_SRC_PORT_1_MASK       (0x7FU << LKUP_EMLC_RES_19_ENCAP_L4_SRC_PORT_1_SHIFT)

/*
 * Used for L3VPN(L3 flows) feature.
 *  For decap mode,set this bit to '1' to insert inner L2 header.
 *  For this, the MAC addresses are used from EMLC_RES registers: macDa* and
 * macSa*.
 *  If insertVlan bit is set,then the VLAN ID is used from EMLC_RES registers:
 * modVidVlan and modCfiCosVlan.
 *  And,the ethernet type field is updated based on the inner L3 header
 * type(i.e. IPv4 or IPv6 or ARP) of the packet.
 *  NOTE-1: The L3VPN feature is supported only for MPLSoUDP or MPLSoGRE tunnel
 * packets.
 *  NOTE-2: When this feature is enabled,the L2 NAT is not supported.
 *  NOTE-3: For L2 header insertion,DOT2Q is not supported and also Intel Header
 * is not supported.
 *  For encap mode,this bit is reserved.
 */
#define LKUP_EMLC_RES_19_INSERT_INNER_L2_SHIFT          7
#define LKUP_EMLC_RES_19_INSERT_INNER_L2_WIDTH          1
#define LKUP_EMLC_RES_19_INSERT_INNER_L2_MASK           (0x1U << LKUP_EMLC_RES_19_INSERT_INNER_L2_SHIFT)

/*
 * Used for L3VPN(L3 flows) feature.
 *  For encap mode,set this bit to '1' to strip inner L2 header.
 *  For decap mode,this bit is reserved.
 */
#define LKUP_EMLC_RES_19_STRIP_INNER_L2_SHIFT           8
#define LKUP_EMLC_RES_19_STRIP_INNER_L2_WIDTH           1
#define LKUP_EMLC_RES_19_STRIP_INNER_L2_MASK            (0x1U << LKUP_EMLC_RES_19_STRIP_INNER_L2_SHIFT)

/* Reserved bits. */
#define LKUP_EMLC_RES_19_RSVD_SHIFT                     9
#define LKUP_EMLC_RES_19_RSVD_WIDTH                     23
#define LKUP_EMLC_RES_19_RSVD_MASK                      (0x7FFFFFU << LKUP_EMLC_RES_19_RSVD_SHIFT)

#ifdef GENHDR_STRUCT
/* The 20th Result register */
typedef union {
  struct {
    uint32_t encap_l4_src_port_1 : 7;   /* Encap tunnel UDP Source port[15:9] in case of encap-tunnel.
                                           Reserved in non-encap packet. */
    uint32_t insert_inner_l2 : 1;       /* Used for L3VPN(L3 flows) feature.
                                           For decap mode,set this bit to '1' to insert inner L2 header.
                                           For this, the MAC addresses are used from EMLC_RES registers: macDa* and
                                           macSa*.
                                           If insertVlan bit is set,then the VLAN ID is used from EMLC_RES registers:
                                           modVidVlan and modCfiCosVlan.
                                           And,the ethernet type field is updated based on the inner L3 header
                                           type(i.e. IPv4 or IPv6 or ARP) of the packet.
                                           NOTE-1: The L3VPN feature is supported only for MPLSoUDP or MPLSoGRE tunnel
                                           packets.
                                           NOTE-2: When this feature is enabled,the L2 NAT is not supported.
                                           NOTE-3: For L2 header insertion,DOT2Q is not supported and also Intel Header
                                           is not supported.
                                           For encap mode,this bit is reserved. */
    uint32_t strip_inner_l2 : 1;        /* Used for L3VPN(L3 flows) feature.
                                           For encap mode,set this bit to '1' to strip inner L2 header.
                                           For decap mode,this bit is reserved. */
    uint32_t rsvd : 23;                 /* Reserved bits. */
  } field;
  uint32_t val;
} LKUP_EMLC_RES_19_t;
#endif /* GENHDR_STRUCT */

/* The 21st Result register */
#define LKUP_EMLC_RES_20                0x00002050U
/* Reserved bits. */
#define LKUP_EMLC_RES_20_RSVD_SHIFT     0
#define LKUP_EMLC_RES_20_RSVD_WIDTH     32
#define LKUP_EMLC_RES_20_RSVD_MASK      (0xFFFFFFFFU << LKUP_EMLC_RES_20_RSVD_SHIFT)

#ifdef GENHDR_STRUCT
/* The 21st Result register */
typedef union {
  struct {
    uint32_t rsvd : 32; /* Reserved bits. */
  } field;
  uint32_t val;
} LKUP_EMLC_RES_20_t;
#endif /* GENHDR_STRUCT */

/* The 22nd Result register */
#define LKUP_EMLC_RES_21                0x00002054U
/* Reserved bits. */
#define LKUP_EMLC_RES_21_RSVD_SHIFT     0
#define LKUP_EMLC_RES_21_RSVD_WIDTH     32
#define LKUP_EMLC_RES_21_RSVD_MASK      (0xFFFFFFFFU << LKUP_EMLC_RES_21_RSVD_SHIFT)

#ifdef GENHDR_STRUCT
/* The 22nd Result register */
typedef union {
  struct {
    uint32_t rsvd : 32; /* Reserved bits. */
  } field;
  uint32_t val;
} LKUP_EMLC_RES_21_t;
#endif /* GENHDR_STRUCT */

/* The 23rd Result register */
#define LKUP_EMLC_RES_22                0x00002058U
/* Reserved bits. */
#define LKUP_EMLC_RES_22_RSVD_SHIFT     0
#define LKUP_EMLC_RES_22_RSVD_WIDTH     8
#define LKUP_EMLC_RES_22_RSVD_MASK      (0xFFU << LKUP_EMLC_RES_22_RSVD_SHIFT)

#ifdef GENHDR_STRUCT
/* The 23rd Result register */
typedef union {
  struct {
    uint32_t rsvd : 8;  /* Reserved bits. */
    uint32_t : 24;
  } field;
  uint32_t val;
} LKUP_EMLC_RES_22_t;
#endif /* GENHDR_STRUCT */

#endif /* _LKUP_REGISTERS_H_ */

