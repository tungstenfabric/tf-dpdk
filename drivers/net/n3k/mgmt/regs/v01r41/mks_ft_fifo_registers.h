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

#ifndef _MKS_FT_FIFO_REGISTERS_H_
#define _MKS_FT_FIFO_REGISTERS_H_

/* Registers for the Flow Tracker FIFO block of Mt Kiska */

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

#define MKS_FT_FIFO_ACCESSTYPE          GENHDR_ACCESSTYPE_RW /*  Default access type. Access types defined above. */
#define MKS_FT_FIFO_REGWIDTH            32 /* Default width of register in bits */
#define MKS_FT_FIFO_ACCESSWIDTH         32 /* Default width of access word in bit */


/* Flow Tracket FIFO Control  */
#define MKS_FT_FIFO_FT_FIFO_CTRL_CFG                            0x00000000U
/*
 * Bank Dedicated Credits:
 *  Per Bank Dedicated credits. The same value is used for all banks.
 */
#define MKS_FT_FIFO_FT_FIFO_CTRL_CFG_DEDICATEDCREDITS_SHIFT     0
#define MKS_FT_FIFO_FT_FIFO_CTRL_CFG_DEDICATEDCREDITS_WIDTH     11
#define MKS_FT_FIFO_FT_FIFO_CTRL_CFG_DEDICATEDCREDITS_MASK      (0x7FFU << MKS_FT_FIFO_FT_FIFO_CTRL_CFG_DEDICATEDCREDITS_SHIFT)

/*
 * Shared Credits: The configured value should not exceed
 * 1024-(8*dedicatedcredits).
 */
#define MKS_FT_FIFO_FT_FIFO_CTRL_CFG_SHAREDCREDITS_SHIFT        11
#define MKS_FT_FIFO_FT_FIFO_CTRL_CFG_SHAREDCREDITS_WIDTH        11
#define MKS_FT_FIFO_FT_FIFO_CTRL_CFG_SHAREDCREDITS_MASK         (0x7FFU << MKS_FT_FIFO_FT_FIFO_CTRL_CFG_SHAREDCREDITS_SHIFT)

/*
 * Fifo mode when set 1, enables the Link list based FIFO mode,
 *  else the Flow tracker FIFO would use the native FIFO mode.
 */
#define MKS_FT_FIFO_FT_FIFO_CTRL_CFG_FIFOMODE_SHIFT             22
#define MKS_FT_FIFO_FT_FIFO_CTRL_CFG_FIFOMODE_WIDTH             1
#define MKS_FT_FIFO_FT_FIFO_CTRL_CFG_FIFOMODE_MASK              (0x1U << MKS_FT_FIFO_FT_FIFO_CTRL_CFG_FIFOMODE_SHIFT)

/*
 * Specify the TDM period in units of 1/[2^TDM_PER_FRAC_BITS] of the clock
 * period.
 */
#define MKS_FT_FIFO_FT_FIFO_CTRL_CFG_TDMPERIODSELECT_SHIFT      23
#define MKS_FT_FIFO_FT_FIFO_CTRL_CFG_TDMPERIODSELECT_WIDTH      9
#define MKS_FT_FIFO_FT_FIFO_CTRL_CFG_TDMPERIODSELECT_MASK       (0x1FFU << MKS_FT_FIFO_FT_FIFO_CTRL_CFG_TDMPERIODSELECT_SHIFT)

