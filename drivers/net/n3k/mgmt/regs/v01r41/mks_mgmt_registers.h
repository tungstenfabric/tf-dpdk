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

#ifndef _MKS_MGMT_REGISTERS_H_
#define _MKS_MGMT_REGISTERS_H_

/* Management Block */

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

#define MKS_MGMT_ACCESSTYPE             GENHDR_ACCESSTYPE_RW /*  Default access type. Access types defined above. */
#define MKS_MGMT_REGWIDTH               32 /* Default width of register in bits */
#define MKS_MGMT_ACCESSWIDTH            32 /* Default width of access word in bit */


/* Version Number of This Module */
#define MKS_MGMT_MGMT_VERSION                   0x00000000U
/* Version format: yyyy_mm_dd. */
#define MKS_MGMT_MGMT_VERSION_VERSION_ACCESSTYPE GENHDR_ACCESSTYPE_RO
#define MKS_MGMT_MGMT_VERSION_VERSION_SHIFT     0
#define MKS_MGMT_MGMT_VERSION_VERSION_WIDTH     32
#define MKS_MGMT_MGMT_VERSION_VERSION_MASK      (0xFFFFFFFFU << MKS_MGMT_MGMT_VERSION_VERSION_SHIFT)

#ifdef GENHDR_STRUCT
/* Version Number of This Module */
typedef union {
  struct {
    uint32_t version : 32;      /* Version format: yyyy_mm_dd. */
  } field;
  uint32_t val;
} MKS_MGMT_MGMT_VERSION_t;
#endif /* GENHDR_STRUCT */

/* Reset Control for Arm */
#define MKS_MGMT_MGMT_CONTROL                           0x00000004U
/*
 * Debug reset request from FPGA fabric to SoC.
 *  Reset to 0. 
 */
#define MKS_MGMT_MGMT_CONTROL_DEBUG_RESET_SHIFT         0
#define MKS_MGMT_MGMT_CONTROL_DEBUG_RESET_WIDTH         1
#define MKS_MGMT_MGMT_CONTROL_DEBUG_RESET_MASK          (0x1U << MKS_MGMT_MGMT_CONTROL_DEBUG_RESET_SHIFT)

/*
 * Warm reset request from FPGA fabric to SoC.
 *  Reset to 0. 
 */
#define MKS_MGMT_MGMT_CONTROL_WARM_RESET_SHIFT          1
#define MKS_MGMT_MGMT_CONTROL_WARM_RESET_WIDTH          1
#define MKS_MGMT_MGMT_CONTROL_WARM_RESET_MASK           (0x1U << MKS_MGMT_MGMT_CONTROL_WARM_RESET_SHIFT)

/*
 * Cold reset request from FPGA fabric to SoC.
 *  Reset to 0. 
 */
#define MKS_MGMT_MGMT_CONTROL_COLD_RESET_SHIFT          2
#define MKS_MGMT_MGMT_CONTROL_COLD_RESET_WIDTH          1
#define MKS_MGMT_MGMT_CONTROL_COLD_RESET_MASK           (0x1U << MKS_MGMT_MGMT_CONTROL_COLD_RESET_SHIFT)

/* Reserved. */
#define MKS_MGMT_MGMT_CONTROL_RESERVED_SHIFT            3
#define MKS_MGMT_MGMT_CONTROL_RESERVED_WIDTH            29
#define MKS_MGMT_MGMT_CONTROL_RESERVED_MASK             (0x1FFFFFFFU << MKS_MGMT_MGMT_CONTROL_RESERVED_SHIFT)

#ifdef GENHDR_STRUCT
/* Reset Control for Arm */
typedef union {
  struct {
    uint32_t debug_reset : 1;   /* Debug reset request from FPGA fabric to SoC.
                                   Reset to 0. */
    uint32_t warm_reset : 1;    /* Warm reset request from FPGA fabric to SoC.
                                   Reset to 0. */
    uint32_t cold_reset : 1;    /* Cold reset request from FPGA fabric to SoC.
                                   Reset to 0. */
    uint32_t reserved : 29;     /* Reserved. */
  } field;
  uint32_t val;
} MKS_MGMT_MGMT_CONTROL_t;
#endif /* GENHDR_STRUCT */

/*
 * Provides visibility of the page for host accesses to HPS. Previously this was
 * a configurable value but is now fixed at FPGA image compile time
 */
