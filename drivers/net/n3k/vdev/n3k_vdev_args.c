/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_vdev_args.h"
#include "n3k_vdev_log.h"
#include "common/n3k_parse_list.h"

#include <errno.h>
#include <rte_bus_vdev.h>
#include <rte_common.h>
#include <rte_ethdev_driver.h>
#include <rte_kvargs.h>
#include <stdlib.h>
#include <string.h>

#define N3K_VDEV_PF_KVARG   ("pf")
#define N3K_VDEV_VFS_KVARG  ("vfs")
#define N3K_VDEV_MGMT_KVARG ("mgmt")
#define N3K_VDEV_LAG_KVARG  ("lag")

#define N3K_VDEV_LIST_DELIM ","

static const char *pmd_n3k_init_valid_arguments[] = {
	N3K_VDEV_PF_KVARG,
	N3K_VDEV_VFS_KVARG,
	N3K_VDEV_MGMT_KVARG,
	N3K_VDEV_LAG_KVARG,
	NULL
};

static int
n3k_vdev_parse_pf_kvarg(const char *key __rte_unused, const char *value,
	void *extra_args)
{
	struct n3k_vdev_args *args;
	struct rte_eth_dev *dev;

	if (!value || !extra_args)
		return -1;

	args = extra_args;

	dev = rte_eth_dev_allocated(value);
	if (!dev) {
		N3K_VDEV_LOG(DEFAULT, ERR, "Cannot find port by name %s", value);
		return -ENODEV;
	}

	/* TODO: Check if the device is N3000 */
	args->pf_dev = dev;

	return 0;
}

static int
n3k_vdev_try_parse_pf_kvarg(
	struct rte_kvargs *kvlist, const char *dev_name, struct n3k_vdev_args *args)
{
	int ret = 0;

	if (rte_kvargs_count(kvlist, N3K_VDEV_PF_KVARG) != 1) {
		N3K_VDEV_LOG(DEFAULT, ERR, "PF must be specified only once for %s", dev_name);
		return -EINVAL;
	}

	ret = rte_kvargs_process(kvlist, N3K_VDEV_PF_KVARG,
		&n3k_vdev_parse_pf_kvarg, args);
	if (ret) {
		N3K_VDEV_LOG(DEFAULT, ERR, "PF device parsing for %s failed", dev_name);
		return ret;
	}

	return 0;
}

static int
n3k_vdev_parse_vf_id(const char *vf_str, void *extra_args)
{
	struct n3k_vdev_args *args;
	unsigned long vf_nb;

	if (!extra_args)
		return -EINVAL;

	args = extra_args;

	vf_nb = strtoul(vf_str, NULL, 10);

	if (vf_nb > N3K_VF_PORTS_MAX_COUNT || vf_nb < N3K_VF_PORT_MIN_ID){
		N3K_VDEV_LOG(DEFAULT, ERR, "Vf id %s is not valid", vf_str);
		return -EINVAL;
	}

	args->vfs_count++;

	if (args->vfs_count > N3K_VF_PORTS_MAX_COUNT){
		N3K_VDEV_LOG(DEFAULT, ERR, "Number of vfs higher than %d", N3K_VF_PORTS_MAX_COUNT);
		return -ERANGE;
	}

	args->vfs[args->vfs_count - 1] = (uint16_t)vf_nb;

	return 0;
}

static int
n3k_vdev_parse_vfs_kvarg(const char *key __rte_unused, const char *value,
	void *extra_args)
{
	struct n3k_vdev_args *args;
	int ret = 0;

	if (!value || !extra_args)
		return -1;

	args = extra_args;

	args->vfs_count = 0;
	memset(args->vfs, 0, sizeof(args->vfs));

	ret = n3k_parse_kvargs_list(value, args, n3k_vdev_parse_vf_id);
	if (ret) {
		N3K_VDEV_LOG(DEFAULT, ERR, "Parsing of vfs argument failed");
		return ret;
	}

	return 0;
}

static int
n3k_vdev_try_parse_vfs_kvarg(
	struct rte_kvargs *kvlist, const char *dev_name, struct n3k_vdev_args *args)
{
	int ret = 0;

