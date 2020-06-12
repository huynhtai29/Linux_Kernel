#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
int main()
{


	/* Creat gpio */
	char *gpio ="86";
	int fd = open("/sys/class/gpio/export",O_RDWR);
	write(fd,gpio,sizeof(gpio));
	sleep(1);
	/* Set mode gpio
	 * gpio_set = "out" --> Set gpio mode output
	 * gpio_set = "in" --> set gpio mode inpunr
	 **/
	char *gpio_set = "out";
	int fd_dir = open("/sys/class/gpio/gpio86/direction",O_RDWR);
	write(fd_dir,gpio_set,sizeof(gpio_set));
	/* Set value gpio in mode output*/
	char *set_dataout = "1";
	int fd_set_data = open("/sys/class/gpio/gpio86/value",O_RDWR);
	write(fd_set_data,set_dataout,sizeof(set_dataout));
	sleep(1);
	set_dataout ="0";
	write(fd_set_data,set_dataout,sizeof(set_dataout));
	sleep(1);
	close(fd);
	close(fd_dir);
	close(fd_set_data);
	return 0;
}
