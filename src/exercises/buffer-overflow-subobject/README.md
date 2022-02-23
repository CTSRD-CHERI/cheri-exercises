# Exercise a subobject buffer overflow

In the CheriABI run-time environment, bounds are typically associated with
memory allocations rather than C types.
For example, if a heap memory allocation is made for 1024 bytes, and the
structure within it is 768 bytes, then the bounds associated with a pointer
will be for the allocation size rather than the structure size.

With subobject bounds, enforcement occurs on C-language objects within
allocations.
This exercise is similar to the prior exercise, but is for such an
intra-object overflow. In our example, we consider an array within
another structure, overflowing onto an integer in the same allocation.

1. Compile `buffer-overflow-subobject.c` with a baseline target and binary
   name of `buffer-overflow-subobject-baseline`, and with a CHERI-enabled
   target and binary name of `buffer-overflow-subobject-cheri`.

2. As in the prior exercise, run the binaries.

3. Explore why the CHERI binary didn't fail.
   Run `buffer-overflow-subobject-cheri` under `gdb` and examine the bounds
   of the `buffer` argument to `fill_buf()`.
   To what do they correspond?

4. Recompile the `buffer-overflow-subobject-cheri` binary with the compiler
   flags `-Xclang -cheri-bounds=subobject-safe`.

5. Run the program to demonstrate that the buffer overflow is now caught.

6. Run the program under `gdb` and examine the bounds again. What has changed?

## Source Files

**buffer-overflow-subobject.c**
```C
{{#include buffer-overflow-subobject.c}}
```

**asserts.inc**
```C
{{#include asserts.inc}}
```
