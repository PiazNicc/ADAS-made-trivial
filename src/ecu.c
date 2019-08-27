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
#define THROTTLE 0
#define STEER 1
#define BRAKE 2
#define WINDSHIELD 3
int components[4],speed = 0;
void danger(int sig)
{
    kill(components[BRAKE], SIGUSR2);
    for (int i = 0; i < 4; i++)
    {
        kill(-(components[i]), SIGSTOP);
        // wait((int *)SIGCHLD);
        //("UCCIDO %d\n", components[i]);
    }
    ecuLog("PERICOLO\n");
    speed = 0;
    kill(getppid(), SIGUSR1);
    pause();
    // exit(0);
}
void restart(int sig)
{
    for (int i = 0; i < 4; i++)
    {
        kill(-components[i], SIGCONT);
    }
    return;
}
void throttleFail(int sig)
{
    kill(getppid(),SIGUSR2);
    for (int i = 0; i < 4; i++)
    {
        kill(-(components[i]), SIGKILL);
        wait((int *)SIGCHLD);
    }
    exit(-1);
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
    signal(SIGIO, throttleFail);
    components[0] = crea(throttleControl);
    components[1] = crea(steerByWire);
    components[2] = crea(brakeByWire);
    components[3] = crea(frontWindshield);
    //int pidRadar = creaSensore(mode, forwardFacing);
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
    speed = 0;
    kill(components[BRAKE], SIGUSR2);

    for (int i = 0; i < 4; i++)
    {
        kill(-(components[i]), SIGKILL);
        wait((int *)SIGCHLD);
    }
    int park = creaConModalita(mode, parkAssist);
    wait((int *)SIGCHLD);
    exit(0);
}
