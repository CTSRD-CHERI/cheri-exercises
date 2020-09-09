/*
 * SPDX-License-Identifier: BSD-2-Clause-DARPA-SSITH-ECATS-HR0011-18-C-0016
 * Copyright (c) 2020 Jessica Clarke
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "btpalloc.h"

void
success(void)
{
	puts("Exploit successful!");
}

void
failure(void)
{
	puts("Exploit unsuccessful!");
}

static uint16_t
ipv4_checksum(uint16_t *buf, size_t words)
{
	uint16_t *p;
	uint_fast32_t sum;

	sum = 0;
	for (p = buf; words > 0; --words, ++p) {
		sum += *p;
		if (sum > 0xffff)
			sum -= 0xffff;
	}

	return (~sum & 0xffff);
}

#include "main-asserts.inc"

int
main(void)
{
	int ch;
	char *buf, *p;
	uint16_t sum;
	void (**fptr)(void);

	buf = btpmalloc(25000);
	fptr = btpmalloc(sizeof(*fptr));

	main_asserts(buf, fptr);

	*fptr = &failure;

	p = buf;
	while ((ch = getchar()) != EOF)
		*p++ = (char)ch;

	if ((uintptr_t)p & 1)
		*p++ = '\0';

	sum = ipv4_checksum((uint16_t *)buf, (p - buf) / 2);
	printf("Checksum: 0x%04x\n", sum);

	btpfree(buf);

	(**fptr)();

	btpfree(fptr);

	return (0);
}
