# Disassemble and debug RISC-V and CHERI-RISC-V programs

This exercise steps a participant through disassembling and debugging RISC-V and CHERI-RISC-V programs. It draws attention to differences in program structure and code generation, particularly relating to control flow, between the two compilation targets.

First, we use `llvm-objdump` to disassemble and explore the two binaries from the previous exercise:

1. Using `llvm-objdump -dS`, disassemble the `print-pointer-riscv` and `print-pointer-cheri` binaries.
2. What jump instruction is used to call `printf()` in `print-pointer-riscv`?
   Where does the target address for that jump originate?
3. What jump instruction is used to call `printf()` in `print-pointer-cheri`?
   Where does the target capability for that jump originate?
   (Hint, you may find it helpful to add the `-s` flag to your `llvm-objdump` command to see all sections.)

Next we use GDB to explore binary execution for RISC-V:

4. Run `print-pointer-riscv` under GDB, setting a breakpoint at the start of `printf()`.
   *Note:* GDB can't find the run-time linker of binaries of the non-default ABI on its own so you need to invoke `set program-interpreter /libexec/ld-elf64.so.1 ` before running the program. <!-- This might want to go in the introductory material -->
5. Run the program and at the breakpoint, print out the value of the string pointer argument.
6. Use `info proc mappings` in GDB to print out the layout of the process address space.
7. Print out the program counter (`info reg pc`).
   What memory mapping is it derived from?

And for CHERI-RISC-V:

8. Run `print-pointer-cheri` under GDB, setting a breakpoint at the start of `printf()`.
9. Print out the value of the string pointer argument.
10. Use `info proc mappings` (in GDB) to print out the layout of the process address space.
11. Print out the program counter (`info reg pcc`).
    What memory mapping is it derived from?
    Where do its bounds appear to originate from?
12. Print out the register file using `info registers`.
    What mappings do the capabilities in the register file point to?
    Notice that some capabilities are labeled with `(sentry)`. Sentry capabilities are sealed (cannot be modified or used to load or store), but can be used as a jump target (where they are unsealed and installed in `pcc`). What implications does this have for attackers?
