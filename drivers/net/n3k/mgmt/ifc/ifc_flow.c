// SPDX-License-Identifier: GPL-2.0
/* Copyright (c) 2018, Intel Corporation. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../n3k_mgmt_log.h"
#include "ifc_flow.h"
#include "ifc_port.h"
#include "../n3k_mgmt_regs.h"
#include "../n3k_mgmt_lock.h"

/*
 * Flags are encoded as follows:
 *         SYN, ACK, FIN, RST
 */
static const uint8_t ifc_ct_hw_flags[IFC_TCP_FLAGS_MAX] = {
	[IFC_TCP_SYN_SET]    = 0x08, //1000 IFC_TCP_SYN_SET,
	[IFC_TCP_SYN_ACK_SET]= 0x0C, //1100 IFC_TCP_SYN_ACK_SET,
	[IFC_TCP_FIN_SET]    = 0x02, //0010 IFC_TCP_FIN_SET,
	[IFC_TCP_FIN_ACK_SET]= 0x06, //0110 IFC_TCP_FIN_ACK_SET,
	[IFC_TCP_ACK_SET]    = 0x04, //0100 IFC_TCP_ACK_SET,
	[IFC_TCP_RST_SET]    = 0x01, //0001 IFC_TCP_RST_ACK_SET,
	[IFC_TCP_RST_ACK_SET]= 0x05, //0101 IFC_TCP_RST_SET,
	[IFC_TCP_NONE_SET]   = 0x00, //0000 IFC_TCP_NONE_SET,
};

const char *ifc_ct_state_name[] = {
	[sNO] = " IFC_sNO",
	[sSS] = " IFC_sSS",
	[sSR] = " IFC_sSR",
	[sES] = " IFC_sES",
	[sFW] = " IFC_sFW",
	[sCW] = " IFC_sCW",
	[sLA] = " IFC_sLA",
	[sTW] = " IFC_sTW",
	[sCL] = " IFC_sCL",
	[sS2] = " IFC_sS2",
	[sIV] = " IFC_sIV",
	[sIG] = " IFC_sIG",

	[sUI] = " IFC_sUI",
	[sUE] = " IFC_sUE",
	[sII] = " IFC_sII",
	[sIE] = " IFC_sIE",

//	[sNO] = "        IFC_TCP_CONNTRACK_NONE",
//	[sSS] = "    IFC_TCP_CONNTRACK_SYN_SENT",
//	[sSR] = "    IFC_TCP_CONNTRACK_SYN_RCVD",
//	[sES] = " IFC_TCP_CONNTRACK_ESTABLISHED",
//	[sFW] = "    IFC_TCP_CONNTRACK_FIN_WAIT",
//	[sCW] = "  IFC_TCP_CONNTRACK_CLOSE_WAIT",
//	[sLA] = "    IFC_TCP_CONNTRACK_LAST_ACK",
//	[sTW] = "   IFC_TCP_CONNTRACK_TIME_WAIT",
//	[sCL] = "       IFC_TCP_CONNTRACK_CLOSE",
//	[sS2] = "   IFC_TCP_CONNTRACK_SYN_SENT2",
//	[sIV] = "     IFC_TCP_CONNTRACK_INVALID",
//	[sIG] = "      IFC_TCP_CONNTRACK_IGNORE",
//	[sUI] = "        IFC_UDP_CONNTRACK_INIT",
//	[sUE] = " IFC_UDP_CONNTRACK_ESTABLISHED",
//	[sII] = "       IFC_ICMP_CONNTRACK_INIT",
//	[sIE] = "IFC_ICMP_CONNTRACK_ESTABLISHED",
};

const char *ifc_ct_flag_name[] = {
	[IFC_TCP_SYN_SET]    = "    SYN",
	[IFC_TCP_SYN_ACK_SET]= "SYN+ACK",
	[IFC_TCP_FIN_SET]    = "    FIN",
	[IFC_TCP_FIN_ACK_SET]= "FIN+ACK",
	[IFC_TCP_ACK_SET]    = "    ACK",
	[IFC_TCP_RST_SET]    = "    RST",
	[IFC_TCP_RST_ACK_SET]= "RST+ACK",
	[IFC_TCP_NONE_SET]   = "   NONE",
	[IFC_TCP_FLAGS_MAX]  = "    MAX",
};

