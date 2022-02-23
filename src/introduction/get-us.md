# Checking out the Exercises Themselves

If you have been handed a pre-configured environment, this section may be
optional and you should proceed to [cross compilation](./cross-ccc.md).
Otherwise, read on!

Once you have a CHERI source directory, either by unpacking the [CHERI Software
Stack](https://cheri-dist.cl.cam.ac.uk/) or by performing a [source
build](./get.md#from-source), you should check out a copy of the exercises
themselves into that source directory.  In the root of the unpacked release or
in `~/cheri` as appropriate, make sure a `cheri-exercises/` directory does not
exist. If it exists (e.g., it was shipped with a release), remove it and run:
```
git clone https://github.com/nwf/cheri-exercises.git
```
If you have already checked out the repository, you may instead simply move it
into place.

Now proceed to [cross compilation](./cross.md).
