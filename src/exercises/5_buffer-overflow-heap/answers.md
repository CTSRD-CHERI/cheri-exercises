# Answers - Exercise heap overflows

2. Example output:
   ```
   # ./buffer-overflow-heap-baseline 0x20
   b1=0x83e82000 b2=0x83e82020 diff=20
   Overflowing by 1
   b2 begins: ABBB
   Overflowing by 2
   b2 begins: AABB
   ```
   ```
   # ./buffer-overflow-heap-cheri 0x20
   sz=20, CRRL(sz)=20
   b1=0x407c7000 [rwRW,0x407c7000-0x407c7020] b2=0x407c7020 [rwRW,0x407c7020-0x407c7040] diff=20
   Overflowing by 1
   In-address space security exception
   ```

3. Example session (abridged):
   ```
   # gdb -ex run -ex bt --args ./buffer-overflow-heap-cheri 0x20

   Starting program: ./buffer-overflow-heap-cheri 0x20
   sz=20, CRRL(sz)=20
   b1=0x407c7000 [rwRW,0x407c7000-0x407c7020] b2=0x407c7020 [rwRW,0x407c7020-0x407c7040] diff=20
   Overflowing by 1

   Program received signal SIGPROT, CHERI protection violation
   Capability bounds fault caused by register ca4.
   memset (dst0=0x407c7000 [rwRW,0x407c7000-0x407c7020], c0=65, length=<optimized out>) at /cheri/source/mainline/cheribsd/lib/libc/string/memset.c:131
   131     /cheri/source/mainline/cheribsd/lib/libc/string/memset.c: No such file or directory.

   Thread 1 (LWP 100057 of process 960):
   #0  memset (dst0=0x407c7000 [rwRW,0x407c7000-0x407c7020], c0=65, length=<optimized out>) at /cheri/source/mainline/cheribsd/lib/libc/string/memset.c:131
   #1  0x00000000001020d2 in main (argc=<optimized out>, argv=<optimized out>) at ./src/exercises/buffer-overflow-heap/buffer-overflow-heap.c:49
   (gdb) i r ca4
   ca4            0xd17d00000409b00400000000407c7020       0x407c7020 [rwRW,0x407c7000-0x407c7020]
   ```

   The capability in `ca4` is, as expected, a reference to the first allocation
   (`b1`).  The bounds on this capability must have been imposed *by malloc*.

4. Example output:
   ```
   # /mnt/buffer-overflow-heap-baseline 0x1001
   b1=0x840ec000 b2=0x840ed400 diff=1400
   Overflowing by 1
   b2 begins: BBBB
   Overflowing by 401
   b2 begins: AABB
   ```
   ```
   # /mnt/buffer-overflow-heap-cheri 0x1001
   sz=1001, CRRL(sz)=1008
   b1=0x407c7000 [rwRW,0x407c7000-0x407c8008] b2=0x407c8400 [rwRW,0x407c8400-0x407c9408] diff=1400
   Overflowing by 1
   Overflowing by 401
   In-address space security exception
   ```

   Using addresses from the CHERI run, we might draw something that highlighted
   these key addresses:

   | Address | Contents |
   | ------: | -------- |
   | ↑ `0x000...00` | |
   | `0x407c7000` | Start of `b1` |
   | `0x407c8001` | Last byte of `b1` allocation ("end" #1) |
   | `0x407c8002` | Start of CHERI representation padding |
   | `0x407c8007` | Last byte of CHERI representation padding ("end" #2) |
   | `0x407c8008` | Start of allocator size-class padding |
   | `0x407c83FF` | Last byte of allocator size-class padding ("end" #3) |
   | `0x407c8400` | `b2` |
   | ↓ `0xFFF...FF` | |

5. The first overflow, by 1 byte, is *within bounds* due to *architectural
   precision* and so, as far as the CPU is concerned, is not an overflow despite
   writing outside the logical bounds of the `b1` allocation.

6. In order to set bounds large enough to encapsulate large objects, CHERI's
   compressed capability representation may be able to express only larger
   bounds than the requested size.  (More generally, the *base* and *limit*s of
   a capability have increased alignment requirements as they are moved further
   apart, that is, as the capability length increases.  For the capabilities
   in this example, the bases have strong alignments, of at least 10 bits, due
   to the allocator's use of size classes.)

   If bounds were simply widened with no additional consideration, then pointers
   to different objects might come to authorize access to (parts of) each
   other's memory!  In order to ensure that capabilities to distinct C objects
   do not alias like this, various system components must take CHERI capability
   compression into consideration:

   - The compiler, for on-stack allocations and [address-taken subobjects](../4_subobject-bounds).
   - The linker, for objects in static storage
   - The heap allocator(s), for objects in dynamic storage

   These concerns do not usually reach "application level" C.
