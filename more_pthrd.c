/*
程序说明：多个线程创建的例子
注意事项：再编译链接该程序的时候记得带上-lpthread选项
author:hww
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *pthrd_func(void *arg)
{
    int i = (int)arg;
    printf("%d in thread : thread id = %lu,process id = %d\n",i,pthread_self(),getpid() );
    sleep(i);
    return NULL;
}

int main(int argc,char *argv[])
{
    pthread_t tid;
    int ret;
    int i;
    for(i=0;i<5;i++)
    {
        ret = pthread_create(&tid,NULL,pthrd_func,(void*)i);
        if(ret != 0 ){
            printf("pthread create error : number = %d,reason = %s",ret ,strerror(ret));
            exit(1);
        }
    }
    sleep(i);
    return 0;
}
