.include "../common.mk"

all: riscv_print_pointer cheriabi_print_pointer

riscv_print_pointer: print-pointer.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_RISCVABI)

cheriabi_print_pointer: print-pointer.c
	$(CC) -o $@ ${.ALLSRC} $(CFLAGS) $(CFLAGS_CHERIABI)

clean:
	rm -f riscv_print_pointer cheriabi_print_pointer
