.include "../common.mk"

all: long-over-pipe-baseline long-over-pipe-cheri

long-over-pipe-baseline: long-over-pipe.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_BASEABI)

long-over-pipe-cheri: long-over-pipe.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI)

clean:
	rm long-over-pipe-baseline long-over-pipe-cheri
