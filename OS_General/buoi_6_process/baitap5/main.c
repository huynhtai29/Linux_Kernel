#include <stdio.h>
#include <unistd.h>



void main()
{
	execl("/bin/ls","-l",(char*)0);
	printf("hello world \n");

}
