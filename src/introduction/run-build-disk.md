# Rebuilding the Disk Image

This section is likely optional!  Unless you have explicit need to rebuild the
disk image (for example, it has become corrupted or you want to [add an ssh
public key](./run-ssh.md) for all future builds), you can safely skip the
documentation here.

In general, it suffices to cause `cheribuild` to run its
`disk-image-${arch}-purecap` target.

- If you are using the [University of Cambridge CHERI Software
  Release](./get.md#using-the-cheri-software-release), it should suffice to run
  exactly that target with `docker-cheribuild`.  See [more targeted
  instructions](./run-qemu-release.md#in-case-of-emergency-rebuilding-the-disk-image)
  for additional details.

- If you have [built from source](./get.md#from-source), the `-d` passed to
  `cheribuild` when [running qemu](./run-qemu.md) will do this automatically,
  or you can invoke it manually.
