#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h> /* For AFUNIX sockets */
#include"ServerOneConnection.h"
#define DEFAULT_PROTOCOL 0

int wServer (int fd, char *line1) {
	int d=write (fd, line1, strlen (line1) + 1); /* Write first line */
	printf("%s",line1);
	return d;
}

int rServer (int fd, char *str) {
/* Read a single ’\0’-terminated line into str from fd */
 int n;
 do { /* Read characters until ’\0’ or end-of-input */
  n = read (fd, str, 1); /* Read one character */
 } while (n > 0 && *str++ != '\0');
 return (n > 0); } /* Return false if end-of-input */

int make_connection (char *name) {
  int serverFd, serverLen;
  struct sockaddr_un serverUNIXAddress; /*Server address */
  struct sockaddr* serverSockAddrPtr; /*Ptr to server address*/
  serverSockAddrPtr = (struct sockaddr*) &serverUNIXAddress;
  serverLen = sizeof (serverUNIXAddress);
  serverFd = socket (AF_UNIX, SOCK_STREAM, DEFAULT_PROTOCOL);
  serverUNIXAddress.sun_family = AF_UNIX; /* Set domain type */
  strcpy (serverUNIXAddress.sun_path, name); /* Set name */
  unlink (name); /* Remove file if it already exists */
  bind (serverFd, serverSockAddrPtr, serverLen);/*Create file*/
  listen (serverFd, 1); /* Maximum pending connection length */
  return serverFd;
}

int make_socket(int serverFd)
{
	int i=0, clientFd=-1, clientLen;
	struct sockaddr_un clientUNIXAddress; /*Client address */
	struct sockaddr* clientSockAddrPtr;/*Ptr to client address*/
	clientSockAddrPtr = (struct sockaddr*) &clientUNIXAddress;
	clientLen = sizeof (clientUNIXAddress);
	while(clientFd==-1)
	clientFd = accept (serverFd, clientSockAddrPtr, &clientLen);
	return clientFd;
}
void close_SS(int fd)
{
	close(fd);
}









