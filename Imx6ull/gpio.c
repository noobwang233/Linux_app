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

int main(int argc, char *argv[])
{
    int ret;
    char *cmd;
    char *value;

    ret = CheckArg(argc, argv, cmd, value);
    if(ret != 0)
    {
        USAGE_ERROR(cmd, value);
        exit(1);
    }


    exit(0);
}

