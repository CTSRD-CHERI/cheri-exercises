# Helper script

Because the command line required to compile exercises is quite unwieldy, we've created a wrapper script to help out, shown below.  If you've checked out this repository it's present in `tools/ccc`.  The usage is:
```
ccc <arch> [...]

Supported architectures:
	riscv64		- conventional riscv64
	riscv64-hybrid	- RISC-V 64-bit supporting CHERI
	riscv64-purecap	- RISC-V 64-bit pure-capability
```
and it can be used in place of your compiler.  If you have built a compiler and sysroot using `cheribuild` in the default location (`~/cheri`) then it should work out of the box.  If you've configured a different location you can set the `CHERIBUILD_SDK` environmental variable to point to to the location of your SDK.  Alternatively, you can set the `CLANG` and `SYSROOT` variables to point to the respective locations.
```sh
{{#include ccc.sh}}
```
