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

#ifndef _MKS_PM_REGISTERS_H_
#define _MKS_PM_REGISTERS_H_

/* OVS Packet Memory */

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

#define MKS_PM_ACCESSTYPE               GENHDR_ACCESSTYPE_RW /*  Default access type. Access types defined above. */
#define MKS_PM_REGWIDTH                 32 /* Default width of register in bits */
#define MKS_PM_ACCESSWIDTH              32 /* Default width of access word in bit */


/* General purpose RW register intended for debug use */
#define MKS_PM_PM_SCRATCH                       0x00000000U
/* Scratch data. */
#define MKS_PM_PM_SCRATCH_DATA_SHIFT            0
#define MKS_PM_PM_SCRATCH_DATA_WIDTH            32
#define MKS_PM_PM_SCRATCH_DATA_MASK             (0xFFFFFFFFU << MKS_PM_PM_SCRATCH_DATA_SHIFT)

#ifdef GENHDR_STRUCT
/* General purpose RW register intended for debug use */
typedef union {
  struct {
    uint32_t data : 32; /* Scratch data. */
  } field;
  uint32_t val;
} MKS_PM_PM_SCRATCH_t;
#endif /* GENHDR_STRUCT */

/* Packet Segment Counts */
#define MKS_PM_PM_SEG_CNT                       0x00000004U
/* Number of availble segments left in Packet Memmory. */
#define MKS_PM_PM_SEG_CNT_CUR_CNT_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define MKS_PM_PM_SEG_CNT_CUR_CNT_SHIFT         0
#define MKS_PM_PM_SEG_CNT_CUR_CNT_WIDTH         12
#define MKS_PM_PM_SEG_CNT_CUR_CNT_MASK          (0xFFFU << MKS_PM_PM_SEG_CNT_CUR_CNT_SHIFT)

/* Minimum value of segment count seen since last read to this register. */
#define MKS_PM_PM_SEG_CNT_MIN_CNT_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define MKS_PM_PM_SEG_CNT_MIN_CNT_SHIFT         16
#define MKS_PM_PM_SEG_CNT_MIN_CNT_WIDTH         12
#define MKS_PM_PM_SEG_CNT_MIN_CNT_MASK          (0xFFFU << MKS_PM_PM_SEG_CNT_MIN_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Packet Segment Counts */
typedef union {
  struct {
    uint32_t cur_cnt : 12;      /* Number of availble segments left in Packet Memmory. */
    uint32_t : 4;
    uint32_t min_cnt : 12;      /* Minimum value of segment count seen since last read to this register. */
    uint32_t : 4;
  } field;
  uint32_t val;
} MKS_PM_PM_SEG_CNT_t;
#endif /* GENHDR_STRUCT */

/* Ingress drop threshold */
#define MKS_PM_PM_DROP_THRESHOLD                        0x00000008U
/* Segment occupancy threshold used to trigger drops to parser. */
#define MKS_PM_PM_DROP_THRESHOLD_DRP_THR_SHIFT          0
#define MKS_PM_PM_DROP_THRESHOLD_DRP_THR_WIDTH          13
#define MKS_PM_PM_DROP_THRESHOLD_DRP_THR_MASK           (0x1FFFU << MKS_PM_PM_DROP_THRESHOLD_DRP_THR_SHIFT)

#ifdef GENHDR_STRUCT
/* Ingress drop threshold */
typedef union {
  struct {
    uint32_t drp_thr : 13;      /* Segment occupancy threshold used to trigger drops to parser. */
    uint32_t : 19;
  } field;
  uint32_t val;
} MKS_PM_PM_DROP_THRESHOLD_t;
#endif /* GENHDR_STRUCT */

/* Number of segments used from P1 and P0 */
#define MKS_PM_PM_SEG_USED_P1_P0                        0x0000000CU
/* Number of segments used in P0. */
#define MKS_PM_PM_SEG_USED_P1_P0_SEG_USED_P0_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_PM_PM_SEG_USED_P1_P0_SEG_USED_P0_SHIFT      0
#define MKS_PM_PM_SEG_USED_P1_P0_SEG_USED_P0_WIDTH      12
#define MKS_PM_PM_SEG_USED_P1_P0_SEG_USED_P0_MASK       (0xFFFU << MKS_PM_PM_SEG_USED_P1_P0_SEG_USED_P0_SHIFT)

/* Number of segments used in P1. */
#define MKS_PM_PM_SEG_USED_P1_P0_SEG_USED_P1_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_PM_PM_SEG_USED_P1_P0_SEG_USED_P1_SHIFT      16
#define MKS_PM_PM_SEG_USED_P1_P0_SEG_USED_P1_WIDTH      12
#define MKS_PM_PM_SEG_USED_P1_P0_SEG_USED_P1_MASK       (0xFFFU << MKS_PM_PM_SEG_USED_P1_P0_SEG_USED_P1_SHIFT)

#ifdef GENHDR_STRUCT
/* Number of segments used from P1 and P0 */
typedef union {
  struct {
    uint32_t seg_used_p0 : 12;  /* Number of segments used in P0. */
    uint32_t : 4;
    uint32_t seg_used_p1 : 12;  /* Number of segments used in P1. */
    uint32_t : 4;
  } field;
  uint32_t val;
} MKS_PM_PM_SEG_USED_P1_P0_t;
#endif /* GENHDR_STRUCT */

