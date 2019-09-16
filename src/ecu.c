#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/un.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "creazione.h"
#include "SocketConnection.h"
#include "ecu.h"
#include "attuatori.h"
#include "sensori.h"
#include "log.h"
#include "azioni.h"
#define BRAKE 2
#define WINDSHIELD 3
#define TOTAL_COMPONENTS 4

int components[TOTAL_COMPONENTS], speed = 0;
pid_t park, surr;
void danger(int sig)
{
    kill(components[BRAKE], SIGUSR2);
    kill(components[WINDSHIELD], SIGSTOP);
    ecuLog("PERICOLO,MACCHINA FERMATA\n");
    speed = 0;
    kill(getppid(), SIGUSR1);
    pause();
}
void restart(int sig)
{
    kill(components[WINDSHIELD], SIGCONT);
    return;
}
void throttleFail(int sig)
{
    for (int i = 0; i < TOTAL_COMPONENTS; i++)
    {
        kill(-(components[i]), SIGKILL);
        wait((int *)SIGCHLD);
    }
    ecuLog("GUASTO TECNICO,ARRESTO TOTALE\n");
    exit(-1);
}
void endParking(int sig)
{
    kill(park, SIGKILL);
    wait((int *)SIGCHLD);
    kill(surr, SIGKILL);
    wait((int *)SIGCHLD);
    exit(EXIT_SUCCESS);
}

void ecu(int mode)
{
    // genero tutti i processi
    char command[255];
    struct sockaddr_un client;
    int serverD, clientD;
    unsigned int len = sizeof(client);
    signal(SIGUSR1, danger);
    signal(SIGUSR2, restart);
    signal(SIGABRT, throttleFail);
    signal(SIGALRM, endParking);
    components[0] = crea(throttleControl);
    components[1] = creaConModalita(mode, steerByWire);
    components[2] = crea(brakeByWire);
    components[3] = crea(frontWindshield);
    /*ecu si mette in ascolto per gli input */
    serverD = serverSocket(".ecu");
    listen(serverD, 5);
    while (strcmp(command, "PARCHEGGIO\n") != 0)
    {
        clientD = accept(serverD, (struct sockaddr *)&client, &len);
        if (clientD < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        if (recv(clientD, command, sizeof(command), 0) < 0)
        {
            perror("receive");
            exit(EXIT_FAILURE);
        }
        speed = ecuAction(speed, command);
        close(clientD);
    }
    char m[255];
    snprintf(m, sizeof(m), "FRENO %d\n", speed);
    int d = connectToServer(".brake");
    if (send(d, m, sizeof(m), 0) < 0)
    {
        perror("send");
        exit(EXIT_FAILURE);
    }
    close(d);
    sleep(speed / 5);
    speed = 0;
    kill(components[BRAKE], SIGUSR2);

    for (int i = 0; i < TOTAL_COMPONENTS; i++)
    {
        kill(-(components[i]), SIGKILL);
        wait((int *)SIGCHLD);
    }
    unsigned char v[16];
    int isPark = 1;
    signal(SIGABRT,endParking);
    park = creaConModalita(mode, parkAssist);
    surr = creaConModalita(mode, surroundViews);
    alarm(30);
    while (1)
    {
        memset(v,'\0',16);
        clientD = accept(serverD, (struct sockaddr *)&client, &len);
        if (clientD < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        if (recv(clientD, v, sizeof v, 0) < 0)
        {
            perror("receive");
            exit(EXIT_FAILURE);
        }
        isPark = checkParking(v);
        close(clientD);
        if (isPark == 0)
        {
            kill(park, SIGKILL);
            wait((int *)SIGCHLD);
            kill(surr, SIGKILL);
            wait((int *)SIGCHLD);
            park = creaConModalita(mode, parkAssist);
            surr = creaConModalita(mode, surroundViews);
        }
    }
    exit(EXIT_FAILURE); //should never run
}
