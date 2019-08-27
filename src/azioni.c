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
#include "log.h"
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
    decAmount = findAmount(message, 6);
    decreaseSpeed(decAmount);
}

void steer(char *message)
{
    steerLog(message);
}

int ecuAction(int currSpeed, char *command)
{
    FILE *log = fopen("log/ecu.log", "a");
    int change = currSpeed, inc = atoi(command), d;
    char m[255];
    char *c;
    if (inc != 0)
    {
        change = inc;

        if (change > currSpeed)
        {
            c = "INCREMENTO ";
            snprintf(m, sizeof(m), "%s%d\n", c, change - currSpeed);
            d = connectToServer(".throttle");
            if (send(d, m, sizeof(m), 0) < 0)
            {
                perror("send");
                exit(EXIT_FAILURE);
            }
        }
        else if (change < currSpeed)
        {
            c = "FRENO ";
            snprintf(m, sizeof(m), "%s%d\n", c, currSpeed - change);
            d = connectToServer(".brake");
            if (send(d, m, sizeof(m), 0) < 0)
            {
                perror("send");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            snprintf(m, sizeof(m), "NO ACTION\n");
        }
        fputs(m, log);
        fflush(log);
    }
    else
    {
        d = connectToServer(".steer");
        if (send(d, command, strlen(command), 0) < 0)
        {
            perror("send");
            exit(EXIT_FAILURE);
        }
        fputs(command, log);
        fflush(log);
    }
    fclose(log);
    return change;
}