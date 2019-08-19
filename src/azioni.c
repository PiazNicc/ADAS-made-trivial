#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include "azioni.h"
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
int throttleLog(int a)
{
    srand((unsigned int)time(0));
    FILE *fd = fopen("log/throttle.log", "a");
    char *ma[] = {" AUMENTO 5\n", " NO ACTION\n"};
    int len[] = {strlen(" AUMENTO 5\n"), strlen(" NO ACTION\n")};

    if (a == 0)
    {

        if (fwrite(ma[1], 1, len[1], fd) <= 0)
        {
            perror("impossibile scrivere");
        }
        sleep(1);
    }
    else
    {
        while (a > 0)
        {
            if (fwrite(ma[0], 1, len[0], fd) <= 0)
            {
                perror("errore in scrittura");
            };
            if (checkFailure() == 1)
            {
                kill(getppid(), SIGSTOP);
                break;
            }
            a -= 5;
            fflush(fd);
            sleep(1);
        }
    }
    fclose(fd);
    return a;
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

void brakeLog(char *message)
{
    FILE *f = fopen("log/brake.log", "a");
    fprintf(f, "%s\n", message);
    fclose(f);
    sleep(1);
}
void steerLog(char *message)
{
    FILE *steerPunt = fopen("log/steer.log", "a");

    if (strcmp(message, "DESTRA") == 0 || strcmp(message, "SINISTRA") == 0)
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
        fclose(steerPunt);
        sleep(1);
    }
}