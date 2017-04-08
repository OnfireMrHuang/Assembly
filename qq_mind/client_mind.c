/*
本地简易聊天室客户端程序：

author:hww
*/

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "qq_ipc.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define FIFO "fifo" 
int fd_priv;
int fd_com;	
void recieveMsgHandler(char *recieveMsg);
void comm_client(struct usr *myInfo);
void be_noblock(int fd);

//首先，客户端程序通过带号码于名称启动进程
int main(int argc,char *argv[])
{
	if(argc < 3){
		perror("参数中请带一个号码和名字！");
		exit(1);
	}
	char recieveMsg[1024];//用于接受私有FIFO传过来的数据
	struct c_msg sendMsg;//用于发送到公共FIFO的数据
	struct usr myMsg;//存储自己的信息

	//1、获取自己的pid、name和号码等信息,然后填充进信息结构体
	pid_t my_pid = getpid();
	myMsg.number =atoi(argv[1]);
	snprintf(myMsg.name,10,"%s",argv[2]);
	myMsg.pid = my_pid;

	//2、非阻塞属性创建私有FIFO
	if(mkfifo(argv[1],0666)<0 && errno != EEXIST){
		perror("create private FIFO fail!\n");
		exit(1);
	}
	if((fd_priv = open(argv[1],O_RDONLY|O_NONBLOCK)) < 0){
		perror("open private FIFO fail!\n");
		exit(1);
	}
	//3、打开服务器公共的FIFO
	if((fd_com = open(FIFO,O_WRONLY|O_NONBLOCK)) < 0 ){
		perror("open FIFO fail!\n");
		exit(1);
	}
	//4、把自己的信息向服务器注册
	sendMsg.num = 0;
	sendMsg.src = myMsg; 
	write(fd_com,&sendMsg,sizeof(struct  c_msg));

	//5、设置标准输入为非阻塞属性
	be_noblock(STDIN_FILENO);

	while(1)
	{
		int len =0;
		//6、非阻塞读私有FIFO
		len = read(fd_priv,recieveMsg,sizeof(recieveMsg));
		if(len > 0 )
		{
			recieveMsgHandler(recieveMsg);
		}
		//7、从标准输入读并处理
		comm_client(&myMsg);
	}	

	close(fd_com);
	unlink(FIFO);

	return 0;
}

void recieveMsgHandler(char *recieveMsg)
{
	printf("收到聊天消息！%s\n" ,recieveMsg);
}
void be_noblock(int fd) //把文件设置为非阻塞
{
    int flags;
    if((flags = fcntl(fd, F_GETFL)) < 0)
        perror("get fcntl fail!\n");
    flags |= O_NONBLOCK;
    if((fcntl(fd, F_SETFL, flags)) < 0)
        perror("set fcntl fail!");
}

void comm_client(struct usr *myInfo)
{
	int dest_number;
	struct c_msg im;
	char buff[256];
   	int len;

	if((len = read(STDIN_FILENO, buff, sizeof(buff))) < 0)
	{ //读用户输入的数据
	    if(errno != EAGAIN)
	        perror("read user input fail!\n");
	}
	else  
	{   
	     buff[len] = '\0';
	    //如果len不小于0，说明有数据，如果有数据就把数据封装到结构体中去
	    if((strncmp(buff, "quit",4)) == 0||(strncmp(buff,"exit",4) == 0)) { 
	        im.num = 3; 
	        im.src = *myInfo;
	        write(fd_com, &im, sizeof(struct c_msg)); //给服务器发送信息
	        close(fd_priv);
	        char str[256];
	        snprintf(str,sizeof(str),"%d",myInfo->number);
	        unlink(str);
	        exit(1);
	    }

	    dest_number = atoi(strtok(buff, ":"));
	    // printf("dest_str = %s,dest_number = %d\n",strtok(buff, ":"),dest_number);
	    im.num = 1; //然后把id置为1，说明是正常的数据
	    im.src = *myInfo;
	    im.dest.number = dest_number;
	    strncpy(im.msg , strtok(NULL, ":"), sizeof(im.msg));
	    printf("im->src = %d, im->dest = %d, im->data = %s\n", im.src.number,  im.dest.number, im.msg);
	    write(fd_com, &im, sizeof(struct c_msg)); //给服务器发送信息

	}
}