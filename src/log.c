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
    //flock(fd, LOCK_EX);
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
            /*if (rand() < 0.00001 * ((double)RAND_MAX + 1.0))
            {
                kill(getppid(), SIGSTOP);
                break;
            }*/
            a -= 5;
            fflush(fd);
            sleep(1);
        }
    }
    //flock(fd,LOCK_UN);
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
