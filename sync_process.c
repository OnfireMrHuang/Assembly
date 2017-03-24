/*
*程序名：sync_process.c
*程序目地：实现父子进程互发消息计数。
*涉及知识点：全局变量的异步IO
*author : hww
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

int n=0;
pid_t pid;
void sys_err(char *str)
{
    perror(str);
    exit(1);
}
void do_sig_child(int num)
{
    printf("I am child   %d\t%d\n",getpid(),n);
    n += 2;
    kill(getppid(),SIGUSR2);
    // sleep(1);
}
void do_sig_parent(int num)
{
    printf("I am parent  %d\t%d\n",getpid(),n);
    n+=2;
    kill(pid,SIGUSR1);
    // sleep(1);
}
int main(int argc,char *argv[])
{
    struct  sigaction act;
    pid = fork();
    if(pid < 0 )
    {
        // sys_err("fork error");
    }
    else if(pid > 0 )
    {
        n=1;
        sleep(1);
        act.sa_handler = do_sig_parent;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
        sigaction(SIGUSR2,&act,NULL);
        do_sig_parent(0);
        while(1)
        {
        }
    }
    else if(pid == 0)
    {
        n=2;
        act.sa_handler = do_sig_child;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
        sigaction(SIGUSR1,&act,NULL);
        while(1)
        {
        }
    }
    return 0;
}