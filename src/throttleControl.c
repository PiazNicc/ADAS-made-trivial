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
    struct timeval timeout;
    fd_set set;
    FD_ZERO(&set);
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    //inizializzo socket

    ClientLen = sizeof(ecuAddr);
    printf("in attesa...\n");
    serverD = serverSocket("throttle");
    FD_SET(serverD, &set);
    listen(serverD, 5);
    while (1)
    {
        //DA RISCRIVERE CICLO INTERNO PER PERMETTERE TIMEOUT
        inp = select(FD_SETSIZE, &set, NULL, NULL, &timeout);

        if (inp < 0)
        {
            perror("errore di connessione");
            exit(EXIT_FAILURE);
        }
        else if (inp == 0)
        {
            throttleLog(0);
        }
        else
        {
            ECUclientD = accept(serverD, (struct sockaddr *)&ecuAddr, &ClientLen);
            if(ECUclientD < 0 ){
                fprintf(stderr,"impossibile connettersi");
            }
            if(recv(ECUclientD, message, strlen(message),0) < 0){
                fprintf(stderr,"impossibile leggere");
            }
            amount = atoi(message);
            throttleLog(amount);
            close(ECUclientD);
        }
    }

    return 0;
}