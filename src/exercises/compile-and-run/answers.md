# Answers - Compile and run RISC-V and CHERI-RISC-V programs

This exercise highlights the difference in size between addresses and pointers to draw attention to the pointer-focused nature of CHERI memory protection.

2. Expected output:
```
# ./print-pointer-riscv 
size of pointer: 8
size of address: 8
# ./print-pointer-cheri 
size of pointer: 16
size of address: 8
```
4. Expected output:
```
# ./print-capability 
cap length: 4
```
6. Expected output:
```
# ./print-capability
cap length: 16
```
7. Possible solution:
```diff
--- print-capability.c.orig
+++ print-capability.c
@@ -4,8 +4,8 @@
 int
 main(void)
 {
-       int i;
-       void *cap = &i;
+       char *c;
+       void *cap = &c;
 
        printf("cap length: %lu\n", cheri_length_get(cap));
 
```
8. Expected output:
```
# ./print-capability
cap length: 16
```
