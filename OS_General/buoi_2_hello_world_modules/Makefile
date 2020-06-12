obj-m +=hello_world.o\
	hello_world1.o


all:
	make -C /usr/src/linux-headers-4.14.71-ti-r80  M=$(PWD) modules
clean:
	make -C /usr/src/linux-headers-4.14.71-ti-r80  M=$(PWD) clean
