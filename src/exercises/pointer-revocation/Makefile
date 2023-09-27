.include "../common.mk"

all: temporal-control-baseline temporal-control-cheri

temporal-control-baseline: temporal-control.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_BASEABI)

temporal-control-cheri: temporal-control.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI)

clean:
	rm temporal-control-baseline temporal-control-cheri
