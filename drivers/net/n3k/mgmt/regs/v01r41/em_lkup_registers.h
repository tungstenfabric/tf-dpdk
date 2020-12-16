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

#ifndef _EM_LKUP_REGISTERS_H_
#define _EM_LKUP_REGISTERS_H_

/* EM Lookup */

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

#define EM_LKUP_ACCESSTYPE              GENHDR_ACCESSTYPE_RW /*  Default access type. Access types defined above. */
#define EM_LKUP_REGWIDTH                32 /* Default width of register in bits */
#define EM_LKUP_ACCESSWIDTH             32 /* Default width of access word in bit */


/* Version number of Exact Match Lookup IP */
#define EM_LKUP_EML_VERSION                     0x00000000U
/*
 * Version format: DDMMYY_CC. CC indicates core module instance: MHL(00) or
 * MBL(10). Core module registers share common register prefix - EMLC.
 */
#define EM_LKUP_EML_VERSION_VERSION_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define EM_LKUP_EML_VERSION_VERSION_SHIFT       0
#define EM_LKUP_EML_VERSION_VERSION_WIDTH       32
#define EM_LKUP_EML_VERSION_VERSION_MASK        (0xFFFFFFFFU << EM_LKUP_EML_VERSION_VERSION_SHIFT)

#ifdef GENHDR_STRUCT
/* Version number of Exact Match Lookup IP */
typedef union {
  struct {
    uint32_t version : 32;      /* Version format: DDMMYY_CC. CC indicates core module instance: MHL(00) or
                                   MBL(10). Core module registers share common register prefix - EMLC. */
  } field;
  uint32_t val;
} EM_LKUP_EML_VERSION_t;
#endif /* GENHDR_STRUCT */

/* Test register to check read/write access */
#define EM_LKUP_EML_SCRATCH                     0x00000004U
/* Scratch register. Resets to 0. */
#define EM_LKUP_EML_SCRATCH_SCRATCH_SHIFT       0
#define EM_LKUP_EML_SCRATCH_SCRATCH_WIDTH       32
#define EM_LKUP_EML_SCRATCH_SCRATCH_MASK        (0xFFFFFFFFU << EM_LKUP_EML_SCRATCH_SCRATCH_SHIFT)

#ifdef GENHDR_STRUCT
/* Test register to check read/write access */
typedef union {
  struct {
    uint32_t scratch : 32;      /* Scratch register. Resets to 0. */
  } field;
  uint32_t val;
} EM_LKUP_EML_SCRATCH_t;
#endif /* GENHDR_STRUCT */

/* Supported number of exact match entries */
#define EM_LKUP_EML_NUM_EM                      0x00000008U
/* Supported number of exact match entries. Resets to 0x00200000. */
#define EM_LKUP_EML_NUM_EM_NUM_EM_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define EM_LKUP_EML_NUM_EM_NUM_EM_SHIFT         0
#define EM_LKUP_EML_NUM_EM_NUM_EM_WIDTH         28
#define EM_LKUP_EML_NUM_EM_NUM_EM_MASK          (0xFFFFFFFU << EM_LKUP_EML_NUM_EM_NUM_EM_SHIFT)

/* Supported number of logical tables. Resets to 0x0. */
#define EM_LKUP_EML_NUM_EM_NUM_TAB_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define EM_LKUP_EML_NUM_EM_NUM_TAB_SHIFT        28
#define EM_LKUP_EML_NUM_EM_NUM_TAB_WIDTH        4
#define EM_LKUP_EML_NUM_EM_NUM_TAB_MASK         (0xFU << EM_LKUP_EML_NUM_EM_NUM_TAB_SHIFT)

#ifdef GENHDR_STRUCT
/* Supported number of exact match entries */
typedef union {
  struct {
    uint32_t num_em : 28;       /* Supported number of exact match entries. Resets to 0x00200000. */
    uint32_t num_tab : 4;       /* Supported number of logical tables. Resets to 0x0. */
  } field;
  uint32_t val;
} EM_LKUP_EML_NUM_EM_t;
#endif /* GENHDR_STRUCT */

