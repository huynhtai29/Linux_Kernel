#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int fd;
	fd = open(argv[1],O_RDONLY);
	if(fd == -1)
	{
		perror("file not exit");
		exit(-1);
	}
	// get info file
	struct stat  mystat;
	fstat(fd,&mystat);
	printf("size file %ld \n",mystat.st_size);
	char cmd[100] = "";
	char _val = 0;
	int size = 0;
	int index = 0;
	while ((size += (read(fd,&_val,1))) <  mystat.st_size)
	{
		if(_val == '\n')
		{
			printf("%s \n",cmd);
			if(cmd[0] == '#')
			{
				memset(cmd,0,sizeof(cmd));
				index = 0;
			}
			else
			{
				system(cmd);
				memset(cmd,0,sizeof(cmd));
				index = 0;
			}
		}
		else
		{
			cmd[index]  = _val;
			index++;
		}
		_val = 0;
	}
	close(fd);
	return 0;
}

