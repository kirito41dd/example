#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include "mysocket.h"

#define MAXBYTES 1024
#define SERVER_PORT 8888

int main(int argc, char ** argv){
	struct sockaddr_in servaddr;
	int sockfd, n;
	char buf[MAXBYTES];
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1",&servaddr.sin_addr);
	servaddr.sin_port = htons(SERVER_PORT);
    Connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	while(fgets(buf,sizeof(buf),stdin)!=NULL){
		 Write(sockfd, buf, strlen(buf));
		 n = Read(sockfd, buf, sizeof(buf)-1);
		 if(n == 0){
			 printf("the other side has been closed\n");
		 }else{
			 buf[n] = 0;
			 printf("%s\n",buf);
		 }
	}
	close(sockfd);
	return 0;
}
