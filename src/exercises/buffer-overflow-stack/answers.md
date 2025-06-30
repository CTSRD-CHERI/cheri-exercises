# Answers - Exercise an inter-stack-object buffer overflow

2. Expected output:
   ```
   # run_qemu ./buffer-overflow-stack-baseline
   upper = 0000003FFFFFEFA0, lower = 0000003FFFFFEF90, diff = 10
   upper[0] = a
   upper[0] = b
   # run_qemu ./buffer-overflow-stack-cheri
   upper = 00000000000000000000003FFFFFEF30, lower = 00000000000000000000003FFFFFEF20, diff = 10
   upper[0] = a
   MON|ERROR: received message 0x00000006  badge: 0x0000000000000001  tcb cap: 0x8000000000000008
   MON|ERROR: faulting PD: buffer-overflow-stack-cheri
   ```

3. An example of the Monitor's output for buffer-overflow-stack-cheri` on CHERI-RISC-V:
   ```
    MON|ERROR: received message 0x00000006  badge: 0x0000000000000001  tcb cap: 0x8000000000000008
    MON|ERROR: faulting PD: buffer-overflow-stack-cheri
    Registers:
    ddc : 0x0
    pcc : 0x200060 [rxCM1111,0x200000-0x204000] (capmode)
    cra : 0x2000ea [rxCM1111,0x200000-0x204000] (capmode) (sentry)
    csp : 0x3fffffeef0 [rwCM1111,0x3fffffe000-0x3ffffff000]
    cgp : 0x0
    cs0 : 0x3fffffef20 [rwCM1111,0x3fffffef20-0x3fffffef30]
    cs1 : 0x202693 [rCM1111,0x202693-0x2026a2]
    cs2 : 0x0
    cs3 : 0x203000 [rwCM1111,0x203000-0x203010]
    cs4 : 0x0
    cs5 : 0x0
    cs6 : 0x0
    cs7 : 0x0
    cs8 : 0x0
    cs9 : 0x0
    cs10 : 0x0
    cs11 : 0x0
    ca0 : 0x3fffffef30 [rwCM1111,0x3fffffef20-0x3fffffef30]
    ca1 : 0x62
    ca2 : 0xd
    ca3 : 0xffffffffffffffff
    ca4 : 0x0
    ca5 : 0x0
    ca6 : 0x0
    ca7 : 0xfffffffffffffff4
    ct0 : 0x0
    ct1 : 0x201d46 [rxCM1111,0x200000-0x202cf0] (capmode)
    ct2 : 0x0
    ct3 : 0x0
    ct4 : 0x0
    ct5 : 0x0
    ct6 : 0x0
    ctp : 0x0
    MON|ERROR: CHERI Security Violation: ip=0x0000000000200060  fault_addr=0x0000003fffffef30  fsr=0x0000000000000814  (data fault)
    MON|ERROR: description of fault: Bounds violation
    MON|ERROR: CHERI fault type: CHERI data fault due to load, store or AMO
    <<seL4(CPU 0) [receiveIPC/142 T0xffffffc0fffe6400 "rootserver" @8a000460]: Reply object already has unexecuted reply!>
   ```

   Using GDB or llvm-objdump to see what instructions at the faulting PC (ip=0x000200060), we see:
   ```
   ;       buf[ix] = 'b';
   200060: 23 00 b5 00   sb      a1, 0(ca0)
   ```

   If we investigate the content of `ca0` (either using GDB's `info reg $ca0` or from the register dump), we can see it does have something
   like:
   ```
   ca0 : 0x3fffffef30 [rwCM1111,0x3fffffef20-0x3fffffef30]
   ```

   The capability in `ca0`, which is a pointer into the `lower` buffer, has been
   taken beyond the end of the allocation, as out of bounds store has been
   attempted (`Capability bounds fault`).

   But where did those bounds originate?  Heading `up` a stack frame and
   `disass`embling, we see (eliding irrelevant instructions):
   ```
(gdb) up
#1  0x00000000002000ea in init () at buffer-overflow-stack.c:31
31              write_buf(lower, sizeof(lower));
(gdb) disass
Dump of assembler code for function init:
   0x0000000000200066 <+0>:     c.addi16sp      csp,-128

   0x0000000000200074 <+14>:    c.addi4spn      a0,csp,48
   0x0000000000200076 <+16>:    scbndsi cs0,ca0,16

   0x00000000002000dc <+118>:   li      a1,16
   0x00000000002000de <+120>:   mv      ca0,cs0
   0x00000000002000e2 <+124>:   auipcc  cra,0x0
   0x00000000002000e6 <+128>:   jalr    -138(cra)
=> 0x00000000002000ea <+132>:   lbu     a0,64(csp)
   ```
   The compiler has arranged for `init` to allocate 128 bytes on the stack by
   decrementing the *capability stack pointer* register (`csp`) by 128 bytes.
   Further, the compiler has placed `lower` 48 bytes up into that allocation:
   `ca0` is made to point at its lowest address and then the pointer to `lower`
   is materialized in `cs0` by *bounding* the capability in `ca0` to be 16
   (`sizeof(lower)`) bytes long.  This capability is passed to `write_buf` in
   `ca0`.

4. The code for `write_buf` function is only slightly changed.  On
   RISC-V it compiles to
   ```
    20004c <write_buf>:
    20004c: 2e 95         add     a0, a0, a1
    20004e: 93 05 20 06   li      a1, 98
    200052: 23 00 b5 00   sb      a1, 0(a0)
    200056: 82 80         ret
   ```
   while on CHERI-RISC-V, it is
   ```
    200058 <write_buf>:
    200058: 33 05 b5 0c   cadd    ca0, ca0, a1
    20005c: 93 05 20 06   li      a1, 98
    200060: 23 00 b5 00   sb      a1, 0(ca0)
    200064: 82 80         ret
   ```
   In both cases, it amounts to displacing the pointer passed in `a0` (resp.
   `ca0`) by the offset passed in `a1` and then performing a store-byte
   instruction before returning.  In the baseline case, the store-byte takes an
   *integer* address for its store, while in the CHERI case, the store-byte takes
   a *capability authorizing the store*.  There are no conditional branches or
   overt bounds checks in the CHERI instruction stream; rather, the `csb`
   instruction itself enforces the requirement for authority to write to memory,
   in the shape of a valid, in-bounds capability.

   We have already seen the CHERI program's call site to `write_buf` in `init`,
   and the derivation of the capability to the `lower` buffer, above.  In the
   baseline version, the corresponding instructions are shown as
   ```
    Breakpoint 1, init () at buffer-overflow-stack.c:23
    23                  upper, lower, (size_t)(upper - lower));
    (gdb) disassemble init
    Dump of assembler code for function init:
       0x0000000000200058 <+0>:     addi    sp,sp,-48

       0x00000000002000c0 <+104>:   mv      a0,sp
       0x00000000002000c2 <+106>:   li      a1,16
       0x00000000002000c4 <+108>:   auipc   ra,0x0
       0x00000000002000c8 <+112>:   jalr    -120(ra) # 0x20004c <write_buf>

   ```
   Here, the compiler has reserved only 48 bytes of stack space and has placed the
   `lower` buffer at the lowest bytes of this reservation.  Thus, to pass a
   pointer to the `lower` buffer to `write_buf`, the program simply copies the
   stack pointer register (an *integer* register, holding an *address*) to the
   argument register `a0`.  The subsequent address arithmetic derives an address
   out of bounds, clobbering a byte of the `upper` register.
