/*
父子进程间通信：管道
author:hww
*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/wait.h>
#include<errno.h>

int main(int argc,char *argv[])
{
    pid_t pid;
    int pipefd[2];
    if(pipe(pipefd) == -1)
    {
        perror("pipe faile!\n");
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {   
        close(pipefd[1]); //关闭写端
        dup2(pipefd[0],STDIN_FILENO);//让WC从管道中读取数据
        execlp("wc","wc","-l",NULL); //WC命令默认从标准读入取数据
    } else {         
        close(pipefd[0]);  //关闭读端
        dup2(pipefd[1],STDOUT_FILENO);//将ls的结果写入管道中
        execlp("ls","ls",NULL);//ls输出结果默认对应屏幕
    }

    return 0;
}
