# Exploiting an uninitialized stack frame to manipulate control flow

The objective of this mission is to demonstrate arbitrary code
execution through the use of uninitialized variables on the stack,
despite CHERI protections.  You will attack three different versions
of the program:

1. A baseline RISC-V compilation, to establish that the vulnerability is
   exploitable without any CHERI protections.

2. A hardened CHERI-RISC-V compilation with stack clearing, which
   should be non-exploitable.

3. A baseline CHERI-RISC-V compilation with no stack clearing, which
   should be non-exploitable due to pointer tagging.

The success condition for an exploit, given attacker-provided input
overriding an on-stack buffer, is to modify control flow in the program
such that the `success` function is executed.

### Program overview

Cookie monster is always hungry for more cookies.  You can sate the
monster's hunger by providing cookies as standard input.  Cookies are
provided as a pair of hexadecimal characters (case is ignored).  Each
cookie is stored at successive bytes in an on-stack character array.
The character array aliases an uninitialized function pointer used in
a subsequent function.  A minus character ('-') can be used to skip
over a character in the array without providing a new cookie.  An
equals sign ('=') can be used to skip over the number of characters in
a pointer without providing any new cookies.  Whitespace is ignored in
the input line.  Input is terminated either by a newline or end of
file (EOF).

### Building and running

The hardened CHERI-RISC-V version with stack clearing is built by
adding `-ftrivial-auto-var-init=zero
-enable-trivial-auto-var-init-zero-knowing-it-will-be-removed-from-clang`
to the compiler command line.

### Source code

***stack-mission.c***
```C
{{#include stack-mission.c}}
```
