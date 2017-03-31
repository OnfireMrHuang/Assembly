/*
程序目的：通过命令行传参指定源文件和输出文件
以及线程数量
在多线程拷贝的同时计算消耗时间
测试是否拷贝线程越多消耗时间越少
author:hww
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/errno.h>
#include<fcntl.h>
#include<string.h>
#include<sys/time.h>

typedef struct _PThrd_Node
{
    char *srcMap;
    char *destMap;
    int pthrd_count;
    int copy_size;
    int num;
}PThrd_Node;

void *pthrd_func(void *arg)
{
    PThrd_Node *node;
    node = (PThrd_Node*) arg;
    int size = node->copy_size/node->pthrd_count;
    if(node->num == node->pthrd_count-1){
        memcpy(node->destMap+(node->num * size),node->srcMap+(node->num * size),size+node->copy_size%node->pthrd_count);
        usleep((size+node->copy_size%node->pthrd_count)*1000);
    } else{
        memcpy(node->destMap+(node->num * size),node->srcMap+(node->num * size),size);
        usleep(size*1000);
    }
    pthread_exit((void*)1);
}

int main(int argc,char *argv[])
{
    char *srcMap;
    char *destMap;
    struct stat buf;
    int ret;
    int i;
    //校验传入参数，判断argc是否为4
    if(argc<4){
        perror("set parmer not enought!\n");
        exit(1);
    }
    const char *srcFile = argv[1];
    const char *destFile = argv[2];
    const int pthrd_count = atoi(argv[3]);
    printf("srcFile = %s,destFile = %s,pthrd_count = %d\n",srcFile,destFile,pthrd_count );
    //为源文件和目标文件创建映射区
    int srcFd = open(srcFile,O_RDWR|O_CREAT,0644);
    if(srcFd < 0 ){
        perror("srcFile open error!\n");
        exit(1);
    }
    ret  = fstat(srcFd,&buf);
    if(ret !=0 ){
        fprintf(stderr, "获取文件大小失败!,错误码：%d\n",errno);
        exit(1);
    }
    ftruncate(srcFd,buf.st_size);
    srcMap = (char*)mmap(NULL,buf.st_size,PROT_WRITE|PROT_READ,MAP_SHARED,srcFd,0);
    if(srcMap == MAP_FAILED){
        perror("srcFile map fail!\n");
        exit(1);
    }
    close(srcFd);//关闭文件描述符
    //*******测试文件映射区建立*********
    // FILE *fd = fopen("test.txt","wb");
    // fwrite(srcMap,buf.st_size,1,fd);
    // fclose(fd);
    //********************************
    int destFd = open(destFile,O_RDWR|O_CREAT,0644);
    if(destFd < 0 ){
        perror("destFile open error!\n");
        exit(1);
    }
    ftruncate(destFd,buf.st_size);
    destMap = (char*)mmap(NULL,buf.st_size,PROT_WRITE|PROT_READ,MAP_SHARED,destFd,0);
    if(destMap == MAP_FAILED){
        perror("destFile map fail!\n");
        exit(1);
    }
    printf("In main pthread :srcMap = %p,destMap = %p,buf.st_size = %d\n", srcMap,destMap,buf.st_size);
    pthread_t *tid = (pthread_t*)malloc((pthrd_count) * sizeof(pthread_t));
    PThrd_Node *nodes = (PThrd_Node*)malloc((pthrd_count) * sizeof(PThrd_Node));
    //创建参数指定个数的线程，设置线程函数进行拷贝
    for(i=0;i<pthrd_count;i++)
    {
        nodes[i].srcMap = srcMap;
        nodes[i].destMap = destMap;
        nodes[i].num = i;
        nodes[i].pthrd_count = pthrd_count;
        nodes[i].copy_size = buf.st_size;
        ret = pthread_create(&tid[i],NULL,pthrd_func,(void*)(&nodes[i]));
        if(ret != 0){
            fprintf(stderr, "pthread_create fail :%s\n", strerror(ret));
            exit(1);
        }
    }
    struct timeval tv1;
    struct timeval tv2;
    gettimeofday (&tv1, NULL);
    printf("tv_sec1=  %d,tv_usec1=  %d\n", tv1.tv_sec,tv1.tv_usec);
    //主线程主要画进度条和回收其他线程
    for(i=0;i<pthrd_count;i++)
    {
        ret = pthread_join(tid[i],NULL);
        if(ret != 0){
            fprintf(stderr, "pthread_join fail ! :%s\n", strerror(ret));
            exit(1);
        }
    }
    gettimeofday (&tv2, NULL);
    printf("tv_sec2=  %d,tv_usec2=  %d\n", tv2.tv_sec,tv2.tv_usec);
    printf("consume=  %d : %d\n", tv2.tv_sec - tv1.tv_sec,tv2.tv_usec - tv1.tv_usec);
    free(tid);
    free(nodes);
    munmap(srcMap,buf.st_size);
    munmap(destMap,buf.st_size);
    close(destFd);
    return 0;
}