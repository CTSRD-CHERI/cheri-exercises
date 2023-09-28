.include "../common.mk"

all: print-pointer-baseline print-pointer-cheri print-capability

print-pointer-baseline: print-pointer.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_BASEABI)

print-pointer-cheri: print-pointer.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI)

print-capability: print-capability.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CAPABI)

clean:
	rm print-pointer-baseline print-pointer-cheri print-capability
