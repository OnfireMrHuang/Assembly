/*
程序说明：一个线程创建的实例
注意事项：再编译链接该程序的时候记得带上-lpthread选项
author:hww
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *pthrd_func(void *arg)
{
    printf("in thread : thread id = %lu,process id = %d\n",pthread_self(),getpid() );
    return NULL;
}

int main(int argc,char *argv[])
{
    pthread_t tid;
    int ret;
    printf("in main : thread id = %lu,process id = %d\n",pthread_self(),getpid() );
    ret = pthread_create(&tid,NULL,pthrd_func,NULL);
    if(ret != 0 ){
        perror("pthread create error!\n");
        exit(1);
    }
    sleep(1);
    printf("in main : thread id = %lu,process id = %d\n",pthread_self(),getpid() );
    return 0;
}