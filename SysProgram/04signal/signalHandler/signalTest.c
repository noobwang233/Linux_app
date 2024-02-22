#include <stdio.h> 
#include <stdlib.h> 
#include <signal.h>
#include <errno.h>

#define ASSERT(x) do{if((x) < 0){ perror(""); exit(errno);}}while (0)

static void sig_handler(int sig) 
{ 
    printf("Received signal: %d\n", sig); 
} 

int main(int argc, char *argv[]) 
{ 
    // sig_t ret = NULL; 
    
    // ret = signal(SIGINT, (sig_t)sig_handler); 
    // if (SIG_ERR == ret) 
    // {
    //     perror("signal error"); 
    //     exit(-1); 
    // }

    int ret;
    struct sigaction sigact;
    sigact.sa_handler = sig_handler;
    sigact.sa_flags = 0;
    ret = sigaction(SIGINT, &sigact, NULL);
    ASSERT(ret);

    /* 死循环 */ 
    for ( ; ; ) { }

    exit(0); 
}