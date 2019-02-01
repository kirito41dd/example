#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXBYTES 1024
#define PORT 8888

int main(int argc, char ** argv){
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddr_len;
	int listenfd, clifd;
	char str[INET_ADDRSTRLEN];
	char buf[MAXBYTES];
	int i,n,len;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
	bind(listenfd, (struct sockaddr*)&servaddr,sizeof(servaddr));
	listen(listenfd, 128);
	while(1){
		printf("accepting connections ...\n");
		cliaddr_len = sizeof(cliaddr);
		clifd = accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddr_len);//可能被信号打断
		if(clifd == -1){
			perror("accept");
			continue;
		}
		while(1){
			len = read(clifd, buf, sizeof(buf)-1);
			if(len > 0){
				buf[len]='\0';
				printf("accept form %s at port %d :[%d] %s\n",
						inet_ntop(AF_INET, &cliaddr.sin_addr, str,sizeof(str)),
						ntohs(cliaddr.sin_port), len, buf);
			}else if(len == 0){//写端关闭将会一直读到0
				close(clifd);
				printf("client closed\n");
				break;
			}else{
				perror("read");
			}
			sleep(1);
		}
	}
	close(listenfd);
	return 0;
}
