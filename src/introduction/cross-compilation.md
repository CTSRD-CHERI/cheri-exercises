# Cross compilation

## Obtaining a compiler and sysroot

If you already have a compiler and sysroot (e.g. you have a docker image with pre-compiled versions), you will need to know the path to `clang` and the path to your sysroot.  You can then proceed to **Compiler command line**.

### Building a cross build environment with cheribuild

First, clone the cheribuild repo:
```
git clone https://github.com/CTSRD-CHERI/cheribuild.git
```
The [README.md](https://github.com/CTSRD-CHERI/cheribuild/blob/master/README.md) file contains considerable information, but to get started, you'll need to bootstrap an LLVM compiler and a CheriBSD build and sysroot.  The easiest path to doing this is:
```
cheribuild.py cheribsd-riscv64-purecap -d
```
This will churn away, prompting occasionally as it bootstraps assorted dependencies.  On a fast machine this will take several hours.
<!-- XXX: Should we advocate `-f` here? -->
Upon completion, you will find a usable Clang compiler in `~/cheri/output/sdk/bin/clang` and a sysroot in `~/cheri/output/rootfs-riscv64-purecap` (unless you have altered `cheribuild`'s default paths).

## Compiler command line
In this set of exercises we cross compile in two basic modes.
Conventional RISC-V ABI and the CheriABI pure-capability ABI.

### Common elements
All command lines will share some comment elements to target 64-bit RISC-V, select the linker, and indicate where to find the sysroot.

Some conventions:
 - `$SYSROOT` is the path to your sysroot.
 - `$CLANG` is the path to your compiler.
 - All compiler commands begin with `$CLANG -target riscv64-unknown-freebsd --sysroot="$SYSROOT" -fuse-ld=lld -mno-relax`
 - As a rule, you will want to add `-g` to the command line to compile with debug symbols.
 - You will generally want to compile with `-O2` as the unoptimized assembly is verbose and hard to follow.
 - We strongly recommend you compile with warnings on including `-Wall` and `-Wcheri`.

### RISC-V
Two additional arguments are required to specify the supported architectural features and ABI.  For conventional RISC-V, those are: `
-march=rv64gc -mabi=lp64d`.
Putting it all together:
```
$CLANG -g -O2 -target riscv64-unknown-freebsd --sysroot="$SYSROOT" -fuse-ld=lld -mno-relax -march=rv64gc -mabi=lp64d -Wall -Wcheri
```
### CheriABI
For CheriABI, the architecture and ABI flags are:
`-march=rv64gcxcheri -mabi=l64pc128d`.
Putting it all together:
```
$CLANG -g -O2 -target riscv64-unknown-freebsd --sysroot="$SYSROOT" -fuse-ld=lld -mno-relax -march=rv64gcxcheri -mabi=l64pc128d -Wall -Wcheri
```
