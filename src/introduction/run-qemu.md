# Running QEMU

This section may be optional!  If you are using the [University of Cambridge
CHERI Software Release](./get.md#ucam_dist), you are more likely to be better
served by [more targeted instructions](./run-qemu-release.md).  Other users of
this guide will have been provided environments for building and running CHERI
systems and programs.  If you have not, or if you also wish to be able to run
things locally or in emulation, building and using our CHERI-RISC-V QEMU
emulator is straightforward.

First, follow [the earlier instructions](./get.md#source) to obtain
`cheribuild`.  Then, it should suffice to run
```
~/cheri/cheribuild/cheribuild.py run-riscv64-purecap -d
```
After quite a lot of activity and some intermixed prompts about cloning
dependencies, you should find yourself running a CheriBSD system.  Log in as
`root` with no password.

In general, re-running that command will fetch any updates and do incremental
rebuilds of everything.  Leave off the `-d` if you just want to re-run the
existing build, and note that rebuilds rebuild the entire virtual disk image,
so don't leave anything you care about around inside qemu.

Please see [additional instructions for importing host
directories](./run-qemu-smb.md) for how to access things outside the disk image.
Then proceed to the first exercise,
[Compile and run baseline and CHERI-enabled
programs](../exercises/compile-and-run).
