# Corrupt a control-flow pointer using a subobject buffer overflow

This exercise demonstrates how CHERI pointer integrity protection prevents
a function pointer overwritten with data due to a buffer overflow from being
used for further memory access.

1. Compile `buffer-overflow-fnptr.c` with a baseline target and binary name
   of `buffer-overflow-fnptr-baseline`, and a CHERI-enabled target and binary
   name of `buffer-overflow-fnptr-cheri`. Do not enable compilation with
   [subobject bounds protection](../buffer-overflow-subobject) when compiling
   with the CHERI-enable target (at least, at first).

2. Run the baseline program under GDB; why does it crash?

3. Run the CHERI-enabled program under GDB; why does it crash?

## Source Files

**buffer-overflow-fnptr.c**
```C
{{#include buffer-overflow-fnptr.c}}
```

### Support code

**asserts.inc**
```C
{{#include asserts.inc}}
```