const char *ifc_ct_ptype_name[] = {
	[IFC_CT_PTYPE_REQ] = " REQUEST",
	[IFC_CT_PTYPE_RSP] = "RESPONSE",
};

/*
 * TCP transition table for both ORIGINAL and REPLY direction.
 */
static const uint8_t
ct_tcp_transition[2][IFC_TCP_FLAGS_MAX][IFC_TCP_CONNTRACK_MAX] = {

	/*
	 * For sIG and sIV we are retaining the original states.
	 * Netfilter does additional processing when the sIG or sIV
	 * state is encountered. Based on that, it chooses to either
	 * change to a different state or not change state at all.
	 */
	{
		/* ORIGINAL */
		/*         sNO, sSS, sSR, sES, sFW, sCW, sLA, sTW, sCL, sS2   */
		/*syn     {sSS, sSS, sIG, sIG, sIG, sIG, sIG, sSS, sSS, sS2 },*/
		/*syn*/   {sSS, sSS, sSR, sES, sFW, sCW, sLA, sSS, sSS, sS2 },

		/*         sNO, sSS, sSR, sES, sFW, sCW, sLA, sTW, sCL, sS2   */
		/*synack  {sIV, sIV, sSR, sIV, sIV, sIV, sIV, sIV, sIV, sSR}, */
		/*synack*/{sNO, sSS, sSR, sES, sFW, sCW, sIV, sTW, sCL, sSR},

		/* 	   sNO, sSS, sSR, sES, sFW, sCW, sLA, sTW, sCL, sS2   */
		/*fin*/   {sNO, sSS, sFW, sFW, sLA, sLA, sLA, sTW, sCL, sS2},
		/*finack*/{sNO, sSS, sFW, sFW, sLA, sLA, sLA, sTW, sCL, sS2},

		/* 	   sNO, sSS, sSR, sES, sFW, sCW, sLA, sTW, sCL, sS2   */
		/*ack*/	  {sES, sSS, sES, sES, sCW, sCW, sTW, sTW, sCL, sS2},

		/* 	   sNO, sSS, sSR, sES, sFW, sCW, sLA, sTW, sCL, sS2   */
		/*rst*/   {sNO, sCL, sCL, sCL, sCL, sCL, sCL, sCL, sCL, sCL},
		/*rstack*/{sNO, sCL, sCL, sCL, sCL, sCL, sCL, sCL, sCL, sCL},

		/*none    {sIV, sIV, sIV, sIV, sIV, sIV, sIV, sIV, sIV, sIV } */
		/*none*/  {sNO, sSS, sSR, sES, sFW, sCW, sLA, sTW, sCL, sS2 }
	},

	{
		/* REPLY */
		/* 	  sNO, sSS, sSR, sES, sFW, sCW, sLA, sTW, sCL, sS2 */
		/*syn*/	 {sNO, sS2, sSR, sES, sFW, sCW, sLA, sSS, sCL, sS2 },

		/* 	   sNO, sSS, sSR, sES, sFW, sCW, sLA, sTW, sCL, sS2  */
		/*synack  {sIV, sSR, sIG, sIG, sIG, sIG, sIG, sIG, sIG, sSR },*/
		/*synack*/{sNO, sSR, sSR, sES, sFW, sCW, sLA, sTW, sCL, sSR },

		/* 	   sNO, sSS, sSR, sES, sFW, sCW, sLA, sTW, sCL, sS2   */
		/*fin*/   {sNO, sSS, sFW, sFW, sLA, sLA, sLA, sTW, sCL, sS2 },
		/*finack*/{sNO, sSS, sFW, sFW, sLA, sLA, sLA, sTW, sCL, sS2 },

		/* 	  sNO, sSS, sSR, sES, sFW, sCW, sLA, sTW, sCL, sS2 */
		/*ack  	 {sIV, sIG, sSR, sES, sCW, sCW, sTW, sTW, sCL, sIG },*/
		/*ack*/	 {sNO, sSS, sSR, sES, sCW, sCW, sTW, sTW, sCL, sS2 },

		/* 	   sNO, sSS, sSR, sES, sFW, sCW, sLA, sTW, sCL, sS2   */
		/*rst*/   {sNO, sCL, sCL, sCL, sCL, sCL, sCL, sCL, sCL, sCL },
		/*rstack*/{sNO, sCL, sCL, sCL, sCL, sCL, sCL, sCL, sCL, sCL },

		/*none*/  {sNO, sSS, sSR, sES, sFW, sCW, sLA, sTW, sCL, sS2 }
	}
};

