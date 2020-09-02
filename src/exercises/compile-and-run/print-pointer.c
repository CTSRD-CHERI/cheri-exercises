// SPDX-License-Identifier: BSD-2-Clause-DARPA-SSITH-ECATS
#include <stdio.h>

int
main(void)
{
	printf("size of pointer: %zu\n", sizeof(void *));
	/* XXX: ideally we'd use ptraddr_t below */
	printf("size of address: %zu\n", sizeof(size_t));

	return (0);
}
