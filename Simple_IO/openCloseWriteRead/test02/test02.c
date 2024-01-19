/*
2. 通过open函数判断文件是否存在（例如test_file），并将判断结果显示出来

*/

#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 

#define ASSERT(x) if((x) < 0){return x;}

int main(int argc, char *argv[])
{
    int retvalue;
    int fd;

    fd = open(argv[1], O_RDONLY);
    if(fd < 0)
    {
        printf("file doesn't exsit !\n");
    }
    else
    {
        printf("file exsit !\n");
    }


}