/*
 * Ptype: request and response both.
 * Flags: Only "None" flag is expected.
 */
#define IFC_NONTCP_CONNTRACK_STATES (IFC_NONTCP_CONNTRACK_MAX -  \
				     IFC_NONTCP_CONNTRACK_NONE)
/*
 * The flags index is ommitted as we are not expecting any TCP flags
 * coming from such packets.
 */
static const uint8_t
ct_nontcp_transition[2][IFC_NONTCP_CONNTRACK_STATES] = {

	/* Reqeust ptype */
	{
		/*         sUI, sUE, sII, sIE  */
		/*none*/ sUI, sUE, sII, sIE
	},

	/* Response ptype */
	{
		/*         sUI, sUE, sII, sIE  */
		/*none*/ sUE, sUE, sIE, sIE
	}
};

int
ifc_ct_upd_transition_tbl(struct n3k_mgmt_hw *hw, u32 i, u32 new_state, u32 old_state)
{
	MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_t cfg;
	int rc;

	rc = ifc_gen_cmd_check(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE,
			MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG,
			MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG_BUSY_SHIFT,
			MKS_FT_CMD_TOUT);
	if (rc)
		return rc;

	cfg.val = csr_read(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE,
			MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG);
	cfg.field.index = i;
	if (new_state == IFC_TCP_CONNTRACK_ESTABLISHED ||
	    new_state == IFC_UDP_CONNTRACK_ESTABLISHED ||
	    new_state == IFC_UDP_CONNTRACK_INIT)
		cfg.field.dis_tstamp_update = IDLE_TIMEOUT_MODE;
	else if (old_state == new_state)
		cfg.field.dis_tstamp_update = HARD_TIMEOUT_MODE;
	else
		cfg.field.dis_tstamp_update = IDLE_TIMEOUT_MODE;
	cfg.field.new_state = new_state;
	cfg.field.operation = IFC_CT_XTION_TBL_WRITE;

	csr_write(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE,
			MKS_FLOW_TRACKER_FT_STATE_TABLE_CFG,
			cfg.val);
	return 0;
}

int
ifc_ct_pgm_state_xtion_table(struct n3k_mgmt_hw *hw)
{
	uint32_t ptype, flag_bits, flag, old_state, i, new_state;
	uint32_t num_valid = 0;
	int rc;

	/*
	 * ptype[0], current_state[4:0], SYN, ACK, FIN, RST
	 *
	 * Here is what index would be composed like.
	 *      ptype: state:  flags
	 *         x   xxxxx   xxxx
	 *  bits   9   87654   3210
	 */
	for (ptype = IFC_FLOW_PTYPE_REQ; ptype <= IFC_FLOW_PTYPE_RSP; ptype++) {

		for (flag = IFC_TCP_FLAGS_START;
				flag < IFC_TCP_FLAGS_MAX; flag++) {

			for (old_state = IFC_TCP_CONNTRACK_NONE;
					old_state < IFC_TCP_CONNTRACK_MAX;
					old_state++) {

				flag_bits = ifc_ct_hw_flags[flag];

				i = hw_state_table_idx(ptype,
							   old_state, flag_bits);
				new_state =
				    ct_tcp_transition[ptype][flag][old_state];

				/*
				 * Write new state at the index calculated.
				 */
				rc = ifc_ct_upd_transition_tbl(hw, i, new_state,
							  old_state);
				if (rc)
					return rc;
				num_valid++;
				N3K_MGMT_LOG(DEBUG, "%ld:ptype:%s flag:%s old_st:%s new_st:%s",
								(long int) num_valid,
								ifc_ct_ptype_name[ptype],
								ifc_ct_flag_name[flag],
								ifc_ct_state_name[old_state],
								ifc_ct_state_name[new_state]);
			}
		}
	}
	return 0;
}

