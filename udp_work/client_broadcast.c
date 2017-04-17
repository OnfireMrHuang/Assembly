/*
udp版客户端程序-广播例子:

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

#define SERVER_PORT 6666
#define CLIENT_PORT 7777

int  main(int argc,char *argv[])
{
    int cfd,n;
    struct sockaddr_in  local_addr;
    char buff[BUFSIZ];
    ssize_t len;

    cfd = socket(AF_INET,SOCK_DGRAM,0);
    if(cfd == -1){
        perror("create socket fail! ");
    }
    bzero(&local_addr,sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(CLIENT_PORT);
    inet_pton(AF_INET,"0.0.0.0",&local_addr.sin_addr);

    int ret = bind(cfd,(struct sockaddr *)&local_addr,sizeof(local_addr));
    if(ret == 0){
        printf("...bind_ok\n");
    }
    while(1){
        len = recvfrom(cfd,buff,sizeof(buff),0,NULL,0);
        write(STDOUT_FILENO,buff,len);
    }

    close(cfd);
    return 0;
}