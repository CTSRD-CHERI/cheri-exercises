# Skills Development Exercises

For a researcher to contribute effectively to CHERI evaluation,
they will need a baseline skill-set that includes significant existing
experience with:

- C/C++-language memory-safety vulnerabilities
- Binary reverse engineering for at least one ISA, such as x86, MIPS, ARMv7, or ARMv8
- Low-level aspects of program representation, such as ELF, GOTs, and PLTs, as well as mechanisms such as dynamic linking and system-call handling
- Attack techniques against program control flow and underlying data structures including ROP and JOP

However, we expect that researchers may need to build specific
additional skills with respect to the specifics of RISC-V or Morello
machine code, assembly, language, and linkage, as well as knowledge
about the CHERI C/C++ protection model and CHERI-RISC-V and Morello
extensions to their base ISAs. These exercises are intended to assist in these
latter two areas, faulting in missing knowledge and experience while
building on existing skills gained on other architectures (such as
x86-64 and ARMv8). Participants successfully completing these
exercises will be able to:
- Compile, run, disassemble, and debug RISC-V or aarch64 compiled C/C++ programs
- Compile, run, disassemble, and debug CHERI-RISC-V or Morello compiled C/C++ programs
- Use specific debugging tools such as GDB and llvm-objdump with
  RISC-V and CHERI-RISC-V as well as Morello programs
- Understand some of the implications of CHERI protections for specific aspects of C/C++ and process execution

Each exercise includes:
- Sample source code and build instructions
- A short document describing what the program does and the objectives
- Where there are exercise questions, sample answers
