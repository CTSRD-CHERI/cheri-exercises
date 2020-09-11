# Introduction

This set of exercises and adversarial missions is intended to:

- Build a baseline skillset with RISC-V and CHERI-RISC-V, as well as awareness
  of some of the dynamics of CHERI-enabled software, through skills development
  exercises.
- Develop adversarial experience with CHERI-RISC-V performing basic
  investigation around gradations of CHERI feature deployment through focused
  adversarial missions.

These activities supplement existing experience
with reverse engineering and exploitation on conventional architectures
and software stacks.

## Skills development exercises

**Skills development exercises** are intended to take 1-2 hours each,
and ask you to build and perform minor modifications to simple
RISC-V and CHERI-RISC-V C/C++ programs. These exercises
facilitate building skills such as compiling, executing,
and debugging RISC-V and CHERI-RISC-V programs, as well as to build basic
understanding of CHERI C/C++ properties. We highlight some key edge
cases in CHERI, including the effects of bounds imprecision, subobject
bounds, weaker temporal safety, and C type confusion.

These exercises take for granted a strong existing understanding of:
- The C/C++ languages
- UNIX program compilation, execution, and debugging
- RISC ISAs and binary structures/reverse engineering (e.g., on MIPS or ARMv8)

## Focused adversarial missions
**Focused adversarial missions** are intended to take
1-3 days, and ask you to exploit, first on RISC-V, and
then on CHERI-RISC-V, documented vulnerabilities in simple "potted"
C/C++-language programs provided by the CHERI-RISC-V team. These missions
engage you more specifically in RISC-V exploitation, and CHERI's
security objectives and mechanisms.

**These take for granted good existing experience with
memory-safety-related attack techniques, such as buffer overflows,
integer-pointer type confusion, Return-Oriented Programming (ROP), and
Jump-Oriented Programming (JOP).**

Successful exploitation of RISC-V variants depends only upon
widely published understanding and techniques (e.g., buffer overflows
combined with ROP). For those familiar with conventional low-level
attack techniques, this will also act as a warm-up exercise on the
baseline RISC-V architecture and expand experience with RISC-V reverse
engineering and exploitation.

The CHERI-RISC-V team has confirmed exploitability for the RISC-V binary
in advance.  We strongly recommend exploiting the RISC-V version of the code
first, as a starting point for understanding potential CHERI-RISC-V
exploitability.
