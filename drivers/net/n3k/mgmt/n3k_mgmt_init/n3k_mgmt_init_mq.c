/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_init_mq.h"
#include "n3k_mgmt_init_mq_hw_access.h"
#include "../n3k_mgmt_log.h"

#include <stdbool.h>

#include <rte_debug.h>

#define N3K_MGMT_INIT_MQ_VF_MAX 128
#define N3K_MGMT_INIT_MQ_PF_MAX 8
#define N3K_MGMT_INIT_MQ_QUEUES_MAX 512

struct n3k_mgmt_init_vq_pool {
	struct n3k_mgmt_init_vq vqs[N3K_MGMT_INIT_MQ_QUEUES_MAX];
	u32 size;
	u32 free;
	u32 busy;
};

struct n3k_mgmt_init_mq_context {
	struct n3k_mgmt_init_vq_pool pool;
	struct n3k_mgmt_init_vq *vf_map[N3K_MGMT_INIT_MQ_PF_MAX][N3K_MGMT_INIT_MQ_VF_MAX];
	struct n3k_mgmt_init_vq *pf_map[N3K_MGMT_INIT_MQ_PF_MAX];
	bool dis_map[N3K_MGMT_INIT_MQ_PF_MAX];
};

static struct n3k_mgmt_init_mq_context n3k_mgmt_init_mq_ctx;

void
n3k_mgmt_init_vq_pool_init(void)
{
	struct n3k_mgmt_init_vq_pool *pool;
	struct n3k_mgmt_init_vq *it;
	u16 i = 0;

	pool = &n3k_mgmt_init_mq_ctx.pool;

	pool->size = sizeof(pool->vqs) / sizeof((pool->vqs)[0]);
	pool->free = pool->size;
	pool->busy = 0;

	for (it = pool->vqs; it != pool->vqs + pool->size; ++it) {
		it->id = i++;
		it->is_available = 1;
		it->is_pf = 0;
		it->is_ctrl = 0;
		it->pf = 0;
		it->vf = 0;
	}
}

static struct n3k_mgmt_init_vq*
n3k_mgmt_init_vq_vf_get(u8 pf, u8 vf)
{
	struct n3k_mgmt_init_mq_context *mq_ctx = &n3k_mgmt_init_mq_ctx;

	if (pf >= N3K_MGMT_INIT_MQ_PF_MAX)
		return NULL;

	if (vf >= N3K_MGMT_INIT_MQ_VF_MAX)
		return NULL;

	return mq_ctx->vf_map[pf][vf];
}

static struct n3k_mgmt_init_vq*
n3k_mgmt_init_vq_pf_get(u8 pf)
{
	struct n3k_mgmt_init_mq_context *mq_ctx = &n3k_mgmt_init_mq_ctx;

	if (pf >= N3K_MGMT_INIT_MQ_PF_MAX)
		return NULL;

	return mq_ctx->pf_map[pf];
}

static void
n3k_mgmt_init_vq_unmap(struct n3k_mgmt_hw *hw, struct n3k_mgmt_init_vq *vqs)
{
	struct n3k_mgmt_init_mq_context *mq_ctx = &n3k_mgmt_init_mq_ctx;
	struct n3k_mgmt_init_vq **map;
	u8 idx;
	u8 hw_idx;

	if (!vqs)
		return;

	if (vqs->pf >= N3K_MGMT_INIT_MQ_PF_MAX) {
		N3K_MGMT_LOG(ERR,
			"given vqs->pf %u under given map exceeds N3K_MGMT_INIT_MQ_PF_MAX %u\n",
			vqs->pf, N3K_MGMT_INIT_MQ_PF_MAX);
		return;
	}

	if (!vqs->is_pf && vqs->vf >= N3K_MGMT_INIT_MQ_VF_MAX) {
		N3K_MGMT_LOG(ERR,
			"given vqs->vf %u under given map exceeds N3K_MGMT_INIT_MQ_VF_MAX %u\n",
			vqs->vf, N3K_MGMT_INIT_MQ_VF_MAX);
		return;
	}

	if (vqs->is_pf) {
		map = mq_ctx->pf_map;
		idx = vqs->pf;
	} else {
		map = mq_ctx->vf_map[vqs->pf];
		idx = vqs->vf;
	}

	if (!map[idx]) {
		N3K_MGMT_LOG(ERR,
			"given idx %u under given map is already unmapped\n",
			idx);
		return;
	}

	map[idx] = NULL;

	hw_idx = vqs->is_pf ? N3K_MGMT_INIT_MQ_VF_MAX - vqs->pf - 1 : vqs->vf;

	n3k_mgmt_init_hw_vf_unmap(hw, hw_idx);

	n3k_mgmt_init_hw_queue_unmap(hw, vqs);

	/* Setting Default Non-MQ Features */
	n3k_mgmt_init_hw_virtio_features_set(hw, hw_idx, false);

	/* Setting Default Queue Pairs As 1 And Queues As 2 */
	n3k_mgmt_init_hw_max_vq_pairs_set(hw, hw_idx, 1);

	n3k_mgmt_init_hw_vqs_set(hw, hw_idx, 1);

	n3k_mgmt_init_hw_pvf_xlate_unset(hw, vqs);
}

