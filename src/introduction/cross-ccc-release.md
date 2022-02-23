# Using the CHERI Software Release

The [CheriBSD Software Release](https://cheri-dist.cl.cam.ac.uk/) ships with
cross-compilers for CHERI-enabled architectures and their baselines.  These
compilers are built to run in the `docker` container of the release, and so
using them in an ad hoc manner as we will do here is possible, but may take
some getting used to.

Begin by getting a shell in the docker container.  `cheribuild` can do this for
you.  From the root of the unpacked release, run:
```
./cheribuild/docker-cheribuild.py sdk-shell
```
The resulting shell is `/bin/sh` which lacks some niceties, so you may wish to
replace it with `bash` thus:
```
exec /bin/bash
```
Inside this container, `/source`, `/build`, and `/output` refer to parts of the
host filesystem.  In particular, `/source` will be the root of the unpacked
distribution tarball and `/build` and `/output` will be the corresponding
directories therein.  `/source` is imported *read-only*.

Regardless of whether you prefer `sh` or `bash`, set an environment variable so
that our [cross-compiler wrapper](./cross-ccc.md) will know where to find the
compilers:
```
export CHERIBUILD_OUTPUT=/output
```

You can then run
```
cd /source/cheri-exercises
```
to sit among the exercise source files.  Because the docker environment binds
`/source` *read-only*, you will need to explicitly choose an output path
outside of `/source` when cross-compiling, by using `-o /build/foo`, for
example, in the compiler invocations.  (Many of the examples will use relative
paths to reach the build directory, but these will need to be altered as just
described.  We apologize for the inconvenience.)  Bear this in mind and
proceed to read about the [cross-compiler wrapper](./cross-ccc.md) we will use
for the exercises in this book.
