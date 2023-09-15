# Introduction

This set of exercises and adversarial missions is intended to
build a baseline skillset with RISC-V and CHERI-RISC-V, as well as awareness
of some of the dynamics of CHERI-enabled software.

These activities supplement existing experience
with reverse engineering and exploitation on conventional architectures
and software stacks.

## Platform

The exercises are designed to be run on the CheriBSD operating system in its
pure-capability CheriABI process environment.
They can be run on various instantiations of CHERI-RISC-V, including on QEMU
and on FPGA implementations.
QEMU-CHERI is a convenient instruction-set-level emulator, and is usually the
best starting point for most users (even those intending to eventually run on
hardware).
For the SOSP CHERI tutorial, we provide pre-built [QEMU-CHERI binaries and, alternatively,
VM images](./introduction/exercise_vm_image.html).

## SOSP 2023 Exercises

The exercises ask you to build and perform minor modifications
to simple RISC-V and CHERI-RISC-V C/C++ programs. These exercises
facilitate building skills such as compiling, executing,
and debugging RISC-V and CHERI-RISC-V programs, as well as to build basic
understanding of CHERI C/C++ properties. We highlight some key edge
cases in CHERI, including the effects of bounds imprecision, and subobject
bounds.

These exercises take for granted a strong existing understanding of:
- The C/C++ languages
- UNIX program compilation, execution, and debugging
- RISC ISAs and binary structures/reverse engineering (e.g., on MIPS or ARMv8)
