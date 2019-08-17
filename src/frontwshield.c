#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include "SocketConnection.h"
#define _GNU_SOURCE

void main()

{
	FILE *wscIN = fopen("input/frontCamera.data", "r");
	FILE *wscOUT = fopen("log/camera.log", "a");
	char data[128];
	memset(data,0,sizeof(data));
	int ecu;

	if (wscIN == NULL)
	{
		perror("error not open fileINPUT");
		exit(EXIT_FAILURE);
	}
	if (wscOUT == NULL)
	{
		perror("error not open fileOUTPUT");
		exit(EXIT_FAILURE);
	}
	while ( fgets(data, sizeof(data), wscIN) != NULL)
	{ 
		ecu = connectToServer("ecu");
		printf("%s",data);
		send(ecu,data,sizeof(data),0);
		sleep(10);
	}

	fclose(wscOUT);
	fclose(wscIN);
}
