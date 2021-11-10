/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_mgmt_init/n3k_mgmt_init_port.h"
#include "n3k_mgmt_bitwise.h"
#include "n3k_mgmt_conversions.h"
#include "n3k_mgmt_dump.h"
#include "n3k_mgmt_hw.h"
#include "n3k_mgmt_lock.h"
#include "n3k_mgmt_log.h"
#include "n3k_mgmt_regs_ctrl.h"
#include "n3k_mgmt_vplkp_tbl.h"
#include "regs/n3k_mgmt_regs.h"
#include "regs/n3k_mgmt_vplkp_tbl_regs_api.h"

#include <errno.h>
#include <rte_debug.h>
#include <stdint.h>

int n3k_mgmt_vplkp_tbl_init(struct n3k_mgmt_hw *hw)
{
	int ret;

	N3K_MGMT_LOG(VPLKP, INFO, "Initializing vplkp tbl");

	ret = n3k_mgmt_poll_ctrl_reg(hw, N3K_MGMT_CTRL_REG_VPLKP_EMLC_GEN_CTRL);
	if (ret != 0) {
		N3K_MGMT_LOG(VPLKP, ERR, "Failed to initialize vplkp tbl");
		return ret;
	}

	ret = n3k_mgmt_vplkp_tbl_flush(hw);
	if (ret != 0) {
		N3K_MGMT_LOG(VPLKP, ERR, "Failed to flush vplkp tbl");
		return ret;
	}
	return 0;
}

static int
exec_vplkp_tbl_cmd_get_result(struct n3k_mgmt_hw *hw,
	enum VPLKP_EMLC_MGMT_CTRL_CMDS type,
	MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_t *mgmt_ctrl)
{
	memset(mgmt_ctrl, 0, sizeof(*mgmt_ctrl));
	mgmt_ctrl->field.op_type = type;
	csr_write(hw, MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE,
		MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL, mgmt_ctrl->val);
	return n3k_mgmt_poll_ctrl_reg_with_result(hw,
		N3K_MGMT_CTRL_REG_VPLKP_EMLC_MGMT_CTRL, &mgmt_ctrl->val);
}

static int
exec_vplkp_tbl_cmd(struct n3k_mgmt_hw *hw,
	enum VPLKP_EMLC_MGMT_CTRL_CMDS type)
{
	MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_t mgmt_ctrl = {{0}};
	return exec_vplkp_tbl_cmd_get_result(hw, type, &mgmt_ctrl);
}

int n3k_mgmt_vplkp_tbl_flush(struct n3k_mgmt_hw *hw)
{
	int ret;

	N3K_MGMT_LOG(VPLKP, INFO, "Flushing vplkp table");

	n3k_mgmt_lock(hw);

	ret = exec_vplkp_tbl_cmd(hw, VPLKP_EMLC_MGMT_CTRL_CMD_FLUSH);
	if (ret < 0) {
		N3K_MGMT_LOG(VPLKP, ERR,
			"Error occurred during Virtual Port lookup flush operation: %d",
			ret);
		goto release_lock;
	}

	ret = 0;

release_lock:
	n3k_mgmt_unlock(hw);
	return ret;
}

int n3k_mgmt_vplkp_tbl_insert(struct n3k_mgmt_hw *hw, uint32_t key,
	const struct n3k_mgmt_vplkp_tbl_res *res)
{
	MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_0_t key_reg = {{0}};
	MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_t res_reg = {{0}};
	int ret;

	N3K_MGMT_LOG(VPLKP, INFO, "Adding vplkp entry: label=%" PRIu32, key);
	if (N3K_MGMT_SHOULD_LOG(VPLKP, DEBUG))
		n3k_mgmt_vplkp_tbl_res_dump(res);

	if (key >= (1 << 20)) {
		N3K_MGMT_LOG(VPLKP, ERR, "Invalid vplkp label: label=%" PRIu32, key);
		return -EINVAL;
	}
	if (res->keymask >= 8 || res->vport >= 4) {
		N3K_MGMT_LOG(VPLKP, ERR, "Invalid vplkp result: label=%" PRIu32, key);
		return -EINVAL;
	}

	key_reg.field.key = 0xff000000 | key;
	res_reg.field.keymask = res->keymask;
	res_reg.field.vport = res->vport;

	if (N3K_MGMT_SHOULD_LOG(REGS, DEBUG)) {
		N3K_MGMT_LOG(REGS, DEBUG, "Adding vplkp entry: regs");
		printf("MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_0_t = 0x%.8" PRIx32 ", 0b%s\n",
			key_reg.val, n3k_mgmt_convert_uint_to_bits_str(key_reg.val, 32));
		printf("MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_t = 0x%.8" PRIx32 ", 0b%s\n",
			res_reg.val, n3k_mgmt_convert_uint_to_bits_str(res_reg.val, 32));
	}

	n3k_mgmt_lock(hw);

	csr_write(hw, MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE,
		MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_0,
		key_reg.val);
	csr_write(hw, MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE,
		MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0,
		res_reg.val);

	ret = exec_vplkp_tbl_cmd(hw, VPLKP_EMLC_MGMT_CTRL_CMD_INSERT);
	if (ret < 0) {
		N3K_MGMT_LOG(VPLKP, ERR,
			"Error occurred during Virtual Port lookup insert operation: %d",
			ret);
		goto release_lock;
	}

	ret = 0;

release_lock:
	n3k_mgmt_unlock(hw);
	return ret;
}

