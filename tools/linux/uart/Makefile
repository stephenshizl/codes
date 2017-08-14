prog	= uart
CC		= arm-linux-gcc
#CC		= gcc
CFLAGS	= -static

FILES	= uart.c

$(prog)	: $(FILES)
	$(CC) $(CFLAGS) $(EXFLAGS) $(FILES) -o $@
