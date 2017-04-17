/*
udp版服务端程序_广播例子:

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
#define BROADCAST_IP "192.168.16.255"

int  main(int argc,char *argv[])
{
    int socketfd;
    struct sockaddr_in  serv_addr,client_addr;
    char buff[BUFSIZ];

    socketfd = socket(AF_INET,SOCK_DGRAM,0);
    if(socketfd == -1){
        perror("create socket fail! ");
    }

    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(socketfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));

    int flag =1;
    setsockopt(socketfd,SOL_SOCKET,SO_BROADCAST,&flag,sizeof(flag));

    /*构造client地址 IP和端口 */
    bzero(&client_addr,sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(CLIENT_PORT);
    inet_pton(AF_INET,BROADCAST_IP,&client_addr.sin_addr);
    
    int i=0;
    while(1)
    {
       sprintf(buff,"Drink %d glasses of water\n",i++);
       sendto(socketfd,buff,strlen(buff),0,(struct sockaddr*)&client_addr,sizeof(client_addr));
       sleep(1);
    }
    close(socketfd);
    return 0;
}