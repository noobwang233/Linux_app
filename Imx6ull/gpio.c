#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>

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

#define GPIO_PATH "/sys/class/gpio/"
#define GPIO_EXPORT_PATH "/sys/class/gpio/export"

#define USAGE_ERROR(cmd, value) do{ \
                                        printf("usage error, usage: %s %s %s %s\n", argv[0],\
                                                (((cmd) == NULL) ? USAGE_DEFAULT_CMD : cmd), \
                                                USAGE_DEFAULT_GPIOS,\
                                                (((value) == NULL) ? USAGE_DEFAULT_VALUE : value));\
                                }while(0)
#define ASSERT(x, flag) do{if((x) < 0){ perror(""); goto (flag)}}while (0)
static int CheckArg(int argc, char *argv[], int *cmd, char **gpios, int *value)
{
    char *cmds = NULL;
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
    *gpios = &argv[2][4];
    printf("gpios: %s, gpio num: %d\n", *gpios, atoi(*gpios));
    if((atoi(*gpios)) > GPIO_NUM_MAX || (atoi(*gpios)) < GPIO_NUM_MIN)
    {
        goto USAGEERROR;
    }
    return 0;

USAGEERROR:
    USAGE_ERROR(cmds, values);
    return -1;
}

static int openGpiox(char *gpios, int cmd)
{
    int ret;
    int exportFd;

    char *pathname = (char *)malloc(sizeof(GPIO_PATH) + sizeof("gpio") + strlen(gpios));
    if(pathname == NULL)
    {
        perror("malloc error");
    }
    strcat(pathname, GPIO_PATH);
    strcat(pathname, "gpio");
    strcat(pathname, gpios);
    printf("gpio pathname = %s\n", pathname);

    //check /sys/class/gpio/gpiox
    if (!access(pathname, F_OK)) 
    {
        //printf("%s file doesn't exits\n", pathname);
        //export gpiox
        exportFd = open(GPIO_EXPORT_PATH, O_WRONLY);
        ASSERT(exportFd, FREE_MALLOC);
        ret = write(exportFd, gpios, strlen(gpios));
        ASSERT(exportFd, FREE_MALLOC);
        if (!access(pathname, F_OK)) 
        {
            printf("can not export gpio%s\n", gpios);
            goto FREE_MALLOC;
        }
    }
    


    if(pathname != NULL)
        free(pathname);
    return 0;
FREE_MALLOC:
    if(pathname != NULL)
        free(pathname);
    return -1;
}

static int GpioTrigger(char *gpios, int value)
{
    int ret;
    //open gpiox path
    ret = openGpiox(gpios, CMD_TRIGGER);
    ASSERT(ret);

}

int main(int argc, char *argv[])
{
    int ret, cmd, value;
    char *gpios = NULL;

    ret = CheckArg(argc, argv, &cmd, &gpios, &value);
    if(ret != 0)
    {
        printf("check args error, exit\n");
        exit(1);
    }
    switch (cmd) 
    {
        case CMD_TRIGGER:
            ret = GpioTrigger(gpios, value);
        break;
        // case CMD_CONFIG:
        //     ret = GpioConfig(gpios, value);
        // break;
        // case CMD_WRITE:
        //     ret = GpioWrite(gpios,value);
        // break;
        // case CMD_READ:
        //     ret = GpioRead(gpios);
        // break;
        default:
        break;
    }
    if(ret != 0)
    {
        printf("working error, exit\n");
        exit(1);
    }
    exit(0);
}

