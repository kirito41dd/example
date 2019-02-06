#ifndef __MYSOCKET_H__
#define __MYSOCKET_H__
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>

void perr_exit(const char *s);//打印错误并退出
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
void Bind(int fd, const struct sockaddr *sa, socklen_t salen);
void Connect(int fd, const struct sockaddr *sa, socklen_t salen);
void Listen(int fd, int backlog);
int Socket(int family, int type, int protocol);
ssize_t Read(int fd, void *ptr, size_t nbytes);
ssize_t Write(int fd, void *ptr, size_t nbytes);
void Close(int fd);

#endif
