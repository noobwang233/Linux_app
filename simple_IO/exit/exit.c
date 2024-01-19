#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <errno.h>

#define ASSERT(x) do{if((x) < 0){ perror(""); exit(errno);}}while (0)


static void fun(const char* file)
{
    int fd;

    printf("function:\n");
    fd = open(file, O_RDONLY);
    ASSERT(fd);
} 

//return 只能在main函数中退出程序，而exit可以在任何函数里退出程序
int main(int argc, char *argv[])
{
    int retval;
    int fd;

    fun(argv[1]);

    printf("return:\n");
    return 0;
}