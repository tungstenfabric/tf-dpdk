# SPDX-License-Identifier: BSD-3-Clause
# Copyright(c) HCL TECHNOLOGIES LTD
# Submitted on behalf of a third-party: Intel Corporation, a
# Delaware corporation, having its principal place of business
# at 2200 Mission College Boulevard,
# Santa Clara, California 95052, USA

APP := n3kflow-dump
SRCS-y := main.c
SRCS-y += n3k_dump_cmdline.c
SRCS-y += n3k_dump_debug_regs.c
SRCS-y += n3k_dump_printers.c
SRCS-y += n3k_dump_utils.c

include $(RTE_SDK)/mk/rte.vars.mk

ifneq ($(shell which pkg-config > /dev/null && pkg-config --exists json-c && echo 1),)
	SRCS-y += n3k_dump_json.c
	LDLIBS += -ljson-c
else
	CFLAGS += -DN3K_DUMP_NO_JSON
endif


CFLAGS += -DALLOW_EXPERIMENTAL_API
CFLAGS += $(WERROR_FLAGS)

include $(RTE_SDK)/mk/rte.app.mk
