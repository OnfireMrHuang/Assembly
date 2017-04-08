/*
本地简易聊天室服务端程序：

注意：在该程序中我增加了glib库的使用，
主要是利用该库中的链表调用，这样省去了
许多的麻烦
如果安装了glib库，则在编译是添加编译路径
如： gcc server_mind.c -o server -I /home/sns/SNS_ENV/PC/include/glib-2.0  -L /home/sns/SNS_ENV/PC/lib -lglib-2.0
author:hww
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <glib.h>
#include "qq_ipc.h"

#define FIFO "fifo"

GList *loginList = NULL	;//等陆列表
int fd_com;//公用FIFO

void msg_handler(struct c_msg *buff);
void userLoginHandler(struct  usr *loginInfo);
gboolean isInLoginList(int number);

int main(int argc,char *argv[])
{
	struct c_msg recieveMsg;
	//1、创建和阻塞打开公共fifo
	if(mkfifo(FIFO,0666)<0 && errno != EEXIST){
		perror("create FIFO fail!\n");
		exit(1);
	}
	if((fd_com = open(FIFO,O_RDONLY)) < 0 ){
		perror("open FIFO fail!\n");
		exit(1);
	}
	//2、读到内容字符串解析
	while(1){
		int len;
		len = read(fd_com,&recieveMsg,sizeof(struct c_msg));
		if(len > 0 )
		{
			printf("fifoReader recieve a string :%s\n", recieveMsg.src.name);
			msg_handler(&recieveMsg);
		}
	}
	close(fd_com);
	unlink(FIFO);
	return 0;
}

void msg_handler(struct c_msg *buff)
{
	int i=0;
	switch(buff->num)
	{
		case 0:
		printf("注册或登陆.........\n");
		userLoginHandler(&buff->src);
		break;
		case 1:
		printf("用户聊天.........\n");
		userChatHandler(buff);
		break;
		case 2:
		printf("用户不在线.........\n");
		break;
		case 3:
		printf("用户退出.........\n");
		userExitHandler(&buff->src);
		break;
	}
	printf("g_list_length(loginList) = %d\n", g_list_length(loginList));
	for(i=0;i<g_list_length(loginList);i++)
		{
			struct  usr *data = g_list_nth_data(loginList,i);
			printf("in list number = %d,name = %s\n", data->number,data->name);
		}
}

void userLoginHandler(struct  usr *loginInfo)
{
	struct  usr *info = g_slice_copy(sizeof(struct usr),loginInfo);
	//1、用户注册
	//增加在线列表节点
	// char fifoPri[20] = {0};
	// snprintf(fifoPri,20,"%d",info->number);
	// printf("私有fifo = %s\n",fifoPri );
	loginList = g_list_append(loginList,info);
}

void userChatHandler(struct c_msg *buff)
{
	int client_fd;
	char sendData[1024];
	char client_fifo[20] = {0};
	printf("客户端 src number = %d,dest number = %d,data = %s\n",buff->src.number,buff->dest.number,buff->msg );
	if(isInLoginList(buff->dest.number)){
		snprintf(sendData,1024,"%d : %s",buff->src.number,buff->msg);
		snprintf(client_fifo,20,"%d",buff->dest.number);	
	}
	else
	{
		snprintf(sendData,1024,"%d : 不在线",buff->src.number);
		snprintf(client_fifo,20,"%d",buff->src.number);	
	}
	//打开该客户端的FIFO
	if((client_fd = open(client_fifo,O_WRONLY | O_NONBLOCK)) < 0 ){
		perror("open client FIFO fail!\n");
		exit(1);
	}
	write(client_fd,sendData,sizeof(sendData));
}

void userExitHandler(struct  usr *info)
{
	int i=0;
	for(i=0;i<g_list_length(loginList);i++)
	{
		struct usr *data = g_list_nth_data(loginList,i);
		if(info->number == data->number)
		{
			loginList = g_list_remove(loginList,data);
			data = NULL;
			break;
		}
	}
}

gboolean isInLoginList(int number)
{
	int i=0;
	for(i=0;i<g_list_length(loginList);i++)
	{
		struct usr *data = g_list_nth_data(loginList,i);
		if(data->number == number)
		{
			return TRUE;
		}
	}
	return FALSE;
}