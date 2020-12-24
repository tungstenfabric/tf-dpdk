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

#ifndef _MKS_CHIP_TOP_CPU_REGS_REGISTERS_H_
#define _MKS_CHIP_TOP_CPU_REGS_REGISTERS_H_

/* Chip & PLL information and control registers */

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

#define MKS_CHIP_TOP_CPU_REGS_ACCESSTYPE GENHDR_ACCESSTYPE_RW /*  Default access type. Access types defined above. */
#define MKS_CHIP_TOP_CPU_REGS_REGWIDTH  32 /* Default width of register in bits */
#define MKS_CHIP_TOP_CPU_REGS_ACCESSWIDTH 32 /* Default width of access word in bit */


/* Chip version */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER                          0x00000000U
/* Build version ID. */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_BUILD_ID_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_BUILD_ID_SHIFT           0
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_BUILD_ID_WIDTH           8
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_BUILD_ID_MASK            (0xFFU << MKS_CHIP_TOP_CPU_REGS_CHIP_VER_BUILD_ID_SHIFT)

/* Patch version ID. */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_PATCH_ID_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_PATCH_ID_SHIFT           8
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_PATCH_ID_WIDTH           8
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_PATCH_ID_MASK            (0xFFU << MKS_CHIP_TOP_CPU_REGS_CHIP_VER_PATCH_ID_SHIFT)

/* Minor version ID. */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_MINOR_ID_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_MINOR_ID_SHIFT           16
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_MINOR_ID_WIDTH           8
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_MINOR_ID_MASK            (0xFFU << MKS_CHIP_TOP_CPU_REGS_CHIP_VER_MINOR_ID_SHIFT)

/* Major version ID. */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_MAJOR_ID_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_MAJOR_ID_SHIFT           24
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_MAJOR_ID_WIDTH           4
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_MAJOR_ID_MASK            (0xFU << MKS_CHIP_TOP_CPU_REGS_CHIP_VER_MAJOR_ID_SHIFT)

/* Board version ID. */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_BOARD_ID_ACCESSTYPE      GENHDR_ACCESSTYPE_RO
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_BOARD_ID_SHIFT           28
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_BOARD_ID_WIDTH           4
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_BOARD_ID_MASK            (0xFU << MKS_CHIP_TOP_CPU_REGS_CHIP_VER_BOARD_ID_SHIFT)

#ifdef GENHDR_STRUCT
/* Chip version */
typedef union {
  struct {
    uint32_t build_id : 8;      /* Build version ID. */
    uint32_t patch_id : 8;      /* Patch version ID. */
    uint32_t minor_id : 8;      /* Minor version ID. */
    uint32_t major_id : 4;      /* Major version ID. */
    uint32_t board_id : 4;      /* Board version ID. */
  } field;
  uint32_t val;
} MKS_CHIP_TOP_CPU_REGS_CHIP_VER_t;
#endif /* GENHDR_STRUCT */

#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_BOARD_ID_N_385ASOC       0x0U /* Nallatech 385A-SOC board. */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_BOARD_ID_NPO_001         0x1U /* NPO v.001 board. */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_BOARD_ID_NPO_001V_256    0x2U /* NPO v.001 board - 256MB DDR4. */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_VER_BOARD_ID_NPO_002         0x3U /* NPO v.002 board. */

/* Chip status */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_STAT                         0x00000004U
/*
 * Current junction temperature. The value returned is the temperature in
 * Celsius, expressed as a 2's complement number. Examples: 0x64 corresponds to
 * 100degC, 0xF0 corresponds to -16degC.
 */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_STAT_TEMP_ACCESSTYPE         GENHDR_ACCESSTYPE_RO
#define MKS_CHIP_TOP_CPU_REGS_CHIP_STAT_TEMP_SHIFT              0
#define MKS_CHIP_TOP_CPU_REGS_CHIP_STAT_TEMP_WIDTH              8
#define MKS_CHIP_TOP_CPU_REGS_CHIP_STAT_TEMP_MASK               (0xFFU << MKS_CHIP_TOP_CPU_REGS_CHIP_STAT_TEMP_SHIFT)

