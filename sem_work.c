/*
利用信号量来设计这样一个程序：
实现两个线程，一个从标准输入读数据，一个打印
hello world，如果没有输入则每隔5秒打印，如果有输入
则马上打印
程序思路：设置一个信号量，默认计数为0阻塞。
使用timedwait最多阻塞5秒，如果有输入，则调用
sem_post唤醒信号量.
author:hww
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<time.h>
#include<semaphore.h>
#define MAX_SIZE 256

sem_t intput_sig;

void *input_cb(void *arg)
{
    char buff[MAX_SIZE]={0};
    char *str = NULL;
    while(1)
    {
        str = fgets(buff,MAX_SIZE,stdin);
        if(str != NULL){
            sem_post(&intput_sig);
        }
    }
    return NULL;
}

void *output_cb(void *arg)
{
    struct timespec date;
    time_t timer;
    while(1)
    {
        timer= time(NULL);
        date.tv_sec = timer+5;
        sem_timedwait(&intput_sig,&date);
        printf("hello world!\n");
    }
    return NULL;
}
int main(int argc,char *argv[])
{
    pthread_t pid,cid;

    sem_init(&intput_sig,0,0);

    pthread_create(&pid,NULL,input_cb,NULL);
    pthread_create(&cid,NULL,output_cb,NULL);

    pthread_join(pid,NULL);
    pthread_join(cid,NULL);
    
    sem_destroy(&intput_sig);

    return 0;
}