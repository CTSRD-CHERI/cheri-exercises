# Obtaining CHERI Compilers and Simulators

<!-- Name is known in-book -->
## Using the CHERI Software Release

The [CheriBSD Software Release](https://cheri-dist.cl.cam.ac.uk/) available from the
University of Cambridge includes a docker environment and pre-built compilers
and emulators for both CHERI-RISC-V and Morello, as well as [pre-built exercise
binaries](./run-release.md) that can be run out of the box without first needing
to cross-compile.  Follow the instructions in the release notes and then proceed
to [check out the exercises](./get-us.md) and then to [cross
compilation](./cross-ccc-release.md).

<!-- Name is known in-book -->
## From Source

<!-- Do not tidy the blank lines.  I am so sorry -->
<div class="ctsrd-frame ctsrd-standout">

<span class="ctsrd-large">⚠</span> This procedure takes *a while* as it involves
building a full LLVM and CheriBSD.  If you are in a hurry and can, you are
encouraged to go use the CHERI Sofwtare Release above.

</div>

There are a lot of moving pieces in the CHERI ecosystem, and, unless someone
hands you a CHERIfied build and runtime system, it can be a little daunting to
get started.  To manage the complexity of the ecosystem, we have a build
orchestration tool called
[`cheribuild`](https://github.com/CTSRD-CHERI/cheribuild).  Its
[README.md](https://github.com/CTSRD-CHERI/cheribuild/blob/master/README.md)
file contains considerable information, but to get started, you'll need to
bootstrap an LLVM compiler, a CheriBSD sysroot, and a CheriBSD disk image.

Begin by checking out `cheribuild` into `~/cheri/cheribuild` (by default, all
work takes place in `~/cheri`):

```
mkdir ~/cheri
git clone https://github.com/CTSRD-CHERI/cheribuild.git ~/cheri/cheribuild
```

Then, follow the instructions in the [Pre-Build
Setup](https://github.com/CTSRD-CHERI/cheribuild/blob/master/README.md#pre-build-setup)
section of its
[README.md](https://github.com/CTSRD-CHERI/cheribuild/blob/master/README.md)
to prepare the host for cross-compilation.

With that in place, all that remains is to select a target architecture and
instruct `cheribuild` to build the appropriate targets.  This will churn away,
prompting occasionally as it bootstraps assorted dependencies.  On a fast
machine, this will take several hours.

### CHERI-RISC-V

Build a CHERI-RISC-V-hosting `qemu` and the full CheriBSD/CHERI-RISC-V stack with
<!-- XXX: Should we advocate `-f` here? -->
```
~/cheri/cheribuild/cheribuild.py qemu disk-image-riscv64-purecap -d
```

Upon completion, you will find your

* SDK in `~/cheri/output/sdk`,
* compiler in `~/cheri/output/sdk/bin/clang`, and
* sysroot in `~/cheri/output/rootfs-riscv64-purecap`.

### Morello

Build a Morello-hosting `qemu` and the full CheriBSD/Morello stack with
```
~/cheri/cheribuild/cheribuild.py qemu-morello disk-image-morello-purecap -d
```

Upon completion, you will find your

* SDK in `~/cheri/output/morello-sdk`,
* compiler in `~/cheri/output/morello-sdk/bin/clang`, and
* sysroot in `~/cheri/output/rootfs-morello-purecap`.

In addition to the Morello-aware qemu just built, Morello users can [use Arm's
Morello FVP](./run-fvp.md), a more accurate, but slower, model of the platform.

### Next Steps

Now proceed to [check out the exercises](./get-us.md) and then to [cross
compilation](./cross-ccc.md).
