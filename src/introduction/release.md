# Running Exercises with the CHERI Software Release

The Summer 2021 release of the [CHERI Software
Stack](https://cheri-dist.cl.cam.ac.uk/) contains pre-built versions
of all of the exercises described in this course, allowing you to work
throught the exercises without first needing to build them from
sources.

With the CHERI Software Release installed, run either the CHERI-RISCV
or Morello simulators using one of the following commands:

`./docker-cheribuild run-riscv64-purecap`

or

`./docker-cheribuild run-morello-purecap`

and wait for the system to boot.

Once the system is booted login with the `root` account.  No password
is required.

During the boot process the system will show you how to properly mount
the various external resources, which include the pre-built versions
of the exercises.  Scroll back to where you issued your
`./docker-cheribuild` command and find these lines:

```
Providing /source over SMB to the guest. Use `mkdir -p /srcroot && mount_smbfs -I 10.0.2.4 -N //10.0.2.4/source_root /srcroot` to mount it
Providing /build over SMB to the guest. Use `mkdir -p /buildroot && mount_smbfs -I 10.0.2.4 -N //10.0.2.4/build_root /buildroot` to mount it
Providing /output over SMB to the guest. Use `mkdir -p /outputroot && mount_smbfs -I 10.0.2.4 -N //10.0.2.4/output_root /outputroot` to mount it
Providing /output/rootfs-riscv64-purecap over SMB to the guest. Use
`mkdir -p /rootfs && mount_smbfs -I 10.0.2.4 -N //10.0.2.4/rootfs
/rootfs` to mount it
```

At the root prompt `#` mount the `outputroot` directory:

`mkdir -p /outputroot && mount_smbfs -I 10.0.2.4 -N //10.0.2.4/output_root /outputroot`

The pre-built exercises for CHERI-RISCV can be found in the
`/outputroot/rootfs-riscv64-purecap/opt/cheri-exercises` and for
Morello are present in
`/outputroot/rootfs-morello-purecap/opt/cheri-exercises`

```
ls -l
total 293
-rwxr-xr-x  1 root  wheel  16536 Aug 29 04:55 buffer-overflow-cheri
-rwxr-xr-x  1 root  wheel  15904 Aug 29 04:55 buffer-overflow-fnptr-cheri
-rwxr-xr-x  1 root  wheel  23336 Aug 29 04:55 buffer-overflow-fnptr-riscv64
-rwxr-xr-x  1 root  wheel  23440 Aug 29 04:55 buffer-overflow-riscv64
-rwxr-xr-x  1 root  wheel  15432 Aug 29 04:55 buffer-overflow-subobject-cheri
-rwxr-xr-x  1 root  wheel  15432 Aug 29 04:55 buffer-overflow-subobject-cheri-subobject-safe
-rwxr-xr-x  1 root  wheel  22976 Aug 29 04:55 buffer-overflow-subobject-riscv64
-rwxr-xr-x  1 root  wheel  16288 Aug 29 04:55 long-over-pipe-cheri
-rwxr-xr-x  1 root  wheel  23448 Aug 29 04:55 long-over-pipe-riscv64
-rwxr-xr-x  1 root  wheel  15072 Aug 29 04:55 print-capability
-rwxr-xr-x  1 root  wheel  15008 Aug 29 04:55 print-pointer-cheri
-rwxr-xr-x  1 root  wheel  22688 Aug 29 04:55 print-pointer-riscv64
-rwxr-xr-x  1 root  wheel  16432 Aug 29 04:55 ptr-over-pipe-cheri
-rwxr-xr-x  1 root  wheel  23496 Aug 29 04:55 ptr-over-pipe-riscv64
-rwxr-xr-x  1 root  wheel  15384 Aug 29 04:55 union-int-ptr-cheri
-rwxr-xr-x  1 root  wheel  22896 Aug 29 04:55 union-int-ptr-riscv64
```

You can now work through the exercises without having to build each
exercise by hand.
