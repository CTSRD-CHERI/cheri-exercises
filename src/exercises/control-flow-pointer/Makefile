.include "../common.mk"

all: buffer-overflow-fnptr-baseline buffer-overflow-fnptr-cheri

buffer-overflow-fnptr-baseline: buffer-overflow-fnptr.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_BASEABI)

buffer-overflow-fnptr-cheri: buffer-overflow-fnptr.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI)

clean:
	rm buffer-overflow-fnptr-baseline buffer-overflow-fnptr-cheri
