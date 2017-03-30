/*
程序说明：通过线程属性来设置线程分离
如果分离成功，则回收该线程出错，
输出为无效的参数
author:hww
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>

void*	pthread_func(void *arg)
{

	pthread_exit((void*)77);
}

int main(int argc,char *argv[])
{
	pthread_t tid;
	int ret;
	pthread_attr_t attr;
	ret = pthread_attr_init(&attr);
	if(ret != 0){
		fprintf(stderr,"pthread_init error:%s\n",strerror(ret));
		exit(1);
	}
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	ret = pthread_create(&tid,&attr,pthread_func,NULL);
	if(ret != 0)
	{
		fprintf(stderr,"pthread_create error:%s\n",strerror(ret));
		exit(1);
	}
	ret = pthread_join(tid,NULL);
	if(ret != 0){	
		fprintf(stderr,"pthread_join error:%s\n",strerror(ret));
		exit(1);
	}
	pthread_exit((void*)1);
}
