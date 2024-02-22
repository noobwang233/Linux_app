#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void subTask(int i)
{
    void *ret = malloc(sizeof(int));
    *(int *)ret = 1;
    printf("sub thread %ld count %d\n", pthread_self(), i);
    if(i > 50)
        pthread_exit(ret);
}

static void *subThread(void *arg)
{
    for(int i = 0; i <= atoi((char *)arg); i++)
    {
        subTask(i);
        sleep(1);
    }
    return NULL;
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
    ret = pthread_join(subThreadID, &subRet);
    if(ret)
    {
        perror("pthread_join error");
        exit(-1);
    }
    printf("sub thread exit, code %d\n",subRet != NULL ? (*(int *)subRet) : 0);
    free(subRet);
    pthread_exit(NULL);
}