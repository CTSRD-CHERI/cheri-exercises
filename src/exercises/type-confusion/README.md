# Exercise integer-pointer type confusion bug

This exercise demonstrates how CHERI distinguishes between integer and pointer
types, preventing certain types of type confusion.
In this example, a union allows an integer value to be used as a pointer,
which cannot then be dereferenced.

1. Compile `type-confusion.c` with a RISC-V target and binary name of
   `type-confusion-riscv`, and with a CHERI-RISC-V target and binary name
   `type-confusion-cheri`.

**type-confusion.c**
```C
{{#include type-confusion.c}}
```
2. Run the RISC-V program. What is the result?
3. Run the CHERI-RISC-V program.  What is the result?
   Run under QEMU and `gdb` and explain why the program crashes in the second `printf`.
