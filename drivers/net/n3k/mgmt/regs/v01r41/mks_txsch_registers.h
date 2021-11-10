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

#ifndef _MKS_TXSCH_REGISTERS_H_
#define _MKS_TXSCH_REGISTERS_H_

/* OVS Transmit Scheduler */

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

#define MKS_TXSCH_ACCESSTYPE            GENHDR_ACCESSTYPE_RW /*  Default access type. Access types defined above. */
#define MKS_TXSCH_REGWIDTH              32 /* Default width of register in bits */
#define MKS_TXSCH_ACCESSWIDTH           32 /* Default width of access word in bit */


/* General purpose RW register intended for debug use */
#define MKS_TXSCH_TXSCH_SCRATCH                 0x00000000U
/* Scratch data. */
#define MKS_TXSCH_TXSCH_SCRATCH_DATA_SHIFT      0
#define MKS_TXSCH_TXSCH_SCRATCH_DATA_WIDTH      32
#define MKS_TXSCH_TXSCH_SCRATCH_DATA_MASK       (0xFFFFFFFFU << MKS_TXSCH_TXSCH_SCRATCH_DATA_SHIFT)

#ifdef GENHDR_STRUCT
/* General purpose RW register intended for debug use */
typedef union {
  struct {
    uint32_t data : 32; /* Scratch data. */
  } field;
  uint32_t val;
} MKS_TXSCH_TXSCH_SCRATCH_t;
#endif /* GENHDR_STRUCT */

/* Tx scheduler packet counter */
#define MKS_TXSCH_TXSCH_PKT_CNT                 0x00000004U
/* Tx scheduler packet counter. */
#define MKS_TXSCH_TXSCH_PKT_CNT_CNT_SHIFT       0
#define MKS_TXSCH_TXSCH_PKT_CNT_CNT_WIDTH       32
#define MKS_TXSCH_TXSCH_PKT_CNT_CNT_MASK        (0xFFFFFFFFU << MKS_TXSCH_TXSCH_PKT_CNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Tx scheduler packet counter */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Tx scheduler packet counter. */
  } field;
  uint32_t val;
} MKS_TXSCH_TXSCH_PKT_CNT_t;
#endif /* GENHDR_STRUCT */

/* Number of packet Txsch returns pointer to PM */
#define MKS_TXSCH_TXSCH_FREE_CNT                0x00000008U
/* Number of packet Txsch returns pointer to PM. */
#define MKS_TXSCH_TXSCH_FREE_CNT_CNT_SHIFT      0
#define MKS_TXSCH_TXSCH_FREE_CNT_CNT_WIDTH      32
#define MKS_TXSCH_TXSCH_FREE_CNT_CNT_MASK       (0xFFFFFFFFU << MKS_TXSCH_TXSCH_FREE_CNT_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Number of packet Txsch returns pointer to PM */
typedef union {
  struct {
    uint32_t cnt : 32;  /* Number of packet Txsch returns pointer to PM. */
  } field;
  uint32_t val;
} MKS_TXSCH_TXSCH_FREE_CNT_t;
#endif /* GENHDR_STRUCT */

/* Number of cycles Modifier stalls TxSch Port#0 */
#define MKS_TXSCH_TXSCH_NRDCYC_CNT_0                    0x0000000CU
/* Counts number of cycles Modifier stalls TxSch. */
#define MKS_TXSCH_TXSCH_NRDCYC_CNT_0_FREE_CNT_SHIFT     0
#define MKS_TXSCH_TXSCH_NRDCYC_CNT_0_FREE_CNT_WIDTH     32
#define MKS_TXSCH_TXSCH_NRDCYC_CNT_0_FREE_CNT_MASK      (0xFFFFFFFFU << MKS_TXSCH_TXSCH_NRDCYC_CNT_0_FREE_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Number of cycles Modifier stalls TxSch Port#0 */
typedef union {
  struct {
    uint32_t free_cnt : 32;     /* Counts number of cycles Modifier stalls TxSch. */
  } field;
  uint32_t val;
} MKS_TXSCH_TXSCH_NRDCYC_CNT_0_t;
#endif /* GENHDR_STRUCT */

/* Number of cycles Modifier stalls TxSch Port#0 */
#define MKS_TXSCH_TXSCH_NRDCYC_CNT_1                    0x00000010U
/* Counts number of cycles Modifier stalls TxSch. */
#define MKS_TXSCH_TXSCH_NRDCYC_CNT_1_FREE_CNT_SHIFT     0
#define MKS_TXSCH_TXSCH_NRDCYC_CNT_1_FREE_CNT_WIDTH     32
#define MKS_TXSCH_TXSCH_NRDCYC_CNT_1_FREE_CNT_MASK      (0xFFFFFFFFU << MKS_TXSCH_TXSCH_NRDCYC_CNT_1_FREE_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Number of cycles Modifier stalls TxSch Port#0 */
typedef union {
  struct {
    uint32_t free_cnt : 32;     /* Counts number of cycles Modifier stalls TxSch. */
  } field;
  uint32_t val;
} MKS_TXSCH_TXSCH_NRDCYC_CNT_1_t;
#endif /* GENHDR_STRUCT */

/* Number of cycles Modifier stalls TxSch Port#0 */
#define MKS_TXSCH_TXSCH_NRDCYC_CNT_2                    0x00000014U
/* Counts number of cycles Modifier stalls TxSch. */
#define MKS_TXSCH_TXSCH_NRDCYC_CNT_2_FREE_CNT_SHIFT     0
#define MKS_TXSCH_TXSCH_NRDCYC_CNT_2_FREE_CNT_WIDTH     32
#define MKS_TXSCH_TXSCH_NRDCYC_CNT_2_FREE_CNT_MASK      (0xFFFFFFFFU << MKS_TXSCH_TXSCH_NRDCYC_CNT_2_FREE_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Number of cycles Modifier stalls TxSch Port#0 */
typedef union {
  struct {
    uint32_t free_cnt : 32;     /* Counts number of cycles Modifier stalls TxSch. */
  } field;
  uint32_t val;
} MKS_TXSCH_TXSCH_NRDCYC_CNT_2_t;
#endif /* GENHDR_STRUCT */

/* Number of cycles Modifier stalls TxSch Port#0 */
#define MKS_TXSCH_TXSCH_NRDCYC_CNT_3                    0x00000018U
/* Counts number of cycles Modifier stalls TxSch. */
#define MKS_TXSCH_TXSCH_NRDCYC_CNT_3_FREE_CNT_SHIFT     0
#define MKS_TXSCH_TXSCH_NRDCYC_CNT_3_FREE_CNT_WIDTH     32
#define MKS_TXSCH_TXSCH_NRDCYC_CNT_3_FREE_CNT_MASK      (0xFFFFFFFFU << MKS_TXSCH_TXSCH_NRDCYC_CNT_3_FREE_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* Number of cycles Modifier stalls TxSch Port#0 */
typedef union {
  struct {
    uint32_t free_cnt : 32;     /* Counts number of cycles Modifier stalls TxSch. */
  } field;
  uint32_t val;
} MKS_TXSCH_TXSCH_NRDCYC_CNT_3_t;
#endif /* GENHDR_STRUCT */

#endif /* _MKS_TXSCH_REGISTERS_H_ */

