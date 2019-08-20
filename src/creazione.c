#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include "creazione.h"

int creaAttuatore(void (*f)())
{
    int pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        (*f)();
    }
    else
    {
        return pid;
    }
}

int creaSensore(int mode,void (*f)(int)){
    int pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        (*f)(mode);
    }
    else
    {
        return pid;
    }
}