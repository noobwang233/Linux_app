#include <stdio.h> 
#include <stdlib.h> 
#include <signal.h>

static void sig_handler(int sig) 
{ 
    printf("Received signal: %d\n", sig); 
} 

int main(int argc, char *argv[]) 
{ 
    sig_t ret = NULL; 
    
    ret = signal(SIGINT, (sig_t)sig_handler); 
    if (SIG_ERR == ret) 
    {
        perror("signal error"); exit(-1); 
    } 
    
    /* 死循环 */ 
    for ( ; ; ) { } 
    exit(0); 
}