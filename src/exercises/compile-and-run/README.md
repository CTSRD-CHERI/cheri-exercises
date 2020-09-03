# Compile and run RISC-V and CHERI-RISC-V programs

In this exercise you will compile and run a couple simple
programs for RISC-V and CHERI-RISC-V. The objectives are:
- To ensure that the participant can compile programs for RISC-V and CHERI-RISC-V, transfer them to a CHERI-RISC-V system, and run them.
- To observe that on CHERI-RISC-V pointers have bounds derived from C-language types.

The first test program is written in conventional C, and can be compiled
to RISC-V or CHERI-RISC-V targets:

1. Compile `print-pointer.c` with a RISC-V target and a binary name of
`print-pointer-riscv`.

**print-pointer.c:**
```C
{{#include print-pointer.c}}
```
2. Run the binary.
3. Compile `print-pointer.c` with a CHERI-RISC-V target and a binary name
   of `print-pointer-cheri`.
4. Run the binary: it should print a pointer size of `16` and address size
   of `8`.

The second test program is written in CHERI C:

5. Compile print-capability.c with a CHERI-RISC-V target and a binary name
   of `print-capability`.
```C
{{#include print-capability.c}}
```
6. Run the binary.
7. Modify `print-capability.c` to point the capability at a pointer instead
   of an integer, and recompile.
8. Run the binary.
