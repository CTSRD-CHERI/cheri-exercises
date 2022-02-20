# Answers

## The Process Memory Map

2. Example output from a baseline architecture:
   ```
   Directly mapped page at p=0x84dc0000
   Punching hole in the heap at p=0x83b48000
   Done
   ```

   And from a CHERI-enabled architecture:
   ```
   Directly mapped page at p=0x40139000 [rwRW,0x40139000-0x4013a000]
    p.perms=0x7817d
   Punching hole in the heap at p=0x407d1000 [rwRW,0x407d1000-0x407d3000]
    p.perms=0x6817d
   munmap failed: Memory protection violation
   Done
   ```

3. This amounts to adding calls to `madvise(p, 4096, MADV_FREE)` and
   `mmap(p, 4096, PROT_READ|PROT_WRITE, MAP_FIXED|MAP_PRIVATE|MAP_ANON, -1, 0)`
   in the right places and verifying that the operations return appropriately.
   Additionally, you may wish to check `errno` in failure cases and the
   *contents* of memory after `mmap` to ensure that it has, indeed, behaved as
   expected in both cases.

## The Kernel as a Potentially Confused Deputy

2. Example output from a baseline architecture:
   ```
   Write OK
   lower=0x80922400 upper=0x80922410
   Read 0x20 OK; lower[0]=0x10 upper[0]=0x20
   ```

   And from a CHERI-enabled architecture:
   ```
   lower=0x3fffdfff28 upper=0x3fffdfff38
   Write OK
   Bad read (Bad address); lower[0]=0x10 upper[0]=0x0
   ```

3. On the baseline architecture, the kernel dutifully writes 0x20 bytes to the
   target address, regardless of the C object model.  On the CHERI architecture,
   the kernel acts *intentionally* with the capability provided by userspace and
   so encounters a trap when copying bytes out to userspace.  Because the kernel
   updates `lower[0]`, we can conjecture that the kernel is not performing
   explicit bounds checks but is rather operating under a trust-but-validate
   model, handling the architectural trap when it uses `copyout()` to copy bytes
   from its internal pipe buffer to the indicated userspace buffer.

4. Not all capability faults are fatal!  While the [inter-stack-object overflow
   exercise](../buffer-overflow-stack) let the program die of the `SIGPROT`
   thrown its way, here, the kernel maps the architectural trap to a *failure
   return* rather than a fatal signal.

## (Extra Credit!) Initial Process Construction

2. Example output from a baseline architecture:
   ```
   &argv[0]=0x806e0d08
    argv[0]=0x806e0fd0
   &stack_local=0x806e0c94
   &rodata_const=0x105c0
   &relro_ptr=0x105c8
   &rw_ptr=0x13e18
   printf=0x11c60
   ```

   And from a CHERI-enabled architecture:
   ```
   &argv[0]=0x3fbfdff8d0 [rwRW,0x3fbfdff8d0-0x3fbfdff8f0]
    argv[0]=0x3fbfdffe30 [rwRW,0x3fbfdffe30-0x3fbfdffe4a]
   &stack_local=0x3fffdfff6c [rwRW,0x3fffdfff6c-0x3fffdfff70]
   &rodata_const=0x1005c8 [rR,0x1005c8-0x1005cc]
   &relro_ptr=0x102f00 [rR,0x102f00-0x102f10]
   &rw_ptr=0x104070 [rwRW,0x104070-0x104080]
   printf=0x402608d8 [rxR,0x4013a000-0x40782000] (sentry)
   ```

   Running the baseline version multiple times should produce different output
   thanks to Address Space Layout Randomization (ASLR), a popular probabilistic
   countermeasure against pointer forgery.  Because CHERI offers deterministic
   protection against pointer forgery by its very nature, ASLR for CheriABI
   programs is turned off.

3. Example session:
   ```
   (gdb) starti
   Starting program: /buildroot/print-more-cheri 
   
   Program stopped.
   rtld_start () at /cheri/source/mainline/cheribsd/libexec/rtld-elf/riscv/rtld_start.S:62
   62      /cheri/source/mainline/cheribsd/libexec/rtld-elf/riscv/rtld_start.S: No such file or directory.
   ```

   We find ourselves nowhere within `print-more-cheri` but, rather, at the very
   beginning of the *dynamic loader* (`rtld`).

