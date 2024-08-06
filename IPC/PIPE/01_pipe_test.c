
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <unistd.h> 
#include <sys/types.h> 


int main(int argc, char *argv[]) 
{ 
    int ret;
    int pipefd[2];

    ret = pipe(pipefd);
    if(ret)
        perror("pipe error");

    switch (fork()) 
    { 
        case -1: 
            perror("fork error"); 
            exit(-1); 
        case 0: 
            /* 子进程 */ 
            ret = close(pipefd[0]);//子进程关闭读取端口，表示子进程要开始发送数据
            if(ret)
                perror("close error");
            sleep(1);
            char *buf = "test\r\n";
            ret = write(pipefd[1], buf, strlen(buf));
            if(!ret)
                perror("write error");
            ret = close(pipefd[1]);
            if(ret)
                perror("close error");
            _exit(0); 
        default: 
            /* 父进程进程 */ 
            ret = close(pipefd[1]);//父进程关闭写端口，表示父进程要开始接收数据
            if(ret)
                perror("close error");
            sleep(2);
            char buf2[50];
            ret = read(pipefd[0], buf2, sizeof(buf2));
            if(!ret)
                perror("read error");
            printf("father received:%s", buf2);
            ret = close(pipefd[0]);
            if(ret)
                perror("close error");
            exit(0); 
    }
    return 0;
}


