#include <stdio.h>
#include <stdlib.h>


#define USAGE_ERROR(cmd, value) do{ \
                                        printf("usage error, usage: %s %s gpiox %s\n", argv[0],\
                                                (((cmd) == NULL) ? "<config/write/read/trigger>" : cmd), \
                                                (((value) == NULL) ? "<value>" : value));\
                                        if(cmd != NULL)\
                                            free(cmd);\
                                        if(value != NULL)\
                                            free(value);\
                                    }while(0)

static int CheckArg(int argc, char *argv[], int cmd, int value);
{
    char *cmd;
    char *value;


    return 0;

ERROR:
    USAGE_ERROR(cmd, value);
    return -1;
}


int main(int argc, char *argv[])
{
    int ret;


    ret = CheckArg(argc, argv, cmd, value);
    if(ret != 0)
    {

        exit(1);
    }
    if (strcmp(cmd, "")) 
    {

    }

    exit(0);
}

