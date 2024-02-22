#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h> 
#include <sys/wait.h>

static void handle_child(int sig)
{
    int status;
    pid_t pid;
    while(1)
    {
        pid = waitpid(-1,&status,WNOHANG);
        if(pid == 0)
            break;
        if(pid > 0)
        {
            printf("father handle child process %d pid %d\n", WEXITSTATUS(status), pid);
            continue;
        }
        else 
        {
            if(errno == ECHILD)
                printf("no child process\n");
            else
                perror("waitpid error");
            break;
        }
    }
}


int main(int argc, char *argv[])
{
    int ret;
    unsigned int count = 0;
    struct sigaction sig = {0};
    if(argc != 3)
        printf("error usage, usage %s <child process num> <father process count>\n", argv[0]);

    /* 为SIGCHLD信号绑定处理函数 */ 
    sigemptyset(&sig.sa_mask);//清空掩码
    sig.sa_handler = handle_child; 
    sig.sa_flags = 0;
    if (-1 == sigaction(SIGCHLD, &sig, NULL)) 
    {
        perror("sigaction error"); 
        exit(-1); 
    }

    //create child process
    for(int i = 0; i < atoi(argv[1]); i++)
    {
        ret = fork();
        switch (ret) 
        {
            case -1:
                perror("create child process failed\n");
                break;
            case 0:
                //child process
                count = 0;
                for (; ;) {
                    printf("child process %d pid: %d output: %d\n",i,getpid(),count);
                    count++;
                    sleep(1);
                    if(count > ((i+1) * 5))
                        break;
                }
                printf("child process %d pid: %d exit, father pid %d\n",i,getpid(),getppid());
                _exit(i);
                break;
            default:
                //father process
                break;
        }
    }

    //father process
    count = 0;
    for (; ;) {
        printf("count: %d\n",count);
        count++;
        sleep(1);
        if(count > (atoi(argv[2])))
            break;
    }

    exit(0);
}