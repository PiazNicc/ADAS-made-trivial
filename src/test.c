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
{ int ECUclientD;
    char message[15];
    memset(message,0,sizeof(message));
    strcpy(message, "DESTRA");
    ECUclientD = connectToServer("steer");
    send(ECUclientD, message, sizeof(message), 0);
    close(ECUclientD);
    sleep(15);
    strcpy(message, "SINISTRA");
    ECUclientD = connectToServer("steer");
    send(ECUclientD, message, sizeof(message), 0);
    close(ECUclientD);
    return 0;
}
