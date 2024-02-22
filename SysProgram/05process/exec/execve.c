#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
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

    execve(argv[1], arg_arr, env_arr);
    perror("execve error"); 
    exit(-1); 
}