/*
 * SPDX-License-Identifier: BSD-2-Clause-DARPA-SSITH-ECATS-HR0011-18-C-0016
 * Copyright (c) 2020 Jessica Clarke
 */
#include <assert.h>
#include <stdint.h>
#ifdef __CHERI_PURE_CAPABILITY__
#include <cheriintrin.h>
#endif

static void
main_asserts(void *buf, void *fptr)
{
	uintptr_t ubuf = (uintptr_t)buf;
	uintptr_t ufptr = (uintptr_t)fptr;
#ifdef __CHERI_PURE_CAPABILITY__
	ptraddr_t ubuf_top;
#endif

#ifdef __CHERI_PURE_CAPABILITY__
	ubuf_top = cheri_base_get(ubuf) + cheri_length_get(ubuf);
#endif

#if defined(__CHERI_PURE_CAPABILITY__) && !defined(CHERI_NO_ALIGN_PAD)
	/*
	 * For the normal pure-capability case, `buf`'s allocation should be
	 * adequately padded to ensure precise capability bounds and `fptr`
	 * should be adjacent.
	 */
	assert(ubuf_top == ufptr);
#else
	/*
	 * Otherwise `fptr` should be 8 bytes (not 0 due to malloc's alignment
	 * requirements) after the end of `buf`.
	 */
	assert(ubuf + 25008 == ufptr);
#ifdef __CHERI_PURE_CAPABILITY__
	/*
	 * For pure-capability code this should result in the bounds of the
	 * large `buf` allocation including all of `fptr`.
	 */
	assert(ubuf_top >= ufptr + sizeof(void *));
#endif
#endif
}
