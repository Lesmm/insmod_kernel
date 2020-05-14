ifneq ($(KERNELRELEASE),)
# 	obj-m :=netl.o
# 	obj-m :=mmap_kernel.o
# 	obj-m :=mmkernel.o
# 	obj-m :=mymap_kernel.o
	obj-m :=pure_netlink_kernel.o
else
	KERNELDIR ?=/lib/modules/$(shell uname -r)/build
	PWD :=$(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif