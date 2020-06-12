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
    const char* message = "hello world\n";
    struct sinhvien *ptr;    
    shm_fd = shm_open("huynhtai_2",O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd,4096);
    ptr = (struct sinhvien *)mmap(0,4096,PROT_WRITE,MAP_SHARED,shm_fd,0);
    //strcpy(ptr,message);
    struct sinhvien *_val;
    sprintf(ptr->name,"nguyen huynh tai");
    ptr->age  =23;
    printf("%s",ptr->name);
    //ptr = _val;
    

    return 0;
}