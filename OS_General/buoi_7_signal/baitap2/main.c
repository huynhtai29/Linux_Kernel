#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void sig_handler(int signum)
{
	printf("hello wolrd");
}

void main()
{

	signal(12,sig_handler);	
	sigset_t setmask,oldmask,pendmask;
	sigemptyset(&setmask);
	sigaddset(&setmask,12);

	if(sigprocmask(SIG_BLOCK,&setmask,&oldmask)<0)
	{
		perror("SIGINT ERROR\n ");
	}
	while(1)
	{
		int a;
		scanf("%d",&a);
		if(a == 1)
		sigprocmask(SIG_UNBLOCK,&setmask,&oldmask);
		if(sigpending(&pendmask) < 0)
		perror("PENDING ERROR\n");
		if(sigismember(&pendmask,12))
		{
			printf("SIGINT pending\n");
			
		}	
	}

}
