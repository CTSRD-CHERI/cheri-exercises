# Running QEMU

This section is optional; some users of this guide will have been provided
environments for building and running CHERI systems and programs.  If you have
not, or if you also wish to be able to run things locally or in emulation,
building and using our CHERI-RISC-V QEMU emulator is straightforward.

First, follow the instructions in [Cross compilation](cross-compilation.md) to
obtain `cheribuild`.  Then, it should suffice to run
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

In general, we use samba to bring parts of the host filesystem into the guest,
so run, in the guest,
```
mount_smbfs -I 10.0.2.4 -N //10.0.2.4/source_root /mnt
```
to get the host's `~/cheri` at the guest's `/mnt`.  Programs can be executed
directly over the smb mount, but core dumping over smb can be quite slow.  We
therefore recommend either disabling coredumps, with
```
sysctl kern.coredump=0
```
or copying programs into `/tmp` before running them.
