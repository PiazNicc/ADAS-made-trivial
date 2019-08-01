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
#include "ServerOneConnection.h"

int main()
{
    char message[10];
    strcpy(message, "25");
    int ECUclientD, serverLen, result;
    struct sockaddr_un serverAddr;
    //inizializzo socket
    serverLen = sizeof(serverAddr);
    ECUclientD = socket(AF_UNIX, SOCK_STREAM, 0);
    serverAddr.sun_family = AF_UNIX;
    strcpy(serverAddr.sun_path, "throttle");
    do
    {
        printf("provo a connetermi...\n");

        result = connect(ECUclientD, (struct sockaddr *)&serverAddr, serverLen);
        sleep(1); /* code */
    } while (result == -1);
    printf("scrivendo %d\n", atoi(message));
    send(ECUclientD, message, strlen(message), 0);
    close(ECUclientD);
    exit(0);
}
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

int main()
{
    char message[10];
    strcpy(message, "25");
    int ECUclientD, serverLen, result;
    struct sockaddr_un serverAddr;
    //inizializzo socket
    serverLen = sizeof(serverAddr);
    ECUclientD = socket(AF_UNIX, SOCK_STREAM, 0);
    serverAddr.sun_family = AF_UNIX;
    strcpy(serverAddr.sun_path, "throttle");
    do
    {
        printf("provo a connetermi...\n");

        result = connect(ECUclientD, (struct sockaddr *)&serverAddr, serverLen);

        /* code */
    } while (result == -1);
    printf("scrivendo %d\n", atoi(message));
    send(ECUclientD, message, strlen(message), 0);
    close(ECUclientD);
    return 0;
}
