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
void killAll(int sig)
{
    kill(ecuD, SIGABRT);
    wait((int *)SIGCHLD);
    exit(-1);
}
void dangerh(int sig)
{
    char *inp = malloc(128);
    do
    {
        printf("\nmacchina fermata,scrivere INIZIO per rimettere in moto\n");
        scanf(" %s", inp);

    } while (strcmp(inp, "INIZIO") != 0);
    kill(ecuD, SIGUSR2);
    printf("Macchina di nuovo in moto,premere INVIO due volte per tornare all'interfaccia di comando\n\n");
    return;
}
int main(int argc, char *argv[])
{
    int mode;
    signal(SIGINT, killAll);
    signal(SIGSEGV, killAll);
    signal(SIGUSR1, dangerh);
    char *input = malloc(128);
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

            printf("\ndigitare PARCHEGGIO per fermare la macchina\n");
            scanf("%s", input);
        } while (strcmp(input, "PARCHEGGIO") != 0);
        ecuD = connectToServer(".ecu");
        send(ecuD, "PARCHEGGIO\n", 255, 0);
        close(ecuD);
        free(input);
        input = NULL;
    }
    wait((int *)SIGCHLD);
    exit(0);
}