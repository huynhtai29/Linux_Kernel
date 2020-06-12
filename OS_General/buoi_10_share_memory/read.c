#include <stdio.h>
#include <stdlib.h> 
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
struct sinhvien
{
    int age;
    char name[100];
};
int main()
{
    int shm_fd;
    struct sinhvien *ptr = NULL;
    shm_fd = shm_open("huynhtai_2",O_RDONLY,0666);
    ptr = (struct sinhvien*)mmap(0,4096,PROT_READ,MAP_SHARED,shm_fd,0);
    printf("%d\n",ptr->age);
    printf("%s",ptr->name);
    shm_unlink("huynhtai_2");

    return 0;
}