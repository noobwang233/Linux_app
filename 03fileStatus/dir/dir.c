#include <sys/types.h> 
#include <dirent.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <errno.h>

#define ASSERT(x)           do{if((x) < 0){ perror(""); exit(errno);}}while (0)

int main(int argc, char *argv[])
{
    int retval;

    DIR *dirHandle = opendir(argv[1]);
    if(dirHandle == NULL)
    {
        perror("");
        exit(errno);
    }

    // while(dirHandle != NULL)
    // {
    //     struct dirent *dir = readdir(dirHandle);
    //     printf("inode: %ld, name: %s\n", dir->d_ino, dir->d_name);
    // }
    struct dirent *dir;
    while((dir = readdir(dirHandle)) != NULL)
    {
        printf("inode: %ld, name: %s\n", dir->d_ino, dir->d_name);
    }

    if(errno != 0)
    {
        perror("error\n");
        retval = closedir(dirHandle);
        ASSERT(retval);
        exit(errno);
    }

    printf("end of dir\n");
    exit(0);
}