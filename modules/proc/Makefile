obj-m += proc.o

KDIR := /usr/src/linux-headers-$(shell uname -r)

PWD := $(shell pwd)

defaults:
	sudo $(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	sudo rm *.o *.ko *.order *.mod.c *.symvers
