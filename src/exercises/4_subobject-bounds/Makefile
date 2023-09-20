.include "../common.mk"

all: buffer-overflow-subobject-baseline buffer-overflow-subobject-cheri buffer-overflow-subobject-cheri-subobject-safe subobject-list-cheri subobject-list-cheri-subobject-safe subobject-list-cheri-subobject-safe-use-cdefs

buffer-overflow-subobject-baseline: buffer-overflow-subobject.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_RISCVABI)

buffer-overflow-subobject-cheri: buffer-overflow-subobject.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI)

buffer-overflow-subobject-cheri-subobject-safe: buffer-overflow-subobject.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI) -Xclang -cheri-bounds=subobject-safe
        
        
subobject-list-cheri: subobject-list.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI)

subobject-list-cheri-subobject-safe: subobject-list.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI) -Xclang -cheri-bounds=subobject-safe

subobject-list-cheri-subobject-safe-use-cdefs: subobject-list.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI) -Xclang -cheri-bounds=subobject-safe -DUSE_CDEFS_CONTAINEROF

clean:
	rm buffer-overflow-subobject-baseline buffer-overflow-subobject-cheri buffer-overflow-subobject-cheri-subobject-safe subobject-list-cheri subobject-list-cheri-subobject-safe subobject-list-cheri-subobject-safe-use-cdefs
