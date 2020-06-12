#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


void main()
{
    int fd;
    char path[] = "/dev/pts/";
    char terminal[20] = "";
    for(int i=1 ; i<4;i++)
    {
        memset(terminal,0,sizeof(terminal));
        strcpy(terminal,path);
        terminal[sizeof(path)-1] = i+48;
        printf("%s\n",terminal);
        fd = open(terminal,O_RDWR);
        if(fd < 0)
        {
            printf("error open file");
            exit(-1);
        }
        write(fd,"tai\n",3);        
    }


}