# Demonstrate pointer revocation
## Indirect control flow through aliased heap objects
This exercise demonstrates CheriBSD's *pointer revocation* facility and its use
by the system `malloc`. It asks the participant to contrast the same program,
`temporal-control.c`, built and run in three slightly different environments.
It must be run on a **heap-temporal-safety enabled** version of CheriBSD; at the
time of writing, heap temporal safety remains an experimental feature not yet
merged to mainline CheriBSD.

1. Compile `temporal-control.c` with a RISC-V target and a binary name of
`temporal-control-riscv`.

**temporal-control.c**
```C
{{#include temporal-control.c}}
```
2. Run the resulting program and observe that the system malloc has **reused**
a location on the heap, such that `obj1` and `obj2` point to the same address.
Moreover, the assignment of `fn2` into `obj2` causes the last printout to be
from `fn2`, not `fn1`, even though the function pointer was fetched through
`obj1` and `obj1->fn` was last set to `fn1`.
3. Recompile `temporal-control.c` with a CHERI-RISC-V target and binary name of
`temporal-control-cheri`.
4. Run this program instead. Why does it no longer exhibit the behavior from
step 2? Ponder the suitability of using just this approach for fixing temporal
aliasing.
5. Recompile `temporal-control.c`, adding `-DCAPREVOKE` to the command line
this time, with a CHERI-RISC-V target and a binary name of
`temporal-control-cheri-revoke`.
6. Run this third program instead and note that it crashes, catching a
`SIGPROT` between declaring its intent to call `obj1->fn` and declaring that it
has made the call. Can you spot why it has crashed?
7. Rerun the third program under `gdb` and look at both the instruction
triggering the `SIGPROT` and the register(s) involved. Why is the program
crashing? What must have happened while the system was executing the
mysterious `malloc_revoke()` function?
8. Modify `temporal-control.c` to try to induce aliasing by making many
allocations: call `malloc` and `free` repeatedly until the new allocation
compares equal to `obj1`. Ah ha, you've caught the allocator now! But wait,
what is `obj1` in full (i.e., as a capability, not merely a virtual address)?
You likely have to call `free` in the loop for this exercise to work; merely
calling `malloc` may instead simply always return new addresses, even if the
initial `obj1` has been `free`-d.

## More attacks through aliased heap objects
The program is called `temporal-control.c` because it exhibits *temporal
aliasing* of heap pointers and because the class of bugs it mimics involve
transfers of control through function pointers held in heap objects. While
CHERI protects against pointer *injection*, it cannot so easily defend against
either:

* *capability farming*: as in the example, a legitimately-held capability can
be (caused to be) stored to a "new" heap object, altering an aliased view while
preserving the set tag bit; or
* *data-based* corruption through temporal aliasing.

These windows open wider considering that, unlike this example, temporal
aliasing often comes paired with *type-confusion*, so it may be possible to
overlap an easily-controlled structure with an exploitable one.

1. Write a program like `temporal-control.c` in which changing a *data byte*
within a temporally-aliased heap object suffices to cause the program to error.
Perhaps the heap object is the state associated with a client session and
contains a flag that indicates superuser status.
2. Demonstrate that this program fails as expected on RISC-V but that any
attempt to induce aliasing is thwarted on CHERI-RISC-V with heap temporal
safety: aliasing becomes possible only after revocation, ensuring that attempts
to use the old session object fail-stop.
