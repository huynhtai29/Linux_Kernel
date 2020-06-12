#include "stdio.h"
#include <linux/watchdog.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <asm/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
void main()
{
	int fd = 0;
	fd = open("/dev/watchdog1",O_RDWR);
	if(fd < 0)
	{
		perror("Can't open file /dev/watchdog1\n");
	}
	int timeout = 60;
	ioctl(fd, WDIOC_SETTIMEOUT,&timeout);

	while(1);
}

