/*
 * SPDX-License-Identifier: BSD-2-Clause-DARPA-SSITH-ECATS-HR0011-18-C-0016
 * Copyright (c) 2020 SRI International
 */
#include <stdio.h>

int
main(void)
{
	printf("size of pointer: %zu\n", sizeof(void *));
	/* XXX: ideally we'd use ptraddr_t below */
	printf("size of address: %zu\n", sizeof(size_t));

	return (0);
}
