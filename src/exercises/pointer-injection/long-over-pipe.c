/*
 * SPDX-License-Identifier: BSD-2-Clause-DARPA-SSITH-ECATS-HR0011-18-C-0016
 * Copyright (c) 2020 SRI International
 */
#include <err.h>
#include <stdio.h>
#include <unistd.h>

int
main(void)
{
	int fds[2];
	pid_t pid;
	long val;

	if (pipe(fds) == -1)
		err(1, "pipe");
	if ((pid = fork()) == -1)
		err(1, "fork");
	if (pid == 0) {
		val = 42;
		if (write(fds[0], &val, sizeof(val)) != sizeof(val))
			err(1, "write");
	} else {
		if (read(fds[1], &val, sizeof(val)) != sizeof(val))
			err(1, "read");
		printf("received %ld\n", val);
	}

	return 0;
}
