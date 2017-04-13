/*
简易服务端程序:io复用版
使用为select系统调用
相比多进程和多线程，改版服务器
节约了CPU资源，同时也没有了受线程数或进程数的限制
但是同样存在一个问题，在多个客户端写时，服务端会阻塞
并依次执行完后在接受新的连接，同时也存在其他的阻塞问题
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
#include <sys/select.h>

#include "wrap.h"

#define SERV_PORT 8888
#define MAXLINE 1024
#define  NOTDEF

int main(int argc,char *argv[])
{
    int i,j,maxi,maxfd,listenfd,connfd,sockfd;
    int nready,client[FD_SETSIZE];
    ssize_t n;
    char clientIp[BUFSIZ];
    fd_set rset,allset;
    char buf[MAXLINE];
    socklen_t clilen;
    struct sockaddr_in cliaddr,servaddr;

    listenfd = Socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    Listen(listenfd,256);

    maxfd = listenfd;           /* initialize */
    maxi = -1;                  /* index into client[] array */
    for (i = 0; i < FD_SETSIZE; i++)
        client[i] = -1;         /* -1 indicates available entry */
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    for ( ; ; ) {
        rset = allset;      /* structure assignment */
        nready = Select(maxfd+1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(listenfd, &rset)) {    /* new client connection */
            clilen = sizeof(cliaddr);
            connfd = Accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
#ifdef  NOTDEF
            printf("client ip = %s,client port = %d\n", inet_ntop(AF_INET,(struct sockaddr*)&cliaddr.sin_addr.s_addr,
        clientIp,sizeof(clientIp)),ntohs(cliaddr.sin_port));
#endif
            for (i = 0; i < FD_SETSIZE; i++)
                if (client[i] < 0) {
                    client[i] = connfd; /* save descriptor */
                    break;
                }
            if (i == FD_SETSIZE)
                perr_exit("too many clients");

            FD_SET(connfd, &allset);    /* add new descriptor to set */
            if (connfd > maxfd)
                maxfd = connfd;         /* for select */
            if (i > maxi)
                maxi = i;               /* max index in client[] array */

            if (--nready <= 0)
                continue;               /* no more readable descriptors */
        }

        for (i = 0; i <= maxi; i++) {   /* check all clients for data */
            if ( (sockfd = client[i]) < 0)
                continue;
            if (FD_ISSET(sockfd, &rset)) {
                if ( (n = Read(sockfd, buf, MAXLINE)) == 0) {
                        /*4connection closed by client */
                    Close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                } else{
                    for(j=0;j<n;j++)
                        buf[j] = toupper(buf[j]);
                    Writen(sockfd, buf, n);
                }

                if (--nready <= 0)
                    break;              /* no more readable descriptors */
            }
        }
    }
    return 0;
}
