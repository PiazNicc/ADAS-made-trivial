#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>
#include <signal.h>
#include <sys/un.h>
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
    int change = currSpeed, inc = isNumber(command), d;
    char m[255];
    char *c;
    if (inc)
    {
        change = atoi(command);

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
            snprintf(m, sizeof(m), "LA MACCHINA PROCEDE ALLA VELOCITÀ DI %d km/h \n", currSpeed);
        }
        ecuLog(m);
        // fputs(m, log);
        //fflush(log);
    }
    else
    {
        if (currSpeed > 0) //STERZATA NON HA SENSO SE MACCHINA FERMA
        {
            d = connectToServer(".steer");
            if (send(d, command, strlen(command), 0) < 0)
            {
                perror("send");
                exit(EXIT_FAILURE);
            }
            ecuLog(command);
            //fputs(command, log);
            // fflush(log);
        }
    }
    fclose(log);
    return change;
}
int isNumber(char m[])
{
    int isN = 1;
    for (int i = 0; i < strlen(m) - 1; i++)
    {
        if (!isdigit(m[i]))
        {
            isN = 0;
            break;
        }
    }
    return isN;
}

int checkParking(unsigned char data[])
{
    unsigned char conf[] = {0x172A, 0xD693, 0x0, 0xBDD8, 0xFAEE, 0x4300};
    for (int i = 0; i < strlen((char *)data) - 1; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (data[i] == conf[j])
            {
                return 0;
            }
        }
    }
    return 1;
}
