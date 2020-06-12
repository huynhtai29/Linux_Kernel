#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
static FILE *fd;
static int check;
static const char *file = "data.txt";
pthread_mutex_t mutex_key;
void open_file();
void read_file(char *buff,int size);
void write_file(char *buff);
void close_file();


