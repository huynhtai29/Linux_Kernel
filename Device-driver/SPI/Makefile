obj-m +=spi_max_driver.o
spi_max_driver-objs := max7219.o spi_max7219.o



all:
	make -C /usr/src/linux-headers-4.14.71-ti-r80  M=$(PWD) modules
clean:
	make -C /usr/src/linux-headers-4.14.71-ti-r80  M=$(PWD) clean
