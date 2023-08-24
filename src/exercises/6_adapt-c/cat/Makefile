.include "../../common.mk"

all: cat-baseline cat-cheri

cat-baseline: cat.c methods.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_RISCVABI)

cat-cheri: cat.c methods.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI)

clean:
	rm -f cat-baseline cat-cheri
