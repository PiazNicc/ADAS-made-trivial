#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include<unistd.h>

int main()
{
    FILE *f = fopen("ECU.log", "r");
    char value[5];
    value[4] = '\0';
    if (f == NULL)
    {
        perror("impossibile aprire il file");
    }

    while (1 == 1)
    {
        {
            fread(value, 1, 4, f);
            printf("%s", value);
            sleep(1);
            
        }
    }
    return 0;
}