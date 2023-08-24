CC=clang
CFLAGS=-g -O2 -target riscv64-unknown-freebsd -mno-relax -Wall -Wcheri
CFLAGS_RISCVABI=-march=rv64gc -mabi=lp64d 
CFLAGS_CHERIABI=-march=rv64gcxcheri -mabi=l64pc128d

.PHONY: all clean