/*
 * Set if junction temperature (as read in .temp) reaches 100degC. The bit can
 * only be cleared by performing a hardware reset of the device.
 */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_STAT_OVER_TEMP_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define MKS_CHIP_TOP_CPU_REGS_CHIP_STAT_OVER_TEMP_SHIFT         8
#define MKS_CHIP_TOP_CPU_REGS_CHIP_STAT_OVER_TEMP_WIDTH         1
#define MKS_CHIP_TOP_CPU_REGS_CHIP_STAT_OVER_TEMP_MASK          (0x1U << MKS_CHIP_TOP_CPU_REGS_CHIP_STAT_OVER_TEMP_SHIFT)

#ifdef GENHDR_STRUCT
/* Chip status */
typedef union {
  struct {
    uint32_t temp : 8;          /* Current junction temperature. The value returned is the temperature in
                                   Celsius, expressed as a 2's complement number. Examples: 0x64 corresponds to
                                   100degC, 0xF0 corresponds to -16degC. */
    uint32_t over_temp : 1;     /* Set if junction temperature (as read in .temp) reaches 100degC. The bit can
                                   only be cleared by performing a hardware reset of the device. */
    uint32_t : 23;
  } field;
  uint32_t val;
} MKS_CHIP_TOP_CPU_REGS_CHIP_STAT_t;
#endif /* GENHDR_STRUCT */

/* Chip configuration */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG                          0x00000008U
/* Set to 0 to release reset for the attached Unionvale devices. */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_UVL_RESET_SHIFT          0
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_UVL_RESET_WIDTH          1
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_UVL_RESET_MASK           (0x1U << MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_UVL_RESET_SHIFT)

/*
 * If set a pulse on nPERST will cause the FPGA to be reconfigured (NPOv002
 * only). Register will power-up with value 0 and not be changed by a G_RESET_N.
 */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_PERST_RECONF_SHIFT       1
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_PERST_RECONF_WIDTH       1
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_PERST_RECONF_MASK        (0x1U << MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_PERST_RECONF_SHIFT)

/* SFP#0 TX Disable. Set to 1 to disable SFP cage. */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_SFP_0_TX_DIS_SHIFT       2
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_SFP_0_TX_DIS_WIDTH       1
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_SFP_0_TX_DIS_MASK        (0x1U << MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_SFP_0_TX_DIS_SHIFT)

/* SFP#1 TX Disable. Set to 1 to disable SFP cage. */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_SFP_1_TX_DIS_SHIFT       3
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_SFP_1_TX_DIS_WIDTH       1
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_SFP_1_TX_DIS_MASK        (0x1U << MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_SFP_1_TX_DIS_SHIFT)

/* SFP#0 RS01. RS01 to SFP cage. */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_SFP_0_RS01_SHIFT         4
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_SFP_0_RS01_WIDTH         1
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_SFP_0_RS01_MASK          (0x1U << MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_SFP_0_RS01_SHIFT)

/* SFP#1 RS01. RS01 to SFP cage. */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_SFP_1_RS01_SHIFT         5
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_SFP_1_RS01_WIDTH         1
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_SFP_1_RS01_MASK          (0x1U << MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_SFP_1_RS01_SHIFT)

/* If set to 0 the FPGA can reconfgure itself (on PCB v003 only). */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_SELF_RECONF_N_SHIFT      6
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_SELF_RECONF_N_WIDTH      1
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_SELF_RECONF_N_MASK       (0x1U << MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_SELF_RECONF_N_SHIFT)

/*
 * If set to 0 the nPERST is delayed until the FPGA is configured (on PCB v003
 * only).
 */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_PERST_DELAY_N_SHIFT      7
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_PERST_DELAY_N_WIDTH      1
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_PERST_DELAY_N_MASK       (0x1U << MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_PERST_DELAY_N_SHIFT)

/* LED#0. Set to 1 to turn on LED. */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_LED_0_SHIFT              8
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_LED_0_WIDTH              4
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_LED_0_MASK               (0xFU << MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_LED_0_SHIFT)

/* LED#1. Set to 1 to turn on LED. */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_LED_1_SHIFT              12
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_LED_1_WIDTH              4
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_LED_1_MASK               (0xFU << MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_LED_1_SHIFT)

