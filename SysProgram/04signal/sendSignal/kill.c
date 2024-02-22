#include <stdio.h> 
#include <stdlib.h> 
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#define ASSERT(x) do{if((x) < 0){ perror(""); exit(errno);}}while (0)

static void sigHandler(int sig)
{
    printf("Received sig %d\n", sig);
}


int main(int argc, char *argv[])
{
    sig_t sig;

    sig = signal(SIGINT, sigHandler);
    if(sig == SIG_ERR)
    {
        perror("signal handler register failed\n");
        exit(-1);
    }

    // // 让内核 6s 之后向本进程发送SIGALRM信号
    // alarm(atoi(argv[1]));
    // printf("start sleep %d seconds\n", atoi(argv[1]));

    for(;;)
    {
        kill(getpid(),SIGINT);
        sleep(5);
    }


    exit(0);
}