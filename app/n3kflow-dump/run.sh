# SPDX-License-Identifier: BSD-3-Clause
# Copyright(c) HCL TECHNOLOGIES LTD
# Submitted on behalf of a third-party: Intel Corporation, a
# Delaware corporation, having its principal place of business
# at 2200 Mission College Boulevard,
# Santa Clara, California 95052, USA

#!/bin/bash

set -x
# TODO(n3k): fix
function main()
{
    if [[ $# -gt 0 ]]; then
        sudo ./app/n3kflow-dump \
            --proc-type secondary \
            --log-level eal:8 \
            --log-level n3kflow_dump:8 \
            --no-pci \
            -- \
            "$@"
    else
        sudo ./app/n3kflow-dump \
            --proc-type secondary \
            --log-level eal:8 \
            --log-level n3kflow_dump:8 \
            --no-pci
