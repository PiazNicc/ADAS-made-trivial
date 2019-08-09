#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include "SocketConnection.h"

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
        sleep(1);
    }
    while (a > 0)
    {
        if (fwrite(ma[0], 1, len[0], fd) <= 0)
        {
            perror("errore in scrittura");
        };
        a -= 5;
    }

    fclose(fd);
}
int main()
{

    char message[10];
    int serverD, ECUclientD, ClientLen, amount, inp;
    struct sockaddr_un ecuAddr;
    ClientLen = sizeof(ecuAddr);
    printf("in attesa...\n");
    serverD = serverSocket("throttle");
    listen(serverD, 5);
    while (1)
    {
        //DA RISCRIVERE CICLO INTERNO PER PERMETTERE TIMEOUT
        ECUclientD = accept(serverD, (struct sockaddr *)&ecuAddr, &ClientLen);
        if (ECUclientD < 0)
        {
            fprintf(stderr, "impossibile connettersi");
        }
        if (recv(ECUclientD, message, strlen(message), 0) < 0)
        {
            fprintf(stderr, "impossibile leggere");
        }
        amount = atoi(message);
        throttleLog(amount);
        close(ECUclientD);
    }

    return 0;
}