# Demonstrate pointer injection

This exercise demonstrates how CHERI's pointer provenance validity prevents
injected pointer values from being dereferenced.
In this example code, a pointer is injected via pipe IPC, and then
dereferenced.

1. Compile `long-over-pipe.c` with a RISC-V target and a binary name of
   `long-over-pipe-riscv`, and with a CHERI-RISC-V target and a binary
   name of `long-over-pipe-cheri`.
```C
{{#include long-over-pipe.c}}
```
2. Run the two binaries, which both send long integers over pipe IPC, and
   print the sent and received values.
3. Compile `ptr-over-pipe.c` with a RISC-V target and a binary name of
  `ptr-over-pipe-riscv`, and with a CHERI-RISC-V target and a binary name of
  `ptr-over-pipe-cheri`.
```C
{{#include ptr-over-pipe.c}}
```
4. Run the two binaries, which both send pointers over pipe IPC, and then
   dereference the received copy to print a string.
5. Why does dereferencing the received pointer in a CHERI binary fail?
