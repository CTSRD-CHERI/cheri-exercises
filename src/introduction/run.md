# Running CHERI Programs

While CHERI-enabled processors have been realized in FPGA (e.g., the several
CHERI-RISC-V cores) and in ASIC silicon (Morello), the primary CHERI-enabled
software execution environments are still, for the moment, emulation.

If you are compiling for CHERI-RISC-V, then see our [instructions for using
QEMU](introduction/run-qemu.md).

If you are compiling for Morello, then you have a choice between QEMU and Arm's
Morello [Fixed Virtual
Platform](https://developer.arm.com/tools-and-software/open-source-software/arm-platforms-software/arm-ecosystem-fvps),
which offers are more accurate simulation of the Morello hardware and is more
extensively tested than the Morello-hosting `qemu`.  The cost for this increased
accuracy is, alas, a significant slowdown; however, especially in adversarial
contexts, the increased fidelity can help distinguish real platform issues from
bugs in the emulator.  Please see either
- [instructions for using QEMU](introduction/run-qemu.md) or
- [instructions for using the Morello FVP](introduction/run-fvp.md).
