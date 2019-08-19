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
#include <time.h>
#include "SocketConnection.h"
#include "attuatori.h"
#include "azioni.h"

int throttleFlag = 0;
int brakeFlag = 0;
int steerFlag = 0;

void throttleflagHandle(int sig)
{
    throttleFlag = (throttleFlag + 1) % 2;
}
void  brakeFlagHandle(int sig)
{
    brakeFlag = (brakeFlag + 1) % 2;
}
void steerFlagHandle(int sig)
{
    steerFlag = (steerFlag + 1) % 2;
}
void exitHandler(int sig)
{
    kill(0, SIGKILL);
    exit(EXIT_SUCCESS);
}

void dangerHandler(int sig)
{
    FILE *f = fopen("log/brake.log", "a");
    fprintf(f, "ARRESTO AUTO");
    fclose(f);
}


//quando sono finiti gli attuatori li mettiamo tutti insieme per facilitare la compilazione
void throttleControl()
{
    char message[15];
    memset(message, 0, sizeof(message));
    FILE *f = fopen("log/throttle.log", "w");
    fprintf(f, __DATE__);
    fclose(f);
    int serverD, ECUclientD;
    unsigned int clientLen;
    signal(SIGKILL, exitHandler);
    signal(SIGUSR2, throttleflagHandle);
    struct sockaddr_un ecuAddr;
    clientLen = sizeof(ecuAddr);
    printf("in attesa...\n");
    serverD = serverSocket("throttle");
    listen(serverD, 5);
    int child = fork();
    if (child < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (child == 0)
    { //scrive no action mentre processo padre aspetta richieste
        //il flag serve a sincronizzare i due processi come se fosse un "lock" fra thread
        for (;;)
        {
            if (throttleFlag == 0)
            {
                throttleLog(0);
            }
        }
    }
    else
    {
        while (1)
        {

            ECUclientD = accept(serverD, (struct sockaddr *)&ecuAddr, &clientLen);
            kill(child, SIGUSR1);
            if (ECUclientD < 0)
            {
                fprintf(stderr, "impossibile connettersi");
            }
            if (recv(ECUclientD, message, sizeof(message), 0) < 0)
            {
                fprintf(stderr, "impossibile leggere");
            }
            throttleAction(message);
            kill(child, SIGUSR1);
            close(ECUclientD);
        }
    }
}

void brakeByWire()
{
    char message[10];
    FILE *f = fopen("log/brake.log", "w");
    fprintf(f, "\n");
    fclose(f);
    int serverD, ECUclientD;
    unsigned int clientLen;
    signal(SIGKILL, exitHandler);
    signal(SIGUSR1, dangerHandler);
    signal(SIGUSR2, brakeFlagHandle);
    struct sockaddr_un ecuAddr;
    clientLen = sizeof(ecuAddr);
    printf("in attesa...\n");
    serverD = serverSocket("brake");
    listen(serverD, 5);
    int child = fork();
    if (child < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (child == 0)
    { //scrive no action mentre processo padre aspetta richieste

        for (;;)
        {
            if (brakeFlag == 0)
            {
                brakeLog("NO ACTION");
            }
        }
    }
    else
    {
        while (1)
        {

            ECUclientD = accept(serverD, (struct sockaddr *)&ecuAddr, &clientLen);
            kill(child, SIGUSR2);
            if (ECUclientD < 0)
            {
                fprintf(stderr, "impossibile connettersi");
            }
            if (recv(ECUclientD, message, sizeof(message), 0) < 0)
            {
                fprintf(stderr, "impossibile leggere");
            }
            brakeAction(message);
            kill(child, SIGUSR2);
            close(ECUclientD);
        }
    }
}

void steerByWire()
{
    char message[15];
    memset(message, 0, sizeof(message));
    FILE *f = fopen("log/steer.log", "w");
    fprintf(f, __DATE__);
    fclose(f);
    int serverD, ECUclientD;
    unsigned int clientLen;
    signal(SIGKILL, exitHandler);
    signal(SIGUSR1, steerFlagHandle);
    struct sockaddr_un ecuAddr;
    clientLen = sizeof(ecuAddr);
    printf("in attesa...\n");
    serverD = serverSocket("steer");
    listen(serverD, 5);
    int child = fork();
    if (child < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (child == 0)
    { //scrive no action mentre processo padre aspetta richieste

        for (;;)
        {
            if (steerFlag == 0)
            {
                steerLog("NO ACTION");
            }
        }
    }
    else
    {
        while (1)
        {
            ECUclientD = accept(serverD, (struct sockaddr *)&ecuAddr, &clientLen);
            kill(child, SIGUSR1);
            if (ECUclientD < 0)
            {
                fprintf(stderr, "impossibile connettersi");
            }
            if (recv(ECUclientD, message, sizeof(message), 0) < 0)
            {
                fprintf(stderr, "impossibile leggere");
            }
            steerLog(message);
            kill(child, SIGUSR1);
            close(ECUclientD);
        }
    }
}