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
#include "ServerOneConnection.h"

void decreaseSpeed(int amount)
{
	FILE *f = fopen("brake.log", "a");
	int len = strlen("DECREMENTO 5\n");
	while (amount != 0)
	{
		fwrite("DECREMENTO 5\n", 1, len, f);
		amount = amount - 5;
		sleep(1);
	}
	fclose(f);
}

void log(char *message)
{
	FILE *f = fopen("brake.log", "a");
	int len = strlen(message);
	fwrite(message, 1, len, f);
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
	int serverD, ECU, clientLen;
	struct sockaddr_un ecuAddr;
	//inizializzo socket
	clientLen = sizeof(ecuAddr);
	printf("in attesa...\n");
	serverD = make_connection("throttle");
	listen(serverD, 5);
	while (1)
	{
		//DA RISCRIVERE CICLO INTERNO PER PERMETTERE TIMEOUT
		ECU = accept(serverD, (struct sockaddr *)&ecuAddr, &clientLen);
		recv(ECU, message, sizeof(message), 0);
		brakeAction(message);
		close(ECU);
	}
}
