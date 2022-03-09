obj-m := usb1.o

all:
	$(MAKE) -C /usr/src/linux-headers-5.13.0-27-generic M=$(PWD) modules
clean:
	$(MAKE) -C /usr/src/linux-headers-5.13.0-27-generic M=$(PWD) clean
