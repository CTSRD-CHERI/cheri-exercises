CC=clang

ARCH!=uname -m
.if ${ARCH} == "arm64"
CFLAGS=-g -O2 -target aarch64-unknown-freebsd -mno-relax -Wall -Wcheri
CFLAGS_CHERI=-march=morello+noa64c -march=morello+c64 -mabi=purecap -Xclang -morello-vararg=new
.else
CFLAGS=-g -O2 -target riscv64-unknown-freebsd -Wall -Wcheri -mno-relax
CFLAGS_CHERI=-march=rv64gcxcheri -mabi=l64pc128d 
.endif

.for progbase in ${PROGBASES}
.for variant in baseline cheri
CLEANFILES+=	${progbase}-${variant}

all: ${progbase}-${variant}

${progbase}-${variant}: ${progbase}.c
	${CC} -o ${.TARGET} ${.ALLSRC} ${CFLAGS} ${CFLAGS_${variant:tu}}
.endfor
.endfor

clean:
	rm -f ${CLEANFILES}

.PHONY: all clean
