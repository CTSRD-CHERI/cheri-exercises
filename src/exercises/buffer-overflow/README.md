# Exercise an inter-object buffer overflow

This exercise demonstrates an inter-object buffer overflow on RISC-V and CHERI-RISC-V, and asks the participant to fix the bug detected by CHERI bounds enforcement. It also asks the participant to use GDB for debugging purposes.

1. Compile `buffer-overflow.c` with a RISC-V target and binary name of `buffer-overflow-riscv`, and with a CHERI-RISC-V target and binary name of `buffer-overflow-cheri`.  **When compiling, add `-G0` to the compiler flags.**

**buffer-overflow.c**
```C
{{#include buffer-overflow.c}}
```
2. Run both programs and observe the output.
3. Using GDB on the core dump: Why has the CHERI program failed?
4. Modify buffer-overflow.c to increase the buffer size from 128 bytes to 1Mbyte + 1 byte.
5. Recompile and re-run `buffer-overflow-cheri`. Why does it no longer crash, even though the buffer overflow exists in the source code? Is the adjacent field still corrupted (i.e., has spatial safety been violated between allocations)?
6. Modify `buffer-overflow.c` to restore the original buffer size of 128 bytes, and fix the bug by correcting accesses to the allocated array.
7. Recompile and run `buffer-overflow-cheri` to demonstrate that the program is now able to continue.

## Support code

**main-asserts.inc**
```C
{{#include main-asserts.inc}}
```
