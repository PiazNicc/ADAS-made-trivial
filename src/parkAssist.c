#include <stdio.h>-/
#include <errno.h>

int main(int argc, char *argv[])
{
    FILE *p = fopen("/dev/urandom", "r");
    FILE *log = fopen("assist.log", "w");
    char data[4];
    while (1)
    {

        if (p == NULL || log == NULL)
        {
            perror("errore in apertura file");
            exit(-1);
            /* code */
        }
        else
        {
           int n = fread(data, 1, 4, p);

            fwrite(data, 1, n, log);
            sleep(1);

            /* code */
        }
    }
    exit(0);
}