/* Set to 1 to soft reset the FPGA core. */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_FPGA_SOFT_RST_SHIFT      31
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_FPGA_SOFT_RST_WIDTH      1
#define MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_FPGA_SOFT_RST_MASK       (0x1U << MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_FPGA_SOFT_RST_SHIFT)

#ifdef GENHDR_STRUCT
/* Chip configuration */
typedef union {
  struct {
    uint32_t uvl_reset : 1;             /* Set to 0 to release reset for the attached Unionvale devices. */
    uint32_t perst_reconf : 1;          /* If set a pulse on nPERST will cause the FPGA to be reconfigured (NPOv002
                                           only). Register will power-up with value 0 and not be changed by a G_RESET_N. */
    uint32_t sfp_0_tx_dis : 1;          /* SFP#0 TX Disable. Set to 1 to disable SFP cage. */
    uint32_t sfp_1_tx_dis : 1;          /* SFP#1 TX Disable. Set to 1 to disable SFP cage. */
    uint32_t sfp_0_rs01 : 1;            /* SFP#0 RS01. RS01 to SFP cage. */
    uint32_t sfp_1_rs01 : 1;            /* SFP#1 RS01. RS01 to SFP cage. */
    uint32_t self_reconf_n : 1;         /* If set to 0 the FPGA can reconfgure itself (on PCB v003 only). */
    uint32_t perst_delay_n : 1;         /* If set to 0 the nPERST is delayed until the FPGA is configured (on PCB v003
                                           only). */
    uint32_t led_0 : 4;                 /* LED#0. Set to 1 to turn on LED. */
    uint32_t led_1 : 4;                 /* LED#1. Set to 1 to turn on LED. */
    uint32_t : 15;
    uint32_t fpga_soft_rst : 1;         /* Set to 1 to soft reset the FPGA core. */
  } field;
  uint32_t val;
} MKS_CHIP_TOP_CPU_REGS_CHIP_CFG_t;
#endif /* GENHDR_STRUCT */

/* Chip information MSB */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_INFO_MSB                             0x0000000CU
/* N/A. */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_INFO_MSB_CHIPINFO_MSB_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_CHIP_TOP_CPU_REGS_CHIP_INFO_MSB_CHIPINFO_MSB_SHIFT          0
#define MKS_CHIP_TOP_CPU_REGS_CHIP_INFO_MSB_CHIPINFO_MSB_WIDTH          32
#define MKS_CHIP_TOP_CPU_REGS_CHIP_INFO_MSB_CHIPINFO_MSB_MASK           (0xFFFFFFFFU << MKS_CHIP_TOP_CPU_REGS_CHIP_INFO_MSB_CHIPINFO_MSB_SHIFT)

#ifdef GENHDR_STRUCT
/* Chip information MSB */
typedef union {
  struct {
    uint32_t chipinfo_msb : 32; /* N/A. */
  } field;
  uint32_t val;
} MKS_CHIP_TOP_CPU_REGS_CHIP_INFO_MSB_t;
#endif /* GENHDR_STRUCT */

/* Chip information LSB */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_INFO_LSB                             0x00000010U
/* N/A. */
#define MKS_CHIP_TOP_CPU_REGS_CHIP_INFO_LSB_CHIPINFO_LSB_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_CHIP_TOP_CPU_REGS_CHIP_INFO_LSB_CHIPINFO_LSB_SHIFT          0
#define MKS_CHIP_TOP_CPU_REGS_CHIP_INFO_LSB_CHIPINFO_LSB_WIDTH          32
#define MKS_CHIP_TOP_CPU_REGS_CHIP_INFO_LSB_CHIPINFO_LSB_MASK           (0xFFFFFFFFU << MKS_CHIP_TOP_CPU_REGS_CHIP_INFO_LSB_CHIPINFO_LSB_SHIFT)

#ifdef GENHDR_STRUCT
/* Chip information LSB */
typedef union {
  struct {
    uint32_t chipinfo_lsb : 32; /* N/A. */
  } field;
  uint32_t val;
} MKS_CHIP_TOP_CPU_REGS_CHIP_INFO_LSB_t;
#endif /* GENHDR_STRUCT */

