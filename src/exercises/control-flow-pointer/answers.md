# Answers - Corrupt a control-flow pointer using a subobject buffer overflow

2. Example session:
```
   # run_qemu ./control-flow-pointer-riscv
   ...
   MON|ERROR: received message 0x00000006  badge: 0x0000000000000001  tcb cap: 0x8000000000000008
   MON|ERROR: faulting PD: control-flow-pointer-riscv
   Registers:
   ddc : 0x0 [rwxCM1111,0x0-0xffffffffffffffff]
   pcc : 0xaaaaaaaa [rwxCM1111,0x0-0xffffffffffffffff]
   cra : 0x20010e
   csp : 0x3fffffefb0
   cgp : 0x203888
   cs0 : 0x203000
   cs1 : 0x0
   cs2 : 0x0
   cs3 : 0x0
   cs4 : 0x0
   cs5 : 0x0
   cs6 : 0x0
   cs7 : 0x0
   cs8 : 0x0
   cs9 : 0x0
   cs10 : 0x0
   cs11 : 0x0
   ca0 : 0x203000
   ca1 : 0xaaaaaaaaaaaaaaaa
   ca2 : 0xaaaaaaaa
   ca3 : 0x0
   ca4 : 0x0
   ca5 : 0x0
   ca6 : 0x0
   ca7 : 0x0
   ct0 : 0x0
   ct1 : 0x0
   ct2 : 0x0
   ct3 : 0x0
   ct4 : 0x0
   ct5 : 0x0
   ct6 : 0x0
   ctp : 0x0
   MON|ERROR: VMFault: ip=0x00000000aaaaaaaa  fault_addr=0x00000000aaaaaaaa  fsr=0x0000000000000001  (instruction fault)
   MON|ERROR: description of fault: Instruction access fault
```
The program attempted an instruction fetch from a nonsensical address
`0xaaaaaaaa`.

3. Example session:
```
   # run_qemu ./control-flow-pointer-cheri
   ...
   MON|ERROR: received message 0x00000006  badge: 0x0000000000000001  tcb cap: 0x8000000000000008
   MON|ERROR: faulting PD: control-flow-pointer-cheri
   Registers:
   ddc : 0x0
   pcc : 0x200134 [rxCM1111,0x200000-0x204000] (capmode)
   cra : 0x202384 [rxCM1111,0x200000-0x204000] (capmode) (sentry)
   csp : 0x3fffffef50 [rwCM1111,0x3fffffe000-0x3ffffff000]
   cgp : 0x0
   cs0 : 0x203000
   cs1 : 0x0
   cs2 : 0x0
   cs3 : 0x203090 [rwCM1111,0x203090-0x2030a0]
   cs4 : 0x0
   cs5 : 0x0
   cs6 : 0x0
   cs7 : 0x0
   cs8 : 0x0
   cs9 : 0x0
   cs10 : 0x0
   cs11 : 0x0
   ca0 : 0x203000 [rwCM1111,0x203000-0x203090]
   ca1 : 0xaaaaaaaa
   ca2 : 0xaaaaaaaa
   ca3 : 0x0
   ca4 : 0x0
   ca5 : 0x0
   ca6 : 0x0
   ca7 : 0x0
   ct0 : 0x0
   ct1 : 0x0
   ct2 : 0x0
   ct3 : 0x0
   ct4 : 0x0
   ct5 : 0x0
   ct6 : 0x0
   ctp : 0x0
   MON|ERROR: CHERI Security Violation: ip=0x0000000000200134  fault_addr=0x0000000000000000  fsr=0x0000000000000820  (data fault)
   MON|ERROR: description of fault: Tag violation
   MON|ERROR: CHERI fault type: CHERI jump or branch fault
```

  If you examine further using GDB by setting a breakpoint on the faulting address, you can see the content of the faulting `ca2`
  that the program tries to jump to:
  ```
  (gdb) break *0x200134
  Breakpoint 1 at 0x200134: file ./control-flow-pointer.c, line 38.
  (gdb) c
  Continuing.
  ...
  Breakpoint 1, init () at ./control-flow-pointer.c:38
  38              printf("Words of screaming in b.buffer %zu\n", b.callback(&b));
  (gdb) disassemble
     0x0000000000200132 <+90>:    sw      a1,124(ca0)
  => 0x0000000000200134 <+92>:    jalr    ca2
  (gdb) info reg $ca2
  ca2            0x1eed8000993800300000000aaaaaaaa        0xaaaaaaaa [I:1111:C:r.xC.l..:1:S:0xaaaa8000-0xaaaaac90]
  ```
The program attempted to load an instruction via an untagged capability `ca2`.
