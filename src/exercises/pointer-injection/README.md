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
2. Run the two binaries, which both send long integers over IPC, and print
   the sent and received values.
3. Modify the program source to create `ptr-over-pipe.c` which sends
   pointers instead, and then to dereference the received copy of the
   pointer (e.g. send a pointer to a string and print it).
4. Compile and run as `ptr-over-pipe-riscv` and `ptr-over-pipe-cheri`.
   What happens?
5. Why does dereferencing the received pointer in a CHERI binary fail?
