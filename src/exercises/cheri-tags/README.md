# Demonstrate CHERI Tag Protection

This exercise demonstrates CHERI's *capability provenance tags*, in particular
by showing that capabilities and their constituent bytes are subtly different
things!

1. Compile `corrupt-pointer.c` for the baseline architecture to the binary
   `corrupt-pointer-baseline` and for the CHERI-aware architecture to
   `corrupt-pointer-cheri`.

2. Run both programs and observe the output.

3. Use `gdb` to inspect the `SIGPROT` thrown to the CHERI program.

   Print out the pseudoregister `$_siginfo`.  `si_signo` `34` is `SIGPROT`, a
   new signal introduced for conveying CHERI traps to user programs.  The
   `si_code` values for `SIGPROT` signals are defined as the various
   `PROT_CHERI_*` values in `<sys/signal.h>` (which can be found in
   `/usr/include` in a CheriBSD system).

4. Examine the disassembly of the construction of `q`,
   ```
   uint8_t *q = (uint8_t*)(((uintptr_t)p.ptr) & ~0xFF) + 5;
   ```
   and the byte-wise mutation of `p.ptr` to construct `r`,
   ```
   p.bytes[0] = 5;
   uint8_t *r = p.ptr;
   ```
   in both baseline and CHERI-enabled programs.

   What stands out?

5. Given that `q` and `r` appear to have identical byte representation in
   memory, why does the CHERI version crash when dereferencing `q`?

## Source

**corrupt-pointer.c**
```C
{{#include corrupt-pointer.c}}
```

## Courseware

This exercise has [presentation materials](./cheri-tags.pptx) available.
