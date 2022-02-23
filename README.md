<!-- ANCHOR: cover -->

# CHERI Exercises and Missions

Robert N. M. Watson (University of Cambridge),
Brooks Davis (SRI International),
Nathaniel Wesley Filardo (Microsoft Research),
Jessica Clarke (University of Cambridge) and
John Baldwin (Ararat River Consulting).

This repository contains a series of skills development exercises for
[CHERI](http://cheri-cpu.org), aimed at familiarizing C/C++ programmers with the
nuances of CHERI C.  Thereafter, it contains more open-ended *adversarial*
missions, specifically intended to be of interest to *red teams*.

## Workshop at ASPLOS 2020

The chapters of the book have been specifically reviewed and prepared for the
[CHERI Workshop at ASPLOS 2022](https://www.cl.cam.ac.uk/research/security/ctsrd/cheri/workshops/2022asplos/).
If you are participating in the workshop, you should consider chapters
highlighted with the icon 🍒 to prepare yourself for the workshop:
* **Before the workshop**, you should read subsections from the
[Introduction](../introduction) chapter. We recommend to use the CHERI
Software Release with Docker if it is your first experience with CHERI.
Make sure to follow instructions for **CHERI-RISC-V**, not Morello.
* During the workshop, we will work with subsections from the
[ASPLOS'22 Exercises](../exercises/asplos22.md) chapter.

The rest of the chapters are included in this book to provide you additional
content to study CHERI and customise your environment, as your time permits.

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
