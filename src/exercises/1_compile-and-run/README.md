# Compile and run RISC-V and CHERI-RISC-V programs

<!--- This exercise steps you through getting up and running with code compilation
#and execution for RISC-V and CHERI-RISC-V programs. --->

The test program is written in conventional C, and can be compiled
to RISC-V or CHERI-RISC-V targets:

1. Compile `print-pointer.c` for the baseline architecture with the `print-pointer-baseline` make target.

**print-pointer.c:**
```C
{{#include print-pointer.c}}
```
2. Run the binary.
3. Compile `print-pointer.c` for the CHERI-aware architecture with the `print-pointer-cheri` make target.
4. Run the binary: it should print a pointer size of `16` and address size
   of `8`.
5. Inspect both binaries with `llvm-readelf -h` and compare the `Flags` rows with each other.

The second test program is written in CHERI C:

5. Compile `include print-capability.c` for the CHERI-aware architecture with the `print-capability` make target.
```C
{{#include print-capability.c}}
```
6. Run the binary: note how the length of the capability depends on the size of
   the type it points to.
