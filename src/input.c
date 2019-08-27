#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include "ecu.h"
#include <sys/socket.h>
#include "SocketConnection.h"
#define NORMALE 0
#define ARTIFICIALE 1
int ecuD;
void failure(int sig)
{
    wait((int *)SIGCHLD);
    printf("GUASTO ALLA MACCHINA,ARRESTO TOTALE\n");
    exit(-1);
}
void killAll(int sig)
{
    kill(ecuD, SIGIO);
}
void dangerh(int sig)
{
    char *inp = malloc(255);
    do
    {
        printf("macchina fermata,scrivere INIZIO per rimettere in moto\n");
        scanf("%s", inp);
    } while (strcmp(inp, "INIZIO") != 0);
    kill(ecuD, SIGUSR2);
    return;
}
int main(int argc, char *argv[])
{
    int mode;
    if (argv[1] == NULL)
    {
        printf("NESSUNA MODALITÀ SELEZIONATA");
        exit(-1);
    }
    signal(SIGINT, killAll);
    signal(SIGSEGV, killAll);
    signal(SIGUSR1, dangerh);
    signal(SIGUSR2, failure);
    char *input = malloc(255);
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
    scanf("%s", input);
    while (strcmp(input, "INIZIO") != 0)
    {
        printf("\ninput non riconosciuto,scrivere INIZIO\n\n");
        scanf("%s", input);
    }
    ecuD = fork();
    if (ecuD < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (ecuD == 0)
    {
        ecu(mode);
    }
    else
    {
        do
        {
            printf("\ndigitare PARCHEGGIO per fermare la macchina\n\n");
            scanf("%s", input);
        } while (strcmp(input, "PARCHEGGIO") != 0);
        ecuD = connectToServer(".ecu");
        send(ecuD, "PARCHEGGIO\n", 255, 0);
        close(ecuD);
        free(input);
    }
    wait((int *)SIGCHLD);
    exit(0);
}