#define MKS_MGMT_HPS_PAGE                       0x00000008U
/* The address presented to the ARM interface will be prepended by this value. */
#define MKS_MGMT_HPS_PAGE_HPS_PAGE_ACCESSTYPE   GENHDR_ACCESSTYPE_RO
#define MKS_MGMT_HPS_PAGE_HPS_PAGE_SHIFT        21
#define MKS_MGMT_HPS_PAGE_HPS_PAGE_WIDTH        11
#define MKS_MGMT_HPS_PAGE_HPS_PAGE_MASK         (0x7FFU << MKS_MGMT_HPS_PAGE_HPS_PAGE_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Provides visibility of the page for host accesses to HPS. Previously this was
 * a configurable value but is now fixed at FPGA image compile time
 */
typedef union {
  struct {
    uint32_t : 21;
    uint32_t hps_page : 11;     /* The address presented to the ARM interface will be prepended by this value. */
  } field;
  uint32_t val;
} MKS_MGMT_HPS_PAGE_t;
#endif /* GENHDR_STRUCT */

/*
 * Allow the firmware to interrupt the host. The host can enable this interrupt
 * using the Host Interrupt Enable Register
 */
#define MKS_MGMT_HOST_INT_REQ                           0x0000000CU
/*
 * Host Interrupt.
 *  Writing to this register will generate an interrupt to the host when the
 * host has enabled it. The interrupt request signal will be asserted for a
 * single cycle, and the interrupt vector will contain the value written to this
 * register.
 *  Note: the width of the interrupt vector is specified by a parameter
 * (F2PCIE_INT_VEC_NUM_BITS, which is currently 5 bits). For example: writing a
 * value of 0 to the HOST_INT_REQ register will send an interrupt vector of 0 to
 * the host if bit 0 of host_int_en register is already set.
 *  This bit is cleared on PCIe reset.
 *  Reset to 0. 
 */
#define MKS_MGMT_HOST_INT_REQ_HOST_INT_SHIFT            0
#define MKS_MGMT_HOST_INT_REQ_HOST_INT_WIDTH            5
#define MKS_MGMT_HOST_INT_REQ_HOST_INT_MASK             (0x1FU << MKS_MGMT_HOST_INT_REQ_HOST_INT_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Allow the firmware to interrupt the host. The host can enable this interrupt
 * using the Host Interrupt Enable Register
 */
typedef union {
  struct {
    uint32_t host_int : 5;      /* Host Interrupt.
                                   Writing to this register will generate an interrupt to the host when the
                                   host has enabled it. The interrupt request signal will be asserted for a
                                   single cycle, and the interrupt vector will contain the value written to this
                                   register.
                                   Note: the width of the interrupt vector is specified by a parameter
                                   (F2PCIE_INT_VEC_NUM_BITS, which is currently 5 bits). For example: writing a
                                   value of 0 to the HOST_INT_REQ register will send an interrupt vector of 0 to
                                   the host if bit 0 of host_int_en register is already set.
                                   This bit is cleared on PCIe reset.
                                   Reset to 0. */
    uint32_t : 27;
  } field;
  uint32_t val;
} MKS_MGMT_HOST_INT_REQ_t;
#endif /* GENHDR_STRUCT */

/* Allow the host to enable interrupts to the host */
#define MKS_MGMT_HOST_INT_EN                            0x00000010U
/*
 * Host Interrupt Enable
 *  Setting a bit in this register to 1 will enable an interrupt to the host
 * when the corresponding vector in the Host Interrupt Register is set.
 *  For example: setting bit 0 in HOST_INT_EN register enables interrupt vector
 * 0 to be sent when host_int_req is written with a value of 0.
 *  This bit is cleared on PCIe reset.
 *  Reset to 0. 
 */
#define MKS_MGMT_HOST_INT_EN_HOST_INT_EN_SHIFT          0
#define MKS_MGMT_HOST_INT_EN_HOST_INT_EN_WIDTH          32
#define MKS_MGMT_HOST_INT_EN_HOST_INT_EN_MASK           (0xFFFFFFFFU << MKS_MGMT_HOST_INT_EN_HOST_INT_EN_SHIFT)

