/*
守护进程例子：
守护进程创建步骤：
1、创建子进程
2、子进程创建会话，脱离终端和父进程
3、改变进程的工作目录
4、指定文件掩码
5、将0/1/2文件描述符重定向到/dev/null（为彻底脱离终端）
6、主逻辑
7、退出
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void mydaemond(void)
{
    pid_t pid;
    pid = fork();
    if(pid == -1){
        perror("fork error!\n");
        exit(1);
    }
    else if(pid > 0 ){
        exit(1);
    }

    setsid();
    int ret = chdir("/home/sns");
    if(ret == -1){
        perror("chdir error!\n");
        exit(1);
    }
    umask(0022);
    close(STDIN_FILENO);
    open("/dev/null",O_RDWR);
    dup2(0,STDOUT_FILENO);
    dup2(0,STDERR_FILENO);
}
int main(int argc,char *argv[])
{
    mydaemond();
    while(1){

    }
    return 0;
}