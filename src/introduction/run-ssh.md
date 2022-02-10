# Using SSH With The Emulated System

The CheriBSD system permits root login over ssh using ssh keys, but it does not
ship with keys installed out of the box.

## Create a Key

If you do not have a SSH key available, create one with, for example,
```
ssh-keygen -t ed25519
```

## Install Key to an Existing Disk Image

Log in as root to CheriBSD system, and then run
```
mkdir .ssh
cat > ~/.ssh/authorized_keys
```
and paste your public key (which will be `~/.ssh/id_ed25519.pub` if you just
created it above).  Then hit Ctrl-D.  Verify that the key copied successfully by
running
```
cat ~/.ssh/authorized_keys
```

## Install Key For Future Disk Images

`cheribuild` can add files to disk images it builds, and your SSH key is a
perfect candidate.  By default, it will pull from `~/cheri/extra-files` (or, for
the [University of Cambridge CHERI Software
Release](./get.md#using-the-cheri-software-release), the `extra-files` directory
from the unpacked tarball).  It therefore suffices to create
`~/cheri/extra-files/root/.ssh/authorized_keys` with your SSH public key.

To check that this worked, have cheribuild [rebuild the disk
image](./build-disk.md), run the system however you like, and verify that
`~root/.ssh/authorized_keys` exists.

## Running the Simulator with SSH Port Forwarding

The CHERI emulators, both [qemu](./run-qemu.md) and [Arm's FVP](./run-fvp.md)
can expose TCP ports on the host that forward to ports on the emulated guest
network.  `cheribuild` provides convenient shortcuts for TCP/22 (`ssh`).  For
`run-${arch}-*` targets, add `--run/ssh-forwarding-port=12345` to specify the
listening port number; for `run-fvp-*` targets, use
`--run-fvp/ssh-forwarding-port=12345` intead.  If these are not given,
`cheribuild` will attempt to guess a suitable default port and will announce it
during the `run-*` (or `run-fvp-*`) target's startup.

## Logging In Over SSH

You should then be able to log in to the CheriBSD system over ssh; if you are
not using docker (for example, you are not using the CHERI Software Release),
try
```
ssh -p 12345 localhost
```
and if you are using docker (as part of the CHERI Software Release), replace
`localhost` in the above with the container's IP address.

## Transferring Files

A file, `$FILE`, can then be transferred using something like
```
scp -P 12345 $FILE localhost:/tmp
```
(again, if using docker, replace `localhost` appropriately).