	if (rte_kvargs_count(kvlist, N3K_VDEV_VFS_KVARG) != 1) {
		N3K_VDEV_LOG(DEFAULT, ERR, "VFs list must be specified only once for %s", dev_name);
		return -EINVAL;
	}

	ret = rte_kvargs_process(kvlist, N3K_VDEV_VFS_KVARG,
		&n3k_vdev_parse_vfs_kvarg, args);
	if (ret) {
		N3K_VDEV_LOG(DEFAULT, ERR, "VF devices parsing for %s failed", dev_name);
		return ret;
	}

	return 0;
}

static int
n3k_vdev_parse_mgmt_kvarg(const char *key __rte_unused, const char *value,
	void *extra_args)
{
	struct n3k_vdev_args *args;
	struct rte_eth_dev *dev;

	if (!value || !extra_args)
		return -1;

	args = extra_args;

	dev = rte_eth_dev_allocated(value);
	if (!dev) {
		N3K_VDEV_LOG(DEFAULT, ERR, "Cannot find port by name %s", value);
		return -ENODEV;
	}

	/* TODO: Check if the device is N3000 */
	args->mgmt_dev = dev;

	return 0;
}

static int
n3k_vdev_try_parse_mgmt_kvarg(
	struct rte_kvargs *kvlist, const char *dev_name, struct n3k_vdev_args *args)
{
	int ret = 0;

	if (rte_kvargs_count(kvlist, N3K_VDEV_MGMT_KVARG) != 1) {
		N3K_VDEV_LOG(DEFAULT, ERR, "MGMT must be specified only once for %s", dev_name);
		return -EINVAL;
	}

	ret = rte_kvargs_process(kvlist, N3K_VDEV_MGMT_KVARG,
		&n3k_vdev_parse_mgmt_kvarg, args);
	if (ret) {
		N3K_VDEV_LOG(DEFAULT, ERR, "MGMT device parsing for %s failed", dev_name);
		return ret;
	}

	return 0;
}

static int
n3k_vdev_parse_lag_kvarg(const char *key __rte_unused, const char *value,
	void *extra_args)
{
	struct n3k_vdev_args *args;

	if (!value || !extra_args)
		return -1;

	args = extra_args;

	args->is_lag_enabled = (strtoul(value, NULL, 10) != 0);

	return 0;
}

static int
n3k_vdev_try_parse_lag_kvarg(
	struct rte_kvargs *kvlist, const char *dev_name, struct n3k_vdev_args *args)
{
	int ret = 0;

	args->is_lag_enabled = false;
	ret = rte_kvargs_count(kvlist, N3K_VDEV_LAG_KVARG);

	if (ret == 0)
		return 0;

	if (ret > 1) {
		N3K_VDEV_LOG(DEFAULT, ERR,
			"Multiple values of LAG switch for %s are not allowed", dev_name);
		return -EINVAL;
	}

	ret = rte_kvargs_process(kvlist, N3K_VDEV_LAG_KVARG,
		&n3k_vdev_parse_lag_kvarg, args);
	if (ret) {
		N3K_VDEV_LOG(DEFAULT, ERR, "LAG switch parsing for %s failed", dev_name);
		return ret;
	}

	return 0;
}

int
n3k_vdev_args_parse(struct rte_vdev_device *dev, struct n3k_vdev_args *args)
{
	struct rte_kvargs *kvlist;
	const char *name;
	int ret = 0;

	name = rte_vdev_device_name(dev);

	kvlist = rte_kvargs_parse(rte_vdev_device_args(dev),
		pmd_n3k_init_valid_arguments);

	if (!kvlist) {
		N3K_VDEV_LOG(DEFAULT, ERR, "Failed to parse arguments for %s", name);
		return -EINVAL;
	}

	ret = n3k_vdev_try_parse_pf_kvarg(kvlist, name, args);
	if (ret)
		goto exit;

	ret = n3k_vdev_try_parse_vfs_kvarg(kvlist, name, args);
	if (ret)
		goto exit;

	ret = n3k_vdev_try_parse_mgmt_kvarg(kvlist, name, args);
	if (ret)
		goto exit;

	ret = n3k_vdev_try_parse_lag_kvarg(kvlist, name, args);
	if (ret)
		goto exit;

exit:
	rte_kvargs_free(kvlist);
	return ret;
}
