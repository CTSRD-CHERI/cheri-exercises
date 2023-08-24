/*
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2022 Microsoft Corporation
 */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __CHERI_PURE_CAPABILITY__
#define PRINTF_PTR "#p"
#else
#define PRINTF_PTR "p"
#endif

int
main(void)
{
	char buf[0x1FF];

	volatile union {
		char *ptr;
		char bytes[sizeof(char*)];
	} p;

	for (size_t i = 0; i < sizeof(buf); i++) {
		buf[i] = i;
	}
	p.ptr = &buf[0x10F];

	printf("buf=%" PRINTF_PTR " &p=%" PRINTF_PTR "\n", buf, &p);
	printf("p.ptr=%" PRINTF_PTR " (0x%zx into buf) *p.ptr=%02x\n",
	    p.ptr, p.ptr - buf, *p.ptr);

	/* One way to align the address down */
	char *q = (char*)(((uintptr_t)p.ptr) & ~0xFF);
	printf("q=%" PRINTF_PTR " (0x%zx into buf)\n", q, q - buf);

	printf("*q=%02x\n", *q);

	/* Maybe another, assuming a little-endian machine. */
	p.bytes[0] = 0;
	char *r = p.ptr;

	printf("r=%" PRINTF_PTR " (0x%zx)\n", r, r - buf);
	printf("*r=%02x\n", *r);

	return 0;
}
