.include "../common.mk"

all: buffer-overflow-stack-baseline buffer-overflow-stack-cheri

buffer-overflow-stack-baseline: buffer-overflow-stack.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_RISCVABI)

buffer-overflow-stack-cheri: buffer-overflow-stack.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI)

clean:
	rm -f buffer-overflow-stack-baseline buffer-overflow-stack-cheri
