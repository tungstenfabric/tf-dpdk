/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) HCL TECHNOLOGIES LTD
 * Submitted on behalf of a third-party: Intel Corporation, a
 * Delaware corporation, having its principal place of business
 * at 2200 Mission College Boulevard,
 * Santa Clara, California 95052, USA
 */

#ifndef _PMD_NET_N3K_TIME_UTILS_H_
#define _PMD_NET_N3K_TIME_UTILS_H_

#include <stdbool.h>
#include <time.h>

static inline void
n3k_timespec_diff(const struct timespec *a, const struct timespec *b,
	struct timespec *result)
{
	result->tv_sec  = a->tv_sec  - b->tv_sec;
	result->tv_nsec = a->tv_nsec - b->tv_nsec;
	if (result->tv_nsec < 0) {
		--result->tv_sec;
		result->tv_nsec += 1000000000L;
	}
}

static inline bool
n3k_is_timespec_lesser(const struct timespec *a, const struct timespec *b)
{
	return a->tv_sec == b->tv_sec ?
		a->tv_nsec < b->tv_nsec :
		a->tv_sec < b->tv_sec;
}

#endif /* _PMD_NET_N3K_TIME_UTILS_H_ */
