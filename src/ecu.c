#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/un.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "creazione.h"
#include "SocketConnection.h"
#include "ecu.h"
#include "attuatori.h"
#include "sensori.h"
#include "azioni.h"
#define READ 0
#define WRITE 1
void killCom(int sig)
{
    kill(0, SIGKILL);
    exit(0);
}

void ecu(int mode)
{
    int pidArray[5];
    int i = 0;
    int pidWindshield = creaAttuatore(frontWindshield);
    pidArray[i] = pidWindshield;
    i++;
    int pidSteer = creaAttuatore(steerByWire);
    pidArray[i] = pidSteer;
    i++;
    int pidBrake = creaAttuatore(brakeByWire);
    pidArray[i] = pidBrake;
    i++;
    int pidThrottle = creaAttuatore(throttleControl);
    pidArray[i] = pidThrottle;
    i++;
    /*int pidRadar = creaSensore(mode, forwardFacing);
    pidArray[i] = pidRadar;
    i++;
    */
    int fd[2];
    pipe(fd);
    int pidClient = fork();
    if (pidClient < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    } //Server crea un altro figlio -client
    else if (pidClient == 0)
    { //client
        /*CODICE del processo*/
        close(fd[WRITE]);
        ecuClient(fd);
    }
    else
    {
        pidArray[i] = pidClient;
        char command[128];
        struct sockaddr_un client;
        int serverD, clientD;
        unsigned int len = sizeof(client);
        signal(SIGKILL, killCom);
        pidArray[i] = pidClient;
        close(fd[READ]);
        serverD = serverSocket(".ecu");
        listen(serverD, 5);
        while (strcmp(command, "PARCHEGGIO") != 0)
        {
            clientD = accept(serverD, (struct sockaddr *)&client, &len);

            if (clientD < 0)
            {
                fprintf(stderr, "impossibile connettersi");
            }
            if (recv(clientD, command, sizeof(command), 0) < 0)
            {
                fprintf(stderr, "impossibile leggere");
            }
            write(fd[WRITE], command, strlen(command));
            close(clientD);
        }
        kill(0, SIGKILL);
        int park = creaSensore(mode, parkAssist);
    }
}

void ecuClient(int pipe[])
{
    int speed = 0;
    char message[127];
    memset(message, 0, sizeof(message));
    while (1)
    {
        if (read(pipe[READ], message, strlen(message)+1) > 0)
        {
            printf("%s",message);
            speed = atoi(message);
        }
        sleep(1);

        /* code */
    }
}
