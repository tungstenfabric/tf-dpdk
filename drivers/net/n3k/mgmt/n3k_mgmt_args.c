/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "../n3k.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_args.h"
#include "n3k_mgmt_log.h"

#include <errno.h>
#include <rte_bus_pci.h>
#include <rte_common.h>
#include <rte_devargs.h>
#include <rte_kvargs.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define N3K_MGMT_MQS_KVARG                  ("mqs")
#define N3K_MGMT_PF_MQS_KVARG               ("pf_mqs")
#define N3K_MGMT_INSERT_TYPE_KVARG          ("insert_type")
#define N3K_MGMT_FLR_SIM_SLEEP_PROB_KVARG   ("sleep_prob")
#define N3K_MGMT_FLR_SIM_RX_DROP_PROB_KVARG ("rx_drop_prob")
#define N3K_MGMT_FLR_SIM_TX_DROP_PROB_KVARG ("tx_drop_prob")

#define N3K_MGMT_LIST_DELIM ","

static const char *n3k_mgmt_init_valid_arguments[] = {
	N3K_MGMT_MQS_KVARG,
	N3K_MGMT_PF_MQS_KVARG,
	N3K_MGMT_INSERT_TYPE_KVARG,
	N3K_MGMT_FLR_SIM_SLEEP_PROB_KVARG,
	N3K_MGMT_FLR_SIM_RX_DROP_PROB_KVARG,
	N3K_MGMT_FLR_SIM_TX_DROP_PROB_KVARG,
	NULL
};

// TODO(n3kflow): remove that function after merging drivers
static int
n3k_mgmt_parse_kvargs_list(const char *str, void *args,
	int (*callback)(const char*, void*))
{
	int ret = 0;
	char *dup_str;
	char *ptr;

	if (strlen(str) == 0) {
		N3K_MGMT_LOG(ERR, "Argument is not provided");
		ret = -EINVAL;
		return ret;
	}

	if (*str != '[' || str[strlen(str) - 1] != ']') {
		N3K_MGMT_LOG(ERR, "Argument %s is not a list", str);
		ret = -EINVAL;
		return ret;
	}

	str++;

	dup_str = strndup(str, strlen(str) - 1);
	if (dup_str == NULL) {
		N3K_MGMT_LOG(ERR, "Not enough memory");
		ret = -ENOMEM;
		return ret;
	}
	ptr = strtok(dup_str, N3K_MGMT_LIST_DELIM);

	while (ptr != NULL) {
		ret = callback(ptr, args);
		if (ret)
			goto free_dup_str;
		ptr = strtok(NULL, N3K_MGMT_LIST_DELIM);
	}

free_dup_str:
	free(dup_str);
	return ret;
}

static int
n3k_mgmt_parse_mq_nb(const char *mq_str, void *extra_args)
{
	unsigned long mq_nb;
	struct n3k_mgmt_args *args;

	if (!extra_args)
		return -EINVAL;

	args = extra_args;

	mq_nb = strtoul(mq_str, NULL, 10);

	if (mq_nb > N3K_MGMT_MAX_MQS_PER_DEVICE) {
		N3K_MGMT_LOG(ERR,
			"MQs number %s is not valid. Max %d per device is allowed",
			mq_str, N3K_MGMT_MAX_MQS_PER_DEVICE);
		return -EINVAL;
	}

	args->mqs_param_count++;

	if (args->mqs_param_count > N3K_VF_PORTS_MAX_COUNT) {
		N3K_MGMT_LOG(ERR, "More MQs parameters than max number of VFs %d",
			N3K_VF_PORTS_MAX_COUNT);
		return -ERANGE;
	}

	args->vf_mqs[args->mqs_param_count - 1] = (uint16_t)mq_nb;

	return 0;
}

static int
n3k_mgmt_parse_mqs_kvarg(const char *key __rte_unused, const char *value,
	void *extra_args)
{
	int ret = 0;

	if (!value || !extra_args)
		return -1;

	ret = n3k_mgmt_parse_kvargs_list(value, extra_args, n3k_mgmt_parse_mq_nb);
	if (ret) {
		N3K_MGMT_LOG(ERR, "Parsing of mqs argument failed");
		return ret;
	}

	return 0;
}

