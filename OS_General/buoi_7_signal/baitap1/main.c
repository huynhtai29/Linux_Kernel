#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void main()
{

	sigset_t setmask,oldmask,pendmask;
	sigemptyset(&setmask);
	sigaddset(&setmask,SIGINT);
	if(sigprocmask(SIG_BLOCK,&setmask,&oldmask)<0)
	{
		perror("SIGINT ERROR\n ");
	}
	while(1)
	{
		if(sigpending(&pendmask) < 0)
		perror("PENDING ERROR\n");
		if(sigismember(&pendmask,SIGINT))
		{
			printf("SIGINT pending\n");
			exit(1);
		}	
	}

}
