##注意情况  
tcp下：    
1. 当socket写端关闭的时候，读端read将一直读取到0字节，可以使用这一特性来关闭读端套接字。   
2. 当socket读端关闭的时候，写端write继续写入数据，将会报错，并且收到一个SIGPIPE信号。    
3. 当网络异常的时候（如拔掉网线），这个时候read不会返回错误，将阻塞等待数据；write也不会返回错误，仍然可以写入数据。   
4. socket有个buffer，并且这个buffer的大小是一定的，当一直往socket write的时候，达到这个buffer的最大值后，write将阻塞。   
