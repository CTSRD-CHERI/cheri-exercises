# Compile and run RISC-V and CHERI-RISC-V programs

<!--- This exercise steps you through getting up and running with code compilation
#and execution for RISC-V and CHERI-RISC-V programs. --->

The test program is written in conventional C, and can be compiled
to RISC-V or CHERI-RISC-V targets:

1. Compile `print-pointer.c` with a RISC-V target and a binary name of
`riscv-print-pointer`.

**print-pointer.c:**
```C
{{#include print-pointer.c}}
```
2. Run the binary.
3. Compile `print-pointer.c` with a CHERI-RISC-V target and a binary name
   of `cheriabi-print-pointer`.
4. Run the binary: it should print a pointer size of `16` and address size
   of `8`.

The second test program is written in CHERI C:

5. Compile `print-capability.c` with a CHERI-RISC-V target and a binary name
   of `print-capability`.
```C
{{#include print-capability.c}}
```
6. Run the binary: note how the length of the capability depends on the size of
   the type it points to.
