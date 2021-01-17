CONFIG_MODULE_SIG=n
obj-m:=myModule.o
# generate the path
CURRENT_PATH:=$(shell pwd)
# current linux kernel version
VERSION_NUM :=$(shell uname -r)
# linux path
LINUX_PATH  :=/usr/src/linux-headers-$(VERSION_NUM)
#compile object
all :   
	make -C $(LINUX_PATH) M=$(CURRENT_PATH) modules   
#clean
clean :   
	make -C $(LINUX_PATH) M=$(CURRENT_PATH) clean