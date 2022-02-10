# Exercise an inter-stack-object buffer overflow

This exercise demonstrates an inter-object buffer overflow on baseline and
CHERI-enabled architectures, and asks you to characterize and fix the bug
detected by CHERI bounds enforcement.  It also asks you to use GDB for
debugging purposes.

By contrast to [the globals-based example](../buffer-overflow-globals), this
example uses two *stack* objects to demonstrate the overflow.  We will be able
to see the CHERI C compiler generate code to apply spatial bounds on the
capability used for the buffer pointer we pass around.

1. Compile `buffer-overflow-stack.c` for the baseline architecture to
   the binary `buffer-overflow-stack-baseline` and for the CHERI-aware
   architecture to `buffer-overflow-stack-cheri`.

2. Run both programs and observe their outputs.

3. Using GDB on the core dump (or run the CHERI program under `gdb`):
   Why has the CHERI program failed?

4. Compare and contrast the disassembly of the baseline and CHERI programs.
   In particular, focus on the `write_buf` function and `main`'s call to it
   and the information flow leading up to it.

## Source

**buffer-overflow-stack.c**
```C
{{#include buffer-overflow-stack.c}}
```
