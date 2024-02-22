/*
1. 打开一个已经存在的文件（例如src_file），使用只读方式；然后打开一个新建文件（例如dest_file），使用只写方式，新建文件的权限设置如下：
   文件所属者拥有读、写、执行权限；
   同组用户与其他用户只有读权限。
从src_file文件偏移头部500个字节位置开始读取1Kbyte字节数据，然后将读取出来的数据写入到dest_file文件中，
从文件开头处开始写入，1Kbyte字节大小，操作完成之后使用close显式关闭所有文件，然后退出程序。
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <errno.h>

#define ASSERT(x) do{if((x) < 0){ perror(""); exit(errno);}}while (0)


int main(int argc, char *argv[])
{
    int retvalue;
    int fd1, fd2;
    off_t oft;
    unsigned char buff[1024];

    fd1 = open(argv[1], O_WRONLY);
    ASSERT(fd1);
    fd2 = open(argv[2], O_CREAT | O_RDWR, S_IRWXU | S_IROTH | S_IRGRP);
    ASSERT(fd2);

    // oft = lseek(fd1, 500, SEEK_SET);
    // if(oft != 500)
    // {
    //     printf("lseek oft = %ld\n", oft);
    //     return -1;
    // }

    // retvalue = read(fd1, buff, 1024);
    // if(retvalue != 1024)
    // {
    //     printf("read retvalue = %d\n", retvalue);
    //     return -1;
    // }
    retvalue = pread(fd1, buff, 1024, 500);//原子操作

    retvalue = write(fd2, buff, 1024);
    if(retvalue != 1024)
    {
        perror("write error");
        return -1;
    }

    retvalue = close(fd1);
    ASSERT(retvalue);
    retvalue = close(fd2);
    ASSERT(retvalue);
    return 0;
}