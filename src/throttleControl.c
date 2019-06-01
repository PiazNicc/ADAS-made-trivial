#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "ServerOneConnection.h"
int amount = 0;

void throttleLog()
{
    FILE *fd = fopen("throttle.log", "a");
    char *ma[] = {"AUMENTO 5\n", "NO ACTION\n"};
    int len[] = {strlen("AUMENTO 5\n"), strlen("NO ACTION\n")};
    if (amount == 0)
    {
        if (fwrite(ma[1], 1, len[1], fd) <= 0)
        {
            perror("impossibile scrivere");
        }
    }
    while (amount > 0)
    {
        if (fwrite(ma[0], 1, len[0], fd) <= 0)
        {
            perror("errore in scrittura");
        };
        amount -= 5;
        sleep(1);
    }
    fclose(fd);
}
int main()
{

    signal(SIGALRM, throttleLog);
    char message[10];
    int serverD, ECUclientD, ClientLen, amount;
    struct sockaddr_un ecuAddr;
    //inizializzo socket
    /* serverLen = sizeof(serverAddr);
    unlink("throttle");
    serverD = socket(AF_UNIX, SOCK_STREAM, 0);
    if (serverD < 0)
    {
        perror("problema nella creazione del canale");
        exit(-1);
        
    }

    serverAddr.sun_family = AF_UNIX;
    strcpy(serverAddr.sun_path, "throttle");
    if (bind(serverD, (struct sockaddr *)&serverAddr, serverLen) < 0)
    {
        perror("errore nella connessione");
        exit(-2);
    }*/
    ClientLen = sizeof(ecuAddr);
    printf("in attesa...\n");
    serverD = make_connection("throttle");
    listen(serverD, 5);
    while (1)
    {
        //DA RISCRIVERE CICLO INTERNO PER PERMETTERE TIMEOUT
        alarm(1);
        ECUclientD = accept(serverD, (struct sockaddr *)&ecuAddr, &ClientLen);
        recv(ECUclientD, message, sizeof(message), 0);
        amount = atoi(message);
        close(ECUclientD);
    }

    return 0;
}