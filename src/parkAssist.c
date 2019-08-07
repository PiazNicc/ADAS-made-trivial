#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "SocketConnection.h"
void parkAssist()
{ //dovrebbe essere ok
	FILE *p = fopen("/dev/urandom", "r");
	FILE *log = fopen("assist.log", "w");
	int data, len = sizeof(data), ecuServer;
	unsigned int rng = 0;
	if (p == NULL  || log == NULL)
	{
		perror("errore in apertura file");
		exit(EXIT_FAILURE);

	
	}
	for (int secs = 0; secs < 30; secs++)
	{
//gestione errori da fare
		rng = getc(p);//devo leggere 4 byte quindi uso un int che è delle stesse dimensioni 
		data = rng;
		fprintf(log, "%02x", rng);
		ecuServer = connectToServer("park");
		send(ecuServer, &data, len, 0);
		fprintf(log, "\n");
		sleep(1);
	}
	fclose(log);
	fclose(p);
}
