# The SOSP 2023 VM Image

Please download a VM image that works on your setup before the tutorial.

## How to Get the VM Image
A tarball with a VM image and statically linked Qemu binaries for Linux, MacOS, and the Ubuntu flavour of the Windows Subsystem for Linux 2 are available [here]() (To do: Add URL). If this setup doesn't work on your machine you can download a VirtualBox image (here)[] (To do: Add URL) . The VirtualBox image uses Ubuntu and contains the files of the tarball.

## How to Use the Qemu VM Image
1. Download the tarball [here]() (To do: Add Url).
1. Decompress the tarball with `tar -xvf sosp2023_cheri_tutorial_qemu.tar.gz`.
1. Boot the VM by running one of the `run-<system>.sh` scripts. `<system>` is either `macos` or `linux` depending on if you use MacOS, Linux, or Windows Subsystem for Linux.
1. Log in as `root` without a password.
1. The exercises are in `~/cheri-exercises-sosp2023_tutorial/src/exercises/`. The numbered directories contain the exercises for the tutorial. Each directory contains a Makefile and one or more C files.

## How to Use the VirtualBox Image
TO DO
