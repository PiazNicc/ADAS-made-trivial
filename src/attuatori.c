#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include "SocketConnection.h"
#include "attuatori.h"
#include "log.h"
#include "azioni.h"

int throttleFlag = 0;
int brakeFlag = 0;
int steerFlag = 0;

void killC(int sig)
{
    exit(EXIT_SUCCESS);
}
void sj(int sig)
{
    return;
}

void throttleflagHandle(int sig)
{
    throttleFlag = (throttleFlag + 1) % 2;
    return;
}
void brakeFlagHandle(int sig)
{
    brakeFlag = (brakeFlag + 1) % 2;
    return;
}
void steerflagHandle(int sig)
{
    steerFlag = (steerFlag + 1) % 2;
    return;
}

void dangerHandler(int sig)
{   brakeLog("ARRESTO AUTO\n");
    return;
}

//quando sono finiti gli attuatori li mettiamo tutti insieme per facilitare la compilazione
void throttleControl()
{
    signal(SIGUSR2, sj);
    char *message = malloc(255);
    FILE *f = fopen("log/throttle.log", "w");
    fprintf(f, __DATE__);
    fclose(f);
    int serverD, ECUclientD;
    unsigned int clientLen;
    struct sockaddr_un ecuAddr;
    clientLen = sizeof(ecuAddr);
    int child = fork();
    if (child < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (child == 0)
    { //scrive no action mentre processo padre aspetta richieste
        //il flag serve a sincronizzare i due processi come se fosse un "lock" fra thread
        signal(SIGUSR1, throttleflagHandle);
        printf("%d\n", getpid());
        kill(getppid(), SIGUSR2);
        for (;;)
        {
            if (throttleFlag == 0)
            {

                throttleLog(0);
            }
            else
            {
                pause();
            }
        }
    }
    else
    {
        pause();
        serverD = serverSocket(".throttle");
        listen(serverD, 5);

        while (1)
        {
            ECUclientD = accept(serverD, (struct sockaddr *)&ecuAddr, &clientLen);
            if (ECUclientD < 0)
            {
                perror("impossibile connettersi");
                exit(EXIT_FAILURE);
            }
            if (recv(ECUclientD, message, 255, 0) < 0)
            {
                perror("impossibile leggere\n");
                exit(EXIT_FAILURE);
            }
            kill(child, SIGUSR1);
            throttleAction(message);
            close(ECUclientD);
            kill(child, SIGUSR1);
        }
    }
}

void brakeByWire()
{
    signal(SIGUSR1, sj);
    signal(SIGUSR2,dangerHandler);
    char *message = malloc(255);
    FILE *f = fopen("log/brake.log", "w");
    fprintf(f, __DATE__);
    fclose(f);
    int serverD, ECUclientD;
    unsigned int clientLen;
    struct sockaddr_un ecuAddr;
    clientLen = sizeof(ecuAddr);
    int child = fork();
    if (child < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (child == 0)
    { //scrive no action mentre processo padre aspetta richieste
        //il flag serve a sincronizzare i due processi come se fosse un "lock" fra thread
        signal(SIGUSR1, brakeFlagHandle);
        printf("%d\n", getpid());
        kill(getppid(), SIGUSR1);
        for (;;)
        {
            if (brakeFlag == 0)
            {

                brakeLog("NO ACTION\n");
            }
            else
            {
                pause();
            }
        }
    }
    else
    {
        pause();
        serverD = serverSocket(".brake");
        listen(serverD, 5);

        while (1)
        {
            ECUclientD = accept(serverD, (struct sockaddr *)&ecuAddr, &clientLen);
            kill(child, SIGUSR1);
            if (ECUclientD < 0)
            {
                perror("impossibile connettersi");
                exit(EXIT_FAILURE);
            }
            if (recv(ECUclientD, message, 255, 0) < 0)
            {
                perror("impossibile leggere\n");
                exit(EXIT_FAILURE);
            }
            brakeAction(message);
            close(ECUclientD);
            kill(child, SIGUSR1);
        }
    }
}

void steerByWire()
{

    signal(SIGUSR2, sj);
    char *message = malloc(10);
    memset(message,NULL,sizeof(message));
    FILE *f = fopen("log/steer.log", "w");
    fprintf(f, __DATE__);
    fclose(f);
    int serverD, ECUclientD;
    unsigned int clientLen;
    struct sockaddr_un ecuAddr;
    clientLen = sizeof(ecuAddr);
    int child = fork();
    if (child < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (child == 0)
    { //scrive no action mentre processo padre aspetta richieste
        //il flag serve a sincronizzare i due processi come se fosse un "lock" fra thread
        signal(SIGUSR1, steerflagHandle);
        printf("%d\n", getpid());
        kill(getppid(), SIGUSR2);
        for (;;)
        {
            if (steerFlag == 0)
            {

                steerLog("NO ACTION");
            }
            else
            {
                pause();
            }
        }
    }
    else
    {
        pause();
        serverD = serverSocket(".steer");
        listen(serverD, 5);
        while (1)
        {
            ECUclientD = accept(serverD, (struct sockaddr *)&ecuAddr, &clientLen);
            kill(child, SIGUSR1);
            if (ECUclientD < 0)
            {
                perror("impossibile connettersi");
                exit(EXIT_FAILURE);
            }
            if (recv(ECUclientD, message,10, 0) < 0)
            {
                perror("impossibile leggere\n");
                exit(EXIT_FAILURE);
            }
            printf(message);
            steerLog(message);
            close(ECUclientD);
            kill(child, SIGUSR1);
            memset(message,0,10);
        }
    }
}