/* Key width in bits supported in hardware */
#define EM_LKUP_EML_KEY_WDTH                    0x0000000CU
/* Key width supported in hardware. Resets to 0x00000408. */
#define EM_LKUP_EML_KEY_WDTH_KEY_WDTH_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define EM_LKUP_EML_KEY_WDTH_KEY_WDTH_SHIFT     0
#define EM_LKUP_EML_KEY_WDTH_KEY_WDTH_WIDTH     32
#define EM_LKUP_EML_KEY_WDTH_KEY_WDTH_MASK      (0xFFFFFFFFU << EM_LKUP_EML_KEY_WDTH_KEY_WDTH_SHIFT)

#ifdef GENHDR_STRUCT
/* Key width in bits supported in hardware */
typedef union {
  struct {
    uint32_t key_wdth : 32;     /* Key width supported in hardware. Resets to 0x00000408. */
  } field;
  uint32_t val;
} EM_LKUP_EML_KEY_WDTH_t;
#endif /* GENHDR_STRUCT */

/* Result width in bits supported in hardware */
#define EM_LKUP_EML_RES_WDTH                    0x00000010U
/* Result width supported in hardware. Resets to 0x00000003f8. */
#define EM_LKUP_EML_RES_WDTH_RES_WDTH_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define EM_LKUP_EML_RES_WDTH_RES_WDTH_SHIFT     0
#define EM_LKUP_EML_RES_WDTH_RES_WDTH_WIDTH     32
#define EM_LKUP_EML_RES_WDTH_RES_WDTH_MASK      (0xFFFFFFFFU << EM_LKUP_EML_RES_WDTH_RES_WDTH_SHIFT)

#ifdef GENHDR_STRUCT
/* Result width in bits supported in hardware */
typedef union {
  struct {
    uint32_t res_wdth : 32;     /* Result width supported in hardware. Resets to 0x00000003f8. */
  } field;
  uint32_t val;
} EM_LKUP_EML_RES_WDTH_t;
#endif /* GENHDR_STRUCT */

/* Alarms reported by the hardware */
#define EM_LKUP_EML_ALARMS                      0x00000014U
/*
 * Alarm indicated by the core lookup component. Check FATAL_0 and WARNING_0
 * registers in the MHL/MBL component for further details. Resets to 0.
 */
#define EM_LKUP_EML_ALARMS_CORE_ALARM_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define EM_LKUP_EML_ALARMS_CORE_ALARM_SHIFT     0
#define EM_LKUP_EML_ALARMS_CORE_ALARM_WIDTH     1
#define EM_LKUP_EML_ALARMS_CORE_ALARM_MASK      (0x1U << EM_LKUP_EML_ALARMS_CORE_ALARM_SHIFT)

/*
 * Alarm indicated by DRC (ddrx_ropt_ctrl). The alarms are a result of FIFO
 * overflows and should be treated as fatal (reset required). Resets to 0.
 */
#define EM_LKUP_EML_ALARMS_DRC_ALARM_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define EM_LKUP_EML_ALARMS_DRC_ALARM_SHIFT      1
#define EM_LKUP_EML_ALARMS_DRC_ALARM_WIDTH      1
#define EM_LKUP_EML_ALARMS_DRC_ALARM_MASK       (0x1U << EM_LKUP_EML_ALARMS_DRC_ALARM_SHIFT)

/*
 * This field indicates that the actual read latency through DDR3 system is
 * greater than the fixed latency pipeline delay.
 *  This error is fatal.
 *  Consider increasing the fixed pipeline latency parameter (compile time
 * modification).
 *  Resets to 0.
 */
