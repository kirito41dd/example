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

void sig_handler(int sig){
	printf("SIGPIPE\n");
	exit(1);
}

int main(int argc, char ** argv){
	signal(SIGPIPE, sig_handler);//当读端关闭的时候，继续写会触发SIG_PIEP信号
	struct sockaddr_in servaddr;
	int sockfd, n, ret;
	char buf[]="helloworld";
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1",&servaddr.sin_addr);
	servaddr.sin_port = htons(SERVER_PORT);
	int times = 0;
	while(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
		if(times++ > 10){//连接可能失败，设置重连
			return -1;
		}
		sleep(1);
		perror("connect");
	}

	int num = 0;
	while(1){
		ret = write(sockfd, buf, strlen(buf));
		if(ret == -1){
			perror("write");
		}else{
			num += ret;
		}
		printf("send bytes %d\n", num);
		sleep(1);
	}
	close(sockfd);
	return 0;
}
