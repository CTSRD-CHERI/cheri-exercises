# Compile and run RISC-V and CHERI-RISC-V programs

This exercise steps a participant through getting up and running with
code compilation and execution for RISC-V and CHERI-RISC-V programs.

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
