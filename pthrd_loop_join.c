/*
多线程创建和回收的一个简单例子
author:hww
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

int var = 100;

void * pthrd_func(void *arg)
{
	int i;
	i=(int)arg;
	if(i==1){
		var = 333;
		printf("var = %d\n",var);
		return var;
	}else if(i==3){
		var = 777;
		printf("i'm %dth pthread pthread id = %d\n,var =%d\n",i+1,pthread_self(),var);
		pthread_exit(var);
	}else{
		printf("I'm %dth pthread pthread id =  %d\n,var = %d\n",i+1,pthread_self(),var);
		pthread_exit(var);
	}
	return NULL;
}

int main(int argc,char *arv[])
{
        pthread_t tid[5];
        int i;
        int *ret[5];
        for(i=0;i<5;i++)
        {
                pthread_create(&tid[i],NULL,pthrd_func,(void*)i);
        }
        for(i=0;i<5;i++)
        {
                pthread_join(tid[i],(void **)&ret[i]);
                printf("---------%d's ret = %d\n",i,(int)ret[i]);
        }
        printf("i'm main pthread tid = %lu\tvar = %d\n",pthread_self(),var);
        sleep(i);
	return 0;
}
