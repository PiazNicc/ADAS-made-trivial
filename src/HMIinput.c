#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char input[11];
    printf("macchina accesa,scrivere INIZIO per mettere in moto\n\n");
<<<<<<< HEAD
    printf("quando si vuole parcheggiare,scrivere PARCHEGGIO sul terminale.\n");
    int In=0;
    char input[10];
    while(In<2)
{
    scanf("%s",&input);
    for(int i=0;i<10;i++)
	{
	if((int)input[i]>90)
	input[i]=input[i]-32;
	}
    switch (In)
	{     
  case 0:
       
	if(strcmp(input,"INIZIO")==0)
	{
		In++;
		printf("Brum Brum partiti \n");
	}
	break;
       
     case 1:
      
	if(strcmp(input,"PARCHEGGIO")==0)
	{
		In++;
		printf("Aspetta parcheggio \n");
	}
	break;
      }
}
   
}
=======
    scanf("%s", &input);
    while (strcmp(input, "INIZIO") != 0)
    {
        printf("\ninput non riconosciuto,scrivere INIZIO\n\n");
        scanf("%s", &input);
    }
    //init_system();
    printf("\nMacchina in moto,digitare PARCHEGGIO quando si vuole avviare la procedura apposita\n\n");
    scanf("%s", &input);
    while (strcmp(input, "PARCHEGGIO") != 0)
    {
        printf("\ninput non riconosciuto,digitare PARCHEGGIO\n\n");
        scanf("%s", &input);
    }
        
    
    //parking()
    printf("sto parcheggiando\n");
    exit(0);
}
>>>>>>> 7ebb634d97675a14d8e93c95d072398552cb0c4d
