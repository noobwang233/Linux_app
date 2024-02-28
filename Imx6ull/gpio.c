#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMD_CONFIG  0
#define CMD_WRITE   1
#define CMD_READ    2
#define CMD_TRIGGER 3

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

#define USAGE_DEFAULT_CMD   "<config/write/read/trigger>"
#define USAGE_DEFAULT_GPIOS "gpiox(0 <= x <= 160)"
#define USAGE_DEFAULT_VALUE "<value>"
#define USAGE_ERROR(cmd, gpios, value) do{ \
                                        printf("usage error, usage: %s %s %s %s\n", argv[0],\
                                                (((cmd) == NULL) ? USAGE_DEFAULT_CMD : cmd), \
                                                (((gpios) == NULL) ? USAGE_DEFAULT_GPIOS : gpios), \
                                                (((value) == NULL) ? USAGE_DEFAULT_VALUE : value));\
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
            *cmd = CMD_READ;
            if(argc != 3 || strncmp(argv[2], "gpio", 4))
            {
                goto USAGEERROR;
            }
        }
        else if (!strcmp(argv[1], "write")) 
        {
            cmds = argv[1];
            *cmd = CMD_WRITE;
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
            *cmd = CMD_CONFIG;
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
            *cmd = CMD_TRIGGER;
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
                values = "<none/both/rising/falling>";
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
    if(argv[2][4] < '0' || argv[2][4] > '9')
    {
        goto USAGEERROR;
    }
    gpios = &argv[2][4];
    *gpio = atoi(gpios);
    printf("gpios: %s, gpio num: %d\n", gpios, *gpio);
    if((*gpio) > GPIO_NUM_MAX || (*gpio) < GPIO_NUM_MIN)
    {
        gpios = USAGE_DEFAULT_GPIOS;
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
    switch (cmd) 
    {
        case CMD_TRIGGER:
            ret = GpioTrigger(gpio, value);
        break;
        case CMD_CONFIG:
            ret = GpioConfig(gpio, value);
        break;
        case CMD_WRITE:
            ret = GpioWrite(gpio, value);
        break;
        case CMD_READ:
            ret = GpioRead(gpio);
        break;
    }
    if(ret != 0)
    {
        printf("working error, exit\n");
        exit(1);
    }
    exit(0);
}

