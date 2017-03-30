/*
模拟sleep函数的一个例子
通过setitimer函数设置计时，当计满时间后会
给进程发送ARLRM信号，然后再重新计时
这里在受到信号后执行默认动作，程序退出
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/time.h>

static int my_alarm(unsigned int sec)
{
	struct itimerval it,oldit;
	int ret;
	it.it_value.tv_sec = 5;
	it.it_value.tv_usec = 0;
	it.it_interval.tv_sec =0;
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
	int i=0;
	my_alarm(1);
	for(i=0;;i++)
	{
		printf("%d\n",i);
	}
	return 0;
}
