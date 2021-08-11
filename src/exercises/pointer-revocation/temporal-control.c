/*
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2020 Microsoft, Inc.
 */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Ensure we're being run on a temporal-safety-aware system */
#ifdef __CHERI_PURE_CAPABILITY__
#include <cheri/revoke.h>
__attribute__((used))
static void *check_cheri_revoke = cheri_revoke;

extern void malloc_revoke(void);
__attribute__((used))
static void *check_malloc_revoke = malloc_revoke;
#endif

static void
fn1(uintptr_t arg)
{
	fprintf(stderr, " First function: %#p\n", (void *)arg);
}

static void
fn2(uintptr_t arg)
{
	fprintf(stderr, " Second function: %#p\n", (void *)arg);
}

struct obj {
	char buf[32];
	/*
	 * The following are marked volatile to ensure the compiler doesn't
	 * constant propagate fn (making aliasing not work) and to ensure
	 * neither stores to them are optimised away entirely as dead due
	 * to calling free.
	 */
	void (* volatile fn)(uintptr_t);
	volatile uintptr_t arg;
};

int
main(void)
{
	struct obj * volatile obj1 = calloc(1, sizeof(*obj1));

	fprintf(stderr, "Installing function pointer in obj1 at %#p\n", obj1);
	obj1->fn = fn1;
	obj1->arg = (uintptr_t)obj1;

	free(obj1);

	fprintf(stderr, "Demonstrating use after free:\n");
	obj1->fn(obj1->arg);

#ifdef CAPREVOKE
	/* Force recycling the free queue now, but with a revocation pass */
	malloc_revoke();
#endif

	struct obj * volatile obj2 = malloc(sizeof(*obj2));
#ifdef CAPREVOKE
	assert(obj1 == obj2);
#endif

	fprintf(stderr, "Assigning function pointer through obj2 at %#p\n",
	    obj2);
	obj2->fn = fn2;

	fprintf(stderr, "Calling function pointer through obj1 (now %#p):\n",
	    obj1);
	obj1->fn(obj1->arg);

	return (0);
}