#ifdef GENHDR_STRUCT
/* Flow Tracket FIFO Control  */
typedef union {
  struct {
    uint32_t dedicatedcredits : 11;     /* Bank Dedicated Credits:
                                           Per Bank Dedicated credits. The same value is used for all banks. */
    uint32_t sharedcredits : 11;        /* Shared Credits: The configured value should not exceed
                                           1024-(8*dedicatedcredits). */
    uint32_t fifomode : 1;              /* Fifo mode when set 1, enables the Link list based FIFO mode,
                                           else the Flow tracker FIFO would use the native FIFO mode. */
    uint32_t tdmperiodselect : 9;       /* Specify the TDM period in units of 1/[2^TDM_PER_FRAC_BITS] of the clock
                                           period. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_CTRL_CFG_t;
#endif /* GENHDR_STRUCT */

/* Contains the WO controls for Statistic capture function */
#define MKS_FT_FIFO_FT_FIFO_STAT_CTRL0                          0x00000004U
/*
 * Starts the capture window. Write to this register would also clear the
 * results from the previous capture.
 */
#define MKS_FT_FIFO_FT_FIFO_STAT_CTRL0_STARTCAPTURE_ACCESSTYPE  GENHDR_ACCESSTYPE_WO
#define MKS_FT_FIFO_FT_FIFO_STAT_CTRL0_STARTCAPTURE_SHIFT       0
#define MKS_FT_FIFO_FT_FIFO_STAT_CTRL0_STARTCAPTURE_WIDTH       1
#define MKS_FT_FIFO_FT_FIFO_STAT_CTRL0_STARTCAPTURE_MASK        (0x1U << MKS_FT_FIFO_FT_FIFO_STAT_CTRL0_STARTCAPTURE_SHIFT)

/* Stops the capture window. */
#define MKS_FT_FIFO_FT_FIFO_STAT_CTRL0_STOPCAPTURE_ACCESSTYPE   GENHDR_ACCESSTYPE_WO
#define MKS_FT_FIFO_FT_FIFO_STAT_CTRL0_STOPCAPTURE_SHIFT        1
#define MKS_FT_FIFO_FT_FIFO_STAT_CTRL0_STOPCAPTURE_WIDTH        1
#define MKS_FT_FIFO_FT_FIFO_STAT_CTRL0_STOPCAPTURE_MASK         (0x1U << MKS_FT_FIFO_FT_FIFO_STAT_CTRL0_STOPCAPTURE_SHIFT)

#ifdef GENHDR_STRUCT
/* Contains the WO controls for Statistic capture function */
typedef union {
  struct {
    uint32_t startcapture : 1;  /* Starts the capture window. Write to this register would also clear the
                                   results from the previous capture. */
    uint32_t stopcapture : 1;   /* Stops the capture window. */
    uint32_t : 30;
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_STAT_CTRL0_t;
#endif /* GENHDR_STRUCT */

/* Contains the threshold controls for Statistic capture function */
#define MKS_FT_FIFO_FT_FIFO_STAT_CTRL1                          0x00000008U
/*
 * Statistic threshold: Used to flag conditions
 *  when the occupancy different between the banks exceeds this
 *  threshold value. This would help detect uneven traffic distribution across
 * banks.
 */
#define MKS_FT_FIFO_FT_FIFO_STAT_CTRL1_STATSTHRESHOLD_SHIFT     0
#define MKS_FT_FIFO_FT_FIFO_STAT_CTRL1_STATSTHRESHOLD_WIDTH     10
#define MKS_FT_FIFO_FT_FIFO_STAT_CTRL1_STATSTHRESHOLD_MASK      (0x3FFU << MKS_FT_FIFO_FT_FIFO_STAT_CTRL1_STATSTHRESHOLD_SHIFT)

/*
 * Water Mark threshold: Used to flag conditions when the occupancy of the bank
 * exceeds this threshold value.
 */
#define MKS_FT_FIFO_FT_FIFO_STAT_CTRL1_VMTHRESHOLD_SHIFT        10
#define MKS_FT_FIFO_FT_FIFO_STAT_CTRL1_VMTHRESHOLD_WIDTH        10
#define MKS_FT_FIFO_FT_FIFO_STAT_CTRL1_VMTHRESHOLD_MASK         (0x3FFU << MKS_FT_FIFO_FT_FIFO_STAT_CTRL1_VMTHRESHOLD_SHIFT)

#ifdef GENHDR_STRUCT
/* Contains the threshold controls for Statistic capture function */
typedef union {
  struct {
    uint32_t statsthreshold : 10;       /* Statistic threshold: Used to flag conditions
                                           when the occupancy different between the banks exceeds this
                                           threshold value. This would help detect uneven traffic distribution across
                                           banks. */
    uint32_t vmthreshold : 10;          /* Water Mark threshold: Used to flag conditions when the occupancy of the bank
                                           exceeds this threshold value. */
    uint32_t : 12;
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_STAT_CTRL1_t;
#endif /* GENHDR_STRUCT */

/* Used to capture the Statistic status */
#define MKS_FT_FIFO_FT_FIFO_STAT_STATUS0                                0x0000000CU
/*
 * This register contains status whenever the occupancy difference between the
 * banks exceeds the statsthreshold. 
 *  bit 0 -> reserved
 *  bit 1 -> bank 0 - bank 1 occupancy exceeded threshold
 *  bit 2 -> bank 0 - bank 2 occupancy exceeded threshold
 *  ..
 *  bit 7 -> bank 0 - bank 7 occupancy exceeded threshold
 *  bit 8 -> bank 1 - bank 0 occupancy exceeded threshold
 *  bit 9 -> reserved
 *  ..
 *  bit 15 -> bank 1 - bank 7 occupancy exceeded threshold
 *  bit 16 -> bank 2 - bank 0 occupancy exceeded threshold
 *  bit 17 -> bank 2 - bank 1 occupancy exceeded threshold
 *  bit 18 -> reserved
 *  ..
 *  bit 23 -> bank 2 - bank 7 occupancy exceeded threshold
 *  bit 24 -> bank 3 - bank 0 occupancy exceeded threshold
 *  bit 25 -> bank 3 - bank 1 occupancy exceeded threshold
 *  bit 26 -> bank 3 - bank 2 occupancy exceeded threshold
 *  bit 27 -> reserved
 *  ..
 *  bit 31 -> bank 3 - bank 7 occupancy exceeded threshold.
 */
#define MKS_FT_FIFO_FT_FIFO_STAT_STATUS0_STATSSTATUS_0_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_FT_FIFO_FT_FIFO_STAT_STATUS0_STATSSTATUS_0_SHIFT            0
#define MKS_FT_FIFO_FT_FIFO_STAT_STATUS0_STATSSTATUS_0_WIDTH            32
#define MKS_FT_FIFO_FT_FIFO_STAT_STATUS0_STATSSTATUS_0_MASK             (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_STAT_STATUS0_STATSSTATUS_0_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to capture the Statistic status */
typedef union {
  struct {
    uint32_t statsstatus_0 : 32;        /* This register contains status whenever the occupancy difference between the
                                           banks exceeds the statsthreshold.
                                           bit 0 -> reserved
                                           bit 1 -> bank 0 - bank 1 occupancy exceeded threshold
                                           bit 2 -> bank 0 - bank 2 occupancy exceeded threshold
                                           ..
                                           bit 7 -> bank 0 - bank 7 occupancy exceeded threshold
                                           bit 8 -> bank 1 - bank 0 occupancy exceeded threshold
                                           bit 9 -> reserved
                                           ..
                                           bit 15 -> bank 1 - bank 7 occupancy exceeded threshold
                                           bit 16 -> bank 2 - bank 0 occupancy exceeded threshold
                                           bit 17 -> bank 2 - bank 1 occupancy exceeded threshold
                                           bit 18 -> reserved
                                           ..
                                           bit 23 -> bank 2 - bank 7 occupancy exceeded threshold
                                           bit 24 -> bank 3 - bank 0 occupancy exceeded threshold
                                           bit 25 -> bank 3 - bank 1 occupancy exceeded threshold
                                           bit 26 -> bank 3 - bank 2 occupancy exceeded threshold
                                           bit 27 -> reserved
                                           ..
                                           bit 31 -> bank 3 - bank 7 occupancy exceeded threshold. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_STAT_STATUS0_t;
#endif /* GENHDR_STRUCT */

/* Used to capture the Statistic status */
#define MKS_FT_FIFO_FT_FIFO_STAT_STATUS1                                0x00000010U
/*
 * This register contains status whenever the occupancy difference between the
 * banks exceeds the statsthreshold. 
 *  bit 0 -> bank 4 - bank 0 occupancy exceeded threshold
 *  bit 1 -> bank 4 - bank 1 occupancy exceeded threshold
 *  bit 2 -> bank 4 - bank 2 occupancy exceeded threshold
 *  ..
 *  bit 4 -> reserved
 *  ..
 *  bit 7 -> bank 4 - bank 7 occupancy exceeded threshold
 *  bit 8 -> bank 5 - bank 0 occupancy exceeded threshold
 *  ..
 *  bit 13 -> reserved
 *  ..
 *  bit 15 -> bank 5 - bank 7 occupancy exceeded threshold
 *  bit 16 -> bank 6 - bank 0 occupancy exceeded threshold
 *  bit 17 -> bank 6 - bank 1 occupancy exceeded threshold
 *  ..
 *  bit 22 -> reserved
 *  bit 23 -> bank 6 - bank 7 occupancy exceeded threshold
 *  bit 24 -> bank 6 - bank 0 occupancy exceeded threshold
 *  bit 25 -> bank 6 - bank 1 occupancy exceeded threshold
 *  bit 26 -> bank 6 - bank 2 occupancy exceeded threshold
 *  ..
 *  bit 31 -> reserved.
 */
#define MKS_FT_FIFO_FT_FIFO_STAT_STATUS1_STATSSTATUS_1_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_FT_FIFO_FT_FIFO_STAT_STATUS1_STATSSTATUS_1_SHIFT            0
#define MKS_FT_FIFO_FT_FIFO_STAT_STATUS1_STATSSTATUS_1_WIDTH            32
#define MKS_FT_FIFO_FT_FIFO_STAT_STATUS1_STATSSTATUS_1_MASK             (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_STAT_STATUS1_STATSSTATUS_1_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to capture the Statistic status */
typedef union {
  struct {
    uint32_t statsstatus_1 : 32;        /* This register contains status whenever the occupancy difference between the
                                           banks exceeds the statsthreshold.
                                           bit 0 -> bank 4 - bank 0 occupancy exceeded threshold
                                           bit 1 -> bank 4 - bank 1 occupancy exceeded threshold
                                           bit 2 -> bank 4 - bank 2 occupancy exceeded threshold
                                           ..
                                           bit 4 -> reserved
                                           ..
                                           bit 7 -> bank 4 - bank 7 occupancy exceeded threshold
                                           bit 8 -> bank 5 - bank 0 occupancy exceeded threshold
                                           ..
                                           bit 13 -> reserved
                                           ..
                                           bit 15 -> bank 5 - bank 7 occupancy exceeded threshold
                                           bit 16 -> bank 6 - bank 0 occupancy exceeded threshold
                                           bit 17 -> bank 6 - bank 1 occupancy exceeded threshold
                                           ..
                                           bit 22 -> reserved
                                           bit 23 -> bank 6 - bank 7 occupancy exceeded threshold
                                           bit 24 -> bank 6 - bank 0 occupancy exceeded threshold
                                           bit 25 -> bank 6 - bank 1 occupancy exceeded threshold
                                           bit 26 -> bank 6 - bank 2 occupancy exceeded threshold
                                           ..
                                           bit 31 -> reserved. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_STAT_STATUS1_t;
#endif /* GENHDR_STRUCT */

/* Used to capture the Statistic status */
#define MKS_FT_FIFO_FT_FIFO_STAT_STATUS2                                0x00000014U
/*
 * This register contains status whenever the occupancy of each bank exceeds the
 * water mark threshold. 
 *  bit 0 -> bank 0 exceeded water mark threshold
 *  ..
 *  bit 7 -> bank 7 exceeded water mark threshold. 
 */
#define MKS_FT_FIFO_FT_FIFO_STAT_STATUS2_STATSSTATUS_2_ACCESSTYPE       GENHDR_ACCESSTYPE_RO
#define MKS_FT_FIFO_FT_FIFO_STAT_STATUS2_STATSSTATUS_2_SHIFT            0
#define MKS_FT_FIFO_FT_FIFO_STAT_STATUS2_STATSSTATUS_2_WIDTH            8
#define MKS_FT_FIFO_FT_FIFO_STAT_STATUS2_STATSSTATUS_2_MASK             (0xFFU << MKS_FT_FIFO_FT_FIFO_STAT_STATUS2_STATSSTATUS_2_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to capture the Statistic status */
typedef union {
  struct {
    uint32_t statsstatus_2 : 8;         /* This register contains status whenever the occupancy of each bank exceeds the
                                           water mark threshold.
                                           bit 0 -> bank 0 exceeded water mark threshold
                                           ..
                                           bit 7 -> bank 7 exceeded water mark threshold. */
    uint32_t : 24;
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_STAT_STATUS2_t;
#endif /* GENHDR_STRUCT */

/* Used to capture the Flow tracker fifo drop counter status for banks0  */
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS0                        0x00000018U
/* This register contains drop count staus for bank0.  */
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS0_DROP_Q0_SHIFT          0
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS0_DROP_Q0_WIDTH          32
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS0_DROP_Q0_MASK           (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_DROP_STATUS0_DROP_Q0_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to capture the Flow tracker fifo drop counter status for banks0  */
typedef union {
  struct {
    uint32_t drop_q0 : 32;      /* This register contains drop count staus for bank0. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_DROP_STATUS0_t;
#endif /* GENHDR_STRUCT */

/* Used to capture the Flow tracker fifo drop counter status for banks1  */
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS1                        0x0000001CU
/* This register contains drop count staus for bank1.  */
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS1_DROP_Q0_SHIFT          0
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS1_DROP_Q0_WIDTH          32
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS1_DROP_Q0_MASK           (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_DROP_STATUS1_DROP_Q0_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to capture the Flow tracker fifo drop counter status for banks1  */
typedef union {
  struct {
    uint32_t drop_q0 : 32;      /* This register contains drop count staus for bank1. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_DROP_STATUS1_t;
#endif /* GENHDR_STRUCT */

/* Used to capture the Flow tracker fifo drop counter status for banks2  */
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS2                        0x00000020U
/* This register contains drop count staus for bank2.  */
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS2_DROP_Q0_SHIFT          0
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS2_DROP_Q0_WIDTH          32
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS2_DROP_Q0_MASK           (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_DROP_STATUS2_DROP_Q0_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to capture the Flow tracker fifo drop counter status for banks2  */
typedef union {
  struct {
    uint32_t drop_q0 : 32;      /* This register contains drop count staus for bank2. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_DROP_STATUS2_t;
#endif /* GENHDR_STRUCT */

/* Used to capture the Flow tracker fifo drop counter status for banks3  */
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS3                        0x00000024U
/* This register contains drop count staus for bank3.  */
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS3_DROP_Q0_SHIFT          0
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS3_DROP_Q0_WIDTH          32
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS3_DROP_Q0_MASK           (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_DROP_STATUS3_DROP_Q0_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to capture the Flow tracker fifo drop counter status for banks3  */
typedef union {
  struct {
    uint32_t drop_q0 : 32;      /* This register contains drop count staus for bank3. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_DROP_STATUS3_t;
#endif /* GENHDR_STRUCT */

/* Used to capture the Flow tracker fifo drop counter status for banks4  */
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS4                        0x00000028U
/* This register contains drop count staus for bank4.  */
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS4_DROP_Q0_SHIFT          0
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS4_DROP_Q0_WIDTH          32
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS4_DROP_Q0_MASK           (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_DROP_STATUS4_DROP_Q0_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to capture the Flow tracker fifo drop counter status for banks4  */
typedef union {
  struct {
    uint32_t drop_q0 : 32;      /* This register contains drop count staus for bank4. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_DROP_STATUS4_t;
#endif /* GENHDR_STRUCT */

/* Used to capture the Flow tracker fifo drop counter status for banks5  */
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS5                        0x0000002CU
/* This register contains drop count staus for bank5.  */
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS5_DROP_Q0_SHIFT          0
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS5_DROP_Q0_WIDTH          32
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS5_DROP_Q0_MASK           (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_DROP_STATUS5_DROP_Q0_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to capture the Flow tracker fifo drop counter status for banks5  */
typedef union {
  struct {
    uint32_t drop_q0 : 32;      /* This register contains drop count staus for bank5. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_DROP_STATUS5_t;
#endif /* GENHDR_STRUCT */

/* Used to capture the Flow tracker fifo drop counter status for banks6  */
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS6                        0x00000030U
/* This register contains drop count staus for bank6.  */
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS6_DROP_Q0_SHIFT          0
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS6_DROP_Q0_WIDTH          32
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS6_DROP_Q0_MASK           (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_DROP_STATUS6_DROP_Q0_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to capture the Flow tracker fifo drop counter status for banks6  */
typedef union {
  struct {
    uint32_t drop_q0 : 32;      /* This register contains drop count staus for bank6. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_DROP_STATUS6_t;
#endif /* GENHDR_STRUCT */

/* Used to capture the Flow tracker fifo drop counter status for banks7  */
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS7                        0x00000034U
/* This register contains drop count staus for bank7.  */
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS7_DROP_Q0_SHIFT          0
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS7_DROP_Q0_WIDTH          32
#define MKS_FT_FIFO_FT_FIFO_DROP_STATUS7_DROP_Q0_MASK           (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_DROP_STATUS7_DROP_Q0_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to capture the Flow tracker fifo drop counter status for banks7  */
typedef union {
  struct {
    uint32_t drop_q0 : 32;      /* This register contains drop count staus for bank7. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_DROP_STATUS7_t;
#endif /* GENHDR_STRUCT */

/* Used to count the Flow tracker fifo input packets for bank0 */
#define MKS_FT_FIFO_FT_FIFO_INPKT0_CNT                          0x00000038U
/* This register contains input packet count on bank0.  */
#define MKS_FT_FIFO_FT_FIFO_INPKT0_CNT_INPKTCOUNT_SHIFT         0
#define MKS_FT_FIFO_FT_FIFO_INPKT0_CNT_INPKTCOUNT_WIDTH         32
#define MKS_FT_FIFO_FT_FIFO_INPKT0_CNT_INPKTCOUNT_MASK          (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_INPKT0_CNT_INPKTCOUNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to count the Flow tracker fifo input packets for bank0 */
typedef union {
  struct {
    uint32_t inpktcount : 32;   /* This register contains input packet count on bank0. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_INPKT0_CNT_t;
#endif /* GENHDR_STRUCT */

/* Used to count the Flow tracker fifo input packets for bank1 */
#define MKS_FT_FIFO_FT_FIFO_INPKT1_CNT                          0x0000003CU
/* This register contains input packet count on bank1.  */
#define MKS_FT_FIFO_FT_FIFO_INPKT1_CNT_INPKTCOUNT_SHIFT         0
#define MKS_FT_FIFO_FT_FIFO_INPKT1_CNT_INPKTCOUNT_WIDTH         32
#define MKS_FT_FIFO_FT_FIFO_INPKT1_CNT_INPKTCOUNT_MASK          (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_INPKT1_CNT_INPKTCOUNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to count the Flow tracker fifo input packets for bank1 */
typedef union {
  struct {
    uint32_t inpktcount : 32;   /* This register contains input packet count on bank1. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_INPKT1_CNT_t;
#endif /* GENHDR_STRUCT */

/* Used to count the Flow tracker fifo input packets for bank2 */
#define MKS_FT_FIFO_FT_FIFO_INPKT2_CNT                          0x00000040U
/* This register contains input packet count on bank2.  */
#define MKS_FT_FIFO_FT_FIFO_INPKT2_CNT_INPKTCOUNT_SHIFT         0
#define MKS_FT_FIFO_FT_FIFO_INPKT2_CNT_INPKTCOUNT_WIDTH         32
#define MKS_FT_FIFO_FT_FIFO_INPKT2_CNT_INPKTCOUNT_MASK          (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_INPKT2_CNT_INPKTCOUNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to count the Flow tracker fifo input packets for bank2 */
typedef union {
  struct {
    uint32_t inpktcount : 32;   /* This register contains input packet count on bank2. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_INPKT2_CNT_t;
#endif /* GENHDR_STRUCT */

/* Used to count the Flow tracker fifo input packets for bank3 */
#define MKS_FT_FIFO_FT_FIFO_INPKT3_CNT                          0x00000044U
/* This register contains input packet count on bank3.  */
#define MKS_FT_FIFO_FT_FIFO_INPKT3_CNT_INPKTCOUNT_SHIFT         0
#define MKS_FT_FIFO_FT_FIFO_INPKT3_CNT_INPKTCOUNT_WIDTH         32
#define MKS_FT_FIFO_FT_FIFO_INPKT3_CNT_INPKTCOUNT_MASK          (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_INPKT3_CNT_INPKTCOUNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to count the Flow tracker fifo input packets for bank3 */
typedef union {
  struct {
    uint32_t inpktcount : 32;   /* This register contains input packet count on bank3. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_INPKT3_CNT_t;
#endif /* GENHDR_STRUCT */

/* Used to count the Flow tracker fifo input packets for bank4 */
#define MKS_FT_FIFO_FT_FIFO_INPKT4_CNT                          0x00000048U
/* This register contains input packet count on bank4.  */
#define MKS_FT_FIFO_FT_FIFO_INPKT4_CNT_INPKTCOUNT_SHIFT         0
#define MKS_FT_FIFO_FT_FIFO_INPKT4_CNT_INPKTCOUNT_WIDTH         32
#define MKS_FT_FIFO_FT_FIFO_INPKT4_CNT_INPKTCOUNT_MASK          (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_INPKT4_CNT_INPKTCOUNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to count the Flow tracker fifo input packets for bank4 */
typedef union {
  struct {
    uint32_t inpktcount : 32;   /* This register contains input packet count on bank4. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_INPKT4_CNT_t;
#endif /* GENHDR_STRUCT */

/* Used to count the Flow tracker fifo input packets for bank5 */
#define MKS_FT_FIFO_FT_FIFO_INPKT5_CNT                          0x0000004CU
/* This register contains input packet count on bank5.  */
#define MKS_FT_FIFO_FT_FIFO_INPKT5_CNT_INPKTCOUNT_SHIFT         0
#define MKS_FT_FIFO_FT_FIFO_INPKT5_CNT_INPKTCOUNT_WIDTH         32
#define MKS_FT_FIFO_FT_FIFO_INPKT5_CNT_INPKTCOUNT_MASK          (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_INPKT5_CNT_INPKTCOUNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to count the Flow tracker fifo input packets for bank5 */
typedef union {
  struct {
    uint32_t inpktcount : 32;   /* This register contains input packet count on bank5. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_INPKT5_CNT_t;
#endif /* GENHDR_STRUCT */

/* Used to count the Flow tracker fifo input packets for bank6 */
#define MKS_FT_FIFO_FT_FIFO_INPKT6_CNT                          0x00000050U
/* This register contains input packet count on bank6.  */
#define MKS_FT_FIFO_FT_FIFO_INPKT6_CNT_INPKTCOUNT_SHIFT         0
#define MKS_FT_FIFO_FT_FIFO_INPKT6_CNT_INPKTCOUNT_WIDTH         32
#define MKS_FT_FIFO_FT_FIFO_INPKT6_CNT_INPKTCOUNT_MASK          (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_INPKT6_CNT_INPKTCOUNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to count the Flow tracker fifo input packets for bank6 */
typedef union {
  struct {
    uint32_t inpktcount : 32;   /* This register contains input packet count on bank6. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_INPKT6_CNT_t;
#endif /* GENHDR_STRUCT */

/* Used to count the Flow tracker fifo input packets for bank7 */
#define MKS_FT_FIFO_FT_FIFO_INPKT7_CNT                          0x00000054U
/* This register contains input packet count on bank7.  */
#define MKS_FT_FIFO_FT_FIFO_INPKT7_CNT_INPKTCOUNT_SHIFT         0
#define MKS_FT_FIFO_FT_FIFO_INPKT7_CNT_INPKTCOUNT_WIDTH         32
#define MKS_FT_FIFO_FT_FIFO_INPKT7_CNT_INPKTCOUNT_MASK          (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_INPKT7_CNT_INPKTCOUNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to count the Flow tracker fifo input packets for bank7 */
typedef union {
  struct {
    uint32_t inpktcount : 32;   /* This register contains input packet count on bank7. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_INPKT7_CNT_t;
#endif /* GENHDR_STRUCT */

/* Used to count the Flow tracker fifo output packets for bank0 */
#define MKS_FT_FIFO_FT_FIFO_OUTPKT0_CNT                         0x00000058U
/* This register contains output packet count on bank0.  */
#define MKS_FT_FIFO_FT_FIFO_OUTPKT0_CNT_OUTPKTCOUNT_SHIFT       0
#define MKS_FT_FIFO_FT_FIFO_OUTPKT0_CNT_OUTPKTCOUNT_WIDTH       32
#define MKS_FT_FIFO_FT_FIFO_OUTPKT0_CNT_OUTPKTCOUNT_MASK        (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_OUTPKT0_CNT_OUTPKTCOUNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to count the Flow tracker fifo output packets for bank0 */
typedef union {
  struct {
    uint32_t outpktcount : 32;  /* This register contains output packet count on bank0. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_OUTPKT0_CNT_t;
#endif /* GENHDR_STRUCT */

/* Used to count the Flow tracker fifo output packets for bank1 */
#define MKS_FT_FIFO_FT_FIFO_OUTPKT1_CNT                         0x0000005CU
/* This register contains output packet count on bank1.  */
#define MKS_FT_FIFO_FT_FIFO_OUTPKT1_CNT_OUTPKTCOUNT_SHIFT       0
#define MKS_FT_FIFO_FT_FIFO_OUTPKT1_CNT_OUTPKTCOUNT_WIDTH       32
#define MKS_FT_FIFO_FT_FIFO_OUTPKT1_CNT_OUTPKTCOUNT_MASK        (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_OUTPKT1_CNT_OUTPKTCOUNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to count the Flow tracker fifo output packets for bank1 */
typedef union {
  struct {
    uint32_t outpktcount : 32;  /* This register contains output packet count on bank1. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_OUTPKT1_CNT_t;
#endif /* GENHDR_STRUCT */

/* Used to count the Flow tracker fifo output packets for bank2 */
#define MKS_FT_FIFO_FT_FIFO_OUTPKT2_CNT                         0x00000060U
/* This register contains output packet count on bank2.  */
#define MKS_FT_FIFO_FT_FIFO_OUTPKT2_CNT_OUTPKTCOUNT_SHIFT       0
#define MKS_FT_FIFO_FT_FIFO_OUTPKT2_CNT_OUTPKTCOUNT_WIDTH       32
#define MKS_FT_FIFO_FT_FIFO_OUTPKT2_CNT_OUTPKTCOUNT_MASK        (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_OUTPKT2_CNT_OUTPKTCOUNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to count the Flow tracker fifo output packets for bank2 */
typedef union {
  struct {
    uint32_t outpktcount : 32;  /* This register contains output packet count on bank2. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_OUTPKT2_CNT_t;
#endif /* GENHDR_STRUCT */

/* Used to count the Flow tracker fifo output packets for bank3 */
#define MKS_FT_FIFO_FT_FIFO_OUTPKT3_CNT                         0x00000064U
/* This register contains output packet count on bank3.  */
#define MKS_FT_FIFO_FT_FIFO_OUTPKT3_CNT_OUTPKTCOUNT_SHIFT       0
#define MKS_FT_FIFO_FT_FIFO_OUTPKT3_CNT_OUTPKTCOUNT_WIDTH       32
#define MKS_FT_FIFO_FT_FIFO_OUTPKT3_CNT_OUTPKTCOUNT_MASK        (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_OUTPKT3_CNT_OUTPKTCOUNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to count the Flow tracker fifo output packets for bank3 */
typedef union {
  struct {
    uint32_t outpktcount : 32;  /* This register contains output packet count on bank3. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_OUTPKT3_CNT_t;
#endif /* GENHDR_STRUCT */

/* Used to count the Flow tracker fifo output packets for bank4 */
#define MKS_FT_FIFO_FT_FIFO_OUTPKT4_CNT                         0x00000068U
/* This register contains output packet count on bank4.  */
#define MKS_FT_FIFO_FT_FIFO_OUTPKT4_CNT_OUTPKTCOUNT_SHIFT       0
#define MKS_FT_FIFO_FT_FIFO_OUTPKT4_CNT_OUTPKTCOUNT_WIDTH       32
#define MKS_FT_FIFO_FT_FIFO_OUTPKT4_CNT_OUTPKTCOUNT_MASK        (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_OUTPKT4_CNT_OUTPKTCOUNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to count the Flow tracker fifo output packets for bank4 */
typedef union {
  struct {
    uint32_t outpktcount : 32;  /* This register contains output packet count on bank4. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_OUTPKT4_CNT_t;
#endif /* GENHDR_STRUCT */

/* Used to count the Flow tracker fifo output packets for bank5 */
#define MKS_FT_FIFO_FT_FIFO_OUTPKT5_CNT                         0x0000006CU
/* This register contains output packet count on bank5.  */
#define MKS_FT_FIFO_FT_FIFO_OUTPKT5_CNT_OUTPKTCOUNT_SHIFT       0
#define MKS_FT_FIFO_FT_FIFO_OUTPKT5_CNT_OUTPKTCOUNT_WIDTH       32
#define MKS_FT_FIFO_FT_FIFO_OUTPKT5_CNT_OUTPKTCOUNT_MASK        (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_OUTPKT5_CNT_OUTPKTCOUNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to count the Flow tracker fifo output packets for bank5 */
typedef union {
  struct {
    uint32_t outpktcount : 32;  /* This register contains output packet count on bank5. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_OUTPKT5_CNT_t;
#endif /* GENHDR_STRUCT */

/* Used to count the Flow tracker fifo output packets for bank6 */
#define MKS_FT_FIFO_FT_FIFO_OUTPKT6_CNT                         0x00000070U
/* This register contains output packet count on bank6.  */
#define MKS_FT_FIFO_FT_FIFO_OUTPKT6_CNT_OUTPKTCOUNT_SHIFT       0
#define MKS_FT_FIFO_FT_FIFO_OUTPKT6_CNT_OUTPKTCOUNT_WIDTH       32
#define MKS_FT_FIFO_FT_FIFO_OUTPKT6_CNT_OUTPKTCOUNT_MASK        (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_OUTPKT6_CNT_OUTPKTCOUNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to count the Flow tracker fifo output packets for bank6 */
typedef union {
  struct {
    uint32_t outpktcount : 32;  /* This register contains output packet count on bank6. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_OUTPKT6_CNT_t;
#endif /* GENHDR_STRUCT */

/* Used to count the Flow tracker fifo output packets for bank7 */
#define MKS_FT_FIFO_FT_FIFO_OUTPKT7_CNT                         0x00000074U
/* This register contains output packet count on bank7.  */
#define MKS_FT_FIFO_FT_FIFO_OUTPKT7_CNT_OUTPKTCOUNT_SHIFT       0
#define MKS_FT_FIFO_FT_FIFO_OUTPKT7_CNT_OUTPKTCOUNT_WIDTH       32
#define MKS_FT_FIFO_FT_FIFO_OUTPKT7_CNT_OUTPKTCOUNT_MASK        (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_OUTPKT7_CNT_OUTPKTCOUNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to count the Flow tracker fifo output packets for bank7 */
typedef union {
  struct {
    uint32_t outpktcount : 32;  /* This register contains output packet count on bank7. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_OUTPKT7_CNT_t;
#endif /* GENHDR_STRUCT */

/* Used to provide the Flow tracker fifo occupancy for bank0, bank1 */
#define MKS_FT_FIFO_FT_FIFO_DEPTH0_STATS                        0x00000078U
/* This register contains Flow tracker fifo occupancy on bank0.  */
#define MKS_FT_FIFO_FT_FIFO_DEPTH0_STATS_BANKCNT_0_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define MKS_FT_FIFO_FT_FIFO_DEPTH0_STATS_BANKCNT_0_SHIFT        0
#define MKS_FT_FIFO_FT_FIFO_DEPTH0_STATS_BANKCNT_0_WIDTH        11
#define MKS_FT_FIFO_FT_FIFO_DEPTH0_STATS_BANKCNT_0_MASK         (0x7FFU << MKS_FT_FIFO_FT_FIFO_DEPTH0_STATS_BANKCNT_0_SHIFT)

/* This register contains Flow tracker fifo occupancy on bank1.  */
#define MKS_FT_FIFO_FT_FIFO_DEPTH0_STATS_BANKCNT_1_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define MKS_FT_FIFO_FT_FIFO_DEPTH0_STATS_BANKCNT_1_SHIFT        11
#define MKS_FT_FIFO_FT_FIFO_DEPTH0_STATS_BANKCNT_1_WIDTH        11
#define MKS_FT_FIFO_FT_FIFO_DEPTH0_STATS_BANKCNT_1_MASK         (0x7FFU << MKS_FT_FIFO_FT_FIFO_DEPTH0_STATS_BANKCNT_1_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to provide the Flow tracker fifo occupancy for bank0, bank1 */
typedef union {
  struct {
    uint32_t bankcnt_0 : 11;    /* This register contains Flow tracker fifo occupancy on bank0. */
    uint32_t bankcnt_1 : 11;    /* This register contains Flow tracker fifo occupancy on bank1. */
    uint32_t : 10;
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_DEPTH0_STATS_t;
#endif /* GENHDR_STRUCT */

/* Used to provide the Flow tracker fifo occupancy for bank2,bank3 */
#define MKS_FT_FIFO_FT_FIFO_DEPTH1_STATS                        0x0000007CU
/* This register contains Flow tracker fifo occupancy on bank2.  */
#define MKS_FT_FIFO_FT_FIFO_DEPTH1_STATS_BANKCNT_2_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define MKS_FT_FIFO_FT_FIFO_DEPTH1_STATS_BANKCNT_2_SHIFT        0
#define MKS_FT_FIFO_FT_FIFO_DEPTH1_STATS_BANKCNT_2_WIDTH        11
#define MKS_FT_FIFO_FT_FIFO_DEPTH1_STATS_BANKCNT_2_MASK         (0x7FFU << MKS_FT_FIFO_FT_FIFO_DEPTH1_STATS_BANKCNT_2_SHIFT)

/* This register contains Flow tracker fifo occupancy on bank3.  */
#define MKS_FT_FIFO_FT_FIFO_DEPTH1_STATS_BANKCNT_3_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define MKS_FT_FIFO_FT_FIFO_DEPTH1_STATS_BANKCNT_3_SHIFT        11
#define MKS_FT_FIFO_FT_FIFO_DEPTH1_STATS_BANKCNT_3_WIDTH        11
#define MKS_FT_FIFO_FT_FIFO_DEPTH1_STATS_BANKCNT_3_MASK         (0x7FFU << MKS_FT_FIFO_FT_FIFO_DEPTH1_STATS_BANKCNT_3_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to provide the Flow tracker fifo occupancy for bank2,bank3 */
typedef union {
  struct {
    uint32_t bankcnt_2 : 11;    /* This register contains Flow tracker fifo occupancy on bank2. */
    uint32_t bankcnt_3 : 11;    /* This register contains Flow tracker fifo occupancy on bank3. */
    uint32_t : 10;
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_DEPTH1_STATS_t;
#endif /* GENHDR_STRUCT */

/* Used to provide the Flow tracker fifo occupancy for bank4, bank5 */
#define MKS_FT_FIFO_FT_FIFO_DEPTH2_STATS                        0x00000080U
/* This register contains Flow tracker fifo occupancy on bank4.  */
#define MKS_FT_FIFO_FT_FIFO_DEPTH2_STATS_BANKCNT_4_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define MKS_FT_FIFO_FT_FIFO_DEPTH2_STATS_BANKCNT_4_SHIFT        0
#define MKS_FT_FIFO_FT_FIFO_DEPTH2_STATS_BANKCNT_4_WIDTH        11
#define MKS_FT_FIFO_FT_FIFO_DEPTH2_STATS_BANKCNT_4_MASK         (0x7FFU << MKS_FT_FIFO_FT_FIFO_DEPTH2_STATS_BANKCNT_4_SHIFT)

/* This register contains Flow tracker fifo occupancy on bank5.  */
#define MKS_FT_FIFO_FT_FIFO_DEPTH2_STATS_BANKCNT_5_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define MKS_FT_FIFO_FT_FIFO_DEPTH2_STATS_BANKCNT_5_SHIFT        11
#define MKS_FT_FIFO_FT_FIFO_DEPTH2_STATS_BANKCNT_5_WIDTH        11
#define MKS_FT_FIFO_FT_FIFO_DEPTH2_STATS_BANKCNT_5_MASK         (0x7FFU << MKS_FT_FIFO_FT_FIFO_DEPTH2_STATS_BANKCNT_5_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to provide the Flow tracker fifo occupancy for bank4, bank5 */
typedef union {
  struct {
    uint32_t bankcnt_4 : 11;    /* This register contains Flow tracker fifo occupancy on bank4. */
    uint32_t bankcnt_5 : 11;    /* This register contains Flow tracker fifo occupancy on bank5. */
    uint32_t : 10;
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_DEPTH2_STATS_t;
#endif /* GENHDR_STRUCT */

/* Used to provide the Flow tracker fifo occupancy for bank6, bank7 */
#define MKS_FT_FIFO_FT_FIFO_DEPTH3_STATS                        0x00000084U
/* This register contains Flow tracker fifo occupancy on bank6.  */
#define MKS_FT_FIFO_FT_FIFO_DEPTH3_STATS_BANKCNT_6_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define MKS_FT_FIFO_FT_FIFO_DEPTH3_STATS_BANKCNT_6_SHIFT        0
#define MKS_FT_FIFO_FT_FIFO_DEPTH3_STATS_BANKCNT_6_WIDTH        11
#define MKS_FT_FIFO_FT_FIFO_DEPTH3_STATS_BANKCNT_6_MASK         (0x7FFU << MKS_FT_FIFO_FT_FIFO_DEPTH3_STATS_BANKCNT_6_SHIFT)

/* This register contains Flow tracker fifo occupancy on bank7.  */
#define MKS_FT_FIFO_FT_FIFO_DEPTH3_STATS_BANKCNT_7_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define MKS_FT_FIFO_FT_FIFO_DEPTH3_STATS_BANKCNT_7_SHIFT        11
#define MKS_FT_FIFO_FT_FIFO_DEPTH3_STATS_BANKCNT_7_WIDTH        11
#define MKS_FT_FIFO_FT_FIFO_DEPTH3_STATS_BANKCNT_7_MASK         (0x7FFU << MKS_FT_FIFO_FT_FIFO_DEPTH3_STATS_BANKCNT_7_SHIFT)

#ifdef GENHDR_STRUCT
/* Used to provide the Flow tracker fifo occupancy for bank6, bank7 */
typedef union {
  struct {
    uint32_t bankcnt_6 : 11;    /* This register contains Flow tracker fifo occupancy on bank6. */
    uint32_t bankcnt_7 : 11;    /* This register contains Flow tracker fifo occupancy on bank7. */
    uint32_t : 10;
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_DEPTH3_STATS_t;
#endif /* GENHDR_STRUCT */

/*
 * Used to capture the Flow tracker fifo max occupancy during stats capture
 * cycle for bank0, bank1
 */
#define MKS_FT_FIFO_FT_FIFO_MAX0_STATS                          0x00000088U
/* This register contains Flow tracker fifo max occupancy on bank0.  */
#define MKS_FT_FIFO_FT_FIFO_MAX0_STATS_BANKCNT_0_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_FT_FIFO_FT_FIFO_MAX0_STATS_BANKCNT_0_SHIFT          0
#define MKS_FT_FIFO_FT_FIFO_MAX0_STATS_BANKCNT_0_WIDTH          11
#define MKS_FT_FIFO_FT_FIFO_MAX0_STATS_BANKCNT_0_MASK           (0x7FFU << MKS_FT_FIFO_FT_FIFO_MAX0_STATS_BANKCNT_0_SHIFT)

/* This register contains Flow tracker fifo max occupancy on bank1.  */
#define MKS_FT_FIFO_FT_FIFO_MAX0_STATS_BANKCNT_1_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_FT_FIFO_FT_FIFO_MAX0_STATS_BANKCNT_1_SHIFT          11
#define MKS_FT_FIFO_FT_FIFO_MAX0_STATS_BANKCNT_1_WIDTH          11
#define MKS_FT_FIFO_FT_FIFO_MAX0_STATS_BANKCNT_1_MASK           (0x7FFU << MKS_FT_FIFO_FT_FIFO_MAX0_STATS_BANKCNT_1_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Used to capture the Flow tracker fifo max occupancy during stats capture
 * cycle for bank0, bank1
 */
typedef union {
  struct {
    uint32_t bankcnt_0 : 11;    /* This register contains Flow tracker fifo max occupancy on bank0. */
    uint32_t bankcnt_1 : 11;    /* This register contains Flow tracker fifo max occupancy on bank1. */
    uint32_t : 10;
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_MAX0_STATS_t;
#endif /* GENHDR_STRUCT */

/*
 * Used to capture the Flow tracker fifo max occupancy during stats capture
 * cycle for bank2, bank3
 */
#define MKS_FT_FIFO_FT_FIFO_MAX1_STATS                          0x0000008CU
/* This register contains Flow tracker fifo max occupancy on bank2.  */
#define MKS_FT_FIFO_FT_FIFO_MAX1_STATS_BANKCNT_2_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_FT_FIFO_FT_FIFO_MAX1_STATS_BANKCNT_2_SHIFT          0
#define MKS_FT_FIFO_FT_FIFO_MAX1_STATS_BANKCNT_2_WIDTH          11
#define MKS_FT_FIFO_FT_FIFO_MAX1_STATS_BANKCNT_2_MASK           (0x7FFU << MKS_FT_FIFO_FT_FIFO_MAX1_STATS_BANKCNT_2_SHIFT)

/* This register contains Flow tracker fifo max occupancy on bank3.  */
#define MKS_FT_FIFO_FT_FIFO_MAX1_STATS_BANKCNT_3_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_FT_FIFO_FT_FIFO_MAX1_STATS_BANKCNT_3_SHIFT          11
#define MKS_FT_FIFO_FT_FIFO_MAX1_STATS_BANKCNT_3_WIDTH          11
#define MKS_FT_FIFO_FT_FIFO_MAX1_STATS_BANKCNT_3_MASK           (0x7FFU << MKS_FT_FIFO_FT_FIFO_MAX1_STATS_BANKCNT_3_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Used to capture the Flow tracker fifo max occupancy during stats capture
 * cycle for bank2, bank3
 */
typedef union {
  struct {
    uint32_t bankcnt_2 : 11;    /* This register contains Flow tracker fifo max occupancy on bank2. */
    uint32_t bankcnt_3 : 11;    /* This register contains Flow tracker fifo max occupancy on bank3. */
    uint32_t : 10;
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_MAX1_STATS_t;
#endif /* GENHDR_STRUCT */

/*
 * Used to capture the Flow tracker fifo max occupancy during stats capture
 * cycle for bank4, bank5
 */
#define MKS_FT_FIFO_FT_FIFO_MAX2_STATS                          0x00000090U
/* This register contains Flow tracker fifo max occupancy on bank4.  */
#define MKS_FT_FIFO_FT_FIFO_MAX2_STATS_BANKCNT_4_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_FT_FIFO_FT_FIFO_MAX2_STATS_BANKCNT_4_SHIFT          0
#define MKS_FT_FIFO_FT_FIFO_MAX2_STATS_BANKCNT_4_WIDTH          11
#define MKS_FT_FIFO_FT_FIFO_MAX2_STATS_BANKCNT_4_MASK           (0x7FFU << MKS_FT_FIFO_FT_FIFO_MAX2_STATS_BANKCNT_4_SHIFT)

/* This register contains Flow tracker fifo max occupancy on bank5.  */
#define MKS_FT_FIFO_FT_FIFO_MAX2_STATS_BANKCNT_5_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_FT_FIFO_FT_FIFO_MAX2_STATS_BANKCNT_5_SHIFT          11
#define MKS_FT_FIFO_FT_FIFO_MAX2_STATS_BANKCNT_5_WIDTH          11
#define MKS_FT_FIFO_FT_FIFO_MAX2_STATS_BANKCNT_5_MASK           (0x7FFU << MKS_FT_FIFO_FT_FIFO_MAX2_STATS_BANKCNT_5_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Used to capture the Flow tracker fifo max occupancy during stats capture
 * cycle for bank4, bank5
 */
typedef union {
  struct {
    uint32_t bankcnt_4 : 11;    /* This register contains Flow tracker fifo max occupancy on bank4. */
    uint32_t bankcnt_5 : 11;    /* This register contains Flow tracker fifo max occupancy on bank5. */
    uint32_t : 10;
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_MAX2_STATS_t;
#endif /* GENHDR_STRUCT */

/*
 * Used to capture the Flow tracker fifo max occupancy during stats capture
 * cycle for bank6, bank7
 */
#define MKS_FT_FIFO_FT_FIFO_MAX3_STATS                          0x00000094U
/* This register contains Flow tracker fifo max occupancy on bank6.  */
#define MKS_FT_FIFO_FT_FIFO_MAX3_STATS_BANKCNT_6_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_FT_FIFO_FT_FIFO_MAX3_STATS_BANKCNT_6_SHIFT          0
#define MKS_FT_FIFO_FT_FIFO_MAX3_STATS_BANKCNT_6_WIDTH          11
#define MKS_FT_FIFO_FT_FIFO_MAX3_STATS_BANKCNT_6_MASK           (0x7FFU << MKS_FT_FIFO_FT_FIFO_MAX3_STATS_BANKCNT_6_SHIFT)

/* This register contains Flow tracker fifo max occupancy on bank7.  */
#define MKS_FT_FIFO_FT_FIFO_MAX3_STATS_BANKCNT_7_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_FT_FIFO_FT_FIFO_MAX3_STATS_BANKCNT_7_SHIFT          11
#define MKS_FT_FIFO_FT_FIFO_MAX3_STATS_BANKCNT_7_WIDTH          11
#define MKS_FT_FIFO_FT_FIFO_MAX3_STATS_BANKCNT_7_MASK           (0x7FFU << MKS_FT_FIFO_FT_FIFO_MAX3_STATS_BANKCNT_7_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Used to capture the Flow tracker fifo max occupancy during stats capture
 * cycle for bank6, bank7
 */
typedef union {
  struct {
    uint32_t bankcnt_6 : 11;    /* This register contains Flow tracker fifo max occupancy on bank6. */
    uint32_t bankcnt_7 : 11;    /* This register contains Flow tracker fifo max occupancy on bank7. */
    uint32_t : 10;
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_MAX3_STATS_t;
#endif /* GENHDR_STRUCT */

/*
 * Used to capture the Flow tracker fifo error pkt drop counter status for
 * banks0 
 */
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR0                   0x00000098U
/* This register contains error pkt drop count staus for bank0.  */
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR0_DROPCNT_SHIFT     0
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR0_DROPCNT_WIDTH     32
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR0_DROPCNT_MASK      (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_DROP_ERR0_DROPCNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Used to capture the Flow tracker fifo error pkt drop counter status for
 * banks0 
 */
typedef union {
  struct {
    uint32_t dropcnt : 32;      /* This register contains error pkt drop count staus for bank0. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_DROP_ERR0_t;
#endif /* GENHDR_STRUCT */

/*
 * Used to capture the Flow tracker fifo error pkt drop counter status for
 * banks1 
 */
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR1                   0x0000009CU
/* This register contains error pkt drop count staus for bank1.  */
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR1_DROPCNT_SHIFT     0
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR1_DROPCNT_WIDTH     32
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR1_DROPCNT_MASK      (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_DROP_ERR1_DROPCNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Used to capture the Flow tracker fifo error pkt drop counter status for
 * banks1 
 */
typedef union {
  struct {
    uint32_t dropcnt : 32;      /* This register contains error pkt drop count staus for bank1. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_DROP_ERR1_t;
#endif /* GENHDR_STRUCT */

/*
 * Used to capture the Flow tracker fifo error pkt drop counter status for
 * banks2 
 */
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR2                   0x000000A0U
/* This register contains error pkt drop count staus for bank2.  */
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR2_DROPCNT_SHIFT     0
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR2_DROPCNT_WIDTH     32
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR2_DROPCNT_MASK      (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_DROP_ERR2_DROPCNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Used to capture the Flow tracker fifo error pkt drop counter status for
 * banks2 
 */
typedef union {
  struct {
    uint32_t dropcnt : 32;      /* This register contains error pkt drop count staus for bank2. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_DROP_ERR2_t;
#endif /* GENHDR_STRUCT */

/*
 * Used to capture the Flow tracker fifo error pkt drop counter status for
 * banks3 
 */
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR3                   0x000000A4U
/* This register contains error pkt drop count staus for bank3.  */
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR3_DROPCNT_SHIFT     0
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR3_DROPCNT_WIDTH     32
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR3_DROPCNT_MASK      (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_DROP_ERR3_DROPCNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Used to capture the Flow tracker fifo error pkt drop counter status for
 * banks3 
 */
typedef union {
  struct {
    uint32_t dropcnt : 32;      /* This register contains error pkt drop count staus for bank3. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_DROP_ERR3_t;
#endif /* GENHDR_STRUCT */

/*
 * Used to capture the Flow tracker fifo error pkt drop counter status for
 * banks4 
 */
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR4                   0x000000A8U
/* This register contains error pkt drop count staus for bank4.  */
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR4_DROPCNT_SHIFT     0
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR4_DROPCNT_WIDTH     32
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR4_DROPCNT_MASK      (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_DROP_ERR4_DROPCNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Used to capture the Flow tracker fifo error pkt drop counter status for
 * banks4 
 */
typedef union {
  struct {
    uint32_t dropcnt : 32;      /* This register contains error pkt drop count staus for bank4. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_DROP_ERR4_t;
#endif /* GENHDR_STRUCT */

/*
 * Used to capture the Flow tracker fifo error pkt drop counter status for
 * banks5 
 */
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR5                   0x000000ACU
/* This register contains error pkt drop count staus for bank5.  */
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR5_DROPCNT_SHIFT     0
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR5_DROPCNT_WIDTH     32
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR5_DROPCNT_MASK      (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_DROP_ERR5_DROPCNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Used to capture the Flow tracker fifo error pkt drop counter status for
 * banks5 
 */
typedef union {
  struct {
    uint32_t dropcnt : 32;      /* This register contains error pkt drop count staus for bank5. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_DROP_ERR5_t;
#endif /* GENHDR_STRUCT */

/*
 * Used to capture the Flow tracker fifo error pkt drop counter status for
 * banks6 
 */
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR6                   0x000000B0U
/* This register contains error pkt drop count staus for bank6.  */
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR6_DROPCNT_SHIFT     0
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR6_DROPCNT_WIDTH     32
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR6_DROPCNT_MASK      (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_DROP_ERR6_DROPCNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Used to capture the Flow tracker fifo error pkt drop counter status for
 * banks6 
 */
typedef union {
  struct {
    uint32_t dropcnt : 32;      /* This register contains error pkt drop count staus for bank6. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_DROP_ERR6_t;
#endif /* GENHDR_STRUCT */

/*
 * Used to capture the Flow tracker fifo error pkt drop counter status for
 * banks7 
 */
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR7                   0x000000B4U
/* This register contains error pkt drop count staus for bank7.  */
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR7_DROPCNT_SHIFT     0
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR7_DROPCNT_WIDTH     32
#define MKS_FT_FIFO_FT_FIFO_DROP_ERR7_DROPCNT_MASK      (0xFFFFFFFFU << MKS_FT_FIFO_FT_FIFO_DROP_ERR7_DROPCNT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Used to capture the Flow tracker fifo error pkt drop counter status for
 * banks7 
 */
typedef union {
  struct {
    uint32_t dropcnt : 32;      /* This register contains error pkt drop count staus for bank7. */
  } field;
  uint32_t val;
} MKS_FT_FIFO_FT_FIFO_DROP_ERR7_t;
#endif /* GENHDR_STRUCT */

#endif /* _MKS_FT_FIFO_REGISTERS_H_ */

