# Compile and run baseline and CHERI-enabled programs

This exercise steps you through getting up and running with code compilation and
execution of baseline and CHERI-enabled programs.  We assume that you have
CheriBSD [running in your preferred emulation
environment](../../introduction/run.md).

For this exercise, we will use the contents of `src/exercises/compile-and-run`;
the salient files are also printed [below](#source-files).

The first test program is written in conventional C and can be compiled on both
baseline and CHERI-enabled targets:

1. Compile `print-pointer.c`, below, with either a *baseline* target and a
   binary name of `print-pointer-baseline`.  For example, use one of these,
   from the root directory of this book, to compile the baseline program:

   ```
   ./tools/ccc aarch64 -o ~/cheri/build/print-pointer-baseline ./src/exercises/compile-and-run/print-pointer.c
   ```

   ```
   ./tools/ccc riscv64 -o ~/cheri/build/print-pointer-baseline ./src/exercises/compile-and-run/print-pointer.c
   ```

   <!-- Do not tidy the blank lines.  I am so sorry -->
   <div class="ctsrd-frame ctsrd-standout">

   <span class="ctsrd-large">⚠</span> Recall that you may have to vary the `-o`
   target, especially if you are [cross-compiling with the CheriBSD Software
   Release](../../introduction/cross-ccc-release.md) within a Docker container.
   In that case, may we suggest something like
   ```
   ./tools/ccc riscv64 -o /build/print-pointer-baseline ./src/exercises/compile-and-run/print-pointer.c
   ```

   </div>

   The C type `ptraddr_t` in the above is likely unfamiliar.  It is provided by
   CHERI C environments and is an integral type whose size is size of an
   *address* rather than the size of a *pointer* (the more familiar `uintptr_t`)
   or the size of an allocatable object (`size_t`).

2. Get the binary somewhere your CHERI system can see it.  This may require

   - [mounting the build directory in qemu over SMB](../../introduction/run-qemu-smb.md)
   - [scp to the CheriBSD system](../../introduction/run-ssh.md)

   (Users of the [CheriBSD Software Release](https://cheri-dist.cl.cam.ac.uk/)
   are encouraged to use the former option.)

3. Run `print-pointer-baseline`.  It should reveal that both pointers and
   addresses are `8` bytes in size.

Now repeat this exercise while building for a CHERI-enabled target:

4. Compile `print-pointer.c` with a CHERI-enabled target (such as
   `riscv64-purecap` or `morello-purecap`) and a binary name of
   `print-pointer-cheri`.

5. Run the binary: it should print a pointer size of `16` and address size
   of `8`.

The second test program is written in CHERI C, and so must be built for, and run
on, CHERI-enabled targets:

6. Compile `print-capability.c` with a CHERI-RISC-V or Morello target and a
   binary name of `print-capability`.

7. Run the binary: note how the length of the capability depends on
   the size of the type it points to.  We defer investigation of *how* this
   comes to be to [a later exercise](../debug-and-disassemble).

<!-- Name is known above -->
## Source Files

**print-pointer.c:**
```C
{{#include print-pointer.c}}
```

**print-capability.c:**
```C
{{#include print-capability.c}}
```
