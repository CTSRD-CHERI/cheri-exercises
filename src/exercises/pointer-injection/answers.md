# Answers

2. Expected output:
```
# ./long-over-pipe-baseline
received 42
# ./long-over-pipe-cheri
received 42
```

4. Expected output:
```
# ./ptr-over-pipe-baseline
received Hello world!
# ./ptr-over-pipe-cheri
In-address space security exception (core dumped)
```

5. Because the tag is stripped when sent via message-passing IPC, leading
   to a tag violation on dereference.
