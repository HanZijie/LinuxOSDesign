ifneq ($(KERNELRELEASE),)    
#kbuild syntax.
mymodule-objs :=mchar.o
obj-m :=mchar.o
# mymodule-objs :=try.o
# obj-m :=try.o
else
PWD :=$(shell pwd)
KVER :=$(shell uname -r)
KDIR :=/lib/modules/$(KVER)/build
all:
	$(MAKE) -C $(KDIR) M=$(PWD)
clean:
	rm -f *.cmd *.o *.mod *.ko *.cmd
endif