# Manual execution

## Compiler and Sysroot Paths

We assume that you have followed the [prior instructions](./get.md) to obtain a
CHERI software stack and are, therefore, in possession of file paths to both the
`clang` compiler and a `sysroot` for your desired architecture.

⚠ At the moment this section provides instructions only for (CHERI-)RISC-V
targets.  Enthusiastic readers eager for DIY compilation on Morello can likely
study `tools/ccc` and extract what they are after in the interim.  We apologize
for the inconvenience. ⚠

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
### RISC-V CheriABI
For CheriABI, the architecture and ABI flags are:
`-march=rv64gcxcheri -mabi=l64pc128d`.
Putting it all together:
```
$CLANG -g -O2 -target riscv64-unknown-freebsd --sysroot="$SYSROOT" -fuse-ld=lld -mno-relax -march=rv64gcxcheri -mabi=l64pc128d -Wall -Wcheri
```

## Verifying Output ABI

CheriBSD supports running baseline and CHERI-enabled programs side-by-side on the same instance, so provided the instance has all features available for the exercise or mission in question, you should be able to complete it on a single CheriBSD instance.


### RISC-V

CheriBSD's `file(1)` has been extended to distinguish RISC-V binaries from CHERI-RISC-V (CheriABI) binaries. For example, on a CheriBSD instance:
```
# file riscv-binary
riscv-binary: ELF 64-bit LSB shared object, UCB RISC-V, RVC, double-float ABI, version 1 (SYSV), dynamically linked, interpreter /libexec/ld-elf.so.1, for FreeBSD 13.0 (1300097), FreeBSD-style, with debug_info, not stripped
# file cheri-binary
cheri-binary: ELF 64-bit LSB shared object, UCB RISC-V, RVC, double-float ABI, capability mode, CheriABI, version 1 (SYSV), dynamically linked, interpreter /libexec/ld-elf.so.1, for FreeBSD 13.0 (1300097), FreeBSD-style, with debug_info, not stripped
```

CHERI-LLVM and the elfutils in CheriBSD also recognise the relevant ELF flags. For example, CHERI-LLVM on the host used for cross-compiling will report:
```
# llvm-readelf -h riscv-binary | grep Flags
  Flags:                             0x5, RVC, double-float ABI
# llvm-readelf -h cheri-binary | grep Flags
  Flags:                             0x30005, RVC, double-float ABI, cheriabi, capability mode
```
and elfutils on a CheriBSD instance will report:
```
# readelf -h riscv-binary | grep Flags
  Flags:                             0x5, double-float ABI, RVC
# readelf -h cheri-binary | grep Flags
  Flags:                             0x30005, double-float ABI, RVC, cheriabi, capmode
```

## Next Steps

With all that in mind, proceed to read about [running CheriBSD](./run.md).
