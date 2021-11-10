# SPDX-License-Identifier: BSD-3-Clause
# Copyright(c) HCL TECHNOLOGIES LTD
# Submitted on behalf of a third-party: Intel Corporation, a
# Delaware corporation, having its principal place of business
# at 2200 Mission College Boulevard,
# Santa Clara, California 95052, USA


include $(RTE_SDK)/mk/rte.vars.mk

APP = n3k-flr-test-app

CFLAGS += $(WERROR_FLAGS)
CFLAGS += -I$(RTE_SDK)/drivers/net/n3k/

SRCS-y := main.c

include $(RTE_SDK)/mk/rte.app.mk
