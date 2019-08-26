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
#include "azioni.h"
#define READ 0
#define WRITE 1
#define BRAKE 2
int pids[4];
void killCom(int sig)
{
    kill(0, SIGKILL);
    exit(0);
}
void danger(int sig)
{
    kill(pids[BRAKE], SIGUSR2);
    for (int i = 0; i < 4; i++)
    {
        kill(-(pids[i]), SIGKILL);
        wait((int *)SIGCHLD);
        printf("UCCIDO %d\n", pids[i]);
    }

    exit(0);
}

void ecu(int mode)
{
    // genero tutti i processi
    int speed = 0;
    //signal(SIGALRM,danger);
    pids[0] = crea(throttleControl);
    pids[1] = crea(steerByWire);
    pids[2] = crea(brakeByWire);
    pids[3] = crea(frontWindshield); //SALVO BRAKE PER MANDARE SEGNALI DI TERMINAZIONE
    //int pidRadar = creaSensore(mode, forwardFacing);
    /* int fd[2];
    pipe(fd);
    int pidClient = fork();
    if (pidClient < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    } //Server crea un altro figlio -client
    else if (pidClient == 0)
    { //client
        //DICE del processo
        setpgid(getpid(), getpid());
        fcntl(fd[READ], F_SETFL, O_NONBLOCK);
        close(fd[WRITE]);
        ecuClient(fd);
    }
    else
    {
        pids[4] = pidClient;
        /*ecu si mette in ascolto per gli input */
    char command[255];
    struct sockaddr_un client;
    int serverD, clientD;
    signal(SIGUSR1, danger);
    signal(SIGINT, SIG_IGN);
    unsigned int len = sizeof(client);
    // close(fd[READ]);
    serverD = serverSocket(".ecu");
    listen(serverD, 5);
    while (strcmp(command, "PARCHEGGIO") != 0)
    {
        clientD = accept(serverD, (struct sockaddr *)&client, &len);
        // memset(command, '\0', 255);
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
    kill(pids[BRAKE], SIGUSR2);

    for (int i = 0; i < 4; i++)
    {
        kill(-(pids[i]), SIGKILL);
        wait((int *)SIGCHLD);
        printf("UCCIDO %d\n", pids[i]);
    }
    int park = creaConModalita(mode, parkAssist);
    wait((int *)SIGCHLD);
    exit(0);

    //free(command);
}

/*void ecuClient(int pipe[])
{
    int speed = 0;
    char message[255];
    while (1)
    {
        if (read(pipe[READ], message, 255) > 0)
        {
            printf("%s", message);
            speed = atoi(message);
        }
        memset(message, 0, sizeof(message));
        sleep(1);

        
    }
}
*/