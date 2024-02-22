//检查执行进程的用户是否对该文件拥有相应的权限
#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdlib.h>


int main(int argc, char *argv[])
{
    int retval;
    retval = access(argv[1], F_OK);
    if(retval < 0)
    {
        printf("%s doesn't exist\n", argv[1]);
    }

    retval = access(argv[1], R_OK);
    if(retval < 0)
    {
        printf("%s doesn't have read permission\n", argv[1]);
    }
    else
    {
        printf("%s have read permission\n", argv[1]);
    }

    retval = access(argv[1], W_OK);
    if(retval < 0)
    {
        printf("%s doesn't have write permission\n", argv[1]);
    }
    else
    {
        printf("%s have write permission\n", argv[1]);
    }

    retval = access(argv[1], X_OK);
    if(retval < 0)
    {
        printf("%s doesn't have execution permission\n", argv[1]);
    }
    else
    {
        printf("%s have execution permission\n", argv[1]);
    }
    exit(0);
}