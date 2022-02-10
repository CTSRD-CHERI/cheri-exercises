# Disassemble and debug baseline and CHERI-enabled programs

This exercise steps you through disassembling and debugging
baseline and CHERI-enabled programs. It draws attention to differences in
program structure and code generation, particularly relating to control
flow, between the two compilation targets.

## Investigating The Generated Instructions

We will first be looking at the generated machine instructions and how they
compare and contrast between our compilation targets.  We are in a relatively
privileged position as we have the source to these programs as well as their
binaries, and so we have (at least) two different ways to generate
human-readable listings of instructions:

- The traditional approach, of using a *disassembler* to extract the program
  text from the binary: follow the [instructions for
  llvm-objdump](#asm-objdump).

- Asking the *compiler* to *generate* the listing instead of the program
  binary.  This has the advantage of also revealing a little bit more of the
  compiler's state in the way of additional annotations.  Follow the
  [instructions for clang](#asm-clang).

### <a id="#asm-objdump">With llvm-objdump

 `llvm-objdump` on the *host* (that is, outside the CHERI
emulation).  The location of the appropriate version of this tool depends on
your choice of target.  If you are using `aarch64` and `morello` targets,
then...

- If you have built the CHERI software stack from source, you can
find this at `~/cheri/output/morello-sdk/bin/llvm-objdump`, unless you have altered
`cheribuild`'s default paths.

- If you are using the [CheriBSD Software
  Release](https://cheri-dist.cl.cam.ac.uk/), you can find this at
`/output/morello-sdk/bin/llvm-objdump`.

For other CHERI targets,

- If you have built the CHERI software stack from source, you can
find this at `~/cheri/output/sdk/bin/llvm-objdump`, unless you have altered
`cheribuild`'s default paths.

- If you are using the [CheriBSD Software
  Release](https://cheri-dist.cl.cam.ac.uk/), you can find this at
`/output/sdk/bin/llvm-objdump`.

Having found `llvm-objdump`, we can use it to disassemble and explore the two
binaries from [the previous exercise](../compile-and-run).  Running
`llvm-objdump` with `-dS` options will cause it to disassemble and intermix
source lines as DWARF debugging information permits.  Proceed to [the exercise
proper](#asm-exercise).

### <a id="#asm-clang">With clang

Adding `-S` to `clang`'s command line will cause it to generate an assembler
listing rather than a binary.  This remains true for our [ccc
wrapper](../../introduction/cross-ccc.md) as well.  As usual, `-o` will direct
the output to a file of your choosing; you probably do not wish to clobber the
binary (lest, having done so, you find yourself quite confused the next time
you try to run the program), so instead have it write to `...-baseline.s` and
`...-cheri.s` files.

Now continue to [the exercise proper](#asm-exercise).

### <a id="#asm-exercise">Exercise

1. Using the mechanism of your choice, obtain the instruction listing for
   `print-pointer-baseline` and `print-pointer-cheri` binaries.

2. What jump instruction is used to call `printf()` in
   `print-pointer-baseline`?  Where does the target address for that jump
   originate?

3. What jump instruction is used to call `printf()` in `print-pointer-cheri`?
   Where does the target capability for that jump originate?
   (Hint, you may find it helpful to add the `-s` flag to your
   `llvm-objdump` command to see all sections.)

## Using The GDB Debugger

Next, we will use `gdb` *in CheriBSD* to explore binary execution.  Begin by
considering baseline executables:

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
