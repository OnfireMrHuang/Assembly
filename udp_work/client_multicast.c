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
#define GROUP "239.0.0.2"

int  main(int argc,char *argv[])
{
    int cfd,n;
    struct sockaddr_in  local_addr;
    char buff[BUFSIZ];
    ssize_t len;
    struct ip_mreqn group;

    cfd = socket(AF_INET,SOCK_DGRAM,0);
    if(cfd == -1){
        perror("create socket fail! ");
    }
    bzero(&local_addr,sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(CLIENT_PORT);
    inet_pton(AF_INET,"0.0.0.0",&local_addr.sin_addr);
    bind(cfd,(struct sockaddr*)&local_addr,sizeof(local_addr));

    inet_pton(AF_INET,GROUP,&group.imr_multiaddr); //设置组地址
    inet_pton(AF_INET,"0.0.0.0",&group.imr_address);    //本地任意IP
    group.imr_ifindex = if_nametoindex("eth0");

    setsockopt(cfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&group,sizeof(group));

    while(1){
        len = recvfrom(cfd,buff,sizeof(buff),0,NULL,0);
        write(STDOUT_FILENO,buff,len);
    }

    close(cfd);
    return 0;
}