#include <stdio.h>
#include <fcntl.h>
#include <string.h>

void accelera(int a, FILE *fd)
{
    int len = strlen("aumento 5\n");
    while (a > 0)
    {
        fwrite("aumento 5\n", 1, len, fd);
        sleep(1);
        a -= 5;
    }
}
int main()
{
    FILE *fp = fopen("throttle.log", "w");
    accelera(15, fp);

    return 0;
}