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

#define GPIO_NUM_MAX 160
#define GPIO_NUM_MIN 0

#define USAGE_DEFAULT_CMD   "<config/write/read/trigger>"
#define USAGE_DEFAULT_GPIOS "gpiox(0 <= x <= 160)"
#define USAGE_DEFAULT_VALUE "<value>"

#define GPIO_PATH "/sys/class/gpio/"
#define GPIO_EXPORT_PATH "/sys/class/gpio/export"
#define GPIO_UNEXPORT_PATH "/sys/class/gpio/unexport"

#define EXPORT_GPIO 0
#define UNEXPORT_GPIO 1

#define USAGE_ERROR(cmd, value) do{ \
                                        printf("usage error, usage: %s %s %s %s\n", argv[0],\
                                                (((cmd) == NULL) ? USAGE_DEFAULT_CMD : cmd), \
                                                USAGE_DEFAULT_GPIOS,\
                                                (((value) == NULL) ? USAGE_DEFAULT_VALUE : value));\
                                }while(0)
#define ASSERT(x, flag) do{if((x) < 0){ perror(""); goto (flag);}}while (0)
static int CheckArg(int argc, char *argv[], int *cmd)
{
    char *cmds = NULL;
    char *values = NULL;
    char *gpios =NULL;

    if(argc == 4)
    {
        if (!strcmp(argv[1], "read")) 
        {
            cmds = argv[1];
            *cmd = CMD_READ;
            if(strcmp(argv[3], "noblock") && strcmp(argv[3], "poll"))
            {
                values = "<noblock/poll>";
                goto USAGEERROR;
            }
        }
        else if (!strcmp(argv[1], "write")) 
        {
            cmds = argv[1];
            *cmd = CMD_WRITE;

            if (strcmp(argv[3], "1") && strcmp(argv[3], "0")) 
            {
                values = "<0/1>";
                goto USAGEERROR;
            }
        }
        else if (!strcmp(argv[1], "config")) 
        {
            cmds = argv[1];
            *cmd = CMD_CONFIG;

            if(strcmp(argv[3], "in") && strcmp(argv[3], "out"))
            {
                values = "<in/out>";
                goto USAGEERROR;
            }
        }
        else if (!strcmp(argv[1], "trigger")) 
        {
            cmds = argv[1];
            *cmd = CMD_TRIGGER;

            if(strcmp(argv[3], "none") && strcmp(argv[3], "both") && strcmp(argv[3], "rising") && strcmp(argv[3], "falling"))
            {
                values = "<none/both/rising/falling>";
                goto USAGEERROR;
            }
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

    if(strncmp(argv[2], "gpio", 4))
    {
        goto USAGEERROR;
    }
    if(argv[2][4] < '0' || argv[2][4] > '9')
    {
        goto USAGEERROR;
    }
    gpios = &argv[2][4];
    printf("gpios:%s, gpio num: %d\n", gpios, atoi(gpios));
    if((atoi(gpios)) > GPIO_NUM_MAX || (atoi(gpios)) < GPIO_NUM_MIN)
    {
        goto USAGEERROR;
    }
    return 0;

USAGEERROR:
    USAGE_ERROR(cmds, values);
    return -1;
}

static int exportGpiox(char *gpios, int cmd)
{
    int ret;
    int fd;
    char *gpioNum;

    char *pathname = (char *)malloc(sizeof(GPIO_PATH)+ strlen(gpios));
    if(pathname == NULL)
    {
        perror("malloc error");
    }
    strcat(pathname, GPIO_PATH);
    strcat(pathname, gpios);
    printf("gpio pathname = %s\n", pathname);

    //check /sys/class/gpio/gpiox
    if (access(pathname, F_OK)) 
    {
        //printf("%s file doesn't exits\n", pathname);
        if(cmd == UNEXPORT_GPIO)
        {
            if(pathname != NULL)
                free(pathname);
            return 0;
        }
        //export gpiox
        fd = open(GPIO_EXPORT_PATH, O_WRONLY);
        if(fd < 0)
        {
            perror("open");
            goto FREE_MALLOC;
        }
        gpioNum = &gpios[4];
        ret = write(fd, gpioNum, strlen(gpioNum) + 1);
        if(ret < 0)
        {
            perror("write");
            goto FREE_MALLOC;
        }
        if (access(pathname, F_OK)) 
        {
            printf("can not export %s\n", gpios);
            goto FREE_MALLOC;
        }
    }
    else 
    {
        //printf("%s file exits\n", pathname);
        if(cmd == EXPORT_GPIO)
        {
            if(pathname != NULL)
                free(pathname);
            return 0;
        }
        //unexport gpiox
        fd = open(GPIO_UNEXPORT_PATH, O_WRONLY);
        if(fd < 0)
        {
            perror("open");
            goto FREE_MALLOC;
        }
        gpioNum = &gpios[4];
        ret = write(fd, gpioNum, strlen(gpioNum)+1);
        if(fd < 0)
        {
            perror("write");
            goto FREE_MALLOC;
        }
        if (!access(pathname, F_OK)) 
        {
            printf("can not unexport %s\n", gpios);
            goto FREE_MALLOC;
        }
    }

    if(fd > 0)
    {
        ret = close(fd);
        if (ret) 
        {
            perror("close error");
        }
    }
    if(pathname != NULL)
        free(pathname);
    return 0;
FREE_MALLOC:
    if(pathname != NULL)
        free(pathname);
    if(fd > 0)
    {
        ret = close(fd);
        if (ret) 
        {
            perror("close error");
        }
    }

    return -1;
}

static int GpioWork(char *gpios, char *value, int cmd)
{
    int ret;
    char *pathname;
    char *filename;
    //access gpiox path
    ret = exportGpiox(gpios, EXPORT_GPIO);
    if(ret)
    {
        printf("export %s error\n", gpios);
        return -1;
    }
    switch (cmd) 
    {
        case CMD_CONFIG:
            filename = "/direction";
        break;
        case CMD_TRIGGER:
            filename = "/edge";
        break;
        case CMD_WRITE:
            filename = "/value";
        break;
        case CMD_READ:
        break;
        default:
        break;
    }
    pathname = (char *)malloc(strlen(GPIO_PATH) + strlen(gpios) + strlen(filename) + 1);
    strcat(pathname, GPIO_PATH);
    strcat(pathname, gpios);
    strcat(pathname, filename);
    printf("file pathname = %s\n", pathname);
    ret = open(pathname, O_WRONLY);
    if (ret < 0) 
    {
        perror("open");
        goto FREE_MALLOC;
    }
    ret = 0;

    ret = exportGpiox(gpios, UNEXPORT_GPIO);
    if(ret)
    {
        printf("unexport Gpio%s error\n", gpios);
        goto FREE_MALLOC;
    }
    return 0;
FREE_MALLOC:
    if(pathname != NULL)
        free(pathname);
    return -1;
}

int main(int argc, char *argv[])
{
    int ret, cmd;

    ret = CheckArg(argc, argv, &cmd);
    if(ret != 0)
    {
        printf("check args error, exit\n");
        exit(1);
    }

    ret = GpioWork(argv[2], argv[3], cmd);
    if(ret != 0)
    {
        printf("working error, exit\n");
        exit(1);
    }
    exit(0);
}

