.include "../common.mk"

all: print-pointer-baseline print-pointer-cheri print-capability

print-pointer-baseline: print-pointer.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_RISCVABI)

print-pointer-cheri: print-pointer.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI)
	
print-capability: print-capability.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI)

clean:
	rm print-pointer-baseline print-pointer-cheri print-capability
