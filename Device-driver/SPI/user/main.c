#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>


#define FILENAME "/dev/spidev1_1"


void main()
{
	int fd;
	unsigned char buff[] = "99-99";
	fd = open(FILENAME,O_RDWR);
	if(fd <	0)
	{
		printf("can't open file\n");
		exit(1);
	}

	if(write(fd,buff,5) < 0	)
	{
		printf("can't write file \n");
	}

}
