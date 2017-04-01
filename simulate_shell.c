/*
程序说明：简易shell制作
执行编译后得到的可执行文件，等待用户输入命令
功能与shell类似，要求实现shell的基本功能
author:hww
*/
// 实现步骤：
// 1、接受用户输入命令字符串，拆分命令及参数存储。
// 2、实现普通命令加载功能
// 3、实现输入、输出重定向功能
// 4、实现管道
// 5、支持多重管道

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#define MAX_SIZE 256
#define MAX_ARG 64
#define MAX_COMMAND 16

//自定义数据结构存储
typedef struct _cmd_t
{
    char *argv[MAX_ARG];
    char *in;
    char *out;
}cmd_t;

int parse(char *buff/*输入命令*/,cmd_t *cmds/*传出*/)
{
    char *tmp = buff,*curcmd;
    int i=0,j=0,cmdnum =0;
    if((cmds[cmdnum].argv[0] = strtok(tmp,"|"))){
        cmdnum++;
    }
    while((cmds[cmdnum].argv[0] = strtok(NULL,"|")))
    {
        cmdnum++;
    }
    //将拆分好的每条命令存入自定义的结构体中
    for (i = 0; i < cmdnum; ++i)
    {
        curcmd = cmds[i].argv[0];
        printf("command = %s\n", curcmd);
        cmds[i].in = cmds[i].out = NULL;
        while(*curcmd)
        {
            if(*curcmd == ' '){
                *(curcmd++) = '\0';
                continue;
            }
            if(*curcmd == '<'){
                *curcmd = '\0';
                while(*(++curcmd)==' ');
                cmds[i].in = curcmd;
                if(*curcmd == '\0')
                    return -1;
                continue;
            }
            if(*curcmd == '>'){
                *curcmd = '\0';
                while(*(++curcmd)==' ');
                cmds[i].out = curcmd;
                if(*curcmd == '\0')
                    return -1;
                continue;
            }
            if(*curcmd != ' ' && ((curcmd == cmds[i].argv[0]) || *(curcmd-1)=='\0'))
            {
                cmds[i].argv[j++] = curcmd++;
                continue;
            }
            curcmd++;
        }
        cmds[i].argv[j] =   NULL; 
        j=0;
    }
    return cmdnum;
}
void show(cmd_t *cmds,int number)
{
    int i,j;
    for(i=0;i < number;i++)
    {
        printf("cmd-show:%s,",cmds[i].argv[0] );
        for(j=1;cmds[i].argv[j];j++)
        {
            printf("argv:%s,", cmds[i].argv[j]);
        }
        printf("in:%s,out:%s\n", cmds[i].in,cmds[i].out);
    }
}
int main(int argc,char *argv[])
{
    char buff[MAX_SIZE]={0};
    cmd_t cmds[MAX_COMMAND];
    int cmdnum;
    pid_t pid;
    int i=0,j=0;
    char cmd_line[80];   
    while(1)
    {
        getcwd(cmd_line,sizeof(cmd_line));   
        printf("%s$ ", cmd_line);   
        fgets(buff,MAX_SIZE,stdin);
        if(buff[strlen(buff)-1] == '\n')
            buff[strlen(buff)-1] = '\0';
        if(strcasecmp(buff,"exit") == 0 || strcasecmp(buff,"quit") == 0
            || strcasecmp(buff,"EXIT") == 0 || strcasecmp(buff,"QUIT") == 0)
            break;
        cmdnum = parse(buff,cmds);
        show(cmds,cmdnum);
        for(i=0;i<cmdnum;i++)
        {
            pid = fork();
            if(pid == -1){
                perror("fork error!\n");
                exit(1);
            }else if(pid == 0){
                break;
            }
        }
        if(pid == 0){
            //子进程执行代码
            int ttyfd = open(cmds[i].out, O_RDWR);
            dup2(ttyfd, STDOUT_FILENO);
            close(ttyfd);
            char *str = cmds[i].argv[0];
            if( execvp(str,cmds[i].argv) < 0 )
            {
                perror( "execvp error " );
            }
            exit( 1 );
        }
        //TODO 回收子进程
        pid_t wpid;
        int n = i;
        do{
            wpid = waitpid(-1,NULL,WNOHANG);
            if(wpid > 0){
                n--;
            }
        }while(n>0);
    }
    return 0;
}

