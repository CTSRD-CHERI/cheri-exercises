CC=clang

ARCH!=uname -m
.if ${ARCH} == "arm64"
CFLAGS=-g -O2 -target aarch64-unknown-freebsd -mno-relax -Wall -Wcheri
CFLAGS_CHERIABI=-march=morello+noa64c
CFLAGS_CAPABI=-march=morello+c64 -mabi=purecap -Xclang -morello-vararg=new
.else
CFLAGS=-g -O2 -target riscv64-unknown-freebsd -mno-relax -Wall -Wcheri
CFLAGS_CHERIABI=-march=rv64gc -mabi=lp64d 
CFLAGS_CAPABI=-march=rv64gcxcheri -mabi=l64pc128d
.endif

.PHONY: all clean
