/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "vdev/n3k_vdev_device.h"
#include "n3k_mgmt_ethdev.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_ports.h"
#include "n3k_mgmt_lag.h"

#include <rte_eth_bond.h>
#include <rte_ethdev.h>

#include <stdint.h>

#define MAC0_PORT	0
#define MAC1_PORT	1
#define VF_PORT		3

#define MAC0_MOD_PORT	128
#define MAC1_MOD_PORT	129

#define VF_SUBPORT_MIN	1
#define VF_SUBPORT_MAX	127

#define PHY_VID_BASE	101
#define MAC0_VID	101
#define MAC1_VID	102
#define LAG_VID 55
#define LAG_PORT 0
#define LAG_SUBPORT 55
#define VF_VID_BASE	127
#define VF_VID_MAX	254

static int
port_subport_to_vid(uint8_t port, uint8_t subport, uint8_t *vid)
{
	if (port == MAC0_PORT && subport == 0) {
		*vid = MAC0_VID;
		return 0;
	}

	if (port == MAC1_PORT && subport == 0) {
		*vid = MAC1_VID;
		return 0;
	}

	if (port == VF_PORT && subport >= VF_SUBPORT_MIN && subport <= VF_SUBPORT_MAX) {
		*vid = VF_VID_BASE + subport;
		return 0;
	}

	if (port == LAG_PORT && subport == LAG_SUBPORT) {
		*vid = LAG_VID;
		return 0;
	}

	N3K_MGMT_LOG(DEFAULT,
		ERR, "PortSubport to VID: Invalid combination: port = %d, subport = %d",
		port, subport);
	return -EINVAL;
}

static int
vid_to_port_subport(uint8_t vid, uint8_t *port, uint8_t *subport)
{
	if (vid == MAC0_VID) {
		*port = MAC0_PORT;
		*subport = 0;
		return 0;
	}

	if (vid == MAC1_VID) {
		*port = MAC1_PORT;
		*subport = 0;
		return 0;
	}

	if (vid > VF_VID_BASE && vid <= VF_VID_MAX) {
		*port = VF_PORT;
		*subport = vid - VF_VID_BASE;
		return 0;
	}

	if (vid == LAG_VID) {
		*port = LAG_PORT;
		*subport = LAG_SUBPORT;
		return 0;
	}

	N3K_MGMT_LOG(DEFAULT, ERR, "VID to PortSubport: Invalid VID: %d", vid);
	return -EINVAL;
}

static int
vid_to_modifier_port(uint8_t vid, uint8_t *mod_port)
{
	if (vid == MAC0_VID) {
		*mod_port = MAC0_MOD_PORT;
		return 0;
	}

	if (vid == MAC1_VID) {
		*mod_port = MAC1_MOD_PORT;
		return 0;
	}

	if (vid > VF_VID_BASE && vid <= VF_VID_MAX) {
		*mod_port = vid - VF_VID_BASE;
		return 0;
	}

	N3K_MGMT_LOG(DEFAULT, ERR, "Invalid VID: %d", vid);
	return -EINVAL;
}

int
n3k_mgmt_dpdk_port_to_hw_vid(
	struct n3k_mgmt_hw *hw, uint16_t dpdk_port, uint8_t *vid)
{
	enum n3k_vdev_dev_type type;
	uint16_t id;
	int lag_port_id;

	if (dpdk_port >= RTE_MAX_ETHPORTS)
		return -EINVAL;

	lag_port_id = n3k_mgmt_get_lag_port_id(hw);

	if (lag_port_id == dpdk_port) {
		*vid = LAG_VID;
		return 0;
	}

	struct rte_eth_dev *repr = &rte_eth_devices[dpdk_port];
	if (!n3k_vdev_is_valid_n3k_repr(repr))
		return -ENODEV;

	n3k_vdev_get_representor_info(repr, &type, &id);
	*vid = (type == N3K_VDEV_DEV_TYPE_VF ? VF_VID_BASE : PHY_VID_BASE) + id;

	return 0;
}

int
n3k_vdev_dpdk_port_to_vdev_id(uint16_t dpdk_port,
	struct n3k_vdev_dev_id *device_id)
{
	if (!rte_eth_dev_is_valid_port(dpdk_port))
		return -EINVAL;

	if (n3k_mgmt_is_port_valid_lag_bond(dpdk_port) == 0) {
		device_id->type = N3K_VDEV_DEV_TYPE_LAG;
		device_id->id = 0;
		return 0;
	}

	struct rte_eth_dev *repr = &rte_eth_devices[dpdk_port];
	if (!n3k_vdev_is_valid_n3k_repr(repr)) {
		return -ENODEV;
	}

	n3k_vdev_get_representor_info(repr, &device_id->type, &device_id->id);
	return 0;
}

int n3k_mgmt_dpdk_port_to_hw_port_subport(struct n3k_mgmt_hw *hw,
	uint16_t dpdk_port, uint8_t *port, uint8_t *subport)
{
	uint8_t vid;
	int ret;

	ret = n3k_mgmt_dpdk_port_to_hw_vid(hw, dpdk_port, &vid);
	if (ret)
		return ret;

	return vid_to_port_subport(vid, port, subport);
}

int
n3k_mgmt_dpdk_port_to_hw_modifier_port(struct n3k_mgmt_hw *hw,
	uint16_t dpdk_port, uint8_t *mod_port)
{
	uint8_t vid;
	int ret;

	ret = n3k_mgmt_dpdk_port_to_hw_vid(hw, dpdk_port, &vid);
	if (ret)
		return ret;

	return vid_to_modifier_port(vid, mod_port);
}

int
n3k_mgmt_hw_vid_to_dpdk_port(struct n3k_mgmt_hw *hw, uint8_t vid,
	uint16_t *dpdk_port)
{
	int lag_port_id = n3k_mgmt_get_lag_port_id(hw);

	if (lag_port_id >= 0 && vid == LAG_VID) {
		*dpdk_port = lag_port_id;
		return 0;
	}

	if (vid != MAC0_VID && vid != MAC1_VID &&
		(vid <= VF_VID_BASE || vid > VF_VID_MAX))
		return -EINVAL;

	struct rte_eth_dev *repr = vid < VF_VID_BASE ?
		hw->phy_representors[vid - PHY_VID_BASE] :
		hw->vf_representors[vid - VF_VID_BASE];
	if (repr == NULL)
		return -ENODEV;

	*dpdk_port = repr->data->port_id;
	return 0;
}

int
n3k_mgmt_hw_port_subport_to_dpdk_port(struct n3k_mgmt_hw *hw,
	uint8_t port, uint8_t subport, uint16_t *dpdk_port)
{
	uint8_t vid = 0;
	int ret;

	ret = port_subport_to_vid(port, subport, &vid);
	if (ret)
		return ret;

	return n3k_mgmt_hw_vid_to_dpdk_port(hw, vid, dpdk_port);
}