#ifdef GENHDR_STRUCT
/* Allow the host to enable interrupts to the host */
typedef union {
  struct {
    uint32_t host_int_en : 32;  /* Host Interrupt Enable
                                   Setting a bit in this register to 1 will enable an interrupt to the host
                                   when the corresponding vector in the Host Interrupt Register is set.
                                   For example: setting bit 0 in HOST_INT_EN register enables interrupt vector
                                   0 to be sent when host_int_req is written with a value of 0.
                                   This bit is cleared on PCIe reset.
                                   Reset to 0. */
  } field;
  uint32_t val;
} MKS_MGMT_HOST_INT_EN_t;
#endif /* GENHDR_STRUCT */

/* Allow firmware interrupts to be masked */
#define MKS_MGMT_FW_INT_MASK                    0x00000014U
/*
 * Firmware Interrupt Mask.
 *  Setting a bit to 1 will mask the corresponding interrupt to the firmware.
 *  This bit is cleared on PCIe reset.
 *  Reset to 0. 
 */
#define MKS_MGMT_FW_INT_MASK_INT_MASK_SHIFT     0
#define MKS_MGMT_FW_INT_MASK_INT_MASK_WIDTH     32
#define MKS_MGMT_FW_INT_MASK_INT_MASK_MASK      (0xFFFFFFFFU << MKS_MGMT_FW_INT_MASK_INT_MASK_SHIFT)

#ifdef GENHDR_STRUCT
/* Allow firmware interrupts to be masked */
typedef union {
  struct {
    uint32_t int_mask : 32;     /* Firmware Interrupt Mask.
                                   Setting a bit to 1 will mask the corresponding interrupt to the firmware.
                                   This bit is cleared on PCIe reset.
                                   Reset to 0. */
  } field;
  uint32_t val;
} MKS_MGMT_FW_INT_MASK_t;
#endif /* GENHDR_STRUCT */

/*
 * Allow the host to interrupt the firmware. Firmware must be able to mask this
 * interrupt
 */
#define MKS_MGMT_FW_INT_SET                     0x00000018U
/*
 * Firmware Interrupt Request.
 *  Setting a bit to 1 will generate the corresponding interrupt to the firmware
 * unless firmware has masked it (eg by setting the corresponding bit in the
 * FW_INT_MASK register). Firmware will clear the interrupt after receiving it
 * and before processing it by setting the corresponding bit in the FW_INT_CLEAR
 * register.
 *  This bit is cleared on PCIe reset.
 *  Reset to 0. 
 */
#define MKS_MGMT_FW_INT_SET_INT_SET_SHIFT       0
#define MKS_MGMT_FW_INT_SET_INT_SET_WIDTH       32
#define MKS_MGMT_FW_INT_SET_INT_SET_MASK        (0xFFFFFFFFU << MKS_MGMT_FW_INT_SET_INT_SET_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Allow the host to interrupt the firmware. Firmware must be able to mask this
 * interrupt
 */
typedef union {
  struct {
    uint32_t int_set : 32;      /* Firmware Interrupt Request.
                                   Setting a bit to 1 will generate the corresponding interrupt to the firmware
                                   unless firmware has masked it (eg by setting the corresponding bit in the
                                   FW_INT_MASK register). Firmware will clear the interrupt after receiving it
                                   and before processing it by setting the corresponding bit in the FW_INT_CLEAR
                                   register.
                                   This bit is cleared on PCIe reset.
                                   Reset to 0. */
  } field;
  uint32_t val;
} MKS_MGMT_FW_INT_SET_t;
#endif /* GENHDR_STRUCT */

/* Allow the firmware to clear the firmware interrupt */
#define MKS_MGMT_FW_INT_CLEAR                   0x0000001CU
/*
 * Firmware Interrupt Clear.
 *  Setting a bit to 1 will clear the corresponding interrupt to the firmware.
 * Firmware will clear the interrupt after receiving it and before processing it
 * by setting the corresponding bit in the FW_INT_CLEAR register.
 *  This bit is cleared on PCIe reset.
 *  Reset to 0. 
 */
#define MKS_MGMT_FW_INT_CLEAR_INT_CLR_SHIFT     0
#define MKS_MGMT_FW_INT_CLEAR_INT_CLR_WIDTH     32
#define MKS_MGMT_FW_INT_CLEAR_INT_CLR_MASK      (0xFFFFFFFFU << MKS_MGMT_FW_INT_CLEAR_INT_CLR_SHIFT)

