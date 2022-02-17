# Answers

2. Example output for the baseline program:
   ```
   buf=0x8085ba59 &p=0x8085ba50
   p.ptr=0x8085bb68 (0x10f into buf) *p.ptr=0f
   q=0x8085bb00 (0xa7 into buf)
   *q=a7
   r=0x8085bb00 (0xa7)
   *r=a7
   ```

   And for the CHERI-enabled program:
   ```
   buf=0x3fffdffd71 [rwRW,0x3fffdffd71-0x3fffdfff70] &p=0x3fffdffd60 [rwRW,0x3fffdffd60-0x3fffdffd70]
   p.ptr=0x3fffdffe80 [rwRW,0x3fffdffd71-0x3fffdfff70] (0x10f into buf) *p.ptr=0f
   q=0x3fffdffe00 [rwRW,0x3fffdffd71-0x3fffdfff70] (0x8f into buf)
   *q=8f
   r=0x3fffdffe00 [rwRW,0x3fffdffd71-0x3fffdfff70] (invalid) (0x8f)
   In-address space security exception
   ```

3. Constructing `r` is very similar on the two targets, differing only by the
   use of integer- or capability-based memory instructions:

   |       | Baseline         | CHERI               |
   | ----- | :-------         | :----               |
   | Store | `sb zero, 0(sp)` | `csb zero, 32(csp)` |
   | Load  | `ld s0, 0(sp)`   | `clc cs1, 32(csp)`  |

   The significant difference is in the construction of `q`.  On the baseline
   architecture, it is a direct bitwise `and` of a pointer loaded from memory:

   ```
   ld   a0, 0(sp)
   andi s0, a0, -256
   ```

   On CHERI, on the other hand, the program makes explicit use of capability
   manipulation instructions to...

   | Instruction | Action |
   | ----------- | ------ |
   | `clc       ca0, 32(csp)` | Load the capability from memory |
   | `cgetaddr  a1, ca0`      | Extract its address field to a register |
   | `andi      a1, a1, -256` | Perform the mask operation |
   | `csetaddr  cs1, ca0, a1` | Update the address field |

   This longer instruction sequence serves to prove to the processor that the
   resulting capability (in `cs1`) was constructed using valid transformations.
   In particular, the `csetaddr` allows the processor to check that the
   combination of the old capability (in `ca0`) and the new address (in `a1`)
   remains *representable*.

4. While the in-memory, byte representation of `q` and `r` are identical, `q`
   has been manipulated as *bytes* rather than as a *capability* and so has had
   its tag zeroed.  (Specifically, the `csb zero, 32(csp)` instruction cleared
   the tag associated with the 16-byte granule pointed to by `32(csp)`; the
   subsequent `clc` transferred this zero tag to `cs1`.)
