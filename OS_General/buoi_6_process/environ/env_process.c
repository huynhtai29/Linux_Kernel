#include <stdio.h>

extern char **environ;

void main()
{
	int i=0;
	for(i = 0;environ[i]!= NULL; i++)
		printf("%s\n",environ[i]);

}