int n3k_mgmt_vplkp_tbl_delete(struct n3k_mgmt_hw *hw, uint32_t key)
{
	MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_0_t key_reg = {{0}};
	int ret;

	N3K_MGMT_LOG(VPLKP, INFO, "Deleting vplkp entry: label=%" PRIu32, key);

	if (key >= (1 << 20)) {
		N3K_MGMT_LOG(VPLKP, ERR, "Invalid vplkp label: label=%" PRIu32, key);
		return -EINVAL;
	}

	key_reg.field.key = 0xff000000 | key;

	if (N3K_MGMT_SHOULD_LOG(REGS, DEBUG)) {
		N3K_MGMT_LOG(REGS, DEBUG, "Deleting vplkp entry: regs");
		printf("MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_0_t = 0x%.8" PRIx32 ", 0b%s\n",
			key_reg.val, n3k_mgmt_convert_uint_to_bits_str(key_reg.val, 32));
	}

	n3k_mgmt_lock(hw);

	csr_write(hw, MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE,
		MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_0,
		key_reg.val);

	ret = exec_vplkp_tbl_cmd(hw, VPLKP_EMLC_MGMT_CTRL_CMD_DELETE);
	if (ret < 0) {
		N3K_MGMT_LOG(VPLKP, ERR,
			"Error occurred during Virtual Port lookup delete operation: %d",
			ret);
		goto release_lock;
	}

	ret = 0;

release_lock:
	n3k_mgmt_unlock(hw);
	return ret;
}

int n3k_mgmt_vplkp_tbl_search(struct n3k_mgmt_hw *hw, uint32_t key,
	struct n3k_mgmt_vplkp_tbl_res *res)
{
	MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_MGMT_CTRL_t mgmt_ctrl = {{0}};
	MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_0_t key_reg = {{0}};
	MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0_t res_reg = {{0}};
	int ret;

	N3K_MGMT_LOG(VPLKP, DEBUG, "Searching vplkp entry: label=%" PRIu32, key);

	if (key >= (1 << 20)) {
		N3K_MGMT_LOG(VPLKP, ERR, "Invalid vplkp label: label=%" PRIu32, key);
		return -EINVAL;
	}

	n3k_mgmt_lock(hw);

	key_reg.field.key = 0xff000000 | key;
	csr_write(hw, MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE,
		MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_KEY_0,
		key_reg.val);

	ret = exec_vplkp_tbl_cmd_get_result(hw, VPLKP_EMLC_MGMT_CTRL_CMD_SEARCH, &mgmt_ctrl);
	if (ret < 0) {
		N3K_MGMT_LOG(VPLKP, ERR,
			"Error occurred during Virtual Port lookup search operation: %d",
			ret);
		goto release_lock;
	}
	if (mgmt_ctrl.field.success == 0) {
		// Entry not found
		goto release_lock;
	}

	res_reg.val = csr_read(hw, MKS_CHIP_MKS_VPORTLKUP_TUNNEL_TABLE_BASE,
		MKS_VPORTLKUP_TUNNEL_TABLE_VPLKP_EMLC_RES_0);
	res->keymask = res_reg.field.keymask;
	res->vport = res_reg.field.vport;

	if (N3K_MGMT_SHOULD_LOG(VPLKP, DEBUG)) {
		N3K_MGMT_LOG(VPLKP, DEBUG, "Found vplkp entry: label=%" PRIu32, key);
		n3k_mgmt_vplkp_tbl_res_dump(res);
	}

	RTE_ASSERT(res->keymask < 8);
	RTE_ASSERT(res->vport < 4);

	ret = 1;

release_lock:
	n3k_mgmt_unlock(hw);
	return ret;
}

int n3k_mgmt_vplkp_tbl_dump(struct n3k_mgmt_hw *hw, uint32_t key)
{
	struct n3k_mgmt_vplkp_tbl_res res;
	int ret;

	ret = n3k_mgmt_vplkp_tbl_search(hw, key, &res);
	if (ret < 0)
		return ret;

	n3k_mgmt_vplkp_tbl_res_dump(&res);
	return 0;
}