int
ifc_ct_pgm_nontcp_xtion_table(struct n3k_mgmt_hw *hw)
{
	uint32_t ptype, flag_bits, flag, old_state, state, i, new_state;
	uint32_t num_valid = 0;
	int rc;

	/*
	 * ptype[0], current_state[4:0], SYN, ACK, FIN, RST
	 *
	 * Here is what index would be composed like.
	 *      ptype: state:  flags
	 *         x   xxxxx   xxxx
	 *  bits   9   87654   3210
	 */
	flag = IFC_TCP_NONE_SET;
	flag_bits = ifc_ct_hw_flags[flag];

	N3K_MGMT_LOG(DEBUG, "sizeof nontcp tbl:%lu", sizeof(ct_nontcp_transition));

	for (ptype = IFC_FLOW_PTYPE_REQ; ptype <= IFC_FLOW_PTYPE_RSP; ptype++) {

		for (old_state = IFC_NONTCP_CONNTRACK_NONE;
				old_state < IFC_NONTCP_CONNTRACK_MAX;
				old_state++) {

			state = old_state - IFC_NONTCP_CONNTRACK_NONE;
			i = hw_state_table_idx(ptype, old_state, flag_bits);
			new_state = ct_nontcp_transition[ptype][state];
			N3K_MGMT_LOG(DEBUG, "state:%d new_state:%d", state, new_state);

			/*
			 * Write new state at the index calculated.
			 */
			rc = ifc_ct_upd_transition_tbl(hw, i, new_state, old_state);
			if (rc)
				return rc;

			num_valid++;
			N3K_MGMT_LOG(DEBUG, "nontcp %ld:ptype:%s flag:%s old:%s new:%s",
				(long int) num_valid,
				ifc_ct_ptype_name[ptype],
				ifc_ct_flag_name[flag],
				ifc_ct_state_name[old_state],
				ifc_ct_state_name[new_state]);
		}
	}

	return 0;
}

int
ifc_gen_cmd_check(struct n3k_mgmt_hw* hw, u32 base, u32 offset, u32 busy_off,
		u32 timeout)
{
	uint32_t data;

	/*
	 * Status 0 means that previous command has completed.
	 * Status 1 means that previous command is still pending.
	 */
	data = csr_read(hw, base, offset);

	while((CT_RD_FIELD(data, busy_off, 1)) && (timeout > 0)) {
		timeout --;
		N3K_MGMT_LOG(DEBUG, "data:0x%x", data);
		data = csr_read(hw, base, offset);
	}

	/*
	 * if timeout, return an error
	 */
	if (timeout == 0) {
		N3K_MGMT_LOG(ERR, "Timed out");
		return -ETIMEDOUT;
	}

	return 0;
}

/*
 * Function to initialize the stats block. Firmware will need to wait
 * for flow_tracker block to finish initialization before it can begin
 * to interact. In addition, we will need to make all the stats
 * entries invalid by writing 0s.
 */
