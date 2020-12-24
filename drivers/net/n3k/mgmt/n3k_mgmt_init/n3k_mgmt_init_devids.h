/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _N3K_MGMT_INIT_DEVIDS_H_
#define _N3K_MGMT_INIT_DEVIDS_H_

#ifndef PCI_VENDOR_ID_REDHAT_QUMRANET
/* Vendor ID */
#define PCI_VENDOR_ID_REDHAT_QUMRANET	0x1AF4
#endif

/* Device IDs */
#define N3K_MGMT_INIT_DEV_ID_I100			0xA001
#define N3K_MGMT_INIT_DEV_ID_V100			0x1041	/* virtio 1.0 VF */

#define N3K_MGMT_INIT_SUBDEV_ID_FPA1066GX_DA2	0x15FE
#define N3K_MGMT_INIT_SUBDEV_ID_FPA1066GX_DA2_VF	0x001A

#endif /* _N3K_MGMT_INIT_DEVIDS_H_ */
