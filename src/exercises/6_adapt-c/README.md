# Adapt a C Program to CHERI C

This excercise presents an example C program that includes capability-related
issues that might appear as bugs in software initially developed for non-CHERI
architectures.  The example C program is `cat(1)` from CheriBSD (and hence
FreeBSD) modified to introduce the issues that we want to investigate.


1. Read Sections 4.2, 4.2.1, 4.2.3 from the
   [CHERI C/C++ Programming Guide](https://www.cl.cam.ac.uk/techreports/UCAM-CL-TR-947.pdf).
   In Section 4.2.1, read only information on the following C-language types:
   `long`, `uintptr_t` and `char *,...` (pointer types).

2. Compile `cat/cat.c` and `cat/methods.c` for the baseline architecture with the
   `cat-baseline` make target and for the CHERI-aware architecture with the `cat-cheri` 
   target. The compiler should print some warnings when compiling `cat-cheri`.  Save the
   output to examine the warnings later.

3. Run both versions to print contents of an arbitrary file (e.g.,
   `/etc/hostid`), once without any additional flags and once with the `-n`
   flag.

4. Run the CHERI version, again without any additional flags, under `gdb` and
   examine the crash in more detail. Use `gdb` and not `gdb-run.sh` to set
   appropriate breakpoints before your program is started.

5. Get back to the compiler warnings and try to solve a bug that triggered the
   crash.

6. Run the CHERI version, again with the `-n` flag, under `gdb` and examine the
   crash in more detail. You can use `gdb-run.sh` this time.

7. Get back to the compiler warnings and try to solve a bug that triggered the
   crash.

8. You just analysed two bugs in `cat`. How are they different and why they
   trigger crashes in different ways?

## Source Files

**cat/cat.c**
```C
{{#include cat/cat.c}}
```

**cat/cat.h**
```C
{{#include cat/cat.h}}
```

**cat/methods.c**
```C
{{#include cat/methods.c}}
```

## Courseware

This exercise has [presentation materials](./adapt-c.pptx) available.
