#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include "ecu.h"
#include <sys/socket.h>
#include "SocketConnection.h"
#define NORMALE 0
#define ARTIFICIALE 1
void killAll(int sig)
{
    kill(0, SIGKILL);
    exit(0);
}
int main(int argc, char *argv[])
{ //usar e pipe per comunicare con ecuServer?
    int mode;
    if (argv[1] == NULL)
    {
        printf("NESSUNA MODALITÀ SELEZIONATA");
        exit(-1);
    }
    signal(SIGINT, killAll);
    signal(SIGSEGV, killAll);
    char input[15];
    int ecuD;
    if (strcmp(argv[1], "NORMALE") == 0)
    {
        mode = NORMALE;
    }
    else if (strcmp(argv[1], "ARTIFICIALE") == 0)
    {
        mode = ARTIFICIALE;
    }
    else
    {
        printf("MODALITÀ SELEZIONATA NON RICONOSCIUTA:SCEGLIERE TRA NORMALE E ARTIFICIALE\n");
        exit(-1);
    }
    printf("macchina accesa,scrivere INIZIO per mettere in moto\n\n");
    scanf("%s", &input);
    while (strcmp(input, "INIZIO") != 0)
    {
        printf("\ninput non riconosciuto,scrivere INIZIO\n\n");
        scanf("%s", &input);
    }
    if (fork() == 0)
    {
        ecu(mode);
    }
    else
    {
        printf("\nMacchina in moto,digitare PARCHEGGIO quando si vuole avviare la procedura apposita\n\n");
        scanf("%s", &input);
        while (strcmp(input, "PARCHEGGIO") != 0)
        {
            printf("\ninput non riconosciuto,digitare PARCHEGGIO per fermare la macchina\n\n");
            scanf("%s", &input);
        }
        ecuD = connectToServer(".ecu");
        send(ecuD, input, strlen(input), 0);
        close(ecuD);
        printf("sto parcheggiando\n");
    }
    exit(0);
}
