obj-m +=misc-module.o


all:
	make -C /usr/src/linux-headers-4.14.71-ti-r80  M=$(PWD) modules
	gcc read.c -o read
clean:
	make -C /usr/src/linux-headers-4.14.71-ti-r80  M=$(PWD) clean
	rm -rf read
