# Answers - Exercise a subobject buffer overflow

This exercise demonstrates how subobject bounds can correct and array in a
structure.

2. Expected output:
   ```
   # ./buffer-overflow-subobject-riscv
   b.i = c
   b.i = b
   # ./buffer-overflow-subobject-cheri
   b.i = c
   b.i = b
   ```

3. Example session:
   ```
   (gdb) f fill_buf
   No registers.
   (gdb) b fill_buf
   Breakpoint 1 at 0x1b3a: file src/exercises/buffer-overflow-subobject/buffer-overflow-subobject.c, line 13.
   (gdb) r
   Starting program: /root/buffer-overflow-subobject-cheri

   Breakpoint 1, fill_buf (buf=0x103e50 <b> [rwRW,0x103e50-0x103ed4] "", len=128)
       at src/exercises/buffer-overflow-subobject/buffer-overflow-subobject.c:13
   13      src/exercises/buffer-overflow-subobject/buffer-overflow-subobject.c: No such file or directory.
   ```
   The bounds are `132` bytes corresponding to the size of the underlying object.

5. Expected output:
   ```
   # ./buffer-overflow-subobject-cheri
   b.i = c
   In-address space security exception (core dumped)
   ```

6. Example session:
   ```
   (gdb) b fill_buf
   Breakpoint 1 at 0x1b3a: file src/exercises/buffer-overflow-subobject/buffer-overflow-subobject.c, line 13.
   (gdb) r
   Starting program: /root/buffer-overflow-subobject-cheri-subobject

   Breakpoint 1, fill_buf (buf=0x103e50 <b> [rwRW,0x103e50-0x103ed0] "", len=128)
       at src/exercises/buffer-overflow-subobject/buffer-overflow-subobject.c:13
   13      src/exercises/buffer-overflow-subobject/buffer-overflow-subobject.c: No such file or directory.
   ```
   The pointer to the buffer is now bounded to the array rather than the object.

   Investigating further will reveal that the compiler has inserted a
   bounds-setting instruction prior to the call to `fill_buf` in `main`, that
   is, when the pointer to `b.buffer` is materialized.
