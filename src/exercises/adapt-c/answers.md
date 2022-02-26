# Answers - Adapt a C Program to CHERI C

2. Example output:
   ```
   # ./tools/ccc riscv64 -o /build/cat-cheri ./src/exercises/adapt-c/cat/cat.c ./src/exercises/adapt-c/cat/methods.c
   Running: /output/sdk/bin/clang -target riscv64-unknown-freebsd -march=rv64gc -mabi=lp64d -mno-relax --sysroot=/output/sdk/sysroot-riscv64-purecap -g -O2 -fuse-ld=lld -Wall -Wcheri -o /build/cat-cheri ./src/exercises/adapt-c/cat/cat.c ./src/exercises/adapt-c/cat/methods.c
   # ./tools/ccc riscv64-purecap -o /build/cat-cheri ./src/exercises/adapt-c/cat/cat.c ./src/exercises/adapt-c/cat/methods.c
   Running: /output/sdk/bin/clang -target riscv64-unknown-freebsd -march=rv64gcxcheri -mabi=l64pc128d -mno-relax --sysroot=/output/sdk/sysroot-riscv64-purecap -g -O2 -fuse-ld=lld -Wall -Wcheri -o /build/cat-cheri ./src/exercises/adapt-c/cat/cat.c ./src/exercises/adapt-c/cat/methods.c
   ./src/exercises/adapt-c/cat/methods.c:70:43: warning: binary expression on capability types 'ptroff_t' (aka 'unsigned __intcap') and 'uintptr_t' (aka 'unsigned __intcap'); it is not clear which should be used as the source of provenance; currently provenance is inherited from the left-hand side [-Wcheri-provenance]
           return (write(fildes, (const void *)(off + (uintptr_t)buf), nbyte));
                                                ~~~ ^ ~~~~~~~~~~~~~~
   ./src/exercises/adapt-c/cat/methods.c:80:7: warning: cast from provenance-free integer type to pointer type will give pointer that can not be dereferenced [-Wcheri-capability-misuse]
           fp = (FILE *)file;
                ^
   2 warnings generated.
   ```


3. Example output:
   ```
   # ./cat-baseline /etc/hostid
   bb5fbb47-10ab-11ec-a609-f5a47707c223
   # ./cat-baseline -n /etc/hostid
        1  bb5fbb47-10ab-11ec-a609-f5a47707c223
   # ./cat-cheri /etc/hostid
   cat-cheri: write(2) failed: Bad address
   # ./cat-cheri -n /etc/hostid
   In-address space security exception (core dumped)
   #
   ```


4. When run without `gdb`, `cat-cheri` prints:
   ```
   # ./cat-cheri /etc/hostid
   cat-cheri: write(2) failed: Bad address
   ```
   Looking at the source code, we can see there is only one call to write(2):
   ```
   # grep -R write src/exercises/adapt-c/cat
   src/exercises/adapt-c/cat/methods.c:write_off(int fildes, const void *buf, ptroff_t off, size_t nbyte)
   src/exercises/adapt-c/cat/methods.c:    return (write(fildes, (const void *)(off + (uintptr_t)buf), nbyte));
   src/exercises/adapt-c/cat/methods.c:                    if ((nw = write_off(wfd, buf, off, (size_t)nr)) < 0)
   src/exercises/adapt-c/cat/methods.c:                            err(1, "write(2) failed");
   ```
   The call is in the `write_off()` function and the message with the error is
   printed in case the call fails. Let's see what are arguments and result
   values for the system call by setting a breakpoint for `write()` in `gdb`:
   ```
   # gdb ./cat-cheri
   (...)
   (gdb) b write
   Function "write" not defined.
   Make breakpoint pending on future shared library load? (y or [n]) y
   Breakpoint 1 (write) pending.
   (gdb) r /etc/hostid
   Starting program: /buildroot/cat-cheri /etc/hostid
   
   Breakpoint 1, write (fd=1, buf=0x40810000, nbytes=37) at /source/cheribsd/lib/libc/sys/write.c:49
   49      /source/cheribsd/lib/libc/sys/write.c: No such file or directory.
   (gdb) info register ca0 ca1 ca2
   ca0            0x1      0x1
   ca1            0x40810000       0x40810000
   ca2            0x25     0x25
   (gdb) 
   (gdb) disassemble 
   Dump of assembler code for function write:
   => 0x0000000040295dc4 <+0>:       auipc   a3,0xc2
      0x0000000040295dc8 <+4>:       lc      ca3,1532(a3) # 0x403583c0 <_CHERI_CAPABILITY_TABLE_+2464>
      0x0000000040295dcc <+8>:       lc      ca5,496(a3)
      0x0000000040295dd0 <+12>:      cjr     ca5
   End of assembler dump.
   (gdb) 
   ```
   We can see that `write()` was called to write to `stdout` (`ca0`) 37 bytes
   (`ca2`) from a buffer with an untagged capability (`ca1`). The `write()` libc
   function does not include a trapping instruction but it jumps with `cjr`.
   Let's see where it jumps to:
   ```
   (gdb) ni 4
   _write () at _write.S:4
   4       _write.S: No such file or directory.
   (gdb) disassemble 
   Dump of assembler code for function _write:
   => 0x0000000040299130 <+0>:       li      t0,4
      0x0000000040299132 <+2>:       ecall
      0x0000000040299136 <+6>:       bnez    t0,0x4029913e <_write+14>
      0x000000004029913a <+10>:      cret
      0x000000004029913e <+14>:      auipc   t1,0xffffd
      0x0000000040299142 <+18>:      cincoffset      ct1,ct1,-846
      0x0000000040299146 <+22>:      cjr     ct1
   End of assembler dump.
   (gdb) 
   ```
   `write()` jumped to `_write()`, a system call wrapper written in assembly,
   that makes a trap. Let's see what is a result of the system call:
   ```
   (gdb) ni 2
   4       in _write.S
   (gdb) info register ca0
   ca0            0xe      0xe
   (gdb) 
   ```
   The `write()` system call returned `0xe`. Looking at `errno(2)` and
   `write(2)`, we can conclude that we passed an incorrect address to the buffer
   which should be tagged.


