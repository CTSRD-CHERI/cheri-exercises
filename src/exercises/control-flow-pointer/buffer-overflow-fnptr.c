/*
 * SPDX-License-Identifier: BSD-2-Clause-DARPA-SSITH-ECATS-HR0011-18-C-0016
 * Copyright (c) 2020 SRI International
 */
#include <stdint.h>
#include <stdio.h>

struct buf {
	size_t length;
	vaddr_t buffer[31];
	size_t (*callback)(struct buf *);
};

size_t
chicken_egg() {
	printf("Don't count your chickens before they hatch!\n");
	return 0;
}

void
fill_buf(struct buf *bp)
{
	bp->length = sizeof(bp->buffer)/sizeof(*bp->buffer);
	for (size_t i = 0; i <= bp->length; i++)
		bp->buffer[i] = (vaddr_t)chicken_egg;
}

size_t
count_eggs(struct buf *bp)
{
	int eggs = 0;

	for (size_t i = 0; i < bp->length; i++)
		eggs += bp->buffer[i] == (vaddr_t)chicken_egg ? 1 : 0;
	return eggs;
}

struct buf b = {.callback = count_eggs};

int
main(void)
{
	fill_buf(&b);

	printf("Number of eggs in b.buffer %zu\n", b.callback(&b));

	return 0;
}

#include "asserts.inc"
