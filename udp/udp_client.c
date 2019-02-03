#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>

#define MAXBYTES 1024
#define SERVER_PORT 8888

int main(int argc, char ** argv){
	struct sockaddr_in servaddr,myaddr;
	int sockfd, len;
	socklen_t myaddr_len;
	char buf[MAXBYTES]="helloworld";
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1",&servaddr.sin_addr);
	servaddr.sin_port = htons(SERVER_PORT);
	memset(&myaddr,0,sizeof(myaddr));	
	while(1){
		len = sendto(sockfd, buf, strlen(buf), 0, 
				(struct sockaddr*)&servaddr, sizeof(servaddr));
		if(len == -1){
			perror("sendto");
			exit(1);
		}
		getsockname(sockfd,(struct sockaddr*)&myaddr, &myaddr_len);
		printf("port %d\n",ntohs(myaddr.sin_port));
		
		sleep(1);
	}
	close(sockfd);
	return 0;
}
