#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define ASSERT(x)           do{if((x) < 0){ perror(""); exit(errno);}}while (0)
#define CHECKFILETYPE(st_mode)  do{                                                                               \
                                if((st_mode & S_IFSOCK) == S_IFSOCK){printf("It is socket file\n");}          \
                                if((st_mode & S_IFLNK) == S_IFLNK){printf("It is link file\n");}              \
                                if((st_mode & S_IFREG) == S_IFREG){printf("It is regular file\n");}           \
                                if((st_mode & S_IFBLK) == S_IFBLK){printf("It is block device file\n");}      \
                                if((st_mode & S_IFDIR) == S_IFDIR){printf("It is directory\n");}              \
                                if((st_mode & S_IFCHR) == S_IFCHR){printf("It is character device file\n");}  \
                                if((st_mode & S_IFIFO) == S_IFIFO){printf("It is FIFO file\n");}              \
                            }while (0)
#define CHECKFILEPERMISSION(st_mode)  do{                                                                               \
                                    if((st_mode & S_IRWXU) == S_IRWXU){printf("owner has read, write, and execute permission\n");}          \
                                    else{\
                                        if((st_mode & S_IRUSR) == S_IRUSR){printf("owner has read permission\n");}              \
                                        if((st_mode & S_IWUSR) == S_IWUSR){printf("owner has write permission\n");}           \
                                        if((st_mode & S_IXUSR) == S_IXUSR){printf("owner has execute permission\n");}};\
                                    if((st_mode & S_IRWXG) == S_IRWXG){printf("group has read, write, and execute permission\n");}              \
                                    else{\
                                        if((st_mode & S_IRGRP) == S_IRGRP){printf("group has read permission\n");}  \
                                        if((st_mode & S_IWGRP) == S_IWGRP){printf("group has write permission\n");}              \
                                        if((st_mode & S_IXGRP) == S_IXGRP){printf("group has execute permission\n");}  };\
                                    if((st_mode & S_IRWXO) == S_IRWXO){printf("others (not in group) have read, write, and execute permission\n");}              \
                                    else{ \
                                        if((st_mode & S_IROTH) == S_IROTH){printf("others has read permission\n");}  \
                                        if((st_mode & S_IWOTH) == S_IWOTH){printf("others has write permission\n");}              \
                                        if((st_mode & S_IXOTH) == S_IXOTH){printf("others has execute permission\n");} };\
                                }while (0)



int main(int argc, char *argv[])
{
    int retval;
    struct stat buf;

    retval = stat(argv[1], &buf);
    ASSERT(retval);

    //(1)获取文件的inode节点编号以及文件大小，并将它们打印出来。
    printf("file's inode: %d, file's size: %d Byte\n", (int)buf.st_ino, (int)buf.st_size);

    //(2)获取文件的类型，判断此文件对于其它用户（Other）是否具有可读可写权限。
    CHECKFILETYPE(buf.st_mode);
    CHECKFILEPERMISSION(buf.st_mode);
    /*(3)获取文件的时间属性，包括文件最后被访问的时间、文件内容最后被修改的时间以及文件状态最后被改变的时间，
         并使用字符串形式将其打印出来，包括时间和日期、表示形式自定。*/
    struct tm result;
    localtime_r(&buf.st_atim.tv_sec, &result);
    char buffer[100];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &result);
    printf("access time: %s\n", buffer);

    localtime_r(&buf.st_mtim.tv_sec, &result);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &result);
    printf("modify time: %s\n", buffer);

    localtime_r(&buf.st_ctim.tv_sec, &result);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &result);
    printf("change time: %s\n", buffer);

    exit(0);
}