#ifdef GENHDR_STRUCT
/* Allow the firmware to clear the firmware interrupt */
typedef union {
  struct {
    uint32_t int_clr : 32;      /* Firmware Interrupt Clear.
                                   Setting a bit to 1 will clear the corresponding interrupt to the firmware.
                                   Firmware will clear the interrupt after receiving it and before processing it
                                   by setting the corresponding bit in the FW_INT_CLEAR register.
                                   This bit is cleared on PCIe reset.
                                   Reset to 0. */
  } field;
  uint32_t val;
} MKS_MGMT_FW_INT_CLEAR_t;
#endif /* GENHDR_STRUCT */

/* Control access to the firmware interface */
#define MKS_MGMT_SMPHORE                        0x00000020U
/*
 * Register Semaphore.
 *  This register can be written to only if the current value is 0x0.
 *  This register can be cleared by writing 0x0.
 *  Software should write an ID to this register (eg partly by using a value
 * read from LOCK_ID register). Software should then read this register. If it
 * then reads back the same ID value it wrote, then the lock operation was
 * successful - otherwise it is currently locked by another process. Software
 * should write 0x0 to this register when it has finished, to clear the lock for
 * another process to use.
 *  Reset to 0. 
 */
#define MKS_MGMT_SMPHORE_REG_SMP_SHIFT          0
#define MKS_MGMT_SMPHORE_REG_SMP_WIDTH          32
#define MKS_MGMT_SMPHORE_REG_SMP_MASK           (0xFFFFFFFFU << MKS_MGMT_SMPHORE_REG_SMP_SHIFT)

#ifdef GENHDR_STRUCT
/* Control access to the firmware interface */
typedef union {
  struct {
    uint32_t reg_smp : 32;      /* Register Semaphore.
                                   This register can be written to only if the current value is 0x0.
                                   This register can be cleared by writing 0x0.
                                   Software should write an ID to this register (eg partly by using a value
                                   read from LOCK_ID register). Software should then read this register. If it
                                   then reads back the same ID value it wrote, then the lock operation was
                                   successful - otherwise it is currently locked by another process. Software
                                   should write 0x0 to this register when it has finished, to clear the lock for
                                   another process to use.
                                   Reset to 0. */
  } field;
  uint32_t val;
} MKS_MGMT_SMPHORE_t;
#endif /* GENHDR_STRUCT */

/* Lock Id for the firmware interface */
#define MKS_MGMT_LOCK_ID                                0x00000024U
/*
 * Lock Id. Lock ID is incremented when this register is read AND the enable bit
 * is set.
 *  This register field is read only, and is never 0x0.
 *  When software wants to read a new LockId, it should first write a '1' to the
 * Enable bit, and then read this register.
 *  A new LockId value read from this register should only be used if the
 * LockIdEnable field is '1' for the same read operation (otherwise it is
 * possible than another process has cleared the enable bit, and so this Lock ID
 * may not be a unique value).
 *  Once software has read the new LockId value, it should write a '0' to the
 * Enable bit (to prevent any subsequent spurious reads from updating the LockId
 * value).
 *  Reset / wrap value of this field is 0x1. 
 */
#define MKS_MGMT_LOCK_ID_REG_LOCK_ID_ACCESSTYPE         GENHDR_ACCESSTYPE_RO
#define MKS_MGMT_LOCK_ID_REG_LOCK_ID_SHIFT              0
#define MKS_MGMT_LOCK_ID_REG_LOCK_ID_WIDTH              16
#define MKS_MGMT_LOCK_ID_REG_LOCK_ID_MASK               (0xFFFFU << MKS_MGMT_LOCK_ID_REG_LOCK_ID_SHIFT)

/* Reserved. */
#define MKS_MGMT_LOCK_ID_RESERVED_SHIFT                 16
#define MKS_MGMT_LOCK_ID_RESERVED_WIDTH                 15
#define MKS_MGMT_LOCK_ID_RESERVED_MASK                  (0x7FFFU << MKS_MGMT_LOCK_ID_RESERVED_SHIFT)

/*
 * Lock Id Enable. Lock ID is incremented when this register is read AND this
 * enable bit is set.
 *  This register field can be cleared by writing 0x0.
 *  When software wants to read a new LockId, it should first write a '1' to
 * this Enable bit, and then read this register.
 *  Once software has read the new LockId value, it should write a '0' to this
 * Enable bit (to prevent any subsequent spurious reads from updating the LockId
 * value).
 *  Reset to 0. 
 */
