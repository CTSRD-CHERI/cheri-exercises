# Answers - Explore Subobject Bounds

## Exercise a subobject buffer overflow

This exercise demonstrates how subobject bounds can correct and array in a
structure.

2. Expected output:
   ```
   # run_qemu ./subobject-bounds-riscv
   b.i = c
   b.i = b
   # run_qemu ./subobject-bounds-cheri
   b.i = c
   b.i = b
   ```

3. Example session:
   ```
   (gdb) target remote :1234
   (gdb) b fill_buf
   Breakpoint 1 at 0x200058: file subobject-bounds.c, line 16.
   (gdb) c

   b.i = c

   Breakpoint 1, fill_buf (buf=0x2032c0 <b> [V:1111:C:rw.C.l..:1:.:0x2032c0-0x203344] "", len=128) at subobject-bounds.c:16
   16              for (size_t i = 0; i <= len; i++)

   ```
   The bounds are `132` bytes corresponding to the size of the underlying object.

5. Expected output:
   ```
   # run_qemu ./subobject-bounds-cheri
   b.i = c
   MON|ERROR: received message 0x00000006  badge: 0x0000000000000001  tcb cap: 0x8000000000000008
   MON|ERROR: faulting PD: subobject-bounds-cheri
   MON|ERROR: CHERI Security Violation: ip=0x0000000000200064  fault_addr=0x0000000000203340  fsr=0x0000000000000814  (data fault)
   MON|ERROR: description of fault: Bounds violation
   MON|ERROR: CHERI fault type: CHERI data fault due to load, store or AMO
   ```

6. Example session:
   ```
   Breakpoint 1, fill_buf (buf=0x2032c0 <b> [V:1111:C:rw.C.l..:1:.:0x2032c0-0x203340] "", len=128) at subobject-bounds.c:16
   16              for (size_t i = 0; i <= len; i++)
   ```
   The pointer to the buffer is now bounded to the array rather than the object.

   Investigating further will reveal that the compiler has inserted a
   bounds-setting instruction prior to the call to `fill_buf` in `init`, that
   is, when the pointer to `b.buffer` is materialized.
   ```
    (gdb) up
    #1  0x00000000002000ba in init () at subobject-bounds.c:26
    26              fill_buf(b.buffer, sizeof(b.buffer));
    (gdb) disassemble
    Dump of assembler code for function init:
       0x0000000000200072 <+0>:     addi    sp,sp,-64
       ...
       0x00000000002000a6 <+52>:    li      a0,128
       0x00000000002000aa <+56>:    scbndsr ca0,cs1,a0
       0x00000000002000ae <+60>:    li      a1,128
       0x00000000002000b2 <+64>:    auipcc  cra,0x0
       0x00000000002000b6 <+68>:    jalr    -90(cra)
    => 0x00000000002000ba <+72>:    lw      a0,128(cs1)
   ```
