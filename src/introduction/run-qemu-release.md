# Using QEMU with the CHERI Software Release

## Run CheriBSD Under QEMU Under Docker

Run
```
./cheribuild/docker-cheribuild.py run-riscv64-purecap
```
or
```
./cheribuild/docker-cheribuild.py run-morello-purecap
```
depending on your choice of baseline ISA.  After the emulated machine boots, log
in as `root`; no password is required.

## SMB Mount The Build Directory

Recalling [the cross-compilation instructions](./cross-ccc-release.md), you will
want to have access to the docker environment's `/build` directory so that you
can run compiled programs.  In the QEMU-emulated CheriBSD machine, run
```
mkdir -p /buildroot && mount_smbfs -I 10.0.2.4 -N //10.0.2.4/build_root /buildroot
```
If curious, feel free to read [additional instructions for importing host
directories](./run-qemu-smb.md).
Then proceed to the first exercise,
[Compile and run baseline and CHERI-enabled
programs](../exercises/compile-and-run).

<!-- Name is known in-book -->
## In Case Of Emergency: Rebuilding the Disk Image

If something goes awry, you may find it necessary to [rebuild the disk
image](./run-build-disk.md) for the emulated system.  Be aware that this is a
"revert to factory settings", and all changes made to the emulated system will
be undone.  Probably best to not be running qemu while doing this.
```
./cheribuild/docker-cheribuild.py disk-image-riscv64-purecap disk-image-morello-purecap
```

### Release Erratum

The Summer 2021 edition of the [University of Cambridge CHERI Software
Release](./get.md#using-the-cheri-software-release) did not ship with all the
requisite tooling to make disk images.  There should be an erratum notice and a
suitable additional tarball to download, which will provide the requisite
tooling.  However, if there is not, it suffices to ask `cheribuild` to run
`cheribsd-${arch}-purecap --freebsd-host-tools-only` before
`disk-image-${arch}-purecap`.  The use of `--freebsd-host-tools-only` will
significantly save time and disk space, not rebuilding all of CheriBSD and the
root filesystems already shipped with the release.