static void
n3k_mgmt_init_vq_free(struct n3k_mgmt_init_vq *vqs)
{
	struct n3k_mgmt_init_mq_context *mq_ctx = &n3k_mgmt_init_mq_ctx;
	struct n3k_mgmt_init_vq *it;
	u32 size;
	u8 pf, vf;
	u8 is_pf;

	if (!vqs)
		return;

	size = vqs->size;
	pf = vqs->pf;
	vf = vqs->vf;
	is_pf = vqs->is_pf;

	for (it = vqs; it != vqs + size; ++it)
		it->is_available = 1;

	mq_ctx->pool.free += size;
	mq_ctx->pool.busy -= size;

	if (is_pf) {
		mq_ctx->dis_map[pf] = false;
		return;
	}

	if (vf >= N3K_MGMT_INIT_MQ_VF_MAX - N3K_MGMT_INIT_MQ_PF_MAX) {
		const u8 idx = N3K_MGMT_INIT_MQ_PF_MAX - (vf % N3K_MGMT_INIT_MQ_PF_MAX) - 1;

		mq_ctx->dis_map[idx] = false;
	}
}

static bool
n3k_mgmt_init_vq_vf_is_dis(u8 pf, u8 vf)
{
	struct n3k_mgmt_init_mq_context *mq_ctx = &n3k_mgmt_init_mq_ctx;
	const u8 idx = N3K_MGMT_INIT_MQ_PF_MAX - (vf % N3K_MGMT_INIT_MQ_PF_MAX) - 1;

	(void)pf;

	if (vf < N3K_MGMT_INIT_MQ_VF_MAX - N3K_MGMT_INIT_MQ_PF_MAX)
		return false;

	if (mq_ctx->dis_map[idx]) {
		N3K_MGMT_LOG(ERR,
			"cannot allocate vf %u because pf %u has taken shared resources.\n",
			vf, idx);
		return true;
	}

	return false;
}


static bool
n3k_mgmt_init_vq_pf_is_dis(u8 pf)
{
	struct n3k_mgmt_init_mq_context *mq_ctx = &n3k_mgmt_init_mq_ctx;
	const u8 idx = N3K_MGMT_INIT_MQ_VF_MAX - pf - 1;

	if (!mq_ctx->dis_map[pf])
		return false;

	N3K_MGMT_LOG(ERR,
		"cannot allocate pf %u because vf %u has taken shared resources.\n",
		pf, idx);

	return true;
}


/*
 * n3k_mgmt_init_vq_find_free() - Find continuous space of given size
 * @size: size of required free space
 *
 * Returns pointer to the continuous free space or NULL
 */
static struct n3k_mgmt_init_vq*
n3k_mgmt_init_vq_find_free(struct n3k_mgmt_init_mq_context *mq_ctx, u8 size)
{
	struct n3k_mgmt_init_vq_pool *pool;
	struct n3k_mgmt_init_vq *it;
	u32 continuous = 0;

	pool = &mq_ctx->pool;

	for (it = pool->vqs; it != pool->vqs + pool->size; ++it) {
		if (!it->is_available) {
			continuous = 0;
			continue;
		}

		if (++continuous == size)
			return it - size + 1;
	}

	return NULL;
}