/* PLL out-of-lock status hold */
#define MKS_CHIP_TOP_CPU_REGS_PLL_OOL_HOLD                      0x00000014U
/* Core PLL (G_CLK). */
#define MKS_CHIP_TOP_CPU_REGS_PLL_OOL_HOLD_CORE_ACCESSTYPE      GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_PLL_OOL_HOLD_CORE_SHIFT           0
#define MKS_CHIP_TOP_CPU_REGS_PLL_OOL_HOLD_CORE_WIDTH           1
#define MKS_CHIP_TOP_CPU_REGS_PLL_OOL_HOLD_CORE_MASK            (0x1U << MKS_CHIP_TOP_CPU_REGS_PLL_OOL_HOLD_CORE_SHIFT)

#ifdef GENHDR_STRUCT
/* PLL out-of-lock status hold */
typedef union {
  struct {
    uint32_t core : 1;  /* Core PLL (G_CLK). */
    uint32_t : 31;
  } field;
  uint32_t val;
} MKS_CHIP_TOP_CPU_REGS_PLL_OOL_HOLD_t;
#endif /* GENHDR_STRUCT */

/* PLL reset control */
#define MKS_CHIP_TOP_CPU_REGS_PLL_RST                   0x00000018U
/* Core PLL (G_CLK). */
#define MKS_CHIP_TOP_CPU_REGS_PLL_RST_CORE_SHIFT        0
#define MKS_CHIP_TOP_CPU_REGS_PLL_RST_CORE_WIDTH        1
#define MKS_CHIP_TOP_CPU_REGS_PLL_RST_CORE_MASK         (0x1U << MKS_CHIP_TOP_CPU_REGS_PLL_RST_CORE_SHIFT)

#ifdef GENHDR_STRUCT
/* PLL reset control */
typedef union {
  struct {
    uint32_t core : 1;  /* Core PLL (G_CLK). */
    uint32_t : 31;
  } field;
  uint32_t val;
} MKS_CHIP_TOP_CPU_REGS_PLL_RST_t;
#endif /* GENHDR_STRUCT */

/* FPGA image build date */
#define MKS_CHIP_TOP_CPU_REGS_BUILD_DATE                        0x0000001CU
/* Debug use. Will be 0 in production images. */
#define MKS_CHIP_TOP_CPU_REGS_BUILD_DATE_BUILD_DATE_ACCESSTYPE  GENHDR_ACCESSTYPE_RO
#define MKS_CHIP_TOP_CPU_REGS_BUILD_DATE_BUILD_DATE_SHIFT       0
#define MKS_CHIP_TOP_CPU_REGS_BUILD_DATE_BUILD_DATE_WIDTH       32
#define MKS_CHIP_TOP_CPU_REGS_BUILD_DATE_BUILD_DATE_MASK        (0xFFFFFFFFU << MKS_CHIP_TOP_CPU_REGS_BUILD_DATE_BUILD_DATE_SHIFT)

#ifdef GENHDR_STRUCT
/* FPGA image build date */
typedef union {
  struct {
    uint32_t build_date : 32;   /* Debug use. Will be 0 in production images. */
  } field;
  uint32_t val;
} MKS_CHIP_TOP_CPU_REGS_BUILD_DATE_t;
#endif /* GENHDR_STRUCT */

/* PR freeze control */
#define MKS_CHIP_TOP_CPU_REGS_PR_FREEZE                         0x00000020U
#define MKS_CHIP_TOP_CPU_REGS_PR_FREEZE_WIDTH                   64
/* Reserved. */
#define MKS_CHIP_TOP_CPU_REGS_PR_FREEZE_RESERVED_ACCESSTYPE     GENHDR_ACCESSTYPE_RO
#define MKS_CHIP_TOP_CPU_REGS_PR_FREEZE_RESERVED_SHIFT          0
#define MKS_CHIP_TOP_CPU_REGS_PR_FREEZE_RESERVED_WIDTH          1
#define MKS_CHIP_TOP_CPU_REGS_PR_FREEZE_RESERVED_MASK           (0x1U << MKS_CHIP_TOP_CPU_REGS_PR_FREEZE_RESERVED_SHIFT)

