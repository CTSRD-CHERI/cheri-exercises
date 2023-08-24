/*
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2022 Microsoft Corporation
 */
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

#ifdef __CHERI_PURE_CAPABILITY__
#include <cheri/cherireg.h>
#define PRINTF_PTR "#p"
#else
#define PRINTF_PTR "p"
#endif

int
main(void)
{
	char *m, *p;
	int res;

	/* Get a page from the kernel and give it back */
	p = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE | MAP_ANON,
	    -1, 0);
	assert(p != MAP_FAILED);
	printf("Directly mapped page at p=%" PRINTF_PTR "\n", p);
#ifdef __CHERI_PURE_CAPABILITY__
	printf(" p.perms=0x%lx\n", __builtin_cheri_perms_get(p));
#endif
	res = madvise(p, 4096, MADV_FREE);
	assert(res == 0);

	p = mmap(p, 4096, PROT_READ|PROT_WRITE, MAP_FIXED | MAP_PRIVATE | MAP_ANON,
	    -1, 0);
	assert(p != MAP_FAILED);

	res = munmap(p, 4096);
	assert(res == 0);

	/* Get a pointer to a whole page of the heap*/
	m = malloc(8192);
	p = __builtin_align_up(m, 4096);
	printf("Punching hole in the heap at p=%" PRINTF_PTR "\n", p);
#ifdef __CHERI_PURE_CAPABILITY__
	printf(" p.perms=0x%lx\n", __builtin_cheri_perms_get(p));
#endif

	char *q = mmap(p, 4096, PROT_READ|PROT_WRITE, MAP_FIXED | MAP_PRIVATE | MAP_ANON,
	    -1, 0);
	assert(q != MAP_FAILED);

	if (madvise(p, 4096, MADV_FREE) != 0) {
		printf("madvise failed: %s\n", strerror(errno));
	}

	if (munmap(p, 4096) != 0) {
		printf("munmap failed: %s\n", strerror(errno));
	}

	printf("Done\n");
}
