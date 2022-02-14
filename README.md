<!-- ANCHOR: cover -->

# Adversarial CHERI Exercises and Missions

Robert N. M. Watson (University of Cambridge),
Brooks Davis (SRI International),
Nathaniel Wesley Filardo (Microsoft Research),
Jessica Clarke (University of Cambridge) and
John Baldwin (Ararat River Consulting).

This repository contains a series of skills development and adversarial
exercises for [CHERI](http://cheri-cpu.org), specifically aimed at the
CHERI-RISC-V implementation.

## Acknowledgements

The authors gratefully acknowledge Reuben Broadfoot, Lawrence Esswood, Brett
Gutstein, Joe Kiniry, Alex Richardson, Austin Roach, and Daniel Zimmerman for
their feedback and support in developing these exercises.

*Some portions of this document remain a work-in-progress.  Feedback and
contributions are welcomed.  Please see our [GitHub
Repository](https://github.com/CTSRD-CHERI/cheri-exercises) for the source
code and an issue tracker.*

<!-- ANCHOR_END: cover -->

## Building

Building the book from the Markdown sources requires
[mdBook](https://github.com/rust-lang/mdBook). Once installed, `mdbook build`
will build the static HTML files in the `book/` directory, whilst `mdbook
serve` will build and serve them at `http://localhost:3000`. Please refer to
the mdBook documentation for futher options.
