# Explore Subobject Bounds

In CHERI C/C++, bounds are typically associated with
memory allocations rather than C types.
For example, if a heap memory allocation is made for 1024 bytes, and the
structure within it is 768 bytes, then the bounds associated with a pointer
will be for the allocation size rather than the structure size.

## Subobject Overflows

With subobject bounds, enforcement occurs on C-language objects within
allocations.
This exercise is similar to earlier buffer-overflow exercises, but is for such
an intra-object overflow. In our example, we consider an array within
another structure, overflowing onto an integer in the same allocation.

1. Compile `buffer-overflow-subobject.c` with a baseline target and binary
   name of `buffer-overflow-subobject-baseline`, and with a CHERI-enabled
   target and binary name of `buffer-overflow-subobject-cheri`.

2. As in the prior exercises, run the binaries.

3. Explore why the CHERI binary didn't fail.
   Run `buffer-overflow-subobject-cheri` under `gdb` and examine the bounds
   of the `buffer` argument to `fill_buf()`.
   To what do they correspond?

4. Recompile the `buffer-overflow-subobject-cheri` binary with the compiler
   flags `-Xclang -cheri-bounds=subobject-safe`.

5. Run the program to demonstrate that the buffer overflow is now caught.

6. Run the program under `gdb` and examine the bounds again. What has changed?

## Deliberately Using Larger Bounds

Operations like `&object->field` that move from super-object to sub-object are
very natural in C, and there is no similarly concise syntax for the reverse
operation.  Nevertheless, C programs occasionally do make use of `containerof`
constructs to do exactly that: derive a pointer to the superobject given a
pointer to a subobject within.

A common example is *intrusive* linked lists, as found, for example, in the BSD
`<sys/queue.h>`.  `subobject-list.c` is an extremely minimal example of such,
which we will use to explore the behavior of CHERI C here.

1. Compile `subobject-list.c` for your CHERI-enabled target to
   `subobject-list-cheri` and run it.

2. What is the length (limit - base) for capabilities to...
   - the sentinel node (`&l`)
   - a next pointer (`ile_next`) to a non-sentinel element
   - a previous-next pointer (`ile_prevnp`) to a non-sentinel element

3. Recompile this program, now with `-Xclang -cheri-bounds=subobject-safe`, and
   run the result.  What happens and why?

4. The CheriBSD system headers have been extended so that examples like this
   which use the `<sys/cdefs.h>` definition of `__containerof` (or things built
   atop that) will trip static assertions.  Try compiling again with `-Xclang
   -cheri-bounds=subobject-safe -DUSE_CDEFS_CONTAINEROF` and observe what the
   compiler tells you.

5. Make the suggested change, marking `struct ilist_elem` as `` and recompile
   once again (with the same flags as just above).  Run the resulting program
   and observe its output.  Which bounds have not been narrowed?  Which have?
   Why is that OK?

## Source Files

### Subobject Overflows

**buffer-overflow-subobject.c**
```C
{{#include buffer-overflow-subobject.c}}
```

**asserts.inc**
```C
{{#include asserts.inc}}
```

### Deliberately Using Larger Bounds

**subobject-list.c**
```C
{{#include subobject-list.c}}
```

## Courseware

This exercise has [presentation materials](./subobject-bounds.pptx) available.