static int
n3k_mgmt_try_parse_mqs_kvarg(
	struct rte_kvargs *kvlist, const char *dev_name,
	struct n3k_mgmt_args *args)
{
	int ret;

	args->mqs_param_count = 0;
	memset(args->vf_mqs, 0, sizeof(args->vf_mqs));

	ret = rte_kvargs_count(kvlist, N3K_MGMT_MQS_KVARG);

	if (ret == 0) {
		N3K_MGMT_LOG(DEBUG,
			"Using default mq configuration: 1 queue for VF0");
		args->mqs_param_count = 1;
		args->vf_mqs[0] = 1;
		return 0;
	}

	if (ret > 1) {
		N3K_MGMT_LOG(ERR,
			"Multiple MQ lists for %s are not allowed", dev_name);
		return -EINVAL;
	}

	ret = rte_kvargs_process(kvlist, N3K_MGMT_MQS_KVARG,
		&n3k_mgmt_parse_mqs_kvarg, args);
	if (ret) {
		N3K_MGMT_LOG(ERR, "MQs parsing for %s failed", dev_name);
		return ret;
	}

	return 0;
}

static int
n3k_mgmt_parse_pf_mqs(const char *key __rte_unused, const char *value,
	void *extra_args)
{
	unsigned long mq_nb;
	struct n3k_mgmt_args *args;

	if (!value || !extra_args)
		return -1;
	args = extra_args;

	mq_nb = strtoul(value, NULL, 10);
	if (mq_nb > N3K_MGMT_MAX_MQS_PER_DEVICE) {
		N3K_MGMT_LOG(ERR,
			"MQs number %s is not valid. Max %d for PF is allowed",
			value, N3K_MGMT_MAX_MQS_PER_DEVICE);
		return -EINVAL;
	}

	args->pf_mqs = mq_nb;

	return 0;
}

static int
n3k_mgmt_try_parse_pf_mqs(
	struct rte_kvargs *kvlist, const char *dev_name,
	struct n3k_mgmt_args *args)
{
	int ret = 0;

	args->pf_mqs = 0;
	ret = rte_kvargs_count(kvlist, N3K_MGMT_PF_MQS_KVARG);

	if (ret == 0)
		return 0;

	if (ret > 1) {
		N3K_MGMT_LOG(ERR,
			"Multiple values of PF MQ numbers for %s are not allowed",
			dev_name);
		return -EINVAL;
	}

	ret = rte_kvargs_process(kvlist, N3K_MGMT_PF_MQS_KVARG,
		&n3k_mgmt_parse_pf_mqs, args);
	if (ret) {
		N3K_MGMT_LOG(ERR, "PF MQ number parsing for %s failed", dev_name);
		return ret;
	}

	return 0;
}

static int
n3k_mgmt_parse_insert_type(const char *key __rte_unused, const char *value,
	void *extra_args)
{
	struct n3k_mgmt_args *args;
	if (!extra_args || !value)
		return -EINVAL;
	args = extra_args;

	if (strcmp(value, "flr") == 0) {
		N3K_MGMT_LOG(INFO, "Setting FLR insert mode");
		args->insert_type = N3K_MGMT_HW_INSERT_TYPE_FLR;
		return 0;
	} else if (strcmp(value, "flr-sim") == 0) {
		N3K_MGMT_LOG(INFO, "Setting FLR-SIM insert mode");
		args->insert_type = N3K_MGMT_HW_INSERT_TYPE_FLR_SIM;
		return 0;
	} else if (strcmp(value, "csr") == 0) {
		N3K_MGMT_LOG(INFO, "Setting CSR insert mode");
		args->insert_type = N3K_MGMT_HW_INSERT_TYPE_CSR;
		return 0;
	}

	N3K_MGMT_LOG(ERR, "Invalid insert type %s", value);

	return -EINVAL;
}

