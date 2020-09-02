# Answers - Corrupt a control-flow pointer using a subobject buffer overflow

2. Example session:
```
(gdb) r
Starting program: /root/buffer-overflow-fnptr-riscv 

Program received signal SIGSEGV, Segmentation fault.
0x00000000aaaaaaaa in ?? ()
```
The program attempted an instruction fetch from a nonsensical address `0xaaaaaaaa`.

3. Example session:
```
(gdb) r
Starting program: /root/buffer-overflow-fnptr-cheri 

Program received signal SIGPROT, CHERI protection violation
Capability tag fault caused by register ca1.
0x0000000000101c5e in main ()
    at src/exercises/control-flow-pointer/buffer-overflow-fnptr.c:34
34      src/exercises/control-flow-pointer/buffer-overflow-fnptr.c: No such file or directory.
(gdb) info reg ca1
ca1            0xd11720000801800600000000aaaaaaaa       0xaaaaaaaa [rxR,0xaaaa0000-0xaaaa4000] (sentry)
(gdb) x/i $pcc
=> 0x101c5e <main+58>:      cjalr   cra,ca1
```
The program attempted to load an instruction via an untagged capability `ca1`.