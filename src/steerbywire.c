#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "SocketConnection.h"

FILE *openFileOutput()
{
	FILE *steerPunt;
	return steerPunt = fopen("steer.log", "w");
}

void Scrivi(int i, FILE *steerOu)
{


	char *ma[] = {"STO GIRANDO A DESTRA", "STO GIRANDO A SINISTRA", "NO ACTION"};
	int maLenght[] = {20, 22, 9};

	int k;
	if (i != 2)
	{
		for (int j = 0; j < 4; j++)
		{

			k = fwrite(ma[i], 1, maLenght[i], steerOu);
			fwrite("\n", 1, 1, steerOu);
			sleep(1);
		}
	}
	else
	{
		k = fwrite(ma[i], 1, maLenght[i], steerOu);
		fwrite("\n", 1, 1, steerOu);
	}
}

void main()
{
	FILE *p;
	int serverD, ECUclientD, clientLen, amount;
	struct sockaddr_un ecuAddr;
	clientLen = sizeof(ecuAddr);
	printf("in attesa...\n");
	serverD = serverSocket("steer");
	listen(serverD, 5);
	while (1)
	{
		ECUclientD = accept(serverD, (struct sockaddr *)&ecuAddr, clientLen);
		//passa il comando
	}
}
