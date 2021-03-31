# Helper script

Because the command line required to compile exercises is quite unwieldy, we've created a wrapper script to help out, shown below.  If you've checked out this repository it's present in `tools/ccc`.  The usage is:
```
ccc <arch> [...]

Supported architectures:
	aarch64         - conventional AArch64
	morello-purecap - AArch64 Morello pure-capability
	riscv64         - conventional RISC-V 64-bit
	riscv64-purecap - RISC-V 64-bit pure-capability
```
and it can be used in place of your compiler.

For the exercises in this book you will use either the `riscv64` and `riscv64-purecap` architectures or the `aarch64` and `morello-purecap` architectures.

If you have built a compiler and sysroot using `cheribuild` in the default location (`~/cheri`) then it should work out of the box.  If you've configured a different location you must set the `CHERIBUILD_SDK` environment variable to point to to the location of your SDK.  Alternatively, you can set the `CLANG` and `SYSROOT` variables to point to the respective locations or your pre-built compiler and sysroot.
```sh
{{#include ../../tools/ccc}}
```

If you were provided a docker image along with these instructions (e.g. as part of a training exercise or bug-bounty), it should be configured such that `ccc` works without setting environment variables.

Although not used by these exercises, the tool will instead function as a C++ compiler if invoked via the name `cc++`, and a `tools/cc++` symlink exists to facilitate this.
