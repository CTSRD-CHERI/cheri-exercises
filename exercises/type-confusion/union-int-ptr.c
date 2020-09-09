/*
 * SPDX-License-Identifier: BSD-2-Clause-DARPA-SSITH-ECATS-HR0011-18-C-0016
 * Copyright (c) 2020 SRI International
 */
#include <stdio.h>

const char hello[] = "Hello World!";

union long_ptr {
	long l;
	const char *ptr;
} lp = { .ptr = hello };

void
inc_long_ptr(union long_ptr *lpp)
{
	lpp->l++;
}

int
main(void)
{
	printf("lp.ptr %s\n", lp.ptr);
	inc_long_ptr(&lp);
	printf("lp.ptr %s\n", lp.ptr);

	return 0;
}
