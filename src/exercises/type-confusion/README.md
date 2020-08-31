# Exercise integer-pointer type confusion bug

This exercise uses a union permitting confusion between integer types and pointer types.
1. Compiler `union-int-ptr.c` with a RISC-V target and binary name of `union-int-ptr-riscv`, and with a CHERI-RISC-V target and binary name `union-int-ptr-cheri`.

**union-int-ptr.c**
```C
{{#include union-int-ptr.c}}
```
2. Run the RISC-V program. What is the result?
3. Run the CHERI-RISC-V program. What is the result? Run under `gdb` and explain why the program crashes in the second `printf`.
