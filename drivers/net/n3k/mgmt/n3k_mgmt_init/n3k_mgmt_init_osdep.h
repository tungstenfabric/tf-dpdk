/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _N3K_MGMT_INIT_OSDEP_H_
#define _N3K_MGMT_INIT_OSDEP_H_

#include "../n3k_mgmt_osdep.h"
#include <linux/types.h>
#include <linux/if_ether.h>
#include <linux/tcp.h>
#include <linux/pci.h>
#include <stddef.h>

#define MSG(l, ...) N3K_MGMT_LOG(INFO, __VA_ARGS__)

#endif /* _N3K_MGMT_INIT_OSDEP_H_ */
