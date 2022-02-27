# Answers

## Introducing heap-allocator bounds

2. GDB will show a CHERI tag violation resulting from `memset()` overwriting
   the `a_next` field in the second allocation entry, which is tripped over by
   a later call to `alloc_allocate()`:

```
Starting program: /opt/cheri-exercises/cheri-allocator-cheri 
Allocator initialised
Allocating memory
Allocation returned 0x104830
Preparing to overflow 0x104830
Overflowed allocation 0x104830
Freeing allocation 0x104830
Allocation 0x104830 freed
Allocating memory
Allocation returned 0x104830
Allocating memory
Allocation returned 0x1048c0
Allocating memory

Program received signal SIGPROT, CHERI protection violation
Capability tag fault caused by register ca2.
0x0000000000102374 in alloc_allocate ()
```

3. When compiling for CHERI C, use `cheri_bounds_set()` to set bounds on the
   returned pointer:

```
        /* Return pointer to allocated memory. */
#ifdef __CHERI_PURE_CAPABILITY__
        return (cheri_bounds_set(alloc->a_bytes, ALLOC_SIZE));
#else
        return (alloc->a_bytes);
#endif
```

4. With this change, the `memset()` call in `main()` triggers a bounds
   violation exception on overflow:

```
Starting program: /opt/cheri-exercises/cheri-allocator-cheri 
Allocator initialised
Allocating memory
Allocation returned 0x104840
Preparing to overflow 0x104840

Program received signal SIGPROT, CHERI protection violation
Capability bounds fault caused by register ca3.
memset (dst0=0x1048c0 <alloc_array+144> [rwRW,0x104840-0x1048c0], c0=65, 
    length=15) at /home/rnw24/cheri/cheribsd/lib/libc/string/memset.c:94
```

## Reaching allocator metadata

6. Following this change, `alloc_free()` crashes with a bounds violation,
   due to reaching outside the bounds of the passed memory allocation:

```
Starting program: /opt/cheri-exercises/cheri-allocator-cheri 
Allocator initialised
Allocating memory
Allocation returned 0x1046e0
Freeing allocation 0x1046e0

Program received signal SIGPROT, CHERI protection violation
Capability bounds fault caused by register ca3.
0x00000000001022fe in alloc_free ()
(gdb) bt
#0  0x00000000001022fe in alloc_free ()
#1  0x0000000000101f52 in main ()
```

7. We need to create a new capability, derived from `alloc_array` but with the
   address generated from pointer to the memory being freed.
   One way to do this is using the `cheri_address_get()` and
   `cheri_address_set()`, reading the address from one capability and setting
   it on the other:

```
#ifdef __CHERI_PURE_CAPABILITY__
        /*
         * Generate a new pointer to the allocation that is derived from the
         * one passed by the consumer.
         */
        ptr = cheri_address_set(alloc_array, cheri_address_get(ptr));
#endif
```

   Note that this is not a complete solution to providing spatial safety here:
   software could still accidentally pass an out-of-bounds pointer.
