.include "../common.mk"

all: union-int-ptr-baseline union-int-ptr-cheri

union-int-ptr-baseline: union-int-ptr.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_BASEABI)

union-int-ptr-cheri: union-int-ptr.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI)

clean:
	rm union-int-ptr-baseline union-int-ptr-cheri
