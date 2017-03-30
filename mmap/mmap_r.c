/*
进程间通过共享内存通信
程序说明：程序为读端，每隔两秒从共享内存
中读取并打印一段数据
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/mman.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

struct STU{
	int id;
	char name[20];
	char sex;
};

void sys_err(char *str)
{
	perror(str);
	exit(-1);
}
int main(int argc,char *argv[])
{
	int fd;
	struct STU student;
	struct STU *mm;
	if(argc < 2){
		printf("./a.out file_shared\n");
		exit(-1);
	}
	fd = open(argv[1],O_RDONLY);
	if(fd == -1)
		sys_err("open error");
	mm = mmap(NULL,sizeof(student),PROT_READ,MAP_SHARED,fd,0);
	if(mm == MAP_FAILED){
		sys_err("mmap error");
	}
	close(fd);
	while(1){
		printf("id =%d\tname=%s\t%c\n",mm->id,mm->name,mm->sex);
		sleep(2);
	}
	munmap(mm,sizeof(student));
}






