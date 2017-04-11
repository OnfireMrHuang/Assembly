/*
简易并发服务端程序:多线程版
author:hww
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

#include "wrap.h"

#define SERV_PORT 8888
struct  s_info
{
    struct sockaddr_in clie_addr;
    int connfd;
};
void *thread_handler(void *arg);
int main(int argc,char *argv[])
{
    int lfd,cfd;
    int i;
    struct sockaddr_in serv_addr,clie_addr;
    socklen_t clie_addr_len;
    pthread_t tid;
    struct s_info ts[256];
    char clientIp[BUFSIZ];
    lfd = Socket(AF_INET,SOCK_STREAM,0);
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    Bind(lfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    Listen(lfd,128);
    while(1){
        clie_addr_len = sizeof(clie_addr);
        cfd = Accept(lfd,(struct sockaddr*)&clie_addr,&clie_addr_len);
        //打印连接的客户端的信息
        printf("client ip = %s,client port = %d\n", inet_ntop(AF_INET,(struct sockaddr*)&clie_addr.sin_addr.s_addr,
        clientIp,sizeof(clientIp)),ntohs(clie_addr.sin_port));
        // 创建线程
        ts[i].clie_addr = clie_addr;
        ts[i].connfd = cfd;
        pthread_create(&tid,NULL,thread_handler,(void*)&ts[i]);
        pthread_detach(tid);//将线程分离
        i++;
    }
    return 0;
}
void *thread_handler(void *arg)
{
    int i,n;
    char buff[BUFSIZ];
    struct sockaddr_in  clie_addr = ((struct s_info *)arg)->clie_addr;
    int fd = ((struct s_info *)arg)->connfd;
    for (;;)
    {
       n = Read(fd,buff,sizeof(buff));
        if(n <0){
            perror("read error!\n");
            break;
        }else{
            if(strncasecmp(buff,"exit",4)==0 || strncasecmp(buff,"quit",4)==0){
                break;
            }
            for(i=0;i<n;i++){
                buff[i]  = toupper(buff[i]);
            }
            Write(fd,buff,n);
        }
    }
    Close(fd);
    pthread_exit(0);
} 