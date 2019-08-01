#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h> /* For AFUNIX sockets */
#include "SocketConnection.h"
#define CLIENT 0
#define SERVER 1

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

int make_socket(char *name, int mode)
{
  int socketFd, socketLen;
  struct sockaddr_un sockUNIXAddress; /*Server address */
  struct sockaddr *sockAddrPtr;       /*Ptr to server address*/
  sockAddrPtr = (struct sockaddr *)&sockUNIXAddress;
  socketLen = sizeof(sockUNIXAddress);
  socketFd = socket(AF_UNIX, SOCK_STREAM, 0);
  sockUNIXAddress.sun_family = AF_UNIX;   /* Set domain type */
  strcpy(sockUNIXAddress.sun_path, name); /* Set name */
  if (mode == SERVER)
  {
    unlink(name);                           /* Remove file if it already exists */
    bind(socketFd, sockAddrPtr, socketLen); /*Create file*/
  }
  else if (mode != CLIENT)
  {
    perror("errore");
    exit(EXIT_FAILURE);
  }

  return socketFd;
}
