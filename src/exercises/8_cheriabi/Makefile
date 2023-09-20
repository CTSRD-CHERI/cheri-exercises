.include "../common.mk"

all: kern-read-over-baseline kern-read-over-cheri perm-vmem-baseline perm-vmem-cheri print-more-baseline print-more-cheri

kern-read-over-baseline: kern-read-over.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_RISCVABI)

kern-read-over-cheri: kern-read-over.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI)


perm-vmem-baseline: perm-vmem.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_RISCVABI)

perm-vmem-cheri: perm-vmem.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI)

print-more-baseline: print-more.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_RISCVABI)

print-more-cheri: print-more.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI)

clean:
	rm kern-read-over-baseline kern-read-over-cheri perm-vmem-baseline perm-vmem-cheri print-more-baseline print-more-cheri
