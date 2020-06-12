#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <string.h>

int main()
{
	int connfd;
	int bind_fd;
	int server_fd;
	struct sockaddr_in server_addr;
	char send_buffer[1024];
	char recv_buffer[1024];
	memset(send_buffer,0,sizeof(send_buffer));
	memset(recv_buffer,0,sizeof(recv_buffer));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("192.168.1.6");
	server_addr.sin_port = htons(5000);
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd < 0)
		printf("Erorr open socket\n");
	else
		printf("Success open socket\n");	
	if(bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0)
		printf("Error bind");
	listen(server_fd,10);
	connfd = accept(server_fd,(struct sockaddr*)NULL,NULL);	
	while (1)
	{

		while(read(connfd, recv_buffer,sizeof(recv_buffer)-1) < 0);
		printf("%s\n",recv_buffer);
//		gets(send_buffer);
//		write(connfd, send_buffer, strlen(send_buffer));
		memset(recv_buffer, 0, sizeof(recv_buffer));
//		memset(send_buffer, 0, sizeof(send_buffer));

	}
		close(connfd);	
	close(server_fd);
	return 0;
}
