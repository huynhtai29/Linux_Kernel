/* Nguyen Thanh Tung <tungnt58@fsoft.com.vn*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>

#define FILENAME	"/dev/led_device"

#define MAGIC_NO                100
#define MODE_NORMAL            _IOW(MAGIC_NO, 0, int)
#define MODE_BLYNK           _IOW(MAGIC_NO, 1, int)

/*
#define MODE_NORMAL 	1UL
#define MODE_BLYNK 	2
*/
struct data_config
{
    int tan_so;
    int time;
}typedef data_config;

int main(int argc, char *argv[])
{
	int option;
	char *cmd_send = NULL;
	int val_send = 0;
	int fd;
    	data_config mydata;
    	mydata.tan_so =5;
    	mydata.time= 100;
	unsigned int mode = 1;
	fd = open(FILENAME, O_RDWR);

	if (fd < 0) {
		perror("open\n");
		exit(-1);
	}
	printf("%d",(unsigned long)&mydata);
	ioctl(fd, MODE_NORMAL, &mydata);
	close(fd);
	return 0;
}
