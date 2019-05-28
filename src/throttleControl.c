#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void throttleLog(int a, FILE *fd)
{
    int len = strlen("aumento 5\n");
    while (a > 0)
    {
        if (fwrite("aumento 5\n", 1, len, fd) <= 0)
        {
            perror("errore in scrittura");
        };
        a -= 5;
    }
}
int main()
{
    FILE *fp = fopen("throttle.log", "a");
    char message[10];
    message[10] = '\0';
    int serverD, ECUclientD, serverLen, ClientLen, amount, nextP;
    struct sockaddr_un serverAddr, ecuAddr;
    //inizializzo socket
    serverLen = sizeof(serverAddr);
    ClientLen = sizeof(ecuAddr);
    unlink("throttle");
    serverD = socket(AF_UNIX, SOCK_STREAM, 0);
    if (serverD < 0)
    {
        perror("problema nella creazione del canale");
        exit(-1);
        /* code */
    }

    serverAddr.sun_family = AF_UNIX;
    strcpy(serverAddr.sun_path, "throttle");
    if (bind(serverD, (struct sockaddr *)&serverAddr, serverLen) < 0)
    {
        perror("errore nella connessione");
        exit(-2);
    }
    printf("in attesa...\n");
    listen(serverD, 5);
    while (1)
    {
        fp = fopen("throttle.log", "a");
        ECUclientD = accept(serverD, (struct sockaddr *)&ecuAddr, &ClientLen);
        recv(ECUclientD, message, sizeof(message), 0);
        amount = atoi(message);
        throttleLog(amount, fp);

        fclose(fp);

        close(ECUclientD);
    }

    return 0;
}