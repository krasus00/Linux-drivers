obj-m := first_driver.o

all:
	$(MAKE) -C /usr/src/linux-headers-$(shell uname -r) M=$(PWD) modules
clean:
	$(MAKE) -C /usr/src/linux-headers-$(shell uname -r) M=$(PWD) clean
