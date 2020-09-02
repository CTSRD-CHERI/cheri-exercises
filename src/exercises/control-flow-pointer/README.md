# Corrupt a control-flow pointer using a subobject buffer overflow

1. Compile `buffer-overflow-fnptr.c` with a RISC-V target and binary name of `buffer-overflow-fnptr-riscv`, and a CHERI-RISC-V target and binary name of `buffer-overflow-fnptr`. Do not enable compilation with subobject bounds protection when compiling with the CHERI-RISC-V target.

**buffer-overflow-fnptr.c**
```C
{{#include buffer-overflow-fnptr.c}}
```
2. Run the RISC-V program under GDB; why does it crash?
3. Run the CHERI-RISC-V program under GDB; why does it crash?

## Support code

**asserts.inc**
```C
{{#include asserts.inc}}
```