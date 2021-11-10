# SPDX-License-Identifier: BSD-3-Clause
# Copyright(c) HCL TECHNOLOGIES LTD
# Submitted on behalf of a third-party: Intel Corporation, a
# Delaware corporation, having its principal place of business
# at 2200 Mission College Boulevard,
# Santa Clara, California 95052, USA

APP := n3k-ut
SRCS-y := mock_dpdk.c n3k_rte_flow_helper.c main.c
SRCS-y += test_flow_create.c
SRCS-y += test_convert_ipv6_to_str.c

include $(RTE_SDK)/mk/rte.vars.mk

CFLAGS += -DALLOW_EXPERIMENTAL_API
CFLAGS += $(WERROR_FLAGS)
CFLAGS += -I$(RTE_SDK)/drivers/net/n3k/

LDLIBS := -lcmocka

# As we want to control stdout from the application, we need to localize librte_eal
# rte_log symbol, to prevent it from interfering with others
APP_BUILD_DIR := $(RTE_SDK_BIN)/build/app/n3k-ut
_LDLIBS-y += -L$(APP_BUILD_DIR)

create_logless_librte_eal:
	@objcopy --localize-symbol=rte_log $(RTE_SDK_BIN)/lib/librte_eal.a $(APP_BUILD_DIR)/librte_eal.a

$(APP): create_logless_librte_eal

include $(RTE_SDK)/mk/rte.app.mk
