# Demonstrate CHERI Tag Protection

This exercise demonstrates CHERI's *capability provenance tags*, in particular
by showing that capabilities and their constituent bytes are subtly different
things!

1. Compile `cheri-tags.c` for the baseline architecture to the binary
   `cheri-tags-baseline` and for the CHERI-aware architecture to
   `cheri-tags-cheri`.

2. Run both programs and observe the output.

3. Inspect the error thrown to the CHERI program, and the registers dump.

4. Examine the disassembly of the construction of `q`,
   ```
   uint8_t *q = (uint8_t*)(((uintptr_t)p.ptr) & ~0xFF);
   ```
   and the byte-wise mutation of `p.ptr` to construct `r`,
   ```
   p.bytes[0] = 0;
   uint8_t *r = p.ptr;
   ```
   in both baseline and CHERI-enabled programs.

   What stands out?

5. Given that `q` and `r` appear to have identical byte representation in
   memory, why does the CHERI version crash when dereferencing `r`?

## Source

**cheri-tags.c**
```C
{{#include cheri-tags.c}}
```

## Courseware

This exercise has [presentation materials](./cheri-tags.pptx) available.
