/*
线程篇：生产者、消费者模型
author:hww
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

struct msg
{
    int num;
    struct msg *next;
};
struct  msg  *head = NULL;
struct msg   *mp = NULL;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *producter_cb(void *arg)
{
    while(1)
    {
        mp = malloc(sizeof(struct msg));
        mp->num = rand() % 400 +1;
        pthread_mutex_lock(&mutex);
        printf("--producted--%d\n", mp->num);
        mp->next = head;
        head = mp;
        pthread_mutex_unlock(&mutex);

        pthread_cond_signal(&cond);
        sleep(1);
    }
    return NULL;
}

void *consume_cb(void *arg)
{
    while(1){
        pthread_mutex_lock(&mutex);
        if(head == NULL){
            pthread_cond_wait(&cond,&mutex);
        }
        struct msg *temp = NULL;
        temp = head;
        head = temp->next;
        temp->next  = NULL; 
        printf("--consume--%d\n", temp->num);
        free(temp);
        pthread_mutex_unlock(&mutex);
        sleep(2);
    }
    return NULL;
}
int main(int argc,char *argv[])
{
    pthread_t pid,cid;
    pthread_create(&pid,NULL,producter_cb,NULL);
    pthread_create(&cid,NULL,consume_cb,NULL);

    pthread_join(pid,NULL);
    pthread_join(cid,NULL);
    return 0;
}