obj-m += holamundo.o
obj-m += ejemplo2.o
obj-m += ejemplo3.o
obj-m += ejemplo4.o
obj-m += param_1.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean