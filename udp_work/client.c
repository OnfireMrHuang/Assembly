/*
udp版客户端程序:

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
    int cfd,n;
    struct sockaddr_in  serv_addr;
    char buff[BUFSIZ];

    cfd = socket(AF_INET,SOCK_DGRAM,0);
    if(cfd == -1){
        perror("create socket fail! ");
    }
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(DEFAULT_PORT);
    inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr);
    
    while(fgets(buff,BUFSIZ,stdin) != NULL)
    {
        n = sendto(cfd,buff,strlen(buff),0,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
        if(n == -1){
            perror("sendto error!");
        }
        n = recvfrom(cfd,buff,BUFSIZ,0,NULL,0);
        if(n == -1){
            perror("recvfrom error!");
        }
        write(STDOUT_FILENO,buff,n);
    }
    close(cfd);
    return 0;
}