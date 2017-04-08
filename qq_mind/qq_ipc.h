/*
本地简易聊天室消息定义：
author:hww
*/

#ifndef __QQ_IPC_H_
#define __QQ_IPC_H_
//0 新用户注册
//1 聊天信息
//2 服务器更新列表信息
struct usr{
	int number;//用户号码
	char name[10];//用户名称
	pid_t pid;//用户的进程ID
};

//协议栈结构体定义
struct c_msg{
	int num;//协议号
	struct usr src;//发送方用户信息
	struct usr dest;//接收方用户信息
	char msg[256];//发送数据
};

#endif

