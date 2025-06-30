/*
 * SPDX-License-Identifier: BSD-2-Clause-DARPA-SSITH-ECATS-HR0011-18-C-0016
 * Copyright (c) 2020 Jessica Clarke
 */
#include <stdint.h>
#include <printf.h>
#include <microkit.h>
#include <sel4/sel4.h>

#include "btpalloc.h"

#define SERIAL_CHANNEL 1

uintptr_t serial_to_client_vaddr;
uintptr_t client_to_serial_vaddr;

#define MOVE_CURSOR_UP "\033[5A"
#define CLEAR_TERMINAL_BELOW_CURSOR "\033[0J"
#define GREEN "\033[32;1;40m"
#define YELLOW "\033[39;103m"
#define DEFAULT_COLOUR "\033[0m"

void
success(void)
{
	printf("Exploit successful!");
}

void
failure(void)
{
	printf("Exploit unsuccessful!");
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

static volatile int empty = 1;

static char getchar() {
    microkit_ppcall(SERIAL_CHANNEL, microkit_msginfo_new(1, 0));
    return ((char *)serial_to_client_vaddr)[0];
}

void
init(void)
{
	int ch;
	char *buf, *p;
	uint16_t sum;
	void (**fptr)(void);

	buf = btpmalloc(25000);
	fptr = btpmalloc(sizeof(*fptr));

	*fptr = &failure;

	p = buf;
	while ((ch = getchar()) != -1) {
    if(ch == '\n' || ch == '\r')
        break;

		*p++ = (char)ch;
  }

	if ((uintptr_t)p & 1)
		*p++ = '\0';

	sum = ipv4_checksum((uint16_t *)buf, (p - buf) / 2);
	printf("Checksum: 0x%04x\n", sum);

	btpfree(buf);

	(**fptr)();

	btpfree(fptr);
}

void notified(microkit_channel channel) {
    switch (channel) {
        case SERIAL_CHANNEL: {
            char ch = ((char *)serial_to_client_vaddr)[0];
            microkit_dbg_putc(ch);
            empty = 0;
            break;
        }
    }
}