static struct n3k_mgmt_init_vq*
n3k_mgmt_init_vq_alloc(u8 size)
{
	struct n3k_mgmt_init_mq_context *mq_ctx = &n3k_mgmt_init_mq_ctx;
	struct n3k_mgmt_init_vq *it;
	struct n3k_mgmt_init_vq *offset;

	if (!size)
		return NULL;

	if (mq_ctx->pool.free < size) {
		N3K_MGMT_LOG(ERR, "not enough vqs available on pool\n");
		return NULL;
	}

	offset = n3k_mgmt_init_vq_find_free(mq_ctx, size);
	if (!offset) {
		N3K_MGMT_LOG(ERR, "pool is fragmented\n");
		return NULL;
	}

	for (it = offset; it != offset + size; ++it)
		it->is_available = 0;

	offset->size = size;

	mq_ctx->pool.free -= size;
	mq_ctx->pool.busy += size;

	return offset;
}

static int
n3k_mgmt_init_vq_init_impl(struct n3k_mgmt_init_vq *vqs, u8 is_pf, u8 pf, u8 vf)
{
	struct n3k_mgmt_init_vq *it;
	struct n3k_mgmt_init_vq *last;
	u32 size;

	if (!vqs) {
		N3K_MGMT_LOG(ERR,
			"vqs should be allocated first\n");
		return -EINVAL;
	}
	size = vqs->size;

	for (it = vqs; it != vqs + size; ++it) {
		if (it->is_available) {
			N3K_MGMT_LOG(ERR,
				"vqs should be allocated first\n");
			return -EINVAL;
		}
	}

	for (it = vqs; it != vqs + size; ++it) {
		it->is_ctrl = 0;
		it->is_pf = is_pf;
		it->pf = pf;
		it->vf = vf;
	}

	last = --it;

	if (size > 1)
		last->is_ctrl = 1;

	return 0;
}

static int
n3k_mgmt_init_vq_vf_init(struct n3k_mgmt_init_vq *vqs, u8 pf, u8 vf)
{
	struct n3k_mgmt_init_mq_context *mq_ctx = &n3k_mgmt_init_mq_ctx;
	int rc;
	int idx;

	rc = n3k_mgmt_init_vq_init_impl(vqs, false, pf, vf);
	if (rc)
		return rc;

	if (vf >= N3K_MGMT_INIT_MQ_VF_MAX - N3K_MGMT_INIT_MQ_PF_MAX) {
		idx = N3K_MGMT_INIT_MQ_PF_MAX - (vf % N3K_MGMT_INIT_MQ_PF_MAX) - 1;
		mq_ctx->dis_map[idx] = true;
	}

	return 0;
}

static int
n3k_mgmt_init_vq_pf_init(struct n3k_mgmt_init_vq *vqs, u8 pf)
{
	struct n3k_mgmt_init_mq_context *mq_ctx = &n3k_mgmt_init_mq_ctx;
	int rc;

	rc = n3k_mgmt_init_vq_init_impl(vqs, true, pf, N3K_MGMT_INIT_MQ_VF_MAX - pf - 1);
	if (rc)
		return rc;

	mq_ctx->dis_map[pf] = true;

	return 0;
}

