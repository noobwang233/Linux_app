#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONFIG_CMD  0
#define WRITE_CMD   1
#define READ_CMD    2
#define TRIGGER_CMD 3

#define CONFIG_VALUE_IN  0
#define CONFIG_VALUE_OUT 1

#define GPIO_VALUE_LOW  0
#define GPIO_VALUE_HIGH 1

#define TRIGGER_VALUE_NONE      0
#define TRIGGER_VALUE_RISING    1
#define TRIGGER_VALUE_FALLING   2
#define TRIGGER_VALUE_BOTH      3

#define GPIO_NUM_MAX 160
#define GPIO_NUM_MIN 0
#define USAGE_ERROR(cmd, gpios, value) do{ \
                                        printf("usage error, usage: %s %s %s %s\n", argv[0],\
                                                (((cmd) == NULL) ? "<config/write/read/trigger>" : cmd), \
                                                (((gpios) == NULL) ? "gpiox(0 <= x <= 160)" : gpios), \
                                                (((value) == NULL) ? "<value>" : value));\
                                }while(0)

static int CheckArg(int argc, char *argv[], int *cmd, int *gpio, int *value)
{
    char *cmds = NULL;
    char *gpios = NULL;
    char *values = NULL;

    if(argc >= 3)
    {
        if (!strcmp(argv[1], "read")) 
        {
            cmds = argv[1];
            *cmd = READ_CMD;
            if(argc != 3 || strncmp(argv[2], "gpio", 4))
            {
                goto USAGEERROR;
            }
        }
        else if (!strcmp(argv[1], "write")) 
        {
            cmds = argv[1];
            *cmd = WRITE_CMD;
            if(argc != 4 || strncmp(argv[2], "gpio", 4))
            {
                goto USAGEERROR;
            }
            if (strcmp(argv[3], "1") && strcmp(argv[3], "0")) 
            {
                values = "<0/1>";
                goto USAGEERROR;
            }
            *value = atoi(argv[3]);
            values = argv[3];
        }
        else if (!strcmp(argv[1], "config")) 
        {
            cmds = argv[1];
            *cmd = CONFIG_CMD;
            if(argc != 4 || strncmp(argv[2], "gpio", 4))
            {
                goto USAGEERROR;
            }

            if(!strcmp(argv[3], "in"))
            {
                *value = CONFIG_VALUE_IN;
            }
            else if(!strcmp(argv[3], "out"))
            {
                *value = CONFIG_VALUE_OUT;
            }
            else 
            {
                values = "<in/out>";
                goto USAGEERROR;
            }
            values = argv[3];
        }
        else if (!strcmp(argv[1], "trigger")) 
        {
            cmds = argv[1];
            *cmd = TRIGGER_CMD;
            if(argc != 4 || strncmp(argv[2], "gpio", 4))
            {
                goto USAGEERROR;
            }

            if(!strcmp(argv[3], "none"))
            {
                *value = TRIGGER_VALUE_NONE;
            }
            else if(!strcmp(argv[3], "both"))
            {
                *value = TRIGGER_VALUE_BOTH;
            }
            else if(!strcmp(argv[3], "rising"))
            {
                *value = TRIGGER_VALUE_RISING;
            }
            else if(!strcmp(argv[3], "falling"))
            {
                *value = TRIGGER_VALUE_FALLING;
            }
            else 
            {
                values = "<in/out>";
                goto USAGEERROR;
            }
            values = argv[3];
        }
        else 
        {
            goto USAGEERROR;
        }

    }
    else
    {
        goto USAGEERROR;
    }

    gpios = &argv[2][3];
    *gpio = atoi(gpios);
    if((*gpio) > GPIO_NUM_MAX || (*gpio) < GPIO_NUM_MIN)
    {
        goto USAGEERROR;
    }

    return 0;

USAGEERROR:
    USAGE_ERROR(cmds, gpios ,values);
    return -1;
}


int main(int argc, char *argv[])
{
    int ret, cmd, gpio, value;

    ret = CheckArg(argc, argv, &cmd, &gpio, &value);
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

