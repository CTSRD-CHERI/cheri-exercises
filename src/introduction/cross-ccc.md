# Helper script

Because the command line required to compile exercises is quite unwieldy, we've created a wrapper script to help out.  If you've checked out this repository it's present in `tools/ccc`; it is also shown [below](#the-ccc-script-itself).  The usage is:
```
ccc <arch> [...]

Supported architectures:
	aarch64         - conventional AArch64
	morello-hybrid  - AArch64 Morello supporting CHERI
	morello-purecap - AArch64 Morello pure-capability
	riscv64         - conventional RISC-V 64-bit
	riscv64-hybrid  - RISC-V 64-bit supporting CHERI
	riscv64-purecap - RISC-V 64-bit pure-capability
```
and, other than the mandatory target architecture argument, it can be used as if it were a traditional `cc`.

For the exercises in this book you will use either
* the `riscv64` (baseline) and `riscv64-purecap` (CHERI-enabled) architectures or
* the `aarch64` (baseline) and `morello-purecap` (CHERI-enabled) architectures.
Do not use the hybrid compilation targets (`-hybrid`) for these exercises.

If you...
* are using the CHERI Software Release, there are [targeted instructions](./cross-ccc-release.md) for preparing `ccc`.
* have been provided with a pre-configured runtime environment, `./tools/ccc` should work out of the box.
* built a compiler and sysroot using `cheribuild` with the default configuration (which generates `~/cheri/output`), `./tools/ccc` should work out of the box.
* configured a different location, you must set the `CHERIBUILD_OUTPUT` environment variable to point to to the location of your `cheribuild` output directory, which contains the SDK(s).
* are doing something fancy, you may set `CHERIBUILD_SDK` to point to an SDK directly or be even more specific and set the `CLANG` and `SYSROOT` variables to point to the respective locations of a pre-built compiler and sysroot.

Although not used by these exercises, the tool will instead function as a C++ compiler if invoked via the name `cc++`, and a `tools/cc++` symlink exists to facilitate this.

With all that in mind, proceed to read about [running CheriBSD](./run.md).

<!-- Name is known above -->
## The ccc script itself

```sh
{{#include ../../tools/ccc}}
```
