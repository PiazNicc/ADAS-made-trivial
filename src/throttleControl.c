#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "ServerOneConnection.h"

void throttleLog(int a)
{
    FILE *fd = fopen("throttle.log", "a");
    char *ma[] = {"AUMENTO 5\n", "NO ACTION\n"};
    int len[] = {strlen("AUMENTO 5\n"), strlen("NO ACTION\n")};
    if (a == 0)
    {
        if (fwrite(ma[1], 1, len[1], fd) <= 0)
        {
            perror("impossibile scrivere");
        }
    }
    while (a > 0)
    {
        if (fwrite(ma[0], 1, len[0], fd) <= 0)
        {
            perror("errore in scrittura");
        };
        a -= 5;
    }
    sleep(1);
    fclose(fd);
}
int main()
{

    char message[10];
    int serverD, ECUclientD, ClientLen, amount;
    struct sockaddr_un ecuAddr;
    //inizializzo socket

    ClientLen = sizeof(ecuAddr);
    printf("in attesa...\n");
    serverD = make_connection("throttle");
    //figlio continua  a loggare mentre padre aspetta richieste di accelerazione
    int child = fork();
    if (child < 0)
    {
        perror("errore creazione processo");
    }
    else if (child == 0)
    {
        while (1)
        {
            throttleLog(0);
        }
    }
    else
    {
        listen(serverD, 5);
        while (1)
        {
            //DA RISCRIVERE CICLO INTERNO PER PERMETTERE TIMEOUT
            ECUclientD = accept(serverD, (struct sockaddr *)&ecuAddr, &ClientLen);
            recv(ECUclientD, message, sizeof(message), 0);
            amount = atoi(message);
            throttleLog(amount);
            close(ECUclientD);
        }
    }

    return 0;
}