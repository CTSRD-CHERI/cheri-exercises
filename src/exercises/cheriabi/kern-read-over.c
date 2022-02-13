/*
 * SPDX-License-Identifier: BSD-2-Clause-DARPA-SSITH-ECATS-HR0011-18-C-0016
 * Copyright (c) 2020 SRI International
 * Copyright (c) 2022 Microsoft Corporation
 */
#include <assert.h>
#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define bsz 16

int
main(void)
{
	int fds[2];
	pid_t pid;

	if (pipe(fds) == -1)
		err(1, "pipe");
	if ((pid = fork()) == -1)
		err(1, "fork");
	if (pid == 0) {
		char out[2*bsz];
		for (size_t i = 0; i < sizeof(out); i++) {
			out[i] = 0x10 + i;
		}

		if (write(fds[0], out, sizeof(out)) != sizeof(out)) {
			err(1, "write");
		}
		printf("Write OK\n");
	} else {
		int res;
		char upper[bsz] = { 0 };
		char lower[bsz] = { 0 };

		printf("lower=%p upper=%p\n", lower, upper);
		assert((ptraddr_t)upper == (ptraddr_t)&lower[sizeof(lower)]);

		waitpid(pid, NULL, 0);

		res = read(fds[1], lower, sizeof(lower) + sizeof(upper));
		assert(res != 0);
		if (res > 0) {
			printf("Read 0x%x OK; lower[0]=0x%x upper[0]=0x%x\n",
			    res, lower[0], upper[0]);
		} else if (res < 0) {
			printf("Bad read (%s); lower[0]=0x%x upper[0]=0x%x\n",
			    strerror(errno), lower[0], upper[0]);
		}
	}

	return 0;
}
