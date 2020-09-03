# Answers - Disassemble and debug RISC-V and CHERI-RISC-V programs

2. `jalr`. The target address is a pc-relative address in the `.plt`
   section addressed by a sequence like:
```
   1182a: 97 00 00 00   auipc   ra, 0
   1182e: e7 80 60 0e   jalr    230(ra)
```
3. `cjalr`. The target capability is loaded from the `.captable` section
   by a sequence like:
```
    1b2e: 17 24 00 00   auipcc  cs0, 2
    1b32: 0f 24 24 27   clc     cs0, 626(cs0)
    1b36: db 00 c4 fe   cjalr   cs0
```
4. Example session:
```
(gdb) b printf
Breakpoint 1 at 0x11914
```
5. Example session:
```
(gdb) r
Starting program: /root/print-pointer-riscv 

Breakpoint 1, printf (fmt=<optimized out>)
    at /Volumes/CheriBSD/cheribsd/lib/libc/stdio/printf.c:56
56      /Volumes/CheriBSD/cheribsd/lib/libc/stdio/printf.c: No such file or directory.
(gdb) info reg a0
a0             0x1054f  66895
```
6. Example session:
```
(gdb) info proc mappings
process 764
Mapped address spaces:

          Start Addr           End Addr       Size     Offset   Flags   File
             0x10000            0x11000     0x1000        0x0  r-- CN-- /root/print-pointer-riscv
             0x11000            0x12000     0x1000        0x0  r-x C--- /root/print-pointer-riscv
             0x12000            0x13000     0x1000        0x0  r-- C--- /root/print-pointer-riscv
             0x13000            0x14000     0x1000        0x0  rw- ---- 
          0x40013000         0x40018000     0x5000        0x0  r-- CN-- /libexec/ld-elf64.so.1
          0x40018000         0x4002a000    0x12000     0x4000  r-x C--- /libexec/ld-elf64.so.1
          0x4002a000         0x4002b000     0x1000    0x15000  rw- C--- /libexec/ld-elf64.so.1
          0x4002b000         0x4004e000    0x23000        0x0  rw- ---- 
          0x4004f000         0x400c2000    0x73000        0x0  r-- CN-- /usr/lib64/libc.so.7
          0x400c2000         0x401de000   0x11c000    0x72000  r-x C--- /usr/lib64/libc.so.7
          0x401de000         0x401e8000     0xa000   0x18d000  r-- C--- /usr/lib64/libc.so.7
          0x401e8000         0x401ef000     0x7000   0x196000  rw- C--- /usr/lib64/libc.so.7
          0x401ef000         0x40419000   0x22a000        0x0  rw- ---- 
          0x40600000         0x40e00000   0x800000        0x0  rw- ---- 
        0x3f3ef00000       0x3f7eee0000 0x3ffe0000        0x0  --- ---- 
        0x3f7eee0000       0x3f7ef00000    0x20000        0x0  rw- ---D 
        0x3f7efff000       0x3f7f000000     0x1000        0x0  r-x ---- 
```
7. Example session:
```
(gdb) info reg pc 
pc             0x401bf640       1075574336
```
In this example, the pointer resides in:
```
0x400c2000         0x401de000   0x11c000    0x72000  r-x C--- /usr/lib64/libc.so.7
```
8. Example session:
```
(gdb) b printf
Function "printf" not defined.
Make breakpoint pending on future shared library load? (y or [n]) y
Breakpoint 1 (printf) pending.
```
9. Example session:
```
(gdb) r
Starting program: /root/print-pointer-cheri 

Breakpoint 1, printf (
    fmt=0x1004ef [rR,0x1004ef-0x100505] "size of pointer: %zu\n")
    at /Volumes/CheriBSD/cheribsd/lib/libc/stdio/printf.c:54
54      /Volumes/CheriBSD/cheribsd/lib/libc/stdio/printf.c: No such file or directory.
(gdb) p fmt
$1 = 0x1004ef [rR,0x1004ef-0x100505] "size of pointer: %zu\n"
```
10. Example session:
```
(gdb) info proc mappings
process 767
Mapped address spaces:

          Start Addr           End Addr       Size     Offset   Flags   File
            0x100000           0x101000     0x1000        0x0  r-- CN-- /root/print-pointer-cheri
            0x101000           0x102000     0x1000        0x0  r-x CN-- /root/print-pointer-cheri
            0x102000           0x103000     0x1000        0x0  r-- C--- /root/print-pointer-cheri
            0x103000           0x104000     0x1000        0x0  rw- ---- 
          0x40103000         0x4010a000     0x7000        0x0  rw- ---- 
          0x4010a000         0x4010c000     0x2000        0x0  --- CN-- 
          0x4010d000         0x40193000    0x86000        0x0  r-- CN-- /lib/libc.so.7
          0x40193000         0x4028c000    0xf9000    0x85000  r-x C--- /lib/libc.so.7
          0x4028c000         0x40293000     0x7000   0x17d000  r-- C--- /lib/libc.so.7
          0x40293000         0x402ac000    0x19000   0x183000  rw- C--- /lib/libc.so.7
          0x402ac000         0x402c9000    0x1d000        0x0  rw- ---- 
          0x402c9000         0x402f1000    0x28000        0x0  rw- ---- 
          0x41000000         0x4100b000     0xb000        0x0  r-- CN-- /libexec/ld-elf.so.1
          0x4100b000         0x4102a000    0x1f000     0xa000  r-x C--- /libexec/ld-elf.so.1
          0x4102a000         0x4102b000     0x1000    0x28000  rw- C--- /libexec/ld-elf.so.1
          0x4102b000         0x4102e000     0x3000    0x28000  rw- C--- /libexec/ld-elf.so.1
          0x4102e000         0x41030000     0x2000        0x0  rw- ---- 
        0x3f3ef00000       0x3f7ece0000 0x3fde0000        0x0  --- ---- 
        0x3f7ece0000       0x3f7eee0000   0x200000        0x0  rw- ---D 
        0x3f7eee0000       0x3f7ef00000    0x20000        0x0  rw- ---D 
        0x3f7efff000       0x3f7f000000     0x1000        0x0  r-x ---- 
        0x3f7f000000       0x4000000000 0x81000000        0x0  rw- ---- 
```
11. Example session:
```
(gdb) info reg pcc
pcc            0xf11720000325d0d4000000004026e9e6       0x4026e9e6 <printf+14> [rxR,0x4010d000-0x402c9000]
```
The capability points at:
```
          0x40193000         0x4028c000    0xf9000    0x85000 r-x C--- /lib/libc.so.7
```
12. Left as an exercise to the reader.
