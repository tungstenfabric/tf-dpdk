/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (c) 2018, Intel Corporation. */

#ifndef _IFC_OSDEP_H_
#define _IFC_OSDEP_H_

#include "../n3k_mgmt_osdep.h"
#include <linux/types.h>
#include <linux/if_ether.h>
#include <linux/tcp.h>
#include <linux/pci.h>
#include <stddef.h>

#define MSG(l, ...) N3K_MGMT_LOG(INFO, __VA_ARGS__)

#endif /* _IFC_OSDEP_H_ */
