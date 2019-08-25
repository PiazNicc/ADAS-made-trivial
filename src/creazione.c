#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include "creazione.h"

int crea(void (*f)())
{
    int pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        setpgid(getpid(),getpid());
        (*f)();
    }
    else
    {
        return pid;
    }
}

int creaConModalita(int mode,void (*f)(int)){
    int pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        setpgid(getpid(),getpid());
        (*f)(mode);
    }
    else
    {
        return pid;
    }
}