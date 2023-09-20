.include "../common.mk"

all: cheri-allocator.c

allocator-cheri: cheri-allocator.c
	$(CC) -o $@ $^ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI)

clean:
	rm allocator-cheri