#define MKS_MGMT_LOCK_ID_REG_LOCK_ID_ENABLE_SHIFT       31
#define MKS_MGMT_LOCK_ID_REG_LOCK_ID_ENABLE_WIDTH       1
#define MKS_MGMT_LOCK_ID_REG_LOCK_ID_ENABLE_MASK        (0x1U << MKS_MGMT_LOCK_ID_REG_LOCK_ID_ENABLE_SHIFT)

#ifdef GENHDR_STRUCT
/* Lock Id for the firmware interface */
typedef union {
  struct {
    uint32_t reg_lock_id : 16;          /* Lock Id. Lock ID is incremented when this register is read AND the enable bit
                                           is set.
                                           This register field is read only, and is never 0x0.
                                           When software wants to read a new LockId, it should first write a '1' to the
                                           Enable bit, and then read this register.
                                           A new LockId value read from this register should only be used if the
                                           LockIdEnable field is '1' for the same read operation (otherwise it is
                                           possible than another process has cleared the enable bit, and so this Lock ID
                                           may not be a unique value).
                                           Once software has read the new LockId value, it should write a '0' to the
                                           Enable bit (to prevent any subsequent spurious reads from updating the LockId
                                           value).
                                           Reset / wrap value of this field is 0x1. */
    uint32_t reserved : 15;             /* Reserved. */
    uint32_t reg_lock_id_enable : 1;    /* Lock Id Enable. Lock ID is incremented when this register is read AND this
                                           enable bit is set.
                                           This register field can be cleared by writing 0x0.
                                           When software wants to read a new LockId, it should first write a '1' to
                                           this Enable bit, and then read this register.
                                           Once software has read the new LockId value, it should write a '0' to this
                                           Enable bit (to prevent any subsequent spurious reads from updating the LockId
                                           value).
                                           Reset to 0. */
  } field;
  uint32_t val;
} MKS_MGMT_LOCK_ID_t;
#endif /* GENHDR_STRUCT */

/*
 * Redundant register that previously controlled host access to PCIe BAR
 * interfaces. Now replaced with BAR_DECODE_ROM. This register has no impact on
 * the device and should not be used
 */
#define MKS_MGMT_PCIE_BAR_DISABLE                                       0x00000028U
/*
 * PF0 BAR 0 disable bit (disabled when '1').
 *  Reset to 0. 
 */
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_BAR_0_DIS_SHIFT              0
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_BAR_0_DIS_WIDTH              1
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_BAR_0_DIS_MASK               (0x1U << MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_BAR_0_DIS_SHIFT)

/*
 * PF0 BAR 2 disable bit (disabled when '1').
 *  Reset to 0. 
 */
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_BAR_2_DIS_SHIFT              1
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_BAR_2_DIS_WIDTH              1
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_BAR_2_DIS_MASK               (0x1U << MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_BAR_2_DIS_SHIFT)

/*
 * PF0 BAR 4 disable bit (disabled when '1').
 *  Reset to 0. 
 */
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_BAR_4_DIS_SHIFT              2
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_BAR_4_DIS_WIDTH              1
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_BAR_4_DIS_MASK               (0x1U << MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_BAR_4_DIS_SHIFT)

/* Reserved. */
#define MKS_MGMT_PCIE_BAR_DISABLE_RESERVED_0_SHIFT                      3
#define MKS_MGMT_PCIE_BAR_DISABLE_RESERVED_0_WIDTH                      1
#define MKS_MGMT_PCIE_BAR_DISABLE_RESERVED_0_MASK                       (0x1U << MKS_MGMT_PCIE_BAR_DISABLE_RESERVED_0_SHIFT)

/*
 * PF0 VF BAR 0 disable bit (disabled when '1').
 *  Reset to 0. 
 */
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_VF_BAR_0_DIS_SHIFT           4
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_VF_BAR_0_DIS_WIDTH           1
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_VF_BAR_0_DIS_MASK            (0x1U << MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_VF_BAR_0_DIS_SHIFT)

/*
 * PF0 VF BAR 2 disable bit (disabled when '1').
 *  Reset to 0. 
 */
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_VF_BAR_2_DIS_SHIFT           5
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_VF_BAR_2_DIS_WIDTH           1
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_VF_BAR_2_DIS_MASK            (0x1U << MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_VF_BAR_2_DIS_SHIFT)

