# Answers

## Introducing heap-allocator bounds

2. GDB will show a CHERI tag violation resulting from `memset()` overwriting
   the `a_next` field in the second allocation entry, which is tripped over by
   a later call to `alloc_allocate()`:

```
Starting program: /root/cheri-allocator
Allocator initialised
Allocating memory
Allocation returned 0x104550
Preparing to overflow 0x104550
Overflowed allocation 0x104550
Freeing allocation 0x104550
Allocation 0x104550 freed
Allocating memory
Allocation returned 0x104550
Allocating memory
Allocation returned 0x1045e0
Allocating memory

Program received signal SIGPROT, CHERI protection violation.
Capability tag fault caused by register ca0.
alloc_allocate () at cheri-allocator.c:83
83              alloc_nextfree = alloc->a_next;
(gdb) p alloc
$1 = (struct alloc_storage *) 0x4141414141414141 [,0x4141402800000000-0x414142a000000000] (invalid,sealed)
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
Starting program: /root/cheri-allocator
Allocator initialised
Allocating memory
Allocation returned 0x104550
Preparing to overflow 0x104550

Program received signal SIGPROT, CHERI protection violation.
Capability bounds fault caused by register ca3.
memset (dst0=0x1045d0 <alloc_array+144> [rwRW,0x104550-0x1045d0], c0=65, length=15) at /usr/home/john/work/cheri/git/cheribsd/lib/libc/string/memset.c:94
94                              *dst++ = VAL;
```

## Reaching allocator metadata

6. Following this change, `alloc_free()` crashes with a bounds violation,
   due to reaching outside the bounds of the passed memory allocation:

```
Starting program: /root/cheri-allocator
Allocator initialised
Allocating memory
Allocation returned 0x104420
Freeing allocation 0x104420

Program received signal SIGPROT, CHERI protection violation.
Capability bounds fault caused by register cfp.
alloc_free (ptr=<optimized out>) at cheri-allocator.c:106
106             alloc->a_next = alloc_nextfree;
(gdb) bt
#0  alloc_free (ptr=<optimized out>) at cheri-allocator.c:106
#1  main () at cheri-allocator.c:137
(gdb) p alloc
$1 = (struct alloc_storage *) 0x104410 <alloc_array> [rwRW,0x104420-0x1044a0]
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