#ifdef GENHDR_STRUCT
/* PR freeze control */
typedef union {
  struct {
    uint32_t reserved : 1;      /* Reserved. */
    uint32_t : 31;
    uint32_t : 32;
  } field;
  uint32_t val[2];
} MKS_CHIP_TOP_CPU_REGS_PR_FREEZE_t;
#endif /* GENHDR_STRUCT */

/* 1 us timer */
#define MKS_CHIP_TOP_CPU_REGS_TIMERUS                   0x00000028U
/* This field is incremented each microsecond and wraps at 0xFFFFFFFF. */
#define MKS_CHIP_TOP_CPU_REGS_TIMERUS_CNT_ACCESSTYPE    GENHDR_ACCESSTYPE_RO
#define MKS_CHIP_TOP_CPU_REGS_TIMERUS_CNT_SHIFT         0
#define MKS_CHIP_TOP_CPU_REGS_TIMERUS_CNT_WIDTH         32
#define MKS_CHIP_TOP_CPU_REGS_TIMERUS_CNT_MASK          (0xFFFFFFFFU << MKS_CHIP_TOP_CPU_REGS_TIMERUS_CNT_SHIFT)

#ifdef GENHDR_STRUCT
/* 1 us timer */
typedef union {
  struct {
    uint32_t cnt : 32;  /* This field is incremented each microsecond and wraps at 0xFFFFFFFF. */
  } field;
  uint32_t val;
} MKS_CHIP_TOP_CPU_REGS_TIMERUS_t;
#endif /* GENHDR_STRUCT */

/* Misc. hold bits */
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD                                 0x0000002CU
/* SFP#0 Rx LOS. */
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_0_RX_LOS_ACCESSTYPE         GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_0_RX_LOS_SHIFT              0
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_0_RX_LOS_WIDTH              1
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_0_RX_LOS_MASK               (0x1U << MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_0_RX_LOS_SHIFT)

/* SFP#0 Module absent. */
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_0_MOD_ABS_ACCESSTYPE        GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_0_MOD_ABS_SHIFT             1
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_0_MOD_ABS_WIDTH             1
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_0_MOD_ABS_MASK              (0x1U << MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_0_MOD_ABS_SHIFT)

/* SFP#0 Tx Fault. */
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_0_TX_FAULT_ACCESSTYPE       GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_0_TX_FAULT_SHIFT            2
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_0_TX_FAULT_WIDTH            1
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_0_TX_FAULT_MASK             (0x1U << MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_0_TX_FAULT_SHIFT)

/* SFP#1 Rx LOS. */
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_1_RX_LOS_ACCESSTYPE         GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_1_RX_LOS_SHIFT              8
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_1_RX_LOS_WIDTH              1
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_1_RX_LOS_MASK               (0x1U << MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_1_RX_LOS_SHIFT)

/* SFP#1 Module absent. */
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_1_MOD_ABS_ACCESSTYPE        GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_1_MOD_ABS_SHIFT             9
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_1_MOD_ABS_WIDTH             1
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_1_MOD_ABS_MASK              (0x1U << MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_1_MOD_ABS_SHIFT)

/* SFP#1 Tx Fault. */
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_1_TX_FAULT_ACCESSTYPE       GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_1_TX_FAULT_SHIFT            10
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_1_TX_FAULT_WIDTH            1
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_1_TX_FAULT_MASK             (0x1U << MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_SFP_1_TX_FAULT_SHIFT)

/* DDR4#0 calibration success. */
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_0_CAL_SUCC_ACCESSTYPE       GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_0_CAL_SUCC_SHIFT            16
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_0_CAL_SUCC_WIDTH            1
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_0_CAL_SUCC_MASK             (0x1U << MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_0_CAL_SUCC_SHIFT)

/* DDR4#0 calibration failure. */
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_0_CAL_FAIL_ACCESSTYPE       GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_0_CAL_FAIL_SHIFT            17
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_0_CAL_FAIL_WIDTH            1
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_0_CAL_FAIL_MASK             (0x1U << MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_0_CAL_FAIL_SHIFT)

/* DDR4#1 calibration success (NPOv002 only). */
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_1_CAL_SUCC_ACCESSTYPE       GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_1_CAL_SUCC_SHIFT            18
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_1_CAL_SUCC_WIDTH            1
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_1_CAL_SUCC_MASK             (0x1U << MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_1_CAL_SUCC_SHIFT)