5. When compiling `cat-cheri`, the compiler printed:
   ```
   ./src/exercises/adapt-c/cat/methods.c:70:43: warning: binary expression on capability types 'ptroff_t' (aka 'unsigned __intcap') and 'uintptr_t' (aka 'unsigned __intcap'); it is not clear which should be used as the source of provenance; currently provenance is inherited from the left-hand side [-Wcheri-provenance]
           return (write(fildes, (const void *)(off + (uintptr_t)buf), nbyte));
                                                ~~~ ^ ~~~~~~~~~~~~~~
   ```
   As the [CHERI C/C++ Programming Guide](https://www.cl.cam.ac.uk/techreports/UCAM-CL-TR-947.pdf)
   says in Section 4.2.3:
   > In the CHERI memory protection model, capabilities are derived from
   a single other capability.

   In our case, `off + (uintptr_t)buf` resulted in an untagged capability
   because `off` holds an integer value in an untagged capability and, as the
   compiler warns, it is used to create a resulting capability.
   In order to create a capability using the correct source capability, we can
   tell the compiler that `off` does not hold a valid address with a cast:
   ```
   diff --git a/src/exercises/adapt-c/cat/methods.c b/src/exercises/adapt-c/cat/methods.c
   index bb78a75..6520735 100644
   --- a/src/exercises/adapt-c/cat/methods.c
   +++ b/src/exercises/adapt-c/cat/methods.c
   @@ -67,7 +67,7 @@ static ssize_t
    write_off(int fildes, const void *buf, ptroff_t off, size_t nbyte)
    {
    
   -       return (write(fildes, (const void *)(off + (uintptr_t)buf), nbyte));
   +       return (write(fildes, (const void *)((size_t)off + (uintptr_t)buf), nbyte));
    }
    
    static void
   ```

6. Example output:
   ```
   # gdb-run.sh ./cat-cheri -n /etc/hostid
   (...)
   Starting program: /buildroot/cat-cheri -n /etc/hostid
   
   Program received signal SIGPROT, CHERI protection violation
   Capability tag fault caused by register cs2.
   verbose_cat (file=<optimized out>) at ./src/exercises/adapt-c/cat/methods.c:87
   87      ./src/exercises/adapt-c/cat/methods.c: No such file or directory.
   
   Thread 1 (LWP 100043 of process 808):
   #0  verbose_cat (file=<optimized out>) at ./src/exercises/adapt-c/cat/methods.c:87
   #1  do_cat (file=<optimized out>, verbose=<optimized out>) at ./src/exercises/adapt-c/cat/methods.c:214
   #2  0x0000000000102f1a in scanfiles (argv=<optimized out>, verbose=<optimized out>) at ./src/exercises/adapt-c/cat/cat.c:172
   #3  0x0000000000102d8c in main (argc=<optimized out>, argv=<optimized out>) at ./src/exercises/adapt-c/cat/cat.c:128
   (gdb) 
   ```
   `gdb` says that `cs2` triggered a CHERI exception:
   ```
   (gdb) info register cs2
   cs2            0x4037a400       0x4037a400
   (gdb) 
   ```
   `cs2` holds an untagged capability and the program tries to load a word using
   `cs2` which violates CHERI restrictions:
   ```
   (gdb) disassemble $pcc,+4
   Dump of assembler code from 0x103094 to 0x103098:
   => 0x0000000000103094 <do_cat+228>: lw      a0,16(s2)
   End of assembler dump.
   (gdb) 
   ```
   Looking at the above backtrace, we can correlate this output with the source
   code and see that `cs2` holds a value of
   the `fp` variable:
   ```
   (gdb) p fp
   $1 = (FILE *) 0x4037a400
   ```
   It means that for some reason `fp` became an invalid capability.


7. When compiling `cat-cheri`, the compiler printed:
   ```
   ./src/exercises/adapt-c/cat/methods.c:80:7: warning: cast from provenance-free integer type to pointer type will give pointer that can not be dereferenced [-Wcheri-capability-misuse]
           fp = (FILE *)file;
                ^
   ```
   As the [CHERI C/C++ Programming Guide](https://www.cl.cam.ac.uk/techreports/UCAM-CL-TR-947.pdf)
   says in Section 4.2:
   > (...) only pointers implemented using valid capabilities can be dereferenced.
   
   and in Section 4.2.1:
   
   > `int`,`int32_t`,`long`,`int64_t`,... These pure integer types should be
   used to hold integer values that will never be cast to a pointer type without
   first combining them with another pointer value – e.g., by using them as an
   array offset.
   
   In our case, `long` is cast to a pointer type which results in an invalid
   capability.  We can fix this bug by using a data type that can hold both
   integer values and pointers - `uintptr_t`:
   ```
   diff --git a/src/exercises/adapt-c/cat/cat.c b/src/exercises/adapt-c/cat/cat.c
   index 344e505..54cc864 100644
   --- a/src/exercises/adapt-c/cat/cat.c
   +++ b/src/exercises/adapt-c/cat/cat.c
   @@ -166,10 +166,10 @@ scanfiles(char *argv[], int verbose)
                           rval = 1;
                   } else if (verbose) {
                           if (fd == STDIN_FILENO)
   -                               do_cat((long)stdin, verbose);
   +                               do_cat((uintptr_t)stdin, verbose);
                           else {
                                   fp = fdopen(fd, "r");
   -                               do_cat((long)fp, verbose);
   +                               do_cat((uintptr_t)fp, verbose);
                                   fclose(fp);
                           }
                   } else {
   diff --git a/src/exercises/adapt-c/cat/cat.h b/src/exercises/adapt-c/cat/cat.h
   index c88f930..047c0b7 100644
   --- a/src/exercises/adapt-c/cat/cat.h
   +++ b/src/exercises/adapt-c/cat/cat.h
   @@ -51,6 +51,6 @@
    
    #define SUPPORTED_FLAGS "belnstuv"
    
   -void do_cat(long file, int verbose);
   +void do_cat(uintptr_t file, int verbose);
    
    #endif /* !_CAT_H_ */
   diff --git a/src/exercises/adapt-c/cat/methods.c b/src/exercises/adapt-c/cat/methods.c
   index bb78a75..afe29d3 100644
   --- a/src/exercises/adapt-c/cat/methods.c
   +++ b/src/exercises/adapt-c/cat/methods.c
   @@ -71,7 +71,7 @@ write_off(int fildes, const void *buf, ptroff_t off, size_t nbyte)
    }
    
    static void
   -verbose_cat(long file)
   +verbose_cat(uintptr_t file)
    {
           FILE *fp;
           int ch, gobble, line, prev;
   @@ -166,7 +166,7 @@ ilseq:
    }
    
    static void
   -raw_cat(long file)
   +raw_cat(uintptr_t file)
    {
           long pagesize;
           int off, rfd, wfd;
   @@ -207,7 +207,7 @@ raw_cat(long file)
    }
    
    void
   -do_cat(long file, int verbose)
   +do_cat(uintptr_t file, int verbose)
    {
    
           if (verbose) {
   ```


8. The first bug resulted in a system call error because there was no capability
   operation on an invalid capability. An operating system could not copy memory
   from the user address space because it checked if a user process passed an
   invalid capability and returned an error.

   The second bug resulted in a CHERI exception because an invalid capability
   was used to load a word from memory.


For more information on the C/C++ programming languages, CHERI compiler warnings
and errors, we recommend to read the
[CHERI C/C++ Programming Guide](https://www.cl.cam.ac.uk/techreports/UCAM-CL-TR-947.pdf).
