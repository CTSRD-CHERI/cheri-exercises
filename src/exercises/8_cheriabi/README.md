# CheriABI Showcase

This exercise demonstrates several aspects of CheriABI, which defines how
CheriBSD processes are constructed, how function calls are made, how a process
interacts with the kernel through system calls, and other such foundational
details.

## The Kernel Voluntarily Honors Capability Bounds

`kern-read-over.c` demonstrates a (potential) loss of spatial safety when
pointers are passed from userspace to the kernel.  The kernel, by convention,
has full access to all of userspace memory.  Even when CheriBSD is running
CheriABI programs, this is true: the kernel holds a capability with full RWX
access to all userspace addresses.  Therefore, the kernel can act as a
*confused deputy*, accessing memory with its legitimate authority but without
intent.

1. Compile `kern-read-over.c` for both the baseline architecture with the
   `kern-read-over-baseline` make target and the CHERI-enabled architecture
   with the `kern-read-over-cheri` target.

2. Run these programs and observe their outputs.

3. Focusing on the `read()` system call, what happens in the two versions of the
   program.  When, in particular, does it look like the CHERI version notices
   something is amiss?

4. If you have done the [inter-stack-object buffer overflow
   exercise](../3_buffer-overflow-stack), contrast the behaviors of the two
   CHERI-enabled programs.

## The Process Memory Map

In most UNIX programs, the rights to manipulate the virtual memory map are
*ambient*: any piece of code can change the virtual memory permissions
associated with a page, `munmap` pages, or even request a replacement mapping
("fixed `mmap`") almost anywhere in the address space.  This risks allowing
evasion of CHERI capabilities' protection properties, as CHERI capabilities are
interpreted in combination with the virtual memory map.

Therefore, the CheriBSD kernel avails itself of a *software permission* bit in
CHERI capabilities.  Such permissions are not architecturally interpreted but
are still subject to architectural protection (and so, for example, a zero
permission bit may not be set to one without simultaneously clearing the
capability tag).  In particular, CheriBSD defines `CHERI_PERM_SW_VMEM`, sets
this permission bit when returning pointers to *new* allocations of address
space, and requires that capabilities passed to address-space-manipulating
functions bear this permission.  Userspace components are free to clear this
permission when delegating access to address space.

1. Compile `perm-vmem.c` for both the baseline architecture with the 
   `perm-vmem-baseline` make target and for the CHERI-enabled architecture with 
   the `perm-vmem-cheri` target.

2. Run these programs and observe their outputs.  The `printf` format strings
   for capabilities, `%p` and `%#p`, elide some usually-excessive details, and
   `CHERI_PERM_SW_VMEM` is generally regarded as one such.  `gdb`'s
   pretty-printing, similarly.  However, we can programmatically extract the
   permissions field and display it.

3. Modify `perm-vmem.c` to verify that `madvise(MADV_FREE)` and
   `mmap(MAP_FIXED)` also are permitted for the capability returned directly
   from `mmap` but are not permitted for the heap-derived pointer.

## (Extra Credit!) Initial Process Construction

We have largely focused on program behavior *after* it has been loaded and is
running.  Let us look in a little more detail at some aspects of the initial
construction.  While modern ELF loading is well beyond the scope of this
document, and is perhaps best summarized as "here be dragons", we can
nevertheless take a quick glance at some interesting features of CheriABI
startup.

1. Compile `print-more.c` for both the baseline (`print-more-baseline`) and the
   CHERI-enabled (`print-more-cheri`) architectures.

2. Run both these programs and observe their outputs.  As might be predicted,
   the CHERI version reports a wide variety of capabilities to different parts
   of the address space.  Run both programs several times; what do you observe?

   Let us examine several interesting aspects of the reported capabilities.

3. Launch `gdb ./print-more-cheri` and have it start the program and stop before
   running any instructions, with `starti`.  Where do we find ourselves?

4. Use `info inferiors` to obtain the child process identifier (PID) and
   `!procstat vm NNN` (replacing `NNN` with the child PID) to show the initial
   address space arranged by the kernel.

   Which of these initial mappings are targeted by the values reported for
   `&rodata_const`, `&relro_ptr`, `&rw_ptr` and `printf` in step 2?  What are
   the permissions for these mappings?

