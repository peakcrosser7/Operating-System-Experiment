CONFIG_MODULE_SIG=n
obj-m:=mydev.o
CURRENT_PATH:=$(shell pwd)
VERSION_NUM :=$(shell uname -r)
LINUX_PATH  :=/usr/src/linux-headers-$(VERSION_NUM)

all :   
	make -C $(LINUX_PATH) M=$(CURRENT_PATH) modules   
clean :   
	make -C $(LINUX_PATH) M=$(CURRENT_PATH) clean