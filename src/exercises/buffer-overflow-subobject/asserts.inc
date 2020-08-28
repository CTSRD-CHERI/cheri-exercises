#include <stddef.h>

_Static_assert(sizeof(b.buffer) == offsetof(struct buf, i),
    "There must be no padding in struct buf between buffer and i members");