/* Number of segments used from P3 and P2 */
#define MKS_PM_PM_SEG_USED_P3_P2                        0x00000010U
/* Number of segments used in P2. */
#define MKS_PM_PM_SEG_USED_P3_P2_SEG_USED_P2_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_PM_PM_SEG_USED_P3_P2_SEG_USED_P2_SHIFT      0
#define MKS_PM_PM_SEG_USED_P3_P2_SEG_USED_P2_WIDTH      12
#define MKS_PM_PM_SEG_USED_P3_P2_SEG_USED_P2_MASK       (0xFFFU << MKS_PM_PM_SEG_USED_P3_P2_SEG_USED_P2_SHIFT)

/* Number of segments used in P3. */
#define MKS_PM_PM_SEG_USED_P3_P2_SEG_USED_P3_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_PM_PM_SEG_USED_P3_P2_SEG_USED_P3_SHIFT      16
#define MKS_PM_PM_SEG_USED_P3_P2_SEG_USED_P3_WIDTH      12
#define MKS_PM_PM_SEG_USED_P3_P2_SEG_USED_P3_MASK       (0xFFFU << MKS_PM_PM_SEG_USED_P3_P2_SEG_USED_P3_SHIFT)

#ifdef GENHDR_STRUCT
/* Number of segments used from P3 and P2 */
typedef union {
  struct {
    uint32_t seg_used_p2 : 12;  /* Number of segments used in P2. */
    uint32_t : 4;
    uint32_t seg_used_p3 : 12;  /* Number of segments used in P3. */
    uint32_t : 4;
  } field;
  uint32_t val;
} MKS_PM_PM_SEG_USED_P3_P2_t;
#endif /* GENHDR_STRUCT */

/* Maximum number of segments used from P1 and P0 */
#define MKS_PM_PM_MAXSEG_USED_P1_P0                             0x00000014U
/* Max number of segments used in P0. */
#define MKS_PM_PM_MAXSEG_USED_P1_P0_MAX_SEG_USED_P0_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define MKS_PM_PM_MAXSEG_USED_P1_P0_MAX_SEG_USED_P0_SHIFT       0
#define MKS_PM_PM_MAXSEG_USED_P1_P0_MAX_SEG_USED_P0_WIDTH       12
#define MKS_PM_PM_MAXSEG_USED_P1_P0_MAX_SEG_USED_P0_MASK        (0xFFFU << MKS_PM_PM_MAXSEG_USED_P1_P0_MAX_SEG_USED_P0_SHIFT)

/* Max number of segments used in P1. */
#define MKS_PM_PM_MAXSEG_USED_P1_P0_MAX_SEG_USED_P1_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define MKS_PM_PM_MAXSEG_USED_P1_P0_MAX_SEG_USED_P1_SHIFT       16
#define MKS_PM_PM_MAXSEG_USED_P1_P0_MAX_SEG_USED_P1_WIDTH       12
#define MKS_PM_PM_MAXSEG_USED_P1_P0_MAX_SEG_USED_P1_MASK        (0xFFFU << MKS_PM_PM_MAXSEG_USED_P1_P0_MAX_SEG_USED_P1_SHIFT)

#ifdef GENHDR_STRUCT
/* Maximum number of segments used from P1 and P0 */
typedef union {
  struct {
    uint32_t max_seg_used_p0 : 12;      /* Max number of segments used in P0. */
    uint32_t : 4;
    uint32_t max_seg_used_p1 : 12;      /* Max number of segments used in P1. */
    uint32_t : 4;
  } field;
  uint32_t val;
} MKS_PM_PM_MAXSEG_USED_P1_P0_t;
#endif /* GENHDR_STRUCT */

/* Maximum number of segments used from P3 and P2 */
#define MKS_PM_PM_MAXSEG_USED_P3_P2                             0x00000018U
/* Max number of segments used in P2. */
#define MKS_PM_PM_MAXSEG_USED_P3_P2_MAX_SEG_USED_P2_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define MKS_PM_PM_MAXSEG_USED_P3_P2_MAX_SEG_USED_P2_SHIFT       0
#define MKS_PM_PM_MAXSEG_USED_P3_P2_MAX_SEG_USED_P2_WIDTH       13
#define MKS_PM_PM_MAXSEG_USED_P3_P2_MAX_SEG_USED_P2_MASK        (0x1FFFU << MKS_PM_PM_MAXSEG_USED_P3_P2_MAX_SEG_USED_P2_SHIFT)

/* Max number of segments used in P3. */
#define MKS_PM_PM_MAXSEG_USED_P3_P2_MAX_SEG_USED_P3_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define MKS_PM_PM_MAXSEG_USED_P3_P2_MAX_SEG_USED_P3_SHIFT       16
#define MKS_PM_PM_MAXSEG_USED_P3_P2_MAX_SEG_USED_P3_WIDTH       13
#define MKS_PM_PM_MAXSEG_USED_P3_P2_MAX_SEG_USED_P3_MASK        (0x1FFFU << MKS_PM_PM_MAXSEG_USED_P3_P2_MAX_SEG_USED_P3_SHIFT)

