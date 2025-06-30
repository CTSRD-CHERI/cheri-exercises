# Answers: Exercise integer-pointer type confusion bug

When the integer value is updated, with CHERI-RISC-V compilation the
pointer side will no longer be dereferenceable, as the tag has been cleared.

2. Expected output:
```
# run_qemu ./type-confusion-riscv
lp.ptr Hello World!
lp.ptr ello World!
```
The `long` member was loaded and stored as an integer (this is identical
to the way it would have been handled if the pointer member were
incremented instead).

3. Expected output:
```
# run_qemu ./type-confusion-cheri
lp.ptr Hello World!
...
MON|ERROR: CHERI Security Violation: ip=0x0000000000200750  fault_addr=0x00000000002023f1  fsr=0x0000000000000810  (data fault)
MON|ERROR: description of fault: Tag violation
MON|ERROR: CHERI fault type: CHERI data fault due to load, store or AMO
```
When the `long` member was loaded and stored, it caused the tag to be
cleared on the pointer.
