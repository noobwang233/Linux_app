#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <poll.h>

#define CMD_CONFIG_DIR  0
#define CMD_CONFIG_ACT  5
#define CMD_WRITE   1
#define CMD_READ    2
#define CMD_TRIGGER 3
#define CMD_UNEXPORT 6

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

static char* gpio;

static void sigio_handler(int signal);
static int CheckArg(int argc, char *argv[], int *cmd);
static int exportGpiox(char *gpios, int cmd);
static int GpioWork(char *gpios, char *value, int cmd);

static void sigio_handler(int signal)
{
    if(signal == SIGIO)
    {
        GpioWork(gpio, "noblock", CMD_READ);
    }

}

static int CheckArg(int argc, char *argv[], int *cmd)
{
    char *cmds = NULL;
    char *values = NULL;
    char *gpios =NULL;

    if(argc == 3)
    {
        if (!strcmp(argv[1], "unexport")) 
        {
            cmds = argv[1];
            *cmd = CMD_UNEXPORT;
            values = "";
        }
    }
    else if(argc == 4)
    {
        if (!strcmp(argv[1], "read")) 
        {
            cmds = argv[1];
            *cmd = CMD_READ;
            if(strcmp(argv[3], "noblock") && strcmp(argv[3], "poll") && strcmp(argv[3], "async"))
            {
                values = "<noblock/poll/async>";
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

            if(!strcmp(argv[3], "in") || !strcmp(argv[3], "out"))
            {
                *cmd = CMD_CONFIG_DIR;
            }
            else if (!strcmp(argv[3], "active_low") || !strcmp(argv[3], "active_high")) 
            {
                *cmd = CMD_CONFIG_ACT;
            }
            else 
            {
                values = "<in/out/active_low/active_high>";
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
    //printf("gpios:%s, gpio num: %d\n", gpios, atoi(gpios));
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
    memset(pathname, 0, strlen(pathname) + 1);
    strcat(pathname, GPIO_PATH);
    strcat(pathname, gpios);
    //printf("gpio pathname = %s\n", pathname);

    //check /sys/class/gpio/gpiox
    if (access(pathname, F_OK)) 
    {
        
        if(cmd == UNEXPORT_GPIO)
        {
            printf("%s has been unexport\n", gpios);
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
        printf("export %s successfully\n", gpios);
    }
    else 
    {
        //printf("%s file exits\n", pathname);
        if(cmd == EXPORT_GPIO)
        {
            printf("%s has been export\n", gpios);
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
        printf("unexport %s successfully\n", gpios);
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
    int ret, fd;
    char *pathname = NULL;
    char *filename = NULL;
    char read_value = 0;

    switch (cmd) 
    {
        case CMD_CONFIG_DIR:
            filename = "/direction";
        break;
        case CMD_CONFIG_ACT:
            filename = "/active_low";
            if(!strcmp(value, "active_low"))
            {
                value = "1";
            }
            else 
            {
                value = "0";
            }
        break;
        case CMD_TRIGGER:
            ret = GpioWork(gpios, "in", CMD_CONFIG_DIR);
            if(ret)
                return -1;
            filename = "/edge";
        break;
        case CMD_WRITE:
            ret = GpioWork(gpios, "out", CMD_CONFIG_DIR);
            if(ret)
                return -1;
            filename = "/value";
        break;
        case CMD_READ:
            ret = GpioWork(gpios, "in", CMD_CONFIG_DIR);
            if(ret)
                return -1;
            filename = "/value";
        break;
        case CMD_UNEXPORT:
            ret = exportGpiox(gpios, UNEXPORT_GPIO);
            if(ret)
            {
                printf("unexport Gpio%s error\n", gpios);
                goto FREE_MALLOC;
            }
            return 0;
        break;
        default:
            filename = NULL;
        break;
    }

    //access gpiox path
    ret = exportGpiox(gpios, EXPORT_GPIO);
    if(ret)
    {
        printf("export %s error\n", gpios);
        return -1;
    }

    pathname = (char *)malloc(strlen(GPIO_PATH) + strlen(gpios) + strlen(filename) + 1);
    if(pathname == NULL)
    {
        perror("malloc error");
    }
    memset(pathname, 0, strlen(pathname) + 1);
    strcat(pathname, GPIO_PATH);
    strcat(pathname, gpios);
    strcat(pathname, filename);
    //printf("file pathname = %s\n", pathname);
    switch (cmd) 
    {
        case CMD_CONFIG_DIR:
        case CMD_CONFIG_ACT:
        case CMD_TRIGGER:
        case CMD_WRITE:
            fd = open(pathname, O_WRONLY);
            if (fd < 0) 
            {
                perror("open");
                goto FREE_MALLOC;
            }
            ret = write(fd, value, strlen(value)+1);
            if(ret != strlen(value)+1)
            {
                perror("write");
                goto CLOSE_FD;
            }
        break;
        case CMD_READ:
            if(!strcmp(value, "noblock"))
            {
                fd = open(pathname, O_RDONLY);
                if (fd < 0) 
                {
                    perror("open");
                    goto FREE_MALLOC;
                }
                ret = read(fd, &read_value, 1);
                if(ret != 1)
                {
                    perror("read");
                    goto CLOSE_FD;
                }
                printf("%s value = %c\n", gpios, read_value);
                break;
            }

            //check irq
            if(pathname != NULL)
            {
                free(pathname);
            }

            filename = "/edge";
            pathname = (char *)malloc(strlen(GPIO_PATH) + strlen(gpios) + strlen(filename) + 1);
            if(pathname == NULL)
            {
                perror("malloc error");
            }
            memset(pathname, 0, strlen(pathname) + 1);
            strcat(pathname, GPIO_PATH);
            strcat(pathname, gpios);
            strcat(pathname, filename);

            fd = open(pathname, O_RDONLY);
            if (fd < 0) 
            {
                perror("open");
                goto FREE_MALLOC;
            }

            char buff[4] = {0};
            ret = read(fd, buff, 4);
            if(ret <= 0)
            {
                perror("read");
                goto CLOSE_FD;
            }
            if(!strncmp(buff, "none", 4))
            {
                printf("please config trigger first, usage: tirgger gpiox <none/both/rising/falling>\n");
                goto CLOSE_FD;
            }
            ret = close(fd);
            if(ret)
                perror("close");


            //open value
            if(pathname != NULL)
            {
                free(pathname);
            }
            filename = "/value";
            pathname = (char *)malloc(strlen(GPIO_PATH) + strlen(gpios) + strlen(filename) + 1);
            if(pathname == NULL)
            {
                perror("malloc error");
            }
            memset(pathname, 0, strlen(pathname) + 1);
            strcat(pathname, GPIO_PATH);
            strcat(pathname, gpios);
            strcat(pathname, filename);

            fd = open(pathname, O_RDONLY);
            if (fd < 0) 
            {
                perror("open");
                goto FREE_MALLOC;
            }


            if(!strcmp(value, "poll"))
            {

            }
            else if(!strcmp(value, "async"))
            {
                int flag; 

                flag = fcntl(fd, F_GETFL); //先获取原来的flag 
                flag |= O_ASYNC; //将O_ASYNC标志添加到flag 
                fcntl(fd, F_SETFL, flag); //重新设置flag
                fcntl(fd, F_SETOWN, getpid());

                signal(SIGIO, sigio_handler);
                if(gpio != NULL)
                {
                    free(gpio);
                }
                gpio = (char *)malloc(strlen(gpios) + 1);
                if(gpio == NULL)
                {
                    perror("malloc error");
                }
                memset(gpio, 0, strlen(gpio) + 1);
                strcat(gpio, gpios);
                while (1) 
                {
                    sleep(1);
                }
            }
            else 
            {
            
            }
        break;
        default:
        break;
    }
    if(gpio != NULL)
    {
        free(gpio);
    }
    if(pathname != NULL)
        free(pathname);
    ret = close(fd);
    if(ret)
        perror("close");
    return 0;

CLOSE_FD:
    if(fd > 0)
    {
        ret = close(fd);
        if(ret)
            perror("close");
    }
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

