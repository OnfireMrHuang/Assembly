/*
父子进程间管道通信例子01
author:hww
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main()
{
	int result;
	int fd[2];
	pid_t pid;
	result = pipe(fd);
	if(result == -1){
		perror("pipe open fail!\n");
		return -1;
	}
	pid =fork();
	if(pid == -1){
		printf("fork file!\n");
		return -1;
	}else if(pid == 0){
		close(fd[1]);
		char buf[1024];
		result = read(fd[0],buf,sizeof(buf));
		if(result ==0 ){
			printf("----------\n");
		}
		write(STDOUT_FILENO,buf,result);
	}else{
		sleep(1);
		close(fd[0]);
		write(fd[1],"hello pipe\n",strlen("hello pipe\n"));
	}
	return 0;
}
