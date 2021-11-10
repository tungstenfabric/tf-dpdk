/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_PARSE_LIST_H_
#define _PMD_NET_N3K_PARSE_LIST_H_

int n3k_parse_kvargs_list(const char *str, void *args,
	int (*callback)(const char*, void*));

#endif /* _PMD_NET_N3K_PARSE_LIST_H_ */
