#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void subTask(int i)
{
    //*(int *)ret = 1;
    printf("sub thread %ld count %d\n", pthread_self(), i);
    if(i > 50)
        pthread_exit((void *)-1);
}

static void *subThread(void *arg)
{
    //void *ret = malloc(sizeof(int));

    for(int i = 0; i <= atoi((char *)arg); i++)
    {
        subTask(i);
        sleep(1);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int ret;
    pthread_t subThreadID;
    void *subRet;
    ret = pthread_create(&subThreadID, NULL, subThread, (void *)(argv[1]));
    if (ret) {
        perror("pthread_create");
        exit(-1);
    }
    for(int i = 0; i <= atoi(argv[2]); i++)
    {
        printf("main thread %ld count %d\n", pthread_self(), i);
        sleep(1);
    }

    if(atoi(argv[2]) < atoi(argv[1]))
    {
        /* 向新线程发送取消请求 */ 
        ret = pthread_cancel(subThreadID); 
        if (ret) 
        { 
            fprintf(stderr, "pthread_cancel error: %s\n", strerror(ret)); exit(-1); 
        }
    }


    ret = pthread_join(subThreadID, &subRet);
    if(ret)
    {
        perror("pthread_join error");
        exit(-1);
    }

    //printf("sub thread exit, code %d\n",subRet != NULL ? (*(int *)subRet) : 0);
    printf("sub thread exit, code %ld\n",(long)subRet);
    //free(subRet);
    pthread_exit(NULL);
}