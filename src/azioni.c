#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <sys/socket.h>
#include "azioni.h"
#include "SocketConnection.h"
void throttleAction(char *message)
{
    int incAmount;
    incAmount = findAmount(message, 11);
    throttleLog(incAmount);
}

int findAmount(char str[], int pos)
{
    int lengthOfSub = strlen(str) - pos;
    int amount;
    char subStr[lengthOfSub];

    int i = 0;
    while (i < lengthOfSub)
    {
        subStr[i] = str[pos + i];
        i++;
    }

    return amount = atoi(subStr);
}
int checkFailure()
{
    return rand() < 0.00001 * ((double)RAND_MAX + 1.0);
}

void decreaseSpeed(int amount)
{
    FILE *f = fopen("log/brake.log", "a");
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

void brakeAction(char *message)
{
    int decAmount;

    if (strcmp(message, "PARCHEGGIO") == 0)
    {
        brakeLog("ARRESTO AUTO");
    }

    else
    {
        decAmount = findAmount(message, 6);
        decreaseSpeed(decAmount);
    }
}

void steerLog(char message[])
{
    FILE *steerPunt = fopen("log/steer.log", "a");
    char m[] = "DESTRA",m2[] = "SINISTRA";
    if (strcmp(message, m2) == 0 || strcmp(message, m) == 0)
    {
        for (int j = 0; j < 4; j++)
        {
            fprintf(steerPunt, "STO GIRANDO A %s\n", message);
            fflush(steerPunt);
            sleep(1);
        }
    }
    else
    {
        fprintf(steerPunt, "NO ACTION\n");
        fflush(steerPunt);
        sleep(1);
    }
    fclose(steerPunt);
}

int ecuAction(int currSpeed, char *command)
{
    int change = currSpeed, inc = atoi(command), d;
    char m[255];
    char *c;
    if (inc != 0)
    {
        change = inc;

        if (change > currSpeed)
        {
            c = "INCREMENTO ";
            int i = snprintf(m, sizeof(m), "%s%d\n", c, change - currSpeed);
            d = connectToServer(".throttle");
            if (send(d, m, sizeof(m), 0) < 0)
            {
                perror("send");
                exit(EXIT_FAILURE);
            }
            printf(m);
        }
        else if (change < currSpeed)
        {
            c = "FRENO ";
            int i = snprintf(m, sizeof(m), "%s%d\n", c, currSpeed - change);
            d = connectToServer(".brake");
            if (send(d, m, sizeof(m), 0) < 0)
            {
                perror("send");
                exit(EXIT_FAILURE);
            }
            printf(m);
        }
        else
        {
            printf("nothing\n");
        }
    }
    else
    {

        printf("%d\n",strlen(command));
        d = connectToServer(".steer");
        if (send(d, command,strlen(command)-1, 0) < 0)
        {
            perror("send");
            exit(EXIT_FAILURE);
        }
        //printf(command);
    }
    return change;
}