5. Just because the page mappings exist, however, CHERI programs need to have
   capabilities installed to access them.  Here at the very beginning of a
   process's life, we are in a good position to see the *root capabilities*
   that the kernel makes available.  Use `info registers` to see the initial
   contents of the register file.

6. Let's begin our tour with `csp`, the capability stack pointer register.

   First, what may strike you as surprising (and why) about the stack pointer
   being replaced by a capability?

   Second, compare the address space map obtained above with the current `csp`
   value; what has the kernel arranged to "back" the region of address space
   within stack bounds?

   If you are familiar with [Stack Clash
   Vulnerabilities](https://blog.qualys.com/vulnerabilities-threat-research/2017/06/19/the-stack-clash),
   explain how the two aspects above work in tandem to mitigate this class of
   vulnerability.

   Third, contrast the relative order of `&argv[0]` and `&stack_local` as
   reported on the two different architectures in step 2 above.

7. Having access to the stack is all well and good, but surely there is more to
   a process than that.  At the beginning of a CheriABI process's life, the
   capability in `ca0` (the first "argument register") points to the "auxiliary
   vector", an array of `Elf_Auxinfo` structures constructed by the kernel.

   `gdb` can ask the kernel for, and display, the information in the auxiliary
   vector with `info auxv`.  However, the pretty-printer is not capability
   aware, so let's also directly spelunk the structure.  Use some `gdb`
   scripting to print out the auxiliary vector in its entirety:
   ```
   set $i = 0
   while(((Elf_Auxinfo *)$ca0)[$i].a_type != 0)
   p ((Elf_Auxinfo *)$ca0)[$i]
   set $i = $i + 1
   end
   ```
   Use the more human-friendly `info auxv` to interpret the `a_type` values.

   In addition to the `AT_ARGV` value we have already (indirectly) seen above,
   there are many other capabilities to nearby parts of the address space,
   including the initial environment vector (`AT_ENVV`) and the executable path
   (`AT_EXECPATH`).

   More usefully, however,

   - `AT_PHDR` supplies a read/write capability to the loaded executable.

   - `AT_ENTRY` supplies a read/execute capability to the loaded executable,
     pointed at its entrypoint.

   - `AT_BASE` supplies a full read/write/execute capability to the program's
     "interpreter" (dynamic loader).  The elevated permissions here allow the
     loader to (relatively) easily relocate *itself* early in execution.

   From which of these capabilities are the displayed values of `&rodata_const`,
   `&relro_ptr`, and `&rw_ptr` from step 2 sourced?  What permissions have been
   shed in the derivation?  How do these permissions differ from those of the
   underlying page mappings?

8. The displayed value for `printf` is tagged as being a `(sentry)`.
   Modify the program to attempt to display the result of computing either

   - `*(char *)(printf)` or
   - `(void*)((uintptr_t)printf + 1)`.

   Compile and run this modified version (or both).  What happens?

   Sentry (short for "Sealed Entry") capabilities are a special form of
   capabilities: they are *immutable* and *inert*, conveying to the bearer no
   authority to the target, until they become the program counter, at which
   point they are unsealed into being an ordinary capability.  Thus, we can
   neither read through nor mutate our handle to `printf`, yet we can jump to
   it.

   If you are familiar with [Return Oriented
   Programming](https://hovav.net/ucsd/dist/geometry.pdf) and Jump Oriented
   Programming,
   you may wish to consider the cumulative challenge added by CHERI's
   architectural provenance requirement combined with pervasive use of sentry
   capabilities for dynamically resolved symbols.

## Source

**kern-read-over.c**
```C
{{#include kern-read-over.c}}
```

**perm-vmem.c**
```C
{{#include perm-vmem.c}}
```

**print-more.c**
```C
{{#include print-more.c}}
```

## Courseware

This exercise has [presentation materials](./exercises/8_cheriabi/cheriabi.pptx) available.
