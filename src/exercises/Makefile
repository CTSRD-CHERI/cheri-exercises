.include "common.mk"

all:
	cd 1_compile-and-run && $(MAKE)
	cd 2_cheri-tags && $(MAKE)
	cd 3_buffer-overflow-stack && $(MAKE)
	cd 4_subobject-bounds && $(MAKE)
	cd 5_buffer-overflow-heap && $(MAKE)
	cd 6_adapt-c/cat && $(MAKE)
	cd 7_cheri-allocator && $(MAKE)
	cd 8_cheriabi && $(MAKE)
