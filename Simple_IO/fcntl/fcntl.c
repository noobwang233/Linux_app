#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 

#define ASSERT(x) do{if((x) < 0){return x;}}while (0)

int main(int argc, char *argv[])
{
    int retvalue;
    int fd;
    unsigned char buf = 0xFF;

    //复制文件描述符
    fd = open(argv[1], O_RDONLY | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH);
    ASSERT(fd);

    int fd2 = fcntl(fd, F_DUPFD);
    ASSERT(fd2);
    printf("fd = %d, fd2 = %d\n",fd, fd2);

    //获取/设置文件状态标志
    int flag = fcntl(fd2, F_GETFL);
    ASSERT(flag);

    //文件权限标志（O_RDONLY、O_WRONLY、O_RDWR）以及文件创建标志（O_CREAT、O_EXCL、O_NOCTTY、O_TRUNC）不能被设置、会被忽略
    //只有O_APPEND、O_ASYNC、O_DIRECT、O_NOATIME以及O_NONBLOCK这些标志可以被修改
    retvalue = fcntl(fd2, F_SETFL, flag | O_WRONLY);
    ASSERT(retvalue);

    //测试权限
    retvalue = write(fd, &buf, 1);
    if(retvalue < 0)
    {
        printf("fd can not be writen \n");
    }
    else
    {
        printf("fd can be writen \n");
    }

    retvalue = write(fd2, &buf, 1);
    if(retvalue < 0)
    {
        printf("fd can not be writen \n");
    }
    else
    {
        printf("fd can be writen \n");
    }
    return 0;
}