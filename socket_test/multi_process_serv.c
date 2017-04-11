/*
简易并发服务端程序:多进程版
author:hww
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>

#include "wrap.h"

#define SERV_PORT 8888

void wait_child(int signo){
    while(waitpid(0,NULL,WNOHANG) > 0);
    return;
}

int main(int argc,char *argv[])
{
    int lfd,cfd;
    struct sockaddr_in serv_addr,clie_addr;
    socklen_t clie_addr_len;
    pid_t pid;
    int n,i;
    char clientIp[BUFSIZ];
    char buff[BUFSIZ];
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
        printf("client ip = %s,client port = %d\n", inet_ntop(AF_INET,(struct sockaddr*)&clie_addr.sin_addr.s_addr,
        clientIp,sizeof(clientIp)),ntohs(clie_addr.sin_port));

        pid = fork();
        if(pid == -1){
            perror("fork child process error!\n");
            exit(1);
        }else if(pid == 0){
            close(lfd);
            break;
        }else{
            close(cfd);
            signal(SIGCHLD,wait_child);
        }
    }
    if(pid == 0){
        for(;;){
            n = Read(cfd,buff,sizeof(buff));
            if(n <0){
                perror("read error!\n");
                exit(1);
            }else if(n == 0){
                close(cfd);
                return 0;
            }else{
                if(strncasecmp(buff,"exit",4)==0 || strncasecmp(buff,"quit",4)==0){
                    return 1;
                }
                for(i=0;i<n;i++){
                    buff[i]  = toupper(buff[i]);
                }
                Write(cfd,buff,n);
            }
        }
    }
    return 0;
}