/*
3. 新建一个文件（例如new_file），新建文件的权限设置为：
   文件所属者拥有读、写、执行权限；
   同组用户与其他用户只有读权限。
使用只写方式打开文件，将文件前1Kbyte字节数据填充为0x00，将下1Kbyte字节数据填充为0xFF，操作完成之后显式关闭文件，退出程序。

*/


#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 

#define ASSERT(x) if((x) < 0){return x;}

int main(int argc, char *argv[])
{
    int retvalue;
    int fd;

   fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, S_IRWXU | S_IRGRP | S_IROTH);
   ASSERT(fd)

   ...

}