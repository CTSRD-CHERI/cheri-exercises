# Exploiting a buffer overflow to manipulate control flow

The objective of this mission is to demonstrate arbitrary code execution
through a control-flow attack, despite CHERI protections. You will attack three
different versions of the program:

1. A baseline RISC-V compilation, to establish that the vulnerability is
   exploitable without any CHERI protections.

2. A baseline CHERI-RISC-V compilation, offering strong spacial safety between
   heap allocations, including accounting for imprecision in the bounds of large
   capabilities.

3. A weakened CHERI-RISC-V compilation, reflecting what would occur if a memory
   allocator failed to pad allocations to account for capability bounds
   imprecision.

The success condition for an exploit, given attacker-provided input overflowing
a buffer, is to modify control flow in the program such that the `success`
function is executed.

1. Compile `buffer-overflow.c` and `btpalloc.c` together with a RISC-V target
   and exploit the binary to execute the `success` function.

**buffer-overflow.c**
```C
{{#include buffer-overflow.c}}
```
2. Recompile with a CHERI-RISC-V target, attempt to exploit the binary and, if
   it cannot be exploited, explain why.
3. Recompile with a CHERI-RISC-V target but this time adding
   `-DCHERI_NO_ALIGN_PAD`, attempt to exploit the binary and, if it cannot be
   exploited, explain why.

**btpalloc.c**
```C
{{#include btpalloc.c}}
```

## Support code

**btpalloc.h**
```C
{{#include btpalloc.h}}
```

**main-asserts.inc**
```C
{{#include main-asserts.inc}}
```