4. Example session for CHERI:
   ```
   (gdb) info inferiors
     Num  Description       Executable        
   * 1    process 1013      /buildroot/print-more-cheri 
   (gdb) !procstat vm 1013
     PID              START                END PRT    RES PRES REF SHD FLAG  TP PATH
    1013           0x100000           0x101000 r--R-    1    3   3   0 CN--- vn /buildroot/print-more-cheri
    1013           0x101000           0x102000 r-xR-    1    3   3   0 CN--- vn /buildroot/print-more-cheri
    1013           0x102000           0x104000 rw-RW    2    3   3   0 CN--- vn /buildroot/print-more-cheri
    1013           0x104000           0x105000 rw-RW    1    1   1   0 ----- df 
    1013         0x40104000         0x4010f000 r--R-   11  344  32   0 CN--- vn /libexec/ld-elf.so.1
    1013         0x4010f000         0x4012a000 r-xR-   27    0   1   0 C---- vn /libexec/ld-elf.so.1
    1013         0x4012a000         0x4012b000 rw-RW    1  344  32   0 CN--- vn /libexec/ld-elf.so.1
    1013         0x4012b000         0x4012d000 rw-RW    2  344  32   0 CN--- vn /libexec/ld-elf.so.1
    1013         0x4012d000         0x4012f000 rw-RW    1    1   1   0 ----- df 
    1013       0x3fbfd80000       0x3fbfe00000 rw-RW    1    1   1   0 ----- df 
    1013       0x3fbfe00000       0x3fffde0000 -----    0    0   0   0 G---- gd 
    1013       0x3fffde0000       0x3fffe00000 rw-RW    0    0   0   0 ---D- -- 
    1013       0x3ffffff000       0x4000000000 r-x--    1    1  13   0 ----- ph 
   ```

   | pointer | mapping | permissions |
   | ------- | ------- | ----------- |
   | `&argv[0]`     | `0x3fbfd80000` | `rw-RW` |
   | `argv`         | "              | "       |
   | `stack_local`  | `0x3fffde0000` | `rw-RW` |
   | `rodata_const` | `0x100000`     | `r--R-` |
   | `relro_ptr`    | `0x102000`     | `rw-RW` |
   | `rw_ptr`       | `0x104000`     | `rw-RW` |
   | `printf`       | not initially mapped | n/a |

5. Abridged output:
   ```
   cra            0xd117200009e18201000000004010f040       0x4010f040 <rtld_start> [rxR,0x40104000-0x4012f000] (sentry)
   csp            0xd17d000003ff2ffe0000003fffe00000       0x3fffe00000 [rwRW,0x3fbfe00000-0x3fffe00000]
   ca0            0xd17d00000785b9b40000003fbfdff9b0       0x3fbfdff9b0 [rwRW,0x3fbfdff9b0-0x3fbfdffe10]
   pcc            0x4010f040       0x4010f040 <rtld_start>
   ddc            0x0      0x0
   cap_valid      0x80800003       2155872259
   ```

6. In baseline programs, the stack is bounded only by operating system measures
   -- the kernel will refuse to grow what it considers to be "the stack" beyond
   some limit.  However, architecturally, there is no *a priori* limit to stack
   growth.  In CHERI, by contrast, the stack is accessed via a capability, which
   must be constructed up front.

   For the CHERI program, the kernel has backed the entirety of stack memory
   with page mappings:
   ```
    1013       0x3fbfe00000       0x3fffde0000 -----    0    0   0   0 G---- gd 
    1013       0x3fffde0000       0x3fffe00000 rw-RW    0    0   0   0 ---D- -- 
   ```
   The latter of these is marked as "growing down" (the `D` in the penultimate
   field) while the former is considered a "guard" mapping (the `G` flag and
   `gd` type), serving to prevent any other claimant to the address space.

   The initial bounds on the stack capability prevent half of Stack Clash: the
   stack capability cannot authorize access to a heap region, even if, say,
   indexing an on-stack array goes very far out of bounds.  The primordial guard
   entry serves to prevent the second half: the heap cannot grow into the stack,
   because the kernel will not use that address space to satisfy `mmap` requests
   (and, moreover, no capability held by userspace, including the one in `csp`,
   bears `CHERI_PERM_SW_VMEM`, so the stack or its guard cannot be torn down).

   Traditionally, `argv` and its contents (as well as the `environ`ment vector
   and indeed the entire auxiliary vector) is placed above the initial stack
   pointer, so `&argv[0]` is above `&stack_local`.  However, here we can see
   that CheriBSD chooses to locate all this initial data *below* the stack
   reservation, meaning that `&stack_local` is further up the address space than
   `&argv[0]`.  This allows the kernel to ensure that parts of this initial
   state are immutable or that there exists exactly one capability to parts of
   the structure (allowing for easier reasoning about capability flow in
   userspace); these would not be true if this initial data were also reachable
   through the stack capability.

