obj-m += ejemplo_hilo.o
obj-m += ejemplo_hilo1.o
obj-m += ejemplo_hilo2.o
obj-m += ejemplo_hilo3.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean