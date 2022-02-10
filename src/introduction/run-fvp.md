# Using Morello FVP

While we cannot offer the Morello FVP in source form, `cheribuild` can
nevertheless download and run the binaries from Arm, assuming you have a
compatible machine (generally, Linux of some flavor on x86\_64, either as the
host OS or within a container or hypervised environment such as Docker for Mac).

Review the license agreement for the FVP and install it with
```
~/cheri/cheribuild/cheribuild.py install-morello-fvp
```
Thereafter, you will be able to run the Morello purecap system disk images built by `cheribuild` using
```
~/cheri/cheribuild/cheribuild.py run-fvp-morello-purecap --run-fvp/ssh-forwarding-port=12345
```

The simplest way to transfer programs to the FVP to be run is to use `ssh`.
Please see the [instructions for adding a ssh key](./run-ssh.md).
Then proceed to the first exercise,
[Compile and run baseline and CHERI-enabled
programs](../exercises/compile-and-run).
