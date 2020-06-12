#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
int main()
{
	int fd, pid;
	pid = fork();
	int i = i+10;
	char *child = "i am child";
	char *parent = "i am parent";
	fd = open("tai.txt",O_RDWR|O_CREAT);
	if(pid < 0 )
	{
		perror("fork");
		return -1;
	}
	else if(pid == 0)
	{

		printf("child %d \n",i);
//		printf("i am child %d ipid : %d\n",pid,getpid());
		write(fd,child,sizeof(child));
	}
	else
	{
		printf("parent %d \n",i);
//		printf("i am parent %d pid : %d\n", pid,getpid());
		write(fd,parent,sizeof(parent));

	}
	close(fd);
	return 0;
}
