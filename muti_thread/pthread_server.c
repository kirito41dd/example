#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/wait.h>
#include "mysocket.h"
#include <pthread.h>
#define MAXBYTES 1024
#define PORT 8888

void* do_work(void *arg){
	int n,i,listenfd,connfd;
	socklen_t cliaddr_len;
	struct sockaddr_in cliaddr;
	pthread_detach(pthread_self());
	cliaddr_len = sizeof(cliaddr);
	listenfd = *((int *)arg);
	connfd = Accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddr_len);
	char buf[MAXBYTES],str[INET_ADDRSTRLEN];
	while(1){
		n = Read(connfd, buf,sizeof(MAXBYTES));
		if(n==0){
			printf("the other side has been closed\n");
			break;
		}
		printf("%lx received %d cahr from %s at PORT %d\n",(long)pthread_self(), n,
			inet_ntop(AF_INET, &cliaddr, str, sizeof(str)),
			ntohs(cliaddr.sin_port));
		for(i = 0; i<n; i++){
			buf[i] = toupper(buf[i]);
		}
		Write(connfd, buf, n);
	}
	Close(connfd);
}


int main(int argc, char ** argv){
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddr_len;
	int listenfd, connfd;
	int i=0;
	pthread_t tid;
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
	Bind(listenfd, (struct sockaddr*)&servaddr,sizeof(servaddr));
	Listen(listenfd, 128);
	printf("Accepting connections ...\n");
	for(i=0;i<36;i++){
		pthread_create(&tid, NULL, do_work, (void*)&listenfd);
		//线程池
	}
	while(1)sleep(10);
	return 0;
}
