/*
 * SPDX-License-Identifier: BSD-2-Clause-DARPA-SSITH-ECATS-HR0011-18-C-0016
 * Copyright (c) 2020 SRI International
 */
#include <assert.h>
#include <stddef.h>
#ifdef __CHERI_PURE_CAPABILITY__
#include <cheriintrin.h>
#endif

#ifndef nitems
#define	nitems(x)	(sizeof((x)) / sizeof((x)[0]))
#endif

static void
main_asserts(void)
{
	/*
	 * Ensure that overflowing `buffer` by 1 will hit `c`.
	 * In the pure-capabilty case, don't assert if the size of
	 * `buffer` requires padding.
	 */
	assert((ptraddr_t)&buffer[nitems(buffer)] == (ptraddr_t)&c
#ifdef __CHERI_PURE_CAPABILITY__
	    || sizeof(buffer) < cheri_representable_length(sizeof(buffer))
#endif
	    );
}
