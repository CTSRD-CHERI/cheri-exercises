# Compile and run baseline and CHERI-enabled programs

This exercise steps you through getting up and running with code compilation
and execution for baseline and CHERI-enabled programs.

The first test program is written in conventional C, and can be compiled
to RISC-V, ARMv8, CHERI-RISC-V or Morello targets:

1. Compile `print-pointer.c` with either a RISC-V target and a binary
name of `print-pointer-riscv` or an ARMv8 target and name of
`print-pointer-morello-aarch64`

**print-pointer.c:**
```C
{{#include print-pointer.c}}
```
2. Run the binary.
3. Compile `print-pointer.c` with a CHERI-RISC-V or Morello target and
   a binary name of `print-pointer-cheri`.
4. Run the binary: it should print a pointer size of `16` and address size
   of `8`.

The second test program is written in CHERI C:

5. Compile `print-capability.c` with a CHERI-RISC-V or Morello target and a binary name
   of `print-capability`.
```C
{{#include print-capability.c}}
```
6. Run the binary: note how the length of the capability depends on
   the size of the type it points to.
