/*
 * SPDX-License-Identifier: BSD-2-Clause-DARPA-SSITH-ECATS-HR0011-18-C-0016
 * Copyright (c) 2020 SRI International
 */
#include <stddef.h>

_Static_assert(sizeof(b.buffer) == offsetof(struct buf, i),
    "There must be no padding in struct buf between buffer and i members");
