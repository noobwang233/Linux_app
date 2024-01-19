#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <errno.h>
#include <string.h>

#define ASSERT(x) do{if((x) < 0){return x;}}while (0)

int main(int argc, char *argv[])
{
    int retvalue;
    int fd;

    //errno 变量
    printf("errno:\nerrno = %d\n",errno);
    fd = open(argv[1], O_RDONLY);
    printf("errno = %d\n",errno);

    //strerror函数
    printf("strerror:\n");
    fd = open(argv[1], O_RDONLY);
    printf("%s\n", strerror(errno));

    //perror函数
    printf("perror:\n");
    fd = open(argv[1], O_RDONLY);
    perror("This is ouput of perror:\n");

    return 0;
}