static int
n3k_mgmt_try_parse_insert_type_kvarg(struct rte_kvargs *kvlist,
	const char *dev_name,
	struct n3k_mgmt_args *args)
{
	int ret = 0;

	args->insert_type = N3K_MGMT_HW_INSERT_TYPE_CSR;
	ret = rte_kvargs_count(kvlist, N3K_MGMT_INSERT_TYPE_KVARG);

	if (ret == 0) {
		N3K_MGMT_LOG(INFO,
			"Insert mode not specified. Setting CSR insert mode");
		return 0;
	}

	ret = rte_kvargs_process(kvlist, N3K_MGMT_INSERT_TYPE_KVARG,
		&n3k_mgmt_parse_insert_type, args);
	if (ret) {
		N3K_MGMT_LOG(ERR, "Insert type parsing for %s failed", dev_name);
		return ret;
	}

	return 0;
}

static int
n3k_mgmt_parse_prob(const char *key __rte_unused, const char *value,
	void *extra_args)
{
	double *prob = extra_args;
	if (!value || !prob)
		return -EINVAL;
	*prob = strtod(value, NULL);
	if (*prob > 1 || *prob < 0)
		return -EINVAL;
	return 0;
}

static int
n3k_mgmt_try_parse_flr_sim_queue_params_kvarg(struct rte_kvargs *kvlist,
	const char *dev_name,
	struct n3k_mgmt_args *args)
{
	int ret;
	size_t i;
	struct n3k_mgmt_flr_sim_queues_sim_params *qparams = &args->flr_sim_queues_params;

	memset(&args->flr_sim_queues_params, 0, sizeof(args->flr_sim_queues_params));

	struct  {
		const char *name;
		double *val;
	} prob_kvargs[] = {
		{
			.name = N3K_MGMT_FLR_SIM_SLEEP_PROB_KVARG,
			.val = &qparams->sleep_prob
		},
		{
			.name = N3K_MGMT_FLR_SIM_RX_DROP_PROB_KVARG,
			.val = &qparams->rx_drop_prob
		},
		{
			.name = N3K_MGMT_FLR_SIM_TX_DROP_PROB_KVARG,
			.val = &qparams->tx_drop_prob
		}
	};

	for (i = 0; i < RTE_DIM(prob_kvargs); ++i) {
		ret = rte_kvargs_count(kvlist, prob_kvargs[i].name);
		if (ret) {
			ret = rte_kvargs_process(kvlist, prob_kvargs[i].name,
				&n3k_mgmt_parse_prob, prob_kvargs[i].val);
			if (ret < 0) {
				N3K_MGMT_LOG(ERR, "FLR sim %s parsing for %s failed",
					prob_kvargs[i].name, dev_name);
				return ret;
			}
		}
	}

	if (qparams->sleep_prob + qparams->rx_drop_prob + qparams->tx_drop_prob > 1) {
		N3K_MGMT_LOG(ERR,
			"Sum of flr sim queue events probablities is greater than 1");
		return -EINVAL;
	}


	return 0;
}

int
n3k_mgmt_args_parse(struct rte_pci_device *dev, struct n3k_mgmt_args *args)
{
	struct rte_kvargs *kvlist;
	const char *name;
	int ret = 0;

	name = dev->name;
	kvlist = rte_kvargs_parse(dev->device.devargs->args,
		n3k_mgmt_init_valid_arguments);

	if (!kvlist) {
		N3K_MGMT_LOG(ERR, "Failed to parse arguments for %s", name);
		return -EINVAL;
	}

	ret = n3k_mgmt_try_parse_mqs_kvarg(kvlist, name, args);
	if (ret)
		goto exit;

	ret = n3k_mgmt_try_parse_pf_mqs(kvlist, name, args);
	if (ret)
		goto exit;

	ret = n3k_mgmt_try_parse_insert_type_kvarg(kvlist, name, args);
	if (ret)
		goto exit;

	if (args->insert_type == N3K_MGMT_HW_INSERT_TYPE_FLR_SIM) {
		ret = n3k_mgmt_try_parse_flr_sim_queue_params_kvarg(kvlist, name, args);
		if (ret)
			goto exit;
	}

