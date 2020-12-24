/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _N3K_MGMT_INIT_TYPE_H_
#define _N3K_MGMT_INIT_TYPE_H_

#ifndef ETH_ALEN
#define ETH_ALEN	6
#endif /* ETH_ALEN */

#ifndef ETH_HLEN
#define ETH_HLEN	14
#endif /* ETH_HLEN */

#include "n3k_mgmt_init_osdep.h"
#include "n3k_mgmt_init_devids.h"

#ifndef BIT
#define BIT(a) (1UL << (a))
#endif /* BIT */
#ifndef BIT_ULL
#define BIT_ULL(a) (1ULL << (a))
#endif /* BIT_ULL */

/* Data type manipulation macros. */
#define N3K_MGMT_INIT_HI_WORD(x)		((u16)(((x) >> 16) & 0xFFFF))

/* debug masks - set these bits in hw->debug_mask to control output */
#define N3K_MGMT_INIT_DEBUG_INIT			BIT_ULL(1)
#define N3K_MGMT_INIT_DEBUG_RELEASE		BIT_ULL(2)
#define N3K_MGMT_INIT_DEBUG_ERR			BIT_ULL(3)

#define N3K_MGMT_INIT_DEBUG_ALL			0xFFFFFFFFFFFFFFFFULL

/* These are structs for managing the hardware information and the operations */
/* MAC types */
enum n3k_mgmt_init_mac_type {
	N3K_MGMT_INIT_MAC_UNKNOWN = 0,
	N3K_MGMT_INIT_MAC_GENERIC,
};

/* PCI bus types */
enum n3k_mgmt_init_bus_type {
	n3k_mgmt_init_bus_unknown = 0,
	n3k_mgmt_init_bus_pci_express,
	n3k_mgmt_init_bus_embedded, /* Is device Embedded versus card */
	n3k_mgmt_init_bus_reserved
};

/* PCI bus speeds */
enum n3k_mgmt_init_pcie_bus_speed {
	n3k_mgmt_init_pcie_speed_unknown	= 0xff,
	n3k_mgmt_init_pcie_speed_2_5GT	= 0x14,
	n3k_mgmt_init_pcie_speed_5_0GT	= 0x15,
	n3k_mgmt_init_pcie_speed_8_0GT	= 0x16,
};

/* PCI bus widths */
enum n3k_mgmt_init_pcie_link_width {
	n3k_mgmt_init_pcie_lnk_width_resrv	= 0x00,
	n3k_mgmt_init_pcie_lnk_x1			= 0x01,
	n3k_mgmt_init_pcie_lnk_x2			= 0x02,
	n3k_mgmt_init_pcie_lnk_x4			= 0x04,
	n3k_mgmt_init_pcie_lnk_x8			= 0x08,
	n3k_mgmt_init_pcie_lnk_x12		= 0x0C,
	n3k_mgmt_init_pcie_lnk_x16		= 0x10,
	n3k_mgmt_init_pcie_lnk_x32		= 0x20,
	n3k_mgmt_init_pcie_lnk_width_unknown	= 0xff,
};

/* Various RESET request, These are not tied with HW reset types */
enum n3k_mgmt_init_reset_request {
	N3K_MGMT_INIT_RESET_PFR	= 0,
	N3K_MGMT_INIT_RESET_CORER	= 1,
	N3K_MGMT_INIT_RESET_GLOBR	= 2,
};

#endif /* _N3K_MGMT_INIT_TYPE_H_ */