/*
 * PF0 VF BAR 4 disable bit (disabled when '1').
 *  Reset to 0. 
 */
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_VF_BAR_4_DIS_SHIFT           6
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_VF_BAR_4_DIS_WIDTH           1
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_VF_BAR_4_DIS_MASK            (0x1U << MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_0_VF_BAR_4_DIS_SHIFT)

/* Reserved. */
#define MKS_MGMT_PCIE_BAR_DISABLE_RESERVED_1_SHIFT                      7
#define MKS_MGMT_PCIE_BAR_DISABLE_RESERVED_1_WIDTH                      1
#define MKS_MGMT_PCIE_BAR_DISABLE_RESERVED_1_MASK                       (0x1U << MKS_MGMT_PCIE_BAR_DISABLE_RESERVED_1_SHIFT)

/*
 * PF1 BAR 0 disable bit (disabled when '1').
 *  Reset to 0. 
 */
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_BAR_0_DIS_SHIFT              8
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_BAR_0_DIS_WIDTH              1
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_BAR_0_DIS_MASK               (0x1U << MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_BAR_0_DIS_SHIFT)

/*
 * PF1 BAR 2 disable bit (disabled when '1').
 *  Reset to 0. 
 */
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_BAR_2_DIS_SHIFT              9
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_BAR_2_DIS_WIDTH              1
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_BAR_2_DIS_MASK               (0x1U << MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_BAR_2_DIS_SHIFT)

/*
 * PF1 BAR 4 disable bit (disabled when '1').
 *  Reset to 0. 
 */
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_BAR_4_DIS_SHIFT              10
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_BAR_4_DIS_WIDTH              1
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_BAR_4_DIS_MASK               (0x1U << MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_BAR_4_DIS_SHIFT)

/* Reserved. */
#define MKS_MGMT_PCIE_BAR_DISABLE_RESERVED_2_SHIFT                      11
#define MKS_MGMT_PCIE_BAR_DISABLE_RESERVED_2_WIDTH                      1
#define MKS_MGMT_PCIE_BAR_DISABLE_RESERVED_2_MASK                       (0x1U << MKS_MGMT_PCIE_BAR_DISABLE_RESERVED_2_SHIFT)

/*
 * PF1 VF BAR 0 disable bit (disabled when '1').
 *  Reset to 0. 
 */
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_VF_BAR_0_DIS_SHIFT           12
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_VF_BAR_0_DIS_WIDTH           1
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_VF_BAR_0_DIS_MASK            (0x1U << MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_VF_BAR_0_DIS_SHIFT)

/*
 * PF1 VF BAR 2 disable bit (disabled when '1').
 *  Reset to 0. 
 */
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_VF_BAR_2_DIS_SHIFT           13
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_VF_BAR_2_DIS_WIDTH           1
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_VF_BAR_2_DIS_MASK            (0x1U << MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_VF_BAR_2_DIS_SHIFT)

/*
 * PF1 VF BAR 4 disable bit (disabled when '1').
 *  Reset to 0. 
 */
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_VF_BAR_4_DIS_SHIFT           14
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_VF_BAR_4_DIS_WIDTH           1
#define MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_VF_BAR_4_DIS_MASK            (0x1U << MKS_MGMT_PCIE_BAR_DISABLE_REG_PF_1_VF_BAR_4_DIS_SHIFT)

/* Reserved. */
#define MKS_MGMT_PCIE_BAR_DISABLE_RESERVED_3_SHIFT                      15
#define MKS_MGMT_PCIE_BAR_DISABLE_RESERVED_3_WIDTH                      17
#define MKS_MGMT_PCIE_BAR_DISABLE_RESERVED_3_MASK                       (0x1FFFFU << MKS_MGMT_PCIE_BAR_DISABLE_RESERVED_3_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Redundant register that previously controlled host access to PCIe BAR
 * interfaces. Now replaced with BAR_DECODE_ROM. This register has no impact on
 * the device and should not be used
 */