/* DDR4#1 calibration failure (NPOv002 only). */
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_1_CAL_FAIL_ACCESSTYPE       GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_1_CAL_FAIL_SHIFT            19
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_1_CAL_FAIL_WIDTH            1
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_1_CAL_FAIL_MASK             (0x1U << MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_1_CAL_FAIL_SHIFT)

/* DDR4#2 calibration success. */
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_2_CAL_SUCC_ACCESSTYPE       GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_2_CAL_SUCC_SHIFT            20
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_2_CAL_SUCC_WIDTH            1
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_2_CAL_SUCC_MASK             (0x1U << MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_2_CAL_SUCC_SHIFT)

/* DDR4#2 calibration failure. */
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_2_CAL_FAIL_ACCESSTYPE       GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_2_CAL_FAIL_SHIFT            21
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_2_CAL_FAIL_WIDTH            1
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_2_CAL_FAIL_MASK             (0x1U << MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_2_CAL_FAIL_SHIFT)

/* DDR4#3 calibration success (NPOv002 only). */
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_3_CAL_SUCC_ACCESSTYPE       GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_3_CAL_SUCC_SHIFT            22
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_3_CAL_SUCC_WIDTH            1
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_3_CAL_SUCC_MASK             (0x1U << MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_3_CAL_SUCC_SHIFT)

/* DDR4#3 calibration failure (NPOv002 only). */
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_3_CAL_FAIL_ACCESSTYPE       GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_3_CAL_FAIL_SHIFT            23
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_3_CAL_FAIL_WIDTH            1
#define MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_3_CAL_FAIL_MASK             (0x1U << MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_DDR_3_CAL_FAIL_SHIFT)

#ifdef GENHDR_STRUCT
/* Misc. hold bits */
typedef union {
  struct {
    uint32_t sfp_0_rx_los : 1;          /* SFP#0 Rx LOS. */
    uint32_t sfp_0_mod_abs : 1;         /* SFP#0 Module absent. */
    uint32_t sfp_0_tx_fault : 1;        /* SFP#0 Tx Fault. */
    uint32_t : 5;
    uint32_t sfp_1_rx_los : 1;          /* SFP#1 Rx LOS. */
    uint32_t sfp_1_mod_abs : 1;         /* SFP#1 Module absent. */
    uint32_t sfp_1_tx_fault : 1;        /* SFP#1 Tx Fault. */
    uint32_t : 5;
    uint32_t ddr_0_cal_succ : 1;        /* DDR4#0 calibration success. */
    uint32_t ddr_0_cal_fail : 1;        /* DDR4#0 calibration failure. */
    uint32_t ddr_1_cal_succ : 1;        /* DDR4#1 calibration success (NPOv002 only). */
    uint32_t ddr_1_cal_fail : 1;        /* DDR4#1 calibration failure (NPOv002 only). */
    uint32_t ddr_2_cal_succ : 1;        /* DDR4#2 calibration success. */
    uint32_t ddr_2_cal_fail : 1;        /* DDR4#2 calibration failure. */
    uint32_t ddr_3_cal_succ : 1;        /* DDR4#3 calibration success (NPOv002 only). */
    uint32_t ddr_3_cal_fail : 1;        /* DDR4#3 calibration failure (NPOv002 only). */
    uint32_t : 8;
  } field;
  uint32_t val;
} MKS_CHIP_TOP_CPU_REGS_MISC_HOLD_t;
#endif /* GENHDR_STRUCT */

/* Misc. Eth hold bits */
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD(eth_hold_ix)                     (0x00000030U + ((eth_hold_ix) * 4))
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_MAX_INDEX                        0x1U
/* UVL backpressure detected. */
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_UVL_BP_ACCESSTYPE                GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_UVL_BP_SHIFT                     0
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_UVL_BP_WIDTH                     1
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_UVL_BP_MASK                      (0x1U << MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_UVL_BP_SHIFT)

/* ETH MAC Tx FIFO xclk underflow. */
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_TX_FUFLW_ACCESSTYPE          GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_TX_FUFLW_SHIFT               1
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_TX_FUFLW_WIDTH               1
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_TX_FUFLW_MASK                (0x1U << MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_TX_FUFLW_SHIFT)

