.include "../common.mk"

all: buffer-overflow-subobject-baseline buffer-overflow-subobject-cheri

buffer-overflow-subobject-baseline: buffer-overflow-subobject.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_BASEABI)

buffer-overflow-subobject-cheri: buffer-overflow-subobject.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI)

clean:
	rm buffer-overflow-subobject-baseline buffer-overflow-subobject-cheri
