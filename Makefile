CFLAGS_MODULE += -Werror
CFLAGS_MODULE += -DDEBUG

ARCH := x86
KERNELDIR := /lib/modules/`uname -r`/build

PWD       := $(shell pwd)

obj-m	:= nf_dummy.o

default:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions
