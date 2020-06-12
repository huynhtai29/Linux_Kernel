#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include "wrapper.h"

FILE *fp;
int a;
char *data = "huynhtai";
static pthread_mutex_t mutex_a;
struct sinhvien{
	int age;
	char *name;

};
struct sinhvien ms;
void my_thread(void *my_argv)
{
	for(int i = 0; i<10;i++)
	{
		write_file("hello world");
	}
}

void main()
{
	//struct sinhvien *ms;
	open_file();
	pthread_t id_thread;
	const char *name = "huynhtai";
	pthread_create(&id_thread,NULL,my_thread,name);
	for(int i = 0; i<10;i++)
	{
		write_file("hello world");
	}
	printf("value %d",a);
	close_file();
}
