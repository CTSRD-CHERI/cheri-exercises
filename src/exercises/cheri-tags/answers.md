# Answers

2. Example output for the baseline program:
   ```
    buf=0000003FFFFFEDA9 &p=0000003FFFFFEDA0
    p.ptr=0000003FFFFFEEB8 (0x10f into buf) *p.ptr=0f
    q=0000003FFFFFEE00 (0x57 into buf)
    *q=57
    r=0000003FFFFFEE00 (0x57)
    *r=57
   ```

   And for the CHERI-enabled program:
   ```
    init=0x200058 [rxCM1111,0x200000-0x202db0] (capmode) (sentry)
    buf=0x3fffffed41 [rwCM1111,0x3fffffed41-0x3fffffef40] &p=0x3fffffed30 [rwCM1111,0x3fffffed30-0x3fffffed40]
    p.ptr=0x3fffffee50 [rwCM1111,0x3fffffed41-0x3fffffef40] (0x10f into buf) *p.ptr=0f
    q=0x3fffffee00 [rwCM1111,0x3fffffed41-0x3fffffef40] (0xbf into buf)
    *q=bf
    r=0x3fffffee00 [rwCM1111,0x3fffffed41-0x3fffffef40] (invalid) (0xbf)
    MON|ERROR: received message 0x00000006  badge: 0x0000000000000001  tcb cap: 0x8000000000000008
    MON|ERROR: faulting PD: cheri-tags-cheri
   ```

3. The CHERI-Microkit's `MONITOR` should report something like
   ```
    MON|ERROR: received message 0x00000006  badge: 0x0000000000000001  tcb cap: 0x8000000000000008
    MON|ERROR: faulting PD: cheri-tags-cheri
    Registers:
    ddc : 0x0
    pcc : 0x200132 [rxCM1111,0x200000-0x204000] (capmode)
    cra : 0x200132 [rxCM1111,0x200000-0x204000] (capmode) (sentry)
    csp : 0x3fffffed10 [rwCM1111,0x3fffffe000-0x3ffffff000]
    cgp : 0x0
    cs0 : 0x3fffffed41 [rwCM1111,0x3fffffed41-0x3fffffef40]
    cs1 : 0x3fffffee00
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
    ca0 : 0x43
    ca1 : 0x3fffffecef [rwCM1111,0x3fffffecef-0x3fffffecf0]
    ca2 : 0x43
    ca3 : 0xffffffffffffffff
    ca4 : 0x0
    ca5 : 0x0
    ca6 : 0x0
    ca7 : 0xfffffffffffffff4
    ct0 : 0x78
    ct1 : 0x201d94 [rxCM1111,0x200000-0x202db0] (capmode)
    ct2 : 0x0
    ct3 : 0x0
    ct4 : 0x100
    ct5 : 0x0
    ct6 : 0x0
    ctp : 0x0
    MON|ERROR: CHERI Security Violation: ip=0x0000000000200132  fault_addr=0x0000003fffffee00  fsr=0x0000000000000810  (data fault)
    MON|ERROR: description of fault: Tag violation
    MON|ERROR: CHERI fault type: CHERI data fault due to load, store or AMO
    <<seL4(CPU 0) [receiveIPC/142 T0xffffffc0fffe6400 "rootserver" @8a000460]: Reply object already has unexecuted reply!>>
    ```

    This tells you there was an attempt to access an untagged CHERI pointer at PC=0x200132. The faulting address of the CHERI pointer
    is 0x3fffffee00. if you look at the registers dump, you will find `cs1` holding that address, without any capability metadata.
    This means it is an invalid pointer capability, and thus it only prints its address field. If you use llvm-obdjump or QEMU/GDB
    to investiage further what instruction that is, you would find something similar to the following:
    ```
    200132: 83 c5 04 00   lbu     a1, 0(cs1)
    200136: 17 35 00 00   auipc   ca0, 3
    20013a: 0f 45 a5 1a   lc      ca0, 426(ca0)
    20013e: 2e e0         sd      a1, 0(csp)
    200140: 97 00 00 00   auipc   cra, 0
    200144: e7 80 60 01   jalr    22(cra)
    ```

4. Constructing `r` is very similar on the two targets, differing only by the
   use of integer- or capability-based memory instructions:

   |       | Baseline         | CHERI               |
   | ----- | :-------         | :----               |
   | Store | `sb zero, 0(sp)` | `sb zero, 32(csp)` |
   | Load  | `ld s0, 0(sp)`   | `lc cs1, 32(csp)`  |

   The significant difference is in the construction of `q`.  On the baseline
   architecture, it is a direct bitwise `and` of a pointer loaded from memory:

   ```
   ld   a0, 0(sp)
   andi s0, a0, -256
   ```

   On CHERI, on the other hand, the program makes explicit use of capability
   manipulation instructions to...

   | Instruction | Action |
   | ----------- | ------ |
   | `lc        ca0, 32(csp)` | Load the capability from memory |
   | `andi      a1, a0, -256` | Perform the mask operation on integer/address (field of) registers |
   | `scaddr    cs1, ca0, a1` | Update the address field |

   This longer instruction sequence serves to prove to the processor that the
   resulting capability (in `cs1`) was constructed using valid transformations.
   In particular, the `scaddr` allows the processor to check that the
   combination of the old capability (in `ca0`) and the new address (in `a1`)
   remains *representable*.

5. While the in-memory, byte representation of `q` and `r` are identical, `r`
   has been manipulated as *bytes* rather than as a *capability* and so has had
   its tag zeroed.  (Specifically, the `sb zero, 32(csp)` instruction cleared
   the tag associated with the 16-byte granule pointed to by `32(csp)`; the
   subsequent `lc` transferred this zero tag to `cs1`.)
