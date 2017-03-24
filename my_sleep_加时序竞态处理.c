/*
*使用系统调用函数实现的一个类似于sleep功能
*在上个程序的该块代码中：
alarm(seconds);
//-----------------------------------------
    ret = pause(); //主动挂起     等信号
//--------处可能会失去CPU，等到程序再得到CPU的时候，也许
信号已经被发送，这是程序会优先处理信号处理函数，我们的程序
中处理是什么也没做，而在这结束后立刻执行puase函数，可是信号
却已经被处理掉啦，pause就会一直阻塞下去
这种情况称作时序竞态
在类Unix系统中提供了一个sigsuspend函数，同样类似于pause,
按我的理解就等同与将pause与信号处理函数绑定。
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
    sigset_t newmask,oldmask,suspmask;
    unsigned int unslept;

//1、为SIGALRM信号设置捕捉函数
    newact.sa_handler = catch_sigalrm;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    ret = sigaction(SIGALRM,&newact,&oldact);
    if(ret == -1){
        perror("sigaction error!\n");
        exit(1);
    }
//2、设置阻塞信号集，阻塞SIGALRM信号
    sigemptyset(&newmask);
    sigaddset(&newmask,SIGALRM);
    sigprocmask(SIG_BLOCK,&newmask,&oldmask);
//3、定时seconds秒，时间记满后产生SIGALRM信号
    alarm(seconds);
//4、构造一个调用sigsuspend临时有效的阻塞信号集，
//在临时阻塞信号集里面解除SIGALRM的阻塞
    suspmask = oldmask;
    sigdelset(&suspmask,SIGALRM);
//5、sigsuspend调用
    sigsuspend(&suspmask);
    // ret = pause(); //主动挂起     等信号
    // if(ret == -1 && errno == EINTR){
    //     printf("pause sucess!\n");
    // }
    unslept = alarm(0);
//6、恢复SIGALRM心信号的原有处理动作
    sigaction(SIGALRM,&oldact,NULL);
//7、解除对SIGALRM信号的阻塞
    sigprocmask(SIG_SETMASK,&oldmask,NULL);
    return unslept;
}
int main(int argc,char *argv[])
{
    while(1){
        mysleep(3);
        printf("--------------------------------------------\n");
    }
    return 0;
}