/*
 * SPDX-License-Identifier: BSD-2-Clause-DARPA-SSITH-ECATS-HR0011-18-C-0016
 * Copyright (c) 2020 Jessica Clarke
 */
#include "btpalloc.h"

#include <assert.h>
#include <stddef.h>

#include <sys/mman.h>

#ifdef __CHERI_PURE_CAPABILITY__
#include <cheriintrin.h>
#endif

static void *btpmem;
static size_t btpsize;

static void
btpinit(void)
{
	btpsize = 0x100000;
	btpmem = mmap(NULL, btpsize, PROT_READ | PROT_WRITE,
	    MAP_PRIVATE | MAP_ANON, -1, 0);
	assert(btpmem != MAP_FAILED);
}

void *
btpmalloc(size_t size)
{
	void *alloc;
	size_t allocsize;

	if (btpmem == NULL)
		btpinit();

	alloc = btpmem;
	/* RISC-V ABIs require 16-byte alignment */
	allocsize = __builtin_align_up(size, 16);

#if defined(__CHERI_PURE_CAPABILITY__) && !defined(CHERI_NO_ALIGN_PAD)
	allocsize = cheri_representable_length(allocsize);
	alloc = __builtin_align_up(alloc,
	    ~cheri_representable_alignment_mask(allocsize) + 1);
	allocsize += (char *)alloc - (char *)btpmem;
#endif

	if (allocsize > btpsize)
		return (NULL);

	btpmem = (char *)btpmem + allocsize;
	btpsize -= allocsize;
#ifdef __CHERI_PURE_CAPABILITY__
	alloc = cheri_bounds_set(alloc, size);
#endif
	return (alloc);
}

void
btpfree(void *ptr)
{
	(void)ptr;
}
