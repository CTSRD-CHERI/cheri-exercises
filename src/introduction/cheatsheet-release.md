# Cheatsheet for the CHERI Software Release

This is meant to be a quick summary of useful commands for readers using [the
University of Cambridge CHERI Software
Release](./get.md#using-the-cheri-software-release).

## Starting a Shell Within the Docker Container

From within `~/cheri` (or equivalent location if you unpacked the release
somewhere other than `~`), run
```
./cheribuild/docker-cheribuild.py sdk-shell
```
to get a shell.  You probably want to run
```
exec /bin/bash
```
to get a slightly friendlier shell, then
```
cd /source/cheri-exercises
```
to sit among the exercise source files, and then
```
export CHERIBUILD_OUTPUT=/output
```
to prepare the environment for `ccc`.

[Additional discussion for this shell environment](./cross-ccc-release.md) is
available.

## Running the Compiler Within the Docker Container

Fill in `$prog` and `$csource` as apporpriate and run, in the shell from above,
```
/source/cheri-exercises/tools/ccc riscv64 -o /build/$prog-baseline ./src/exercises/$csource.c
/source/cheri-exercises/tools/ccc riscv64-purecap -o /build/$prog-cheri ./src/exercises/$csource.c
```

[Additional discussion about cross-compilation](./crosc-ccc.md) is available.

## Starting Qemu

To run the CheriBSD system, on the *host*, not in Qemu, run
```
./cheribuild/docker-cheribuild.py run-riscv64-purecap
```
Log in as `root` (no password is required).

[Additional instructions for running qemu in the release
environment](./run-qemu-release.md) are available.

## Mounting SMB Within Qemu

```
mkdir -p /buildroot && mount_smbfs -I 10.0.2.4 -N //10.0.2.4/build_root /buildroot
```
After this, you can run the compiled programs with, for example,
```
/buildroot/$prog-baseline
```

[Additional instructions for importing host directories](./run-qemu-smb.md) are
available.
