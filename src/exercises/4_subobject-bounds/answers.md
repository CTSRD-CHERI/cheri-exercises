# Answers - Explore Subobject Bounds

## Exercise a subobject buffer overflow

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
   (gdb) b fill_buf
   Breakpoint 1 at 0x1bae: file buffer-overflow-subobject.c, line 17.
   (gdb) r
   Starting program: /root/buffer-overflow-subobject-cheri
   b.i = c

   Breakpoint 1, fill_buf (buf=0x103f50 <b> [rwRW,0x103f50-0x103fd4] "", len=128)
       at buffer-overflow-subobject.c:17
   17                      buf[i] = 'b';
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
   Breakpoint 1 at 0x1bae: file buffer-overflow-subobject.c, line 17.
   (gdb) r
   Starting program: /root/buffer-overflow-subobject-cheri
   b.i = c

   Breakpoint 1, fill_buf (buf=0x103f50 <b> [rwRW,0x103f50-0x103fd0] "", len=128) at buffer-overflow-subobject.c:17
   17                      buf[i] = 'b';
   ```
   The pointer to the buffer is now bounded to the array rather than the object.

   Investigating further will reveal that the compiler has inserted a
   bounds-setting instruction prior to the call to `fill_buf` in `main`, that
   is, when the pointer to `b.buffer` is materialized.
   ```
   (gdb) up
   #1  0x0000000000101c0c in main () at buffer-overflow-subobject.c:26
   26              fill_buf(b.buffer, sizeof(b.buffer));
   (gdb) disassemble
   Dump of assembler code for function main:
      0x0000000000101bc0 <+0>:     cincoffset      csp,csp,-64
   ...
      0x0000000000101bfc <+60>:    csetbounds      ca0,cs1,128
      0x0000000000101c00 <+64>:    li      a1,128
      0x0000000000101c04 <+68>:    auipcc  cra,0x0
      0x0000000000101c08 <+72>:    cjalr   -92(cra)
   => 0x0000000000101c0c <+76>:    clw     a0,128(cs1)
   ```

## Deliberately Using Larger Bounds

1. Example output:
   ```
   Traversing list=0x104320 [rwRW,0x104320-0x104340] first=0x1040e0 [rwRW,0x1040d0-0x104100] lastnp=0x104150 [rwRW,0x104130-0x104160]
    Ilist cursor=0x1040e0 [rwRW,0x1040d0-0x104100]
     next=0x104140 [rwRW,0x104130-0x104160]
     prevnp=0x104330 [rwRW,0x104320-0x104340]
     val field at 0x1040d0 [rwRW,0x1040d0-0x104100]
    Ilist cursor=0x104140 [rwRW,0x104130-0x104160]
     next=0x104320 [rwRW,0x104320-0x104340]
     prevnp=0x1040f0 [rwRW,0x1040d0-0x104100]
     val field at 0x104130 [rwRW,0x104130-0x104160]
   Traversing list again, accessing superobject field...
    Ilist cursor=0x1040e0 [rwRW,0x1040d0-0x104100] value=1 (at 0x1040d0 [rwRW,0x1040d0-0x104100])
    Ilist cursor=0x104140 [rwRW,0x104130-0x104160] value=3 (at 0x104130 [rwRW,0x104130-0x104160])
   ```

2. In turn:

   - All capabilities referencing the sentinel or its fields (including
     `&l->ile_next`) have length `0x20`, corresponding to `sizeof(struct
     ilist_elem`).

   - The next pointers in the sentinel, `0x1040d0 [rwRW,0x1040c0-0x1040f0]`, and
     in the first list element, `0x104130 [rwRW,0x104120-0x104150]`, have legth
     `0x30`, corresponding to `sizeof(struct obj)`.

   - The previous-next pointers in the sentinel, `0x104140
     [rwRW,0x104120-0x104150]` and in the last list element, `0x1040e0
     [rwRW,0x1040c0-0x1040f0]` also have length `0x30`.

3. Example output:
   ```
   Traversing list=0x104350 [rwRW,0x104350-0x104370] first=0x104120 [rwRW,0x104120-0x104140] lastnp=0x104190 [rwRW,0x104190-0x1041a0]
    Ilist cursor=0x104120 [rwRW,0x104120-0x104140]
     next=0x104180 [rwRW,0x104180-0x1041a0]
     prevnp=0x104360 [rwRW,0x104360-0x104370]
     val field at 0x104110 [rwRW,0x104120-0x104140]
    Ilist cursor=0x104180 [rwRW,0x104180-0x1041a0]
     next=0x104350 [rwRW,0x104350-0x104370]
     prevnp=0x104130 [rwRW,0x104130-0x104140]
     val field at 0x104170 [rwRW,0x104180-0x1041a0]
   Traversing list again, accessing superobject field...
   In-address space security exception
   ```

   Notice the line `val field at 0x104110 [rwRW,0x104120-0x104140]`.  This is
   out of bounds!

   The compiler has taken our use of `&obj1.ilist` as an argument to
   `ilist_insert_after` as license to *narrow* the bounds to just the subobject.
   Indeed, the length of all `ile_next` pointers is now `0x20`.  Further, all
   `ile_lastnp` pointers now have length `0x10`, the size of just the capability
   they point to!

4. The compiler will emit a pair of warnings about the uses of
   `ILIST_CONTAINER`:
   ```
   ./subobject-list.c:75:20: error: static_assert failed due to requirement '__builtin_marked_no_subobject_bounds(struct obj) || __builtin_marked_no_subobject_bounds(struct ilist_elem)' "this type is unsafe for use in containerof() with sub-objectbounds. Please mark the member/type with __subobject_use_container_bounds"
   ```

5. We can take the compiler's advice in at least two ways:

   1. We could mark the `struct ilist_elem` type itself:
      ```
      struct ilist_elem {
        struct ilist_elem **ile_prevnp;
        struct ilist_elem *ile_next;
      } __subobject_use_container_bounds;
      ```

      When we run the program now, we will find that we are largely back in the
      case where no sub-object bounds were applied: pointers to the sentinel
      have length `0x20` and pointers to list elements have length `0x30`.
      However, the `&co->val` pointers are still bounded:

      ```
      Traversing list=0x104300 [rwRW,0x104300-0x104320] first=0x1040d0 [rwRW,0x1040c0-0x1040f0] lastnp=0x104140 [rwRW,0x104120-0x104150]
       Ilist cursor=0x1040d0 [rwRW,0x1040c0-0x1040f0]
        next=0x104130 [rwRW,0x104120-0x104150]
        prevnp=0x104310 [rwRW,0x104300-0x104320]
        val field at 0x1040c0 [rwRW,0x1040c0-0x1040f0]
       Ilist cursor=0x104130 [rwRW,0x104120-0x104150]
        next=0x104300 [rwRW,0x104300-0x104320]
        prevnp=0x1040e0 [rwRW,0x1040c0-0x1040f0]
        val field at 0x104120 [rwRW,0x104120-0x104150]
      Traversing list again, accessing superobject field...
       Ilist cursor=0x1040d0 [rwRW,0x1040c0-0x1040f0] value=1 (at 0x1040c0 [rwRW,0x1040c0-0x1040c4])
       Ilist cursor=0x104130 [rwRW,0x104120-0x104150] value=3 (at 0x104120 [rwRW,0x104120-0x104124])
      ```

   2. We can mark the `ilist` field of `struct obj`:
      ```
      struct obj {
        int val;
        struct ilist_elem ilist __subobject_use_container_bounds;
      };
      ```

      In this case, we find that the `ile_next` pointers are offset and not
      bounded, while the `ile_prevnp` pointers are tightly bounded:
      ```
      Traversing list=0x104340 [rwRW,0x104340-0x104360] first=0x104110 [rwRW,0x104100-0x104130] lastnp=0x104180 [rwRW,0x104180-0x104190]
       Ilist cursor=0x104110 [rwRW,0x104100-0x104130]
        next=0x104170 [rwRW,0x104160-0x104190]
        prevnp=0x104350 [rwRW,0x104350-0x104360]
        val field at 0x104100 [rwRW,0x104100-0x104130]
       Ilist cursor=0x104170 [rwRW,0x104160-0x104190]
        next=0x104340 [rwRW,0x104340-0x104360]
        prevnp=0x104120 [rwRW,0x104120-0x104130]
        val field at 0x104160 [rwRW,0x104160-0x104190]
      Traversing list again, accessing superobject field...
       Ilist cursor=0x104110 [rwRW,0x104100-0x104130] value=1 (at 0x104100 [rwRW,0x104100-0x104104])
       Ilist cursor=0x104170 [rwRW,0x104160-0x104190] value=3 (at 0x104160 [rwRW,0x104160-0x104164])
      ```
