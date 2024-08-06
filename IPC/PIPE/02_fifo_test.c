 #include <sys/wait.h>
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <unistd.h>
 #include <errno.h>
 #include <fcntl.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <limits.h>
 #include <string.h>

 #define MYFIFO "myfifo"    /* 命名管道文件名*/

 #define MAX_BUFFER_SIZE PIPE_BUF /* 4096 定义在于 limits.h 中*/


void fifo_read(void)
{
    char buff[MAX_BUFFER_SIZE];
    int fd;
    int nread;

    printf("***************** read fifo ************************\n");
    /* 判断命名管道是否已存在，若尚未创建，则以相应的权限创建*/
    if (access(MYFIFO, F_OK) == -1)                 //(4)
    {
        if ((mkfifo(MYFIFO, 0666) < 0) && (errno != EEXIST))    //(5)
        {
            printf("Cannot create fifo file\n");
            exit(1);
        }
    }

    /* 以只读阻塞方式打开命名管道 */
    fd = open(MYFIFO, O_RDONLY);                //(6)
    if (fd == -1)
    {
        printf("Open fifo file error\n");
        exit(1);
    }

    memset(buff, 0, sizeof(buff));

    if ((nread = read(fd, buff, MAX_BUFFER_SIZE)) > 0)      // (7)
    {
        printf("Read '%s' from FIFO\n", buff);
    }

    printf("***************** close fifo ************************\n");

    close(fd);                              //(8)

    exit(0);
}


void fifo_write(void)
{
    int fd;
    char buff[] = "this is a fifo test demo";
    int nwrite;

    sleep(2);   //等待子进程先运行              //(9)

    printf("***************** write fifo ************************\n");
    /* 以只写阻塞方式打开 FIFO 管道 */
    fd = open(MYFIFO, O_WRONLY | O_CREAT, 0644);        //(10)
    if (fd == -1)
    {
        printf("Open fifo file error\n");
        exit(1);
    }

    printf("Write '%s' to FIFO\n", buff);

    /*向管道中写入字符串*/
    nwrite = write(fd, buff, MAX_BUFFER_SIZE);          //(11)

    printf("***************** 2 close fifo ************************\n");
    if(wait(NULL))  //等待子进程退出
    {
        close(fd);                          //(12)
        exit(0);
    }

}


 int main()
 {
    pid_t result;
    /*调用 fork()函数*/
    result = fork();                //(1)

    /*通过 result 的值来判断 fork()函数的返回情况，首先进行出错处理*/
    if(result == -1)
    {
        printf("Fork error\n");
    }


    else if (result == 0) /*返回值为 0 代表子进程*/
    {
        fifo_read();            //(2)
    }

    else /*返回值大于 0 代表父进程*/
    {
        fifo_write();       //(3)
    }

    return result;
 }