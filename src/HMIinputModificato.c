#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

int main(int argc,char *argv[]){
    printf("macchina accesa,scrivere INIZIO per mettere in moto\n\n");
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