int
ifc_ft_init(struct n3k_mgmt_hw* hw)
{
	int rc;
	uint32_t init_status = 0, max_retries = 10;
	MKS_FLOW_TRACKER_FT_COMMAND_t ft_cmd;

	n3k_mgmt_lock(hw);

	init_status = csr_read(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE,
		MKS_FLOW_TRACKER_FT_STAT);
	while (init_status == 0 && max_retries--) {

		rte_delay_ms(100); // Busy wait 100ms
		init_status = csr_read(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE,
			MKS_FLOW_TRACKER_FT_STAT);

		N3K_MGMT_LOG(DEBUG, "init status:0x%x", init_status);
	}

	if (init_status == 0 && max_retries == 0) {
		N3K_MGMT_LOG(ERR, "Quitting! Flow Tracker module not ready!!!");
		return -1;
	}

	N3K_MGMT_LOG(INFO, "version:0x%x init_status:0x%x",
		csr_read(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE,
			MKS_FLOW_TRACKER_FT_VERSION),
		init_status);

	N3K_MGMT_LOG(DEBUG, "Flushing stats block.");

	/*
	 * Ideally we don't need to verify the busy bit but it is added
	 * just as a safely check.
	 */
	rc = ifc_gen_cmd_check(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE,
		MKS_FLOW_TRACKER_FT_COMMAND,
		MKS_FLOW_TRACKER_FT_COMMAND_BUSY_SHIFT,
		10);
	if (rc) {
		N3K_MGMT_LOG(ERR, "failed to verify busy bit of ft %d", rc);
		return rc;
	}

	ifc_ft_write_cmd(hw, IFC_FT_OP_FLUSH, 0);

	n3k_mgmt_unlock(hw);

	ft_cmd.val = csr_read(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE,
				MKS_FLOW_TRACKER_FT_COMMAND);
	while (ft_cmd.field.busy == 1) {
		/* FT COMMAND Register busy performing previous cmd. */
		rte_delay_ms(100);
		ft_cmd.val = csr_read(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE,
				MKS_FLOW_TRACKER_FT_COMMAND);
	}

	n3k_mgmt_lock(hw);

	/*
	 * Program the CT transition tables (TCP and nonTCP).
	 */
	ifc_ct_pgm_state_xtion_table(hw);
	ifc_ct_pgm_nontcp_xtion_table(hw);

	n3k_mgmt_unlock(hw);

	return 0;
}

/*
 * Notify Nios of the beginning of the init sequence.
 */
void
ifc_notify_init_begin(struct n3k_mgmt_hw* hw)
{
	uint32_t val;

	val = csr_read(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE,
			       MKS_FLOW_TRACKER_FT_SCRATCH);
	N3K_MGMT_LOG(DEBUG, "val:0x%x", val);
	val = (val & ~( 1 << IFC_CT_INIT_STATUS_BIT ));
	N3K_MGMT_LOG(DEBUG, "modified val:0x%x", val);

	csr_write(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE,
			  MKS_FLOW_TRACKER_FT_SCRATCH, val);

	return;
}

/*
 * Notify Nios of the end of the init sequence.
 */
void
ifc_notify_init_done(struct n3k_mgmt_hw* hw)
{
	uint32_t val;

	val = csr_read(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE,
			       MKS_FLOW_TRACKER_FT_SCRATCH);
	N3K_MGMT_LOG(DEBUG, "val:0x%x", val);
	val = (val | ( 1 << IFC_CT_INIT_STATUS_BIT ));
	N3K_MGMT_LOG(DEBUG, "modified val:0x%x", val);

	csr_write(hw, MKS_CHIP_MKS_FLOW_TRACKER_BASE,
			  MKS_FLOW_TRACKER_FT_SCRATCH, val);
	return;
}

/*
 * Enable Chicken Bit for Vxlan source port masking.
 *
 * Vxlan is a uni-directional protocol.
 * Meaning, one can’t get the response flow 5 tuple from request 5 tuple.
 * This is a problem for connection tracking, as we pre-populate response flow, when we setup request flow.
 * So, we need a change in parser.  For vxlan, we need the source port to be ignored/wildcarded.
 */
void ifc_msk_l4_src0(struct n3k_mgmt_hw* hw)
{
	uint32_t val;

	val = csr_read(hw, MKS_CHIP_MKS_PARS_BASE, MKS_PARS_PARS_MSKKEY_CFG);
	N3K_MGMT_LOG(DEBUG, "val:0x%x", val);

	val = val | (1 << MKS_PARS_PARS_MSKKEY_CFG_MSK_L4_SRC0_SHIFT);
	N3K_MGMT_LOG(DEBUG, "modified val:0x%x", val);
	csr_write(hw, MKS_CHIP_MKS_PARS_BASE, MKS_PARS_PARS_MSKKEY_CFG, val);

	N3K_MGMT_LOG(INFO, "Vxlan L4 SP Masking in on");

	return;
}
