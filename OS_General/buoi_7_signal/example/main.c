#include <stdio.h>
#include <signal.h>

void sig_handler(int signum)
{
	printf("hello world\n");
}
void main()
{
	sigignore(SIGINT); // ignore signal ctr + d form terminal
	signal(12, sig_handler);
	while(1);
}
