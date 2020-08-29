obj-m += ddc-1.o
obj-m += ddc-2.o
obj-m += ddc-3.o
obj-m += ddc-4.o
obj-m += ddc-5.o
obj-m += ddc-6.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean