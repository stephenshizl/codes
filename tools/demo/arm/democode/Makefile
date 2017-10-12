CROSS_COMPILE = arm-rkcvr-linux-uclibcgnueabihf-

CC = ${CROSS_COMPILE}gcc
STRIP = ${CROSS_COMPILE}strip

CFLAGS = -static -lrt

TARGETS = democode

ALL: ${TARGETS}

%: %.c
	${CC} $< -o $@ ${CFLAGS}
	${STRIP} $@
