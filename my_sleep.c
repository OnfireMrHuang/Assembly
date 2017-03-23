/*
*使用系统调用函数实现的一个类似于sleep功能
*author:hww
*
*/

#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<errno.h>

void catch_sigalrm(int signo)
{
    ;
}
unsigned int mysleep(unsigned int seconds)
{
    int ret =0;
    struct sigaction newact,oldact;
    newact.sa_handler = catch_sigalrm;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    ret = sigaction(SIGALRM,&newact,&oldact);
    if(ret == -1){
        perror("sigaction error!\n");
        exit(1);
    }
    alarm(seconds);
    ret = pause(); //主动挂起     等信号
    if(ret == -1 && errno == EINTR){
        printf("pause sucess!\n");
    }
    alarm(0);
    sigaction(SIGALRM,&oldact,NULL);//恢复SIGALRM信号旧有的处理方式
    return 0;
}
int main(int argc,char *argv[])
{
    while(1){
        mysleep(3);
        printf("--------------------------------------------\n");
    }
    return 0;
}
