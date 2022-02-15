# Demonstrate pointer injection

This exercise demonstrates how CHERI's pointer provenance validity prevents
injected pointer values from being dereferenced.
In this example code, a pointer is injected via pipe IPC, and then
dereferenced.

1. Compile `long-over-pipe.c` with a baseline target and a binary name of
   `long-over-pipe-baseline`, and with a CHERI-enabled target and a binary
   name of `long-over-pipe-cheri`.

2. Run the two binaries, which both send long integers over pipe IPC, and
   print the sent and received values.

3. Compile `ptr-over-pipe.c` with a baseline target and a binary name of
  `ptr-over-pipe-baseline`, and with a CHERI-enabled target and a binary name of
  `ptr-over-pipe-cheri`.

4. Run the two binaries, which both send pointers over pipe IPC, and then
   dereference the received copy to print a string.

5. Why does dereferencing the received pointer in a CHERI binary fail?

## Source Files

**long-over-pipe.c**
```C
{{#include long-over-pipe.c}}
```

**ptr-over-pipe.c**
```C
{{#include ptr-over-pipe.c}}
```
