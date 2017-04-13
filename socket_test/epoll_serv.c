/*
简易服务端程序:epoll版本
*
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
#include <sys/epoll.h>

#include "wrap.h"

#define SERV_PORT 8888
#define MAXLINE 1024
#define OPEN_MAX 10
#define  NOTDEF

int main(int argc,char *argv[])
{
    int i,j,n,listenfd,connfd,sockfd,nready,res;
    struct sockaddr_in cliaddr,servaddr;
    int efd;
    socklen_t clilen;
    struct epoll_event tep,ep[OPEN_MAX];
    char clientIp[BUFSIZ],buf[MAXLINE];

    listenfd = Socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    Listen(listenfd,256);
    //设置端口复用
    int opt=1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    efd = epoll_create(OPEN_MAX);
    if(efd == -1){
        perr_exit("epoll_create error!\n");
    }
    tep.events = EPOLLIN; tep.data.fd = listenfd;
    res = epoll_ctl(efd,EPOLL_CTL_ADD,listenfd,&tep);
    if(res == -1){
        perr_exit("epoll_ctl error!\n");
    }

    for(;;){
        nready = epoll_wait(efd,ep,OPEN_MAX,-1);
        if(nready == -1){
            perr_exit("epoll_wait error!\n");
        }
        for(i=0;i<nready;i++){
            if(!(ep[i].events & EPOLLIN))//如果不是读事件则继续循环
                continue;
            if(ep[i].data.fd == listenfd){
                clilen = sizeof(cliaddr);
                connfd = Accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
#ifdef  NOTDEF
    printf("client ip = %s,client port = %d\n", inet_ntop(AF_INET,(struct sockaddr*)&cliaddr.sin_addr.s_addr,
    clientIp,sizeof(clientIp)),ntohs(cliaddr.sin_port));
#endif
                tep.events = EPOLLIN;tep.data.fd = connfd;
                res = epoll_ctl(efd,EPOLL_CTL_ADD,connfd,&tep);
                if(res == -1){
                    perr_exit("epoll_ctl error!\n");
                }
            }else{
                sockfd = ep[i].data.fd;
                if ( (n = Read(sockfd, buf, MAXLINE)) == 0) {
                    printf("client %d closed connection\n",sockfd );
                        Close(sockfd);
                        res = epoll_ctl(efd,EPOLL_CTL_DEL,sockfd,NULL);
                        if(res == -1){
                            perr_exit("epoll_ctl del error!\n");
                        }
                    }else if(n < 0 ){
                        Close(sockfd);
                        res = epoll_ctl(efd,EPOLL_CTL_DEL,sockfd,NULL);
                        if(res == -1){
                            perr_exit("epoll_ctl del error!\n");
                        }
                    }else{
                        for(j=0;j<n;j++)
                            buf[j] = toupper(buf[j]);
                        Writen(sockfd, buf, n);
                    }
            }
        }
    }
    return 0;
}