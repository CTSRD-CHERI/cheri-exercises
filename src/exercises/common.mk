CC=cc
CFLAGS=-g -O2 -Wall -Wcheri

ARCH!=uname -m
.if ${ARCH} == "arm64"
CFLAGS_BASELINE=-march=morello+noa64c -mabi=aapcs
CFLAGS_CHERI=-march=morello -mabi=purecap
.else
CFLAGS_BASELINE=-march=rv64gc -mabi=lp64d
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
