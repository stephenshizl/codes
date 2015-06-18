prog	= fb
CC		= arm-boeye-linux-gnueabi-gcc
#CC		= gcc
CFLAGS	= -static
#EXFLAGS	= -DCONFIG_LIBPNG -lpng

FILES	= fb.c ebc.c
#FILES	= disfb.c

$(prog)	: $(FILES)
	$(CC) $(CFLAGS) $(EXFLAGS) $(FILES) -o $@
