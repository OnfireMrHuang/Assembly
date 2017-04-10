/*
服务器流程：
1、使用socket得到套接字文件描述符
2、使用bind绑定IP和端口
3、listen:设置同时访问服务器最大的客户端数
4、使用accept来等待客户端的链接并返回新的socket描述符
5、使用read读取socket描述符数据
6、处理接收数据:这里将收到的字符大写化
7、使用write将处理过的数据返回给客户端
8、如果收到客户端退出请求则调用close退出

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

#define DEFAULT_PORT 6666
#define DEFAULT_IP "127.0.0.1"

int  main(int argc,char *argv[])
{
    int sd,csd;
    struct sockaddr_in  addr,client_addr;
    char buff[BUFSIZ],clientIp[BUFSIZ];
    socklen_t addr_len,client_addr_len;
    int len ,i;
    sd = socket(AF_INET,SOCK_STREAM,0);
    if(sd == -1){
        perror("create socket fail! ");
    }
    bzero(&addr,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(DEFAULT_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_len = sizeof(addr);
    bind(sd,(struct sockaddr *)&addr,addr_len);
    listen(sd,256);
    client_addr_len = sizeof(client_addr);
    csd = accept(sd,(struct sockaddr *)&client_addr,&client_addr_len);
    printf("client ip = %s,client port = %d\n", inet_ntop(AF_INET,(struct sockaddr*)&client_addr.sin_addr.s_addr,
        clientIp,sizeof(clientIp)),ntohs(client_addr.sin_port));
    for (;;)
    {
        len = read(csd,buff,sizeof(buff));
        if(strncasecmp(buff,"exit",4)==0 || strncasecmp(buff,"quit",4)==0){
            break;
        }
        for(i=0;i<len;i++){
            buff[i]  = toupper(buff[i]);
        }
        write(csd,buff,len);
    }
    close(csd);
    close(sd);
    return 0;
}