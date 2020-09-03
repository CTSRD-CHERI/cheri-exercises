/*
 * SPDX-License-Identifier: BSD-2-Clause-DARPA-SSITH-ECATS-HR0011-18-C-0016
 * Copyright (c) 2020 SRI International
 */
#include <stdio.h>

char buffer[128];
char c;

void
fill_buf(char *buf, size_t len)
{
	for (size_t i = 0; i <= len; i++)
		buf[i] = 'b';
}

#include "main-asserts.inc"

int
main(void)
{
	(void)buffer;
	main_asserts();

	c = 'c';
	printf("c = %c\n", c);

	fill_buf(buffer, sizeof(buffer));

	printf("c = %c\n", c);

	return 0;
}
