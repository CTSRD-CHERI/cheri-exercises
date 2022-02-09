/*
 * SPDX-License-Identifier: BSD-2-Clause-DARPA-SSITH-ECATS-HR0011-18-C-0016
 * Copyright (c) 2020 SRI International
 */
#include <stdio.h>
#include <stddef.h>

int
main(void)
{
	printf("size of pointer: %zu\n", sizeof(void *));
	printf("size of address: %zu\n", sizeof(ptraddr_t));

	return (0);
}
