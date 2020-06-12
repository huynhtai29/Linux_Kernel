#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "wrapper.h"

void open_file()
{
	if(check == 0)
	{
		pthread_mutex_init(&mutex_key,NULL);
		fd = fopen("data.txt","a");
		if(fd < 0 )
		{
			perror("error open file\n");	
		}
		check = 1;
	}
}

void read_open(char *buff,int size)
{
	pthread_mutex_lock(&mutex_key);
	buff = fread(buff,1,size,fd);
	pthread_mutex_unlock(&mutex_key);
}

void write_file(char *buff)
{
	pthread_mutex_lock(&mutex_key);
	fwrite(buff,1,sizeof(buff),fd);
	pthread_mutex_unlock(&mutex_key);
}
void close_file()
{
	if(check == 1)
	{
		pthread_mutex_destroy(&mutex_key);
		fclose(fd);
		check = 0;
	}
}