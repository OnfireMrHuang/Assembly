/*
模拟sleep函数的一个例子
通过setitimer函数设置计时，当计满时间后会
给进程发送ARLRM信号，然后再重新计时
这里在受到信号后执行捕捉，程序打印后
再到下一个5秒再打印，一直循环
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/time.h>
#include<signal.h>

void signal_handle(int sig)
{
	printf("hello world!\n");
}

int my_alarm(unsigned int sec)
{
	struct itimerval it,oldit;
	int ret;
	it.it_value.tv_sec = 5;
	it.it_value.tv_usec = 0;
	it.it_interval.tv_sec =3;
	it.it_interval.tv_usec = 0;
	ret = setitimer(ITIMER_REAL,&it,&oldit);
	if(ret == -1){
		perror("settimer");
		exit(1);
	} 
	return oldit.it_value.tv_sec;
}

int main(int argc,char *argv[])
{
	my_alarm(1);
	signal(SIGALRM,signal_handle);
	while(1);
	return 0;
}
