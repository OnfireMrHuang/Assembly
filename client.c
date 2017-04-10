/*
客户端流程：
1、使用socket得到套接字文件描述符
2、不调用bind函数，使用系统默认分配的ip和端口
3、使用connent来链接服务器
4、使用write发送数据
5、使用read读取数据
6、处理接收数据
7、如果退出则发送退出消息到服务器

author: hww
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6666

int  main(int argc,char *argv[])
{
    int sd;
    struct sockaddr_in client_addr;
    char buff[BUFSIZ];  
    socklen_t client_addr_len;
    int n;

    sd = socket(AF_INET,SOCK_STREAM,0);

    client_addr_len = sizeof(client_addr);
    memset(&client_addr,0,client_addr_len);
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET,SERVER_IP,(struct sockaddr *)&client_addr.sin_addr.s_addr);
    connect(sd,(struct sockaddr *)&client_addr,client_addr_len);

    for (;;)
    {
        fgets(buff,sizeof(buff),stdin);
        write(sd,buff,strlen(buff));
        if(strncasecmp(buff,"exit",4)==0 || strncasecmp(buff,"quit",4)==0){
            break;
        }
        n = read(sd,buff,sizeof(buff));
        write(STDOUT_FILENO,buff,n);
    }
    close(sd);
    return 0;
}