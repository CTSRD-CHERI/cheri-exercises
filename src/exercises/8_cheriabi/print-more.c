/*
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2022 Microsoft Corporation
 */
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __CHERI_PURE_CAPABILITY__
#define PRINTF_PTR "#p"
#else
#define PRINTF_PTR "p"
#endif

static const int rodata_const = 42;
static int (*const relro_ptr)(const char *, ...) = printf;
static int (*rw_ptr)(const char *, ...) = printf;

int
main(int argc, char **argv)
{
	int stack_local;

	printf("&argv[0]=%" PRINTF_PTR "\n", &argv[0]);
	printf(" argv[0]=%" PRINTF_PTR "\n",  argv[0]);
	printf("&stack_local=%" PRINTF_PTR "\n", &stack_local);

	printf("&rodata_const=%" PRINTF_PTR "\n", &rodata_const);

	printf("&relro_ptr=%" PRINTF_PTR "\n", &relro_ptr);
	printf("&rw_ptr=%" PRINTF_PTR "\n", &rw_ptr);

	printf("printf=%" PRINTF_PTR "\n", printf);
}
