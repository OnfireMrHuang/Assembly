/*
sigpending函数为返回内核未决信号集
程序大意为：先设置阻塞信号集，屏蔽SIGQUIT等信号
然后一直循环打印未决信号集，信号被处理表示0,
(一些信号可以通过按键来产生)
未决表示1
author:hww
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

void printped(sigset_t *ped)
{
	int i=0;
	for(i=0;i<32;i++)
	{
		if(sigismember(ped,i) == 1){
			putchar('1');
		}else{
			putchar('0');
		}
	}
	printf("\n");
}

int main(int argc,char *argv[])
{
	sigset_t myset,oldset,ped;
	sigemptyset(&myset);
	sigaddset(&myset,SIGQUIT);
	sigaddset(&myset,SIGINT);
	sigaddset(&myset,SIGSTOP);
	sigaddset(&myset,SIGSEGV);
	sigaddset(&myset,SIGKILL);

	sigprocmask(SIG_BLOCK,&myset,&oldset);
	while(1){
		sigpending(&ped);
		printped(&ped);
		sleep(1);
	}	
	return 0;
}
