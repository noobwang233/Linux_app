/*
4. 打开一个已经存在的文件（例如test_file），通过lseek函数计算该文件的大小，并打印出来
*/


#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 

#define ASSERT(x) if((x) < 0){return -1;}

int main(int argc, char *argv[])
{
    int fd;
    off_t oft;
    unsigned char buff[1024];

    fd = open(argv[1], O_RDWR);
    ASSERT(fd)

    oft = lseek(fd, 0, SEEK_END);
    printf("file size: %ld Bytes\n",oft);
    return 0;
}