typedef union {
  struct {
    uint32_t reg_pf_0_bar_0_dis : 1;            /* PF0 BAR 0 disable bit (disabled when '1').
                                                   Reset to 0. */
    uint32_t reg_pf_0_bar_2_dis : 1;            /* PF0 BAR 2 disable bit (disabled when '1').
                                                   Reset to 0. */
    uint32_t reg_pf_0_bar_4_dis : 1;            /* PF0 BAR 4 disable bit (disabled when '1').
                                                   Reset to 0. */
    uint32_t reserved_0 : 1;                    /* Reserved. */
    uint32_t reg_pf_0_vf_bar_0_dis : 1;         /* PF0 VF BAR 0 disable bit (disabled when '1').
                                                   Reset to 0. */
    uint32_t reg_pf_0_vf_bar_2_dis : 1;         /* PF0 VF BAR 2 disable bit (disabled when '1').
                                                   Reset to 0. */
    uint32_t reg_pf_0_vf_bar_4_dis : 1;         /* PF0 VF BAR 4 disable bit (disabled when '1').
                                                   Reset to 0. */
    uint32_t reserved_1 : 1;                    /* Reserved. */
    uint32_t reg_pf_1_bar_0_dis : 1;            /* PF1 BAR 0 disable bit (disabled when '1').
                                                   Reset to 0. */
    uint32_t reg_pf_1_bar_2_dis : 1;            /* PF1 BAR 2 disable bit (disabled when '1').
                                                   Reset to 0. */
    uint32_t reg_pf_1_bar_4_dis : 1;            /* PF1 BAR 4 disable bit (disabled when '1').
                                                   Reset to 0. */
    uint32_t reserved_2 : 1;                    /* Reserved. */
    uint32_t reg_pf_1_vf_bar_0_dis : 1;         /* PF1 VF BAR 0 disable bit (disabled when '1').
                                                   Reset to 0. */
    uint32_t reg_pf_1_vf_bar_2_dis : 1;         /* PF1 VF BAR 2 disable bit (disabled when '1').
                                                   Reset to 0. */
    uint32_t reg_pf_1_vf_bar_4_dis : 1;         /* PF1 VF BAR 4 disable bit (disabled when '1').
                                                   Reset to 0. */
    uint32_t reserved_3 : 17;                   /* Reserved. */
  } field;
  uint32_t val;
} MKS_MGMT_PCIE_BAR_DISABLE_t;
#endif /* GENHDR_STRUCT */

/* Specifies the page used for accesses to PCIE Local Management Interface */
#define MKS_MGMT_LMI_PAGE                       0x0000002CU
/*
 * Specifies the page used for accesses to PCIE Local Management Interface.
 * Resets to 0.
 */
#define MKS_MGMT_LMI_PAGE_LMI_PAGE_SHIFT        0
#define MKS_MGMT_LMI_PAGE_LMI_PAGE_WIDTH        16
#define MKS_MGMT_LMI_PAGE_LMI_PAGE_MASK         (0xFFFFU << MKS_MGMT_LMI_PAGE_LMI_PAGE_SHIFT)

#ifdef GENHDR_STRUCT
/* Specifies the page used for accesses to PCIE Local Management Interface */
typedef union {
  struct {
    uint32_t lmi_page : 16;     /* Specifies the page used for accesses to PCIE Local Management Interface.
                                   Resets to 0. */
    uint32_t : 16;
  } field;
  uint32_t val;
} MKS_MGMT_LMI_PAGE_t;
#endif /* GENHDR_STRUCT */

/* An optional 2KB onchip shared buffer for use by the host and ARM */
#define MKS_MGMT_ONCHIP_SHARED_BUFFER(onchip_shared_buffer_ix) (0x00011E00U + ((onchip_shared_buffer_ix) * 4))
#define MKS_MGMT_ONCHIP_SHARED_BUFFER_MAX_INDEX         0x1FFU
/*
 * Optional 2KB on-chip shared buffer for use by the host and ARM. This buffer
 * is present only if parameter MGMT_ONCHIP_SHARED_BUFFER=1 in RTL.
 *  Note: this on-chip buffer is provided as an alternative to the DDR4 shared
 * buffer. Once the DDR4 shared buffer is working as expected, this on-chip
 * buffer will be removed.
 */
#define MKS_MGMT_ONCHIP_SHARED_BUFFER_ENTRY_SHIFT       0
#define MKS_MGMT_ONCHIP_SHARED_BUFFER_ENTRY_WIDTH       32
#define MKS_MGMT_ONCHIP_SHARED_BUFFER_ENTRY_MASK        (0xFFFFFFFFU << MKS_MGMT_ONCHIP_SHARED_BUFFER_ENTRY_SHIFT)

