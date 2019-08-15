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
#include <signal.h>
#include "SocketConnection.h"
int flag = 0;
void flagHandle(int sig)
{
    flag = (flag + 1) % 2;
}
void handler(int sig)
{
    kill(0, SIGINT);
    exit(EXIT_SUCCESS);
};
void checkFailure()
{
    srand((unsigned int)time(0));
    if (rand() < 0.00001 * ((double)RAND_MAX + 1.0))
    {
        kill(getppid(), SIGSTOP);
    }
}
void throttleAction(char *message)
{
    int incAmount;
    incAmount = findAmount(message, 11);
    throttleLog(incAmount);
}
int throttleLog(int a)
{
    FILE *fd = fopen("throttle.log", "a");
    char *ma[] = {" AUMENTO 5\n", " NO ACTION\n"}, buff[20];
    int len[] = {strlen(" AUMENTO 5\n"), strlen(" NO ACTION\n")};

    if (a == 0)
    {

        if (fwrite(ma[1], 1, len[1], fd) <= 0)
        {
            perror("impossibile scrivere");
        }
        sleep(1);
    }
    else
    {
        while (a > 0)
        {
            if (fwrite(ma[0], 1, len[0], fd) <= 0)
            {
                perror("errore in scrittura");
            };
            a -= 5;
            checkFailure();
            fflush(fd);
            sleep(1);
        }
    }
    fclose(fd);
    return a;
}
int main()
{
    char message[15];
    memset(message, 0, sizeof(message));
    FILE *f = fopen("throttle.log", "w");
    fprintf(f, __DATE__);
    fclose(f);
    int serverD, ECUclientD, clientLen, amount;
    signal(SIGINT, handler);
    signal(SIGUSR1, flagHandle);
    struct sockaddr_un ecuAddr;
    clientLen = sizeof(ecuAddr);
    printf("in attesa...\n");
    serverD = serverSocket("throttle");
    listen(serverD, 5);
    int child = fork();
    if (child < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (child == 0)
    { //scrive no action mentre processo padre aspetta richieste
        //il flag serve a sincronizzare i due processi come se fosse un "lock" fra thread
        for (;;)
        {
            if (flag == 0)
            {
                throttleLog(0);
            }
        }
    }
    else
    {
        while (1)
        {

            ECUclientD = accept(serverD, (struct sockaddr *)&ecuAddr, &clientLen);
            kill(child, SIGUSR1);
            if (ECUclientD < 0)
            {
                fprintf(stderr, "impossibile connettersi");
            }
            if (recv(ECUclientD, message, sizeof(message), 0) < 0)
            {
                fprintf(stderr, "impossibile leggere");
            }
            printf("%s\n", message);
            throttleAction(message);
            kill(child, SIGUSR1);
            close(ECUclientD);
        }
    }