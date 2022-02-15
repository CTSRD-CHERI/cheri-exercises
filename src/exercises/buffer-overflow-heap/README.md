# Exercise heap overflows

This exercise demonstrates inter-object *heap* buffer overflows on baseline and
CHERI-enabled architectures, and asks you to characterize and fix the bug
detected by CHERI bounds enforcement.

1. Compile `buffer-overflow-heap.c` for the baseline architecture to the binary
   `buffer-overflow-heap-baseline` and for the CHERI-aware architecture to
   `buffer-overflow-heap-cheri`.

2. Run both versions, passing `0x20` as the (sole) command line argument.
   Observe that the CHERI version crashes with "In-address space security
   exception".

3. Run the CHERI version, again with `0x20`, under `gdb` and examine the crash
   in more detail.  Where must the bounds on the capability implementing `b1`
   have come from?

4. Run both programs again, but now with `0x1001` as the argument.
   Draw a picture of the portion of the heap containing (the end of) `b1` and
   (the start of) `b2`.  There are, in some sense, *two* different ends of `b1`
   in the baseline program and *three* in the CHERI program!  What are they and
   how do they arise?

5. While this program does crash on CHERI, again of a bounds violation, this
   happens slightly later than might be expected looking at the program's
   source.  In particular, this program actually commits *two* out of bounds
   stores using the `b1` capability.  Examine the output carefully and describe,
   merely in terms of the mechanism, without venturing philosophical, why the
   first does not trigger a trap.

6. Now consider the bigger picture.  Since CHERI uses *compressed* capability
   bounds, what additional steps must be taken, and by whom, to ensure spatial
   safety of a C program?

## Source Files

**buffer-overflow-heap.c**
```C
{{#include buffer-overflow-heap.c}}
```