static int
n3k_mgmt_init_vq_map(struct n3k_mgmt_hw *hw, struct n3k_mgmt_init_vq *vqs)
{
	struct n3k_mgmt_init_mq_context *mq_ctx = &n3k_mgmt_init_mq_ctx;
	struct n3k_mgmt_init_vq **map;
	u8 idx;
	u8 hw_idx;

	if (!vqs) {
		N3K_MGMT_LOG(ERR,
			"given pointer is a NULL (%p)\n",
		       (void *)vqs);
		return -EINVAL;
	}

	if (vqs->pf >= N3K_MGMT_INIT_MQ_PF_MAX) {
		N3K_MGMT_LOG(ERR,
			"given vqs->pf %u under given map exceeds N3K_MGMT_INIT_MQ_PF_MAX %u\n",
			vqs->pf, N3K_MGMT_INIT_MQ_PF_MAX);
		return -EINVAL;
	}

	if (!vqs->is_pf && vqs->vf >= N3K_MGMT_INIT_MQ_VF_MAX) {
		N3K_MGMT_LOG(ERR,
			"given vqs->vf %u under given map exceeds N3K_MGMT_INIT_MQ_VF_MAX %u\n",
			vqs->vf, N3K_MGMT_INIT_MQ_VF_MAX);
		return -EINVAL;
	}

	if (vqs->is_pf) {
		map = mq_ctx->pf_map;
		idx = vqs->pf;
	} else {
		map = mq_ctx->vf_map[vqs->pf];
		idx = vqs->vf;
	}

	if (map[idx]) {
		N3K_MGMT_LOG(ERR,
			"given idx %u under given map is already mapped\n",
			idx);
		return -EINVAL;
	}

	map[idx] = vqs;

	hw_idx = vqs->is_pf ? N3K_MGMT_INIT_MQ_VF_MAX - vqs->pf - 1 : vqs->vf;

	n3k_mgmt_init_hw_mq_enable(hw);
	n3k_mgmt_init_hw_max_vq_pairs_set(hw, hw_idx, vqs->size);
	n3k_mgmt_init_hw_virtio_features_set(hw, hw_idx, vqs->size != 1);
	n3k_mgmt_init_hw_vqs_set(hw, hw_idx, vqs->size);
	n3k_mgmt_init_hw_vf_map_set(hw, hw_idx, vqs->is_pf, vqs->size, vqs->id);
	n3k_mgmt_init_hw_queue_map_set(hw, vqs);
	n3k_mgmt_init_hw_pvf_xlate_set(hw, vqs);

	return 0;
}

static int
n3k_mgmt_init_set_vq(struct n3k_mgmt_hw *hw, u8 is_pf, u8 pf, u8 vf, u8 queues)
{
	struct n3k_mgmt_init_vq *vqs;
	bool is_dis;

	vqs = is_pf ? n3k_mgmt_init_vq_pf_get(pf) :
		n3k_mgmt_init_vq_vf_get(pf, vf);

	if (!queues) {
		if (!vqs) {
			N3K_MGMT_LOG(ERR, "given %s %u has no assigned queues.\n",
				is_pf ? "pf" : "vf", is_pf ? pf : vf);
			return -EINVAL;
		}

		n3k_mgmt_init_vq_unmap(hw, vqs);
		n3k_mgmt_init_vq_free(vqs);

		return 0;
	}

	if (vqs) {
		N3K_MGMT_LOG(ERR, "given %s %u has already assigned queues.\n",
			is_pf ? "pf" : "vf", is_pf ? pf : vf);
		return -EINVAL;
	}

	is_dis = is_pf ? n3k_mgmt_init_vq_pf_is_dis(pf) :
		 n3k_mgmt_init_vq_vf_is_dis(pf, vf);
	if (is_dis)
		return -EINVAL;

	vqs = n3k_mgmt_init_vq_alloc(queues);
	if (!vqs)
		return -ENOMEM;

	is_pf ? n3k_mgmt_init_vq_pf_init(vqs, pf) :
		n3k_mgmt_init_vq_vf_init(vqs, pf, vf);

	n3k_mgmt_init_vq_map(hw, vqs);

	return 0;
}

struct n3k_mgmt_init_mq_attr_range {
	u32 min;
	u32 max;
	const char * const desc;
};

/* attributes */
enum n3k_mgmt_init_mq_attr {
	N3K_MGMT_INIT_MQ_ATTR_PF,
	N3K_MGMT_INIT_MQ_ATTR_VF,
	N3K_MGMT_INIT_MQ_ATTR_QUEUES,

	__N3K_MGMT_INIT_MQ_ATTR_MAX,
};

#define N3K_MGMT_INIT_MQ_ATTR_MAX  (__N3K_MGMT_INIT_MQ_ATTR_MAX - 1)

static const struct n3k_mgmt_init_mq_attr_range MQ_RANGE[N3K_MGMT_INIT_MQ_ATTR_MAX + 1] = {
	[N3K_MGMT_INIT_MQ_ATTR_PF] = {
		.min = 0,
		.max = N3K_MGMT_INIT_MQ_PF_MAX,
		.desc = "N3K_MGMT_INIT_MQ_ATTR_PF"
	},
	[N3K_MGMT_INIT_MQ_ATTR_VF] = {
		.min = 0,
		.max = N3K_MGMT_INIT_MQ_VF_MAX,
		.desc = "N3K_MGMT_INIT_MQ_ATTR_VF"
	},
	[N3K_MGMT_INIT_MQ_ATTR_QUEUES] = {
		.min = 0,
		.max = N3K_MGMT_INIT_MQ_QUEUES_MAX,
		.desc = "N3K_MGMT_INIT_MQ_ATTR_QUEUES"
	},
};

