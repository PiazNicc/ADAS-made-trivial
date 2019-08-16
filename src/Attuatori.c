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
#include "Attuatori.h"
int flag = 0;
void flagHandle(int sig)
{
    flag = (flag + 1) % 2;
}
void handler(int sig)
{
    kill(0, SIGINT);
    exit(EXIT_SUCCESS);
};
void throttleAction(char *message)
{
    int incAmount;
    incAmount = findAmount(message, 11);
    throttleLog(incAmount);
}
int findAmount(char str[], int pos)
{
    int lengthOfSub = strlen(str) - pos;
    int amount;
    char subStr[lengthOfSub];

    int i = 0;
    while (i < lengthOfSub)
    {
        subStr[i] = str[pos + i];
        i++;
    }

    return amount = atoi(subStr);
}
int checkFailure()
{
    return rand() < 0.00001 * ((double)RAND_MAX + 1.0);
}
int throttleLog(int a)
{
    srand((unsigned int)time(0));
    FILE *fd = fopen("throttle.log", "a");
    char *ma[] = {" AUMENTO 5\n", " NO ACTION\n"}, buff[20];
    int len[] = {strlen(" AUMENTO 5\n"), strlen(" NO ACTION\n")};

    if (a == 0)
    {

        if (fwrite(ma[1], 1, len[1], fd) <= 0)
        {
            perror("impossibile scrivere");
        }
        sleep(1);
    }
    else
    {
        while (a > 0)
        {
            if (fwrite(ma[0], 1, len[0], fd) <= 0)
            {
                perror("errore in scrittura");
            };
            if (checkFailure() == 1)
            {
                kill(getppid(), SIGSTOP);
                break;
            }
            a -= 5;
            fflush(fd);
            sleep(1);
        }
    }
    fclose(fd);
    return a;
}
void dangerHandler(int sig)
{
    FILE *f = fopen("brake.log", "a");
    fprintf(f, "ARRESTO AUTO");
    fclose(f);
}

void decreaseSpeed(int amount)
{
    FILE *f = fopen("brake.log", "a");
    int len = strlen("DECREMENTO 5\n");
    while (amount != 0)
    {

        fwrite("DECREMENTO 5\n", 1, len, f);
        amount = amount - 5;
        fflush(f);

        sleep(1);
    }
    fclose(f);
}
void brakeAction(char *message)
{
    int decAmount;

    if (strcmp(message, "PARCHEGGIO") == 0)
    {
        brakeLog("ARRESTO AUTO");
    }

    else
    {
        decAmount = findAmount(message, 6);
        decreaseSpeed(decAmount);
    }
}

void brakeLog(char *message)
{
    FILE *f = fopen("brake.log", "a");
    int len = strlen(message);
    fprintf(f, "%s\n", message);
    fclose(f);
    sleep(1);
}
void steerLog(unsigned char *message)
{
    FILE *steerPunt = fopen("steer.log", "a");

    int k;
    if (strcmp(message, "DESTRA") == 0 || strcmp(message, "SINISTRA") == 0)
    {
        for (int j = 0; j < 4; j++)
        {
            fprintf(steerPunt, "STO GIRANDO A %s\n", message);
            fflush(steerPunt);
            sleep(1);
        }
    }
    else
    {
        fprintf(steerPunt, "NO ACTION\n");
        fclose(steerPunt);
        sleep(1);
    }
}

//quando sono finiti gli attuatori li mettiamo tutti insieme per facilitare la compilazione
void throttleControl()
{
    char message[15];
    memset(message, 0, sizeof(message));
    FILE *f = fopen("throttle.log", "w");
    fprintf(f, __DATE__);
    fclose(f);
    int serverD, ECUclientD, clientLen, amount;
    signal(SIGINT, handler);
    signal(SIGUSR1, flagHandle);
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
            if (flag == 0)
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
    FILE *f = fopen("brake.log", "w");
    fprintf(f, "\n");
    fclose(f);
    int serverD, ECUclientD, clientLen, amount;
    signal(SIGINT, handler);
    signal(SIGUSR1, dangerHandler);
    signal(SIGUSR2, flagHandle);
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
            if (flag == 0)
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
    FILE *f = fopen("steer.log", "w");
    fprintf(f, __DATE__);
    fclose(f);
    int serverD, ECUclientD, clientLen, amount;
    signal(SIGINT, handler);
    signal(SIGUSR1, flagHandle);
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
            if (flag == 0)
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