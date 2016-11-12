obj-m	:= build_modules.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	rm -rf *.o *~ core .depend .*.cmd *.mod.c .tmp_versions
	rm -rf Module.symvers modules.order

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
