#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "SocketConnection.h"
int flag = 0;
void flagHandle(int sig)
{
	flag = (flag + 1) % 2;
}
void handler(int sig)
{
	kill(0, SIGINT);
	exit(EXIT_SUCCESS);
}
FILE *openFileOutput()
{
	FILE *steerPunt;
	return steerPunt = fopen("steer.log", "w");
}

void steerLog(unsigned char *message)
{
	FILE *steerPunt;

	int k;
	if (strcmp(message, "DESTRA") == 0 || strcmp(message, "SINISTRA") == 0)
	{
		for (int j = 0; j < 4; j++)
		{
			steerPunt = fopen("steer.log", "a");
			fprintf(steerPunt, "STO GIRANDO A %s\n", message);
			fclose(steerPunt);
			sleep(1);
		}
	}
	else
	{
		steerPunt = fopen("steer.log", "a");
		fprintf(steerPunt, "%s\n", message);
		fclose(steerPunt);
		sleep(1);
	}
}

void main()
{
	char message[15];
	memset(message, 0, sizeof(message));
	FILE *f = fopen("steer.log", "w");
	fprintf(f, __DATE__);
	fclose(f);
	int serverD, ECUclientD, clientLen, amount;
	signal(SIGINT, handler);
	signal(SIGUSR1, flagHandle);
	struct sockaddr_un ecuAddr;
	clientLen = sizeof(ecuAddr);
	printf("in attesa...\n");
	serverD = serverSocket("steer");
	listen(serverD, 5);
	int child = fork();
	if (child < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (child == 0)
	{ //scrive no action mentre processo padre aspetta richieste

		for (;;)
		{
			if (flag == 0)
			{
				steerLog("NO ACTION");
			}
		}
	}
	else
	{
		while (1)
		{
			ECUclientD = accept(serverD, (struct sockaddr *)&ecuAddr, &clientLen);
			kill(child, SIGUSR1);
			if (ECUclientD < 0)
			{
				fprintf(stderr, "impossibile connettersi");
			}
			if (recv(ECUclientD, message, sizeof(message), 0) < 0)
			{
				fprintf(stderr, "impossibile leggere");
			}
			steerLog(message);
			kill(child, SIGUSR1);
			close(ECUclientD);
		}
	}
}
