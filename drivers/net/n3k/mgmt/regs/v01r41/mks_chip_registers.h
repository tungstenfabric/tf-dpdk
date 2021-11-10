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
 * Date: 2020-12-02 10:40
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

#ifndef _MKS_CHIP_REGISTERS_H_
#define _MKS_CHIP_REGISTERS_H_

/* MKS Chip CSR Address Map */

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

#define MKS_CHIP_ACCESSTYPE             GENHDR_ACCESSTYPE_RW /*  Default access type. Access types defined above. */
#define MKS_CHIP_REGWIDTH               32 /* Default width of register in bits */
#define MKS_CHIP_ACCESSWIDTH            32 /* Default width of access word in bit */


#define MKS_CHIP_MKS_MGMT_BASE                  0x00080000U
#define MKS_CHIP_MKS_MGMT_SIZE                  0x20080U
#define MKS_CHIP_MKS_FLOW_TRACKER_BASE          0x000C0000U
#define MKS_CHIP_MKS_FLOW_TRACKER_SIZE          0x440U
#define MKS_CHIP_MKS_METERS_BASE                0x000D0000U
#define MKS_CHIP_MKS_METERS_SIZE                0x10CU
#define MKS_CHIP_EM_LKUP_BASE                   0x000E0000U
#define MKS_CHIP_EM_LKUP_SIZE                   0x20U
#define MKS_CHIP_LKUP_BASE                      0x000F0000U
#define MKS_CHIP_LKUP_SIZE                      0x205CU
#define MKS_CHIP_MKS_CHIP_TOP_CPU_REGS_BASE     0x00140000U
#define MKS_CHIP_MKS_CHIP_TOP_CPU_REGS_SIZE     0x100U
#define MKS_CHIP_MKS_LKUP_BASE                  0x001CC000U
#define MKS_CHIP_MKS_LKUP_SIZE                  0x12CU
#define MKS_CHIP_MKS_PM_BASE                    0x001D0000U
#define MKS_CHIP_MKS_PM_SIZE                    0x20U
#define MKS_CHIP_MKS_TXQ_BASE                   0x001D4000U
#define MKS_CHIP_MKS_TXQ_SIZE                   0x84U
#define MKS_CHIP_MKS_TXSCH_BASE                 0x001D8000U
#define MKS_CHIP_MKS_TXSCH_SIZE                 0x1CU
#define MKS_CHIP_MKS_PARS_BASE                  0x001DC000U
#define MKS_CHIP_MKS_PARS_SIZE                  0x80U
#define MKS_CHIP_MKS_MOD_BASE                   0x001E0000U
#define MKS_CHIP_MKS_MOD_SIZE                   0x5CU
#define MKS_CHIP_MKS_VIOSHM_BASE                0x001EC000U
#define MKS_CHIP_MKS_VIOSHM_SIZE                0x28U
#define MKS_CHIP_MKS_FT_FIFO_BASE               0x001F8000U
#define MKS_CHIP_MKS_FT_FIFO_SIZE               0xB8U
#define MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE 0x001FC000U
#define MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_SIZE 0x120U
#define MKS_CHIP_VIRTIO_DMA_CSR_BASE            0x00100000U
#define MKS_CHIP_VIRTIO_DMA_CSR_SIZE            0x3E040U
#define MKS_CHIP_VIRTIO_LIVE_MIGRATION_BASE(ix) (0x00180000U + (ix) * 0x1000U)
#define MKS_CHIP_VIRTIO_LIVE_MIGRATION_SIZE     0x1000U
#define MKS_CHIP_VIRTIO_LIVE_MIGRATION_MAX_INDEX 0x3FU
#endif /* _MKS_CHIP_REGISTERS_H_ */

