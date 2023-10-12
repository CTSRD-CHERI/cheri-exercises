# The CHERI Tutorial VM Image
For these exercises we will run CheriBSD in a CHERI-QEMU VM. You can either run CHERI-QEMU natively on your machine or you can run CHERI-QEMU on one of the Ubuntu VMs that we offer. We recommed to run CHERI-QEMU natively but we offer the Ubuntu VMs in case the binary doesn't work on some systems.
Below are instructions for how to run CheriBSD and CHERI-QEMU on [Linux](#linux), [ARM macOS](#arm-macos), [x86-64 macOS](x86-64-macos), and [Windows](#windows).

## SSH
This is optional. The VMs can be used without SSH.
1. Boot CHERI-RISC-V CheriBSD with one of the setup guides below. If you use one of the Ubuntu VMs run `dhclient` in the Ubuntu VM before you boot CheriBSD.
1. Set a password for root on CheriBSD.
1. Open the config file of the SSH daemon with `vi /etc/ssh/sshd_config` and change `PermitRootLogin` from `no` to `yes`. ('x' deletes characters in vi.)
1. Run in `service sshd onestart` in CheriBSD to start the SSH server.
1. Wait until the SSH server has generated the SSH host keys.
1. Run `ssh -p 10022 root@127.0.0.1` to ssh into CheriBSD.

Caution: Our scripts and UTM config files set up port forwarding from port 10022 on the host to the SSH server on CheriBSD. If you don't want that delete the relevant Qemu parameters or adjust the setting of the Ubuntu VM in UTM.

## Linux
### Run CHERI-QEMU Natively

1. Download this [zip](https://www.cl.cam.ac.uk/~pffm2/sosp2023_cheri_tutorial/files/sosp2023_qemu.zip) archive.
1. Unzip it with `unzip sosp2023_qemu.zip`.
1. `cd` into the unzipped directory.
1. Start CheriBSD with `./run-cheribsd-vm-on-linux.sh`
1. Log into CheriBSD with `root` and no password.
1. The source code of the exercises is in `~/cheri-exercises-sosp2023_tutorial/src/exercises/` in the CheriBSD VM.

The CHERI-QEMU binary is a statically compiled binary and so doesn't need any dependencies to be installed.

### Run CHERI-QEMU on an Ubuntu VM

1. Download this [zip](https://www.cl.cam.ac.uk/~pffm2/sosp2023_cheri_tutorial/files/amd64-vm-image.zip) archive.
1. Unzip it with `unzip amd64-vm-image.zip`.
1. `cd` into the unzipped directory.
1. Start the Ubuntu VM with `./run-amd64-ubuntu-vm.sh`.
1. Log into the Ubuntu VM with `root` and the password `sosp2023`.
1. Change the directory with `cd ./sosp2023_qemu`.
1. Start the CHERI-QEMU CheriBSD VM with `./run-cheribsd-vm-on-ubuntu-vm.sh`.
1. Log into CheriBSD with `root` and no password.
1. The source code of the exercises is in `~/cheri-exercises-sosp2023_tutorial/src/exercises/` in the CheriBSD VM.


## ARM macOS
### Run CHERI-QEMU Natively

These setup steps require you to install dependencies with the Homebrew package manager. If you don't want to use Homebrew follow the steps for how to run the Ubuntu VM with [UTM](#run-CHERI-QEMU-on-an-arm64-utm-ubuntu-vm).

1. Install the dependencies of CHERI-QEMU with Homebrew: `brew install libarchive glib pixman xz zstd libusb libpng gnutls libssh libnettle jpeg-turbo` This takes about 20 minutes on our M1 Mac-Mini test machine.
1. Download this [zip](https://www.cl.cam.ac.uk/~pffm2/sosp2023_cheri_tutorial/files/sosp2023_qemu.zip) archive.
1. Unzip it with `unzip sosp2023_qemu.zip`.
1. `cd` into the unzipped directory.
1. Start CheriBSD with `./run-cheribsd-vm-on-arm-macos.sh`
1. Log into CheriBSD with `root` and no password.
1. The source code of the exercises is in `~/cheri-exercises-sosp2023_tutorial/src/exercises/` in the CheriBSD VM.

### Run CHERI-QEMU on a Qemu Ubuntu VM

These setup steps require you to install dependencies with the Homebrew package manager. If you don't want to use Homebrew follow the steps for how to run the Ubuntu VM with [UTM](#run-CHERI-QEMU-on-an-arm64-utm-ubuntu-vm).

1. Install Qemu with Homebrew: `brew install qemu`
1. Download this [zip](https://www.cl.cam.ac.uk/~pffm2/sosp2023_cheri_tutorial/files/arm64-vm-image.zip) archive.
1. Unzip it with `unzip arm64-vm-image.zip`.
1. `cd` into the unzipped directory.
1. Start the Ubuntu VM with `./run-arm64-ubuntu-vm.sh`.
1. Log into the Ubuntu VM with `root` and the password `sosp2023`.
1. Change the directory with `cd ./sosp2023_qemu`.
1. Start the CHERI-QEMU CheriBSD VM with `./run-cheribsd-vm-on-ubuntu-vm.sh`.
1. Log into CheriBSD with `root` and no password.
1. The source code of the exercises is in `~/cheri-exercises-sosp2023_tutorial/src/exercises/` in the CheriBSD VM.

### Run CHERI-QEMU on a ARM64 UTM Ubuntu VM

1. Download this [zip](https://www.cl.cam.ac.uk/~pffm2/sosp2023_cheri_tutorial/files/arm64-utm-vm.zip) archive.
1. Unzip it with `unzip arm64-utm-vm.zip`.
1. Install UTM with these [instructions](https://docs.getutm.app/installation/macos/).
1. Launch UTM.
1. Click on the "+" icon in the top left corner next to "UTM".
1. Click on "Open..." in the "Start" menu.
1. Navigate into the directory into which the zip archive was extracted and double click on the directory `build-for-arm64-utm-linux-vm` to import the UTM Ubuntu VM.
1. Click on the "Play" arrow to start the Ubuntu VM.
1. Log into the Ubuntu VM with `root` and the password `sosp2023`.
1. Change the directory with `cd ./sosp2023_qemu`.
1. Run `dhclient` to make sure the network interface is configured. (This is only necessary if you plan to SSH into the CheriBSD VM.)
1. Start the CHERI-QEMU CheriBSD VM with `./run-cheribsd-vm-on-ubuntu-vm.sh`.
1. Log into CheriBSD with `root` and no password.
1. The source code of the exercises is in `~/cheri-exercises-sosp2023_tutorial/src/exercises/` in the CheriBSD VM.

## x86-64 macOS
### Run CHERI-QEMU Natively

These setup steps require you to install dependencies with the Homebrew package manager. If you don't want to use Homebrew follow the steps for how to run the Ubuntu VM with [UTM](#run-CHERI-QEMU-on-a-x86-utm-ubuntu-vm).

1. Install the dependencies of CHERI-QEMU with Homebrew: `brew install libarchive glib pixman xz zstd libusb libpng gnutls libssh libnettle jpeg-turbo`
1. Download this [zip](https://www.cl.cam.ac.uk/~pffm2/sosp2023_cheri_tutorial/files/sosp2023_qemu.zip) archive.
1. Unzip it with `unzip sosp2023_qemu.zip`.
1. `cd` into the unzipped directory.
1. Start CheriBSD with `./run-cheribsd-vm-on-x86-64-macos.sh`
1. Log into CheriBSD with `root` and no password.
1. The source code of the exercises is in `~/cheri-exercises-sosp2023_tutorial/src/exercises/` in the CheriBSD VM.

### Run CHERI-QEMU on a Qemu Ubuntu VM

These setup steps require you to install dependencies with the Homebrew package manager. If you don't want to use Homebrew follow the steps for how to run the Ubuntu VM with [UTM](#run-CHERI-QEMU-on-a-x86-utm-ubuntu-vm).

1. Install Qemu with Homebrew: `brew install qemu`
1. Download this [zip](https://www.cl.cam.ac.uk/~pffm2/sosp2023_cheri_tutorial/files/amd64-vm-image.zip) archive.
1. Unzip it with `unzip amd64-vm-image.zip`.
1. `cd` into the unzipped directory.
1. Start the Ubuntu VM with `./run-amd64-ubuntu-vm.sh`.
1. Log into the Ubuntu VM with `root` and the password `sosp2023`.
1. Change the directory with `cd ./sosp2023_qemu`.
1. Start the CHERI-QEMU CheriBSD VM with `./run-cheribsd-vm-on-ubuntu-vm.sh`.
1. Log into CheriBSD with `root` and no password.
1. The source code of the exercises is in `~/cheri-exercises-sosp2023_tutorial/src/exercises/` in the CheriBSD VM.

### Run CHERI-QEMU on a x86 UTM Ubuntu VM

1. Download this [zip](https://www.cl.cam.ac.uk/~pffm2/sosp2023_cheri_tutorial/files/amd64-utm-vm.zip) archive.
1. Unzip it with `unzip amd64-utm-vm.zip`.
1. Install UTM with these [instructions](https://docs.getutm.app/installation/macos/).
1. Launch UTM.
1. Click on the "+" icon in the top left corner next to "UTM".
1. Click on "Open..." in the "Start" menu.
1. Navigate into the directory into which the zip archive was extracted and double click on the directory `build-for-amd64-utm-linux-vm` to import the UTM Ubuntu VM.
1. Click on the "Play" arrow to start the Ubuntu VM.
1. Log into the Ubuntu VM with `root` and the password `sosp2023`.
1. Change the directory with `cd ./sosp2023_qemu`.
1. Run `dhclient` to make sure the network interface is configured. (This is only necessary if you plan to SSH into the CheriBSD VM.)
1. Start the CHERI-QEMU CheriBSD VM with `./run-cheribsd-vm-on-ubuntu-vm.sh`.
1. Log into CheriBSD with `root` and no password.
1. The source code of the exercises is in `~/cheri-exercises-sosp2023_tutorial/src/exercises/` in the CheriBSD VM.


## Windows

We use the Windows Subsystem for Linux (WSL) to run the Linux qemu binaries. A short introduction to WSL and an installation guide can be found [here](https://learn.microsoft.com/en-us/windows/wsl/).

1. Install Ubuntu WSL with `wsl --install -d Ubuntu`. Note, this will install either an Ubuntu VM or a Linux compatibility layer on your machine. See [here](https://learn.microsoft.com/en-us/windows/wsl/) for more details on WSL.
1. Setup a username and password for WSL Ubuntu during the installation process.
1. Once the installation has finished and a shell for the WSL installation has opened, install `zip` with `sudo apt install zip`.
1. Download this [zip](https://www.cl.cam.ac.uk/~pffm2/sosp2023_cheri_tutorial/files/sosp2023_qemu.zip) archive.
1. Unzip it with `unzip sosp2023_qemu.zip`.
1. `cd` into the unzipped directory.
1. Start CheriBSD with `./run-cheribsd-vm-on-linux.sh`
1. Log into CheriBSD with `root` and no password.
1. The source code of the exercises is in `~/cheri-exercises-sosp2023_tutorial/src/exercises/` in the CheriBSD VM.

It doesn't matter whether your system uses WSL 1 or WSL 2. We've tested these setup steps on both versions.

## Miscellaneous

Shutdown CheriBSD and the Ubuntu VM with `shutdown -h now`. CheriBSD will display "Please press any key to reboot." at the end of the shutdown procedure. This is false and any key press will the exit the VM instead.
