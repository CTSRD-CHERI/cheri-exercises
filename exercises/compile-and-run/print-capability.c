/*
 * SPDX-License-Identifier: BSD-2-Clause-DARPA-SSITH-ECATS-HR0011-18-C-0016
 * Copyright (c) 2020 SRI International
 */
#include <stdio.h>
#include <cheriintrin.h>

int
main(void)
{
	int i;
	void *cap = &i;

	printf("cap length: %lu\n", cheri_length_get(cap));

	return (0);
}