#ifdef GENHDR_STRUCT
/* Maximum number of segments used from P3 and P2 */
typedef union {
  struct {
    uint32_t max_seg_used_p2 : 13;      /* Max number of segments used in P2. */
    uint32_t : 3;
    uint32_t max_seg_used_p3 : 13;      /* Max number of segments used in P3. */
    uint32_t : 3;
  } field;
  uint32_t val;
} MKS_PM_PM_MAXSEG_USED_P3_P2_t;
#endif /* GENHDR_STRUCT */

/*
 * This is a RW1C register used for logging MBE/SBE errors. Write 1 to clear the
 * corresponding error bits
 */
#define MKS_PM_PM_ECC_ERR                       0x0000001CU
/*
 * SBE errors. 
 *  pmSbe[0] = 1 indicates SBE error detected in bits [63:0].
 *  pmSbe[1] = 1 indicates SBE error detected in bits[127:64].
 *  pmSbe[2] = 1 indicates SBE error detected in bits[191:128].
 *  pmSbe[3] = 1 indicates SBE error detected in bits[255:256].
 *  pmSbe[4] = 1 indicates SBE error detected in bits[319:256].
 *  pmSbe[5] = 1 indicates SBE error detected in bits[383:320].
 *  pmSbe[6] = 1 indicates SBE error detected in bits[447:384].
 *  pmSbe[7] = 1 indicates SBE error detected in bits[511:448].
 */
#define MKS_PM_PM_ECC_ERR_PM_SBE_ACCESSTYPE     GENHDR_ACCESSTYPE_W1C
#define MKS_PM_PM_ECC_ERR_PM_SBE_SHIFT          0
#define MKS_PM_PM_ECC_ERR_PM_SBE_WIDTH          8
#define MKS_PM_PM_ECC_ERR_PM_SBE_MASK           (0xFFU << MKS_PM_PM_ECC_ERR_PM_SBE_SHIFT)

/*
 * MBE errors.
 *  pmMbe[8] = 1 indicates MBE error detected in bits [63:0].
 *  pmMbe[9] = 1 indicates MBE error detected in bits[127:64].
 *  pmMbe[10] = 1 indicates MBE error detected in bits[191:128].
 *  pmMbe[11] = 1 indicates MBE error detected in bits[255:256].
 *  pmMbe[12] = 1 indicates MBE error detected in bits[319:256].
 *  pmMbe[13] = 1 indicates MBE error detected in bits[383:320].
 *  pmMbe[14] = 1 indicates MBE error detected in bits[447:384].
 *  pmMbe[15] = 1 indicates MBE error detected in bits[511:448].
 */
#define MKS_PM_PM_ECC_ERR_PM_MBE_ACCESSTYPE     GENHDR_ACCESSTYPE_W1C
#define MKS_PM_PM_ECC_ERR_PM_MBE_SHIFT          8
#define MKS_PM_PM_ECC_ERR_PM_MBE_WIDTH          8
#define MKS_PM_PM_ECC_ERR_PM_MBE_MASK           (0xFFU << MKS_PM_PM_ECC_ERR_PM_MBE_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * This is a RW1C register used for logging MBE/SBE errors. Write 1 to clear the
 * corresponding error bits
 */
typedef union {
  struct {
    uint32_t pm_sbe : 8;        /* SBE errors.
                                   pmSbe[0] = 1 indicates SBE error detected in bits [63:0].
                                   pmSbe[1] = 1 indicates SBE error detected in bits[127:64].
                                   pmSbe[2] = 1 indicates SBE error detected in bits[191:128].
                                   pmSbe[3] = 1 indicates SBE error detected in bits[255:256].
                                   pmSbe[4] = 1 indicates SBE error detected in bits[319:256].
                                   pmSbe[5] = 1 indicates SBE error detected in bits[383:320].
                                   pmSbe[6] = 1 indicates SBE error detected in bits[447:384].
                                   pmSbe[7] = 1 indicates SBE error detected in bits[511:448]. */
    uint32_t pm_mbe : 8;        /* MBE errors.
                                   pmMbe[8] = 1 indicates MBE error detected in bits [63:0].
                                   pmMbe[9] = 1 indicates MBE error detected in bits[127:64].
                                   pmMbe[10] = 1 indicates MBE error detected in bits[191:128].
                                   pmMbe[11] = 1 indicates MBE error detected in bits[255:256].
                                   pmMbe[12] = 1 indicates MBE error detected in bits[319:256].
                                   pmMbe[13] = 1 indicates MBE error detected in bits[383:320].
                                   pmMbe[14] = 1 indicates MBE error detected in bits[447:384].
                                   pmMbe[15] = 1 indicates MBE error detected in bits[511:448]. */
    uint32_t : 16;
  } field;
  uint32_t val;
} MKS_PM_PM_ECC_ERR_t;
#endif /* GENHDR_STRUCT */

#endif /* _MKS_PM_REGISTERS_H_ */

