/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#include "n3k_parse_list.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define N3K_LIST_DELIM ","

int n3k_parse_kvargs_list(const char *str, void *args,
	int (*callback)(const char*, void*))
{
	int ret = 0;
	char *dup_str;
	char *ptr;

	if (strlen(str) == 0) {
		// N3K_MGMT_LOG(DEFAULT, ERR, "Argument is not provided");
		ret = -EINVAL;
		return ret;
	}

	if (*str != '[' || str[strlen(str) - 1] != ']') {
		// N3K_MGMT_LOG(DEFAULT, ERR, "Argument %s is not a list", str);
		ret = -EINVAL;
		return ret;
	}

	str++;

	dup_str = strndup(str, strlen(str) - 1);
	if (dup_str == NULL) {
		// N3K_MGMT_LOG(DEFAULT, ERR, "Not enough memory");
		ret = -ENOMEM;
		return ret;
	}
	ptr = strtok(dup_str, N3K_LIST_DELIM);

	while (ptr != NULL) {
		ret = callback(ptr, args);
		if (ret)
			goto free_dup_str;
		ptr = strtok(NULL, N3K_LIST_DELIM);
	}

free_dup_str:
	free(dup_str);
	return ret;
}