#ifdef GENHDR_STRUCT
/* An optional 2KB onchip shared buffer for use by the host and ARM */
typedef union {
  struct {
    uint32_t entry : 32;        /* Optional 2KB on-chip shared buffer for use by the host and ARM. This buffer
                                   is present only if parameter MGMT_ONCHIP_SHARED_BUFFER=1 in RTL.
                                   Note: this on-chip buffer is provided as an alternative to the DDR4 shared
                                   buffer. Once the DDR4 shared buffer is working as expected, this on-chip
                                   buffer will be removed. */
  } field;
  uint32_t val;
} MKS_MGMT_ONCHIP_SHARED_BUFFER_t;
#endif /* GENHDR_STRUCT */

/*
 * Each bar decode entry determines whether the host PFs or VFs can access the
 * device PCIe bars. The index value is {PF[3:0],is_vf,Bar[3:0]} where is_vf
 * determines whether it is PF access (0) or VFs (1) associated with the PF
 * access to each bar
 */
#define MKS_MGMT_BAR_DECODE_ROM(bar_decode_rom_ix)      (0x00020000U + ((bar_decode_rom_ix) * 4))
#define MKS_MGMT_BAR_DECODE_ROM_MAX_INDEX               0x1FU
/*
 * Destination. TBC
 *  dest[2:0] = x00, bar0, virtio, - pf0_vf_bar0, pf1_pf_bar0
 *  dest[2:0] = x01, bar2, irq, - pf0_pf_bar2, pf0_vf_bar2, pf1_pf_bar2
 *  dest[2:0] = x10, pf_bar4, management, - pf0_pf_bar4, pf0_pf_bar6
 *  dest[2:0] = x11, vf_bar4, live_migration, - pf0_vf_bar4.
 */
#define MKS_MGMT_BAR_DECODE_ROM_DEST_ACCESSTYPE         GENHDR_ACCESSTYPE_WO
#define MKS_MGMT_BAR_DECODE_ROM_DEST_SHIFT              0
#define MKS_MGMT_BAR_DECODE_ROM_DEST_WIDTH              3
#define MKS_MGMT_BAR_DECODE_ROM_DEST_MASK               (0x7U << MKS_MGMT_BAR_DECODE_ROM_DEST_SHIFT)

/*
 * When set the PF or VF has access to the destination bar, ie Safe Mode is off.
 *  When cleared access is disabled, ie Safe Mode is on.
 */
#define MKS_MGMT_BAR_DECODE_ROM_BAR_ENABLE_ACCESSTYPE   GENHDR_ACCESSTYPE_WO
#define MKS_MGMT_BAR_DECODE_ROM_BAR_ENABLE_SHIFT        3
#define MKS_MGMT_BAR_DECODE_ROM_BAR_ENABLE_WIDTH        1
#define MKS_MGMT_BAR_DECODE_ROM_BAR_ENABLE_MASK         (0x1U << MKS_MGMT_BAR_DECODE_ROM_BAR_ENABLE_SHIFT)

#ifdef GENHDR_STRUCT
/*
 * Each bar decode entry determines whether the host PFs or VFs can access the
 * device PCIe bars. The index value is {PF[3:0],is_vf,Bar[3:0]} where is_vf
 * determines whether it is PF access (0) or VFs (1) associated with the PF
 * access to each bar
 */
typedef union {
  struct {
    uint32_t dest : 3;          /* Destination. TBC
                                   dest[2:0] = x00, bar0, virtio, - pf0_vf_bar0, pf1_pf_bar0
                                   dest[2:0] = x01, bar2, irq, - pf0_pf_bar2, pf0_vf_bar2, pf1_pf_bar2
                                   dest[2:0] = x10, pf_bar4, management, - pf0_pf_bar4, pf0_pf_bar6
                                   dest[2:0] = x11, vf_bar4, live_migration, - pf0_vf_bar4. */
    uint32_t bar_enable : 1;    /* When set the PF or VF has access to the destination bar, ie Safe Mode is off.
                                   When cleared access is disabled, ie Safe Mode is on. */
    uint32_t : 28;
  } field;
  uint32_t val;
} MKS_MGMT_BAR_DECODE_ROM_t;
#endif /* GENHDR_STRUCT */

#endif /* _MKS_MGMT_REGISTERS_H_ */