#define EM_LKUP_EML_ALARMS_RLAT_ALARM_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define EM_LKUP_EML_ALARMS_RLAT_ALARM_SHIFT     2
#define EM_LKUP_EML_ALARMS_RLAT_ALARM_WIDTH     1
#define EM_LKUP_EML_ALARMS_RLAT_ALARM_MASK      (0x1U << EM_LKUP_EML_ALARMS_RLAT_ALARM_SHIFT)

#ifdef GENHDR_STRUCT
/* Alarms reported by the hardware */
typedef union {
  struct {
    uint32_t core_alarm : 1;    /* Alarm indicated by the core lookup component. Check FATAL_0 and WARNING_0
                                   registers in the MHL/MBL component for further details. Resets to 0. */
    uint32_t drc_alarm : 1;     /* Alarm indicated by DRC (ddrx_ropt_ctrl). The alarms are a result of FIFO
                                   overflows and should be treated as fatal (reset required). Resets to 0. */
    uint32_t rlat_alarm : 1;    /* This field indicates that the actual read latency through DDR3 system is
                                   greater than the fixed latency pipeline delay.
                                   This error is fatal.
                                   Consider increasing the fixed pipeline latency parameter (compile time
                                   modification).
                                   Resets to 0. */
    uint32_t : 29;
  } field;
  uint32_t val;
} EM_LKUP_EML_ALARMS_t;
#endif /* GENHDR_STRUCT */

/* Read latency through the DDR subsystem 0 */
#define EM_LKUP_DRC_0_RLAT                      0x00000018U
/*
 * read latency through the DDR subsystem 0, in user clock cycles. This latency
 * must always be smaller than the fixed latency pipeline delay. Resets to 0.
 */
#define EM_LKUP_DRC_0_RLAT_DRC_RLAT_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define EM_LKUP_DRC_0_RLAT_DRC_RLAT_SHIFT       0
#define EM_LKUP_DRC_0_RLAT_DRC_RLAT_WIDTH       32
#define EM_LKUP_DRC_0_RLAT_DRC_RLAT_MASK        (0xFFFFFFFFU << EM_LKUP_DRC_0_RLAT_DRC_RLAT_SHIFT)

#ifdef GENHDR_STRUCT
/* Read latency through the DDR subsystem 0 */
typedef union {
  struct {
    uint32_t drc_rlat : 32;     /* read latency through the DDR subsystem 0, in user clock cycles. This latency
                                   must always be smaller than the fixed latency pipeline delay. Resets to 0. */
  } field;
  uint32_t val;
} EM_LKUP_DRC_0_RLAT_t;
#endif /* GENHDR_STRUCT */

/* Read latency through the DDR subsystem 1 */
#define EM_LKUP_DRC_1_RLAT                      0x0000001CU
/*
 * read latency through the DDR subsystem 1, in user clock cycles. This latency
 * must always be smaller than the fixed latency pipeline delay. Resets to 0.
 */
#define EM_LKUP_DRC_1_RLAT_DRC_RLAT_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define EM_LKUP_DRC_1_RLAT_DRC_RLAT_SHIFT       0
#define EM_LKUP_DRC_1_RLAT_DRC_RLAT_WIDTH       32
#define EM_LKUP_DRC_1_RLAT_DRC_RLAT_MASK        (0xFFFFFFFFU << EM_LKUP_DRC_1_RLAT_DRC_RLAT_SHIFT)

#ifdef GENHDR_STRUCT
/* Read latency through the DDR subsystem 1 */
typedef union {
  struct {
    uint32_t drc_rlat : 32;     /* read latency through the DDR subsystem 1, in user clock cycles. This latency
                                   must always be smaller than the fixed latency pipeline delay. Resets to 0. */
  } field;
  uint32_t val;
} EM_LKUP_DRC_1_RLAT_t;
#endif /* GENHDR_STRUCT */

#endif /* _EM_LKUP_REGISTERS_H_ */

