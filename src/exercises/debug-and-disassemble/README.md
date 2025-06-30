# Disassemble and debug RISC-V and CHERI-RISC-V programs

This exercise steps you through disassembling and debugging
RISC-V and CHERI-RISC-V programs. It draws attention to differences in
program structure and code generation, particularly relating to control
flow, between the two compilation targets.

First, use `llvm-objdump` on the host (which you can find at
`~/cheri/output/cheri-alliance-sdk/bin/llvm-objdump`, unless you have altered `cheribuild`'s
default paths) to disassemble and explore the two binaries from the previous
exercise:

1. Using `llvm-objdump -dS`, disassemble the `print-pointer-riscv` and
   `print-pointer-cheri` binaries.
2. What instructions are generated to load `printf()`'s format string argument
   in `print-pointer-riscv`? Where does the target address for the string
   pointer originate?
3. What instructions are generated to load `printf()`'s format string argument
   in `print-pointer-cheri`? Where does the target capability for the string
   pointer originate? (Hint, you may find it helpful to add the `-s` flag to
   your `llvm-objdump` command to see all sections.)

Next use GDB to explore binary execution for RISC-V:

4. Run `print-pointer-riscv` under GDB (same path as llvm-objdump),
   setting a breakpoint at the start of `printf()`.
   *Note:* You will need to run QEMU in halted mode and make it wait for GDB
   connections, then, from another shell, run GDB on the ELF, connect to QEMU,
   then debug. An example sequence of commands will look like the following:
```
# run_qemu print-pointer-riscv -s -S

# gdb print-pointer-riscv
Reading symbols from print-pointer-riscv...
(gdb) target remote :1234
Remote debugging using :1234
0x0000000000001000 in ?? ()
(gdb) break printf_
Breakpoint 1 at 0x200090: file src/printf.c, line 1151.
(gdb) c
Breakpoint 1, printf_ (format=0x201f5d "size of pointer: %zu\n") at src/printf.c:1151
```
   <!-- This might want to go in the introductory material -->
5. Run the program and at the breakpoint, print out the value of the
   string pointer argument.
6. Print out the program counter (`info reg pc`).
   What memory mapping is it derived from?

And for CHERI-RISC-V:

7. Run `print-pointer-cheri` under GDB, setting a breakpoint at the start
   of `printf()`.
8. Print out the value of the string pointer argument.
9. Print out the program counter (`info reg pcc`).
    Where do its bounds appear to originate from?
10. Print out the register file using `info registers`.
   What mappings do the capabilities in the register file point to?
   Notice that some capabilities have `S` in their permissions which means they
   are sentries. Sentry capabilities are sealed (cannot be modified or used to
   load or store), but can be used as a jump target (where they are unsealed and
   installed in `pcc`).
   What implications does this have for attackers?
