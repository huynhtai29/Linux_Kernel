#include <stdio.h>
#include <pthread.h>
#include <time.h>


struct sinhvien{
	int age;
	char *name;

};
struct sinhvien ms;
void my_thread(void *my_argv)
{
	ms.age = 22;
	ms.name = "nguyen huynh tai";
	printf("hello : %s \n",(char *)my_argv);
	return ms;
}

void main()
{
	//struct sinhvien *ms;
	pthread_t id_thread;
	const char *name = "huynhtai";
	pthread_create(&id_thread,NULL,my_thread,name);
	pthread_join(id_thread,NULL);
	while(1)
	{
		printf("nam: %s, age: %d\n",ms.name,ms.age);
		sleep(1);
	}

}
