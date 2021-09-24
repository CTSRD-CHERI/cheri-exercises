# Answers - Compile and run baseline and CHERI-enabled programs

This exercise explores the difference in size between addresses and
pointers, drawing attention to the pointer-focused nature of CHERI memory
protection.  Note that whenever a binary name changes based on the underlying
ISA the output for each ISA is shown.

2. Expected output:

Baseline RISC-V binary:
```
# ./print-pointer-riscv
size of pointer: 8
size of address: 8
```

Baseline ARMv8 binary:
```
# ./print-pointer-morello-aarch64
size of pointer: 8
size of address: 8
```

The output for each of the following answers is the same irrespective
of the the CHERI-enabled ISA being used.

4. Expected output:
```
# ./print-pointer-cheri
size of pointer: 16
size of address: 8
```
6. Expected output:
```
# ./print-capability
cap to int length: 4
cap to cap length: 16
```
