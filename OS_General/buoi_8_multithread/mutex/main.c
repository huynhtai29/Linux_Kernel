#include <stdio.h>
#include <pthread.h>
#include <time.h>

FILE *fp;
int a;

static pthread_mutex_t mutex_a;
struct sinhvien{
	int age;
	char *name;

};
struct sinhvien ms;
void my_thread(void *my_argv)
{
	for(int i = 0; i<100000;i++)
	{
		pthread_mutex_lock(&mutex_a);
		a++;
		pthread_mutex_unlock(&mutex_a);
	}
}

void main()
{
	//struct sinhvien *ms;
	//fp = fopen("data.txt","a");
	pthread_mutex_init(&mutex_a,NULL);
	pthread_t id_thread;
	const char *name = "huynhtai";
	pthread_create(&id_thread,NULL,my_thread,name);
	for(int i = 0; i<100000;i++)
	{
		pthread_mutex_lock(&mutex_a);
		a++;
		pthread_mutex_unlock(&mutex_a);
	}
	pthread_join(id_thread,NULL);
	printf("value %d",a);
	pthread_mutex_detroy(&mutex_a);
	//fclose(fp);
}
