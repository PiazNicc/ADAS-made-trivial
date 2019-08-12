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

void decreaseSpeed(int amount)
{
	FILE *f = fopen("brake.log", "a");
	int len = strlen("DECREMENTO 5\n");
	while (amount != 0)
	{

		fwrite("DECREMENTO 5\n", 1, len, f);
		amount = amount - 5;
		fflush(f);

		sleep(1);
	}
	fclose(f);
}

void log(char *message)
{
	FILE *f = fopen("brake.log", "a");
	int len = strlen(message);
	fprintf(f, "%s\n",message);
	fclose(f);
	sleep(1);
}

int findSubString(char str[], int pos)
{
	int lengthOfSub = sizeof(str) - pos;
	int amount = 0;
	char subStr[lengthOfSub];

	int i = 0;
	while (i < lengthOfSub)
	{
		subStr[i] = str[pos + i];
		i++;
	}
	return amount = atoi(subStr);
}
void brakeAction(char *message)
{
	int decAmount;

	if (strcmp(message, "ARRESTO") == 0 || strcmp(message, "NO ACTION") == 0)
	{
		log(message);
	}

	else
	{
		decAmount = findSubString(message, 6);
		decreaseSpeed(decAmount);
	}
}

int main(int argc, char *argv[])
{
	char message[10];
	FILE *f = fopen("brake.log", "w");
	fprintf(f, "\n");
	fclose(f);
	int serverD, ECUclientD, clientLen, amount;
	signal(SIGINT, handler);
	signal(SIGUSR1, flagHandle);
	struct sockaddr_un ecuAddr;
	clientLen = sizeof(ecuAddr);
	printf("in attesa...\n");
	serverD = serverSocket("brake");
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
				brakeAction("NO ACTION");
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
			brakeAction(message);
			kill(child, SIGUSR1);
			close(ECUclientD);
		}
	}

	return 0;
}
