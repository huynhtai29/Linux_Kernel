#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
	sleep(10);
	printf("id of C.c  %lld" ,(long long ) getpgid(0));
	return 0;
}