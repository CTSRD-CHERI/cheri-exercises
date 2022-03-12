# Exercise an inter-global-object buffer overflow

This exercise uses two global objects to demonstrate an inter-object buffer
overflow on baseline and CHERI-enabled architectures, and asks you to
characterize and fix the bug detected by CHERI bounds enforcement.  It also asks
you to use GDB for debugging purposes.

This example uses two *global* objects (in `.data`) to demonstrate an overflow.
It is worth pondering how the bounds for pointers to globals come to be set!

1. Compile `buffer-overflow-global.c` for the baseline architecture to
   the binary `buffer-overflow-global-baseline` and for the CHERI-aware
   architecture to `buffer-overflow-global-cheri`.

   **For this exercise, add `-G0` to your
   compiler flags** (this ensures `c` is not placed in the small data section
   away from `buffer`).

2. Run both programs and observe the output.

3. Using GDB on the core dump (or run the CHERI program under `gdb`):
   Why has the CHERI program failed?

4. Modify `buffer-overflow-global.c` to increase the buffer size from 128 bytes
   to 1Mbyte + 1 byte.

5. Recompile and re-run `buffer-overflow-global-cheri`. Why does it no longer
   crash, even though the buffer overflow exists in the source code? Is
   the adjacent field still corrupted (i.e., has spatial safety been
   violated between allocations)?

6. Modify `buffer-overflow-global.c` to restore the original buffer size of 128
   bytes, and fix the bug by correcting accesses to the allocated array.

7. Recompile and run `buffer-overflow-global-cheri` to demonstrate that the
   program is now able to continue.

## Source Files

**buffer-overflow-global.c**
```C
{{#include buffer-overflow-global.c}}
```

### Support code

**main-asserts.inc**
```C
{{#include main-asserts.inc}}
```