7. Example session:
   ```
   (gdb) info auxv
   3    AT_PHDR              Program headers for program    0x100040
   4    AT_PHENT             Size of program header entry   56
   5    AT_PHNUM             Number of program headers      11
   6    AT_PAGESZ            System page size               4096
   8    AT_FLAGS             Flags                          0x0
   9    AT_ENTRY             Entry point of program         0x101a30
   7    AT_BASE              Base address of interpreter    0x40104000
   24   AT_EHDRFLAGS         ELF header e_flags             0x30005
   15   AT_EXECPATH          Executable path                0x3fbfdfffa0 "/mnt/tmp/print-more-cheri"
   18   AT_OSRELDATE         OSRELDATE                      1400051
   16   AT_CANARY            Canary for SSP                 0x3fbfdfff60
   17   AT_CANARYLEN         Length of the SSP canary       64
   19   AT_NCPUS             Number of CPUs                 1
   20   AT_PAGESIZES         Pagesizes                      0x3fbfdfff40
   21   AT_PAGESIZESLEN      Number of pagesizes            24
   22   AT_TIMEKEEP          Pointer to timehands           0x3ffffff020
   23   AT_STACKPROT         Initial stack protection       0x3
   25   AT_HWCAP             Machine-dependent CPU capability hints 0x112d
   27   AT_BSDFLAGS          ELF BSD flags                  0x0
   28   AT_ARGC              Argument count                 1
   29   AT_ARGV              Argument vector                0x3fbfdff880
   30   AT_ENVC              Environment count              16
   31   AT_ENVV              Environment vector             0x3fbfdff8a0
   32   AT_PS_STRINGS        Process strings                0x3fbfdfffc0
   0    AT_NULL              End of vector                  0x0
   (gdb)    set $i = 0
   (gdb)    while(((Elf_Auxinfo *)$ca0)[$i].a_type != 0)
    >   p ((Elf_Auxinfo *)$ca0)[$i]
    >   set $i = $i + 1
    >   end
   $1 = {a_type = 3, a_un = {a_val = 1048640, a_ptr = 0x100040 [rwRW,0x100000-0x104260], a_fcn = 0x100040 [rwRW,0x100000-0x104260]}}
   $2 = {a_type = 4, a_un = {a_val = 56, a_ptr = 0x38, a_fcn = 0x38}}
   $3 = {a_type = 5, a_un = {a_val = 11, a_ptr = 0xb, a_fcn = 0xb}}
   $4 = {a_type = 6, a_un = {a_val = 4096, a_ptr = 0x1000, a_fcn = 0x1000}}
   $5 = {a_type = 8, a_un = {a_val = 0, a_ptr = 0x0, a_fcn = 0x0}}
   $6 = {a_type = 9, a_un = {a_val = 1055280, a_ptr = 0x101a30 <_start> [rxR,0x100000-0x104260], a_fcn = 0x101a30 <_start> [rxR,0x100000-0x104260]}}
   $7 = {a_type = 7, a_un = {a_val = 1074806784, a_ptr = 0x40104000 [rwxRW,0x40104000-0x4012f000], a_fcn = 0x40104000 [rwxRW,0x40104000-0x4012f000]}}
   $8 = {a_type = 24, a_un = {a_val = 196613, a_ptr = 0x30005, a_fcn = 0x30005}}
   $9 = {a_type = 15, a_un = {a_val = 273802067872, a_ptr = 0x3fbfdfffa0 [rwRW,0x3fbfdfffa0-0x3fbfdfffba], a_fcn = 0x3fbfdfffa0 [rwRW,0x3fbfdfffa0-0x3fbfdfffba]}}
   $10 = {a_type = 18, a_un = {a_val = 1400051, a_ptr = 0x155cf3, a_fcn = 0x155cf3}}
   $11 = {a_type = 16, a_un = {a_val = 273802067808, a_ptr = 0x3fbfdfff60 [rwRW,0x3fbfdfff60-0x3fbfdfffa0], a_fcn = 0x3fbfdfff60 [rwRW,0x3fbfdfff60-0x3fbfdfffa0]}}
   $12 = {a_type = 17, a_un = {a_val = 64, a_ptr = 0x40, a_fcn = 0x40}}
   $13 = {a_type = 19, a_un = {a_val = 1, a_ptr = 0x1, a_fcn = 0x1}}
   $14 = {a_type = 20, a_un = {a_val = 273802067776, a_ptr = 0x3fbfdfff40 [rwRW,0x3fbfdfff40-0x3fbfdfff58], a_fcn = 0x3fbfdfff40 [rwRW,0x3fbfdfff40-0x3fbfdfff58]}}
   $15 = {a_type = 21, a_un = {a_val = 24, a_ptr = 0x18, a_fcn = 0x18}}
   $16 = {a_type = 22, a_un = {a_val = 274877902880, a_ptr = 0x3ffffff020 [rwRW,0x3ffffff020-0x3ffffff190], a_fcn = 0x3ffffff020 [rwRW,0x3ffffff020-0x3ffffff190]}}
   $17 = {a_type = 23, a_un = {a_val = 3, a_ptr = 0x3, a_fcn = 0x3}}
   $18 = {a_type = 25, a_un = {a_val = 4397, a_ptr = 0x112d, a_fcn = 0x112d}}
   $19 = {a_type = 27, a_un = {a_val = 0, a_ptr = 0x0, a_fcn = 0x0}}
   $20 = {a_type = 28, a_un = {a_val = 1, a_ptr = 0x1, a_fcn = 0x1}}
   $21 = {a_type = 29, a_un = {a_val = 273802066048, a_ptr = 0x3fbfdff880 [rwRW,0x3fbfdff880-0x3fbfdff8a0], a_fcn = 0x3fbfdff880 [rwRW,0x3fbfdff880-0x3fbfdff8a0]}}
   $22 = {a_type = 30, a_un = {a_val = 16, a_ptr = 0x10, a_fcn = 0x10}}
   $23 = {a_type = 31, a_un = {a_val = 273802066080, a_ptr = 0x3fbfdff8a0 [rwRW,0x3fbfdff8a0-0x3fbfdff9b0], a_fcn = 0x3fbfdff8a0 [rwRW,0x3fbfdff8a0-0x3fbfdff9b0]}}
   $24 = {a_type = 32, a_un = {a_val = 273802067904, a_ptr = 0x3fbfdfffc0 [rwRW,0x3fbfdfffc0-0x3fbfe00000], a_fcn = 0x3fbfdfffc0 [rwRW,0x3fbfdfffc0-0x3fbfe00000]}}
   ```

   `rodata_const` and `relro_ptr` could each be derived from either `AT_PHDR`
   (shedding write permission) or `AT_BASE` (shedding execute); despite that the
   pages are mapped read-write, the capability permissions will enforce that
   they cannot be used to modify the values here.  If these are the only
   (non-TCB) capabilities to those locations, then the values must, indeed, be
   constant (outside bugs in the TCB).

   `rw_ptr` must, on the other hand, have come from `AT_PHDR`.

8. In either case, the program will trap and abort (`In-address space security
   exception`).

   Sentries are believed to complicate would-be ROP or JOP attacks without
   excessively complicating the architecture or system software.  Without, it
   would be possible to locate and invoke gadgets within resolvable functions in
   any loaded object, as the function must have execute permission to its entire
   body, and so the capability used to reference the function would need to
   (transitively) confer such rights.  Sentries let us refer to some code
   without the rights to jump to any part of it except the intended entry point.
   At the time of this writing, CHERI-RISC-V always, and Morello optionally,
   automatically constructs sentry capabilities when executing linked control
   transfers.

   At present, the default linkage model of CHERI means that PCC has bounds of
   an entire loaded `.text` segment (the executable or one of its loaded
   libraries), so ensuring the use of sentries when crossing segments restricts
   the ability to source gadgets to any that may exist within the segment
   vulnerable to ROP or JOP injection.

   The curious reader should seek out additional information about CHERI's
   "object type" mechanism and sealed capabilities, of which sentries are just
   one example.
