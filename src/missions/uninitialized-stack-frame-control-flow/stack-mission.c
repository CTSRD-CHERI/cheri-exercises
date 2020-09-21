/*
 * SPDX-License-Identifier: BSD-2-Clause-DARPA-SSITH-ECATS-HR0011-18-C-0016
 * Copyright (c) 2020 SRI International
 */

#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void
success(void)
{
	fprintf(stderr, "Exploit successful, yum!\n");
	exit(42);
}

void
no_cookies(void)
{
	fprintf(stderr, "No cookies??\n");
	exit(1);
}

#pragma weak init_pointer
void
init_pointer(void *p)
{
}

static void __attribute__((noinline))
init_cookie_pointer(void)
{
	void *pointers[12];
	void (* volatile cookie_fn)(void);

	for (size_t i = 0; i < sizeof(pointers) / sizeof(pointers[0]); i++)
		init_pointer(&pointers[i]);
	cookie_fn = no_cookies;
}

static void __attribute__((noinline))
get_cookies(void)
{
	alignas(void *) char cookies[sizeof(void *) * 32];
	char *cookiep;
	int ch, cookie;

	printf("Cookie monster is hungry, provide some cookies!\n");
	printf("'=' skips the next %zu bytes\n", sizeof(void *));
	printf("'-' skips to the next character\n");
	printf("XX as two hex digits stores a single cookie\n");
	printf("> ");

	cookiep = cookies;
	for (;;) {
		ch = getchar();

		if (ch == '\n' || ch == EOF)
			break;

		if (isspace(ch))
			continue;

		if (ch == '-') {
			cookiep++;
			continue;
		}

		if (ch == '=') {
			cookiep += sizeof(void *);
			continue;
		}

		if (isxdigit(ch)) {
			cookie = digittoint(ch) << 4;
			ch = getchar();
			if (ch == EOF)
				errx(1, "Half-eaten cookie, yuck!");
			if (!isxdigit(ch))
				errx(1, "Malformed cookie");
			cookie |= digittoint(ch);
			*cookiep++ = cookie;
			continue;
		}

		errx(1, "Malformed cookie");
	}
}

static void __attribute__((noinline))
eat_cookies(void)
{
	void *pointers[12];
	void (* volatile cookie_fn)(void);

	for (size_t i = 0; i < sizeof(pointers) / sizeof(pointers[0]); i++)
		init_pointer(&pointers[i]);
	cookie_fn();
}

int
main(void)
{
	init_cookie_pointer();
	get_cookies();
	eat_cookies();
	return (0);
}
