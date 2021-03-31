# Introduction

This set of exercises and adversarial missions is intended to:

- Build a baseline skill set with unmodified instruction set
  architectures (ISAs) such as ARMv8 and RISC-V and the same
  baseline ISAs with CHERI extensions, as well as awareness
  of some of the dynamics of CHERI-enabled software, through skills development
  exercises.
- Develop adversarial experience with CHERI-enabled ISAs performing basic
  investigation around gradations of CHERI feature deployment through focused
  adversarial missions.

These activities supplement existing experience
with reverse engineering and exploitation on conventional architectures
and software stacks.

## Platform

These exercises are designed to be run on the CheriBSD operating system in its
pure-capability CheriABI process environment.
They can be run on various instantiations of CHERI-RISC-V, including on QEMU
and on FPGA implementations.
QEMU-CHERI is a convenient instruction-set-level emulator, and is usaully the
best starting point for most users (even those intending to eventually run on
hardware).
You can use our [cheribuild](https://github.com/CTSRD-CHERI/cheribuild) tool
to build the CHERI-RISC-V SDK, CheriBSD, and QEMU on macOS, FreeBSD, and Linux.

## Skills development exercises

**Skills development exercises** are intended to take 1-2 hours each,
and ask you to build and perform minor modifications to simple C/C++
programs. These exercises facilitate building skills such as
compiling, executing, and debugging programs with both CHERI-enabled
and default ISAs, as well as to build basic understanding of CHERI
C/C++ properties. We highlight some key edge cases in CHERI, including
the effects of bounds imprecision, subobject bounds, weaker temporal
safety, and C type confusion.

These exercises take for granted a strong existing understanding of:
- The C/C++ languages
- UNIX program compilation, execution, and debugging
- RISC ISAs (e.g. Armv8-A or RISC-V) as well as binary structures/reverse engineering

## Focused adversarial missions
**Focused adversarial missions** are intended to take
1-3 days, and ask you to exploit, first on an unmodified ISA, and
then on a CHERI-enabled ISA, documented vulnerabilities in simple "potted"
C/C++-language programs provided by the CHERI team. These missions
engage you more specifically in exploitation, and CHERI's
security objectives and mechanisms.

**These take for granted good existing experience with
memory-safety-related attack techniques, such as buffer overflows,
integer-pointer type confusion, Return-Oriented Programming (ROP), and
Jump-Oriented Programming (JOP).**

Successful exploitation of non-CHERI-enabled variants depends only
upon widely published understanding and techniques (e.g., buffer
overflows combined with ROP). For those familiar with conventional
low-level attack techniques, this will also act as a warm-up exercise
on the baseline architecture and expand experience with reverse
engineering and exploitation.

The CHERI team has confirmed exploitability for the non-CHERI-enabled
binaries in advance.  We strongly recommend exploiting the non-CHERI-enabled
version of the code first, as a starting point for
understanding potential exploitability of the CHERI version.
