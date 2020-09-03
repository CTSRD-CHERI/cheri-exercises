# Exercise a subobject buffer overflow

This exercise is similar to the prior exercise, but is for an
intra-object overflow. In our example, we consider an array within
another structure, overflowing onto an integer in the same allocation.

1. Compile `buffer-overflow-subobject.c` with a RISC-V target and binary
   name of `buffer-overflow-subobject-riscv`, and with a CHERI-RISC-V
   target and binary name of `buffer-overflow-subobject-cheri`.

**buffer-overflow-subobject.c**
```C
{{#include buffer-overflow-subobject.c}}
```
2. As in the prior exercise, run the binaries.
3. Explore why the CHERI binary didn't fail.
   Run `buffer-overflow-subobject-cheri` under `gdb` and examine the bounds
   of the `buffer` argument to `fill_buf()`.
   What do they correspond to?
4. Recompile the `buffer-overflow-subobject-cheri` binary with the compiler
   flags `-Xclang -cheri-bounds=subobject-safe`.
5. Run the program to demonstrate that the buffer overflow is now caught.
6. Run the program under `gdb` and examine the bounds again. What has changed?

## Support code

**asserts.inc**
```C
{{#include asserts.inc}}
```
