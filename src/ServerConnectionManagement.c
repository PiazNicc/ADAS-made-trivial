#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h> /* For AFUNIX sockets */
#define DEFAULT_PROTOCOL 0
#define NUMBER_OF_CONNECTION 5
struct connection
{
	int clientFD;//client descriptor
	char *name;//name of the client
}data[10];

int writeRecipe (int fd,char *line1) {
  write (fd, line1, strlen (line1) + 1); /* Write second line */
}
int make_connection () {
  int serverFd, serverLen;
  struct sockaddr_un serverUNIXAddress; /*Server address */
  struct sockaddr* serverSockAddrPtr; /*Ptr to server address*/
 

  serverSockAddrPtr = (struct sockaddr*) &serverUNIXAddress;
  serverLen = sizeof (serverUNIXAddress);
 
  
  serverFd = socket (AF_UNIX, SOCK_STREAM, DEFAULT_PROTOCOL);
  serverUNIXAddress.sun_family = AF_UNIX; /* Set domain type */
  strcpy (serverUNIXAddress.sun_path, "server"); /* Set name */
  unlink ("server"); /* Remove file if it already exists */
  bind (serverFd, serverSockAddrPtr, serverLen);/*Create file*/
  listen (serverFd, NUMBER_OF_CONNECTION); /* Maximum pending connection length */
  return serverFd;
}



int readLine (int fd, char *str) {
/* Read a single ’\0’-terminated line into str from fd */
 int n;
 do { /* Read characters until ’\0’ or end-of-input */
  n = read (fd, str, 1); /* Read one character */
 } while (n > 0 && *str++ != '\0');
 return (n > 0); } /* Return false if end-of-input */




int make_socket(struct connection *fd[],int serverFd)
{
	int i=0, clientFd, clientLen;
	struct sockaddr_un clientUNIXAddress; /*Client address */
	struct sockaddr* clientSockAddrPtr;/*Ptr to client address*/
	clientSockAddrPtr = (struct sockaddr*) &clientUNIXAddress;
	clientLen = sizeof (clientUNIXAddress);
	while (1) {/* Loop forever */ /* Accept a client connection */
	clientFd = accept (serverFd, clientSockAddrPtr, &clientLen);
	fd[i]->clientFD=clientFd;
   		if(fork()==0)
   		{
			int d=0;
			while(d==0)
				d=readLine(clientFd,fd[i]->name);
			exit(0);
		}
	}
}

void manage_fd()
{
	if(fork()==0)
	{
	int serverFD=make_connection();
	make_socket(&data,serverFD);
	}
}

int waitClientFd(char *name)
{
	int fd=-1;
	for(int i=0;i<10;i++)
	{
		if(strcmp(data[i].name,name));
		{
			fd=data[i].clientFD;
		}
	}
	return fd;
}

			

	
		



