.include "../common.mk"

all: corrupt-pointer-baseline corrupt-pointer-cheri corrupt-capability

corrupt-pointer-baseline: corrupt-pointer.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_BASEABI)

corrupt-pointer-cheri: corrupt-pointer.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI)

clean:
	rm corrupt-pointer-baseline corrupt-pointer-cheri
