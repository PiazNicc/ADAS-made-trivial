#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
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
        fflush(fd);
        sleep(1);
    }
    else
    {
        while (a > 0)
        {
            if (fwrite(ma[0], 1, len[0], fd) <= 0)
            {
                perror("errore in scrittura");
            }
            fflush(fd);
            if (rand() < 0.00001 * ((double)RAND_MAX + 1.0))
            {
                kill(getppid(), SIGIO);
                break;
            }
            a -= 5;
            sleep(1);
        }
    }
    fclose(fd);
    return a;
}
void brakeLog(char *message)
{
    FILE *f = fopen("log/brake.log", "a");
    fprintf(f, "%s", message);
    fclose(f);
    sleep(1);
}
void steerLog(char *message)
{
    FILE *steerPunt = fopen("log/steer.log", "a");
    char m[] = "DESTRA\n", m2[] = "SINISTRA\n";
    if (strcmp(message, m2) == 0 || strcmp(message, m) == 0)
    {
        for (int j = 0; j < 4; j++)
        {
            fprintf(steerPunt, "STO GIRANDO A %s", message);
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

void ecuLog(char *message)
{
    FILE *f = fopen("log/ecu.log", "a");
    fprintf(f, "%s", message);
    fclose(f);
    sleep(1);
}