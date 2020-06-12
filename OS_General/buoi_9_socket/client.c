#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

void main()
{
	int sockfd = -1;
	int connectfd = -1;
	struct sockaddr_in server_addr;
	char recv_buffer[1024];
	char send_buffer[10];
	time_t t;
	srand((unsigned) time(&t));
	memset(recv_buffer, 0, sizeof(recv_buffer));
	memset(send_buffer, 0, sizeof(send_buffer));
	recv_buffer[1] = '1';
//	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("192.168.1.8");
	server_addr.sin_port = htons(7000);
//	connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		printf("error server\n");
	}	
	else
		printf("success server\n");
	connectfd = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(connectfd <0)
		printf("error connect\n");
	else 
		printf("success connect\n");
	while(1)
	{
		for(int i = 0;i<=100;i++){
		sprintf(send_buffer,"%d\n\r",rand()%50);
//		gets(send_buffer);
//		for(int i =0;i<1000;i++){
//		write(sockfd, send_buffer, strlen(send_buffer));
		send(sockfd,send_buffer,sizeof(send_buffer),0);
		printf("%s\n",send_buffer);
		usleep(10000);
		memset(send_buffer, 0, sizeof(send_buffer));

		}
		break;
//		}
//		while(read(sockfd, recv_buffer, sizeof(recv_buffer)-1)<0);
//		printf("%s\n", recv_buffer);		
//		memset(recv_buffer, 0, sizeof(recv_buffer));
//		memset(send_buffer, 0, sizeof(send_buffer));		
	
	}	
	close(sockfd);
}
