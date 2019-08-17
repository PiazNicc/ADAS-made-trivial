#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "SocketConnection.h"
void main()
{ //dovrebbe essere ok
	unsigned char data[4];
	FILE *p = fopen("/dev/urandom", "r");
	FILE *log = fopen("assist.log", "w");
	int len = sizeof(data), ecuServer;
	unsigned int *rng;
	if (p == NULL || log == NULL)
	{
		perror("errore in apertura file");
		exit(EXIT_FAILURE);
	}
	for (int secs = 0; secs < 30; secs++)
	{
		fread(rng, 1, 4, p);
		for (int i = 0; i < 4; i++)
		{
			fprintf(log,"%X ",(int)data[i]);
		
		}
		//ecuServer = connectToServer("ecu");
		//send(ecuServer, &data, len, 0);
		
		fprintf(log,"\n");
		fflush(log);
		//sleep(1);
	}
	fclose(log);
	fclose(p);
}
