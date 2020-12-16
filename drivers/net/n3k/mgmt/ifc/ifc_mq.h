#ifndef _IFC_MQ_
#define _IFC_MQ_

#include "../n3k_mgmt_hw.h"

struct ifc_vq {
	u16 id;
	u16 next_queue;
	u8 pf;
	u8 vf;
	u8 size;
	u8 is_available : 1;
	u8 is_pf : 1;
	u8 is_ctrl : 1;
};

void
ifc_vq_pool_init(void);

int
ifc_set_vf_mq(struct n3k_mgmt_hw *hw, u8 pf, u8 vf, u8 queues);

int
ifc_set_pf_mq(struct n3k_mgmt_hw *hw, u8 pf, u8 queues);

void
ifc_get_pf_mq(u8 pf, u8 *queues);

void
ifc_get_vf_mq(u8 pf, u8 vf, u8 *queues);

#endif /*_IFC_MQ_*/
