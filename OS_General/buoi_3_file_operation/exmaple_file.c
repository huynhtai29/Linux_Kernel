#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#define SIZE_BYTE   100


int main()
{
	

	int fd;
	char a[SIZE_BYTE];
	fd = open("/home/debian/project/file_operation",O_RDONLY);
//	read(fd,a,SIZE_BYTE);
	if(fd < 0 )
	{
		printf("error open \n");
		return -1;
	}
	lseek(fd,2,SEEK_SET);
	read(fd,a,SIZE_BYTE);
	printf("%s",a);
	lseek(fd,2,SEEK_CUR);
	read(fd,a,SIZE_BYTE);
	printf("%s",a);
	close(fd);
/*
	DIR  *dir;
	struct dirent *dp;
	dir = opendir("/home/debian/project");
       while((dp = readdir(dir))!=NULL){
	printf("%s\n",dp->d_type);
       }
	closedir(dir);
*/
	return 0;
}
