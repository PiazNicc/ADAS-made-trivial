#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{ //usar e pipe per comunicare con ecuServer?
    char input[11];
    printf("macchina accesa,scrivere INIZIO per mettere in moto\n\n");
    scanf("%s", &input);
    while (strcmp(input, "INIZIO") != 0)
    {
        printf("\ninput non riconosciuto,scrivere INIZIO\n\n");
        scanf("%s", &input);
    }
    if (fork() == 0)
    {
        //ecuserver();
    } else{
    

    printf("\nMacchina in moto,digitare PARCHEGGIO quando si vuole avviare la procedura apposita\n\n");
    scanf("%s", &input);
    while (strcmp(input, "PARCHEGGIO") != 0)
    {
        printf("\ninput non riconosciuto,digitare PARCHEGGIO\n\n");
        scanf("%s", &input);
    }
        
    
    //parking()
    printf("sto parcheggiando\n");
    }
    exit(0);
}
