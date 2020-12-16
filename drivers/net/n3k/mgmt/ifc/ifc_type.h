/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (c) 2018, Intel Corporation. */

#ifndef _IFC_TYPE_H_
#define _IFC_TYPE_H_

#ifndef ETH_ALEN
#define ETH_ALEN	6
#endif /* ETH_ALEN */

#ifndef ETH_HLEN
#define ETH_HLEN	14
#endif /* ETH_HLEN */

#include "ifc_osdep.h"
#include "ifc_devids.h"

#ifndef BIT
#define BIT(a) (1UL << (a))
#endif /* BIT */
#ifndef BIT_ULL
#define BIT_ULL(a) (1ULL << (a))
#endif /* BIT_ULL */

/* Data type manipulation macros. */
#define IFC_HI_WORD(x)		((u16)(((x) >> 16) & 0xFFFF))

/* debug masks - set these bits in hw->debug_mask to control output */
#define IFC_DEBUG_INIT			BIT_ULL(1)
#define IFC_DEBUG_RELEASE		BIT_ULL(2)
#define IFC_DEBUG_ERR			BIT_ULL(3)

#define IFC_DEBUG_ALL			0xFFFFFFFFFFFFFFFFULL

/* These are structs for managing the hardware information and the operations */
/* MAC types */
enum ifc_mac_type {
	IFC_MAC_UNKNOWN = 0,
	IFC_MAC_GENERIC,
};

/* PCI bus types */
enum ifc_bus_type {
	ifc_bus_unknown = 0,
	ifc_bus_pci_express,
	ifc_bus_embedded, /* Is device Embedded versus card */
	ifc_bus_reserved
};

/* PCI bus speeds */
enum ifc_pcie_bus_speed {
	ifc_pcie_speed_unknown	= 0xff,
	ifc_pcie_speed_2_5GT	= 0x14,
	ifc_pcie_speed_5_0GT	= 0x15,
	ifc_pcie_speed_8_0GT	= 0x16,
};

/* PCI bus widths */
enum ifc_pcie_link_width {
	ifc_pcie_lnk_width_resrv	= 0x00,
	ifc_pcie_lnk_x1			= 0x01,
	ifc_pcie_lnk_x2			= 0x02,
	ifc_pcie_lnk_x4			= 0x04,
	ifc_pcie_lnk_x8			= 0x08,
	ifc_pcie_lnk_x12		= 0x0C,
	ifc_pcie_lnk_x16		= 0x10,
	ifc_pcie_lnk_x32		= 0x20,
	ifc_pcie_lnk_width_unknown	= 0xff,
};

/* Various RESET request, These are not tied with HW reset types */
enum ifc_reset_request {
	IFC_RESET_PFR	= 0,
	IFC_RESET_CORER	= 1,
	IFC_RESET_GLOBR	= 2,
};

#endif /* _IFC_TYPE_H_ */
