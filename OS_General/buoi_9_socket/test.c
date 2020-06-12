#include <stdio.h>
#include <unistd.h>
#include <time.h>

void main()
{
    int i, n;
    time_t t;
    char buff[1024];
    n = 5;
    
    /* Khoi tao bo sinh so ngau nhien */
    srand((unsigned) time(&t));

    /* in 5 so ngau nhien trong day tu 0 toi 49 */
    for( i = 0 ; i < 1000 ; i++ ) 
    {
         sprintf(buff,"%d",rand() % 50);
         printf("%s\n",buff);
         usleep(10000);
    }

}