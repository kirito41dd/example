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
	int sockfd;
	char str[INET_ADDRSTRLEN];
	char buf[MAXBYTES];
	int len;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
	bind(sockfd, (struct sockaddr*)&servaddr,sizeof(servaddr));
	printf("accepting connections ...\n");
	while(1){
		cliaddr_len = sizeof(cliaddr);
		len = recvfrom(sockfd, buf, sizeof(buf)-1, 0, 
				(struct sockaddr*)&cliaddr, &cliaddr_len);
		if(len != -1){
			buf[len]='\0';
			printf("received from %s at port %d : [%d]%s\n",
					inet_ntop(AF_INET, &cliaddr.sin_addr,str,sizeof(str)),
					ntohs(cliaddr.sin_port), len, buf);
		}
		//sleep(1);
	}
	close(sockfd);
	return 0;
}
