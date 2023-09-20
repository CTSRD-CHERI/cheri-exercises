.include "../common.mk"

all: buffer-overflow-heap-baseline buffer-overflow-heap-cheri

buffer-overflow-heap-baseline: buffer-overflow-heap.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_RISCVABI)

buffer-overflow-heap-cheri: buffer-overflow-heap.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI)

clean:
	rm buffer-overflow-heap-baseline buffer-overflow-heap-cheri
