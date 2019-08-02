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

int writeToSocket(int fd, char *line1)
{
  int d = write(fd, line1, strlen(line1) + 1); /* Write first line */
  printf("%s", line1);
  return d;
}

int readLineFromSocket(int fd, char *str)
{
  /* Read a single ’\0’-terminated line into str from fd */
  int n;
  do
  {                       /* Read characters until ’\0’ or end-of-input */
    n = read(fd, str, 1); /* Read one character */
  } while (n > 0 && *str++ != '\0');
  return (n > 0);
} /* Return false if end-of-input */

int serverSocket(unsigned char *name, int mode)
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

int connectToServer(unsigned char *serverName)
{
  int clientFd, serverLen, result;
  struct sockaddr_un serverAddr;
  //inizializzo socket
  serverLen = sizeof(serverAddr);
  clientFd = socket(AF_UNIX, SOCK_STREAM, 0);
  serverAddr.sun_family = AF_UNIX;
  strcpy(serverAddr.sun_path, serverName);
  do
  {
    result = connect(clientFd, (struct sockaddr *)&serverAddr, serverLen);
    if (result == -1)
    {
      printf("impossibile connetersi,nuovo tentativo in 1 secondo\n");
      sleep(1); /* Wait and then try again */
    }
  } while (result == -1);
  return clientFd;
}
