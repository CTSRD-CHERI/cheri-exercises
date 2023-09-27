.include "../../common.mk"

all: cat-baseline cat-cheri

cat-baseline: cat.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_BASEABI)

cat-cheri: cat.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI)

clean:
	rm cat-baseline cat-cheri
