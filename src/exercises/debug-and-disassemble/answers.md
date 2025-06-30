# Answers - Disassemble and debug RISC-V and CHERI-RISC-V programs

2. The target address of the string pointer is constructed as an integer by
   emitting lui/addi to form a literal number. The sequence looks like:
```
  200050: 37 25 20 00   lui     a0, 514
  200054: 13 05 d5 f5   addi    a0, a0, -163
```
3. The target capability pointer for the string is loaded from the `.captable`
   section by a sequence like:
```
  20005c: 17 35 00 00   auipc   ca0, 3
  200060: 0f 45 45 0f   lc      ca0, 244(ca0)
```
4. Example session:
```
(gdb) break printf_
Breakpoint 1 at 0x200090: file src/printf.c, line 1151.
(gdb) c
Breakpoint 1, printf_ (format=0x201f5d "size of pointer: %zu\n") at src/printf.c:1151
```
5. Example session:
```
(gdb) c
Breakpoint 1, printf_ (format=0x201f5d "size of pointer: %zu\n") at src/printf.c:1151
(gdb) info reg a0
a0             0x3fffffef78     274877902712
```
6. Example session:
```
(gdb) info reg pc
pc             0x200090 0x200090 <printf_+22>
```
7. Example session:
```
(gdb) info reg pc
pc             0x401bf640       1075574336
```
8. Example session:
```
(gdb) info reg ca0
ca0            0x1ee580005b4e6bd00000000002026bd        0x2026bd [V:1111:C:r..C.l..:1:.:0x2026bd-0x2026d3]
```
9. Example session:
```
(gdb) info reg pcc
pcc            0x1eed800000180020000000000200098        0x200098 <printf_+12> [V:1111:C:r.xC.l..:1:.:0x200000-0x204000]
```
10. Left as an exercise to the reader.