/* ETH MAC Tx FIFO xclk overflow. */
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_TX_FOFLW_ACCESSTYPE          GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_TX_FOFLW_SHIFT               2
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_TX_FOFLW_WIDTH               1
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_TX_FOFLW_MASK                (0x1U << MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_TX_FOFLW_SHIFT)

/* ETH MAC Tx FIFO xclk protocol violation from CORE. */
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_TX_FPROT_VIOL_ACCESSTYPE     GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_TX_FPROT_VIOL_SHIFT          3
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_TX_FPROT_VIOL_WIDTH          1
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_TX_FPROT_VIOL_MASK           (0x1U << MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_TX_FPROT_VIOL_SHIFT)

/* ETH MAC Tx FIFO xclk highest fill level detected. */
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_TX_FHIGH_LVL_ACCESSTYPE      GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_TX_FHIGH_LVL_SHIFT           4
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_TX_FHIGH_LVL_WIDTH           9
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_TX_FHIGH_LVL_MASK            (0x1FFU << MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_TX_FHIGH_LVL_SHIFT)

/* ETH MAC Rx FIFO xclk overflow. */
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_RX_FOFLW_ACCESSTYPE          GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_RX_FOFLW_SHIFT               16
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_RX_FOFLW_WIDTH               1
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_RX_FOFLW_MASK                (0x1U << MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_RX_FOFLW_SHIFT)

/* ETH MAC Rx FIFO xclk half full reached. */
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_RX_FHALF_FULL_ACCESSTYPE     GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_RX_FHALF_FULL_SHIFT          17
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_RX_FHALF_FULL_WIDTH          1
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_RX_FHALF_FULL_MASK           (0x1U << MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_RX_FHALF_FULL_SHIFT)

/* ETH MAC Rx FIFO xclk protocol violation from MAC. */
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_RX_FPROT_VIOL_ACCESSTYPE     GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_RX_FPROT_VIOL_SHIFT          18
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_RX_FPROT_VIOL_WIDTH          1
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_RX_FPROT_VIOL_MASK           (0x1U << MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_RX_FPROT_VIOL_SHIFT)

/* ETH MAC Rx FIFO xclk highest fill level detected. */
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_RX_FHIGH_LVL_ACCESSTYPE      GENHDR_ACCESSTYPE_W1C
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_RX_FHIGH_LVL_SHIFT           20
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_RX_FHIGH_LVL_WIDTH           9
#define MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_RX_FHIGH_LVL_MASK            (0x1FFU << MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_ETH_RX_FHIGH_LVL_SHIFT)

#ifdef GENHDR_STRUCT
/* Misc. Eth hold bits */
typedef union {
  struct {
    uint32_t uvl_bp : 1;                /* UVL backpressure detected. */
    uint32_t eth_tx_fuflw : 1;          /* ETH MAC Tx FIFO xclk underflow. */
    uint32_t eth_tx_foflw : 1;          /* ETH MAC Tx FIFO xclk overflow. */
    uint32_t eth_tx_fprot_viol : 1;     /* ETH MAC Tx FIFO xclk protocol violation from CORE. */
    uint32_t eth_tx_fhigh_lvl : 9;      /* ETH MAC Tx FIFO xclk highest fill level detected. */
    uint32_t : 3;
    uint32_t eth_rx_foflw : 1;          /* ETH MAC Rx FIFO xclk overflow. */
    uint32_t eth_rx_fhalf_full : 1;     /* ETH MAC Rx FIFO xclk half full reached. */
    uint32_t eth_rx_fprot_viol : 1;     /* ETH MAC Rx FIFO xclk protocol violation from MAC. */
    uint32_t : 1;
    uint32_t eth_rx_fhigh_lvl : 9;      /* ETH MAC Rx FIFO xclk highest fill level detected. */
    uint32_t : 3;
  } field;
  uint32_t val;
} MKS_CHIP_TOP_CPU_REGS_ETH_HOLD_t;
#endif /* GENHDR_STRUCT */

#endif /* _MKS_CHIP_TOP_CPU_REGS_REGISTERS_H_ */

