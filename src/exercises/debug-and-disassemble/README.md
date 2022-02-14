# Disassemble and debug baseline and CHERI-enabled programs

This exercise steps you through disassembling and debugging
baseline and CHERI-enabled programs. It draws attention to differences in
program structure and code generation, particularly relating to control
flow, between the two compilation targets.

## Disassembly Exercise

1. Using the mechanism of your choice from
   [disassembling CHERI programs](../../introduction/inspect-disassemble.md),
   obtain the instruction listing for `print-pointer-baseline` and
   `print-pointer-cheri` binaries.

2. What jump instruction is used to call `printf()` in
   `print-pointer-baseline`?  Where does the target address for that jump
   originate?

3. What jump instruction is used to call `printf()` in `print-pointer-cheri`?
   Where does the target capability for that jump originate?
   (Hint, you may find it helpful to add the `-s` flag to your
   `llvm-objdump` command to see all sections.)

## Using The GDB Debugger

Next, we will use `gdb` *in CheriBSD* to explore binary execution.  If you are
not using the [University of Cambridge CHERI Software
Release](../../introductions/get.md#using-the-cheri-software-release), please
see [the instructions for building
gdb](../../introduction/inspect-debug.md#building-gdb).  You may also wish to
read our [notes for running
gdb](../../introduction/inspect-debug.md#running-gdb), but the instructions
here suffice.

Begin by considering baseline executables:

4. Run `print-pointer-baseline` under GDB, setting a breakpoint at the start
   of `printf()`.
   *Note:* GDB can't find the run-time linker of binaries of the
   non-default ABI on its own so you need to invoke
   `set program-interpreter /libexec/ld-elf64.so.1`
   before running the program.
   <!-- This might want to go in the introductory material -->

   That is, run, in the CheriBSD emulated environment,
```
# gdb /mnt/print-pointer-baseline
(gdb) set program-interpreter /libexec/ld-elf64.so.1
(gdb) b printf
(gdb) r
```

5. Run the program and at the breakpoint, print out the value of the
   string pointer argument with `bt -full`.

6. Use `info proc mappings` in GDB to print out the layout of the
   process address space.

7. Print out the program counter (`info reg pc`).
   What memory mapping is it derived from?

And for CHERI executables:

8. Run `print-pointer-cheri` under GDB, setting a breakpoint at the start
   of `printf()`.

9. Print out the value of the string pointer argument.

10. Use `info proc mappings` (in GDB) to print out the layout of the
   process address space.

11. Print out the program counter (`info reg pcc`).
   What memory mapping is it derived from?
   Where do its bounds appear to originate from?

12. Print out the register file using `info registers`.
   What mappings do the capabilities in the register file point to?
   Notice that some capabilities are labeled with `(sentry)` (or `(sealed)` in
   the case of older versions of GDB which do not distinguish sentries from
   other sealed capabilities).
   Sentry capabilities are sealed (cannot be modified or used to load or
   store), but can be used as a jump target (where they are unsealed and
   installed in `pcc`).
   What implications does this have for attackers?
