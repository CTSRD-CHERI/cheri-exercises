# Extending heap allocators for CHERI

CHERI's architectural protection is driven by software -- the compiler,
linker, OS kernel, run-time linker, run-time libraries, and so on all manage
capabilities as part of their program execution.  Heap allocators, which are
integrally tied into our notions of spatial and temporal safety, are typically
extended to use CHERI in five ways:

1. To implement spatial safety, bounds and permissions are set on returned
   pointers.
   *(In this exercise.)*

2. To prevent bounds overlap on larger allocations from arising due to
   imprecise bounds caused by capability compression, large allocations are
   aligned and padded more strongly.
   *(Not in this exercise.)*

3. If the allocator's `free()` implementation relies on reaching allocator
   metadata via its pointer argument (e.g., by looking immediately before or
   after to reach free-list poointers), then the implementation must be
   changed as access will otherwise be prevented by CHERI bounds and
   monotonicity.
   *(In this exercise.)*

4. To implement temporal safety, allocated memory is registered with a
   temporal-safety run-time library when allocated, to implement
   kernel-assisted revocation.
   On free, the memory is is held in quarantine until revocation has been
   performed.
   *(Not in this exercise.)*

5. To handle a further set of classes of misuse and pointer corruption, it is
   also important to perform validation of arguments to `free()`, such as by
   checking that the pointer is to the first byte of a valid allocation.
   *(Not in this exercise.)*

This exercise asks you to extend a simplified memory allocator with CHERI
focusing only on (1) and (3) above.
It supports only small fixed-size allocations that will not require further
alignment or padding, and we will not consider temporal safety in this
exercise.

The complete exercise is embodied in `cheri-allocator.c`, including the
simplified allocator and also a `main()` routine that initializes and uses the
allocator.
`main()` allocates memory, and then overflows the allocation to corrupt
internal allocator metadata, leading to a crash.
Heap metadata corruption is a powerful exploitation tool; CHERI assists with
mitigating it through pointer integrity features, but it is preferable to
deterministically close vulnlerabilities (e.g., via spatial safety).

1. Compile `cheri-allocator.c` with a CHERI-enabled target.
   Run the binary, which will crash.

2. Use GDB to demonstrate to yourself that the overflow has corrupted
   allocator metadata, leading to an eventual crash during a later call to
   `alloc_allocate()`.

3. Modify the allocator to use the `cheri_bounds_set()` API to set suitable
   bounds on the pointer returned by `alloc_allocate()`.
   Recompile `cheri-allocator.c` with a CHERI-enabled target.

4. Use GDB to demonstrate to yourself that the overflow operation now causes
   an immediate crash as a result of attempting to store out of bounds, rather
   than triggering a later crash due to heap metadata corruption.

5. Remove the overflow (performed with `memset()`) from the program.
   Recompile `cheri-allocator.c` with a CHERI-enabled target.

6. Use GDB to explore why the program now crashes in `alloc_free()`: How did
   adding bounds during allocation break later freeing of that memory?

7. Correct the bug through the use of the `cheri_address_get()` and
   `cheri_address_set()` APIs, which allow transfering an address from one
   capability (with one set of bounds) to another (with a different set of
   bounds).
   What capability should we be using to provide the new bounds?
   Recompile `cheri-allocator.c` with a CHERI-enabled target.

8. Demonstrate that the program now runs successfully to completion.

The resulting allocator is now substantially safer with respect to spatial
safety, preventing underflows and overflows from corrupting allocator metadata
or the contents of other allocations.
However, to continue hardening the allocator against various attacks, further
work would be required, including better validating the argument of the
`free()` function.
This would ideally test that the pointer being freed points to memory managed
by the allocator, that the pointer is in bounds, and that it points to the
start of a current allocation.
Further temporal safety also requires quarantining freed memory until all
pointers to it have been revoked.

## Source Files

**cheri-allocator.c**
```C
{{#include cheri-allocator.c}}
```
