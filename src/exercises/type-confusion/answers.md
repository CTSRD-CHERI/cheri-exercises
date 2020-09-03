# Answers: Exercise integer-pointer type confusion bug

When the integer value is updated, with CHERI-RISC-V compilation the
pointer side will no longer be dereferenceable, as the tag has been cleared.

2. Expected output:
```
# ./union-int-ptr-riscv
lp.ptr Hello World!
lp.ptr ello World!
```
The `long` member was loaded and stored as an integer (this is identical
to the way it would have been handled if the pointer member were
incremented instead).

3. Expected output:
```
# ./union-int-ptr-cheri
lp.ptr Hello World!
In-address space security exception (core dumped)
```
When the `long` member was loaded and stored, it caused the tag to be
cleared on the pointer.
