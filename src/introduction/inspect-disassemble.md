# Disassembling CHERI Programs

Throughout this book, it will be occasionally useful to inspect generated
machine instructions and, especially, how they compare and contrast between our
compilation targets.  We are in a relatively privileged position as we have the
source to these programs as well as their binaries, and so we have (at least)
two different ways to generate human-readable listings of instructions:

- The traditional approach, of using a *disassembler* to extract the program
  text from the binary: follow the [instructions for
  llvm-objdump](#with-llvm-objdump).

- Asking the *compiler* to *generate* the listing instead of the program
  binary.  This has the advantage of also revealing a little bit more of the
  compiler's state in the way of additional annotations.  Follow the
  [instructions for clang](#with-clang).

- Again, since we have the source, the [compiler-explorer
  tool](https://github.com/compiler-explorer/compiler-explorer), can give us
  nicely annotated dumps of assembler.  You are welcome to run this locally, but
  the CHERI group at the University of Cambridge Computer Lab also runs a
  [publicly available instance](https://cheri-compiler-explorer.cl.cam.ac.uk/)
  with up-to-date compilers. (This instance does not permit *executing* the
  generated code, so you will still want to have your own compiler, too).  For
  LLVM targets, adding `-emit-llvm` to the compiler arguments will show,
  instead, the LLVM Intermediate Representation (IR).

<!-- Name is known in-book -->
## With llvm-objdump

 `llvm-objdump` on the *host* (that is, outside the CHERI
emulation).  The location of the appropriate version of this tool depends on
your choice of target.  If you are using `aarch64` and `morello` targets,
then...

- If you have built the CHERI software stack from source, you can find this at
  `~/cheri/output/morello-sdk/bin/llvm-objdump`, unless you have altered
  `cheribuild`'s default paths.

- If you are using the [CheriBSD Software
  Release](https://cheri-dist.cl.cam.ac.uk/), you can find this at
  `/output/morello-sdk/bin/llvm-objdump`.

For other CHERI targets,

- If you have built the CHERI software stack from source, you can find this at
  `~/cheri/output/sdk/bin/llvm-objdump`, unless you have altered `cheribuild`'s
  default paths.

- If you are using the [CheriBSD Software
  Release](https://cheri-dist.cl.cam.ac.uk/), you can find this at
 `/output/sdk/bin/llvm-objdump` inside the `docker` container.

Having found `llvm-objdump`, we can use it to disassemble and explore the two
binaries from [the previous exercise](../compile-and-run).  Running
`llvm-objdump` with `-dS` options will cause it to disassemble and intermix
source lines as DWARF debugging information permits.  To simplify side-by-side
comparison between multiple binaries, the `--no-leading-addr` option may also
be useful.  The `--no-show-raw-insn` option may also reduce visual clutter by
suppressing the printout of instruction bytes, leaving only mnemonics.

<!-- Name is known above -->
## With clang

Adding `-S` to `clang`'s command line will cause it to generate an assembler
listing rather than a binary.  This remains true for our [ccc
wrapper](../../introduction/cross-ccc.md) as well.  As usual, `-o` will direct
the output to a file of your choosing; you probably do not wish to clobber the
binary (lest, having done so, you find yourself quite confused the next time
you try to run the program), so instead have it write to `...-baseline.s` and
`...-cheri.s` files.

You can additionally pass `-emit-llvm` to have `clang` dump not the assembler
but the LLVM Intermediate Representation (IR) that would have been handed to the
backend (that is, as emitted by the compiler frontend and processed by the
driver's optimization passes).
