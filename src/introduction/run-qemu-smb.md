# Using SMB under QEMU

In general, we use `samba` to bring parts of the host filesystem into the guest.

## Quickstart

For the purposes of this book, it is sufficient to bring just the build root in:
```
mount_smbfs -I 10.0.2.4 -N //10.0.2.4/build_root /mnt
```

Programs can be executed directly over the smb mount, but core dumping over smb
can be quite slow.  We therefore recommend either disabling coredumps, with
```
sysctl kern.coredump=0
```
or copying programs into `/tmp` before running them.

## More Detail

During the boot process, `cheribuild` will show you how to properly mount
various external resources.  In particular, it will generate a series of lines
such as 
```
Providing /source over SMB to the guest. Use `mkdir -p /srcroot && mount_smbfs -I 10.0.2.4 -N //10.0.2.4/source_root /srcroot` to mount it
Providing /build over SMB to the guest. Use `mkdir -p /buildroot && mount_smbfs -I 10.0.2.4 -N //10.0.2.4/build_root /buildroot` to mount it
Providing /output over SMB to the guest. Use `mkdir -p /outputroot && mount_smbfs -I 10.0.2.4 -N //10.0.2.4/output_root /outputroot` to mount it
Providing /output/rootfs-riscv64-purecap over SMB to the guest. Use `mkdir -p /rootfs && mount_smbfs -I 10.0.2.4 -N //10.0.2.4/rootfs /rootfs` to mount it
```

Feel free to run any of these commands, or your preferred variations, to access
more of the host system.
