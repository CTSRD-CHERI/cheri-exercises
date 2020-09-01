# Answers
## Indirect control flow through aliased heap objects

2. Expected output (addresses may vary):
```
Installing function pointer in obj1 at 0x40809000
Demonstrating use after free:
 First function: 0x40809000
Assigning function pointer through obj2 at 0x40809000
Calling function pointer through obj1 (now 0x40809000):
 Second function: 0x40809000
```
4. Expected output (addresses may vary):
```
Installing function pointer in obj1 at v:1 s:0 p:0006817d b:0000000041200040 l:0000000000000040 o:0 t:-1
Demonstrating use after free:
 First function: v:1 s:0 p:0006817d b:0000000041200040 l:0000000000000040 o:0 t:-1
Assigning function pointer through obj2 at v:1 s:0 p:0006817d b:00000000412000c0 l:0000000000000040 o:0 t:-1
Calling function pointer through obj1 (now v:1 s:0 p:0006817d b:0000000041200040 l:0000000000000040 o:0 t:-1):
 First function: v:1 s:0 p:0006817d b:0000000041200040 l:0000000000000040 o:0 t:-1
```
6. Expected output (addresses may vary):
```
Installing function pointer in obj1 at v:1 s:0 p:0006817d b:0000000041200040 l:0000000000000040 o:0 t:-1
Demonstrating use after free:
 First function: v:1 s:0 p:0006817d b:0000000041200040 l:0000000000000040 o:0 t:-1
Assigning function pointer through obj2 at v:1 s:0 p:0006817d b:0000000041200040 l:0000000000000040 o:0 t:-1
Calling function pointer through obj1 (now v:1 s:0 p:00000000 b:0000000041200040 l:0000000000000040 o:0 t:-1):
In-address space security exception (core dumped)
```
7. The process is attempting a load through a capability with valid tag but no
permissions, as can be readily seen in `gdb`:
```
Program received signal SIGPROT, CHERI protection violation
Capability permission fault caused by register ca2.
0x0000000000102140 in main ()

(gdb) x/i 0x0000000000102140
=> 0x102140 <main+524>:     lc      ca2,32(a2)

(gdb) p $ca2
$1 = (void *) 0x41200040 [,0x41200040-0x41200080]
```
`malloc_revoke()` must have replaced the `obj1` capability, which previously
had permissions for loading and storing both data and capabilities to the
memory backing `obj1`, with this permissionless form.
