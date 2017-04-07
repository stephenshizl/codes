CROSS_COMPILE = arm-boeye-linux-gnueabi-

CC = ${CROSS_COMPILE}gcc
STRIP = ${CROSS_COMPILE}strip

CFLAGS = -static -lrt

TARGETS = input_by_name

ALL: ${TARGETS}

%: %.c
	${CC} $< -o $@ ${CFLAGS}
	${STRIP} $@
