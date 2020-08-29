obj-m += holamundo.o
obj-m += ejemplo2.o
obj-m += ejemplo3.o
obj-m += ejemplo4.o
obj-m += param_1.o

all:
# 	Para compilar el módulo de kernel en PC con Ubuntu:
# 	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

# 	Para compilar el módulo de kernel con el compilador cruzado para la Raspberry PI 3
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C /home/$(USER)/linux M=$(PWD) modules

clean:
# 	Para compilar el módulo de kernel en PC con Ubuntu:
# 	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

# 	Para compilar el módulo de kernel con el compilador cruzado para la Raspberry PI 3
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C /home/$(USER)/linux M=$(PWD) clean