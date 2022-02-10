# Answers - Exercise an inter-stack-object buffer overflow

2. Expected output:
```
# ./buffer-overflow-stack-baseline
upper = 0x80d879d0, lower = 0x80d879c0, diff = 10
upper[0] = a
upper[0] = b
# ./buffer-overflow-stack-cheri
upper = 0x3fffdfff50, lower = 0x3fffdfff40, diff = 10
upper[0] = a
In-address space security exception
```

3. An example session of `gdb-run.sh ./buffer-overflow-stack-cheri` on CHERI-RISC-V:
```
Reading symbols from ./buffer-overflow-stack-cheri...
Starting program: /mnt/buffer-overflow-stack-cheri 
upper = 0x3fffdfff50, lower = 0x3fffdfff40, diff = 10
upper[0] = a

Program received signal SIGPROT, CHERI protection violation
Capability bounds fault caused by register ca0.
0x0000000000101ce8 in write_buf (buf=<optimized out>, ix=<optimized out>) at ./buffer-overflow-stack.c:13
13              buf[ix] = 'b';

Thread 1 (LWP 100044 of process 838):
#0  0x0000000000101ce8 in write_buf (buf=<optimized out>, ix=<optimized out>) at ./buffer-overflow-stack.c:13
#1  0x0000000000101d72 in main () at ./buffer-overflow-stack.c:31
(gdb) disass
Dump of assembler code for function write_buf:
   0x0000000000101ce0 <+0>:       cincoffset      ca0,ca0,a1
   0x0000000000101ce4 <+4>:       li      a1,98
=> 0x0000000000101ce8 <+8>:       sb      a1,0(a0)
   0x0000000000101cec <+12>:      ret
End of assembler dump.
```

*Note:* due to deficiencies in the current GDB implementation, the faulting
instruction incorrectly decodes as `sb` rather than correctly as `csb a1,
0(ca0)`.  Asking `gdb` about the registers with `info registers` and focusing
on the ones involved here, we see
```
a0             0x3fffdfff50     274875809616
a1             0x62     98

ca0            0xd17d000007d5bf440000003fffdfff50       0x3fffdfff50 [rwRW,0x3fffdfff40-0x3fffdfff50]
ca1            0x62     0x62
```
The capability in `ca0`, which is a pointer into the `lower` buffer, has been
taken beyond the end of the allocation, as out of bounds store has been
attempted (`Capability bounds fault`).

But where did those bounds originate?  Heading `up` a stack frame and
disassembling, we see (eliding irrelevant instructions):
```
(gdb) up
#1  0x0000000000101d72 in main () at ./buffer-overflow-stack.c:31
31              write_buf(lower, sizeof(lower));
(gdb) disass
Dump of assembler code for function main:
   0x0000000000101cf0 <+0>:       cincoffset      csp,csp,-144

   0x0000000000101d0c <+28>:      cincoffset      ca0,csp,48
   0x0000000000101d10 <+32>:      csetbounds      cs0,ca0,16

   0x0000000000101d64 <+116>:     li      a1,16
   0x0000000000101d66 <+118>:     cmove   ca0,cs0
   0x0000000000101d6a <+122>:     auipc   ra,0x0
   0x0000000000101d6e <+126>:     jalr    -138(ra) # 0x101ce0 <write_buf>
=> 0x0000000000101d72 <+130>:     lbu     a0,0(s1)
```
The compiler has arranged for `main` to allocate 144 bytes on the stack by
decrementing the *capability stack pointer* register (`csp`) by 144 bytes.
Further, the compiler has placed `lower` 48 bytes up into that allocation:
`ca0` is made to point at its lowest address and then the pointer to `lower`
is materialized in `cs0` by *bounding* the capability in `ca0` to be 16
(`sizeof(lower)`) bytes long.  This capability is passed to `write_buf` in
`ca0`.

4. The code for `write_buf` function is only slightly changed.  On
RISC-V it compiles to
```
<write_buf>:
 add     a0, a0, a1
 addi    a1, zero, 98
 sb      a1, 0(a0)
 ret
```
while on CHERI-RISC-V, it is
```
<write_buf>:
 cincoffset      ca0, ca0, a1
 addi    a1, zero, 98
 csb     a1, 0(ca0)
 cret
```
In both cases, it amounts to displacing the pointer passed in `a0` (resp.
`ca0`) by the offset passed in `a1` and then performing a store-byte
instruction before returning.  In the baseline case, the store-byte takes an
*integer* address for its store, while in the CHERI case, the store-byte takes
a *capability authorizing the store*.  There are no conditional branches or
overt bounds checks in the CHERI instruction stream; rather, the `csb`
instruction itself enforces the requirement for authority to write to memory,
in the shape of a valid, in-bounds capability.

We have already seen the CHERI program's call site to `write_buf` in `main`,
and the derivation of the capability to the `lower` buffer, above.  In the
baseline version, the corresponding instructions are shown as
```
(gdb) disass main
Dump of assembler code for function main:
   0x0000000000011b44 <+0>:       addi    sp,sp,-48

   0x0000000000011b8a <+70>:      mv      a0,sp
   0x0000000000011b8c <+72>:      li      a1,16
   0x0000000000011b8e <+74>:      auipc   ra,0x0
   0x0000000000011b92 <+78>:      jalr    -86(ra) # 0x11b38 <write_buf>
```
Here, the compiler has reserved only 48 bytes of stack space and has placed the
`lower` buffer at the lowest bytes of this reservation.  Thus, to pass a
pointer to the `lower` buffer to `write_buf`, the program simply copies the
stack pointer register (an *integer* register, holding an *address*) to the
argument register `a0`.  The subsequent address arithmetic derives an address
out of bounds, clobbering a byte of the `upper` register.
