.include "../common.mk"

all: cheri-allocator

cheri-allocator: cheri-allocator.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI)

clean:
	rm cheri-allocator
