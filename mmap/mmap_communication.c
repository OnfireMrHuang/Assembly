/*
父子进程共享内存通信
程序目地：检验是否共享全局变量
子进程往共享内存写数据，父进程从内存中读数据
unlink在通过文件建立好映射区后，移除文件
在父进程打印输出后回收子进程并卸载指定映射区
author:hww
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/wait.h>

int var = 100;

int main()
{
	int *p;
	pid_t pid;
	int fd;
	fd =open("temp",O_RDWR|O_CREAT|O_TRUNC,0644);
	if(fd < 0 )
	{
		perror("open error");
		exit(1);
	}
	unlink("temp");//delete temp file director item
	ftruncate(fd,4);

	p = (int*)mmap(NULL,4,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	//p = (int*)mmap(NULL,4,PROT_READ|PROT_WRITE,MAP_PRIVATE,fd,0);
	if(p == MAP_FAILED){
		perror("mmap error");
		exit(1);
	}
	close(fd);

	pid = fork();
	if(pid == 0){
		*p = 2000;
		var =1000;
		printf("child,*p = %d, var = %d\n",*p,var);
	}else{
		sleep(1);
		printf("parent,*p = %d, var = %d\n",*p,var);
		wait(NULL);
		int ret = munmap(p,4);
		if(ret == -1){
			perror("munmap error");
			exit(1);
		}
	}
	return 0;
}






