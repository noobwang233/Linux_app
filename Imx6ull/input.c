#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>
#include <linux/input.h>

int main(int argc, char *argv[])
{
    struct pollfd pfd;
    int fd, ret;
    struct input_event in_ev = {0};
    /* 校验传参 */ 
    if (2 != argc) 
    { 
        fprintf(stderr, "usage: %s <input-dev>\n", argv[0]); 
        exit(-1); 
    }
    /* 打开文件 */ 
    if (0 > (fd = open(argv[1], O_RDONLY))) 
    { 
        perror("open error"); 
        exit(-1); 
    }

    pfd.fd = fd;
    pfd.events = POLLIN;
    while(1)
    {
        ret = poll(&pfd, 1, -1);
        if(ret < 0)
        {
            perror("poll");
            goto CLOSE_FD;
        }
        else if(ret > 0)
        {
            if(pfd.revents & POLLIN) 
            {
                ret = read(fd, &in_ev, sizeof(in_ev));
                if(ret != sizeof(in_ev))
                {
                    perror("read");
                    goto CLOSE_FD;
                }
                printf("[%ld]%s type %d code %d value %d\n", in_ev.time.tv_sec, argv[1], in_ev.code, in_ev.type, in_ev.value);
            }
        }
        else 
        {
            printf("time out\n");
        }
    }
CLOSE_FD:
    if(fd > 0)
    {
        ret = close(fd);
        if(ret)
            perror("close");
    }

    exit(0);
}
