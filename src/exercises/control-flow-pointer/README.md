# Corrupt a control-flow pointer using a subobject buffer overflow

This exercise demonstrates how CHERI pointer integrity protection prevents
a function pointer overwritten with data due to a buffer overflow from being
used for further memory access.

1. Compile `control-flow-pointer.c` with a RISC-V target and binary name
   of `control-flow-pointer-riscv`, and a CHERI-RISC-V target and binary
   name of `control-flow-pointer-cheri`. Do not enable compilation with
   subobject bounds protection when compiling with the CHERI-RISC-V target.

**control-flow-pointer.c**
```C
{{#include control-flow-pointer.c}}
```
2. Run the RISC-V program under QEMU; why does it crash?
3. Run the CHERI-RISC-V program under QEMU; why does it crash?

## Support code

**asserts.inc**
```C
{{#include asserts.inc}}
```
