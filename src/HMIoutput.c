#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main()
{
    FILE *f = fopen("ECU.log", "r");
    char buff[128];
    

    if (f == NULL)
    {
        perror("impossibile aprire il file");
        
    }

    while (1) 
    {
        {
            fgets(buff, sizeof(buff), f);
            fputs(buff, stdout);
            sleep(1);
        }
    }
    return 0;
}