#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/un.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include "SocketConnection.h"

int ecuServer()
{
    /* versione base che gestisce solo un sensore(front windshield),per i sensori
    facoltativi andrebbe implementata la gestione in contemporanea di 
    piu client
    */
    char *command;
    int speed = 0, serverD, clientD;
    struct sockaddr_un client;
    //genera processi attuatori
    serverD = serverSocket("ecu");
    listen(serverD, 5);
    while (1)
    {
        clientD = accept(serverD, (struct sockaddr *)&client, sizeof(client));
        if (clientD < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
            /* code */
        }
        if (recv(clientD, command, strlen(command), 0) < 0)
        {
            perror("receive");
            exit(EXIT_FAILURE);
        }
        //INVIA COMANDO A ECUCLIENT(pipe?)
        //logga comando in ecu.log(oppure lo fa ecuClient?)
    }
    return 0;
}