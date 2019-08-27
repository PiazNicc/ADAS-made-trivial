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

static volatile int throttleFlag = 1;
static volatile int brakeFlag = 1;
static volatile int steerFlag = 1;

void killC(int sig)
{
    exit(EXIT_SUCCESS);
}


void throttleflagHandle(int sig)
{
    throttleFlag = 1-throttleFlag;
    return;
}
void brakeFlagHandle(int sig)
{
    brakeFlag = 1-brakeFlag;
    return;
}
void steerflagHandle(int sig)
{
    steerFlag = 1-steerFlag;
    return;
}

void dangerHandler(int sig)
{
    brakeLog("ARRESTO AUTO\n");
    return;
}

//quando sono finiti gli attuatori li mettiamo tutti insieme per facilitare la compilazione
void throttleControl()
{
    signal(SIGUSR1, throttleflagHandle);
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
        kill(getppid(), SIGUSR1);
        for (;;)
        {
            if (throttleFlag == 1)
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
    signal(SIGUSR1, brakeFlagHandle);
    signal(SIGUSR2, dangerHandler);
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
        kill(getppid(), SIGUSR1);
        for (;;)
        {
            if (brakeFlag == 1)
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

    signal(SIGUSR1, brakeFlagHandle);
    char *message = malloc(10);
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
        kill(getppid(), SIGUSR1);
        for (;;)
        {
            if (steerFlag == 1)
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
            if (recv(ECUclientD, message, 10, 0) < 0)
            {
                perror("impossibile leggere\n");
                exit(EXIT_FAILURE);
            }
            steerLog(message);
            close(ECUclientD);
            kill(child, SIGUSR1);
            memset(message, 0, 10);
        }
    }
}