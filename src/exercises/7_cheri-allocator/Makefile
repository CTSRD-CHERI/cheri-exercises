.include "../common.mk"

all: allocator-cheri

allocator-cheri: cheri-allocator.c
	$(CC) -o $@ $^ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI)

clean:
	rm allocator-cheri
