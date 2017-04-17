/*
udp版服务端程序:

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

int  main(int argc,char *argv[])
{
    int lfd;
    struct sockaddr_in  serv_addr,client_addr;
    char buff[BUFSIZ],clientIp[BUFSIZ];
    socklen_t client_addr_len;
    int n,i;

    lfd = socket(AF_INET,SOCK_DGRAM,0);
    if(lfd == -1){
        perror("create socket fail! ");
    }
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(DEFAULT_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(lfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    printf("Accetping connections ...\n");
    while(1)
    {
        client_addr_len = sizeof(client_addr);
        n = recvfrom(lfd,buff,BUFSIZ,0,(struct sockaddr *)&client_addr,&client_addr_len);
        if(n == -1){
            printf("recvfrom error!\n");
        }
        printf("client ip = %s,client port = %d\n", inet_ntop(AF_INET,(struct sockaddr*)&client_addr.sin_addr.s_addr,
        clientIp,sizeof(clientIp)),ntohs(client_addr.sin_port));
        for(i=0;i<n;i++)
        {
            buff[i] = toupper(buff[i]);
        }
        n = sendto(lfd,buff,n,0,(struct sockaddr*)&client_addr,sizeof(client_addr));
        if(n==-1){
            perror("sendto error!\n");
        }
    }
    close(lfd);
    return 0;
}