#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <signal.h>

int main(int argc, char *argv[]) 
{ 
    char **arg_arr; 
    char *env_arr[5] = {"NAME=app", "AGE=25", "SEX=man", NULL}; 
    
    if (2 > argc)
    {
        printf("error usage\n");
        exit(-1);
    }
    printf("pid = %d\n",getpid());
    arg_arr = (char **)malloc(sizeof(char *) * (argc));
    for(int i = 0; i <= argc; i++)
    {
        if(i != argc)
            arg_arr[i] = argv[i + 1];
        else
            arg_arr[i] = NULL;
    }

    switch (fork()) {
        case -1:
            perror("fork");
            exit(-1);
        case 0:
            //child process
            // if(setsid() == -1)
            // {
            //     perror("setsid");
            //     exit(-1);
            // }
            printf("pid = %d pgid = %d sid= %d\n", getpid(), getpgid(getpid()),getsid(getpid()));
            // if (0 > chdir("/")) 
            // { 
            //     perror("chdir error"); 
            //     exit(-1);
            // }
            // /* 3.重设文件权限掩码umask */ 
            // umask(0);

            // /* 4.关闭所有文件描述符 */ 
            // for (int i = 0; i < sysconf(_SC_OPEN_MAX); i++) 
            //     close(i);
            // /* 5.将文件描述符号为0、1、2定位到/dev/null */ 
            // open("/dev/null", O_RDWR); //0
            // dup(0); //1
            // dup(0); //2
            execve(argv[1], arg_arr, env_arr);
            perror("execve error"); 
            _exit(-1);
        default:
            //father process
            sleep(30);
            printf("close father process %s\n", argv[0]);
            exit(0);
    }

}