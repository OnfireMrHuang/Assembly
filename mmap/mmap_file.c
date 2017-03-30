/*
共享内存测试程序
通过向映射内存写数据然后映射到文件
结果：从生成文件中查看是否有写入内容
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/mman.h>
#include<fcntl.h>

int main()
{
	int len,ret;
	char *p = NULL;
	int fd = open("mytest.txt",O_CREAT|O_RDWR,0644);
	if(fd <0 ){
		perror("open error!\n");
		exit(1);
	}
	len = ftruncate(fd,4);
	if(len == -1){
		perror("ftruncate fail!\n");
		exit(1);
	}
	p = mmap(NULL,4,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(p == MAP_FAILED){
		perror("mmap fail!\n");
		exit(1);
	}
	strcpy(p,"abc\n");
	ret = munmap(p,4);	
	if(ret == -1){
		perror("munmap error!\n");
		exit(1);
	}
	close(fd);
	return 0;
}





