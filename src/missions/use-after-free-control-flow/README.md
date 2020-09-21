# Exploiting heap use-after-free to manipulate control flow

**This mission requires a CheriBSD sysroot and image with temporal safety!**
The executable can be built for RISC-V and CHERI-RISC-V (and exploring both may
be worthwhile), but the mission is for CHERI-RISC-V with heap temporal safety
enforcement in place.

This mission is a potted exercise inspired by real-world vulnerabilities where
an active adversary can influence the contents of a server's heap and, often
without completing authentication, walk the server's protocol state machine
through erroneous states. Popular bugs facilitating these exploits include
double free, use-after-reallocation, and type confusion (esp. of temporally
aliased objects). For this mission, we have simplified the interface to be a
little "command language" read over `stdin`, detailed below.

The success criterion is executing the `success` function. To ease testing,
this is the only case in which the program source claims to exit with a code of
`42`. Ordinary termination is signaled with `0` and invalid input results in
the program terminating with `1`. While the source does not overtly claim any
other outcome, if control flow can be redirected, almost anything goes.

### Program Overview

This is a (very) minimal simulation of a program that manages multiple
sessions, objects within each session, and limited information flow between
those sessions. Rather than network sockets, each session is represented by a
`struct farm` and there are up to four such active at any moment. Within each
`struct farm` is a circular collection of `struct crop`s, at most one of which
may be selected by the `cursor` of its owning `struct farm`. If no crop is
selected, the cursor is said to be "in the gap". The crop at the cursor, if
any, can be asked to describe itself, using a function pointer within the
`struct crop` itself.

To make matters more exciting, a UFO may be summoned to perform particular
kinds of mischief. The UFO may "abduct" a pointer into itself and can create
crop signs within `struct crop` (with data) or `struct farm` (with a capability
to the `success()` function).

The gadgets offered by the UFO are somewhat limited (that is, they are not
"write what where" or arbitrary control transfer) as one might expect to see in
a real application. Nevertheless, they are sufficiently powerful to provide
*myriad* exploitation vectors on RISC-V without CHERI, and even a few on
CHERI-RISC-V. However, we believe that enforced heap temporal safety will
ensure that any use of them is either overwritten by subsequent program
operations before the gadgets' effects influence control flow or will cause the
program to fail-stop with a default-fatal signal (e.g., `SIGSEGV` or
`SIGPROT`).

### Building and running

We suggest using the `ccc` tool provided with this book, building for
`riscv64-purecap`. For experimentation, this program also builds for
`riscv64`, without CHERI. On `riscv64-purecap` builds, heap temporal safety
enforcement may be disabled at program load time by setting the environment
variable `MALLOC_DISABLE_REVOCATION` to a non-empty value. (While setting this
flag prior to program loading disqualifies such invocations from completing the
mission, being able to disable revocation from within `main()` would be quite
interesting indeed.)

As said before, the program expects to read a command language from `stdin`.
It will print "Ready" and then await input. Unlike real applications, this
program is fairly chatty about its own operation to ease exploration.
Nevertheless, it may be useful to run it within `gdb`, especially to
differentiate causes of crashes.

### Command Language Directives

* Whitespace is quietly ignored in most cases; this may simplify reading
  programs.

* Digits `0` through `3` focus on the corresponding farm slots, making it the
  locus of subsequent commands until altered.

* `F` allocates a new farm at the current slot.

* `f` frees the current slot's farm (along with any crops in its collection)

* `C` creates a new crop in the current farm and puts it at the left of the
  collection. The cursor is left in the gap or pointing at the crop it was
  before. If the cursor is not in the gap, the new crop will inherit the
  description of the selected crop; otherwise, the program chooses one of two
  varieties of cherry.

* `L` and `R` move the current farm's cursor left and right, respectively. The
  farm's collection is circular with a gap; moving left (right) from the gap
  places the cursor at the rightmost (leftmost) element, and walking off either
  end returns the cursor to the gap.

* `Z` moves the current farm's cursor to the gap.

* `D` describes the crop at the current farm's cursor, if that cursor is not in
  the gap.

* `c` removes the crop at the current farm's cursor from the collection and
  frees it.

* `U` causes a UFO to arrive; `u` causes it to leave. There is at most one UFO
  at any moment.

* `A` causes the UFO, if present, to abduct the current farm's cursor,
  presumably for further scrutiny.

* `S` causes the UFO to make a crop sign on the current farm; `s` causes the
  UFO to read the next `sizeof(void*)` characters from `stdin` (whitespace is
  not ignored for this) and use that to sign the crop indicated by the current
  farm's cursor (a smaller crop sign, if you will). Writing crop signs of
  either variety is very destabilizing and likely to lead to crashes!

* On CHERI-RISC-V builds running with heap temporal safety enforcement, `!`
  will force a revocation pass, destroying pointers to free objects and
  allowing reuse of memory and address space.

### Example Session

Here is a short session which creates one `struct farm`, two `struct crop`s,
exhibits cursor control and description of `struct crop`s, and then tears down
the `struct farm`:
```
$ echo FCC RD ZLD f | ./temporal-mission
Ready (CHERI-RISC-V)
New farm (index 0) at 0x41201080
New crop at 0x41201100
New crop at 0x41201180
Farm 0 cursor 0x41201180
FYI: Current farm is 0x41201080
FYI:  cursor 0x41201180
FYI:  cursor->describe 0x102fd2
Chelan at 0x41201180
Farm 0 cursor 0x41201100
FYI: Current farm is 0x41201080
FYI:  cursor 0x41201100
FYI:  cursor->describe 0x10300e
Colt at 0x41201100
Tear down farm (index 0) at 0x41201080
```

### Source code

**temporal-mission.c**
```C
{{#include temporal-mission.c}}
```
