# Answers - Exercise an inter-global-object buffer overflow

2. Expected output:
   ```
   # ./buffer-overflow-global-baseline
   c = c
   c = b
   # ./buffer-overflow-global-cheri
   c = c
   In-address space security exception (core dumped)
   ```

3. Example session:
   ```
   Program received signal SIGPROT, CHERI protection violation
   Capability bounds fault caused by register ca4.
   fill_buf (buf=0x104160 <buffer> [rwRW,0x104160-0x1041e0] 'b' <repeats 128 times>, "c", len=128) at buffer-overflow-global.c:15
   15                      buf[i] = 'b';
   (gdb) info reg ca4
   ca4            0xf17d00000479816400000000001041e0       0x1041e0 <c> [rwRW,0x104160-0x1041e0]
   (gdb) x/i $pcc
   => 0x101d2c <fill_buf+12>:      csb     a3,0(ca4)
   ```
   The array has been incremented beyond the end of the allocation as out
   of bounds store has been attempted (`Capability bounds fault`).

5. Expected output:
   ```
   # ./buffer-overflow-global-cheri
   c = c
   c = c
   ```
   To see why this occurs, examine the bounds of the buffer in `fill_buf`.
   ```
   (gdb) b fill_buf
   Breakpoint 1 at 0x101d26: file buffer-overflow-global.c, line 15.
   (gdb) r
   Starting program: /root/buffer-overflow-global-cheri
   c = c

   Breakpoint 1, fill_buf (buf=0x105000 <buffer> [rwRW,0x105000-0x205800] "", len=1048577) at buffer-overflow-global.c:15
   15                      buf[i] = 'b';
   ```
   This indicates that buffer has been allocated (1024 * 1026) bytes. This
   is due to the padding required to ensure that the bounds of `buffer`
   don't overlap with other allocations. As a result, there as an area beyond
   the end of the C-language object that is nonetheless in bounds.

6. Solution:
   ```diff
   --- buffer-overflow-global.c
   +++ buffer-overflow-global.c
   @@ -6,7 +6,7 @@ char c;
    void
    fill_buf(char *buf, size_t len)
    {
   -       for (size_t i = 0; i <= len; i++)
   +       for (size_t i = 0; i < len; i++)
                   buf[i] = 'b';
    }
   ```

7. Expected output:
   ```
   # ./buffer-overflow-global-cheri
   c = c
   c = c
   ```
