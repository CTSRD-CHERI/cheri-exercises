/*
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2022 Microsoft Corporation
 */
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int
main(int argc, char **argv)
{
	char *b1, *b2;

	assert(argc == 2);
	char *p;
	size_t sz = (size_t)strtoull(argv[1], &p, 0);

	assert(sz > 0);
	assert(sz <= 0x8000);

	b1 = malloc(sz);
	assert(b1 != NULL);

	b2 = malloc(sz);
	assert(b2 != NULL);

#ifdef __CHERI_PURE_CAPABILITY__
	printf("sz=%zx, CRRL(sz)=%zx\n", sz,
	    __builtin_cheri_round_representable_length(sz));
	printf("b1=%#p b2=%#p diff=%tx\n", b1, b2, b2 - b1);
#else
	printf("b1=%p b2=%p diff=%tx\n", b1, b2, b2 - b1);
#endif

	/*
	 * The default CheriBSD malloc uses "size classes" for allocations.
	 * Check that we've landed "nearby".
	 */
	assert((ptraddr_t)(b1 + sz) <= (ptraddr_t)b2); 
	assert((ptraddr_t)(b1 + sz + sz/2) > (ptraddr_t)b2); 

	memset(b2, 'B', sz);

	printf("Overflowing by 1\n");
	memset(b1, 'A', sz + 1);

	printf("b2 begins: %.4s\n", b2);


	/* And now let's definitely make trouble */
	const size_t oversz = b2 - b1 + 2 - sz;
	printf("Overflowing by %zx\n", oversz);
	memset(b1 + sz, 'A', oversz);

	printf("b2 begins: %.4s\n", b2);
}
