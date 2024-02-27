#include <stdio.h>
#include <stdlib.h>

#define CONFIG_CMD  0
#define WRITE_CMD   1
#define READ_CMD    2
#define TRIGGER_CMD 3
#define USAGE_ERROR(cmd, value) do{ \
                                        printf("usage error, usage: %s %s gpiox %s\n", argv[0],\
                                                (((cmd) == NULL) ? "<config/write/read/trigger>" : cmd), \
                                                (((value) == NULL) ? "<value>" : value));\
                                    }while(0)






static int CheckArg(int argc, char *argv[], int *cmd, int *value)
{
    char *cmds = NULL;
    char *values = NULL;

    if(argc >= 3)
    {

    }
    else
    {
        goto USAGEERROR;
    }

    return 0;

USAGEERROR:
    USAGE_ERROR(cmds, values);
//MALLOC_ERROR
    if(cmds != NULL)
        free(cmds);
    if(values != NULL)
        free(values);
    return -1;
}


int main(int argc, char *argv[])
{
    int ret, cmd, value;

    ret = CheckArg(argc, argv, &cmd, &value);
    if(ret != 0)
    {
        printf("check args error, exit\n");
        exit(1);
    }
    // if (strcmp(cmd, "")) 
    // {

    // }

    exit(0);
}

