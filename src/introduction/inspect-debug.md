# Debugging CHERI Programs

We have forked `gdb` and taught it to understand CHERI programs.

<!-- Name is known in-book -->
## Building gdb

If you are using the [University of Cambridge CHERI Software
Release](./get.md#using-the-cheri-software-release), these steps have already
been taken.

### For the CheriBSD system

It is, generally, much more convenient to run `gdb` *inside* the CheriBSD
environment, and so the CHERI-aware fork of `gdb` can be built as a baseline
executable and installed in the CHERI-enabled disk images using the `cheribuild`
target `gdb-riscv64-hybrid-for-purecap-rootfs` (or
`gdb-morello-hybrid-for-purecap-rootfs` &c).  Rebuild the disk image afterwards
to ensure that the binaries are picked up.

### For the host system

Sometimes it is useful to have a *host* CHERI-aware `gdb` as well.  In
particular, `qemu` implements its own `gdbstub` and can offer a reasonable path
for debugging system software such as kernels.  Similarly, many FPGA workflows
bridge JTAG and `gdb`.  Use `cheribuild` 

<!-- Name is known in-book -->
## Running GDB

There is very little unique to CHERI here; most everything about `gdb` remains
as it was.

If you are debugging *baseline* architecture programs in a *default-CHERI*
(`purecap`) disk image, then `gdb` will be slightly confused, as it cannot, by
itself, find the run-time linker of binaries of the non-default ABI on its own
so you need to invoke `set program-interpreter /libexec/ld-elf64.so.1` before
running the program.

Our disk images include a `gdb-run.sh` that performs some minimal, but
convenient, alteration to a bare `gdb --args $PROG` invocation.  In particular,
it smooths out the need to interact between two potentially slow parts of
debugging -- running the program and loading and parsing the symbol tables -- by
automatically running `sharedlibaries` and `thread apply all bt 5` the first
time the program stops and `gdb` returns to a prompt.  (The above caveat about
non-default ABIs means that `gdb-run.sh` is really useful only for CHERI-aware
programs.)
