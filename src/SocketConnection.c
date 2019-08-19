#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h> /* For AFUNIX sockets */
#include "SocketConnection.h"
#define DEFAULT 0


int serverSocket(char *name)
{
  int socketFd, socketLen;
  struct sockaddr_un sockUNIXAddress; /*Server address */
  struct sockaddr *sockAddrPtr;       /*Ptr to server address*/
  sockAddrPtr = (struct sockaddr *)&sockUNIXAddress;
  socketLen = sizeof(sockUNIXAddress);
  socketFd = socket(AF_UNIX, SOCK_STREAM, DEFAULT);
  sockUNIXAddress.sun_family = AF_UNIX;   /* Set domain type */
  strcpy(sockUNIXAddress.sun_path, name); /* Set name */
  unlink(name);                           /* Remove file if it already exists */
  bind(socketFd, sockAddrPtr, socketLen); /*Create file*/
  return socketFd;
}

int connectToServer(char *serverName)
{
  int clientFd, serverLen, result;
  struct sockaddr_un serverAddr;
  //inizializzo socket
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverLen = sizeof(serverAddr);
  clientFd = socket(AF_UNIX, SOCK_STREAM, 0);
  serverAddr.sun_family = AF_UNIX;
  strcpy(serverAddr.sun_path, serverName);
  do
  {
    result = connect(clientFd, (struct sockaddr *)&serverAddr, serverLen);
    /* Wait and then try again */

  } while (result == -1);
  return clientFd;
}
