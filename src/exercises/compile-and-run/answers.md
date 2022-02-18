# Answers - Compile and run baseline and CHERI-enabled programs

This exercise explores the difference in size between addresses and
pointers, drawing attention to the pointer-focused nature of CHERI memory
protection.

2. Expected output:

   ```
   # ./print-pointer-baseline
   size of pointer: 8
   size of address: 8
   ```

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
