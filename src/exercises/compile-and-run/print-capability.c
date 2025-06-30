/*
 * SPDX-License-Identifier: BSD-2-Clause-DARPA-SSITH-ECATS-HR0011-18-C-0016
 * Copyright (c) 2020 SRI International
 */
#include <printf.h>

void
init(void)
{
	int i;
	char *c;
	void *cap_to_int = &i;
	void *cap_to_cap = &c;

	printf("cap to int length: %lu\n", __builtin_cheri_length_get(cap_to_int));
	printf("cap to cap length: %lu\n", __builtin_cheri_length_get(cap_to_cap));
}

void notified(void){}
