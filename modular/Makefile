
obj-m += my_module.o
all:
	sudo make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean: 
	sudo rm *.order *.symvers *.mod.c *.ko *.o *.mod.o