static int
n3k_mgmt_init_check_range(u32 value, u32 min, u32 max)
{
	if (value < min || value > max)
		return -ERANGE;
	return 0;
}

static int
n3k_mgmt_init_check_attr_range(u32 data, u32 attr)
{
	if (n3k_mgmt_init_check_range(data, MQ_RANGE[attr].min, MQ_RANGE[attr].max)) {
		N3K_MGMT_LOG(ERR, "%s = %u - out of range\n",
			MQ_RANGE[attr].desc, data);
		return -ERANGE;
	}
	return 0;
}

int
n3k_mgmt_init_set_vf_mq(struct n3k_mgmt_hw *hw, u8 pf, u8 vf, u8 queues)
{
	int rc;

	rc = n3k_mgmt_init_check_attr_range(pf, N3K_MGMT_INIT_MQ_ATTR_PF);
	if (rc) {
		N3K_MGMT_LOG(ERR, "pf id out of range: %u.\n", pf);
		return rc;
	}

	rc = n3k_mgmt_init_check_attr_range(vf, N3K_MGMT_INIT_MQ_ATTR_VF);
	if (rc) {
		N3K_MGMT_LOG(ERR, "vf id out of range: %u.\n", vf);
		return rc;
	}

	rc = n3k_mgmt_init_check_attr_range(queues, N3K_MGMT_INIT_MQ_ATTR_QUEUES);
	if (rc) {
		N3K_MGMT_LOG(ERR, "number of queues out of range: %u.\n", queues);
		return rc;
	}

	n3k_mgmt_init_set_vq(hw, 0, pf, vf, queues);

	return 0;
}

int
n3k_mgmt_init_set_pf_mq(struct n3k_mgmt_hw *hw, u8 pf, u8 queues)
{
	int rc;

	rc = n3k_mgmt_init_check_attr_range(pf, N3K_MGMT_INIT_MQ_ATTR_PF);
	if (rc) {
		N3K_MGMT_LOG(ERR, "pf id out of range: %u.\n", pf);
		return rc;
	}

	rc = n3k_mgmt_init_check_attr_range(queues, N3K_MGMT_INIT_MQ_ATTR_QUEUES);
	if (rc) {
		N3K_MGMT_LOG(ERR, "number of queues out of range: %u.\n", queues);
		return rc;
	}

	n3k_mgmt_init_set_vq(hw, 1, pf, 0, queues);

	return 0;
}

static u8
n3k_mgmt_init_get_queue_count(struct n3k_mgmt_init_vq *vqs)
{
	if (!vqs) {
		return 1;
	}

	// When user specify queue count for some n3k interface,
	// additional queue internally is added, which is used as a virtio control queue
	// If MQ configuration has only one queue, it means that there is error in logic,
	// and control vq is not added.
	RTE_VERIFY(vqs->size >= 1);

	return vqs->size - 1;
}

void
n3k_mgmt_init_get_pf_mq(u8 pf, u8 *queues)
{
	struct n3k_mgmt_init_vq *vqs;

	vqs = n3k_mgmt_init_vq_pf_get(pf);

	if (!vqs)
		N3K_MGMT_LOG(DEBUG, "couldn't find queues for pf %u, assuming non-MQ mode\n", pf);

	*queues = n3k_mgmt_init_get_queue_count(vqs);
}

void
n3k_mgmt_init_get_vf_mq(u8 pf, u8 vf, u8 *queues)
{
	struct n3k_mgmt_init_vq *vqs;

	vqs = n3k_mgmt_init_vq_vf_get(pf, vf);

	if (!vqs)
		N3K_MGMT_LOG(DEBUG, "couldn't find queues for pf %u, vf %u. assuming non-MQ mode\n", pf, vf);

	*queues = n3k_mgmt_init_get_queue_count(vqs);
}
