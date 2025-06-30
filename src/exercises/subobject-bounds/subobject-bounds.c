/*
 * SPDX-License-Identifier: BSD-2-Clause-DARPA-SSITH-ECATS-HR0011-18-C-0016
 * Copyright (c) 2020 SRI International
 */
#include <printf.h>

struct buf {
	char buffer[128];
	int i;
} b;

#pragma weak fill_buf
void
fill_buf(char *buf, size_t len)
{
	for (size_t i = 0; i <= len; i++)
		buf[i] = 'b';
}

void
init(void)
{
	b.i = 'c';
	printf("b.i = %c\n", b.i);

	fill_buf(b.buffer, sizeof(b.buffer));

	printf("b.i = %c\n", b.i);
}

void notified(void){}

#include "asserts.inc"
