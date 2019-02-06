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
#define MAXBYTES 1024
#define PORT 8888

void do_sigchild(int num){
	while(waitpid(-1, NULL, WNOHANG)>0);
}

int main(int argc, char ** argv){
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddr_len;
	int listenfd, connfd;
	char str[INET_ADDRSTRLEN];
	char buf[MAXBYTES];
	int i,n;
	pid_t pid;
	signal(SIGCHLD, do_sigchild);
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
	Bind(listenfd, (struct sockaddr*)&servaddr,sizeof(servaddr));
	Listen(listenfd, 128);
	printf("Accepting connections ...\n");
	while(1){
		cliaddr_len = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddr_len);
		pid = fork();
		if(pid ==0){
			Close(listenfd);
			while(1){
				n = Read(connfd, buf, sizeof(buf)-1);
				if(n == 0){
					printf("the other side has been closed\n");
					break;
				}
				buf[n] = '\0';
				printf("received form %s at port %d :[%d] %s\n",
					inet_ntop(AF_INET, &cliaddr.sin_addr, str,sizeof(str)),
					ntohs(cliaddr.sin_port), n, buf);
				for(i=0; i<n; i++){
					buf[i] = toupper(buf[i]);
				}
				Write(connfd, buf, n);
			}
			close(connfd);
			return 0;
		}else if(pid >0){
			Close(connfd);
		}else{
			perr_exit("fork");
		}
	}
	Close(listenfd);
	return 0;
}
