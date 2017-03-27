/*
*在多进程编程里面，如果子进程终止会发送一个SIGCHLD
*信号，本篇例子便是通过该特性使用父进程捕捉该信号
*来达到回收子进程的目地
*author:hww
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<errno.h>
#include<sys/types.h>

void do_sig_handler(int num)
{
    int status;
    pid_t pid;
    while((pid = waitpid(0,&status,WNOHANG) )> 0 ){
        if(WIFEXITED(status)){
            printf("--------------child %d exit %d\n", pid,WEXITSTATUS(status));
        }else if(WIFSIGNALED(status)){
            printf("child %d cancel signal %d\n", pid,WTERMSIG(status));
        }
    }
}

int main(int argc,char *argv[])
{
    pid_t pid;
    struct sigaction act;
    int i=0;
    for (i = 0; i < 10; ++i)
    {
        pid = fork();
        if(pid == 0){
            break;
        }else if(pid == -1){
            perror("fork error!\n");
            exit(1);
        }
    }
    if(pid > 0 ){
        act.sa_handler = do_sig_handler;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
        sigaction(SIGCHLD,&act,NULL);
        while(1){
            printf("Parent ID %d\n", getpid());
            sleep(1);
        }
    }else if(pid == 0)
    {
            printf("Child ID %d\n", getpid());
            sleep(1);
    }

    return 0;
}